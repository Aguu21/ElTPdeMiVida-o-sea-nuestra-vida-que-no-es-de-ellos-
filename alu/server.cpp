#include "header.h"
using namespace std;

// Sugerencia: El servidor va a necestiar varios threads. Recordar que peuden
// compartir variables y que por lo tanto las herramientas de sincronziacion
// como semaforos son perfectamente validas.

void connection_handler(int socket_desc){
    while(1)
    {
        if(read_sock(socket_desc)==(-1)){
            break;
        }
    }
}

void setVecinos(vector<vector<int>> matriz){
    for (int clienteY = 0; clienteY < matriz[0].size(); clienteY++) {
        for (int clienteX = 0; clienteX < matriz.size(); clienteX++) {
            matriz[clienteY][clienteX] = 1;
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
    int delay = 10;

    delay *= CLOCKS_PER_SEC;

    clock_t now = clock();

    while(clock() - now < delay);
    cout<<"Message Show after delay that you entered"<<endl;

}



// Thread map_creator: Agrega los nuevos nodos al mapa
void map_creator(vector<vector<int>> matriz)
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
    struct sockaddr_in remote;
    vector<vector<int>> matriz;
    vector<int> listSocket;
    while(1)
    {
        /* Acpetar nueva celula*/
        client_len = sizeof(remote);
        if((socketNuevo = accept(s, (struct sockaddr *) &remote, (socklen_t*) &client_len)) < 0){
            perror("Error aceptando");
            exit(-1);
        }
        else{
            threads.push_back(thread(connection_handler, socketNuevo));
            //listSocket.push_back(socketNuevo);
            //threads[clientes] = thread(connection_handler, socketNuevo);
            clientes++;
        }
        //if (clientes == 9){
        //    for (size_t i = 0; i < 3 ; i++)
        //    {
        //        vector<int> value;
        //        for (size_t j = 0; j < 3; j++)
        //        {
        //            value.push_back(listSocket[clientes]);
        //            clientes++;
        //        }
        //        matriz.push_back(value);
        //    }
        //    map_creator(matriz);
        //}
        //else{
        //    int variable = 4;
        //    vector<vector<int>> matrizHelper;
        //    for (size_t i = 0; i < variable-1 ; i++)
        //    {
        //        vector<int> value;
        //        for (size_t j = 0; j < variable; j++)
        //        {
        //            if (j==variable-2){
        //                value.push_back(listSocket[i]);
        //            }
        //            else{
        //                value.push_back(matriz[i][j]);
        //            }
        //        }
        //        matrizHelper.push_back(value);
        //    }
        //    vector<int> value;
        //        for (size_t j = 0; j < variable; j++)
        //        {
        //            value.push_back(listSocket[variable+j]);
        //        }
        //        matrizHelper.push_back(value);
        //    variable += 1;
        //    map_creator(matriz);
        //}

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
    int s, s1, s2, len;
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
    /* Levantar servicios y aceptar conexiones */
   /* TO DO*/
    /*
    *Hacer listening
   *Aceptar Clientes

    Conectar clientes entre si
    Chequear que hay 9 clientes conectados
    Comenzar el juego
    envia mensajes a cada cliente de que empezo
    termina el turno, anexar clientes si es que hay suficientes
    actualiza los vecinos
    empieza otro turno
    */
    return 0;
}

