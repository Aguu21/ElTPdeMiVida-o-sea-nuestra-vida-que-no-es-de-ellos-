#include "header.h"
using namespace std;

// Sugerencia: El servidor va a necestiar varios threads. Recordar que peuden
// compartir variables y que por lo tanto las herramientas de sincronziacion
// como semaforos son perfectamente validas.
int po = 0;
void conectar_Vecinos(vector<vector<vector<int>>> matriz, int matriz_size){
    vector<int> vecinos;
    string vecinos_str;
    int help = 2;
    for (int y = 0; y < matriz_size ; y++)
    {
        for (int x = 0; x < matriz_size; x++)
        {
            if (y==0 && x==0){
                vecinos.push_back(matriz[0][1][help]);
                vecinos.push_back(matriz[0][1][help]);
                vecinos.push_back(matriz[0][1][help]);
            }
            else if((y==0 && x==matriz_size)){
                vecinos.push_back(matriz[0][matriz_size-2][help]);
                vecinos.push_back(matriz[1][matriz_size-1][help]);
                vecinos.push_back(matriz[1][matriz_size-2][help]);                
            }
            else if((y==matriz_size && x==0)){
                vecinos.push_back(matriz[matriz_size-2][0][help]);
                vecinos.push_back(matriz[matriz_size-2][1][help]);
                vecinos.push_back(matriz[matriz_size-1][1][help]);
            }
            else if((y==matriz_size && x==matriz_size)){
                vecinos.push_back(matriz[matriz_size-1][matriz_size-2][help]);
                vecinos.push_back(matriz[matriz_size-2][matriz_size-2][help]);
                vecinos.push_back(matriz[matriz_size-2][matriz_size-1][help]);
            }
            else if((y==0 && x!=0 && x!=matriz_size-1)){
                vecinos.push_back(matriz[0][x-1][help]);
                vecinos.push_back(matriz[0][x+1][help]);
                vecinos.push_back(matriz[1][x-1][help]);
                vecinos.push_back(matriz[1][x+1][help]);
                vecinos.push_back(matriz[1][x][help]);
            }
            else if((y==matriz_size-1 && x!=0 && x!=matriz_size-1)){
                vecinos.push_back(matriz[matriz_size-1][x-1][help]);
                vecinos.push_back(matriz[matriz_size-1][x+1][help]);
                vecinos.push_back(matriz[matriz_size-2][x-1][help]);
                vecinos.push_back(matriz[matriz_size-2][x+1][help]);
                vecinos.push_back(matriz[matriz_size-2][x][help]);
            }
            else if((x==0 && y!=0 && y!=matriz_size-1)){
                vecinos.push_back(matriz[y-1][x][help]);
                vecinos.push_back(matriz[y+1][x][help]);
                vecinos.push_back(matriz[y-1][x+1][help]);
                vecinos.push_back(matriz[y+1][x+1][help]);
                vecinos.push_back(matriz[y][x+1][help]);
            }
            else if((x==matriz_size-1 && y!=0 && y!=matriz_size-1)){
                vecinos.push_back(matriz[y-1][x][help]);
                vecinos.push_back(matriz[y+1][x][help]);
                vecinos.push_back(matriz[y-1][x-1][help]);
                vecinos.push_back(matriz[y+1][x-1][help]);
                vecinos.push_back(matriz[y][x-1][help]);
            }
            else{
                vecinos.push_back(matriz[y-1][x-1][help]);
                vecinos.push_back(matriz[y-1][x+1][help]);
                vecinos.push_back(matriz[y-1][x][help]);
                vecinos.push_back(matriz[y+1][x+1][help]);
                vecinos.push_back(matriz[y+1][x-1][help]);
                vecinos.push_back(matriz[y+1][x][help]);
                vecinos.push_back(matriz[y][x+1][help]);
                vecinos.push_back(matriz[y][x-1][help]);
            }
            
            //Pasarselo al cliente
            for (int i = 0; i < vecinos.size(); i++)
            {
                request req;
                vecinos_str += to_string(vecinos[i]);
                vecinos_str += " ";
                strncpy(req.type, "VECINOS", 8);
                strncpy(req.msg, vecinos_str.c_str(), MENSAJE_MAXIMO);
                send_request(req, matriz[y][x][1]);
                sleep(2);
            }            
            
        }
    }
}
void connection_handler(int socket_desc,vector<vector<int>> &listaPorts, vector<vector<int>> &listaListen){
    struct request riq;
    while(1)
    {
        int n;
        
        vector<int> helper;
        int z;

        n = recv(socket_desc, &riq, 2*MENSAJE_MAXIMO, 0);

        string temp(riq.type);

        if (temp == "PORT"){
            
            helper.push_back(socket_desc);
            string hola = riq.msg;
            int port = std::stoi(hola);
            helper.push_back(port);
            listaPorts.push_back(helper);
            
            
            cout << "Servidor Recibi: ";
            cout << riq.type;
            cout << " ";
            cout << riq.msg << endl;
            
        }
        else if (temp == "S_Listen"){
            
                helper.push_back(socket_desc);
                string hola = riq.msg;
                int port = std::stoi(hola);
                helper.push_back(port);
                cout << "Servidor Recibi: ";
                cout << riq.type;
                cout << " ";
                cout << riq.msg << endl;
                listaListen.push_back(helper);
                
        }
        
        if (listaPorts.size() == 9 && listaListen.size() == 9){
            
            if (po == 0){
                po++;
                struct request riq;
                vector<vector<vector<int>>> matriz = crearMatriz(listaPorts, listaListen);
                conectar_Vecinos(matriz, 3);
                
            }
        }
        
    }
}

