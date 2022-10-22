#include "header.h"

// Un regalito, puede que quieran modificarla
// Dado un arreglo de char str y un socket descriptor s, hace una lectura
// bloqueante sobre s y llena el buffer str con los bytes leídos.
int read_sock(int s) 
{
    int n;
    int random=rand()%2;
    char str[MENSAJE_MAXIMO];
    n = recv(s, str, 2*MENSAJE_MAXIMO, 0);

    if (n == 0) 
        return -1;
    if (n < 0) { 
        perror("recibiendo");
        exit(1);
    }
    str[n] = '\0';
    
    printf("%d",n);
    printf("recibi: %s",str);
    return 0;
}


// Dado un puntero a un request req y un socket s, recibe una request en s y la 
// almacena en req. La funcion es bloqueante
void get_request(struct request* req, int s)
{
   // TO DO  
}

// Dado un vector de enteros que representan socket descriptors y un request,
// envía a traves de todos los sockets la request.
void broadcast(vector<int>& sockets, struct request* req)
{
    // TO DO    
}

// Por siempre, acepta conexiones sobre un socket s en estado listen y 
// agrega los sockets asociados al vector v.
void accept_conns(int s, vector<int>& v)
{
	int socketNuevo;
	struct sockaddr_in remote;
	socklen_t client_len;
    int clientes;
    thread threads[MAX_CLIENTS];
	while(1)
	{
		if((socketNuevo = accept(s, (struct sockaddr *) &remote, &client_len)) < 0){
			perror("Aceptando la conexion entrante");
			exit(1);
		}
		
	}
}
// Dado un puerto lsn_port devuelve un socket en estado listen asociado
// a todas las interfaces de red local y a ese puerto (ej 127.0.0.1:lsn_port)
int set_acc_socket(int lsn_port)
{
    // TO DO  
    return 0;
}

// Setea un socket al modo nobloqueante
static int nonblockingsocket(int s)
{
    int flags;

    if ((flags = fcntl(s, F_GETFL)) == -1)
        return -1;

    return fcntl(s, F_SETFL, flags | O_NONBLOCK);
}
