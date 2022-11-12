#include "header.h"
using namespace std;

void conectar_Vecinos(vector<vector<int>> matrizPorts, vector<vector<int>> matrizSocket){
    int matriz_size = matrizSocket.size();
    for (int y = 0; y < matrizSocket.size() ; y++)
    {
        for (int x = 0; x < matrizSocket.size(); x++)
        {
            vector<int> vecinos;
            string vecinos_str;
            if (y == 0 && x == 0){
                vecinos.push_back(matrizPorts[y][x+1]);
                vecinos.push_back(matrizPorts[y+1][x+1]);
                vecinos.push_back(matrizPorts[y+1][x]);
            }
            else if((y == 0 && x == matriz_size - 1)){
                vecinos.push_back(matrizPorts[y][matriz_size-2] );
                vecinos.push_back(matrizPorts[y+1][matriz_size-1] );
                vecinos.push_back(matrizPorts[y+1][matriz_size-2] );                
            }
            else if((y == matriz_size - 1 && x == 0)){
                vecinos.push_back(matrizPorts[matriz_size-2][x] );
                vecinos.push_back(matrizPorts[matriz_size-2][x+1] );
                vecinos.push_back(matrizPorts[matriz_size-1][x+1] );
            }
            else if((y == matriz_size - 1 && x == matriz_size - 1)){
                vecinos.push_back(matrizPorts[matriz_size-1][matriz_size-2] );
                vecinos.push_back(matrizPorts[matriz_size-2][matriz_size-2] );
                vecinos.push_back(matrizPorts[matriz_size-2][matriz_size-1] );
            }
            else if((y == 0 && x != 0 && x != matriz_size - 1)){
                vecinos.push_back(matrizPorts[y][x-1] );
                vecinos.push_back(matrizPorts[y][x+1] );
                vecinos.push_back(matrizPorts[y+1][x-1] );
                vecinos.push_back(matrizPorts[y+1][x+1] );
                vecinos.push_back(matrizPorts[y+1][x] );
            }
            else if((y == matriz_size - 1 && x != 0 && x != matriz_size - 1)){
                vecinos.push_back(matrizPorts[matriz_size-1][x-1] );
                vecinos.push_back(matrizPorts[matriz_size-1][x+1] );
                vecinos.push_back(matrizPorts[matriz_size-2][x-1] );
                vecinos.push_back(matrizPorts[matriz_size-2][x+1] );
                vecinos.push_back(matrizPorts[matriz_size-2][x] );
            }
            else if((x == 0 && y != 0 && y != matriz_size - 1)){
                vecinos.push_back(matrizPorts[y-1][x] );
                vecinos.push_back(matrizPorts[y+1][x] );
                vecinos.push_back(matrizPorts[y-1][x+1] );
                vecinos.push_back(matrizPorts[y+1][x+1] );
                vecinos.push_back(matrizPorts[y][x+1] );
            }
            else if((x == matriz_size - 1 && y != 0 && y != matriz_size - 1)){
                vecinos.push_back(matrizPorts[y-1][x] );
                vecinos.push_back(matrizPorts[y+1][x] );
                vecinos.push_back(matrizPorts[y-1][x-1] );
                vecinos.push_back(matrizPorts[y+1][x-1] );
                vecinos.push_back(matrizPorts[y][x-1] );
            }
            else{
                vecinos.push_back(matrizPorts[y-1][x-1] );
                vecinos.push_back(matrizPorts[y-1][x+1] );
                vecinos.push_back(matrizPorts[y-1][x] );
                vecinos.push_back(matrizPorts[y+1][x+1] );
                vecinos.push_back(matrizPorts[y+1][x-1] );
                vecinos.push_back(matrizPorts[y+1][x] );
                vecinos.push_back(matrizPorts[y][x+1] );
                vecinos.push_back(matrizPorts[y][x-1] );
            }
            
            //Pasarselo al cliente
            struct request req;
            for (int i = 0; i < vecinos.size(); i++)
            {
                vecinos_str += to_string(vecinos[i]);
                vecinos_str += " ";
            }            
            strncpy(req.type, "VECINOS", 8);
            strncpy(req.msg, vecinos_str.c_str(), MENSAJE_MAXIMO);

            send_request(&req, matrizSocket[y][x]);
        }
    }
}

int powInt(int x, int y)
{
    for (int i = 0; i < y; i++)
    {
        x *= 10;
    }
    return x;
}

int parseInt(char* chars)
{
    int sum = 0;
    int len = strlen(chars);
    for (int x = 0; x < len; x++)
    {
        int n = chars[len - (x + 1)] - '0';
        sum = sum + powInt(n, x);
    }
    return sum;
}

// Servicio draw: En cada tick, imprime el mapa con el estado de cada celula 
void draw(vector<vector<int>> matrizEstados)
{
    cout << "\n";
    for (int y = 0; y < matrizEstados.size() ; y++){
        for (int x = 0; x < matrizEstados.size(); x++)
        {
            cout << " | ";
            cout << matrizEstados[y][x];
            cout << " | ";
        }
        cout << "" << endl;

    }
    cout << "\n";
}

// Servicio timer: Cada cierto intervalo de tiempo publica un tick. 
//Es importante que cada tick se envie utilizando el mapa de vecinos actualizado