// Servicio draw: En cada tick, imprime el mapa con el estado de cada celula 
void draw()
{
    /* TO DO*/
}

// Servicio timer: Cada cierto intervalo de tiempo publica un tick. 
//Es importante que cada tick se envie utilizando el mapa de vecinos actualizado

void timer()
{
   /* TO DO*/
}



// Thread map_creator: Agrega los nuevos nodos al mapa
void map_creator(/* TO DO*/)
{
    /* Registrar los lsn ports de los nuevos */
    /* TIP: Hay que esperar que los clientes manden el mensaje con el lsn port*/
    /* Varias formas de hacerlo, pselect puede resultar comodo para este caso */
    /* Crear threads podria ser demasiado overhead, pero es valido */
    /* TO DO*/

    // Avisar a las celulas que correspondan la nueva estructura de vecinos
    // TIP: Puede ser util separar el caso inicial del resto, sobre todo para
    //      facilitar luego el testeo
    int N;
    if(N ==3){
        //base_case_3x3(/* TO DO*/);
        return;
    }
    //general_case_nxn(/* TO DO*/);

}



void server_accept_conns(int s)
{
    int clientes = 0;
    int client_len = 0;
    vector <thread> threads;
    int socketNuevo;
    list<int> grilla;
    vector<vector<int>> listaPorts;
    vector<vector<int>> listaListen;
    struct sockaddr_in remote;
    struct request req;
    int z=0;
    while(1)
    {
        /* Acpetar nueva celula*/
        client_len = sizeof(remote);
        if((socketNuevo = accept(s, (struct sockaddr *) &remote, (socklen_t*) &client_len)) < 0){
            perror("Error aceptando");
            exit(-1);
        }
        else{
            threads.push_back(thread(connection_handler, socketNuevo, ref(listaPorts), ref(listaListen)));
            
        }
        
        /* Si ya hay suficientes para armar matriz de 3x3 o para agregar L*/
        /* Actualizar el mapa permitiendo que sigan llegando conexiones */
        /* Sugerencia: Lanzar thread pmap_creator

        /* Si no, marcarlas como pendientes y continuar*/
        /* TO DO*/   

    }
    threads[clientes].join();
    close(socketNuevo);
    close(s);
}


int main(int argc, char* argv[])
{
    int s;
    struct sockaddr_in local;
    thread threads[2];

    /* crea socket */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* configura dirección */
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = INADDR_ANY;


    /* linkea socket con dirección */
    if (bind(s, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        exit(1);
    }

    /* setea socket a modo "listen"*/
    if (listen(s, 10) == -1) {
        perror("listen");
        exit(1);
    }

    threads[0]= thread(server_accept_conns, s);



    threads[0].join();
    close(s);
    return 0;
}

