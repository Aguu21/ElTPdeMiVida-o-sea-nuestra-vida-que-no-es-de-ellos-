#include "header.h"

// Un regalito, puede que quieran modificarla
// Dado un arreglo de char str y un socket descriptor s, hace una lectura
// bloqueante sobre s y llena el buffer str con los bytes leídos.
int read_sock(int s) 
{
    int n;
    struct request riq;
    vector<vector<int>> listaPorts;
    vector<vector<int>> listaListen;
    vector<vector<vector<int>>> matriz;
    vector<int> listSocket;
    vector<int> helper;
    int z;

    n = recv(s, &riq, 2*MENSAJE_MAXIMO, 0);

    string temp(riq.type);
    string msg = riq.msg;
    if (n == 0) 
        return -1;
    if (n < 0) { 
        perror("recibiendo");
        exit(1);
    }
    if (msg == "PORT"){
        helper.push_back(s);
        string hola = riq.msg;
        int port = std::stoi(hola);
        helper.push_back(port);
        cout << "Servidor Recibi: ";
        cout << riq.type;
        cout << " ";
        cout << riq.msg << endl;
        listaPorts.push_back(helper);
    }
    else if (msg == "S_Listen"){
        helper.push_back(s);
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
        int ayuda;
        vector<int> listaHelper;
        vector<vector<int>> listalistahelper;
        //Ordenar las listas para juntar puertos con socketListen con Socket de conexion.
        
        for(int i; i<9; i++){
            listaHelper.push_back(listaPorts[i][0]);
        }
        
        sort(listaHelper.begin(), listaHelper.end());

        for(int i; i<9; i++){
            listalistahelper[i][0]=listaHelper[i];
            for(int j; j<9;i++){
                if (listaHelper[i] ==listaPorts[j][1]){
                    listalistahelper[i][1]=listaPorts[j][1];
                }
            }
        }

        listaPorts=listalistahelper;

        vector<int> listaHelper;
        vector<vector<int>> listalistahelper;

        for(int i; i<9; i++){
            listaHelper.push_back(listaListen[i][0]);
        }
        
        sort(listaHelper.begin(), listaHelper.end());

        for(int i; i<9; i++){
            listalistahelper[i][0]=listaHelper[i];
            for(int j; j<9;i++){
                if (listaHelper[i] ==listaListen[j][1]){
                    listalistahelper[i][1]=listaListen[j][1];
                }
            }
        }

        listaListen=listalistahelper;

        for (int y = 0; y < 3 ; y++)
        {
            vector<vector<int>> value;
            for (int x = 0; x < 3; x++)
            {
                vector<int> value2;
                for(int z=0;z<3;z++){
                    value2.push_back(0);
                }
                value.push_back(value2);
            }
            matriz.push_back(value);
        }

        for (int y = 0; y < 3 ; y++)
        {
            for (int x = 0; x < 3; x++)
            {
                for (int z=0;z<3;z++){
                    if(z==0){
                        matriz[y][x][z]=listaListen[x][0];
                    }
                    else if(z==1){
                        matriz[y][x][z]=listaListen[x][1];
                    }
                    else{
                        matriz[y][x][z]=listaPorts[x][1];
                    }

                }
            }    
        }

            //map_creator(/*matriz*/);
    }

    return 0;
}


// Dado un puntero a un request req y un socket s, recibe una request en s y la 
// almacena en req. La funcion es bloqueante
void get_request(struct request req, int s)
{
    int n = recv(s, &req,MENSAJE_MAXIMO, 0);
    if (n == -1){
        perror("error recibiendo");
        exit(1);
    }
    else{
        cout << "Cliente Recibi: ";
    
        cout << req.type << endl;
        cout << req.msg << endl;
    }
}

void send_request(struct request req, int s){
    int socket = send(s, &req , MENSAJE_MAXIMO, 0);
    if ( socket < 0) { 
    	perror("error enviando");
    }
}

// Dado un vector de enteros que representan socket descriptors y un request,
// envía a traves de todos los sockets la request.
void broadcast(vector<int>& sockets, struct request* req)
{
    for (int i = 0; i < sockets.size(); i++)
    {
        send(sockets[i], (struct request*) req, MENSAJE_MAXIMO,0);
    }  
}

// Por siempre, acepta conexiones sobre un socket s en estado listen y 
// agrega los sockets asociados al vector v.
void accept_conns(int s, vector<int>& v)
{
    int socketNuevo;
	struct sockaddr_in remote;
	socklen_t client_len;

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
    struct sockaddr_in local;

    if(bind(lsn_port, (struct sockaddr *)&local, sizeof(local))<0){
        perror("bind");
        exit(1);
    }
    if(listen(lsn_port, 10) == -1){
        perror("listen");
        exit(1);
    }
    return lsn_port; 
}

// Setea un socket al modo nobloqueante
static int nonblockingsocket(int s)
{
    int flags;

    if ((flags = fcntl(s, F_GETFL)) == -1)
        return -1;

    return fcntl(s, F_SETFL, flags | O_NONBLOCK);
}