void timer(vector<vector<int>> matrizSocket, bool &sigo)
{
    
    while(sigo){
        struct request req;
        vector<vector<int>> estados;
        /* Getea los estados de los clientes y cuando termine printea el mapa y les manda el tick*/
        for (int i = 0; i< matrizSocket.size(); i++){

            vector<int> estadito;
            for(int j = 0; j < matrizSocket.size(); j++){
               
                struct request req;
                get_request(&req, matrizSocket[i][j]);
                int help = parseInt(req.msg);
                estadito.push_back(help);
                

            }
            estados.push_back(estadito);

        }
        
        for (int y = 0; y < matrizSocket.size(); y++){
            for (int x = 0; x < matrizSocket.size(); x++){
                
                struct request req;
                
                strncpy(req.type, "TICK", 8);
                strncpy(req.msg, "dale wachO", MENSAJE_MAXIMO);
                
                send_request(&req, matrizSocket[y][x]);
                
            } 
        }
        sleep(3);
        draw(estados);
    }
}

void server_accept_conns(int s)
{
    int client_len = 0;
    int socketNuevo;
    int z = 0;
    int lvl = 0;
    vector<int> sockets;
    vector<thread> threads;
    vector<vector<int>> matrizPorts;
    vector<vector<int>> matrizSocket;

    struct sockaddr_in remote;
    struct request req;
    vector<vector<int>> estados;

    int ultSocket = 0;
    int size = 0;
    bool sigo = true;
    
    while(1)
    {
        /* Aceptar nueva celula*/
        client_len = sizeof(remote);
        if((socketNuevo = accept(s, (struct sockaddr *) &remote, (socklen_t*) &client_len)) < 0){
            perror("Error aceptando");
            exit(-1);
        }
        else{
            sockets.push_back(socketNuevo);
            
            if ((sockets.size() >= 9)){
                if (sockets.size() == 9){
                    for (int y = 0; y < 3; y++)
                    {
                        vector<int> helperSocket;
                        for (int x = 0; x < 3; x++)
                        {
                            helperSocket.push_back(sockets[z]);
                            z++;
                        }
                        matrizSocket.push_back(helperSocket);
                    }
                    
                    for (int i = 0; i < matrizSocket.size(); i++){
                        vector<int> helperPorts;
                        for (int j = 0; j < matrizSocket.size(); j++){
                            struct request req;

                            get_request(&req, matrizSocket[i][j]);
                            
                            string puerto = req.msg;

                            cout << "El puerto es: ";
                            cout << puerto <<endl;

                            helperPorts.push_back(stoi(puerto));
                        }
                        matrizPorts.push_back(helperPorts);
                    }
                    conectar_Vecinos(matrizPorts, matrizSocket);
                    for (int y = 0; y < matrizSocket.size(); y++)
                    {
                        for (int x = 0; x < matrizSocket.size(); x++)
                        {
                            struct request req;
                            
                            strncpy(req.type, "SETEATE", 8);
                            strncpy(req.msg, "dale wachin", MENSAJE_MAXIMO);

                            send_request(&req, matrizSocket[y][x]);
                            sleep(1);
                        }       
                    }
                    sigo = true;
                    threads.push_back(thread(timer, matrizSocket, ref(sigo)));  
                    /* El server lea manda a los clientes que se seteen un estado*/
                    
                }
                else if (sockets.size() == (matrizSocket.size() + 1) * (matrizSocket.size() + 1)){
                    //Llenar matrizSockets
                    
                    ultSocket = matrizSocket.size() * matrizSocket.size();
                    size = matrizSocket.size();
                    sigo = false;
                    vector<int> helperSocket;
                    
                    for (int x = 0; x < (size + 1); x++){
                        
                        helperSocket.push_back(sockets[ultSocket]);
                        ultSocket+=1;
                    }
                    
                    for (int y = 0; y < size; y++){
                        
                        matrizSocket[y].push_back(sockets[ultSocket]);
                        ultSocket+=1;
                    }
                    
                    matrizSocket.push_back(helperSocket);
                   
                    //Pedir request de Ports y meterlos en matrizPorts
                    for (int y = 0; y < (matrizSocket.size() - 1); y++){
                        
                        get_request(&req, matrizSocket[y][matrizSocket.size() - 1]);
                        string puerto = req.msg;
                        matrizPorts[y].push_back(stoi(puerto));
                    }
                    
                    vector<int> helperPorts;
                    for (int x = 0; x < (matrizSocket.size()); x++){
                        get_request(&req, matrizSocket[matrizSocket.size() - 1][x]);
                        string puerto = req.msg;
                        helperPorts.push_back(stoi(puerto));
                    }
                    
                    matrizPorts.push_back(helperPorts);
                    
                    conectar_Vecinos(matrizPorts, matrizSocket);
                   
                    for (int y = 0; y < (matrizSocket.size() - 1); y++){
                        struct request req;
                            
                        strncpy(req.type, "SETEATE", 8);
                        strncpy(req.msg, "dale wachin", MENSAJE_MAXIMO);
                        send_request(&req, matrizSocket[y][matrizSocket.size() - 1]);
                        sleep(1);
                    }
                    
                    for (int x = 0; x < (matrizSocket.size()); x++){
                        struct request req;
                            
                        strncpy(req.type, "SETEATE", 8);
                        strncpy(req.msg, "dale wachin", MENSAJE_MAXIMO);
                        send_request(&req, matrizSocket[matrizSocket.size() - 1][x]);
                        sleep(1);
                    }
                    
                    lvl+=1;
                    sigo = true;
                    threads.push_back(thread(timer, matrizSocket, ref(sigo)));
                }
            }
        }
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}


int main(int argc, char* argv[])
{
    int s;
    struct sockaddr_in local;
	vector<thread> threads;

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

    threads.push_back(thread(server_accept_conns, s));

    for (int i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}

    close(s);
    return 0;
}

