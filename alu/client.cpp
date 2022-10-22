#include "header.h"

using namespace std;


// Asumiendo servidor en ip local, y dado un puerto, establece una conexion
// con el destino retornando un socket en estado conectado en caso de exito
int connect_socket(int port)
{
   // TO DO
   int socket_nuevo;
   if (socket_nuevo = accept(port, (struct sockaddr *) &remote, ((socklen_t*) &t) == -1){
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

    if ((int lsn_port =socket(PF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    if (bind(lsn_port, (struct sockaddr *)&local, sizeof(local))< 0){
        perror("bind");
        exit(1);
    }

    if(listen(lsn_port, 10) == -1){
        perror("listen");
        exit(1);
    }
    /* Crear socket de escucha y permitir aceptar conexiones concurrentemente */
    //int lsn_port = /* TO DO*/ 
    //acc_sock_fd = /* TO DO*/
    /* TO DO*/
 
 
    /* Enviar msg al srv con el puerto de escucha */
    /* TO DO*/
    
    send(srv_socket, lsn_port, MENSAJE_MAXIMO);
    /* Obtener lista de vecinos inicial */
    /* TO DO*/

    /* Conectarse a los vecinos */
    /* TO DO*/ 

    /* Enviar msg ready para el server */
    /* TO DO*/
    send(srv_socket, "ready?", MENSAJE_MAXIMO);
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


int main(int argc, char* argv[]){
    int pid;
    int socket_fd;
    int len;
    struct sockaddr_in  remote;
    struct hostent *hp;
    struct in_addr addr;
    char buf[MENSAJE_MAXIMO];

    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("creando socket");
        exit(1);
    }

    /* Establecer la dirección a la cual conectarse. */
    remote.sin_family = AF_INET;
    remote.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &(remote.sin_addr));

    /* Conectarse. */
    int s = connect(socket_fd, (struct sockaddr *)&remote, sizeof(remote));
    if ( s == -1) {
        perror("conectandose");
        exit(1);
    }

    do{
        printf("> ");
        fgets(buf, MENSAJE_MAXIMO, stdin);
        int s = send(socket_fd, buf, strlen(buf), 0);
        if (s == -1) {
            perror("enviando");
            exit(1);
        }

    }while(!feof(stdin));

    /* Cerrar el socket. */
    close(socket_fd);


    /* Lanzar tantos procesos celulas como los indicados por argv[1]*/
    /* TO DO*/
    /*
    Se conectan al servidor
    
    Les llega del servidor sus vecinos
    Hacen conexiones con ellos
    Avisan que estan listas
    Comienza el juego
    Actualizan su estado
    Se lo comunican al servidor
    Avisan que terminaron
    */
    
}