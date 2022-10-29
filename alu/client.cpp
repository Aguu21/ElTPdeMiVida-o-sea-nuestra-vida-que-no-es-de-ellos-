#include "header.h"

using namespace std;


// Asumiendo servidor en ip local, y dado un puerto, establece una conexion
// con el destino retornando un socket en estado conectado en caso de exito
int connect_socket(int port)
{
   struct sockaddr_in remote;
   int socket_nuevo;
   if (socket_nuevo = connect(port, (struct sockaddr *) &remote, sizeof(remote)) == -1){
       perror("aceptando la conexion");
       exit(1);
   }
   else {
       return socket_nuevo;
   }
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
        get_request(srv_req, srv_socket);
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

void client_accept_conns(int s)
{
    int client_len = 0;
    int socketNuevo;
    struct sockaddr_in remote;

    while(1)
    {
        /* Acpetar nueva celula*/
        client_len = sizeof(remote);
        if((socketNuevo = accept(s, (struct sockaddr *) &remote, (socklen_t*) &client_len)) < 0){
            perror("Error aceptando");
            exit(-1);
        }
    }
}

int main(int argc, char* argv[]){
    int pid;
    int socket_fd;
    int len;
    struct request req;
    struct request riq;
    struct sockaddr_in  remote;
    struct sockaddr_in local;
    struct hostent *hp;
    struct in_addr addr;
    char buf[MENSAJE_MAXIMO];
    thread threads[2];
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

    /* crea socket listen */
    if ((s_listen = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* configura dirección para el listen */
    srand(time(0));
    int port = 1025 + rand();
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = INADDR_ANY;


    /* linkea socket con listen con dirección */
    if (bind(s_listen, (struct sockaddr *)&local, sizeof(local)) < 0) {
        perror("bind");
        exit(1);
    }

    /* setea socket a modo "listen"*/
    if (listen(s_listen, 10) == -1) {
        perror("listen");
        exit(1);
    }

    threads[0] = thread(client_accept_conns, s_listen);

    string puerto = std::to_string(local.sin_port);
    
    // Apenas se conecta, tiene que mandar su puerto y su socket en listen
    // El servidor tiene que alamacenar esa informacion
    // El servidor tiene que alamacenar un listo por cada cliente
    // Una vez listos les manda a cada uno a donde conectarse
    // Cada cliente se conecta
    
    strncpy(req.type, "PORT\0", 10);
    strncpy(req.msg, puerto.c_str(), MENSAJE_MAXIMO);
    
    send_request(req, socket_fd);
    
    strncpy(req.type, "S_Listen\0", 10);
    string soc_listen = std::to_string(s_listen);
    strncpy(req.msg, soc_listen.c_str(), MENSAJE_MAXIMO);
    
    send_request(req, socket_fd);

    get_request(riq, socket_fd);
    
    threads[0].join();
    close(s_listen);
    close(socket_fd);
    return 0;
    /* Lanzar tantos procesos celulas como los indicados por argv[1]*/
    /* TO DO*/
    
}