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


// Dada una lista de puertos de vecinos, conecta el cliente con cada vecino
// agregando cada socket al vector de sockets
void con2neigh(string list, vector<int>& sockets)
{
    // TO DO
}

// Dado el estado actual de la celula y el estado de los vecinos en una ronda
// computa el nuevo estado de la celula segun las reglas del juego
bool set_state(bool alive, const vector<request>& cl)
{
	// TO DO
    return true;
}

int run_cell(int port)
{   
    char                buf[MENSAJE_MAXIMO+1];
    struct request      srv_req;
    int                 srv_socket, accepting_socket;
    // Definir estructuras para manejar los sockets
    // Sugerancia: Diferenciar los canales donde la celula publica su estado
    //             de los que usa para recibir estado de sus vecinos
  
    /* Conectarse al server */
    srv_socket = connect_socket(htons(port));

    /* Crear socket de escucha y permitir aceptar conexiones concurrentemente */

    int lsn_port = 0/* TO DO*/ ;
    int acc_sock_fd = 0;/* TO DO*/
    /* TO DO*/
  
    /* Enviar msg al srv con el puerto de escucha */
    /* TO DO*/
    
    /* Obtener lista de vecinos inicial */
    /* TO DO*/

    /* Conectarse a los vecinos */
    /* TO DO*/ 

    /* Enviar msg ready para el server */
    /* TO DO*/

    /* Comenzar juego */
    srand(getpid());
    char alive = random() % 2;
    while(1)
    {
        // Esperar request del srv
        get_request(&srv_req, srv_socket);
        if (strncmp(srv_req.type,"TICK",4) == 0)
        {
            /* Publicar estado*/
            /* TO DO*/

            /* Obtener estado de los vecinos*/
            /* TO DO*/

            /* Computar nuevo estado*/
            /* TO DO*/

            /* Informar al srv nuevo estado*/
            /* TO DO*/
        }
        else if (strncmp(srv_req.type,"NEW",3) == 0)
        {
            /* Conectarse a los nuevos vecinos*/
            /* TO DO*/

            /* Avisar con CLIENT_READY al srv*/
            /* TO DO*/

        } 

    }
    
    return 0;
}

void client_accept_conns(int s, vector<int> &listListen)
{
    struct sockaddr_in remote;
    int t = sizeof(remote);
    
    while(1)
    {
        int socketNuevo = accept(s, (struct sockaddr *) &remote, (socklen_t *) &t);
        /* Aceptar nueva celula*/
        if(socketNuevo == -1){
            perror("Error aceptando");
            exit(1);
        }
        cout<< "Acepté a: ";
        cout << socketNuevo << endl;
        listListen.push_back(socketNuevo);
    }
}

void client_connects(vector<int> Portslist, vector<int> &sVecinos){
    for(int i = 0; i < Portslist.size(); i++){
        sVecinos.push_back(connect_socket(Portslist[i]));
    }
}

int main(int argc, char* argv[]){
    int socket_fd;
    struct sockaddr_in  remote;
    struct sockaddr_in local;
    struct in_addr addr;
	vector<thread> threads;
    vector<int> Portslist;
    vector<int> sVecinos;
    vector<int> lVecinos;
    int s;
    int s_listen;
    
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

    /* configura dirección */
    srand(time(0));
    int puerto_Socket = 1025 + (rand()%128);
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
    string puerto = to_string(puerto_Socket);
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
            cout << "Los puertos son:";
            for(int i = 0; i < Portslist.size(); i++){
                cout << Portslist[i];
                cout << " ";
            }
            cout << "" << endl;
            threads.push_back(thread(client_connects, Portslist, ref(sVecinos)));
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