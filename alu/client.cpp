#include "header.h"
using namespace std;

// Asumiendo servidor en ip local, y dado un puerto, establece una conexion
// con el destino retornando un socket en estado conectado en caso de exito
int connect_socket(int puerto)
{
    struct sockaddr_in remote;
    int socket_nuevo;
    int s;

    if ((socket_nuevo = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creando socket");
        exit(1);
    }

    /* Establecer la dirección a la cual conectarse. */
    remote.sin_family = AF_INET;
    remote.sin_port = htons(puerto);
    remote.sin_addr.s_addr = INADDR_ANY;
    inet_pton(AF_INET, "127.0.0.1", &(remote.sin_addr));

    /* Conectarse. */
    s = connect(socket_nuevo, (struct sockaddr *)&remote, sizeof(remote));
    if (s == -1) {
        perror("conectandose entre clientes");
        exit(1);
    }
    return socket_nuevo;
}

int powInt(int x, int y)
{
    for (int i = 0; i < y; i++){
        x *= 10;
    }
    return x;
}

int parseInt(char* chars)
{
    int sum = 0;
    int len = strlen(chars);
    for (int x = 0; x < len; x++){
        int n = chars[len - (x + 1)] - '0';
        sum = sum + powInt(n, x);
    }
    return sum;
}

void calcularEstadito(vector<int> estadosVecinos, int &estado)
{
    int vivos;
    
    for (int i= 0; i < estadosVecinos.size(); i++){
        if (estadosVecinos[i] == 1){
            vivos++;
        }
    }
    if (estado == 0 && vivos == 3){
        estado = 1;
    }
    else if(estado == 1 && (vivos < 2 || vivos > 3)){
        estado = 0;
    }
}


void client_accept_conns(int s, vector<int> &listListen)
{
    struct sockaddr_in remote;
    int t = sizeof(remote);
    
    while(1)
    {
        /* Aceptar nueva celula*/
        int socketNuevo = accept(s, (struct sockaddr *) &remote, (socklen_t *) &t);
        if(socketNuevo == -1){
            perror("Error aceptando");
            exit(1);
        }
        listListen.push_back(socketNuevo);
    }
}

void client_connects(vector<int> Portslist, vector<int> &sVecinos, vector<int> &newPortlist, bool &primeraVez){
    if (primeraVez){
        for(int i = 0; i < Portslist.size(); i++){
            sVecinos.push_back(connect_socket(Portslist[i]));
        }
        newPortlist = Portslist;
        primeraVez = false;
    }
    else{
        vector<int> helper;
        int z = 0;
        //Repasa la lista de puertos que ya tenia y lo compara con la nueva,
        //agregando solo los puertos nuevos.
        for(int i = 0; i < (Portslist.size()); i++){
            for(int j = 0; j < newPortlist.size(); j++){
                if(Portslist[i] == newPortlist[j]){
                    z+=1;
                }
            }
            if (z < 1){
                helper.push_back(Portslist[i]);
            }
            else{
                z=0;
            }
        }
        if(helper.size()!=0){
            for(int i = 0; i < helper.size(); i++){
                newPortlist.push_back(helper[i]);
                sVecinos.push_back(connect_socket(helper[i]));
            }
        }
    }
}

int main(int argc, char* argv[]){
    int socket_fd;
    struct sockaddr_in  remote;
    struct sockaddr_in local;
	vector<thread> threads;
    vector<int> Portslist;
    vector<int> sVecinos;
    vector<int> lVecinos;
    vector<int> newPortlist;
    bool primeraVez = true;
    int s;
    int s_listen;
    int estado;

    /* configura dirección */
    int puerto_Socket = atoi(argv[1]);
    
    /* crea socket */
    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creando socket");
        exit(1);
    }

    /* Establecer la dirección a la cual conectarse. */
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &(remote.sin_addr));

    /* Conectarse. */
    if ((s = connect(socket_fd, (struct sockaddr *)&remote, sizeof(remote))) == -1) {
        perror("conectandose");
        exit(1);
    }

    /* crea socket */
    if ((s_listen = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    local.sin_family = AF_INET;
    local.sin_port = htons(puerto_Socket);
    local.sin_addr.s_addr = INADDR_ANY;


    /* linkea socket con dirección */
    if (bind(s_listen, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        exit(1);
    }

    /* setea socket a modo "listen"*/
    if (listen(s_listen, 10) == -1) {
        perror("listen");
        exit(1);
    }

    threads.push_back(thread(client_accept_conns, s_listen, ref(lVecinos)));

    struct request req;
    vector<int> estadosVecinos;
    string puerto = to_string(puerto_Socket);
    int contador = 0;
    strncpy(req.type, "PORT", 6);
    strncpy(req.msg, puerto.c_str(), sizeof(puerto.c_str()));
    send_request(&req, socket_fd);
    cout << req.msg << endl;
    while(1) {
        struct request roq;
        
        get_request(&roq, socket_fd);
        
        if (strncmp(roq.type, "VECINOS", 8) == 0){
            string msg = roq.msg;
            string var;
            for (auto x : msg){
                if (x == ' '){
                    Portslist.push_back(stoi(var));
                    var = "";
                }
                else {
                    var+=x;
                }
            }

            threads.push_back(thread(client_connects, Portslist, ref(sVecinos),ref(newPortlist), ref(primeraVez)));
		}

        /* El server les manda a todos los clientes que se seteen un estado aleatorio*/
        else if(strncmp(roq.type, "SETEATE", 8) == 0){
            srand(getpid());
            estado = rand() % 2;
            struct request ruq;
            strncpy(ruq.type, "ESTADO", 7);
            strncpy(ruq.msg, to_string(estado).c_str(), MENSAJE_MAXIMO);
            send_request(&ruq, socket_fd);
        }
        /* El server les manda el tick y se fija en que estados estan sus vencinos*/
        else if(strncmp(roq.type, "TICK", 5) == 0){
        
            
            estadosVecinos.clear();
            
            for(int o = 0; o < lVecinos.size(); o++){
                struct request ruq;
                strncpy(ruq.type, "ESTADOVECINO", 13);
                strncpy(ruq.msg, to_string(estado).c_str(), MENSAJE_MAXIMO);
            
                send_request(&ruq, lVecinos[o]);
            }
            
            for(int i = 0; i < sVecinos.size(); i++){
                struct request req;
                get_request(&req, sVecinos[i]);
                
                int help = parseInt(req.msg);
                estadosVecinos.push_back(help);
            }
            
            calcularEstadito(estadosVecinos, ref(estado));
            
            struct request ruq;
            strncpy(ruq.type, "ESTADO", 6);
            strncpy(ruq.msg, to_string(estado).c_str(), MENSAJE_MAXIMO);
            send_request(&ruq, socket_fd);
            
        }
        
        
    }
    
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}

    close(s_listen);
    close(socket_fd);
    return 0;    
}