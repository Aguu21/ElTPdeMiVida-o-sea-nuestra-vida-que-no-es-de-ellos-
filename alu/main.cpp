#include "header.h"

void clients(int port){
    
    std::string comando = "./client ";
    comando+=to_string(port);
    system(comando.c_str());
}

int main(int argc, char const *argv[])
{
    srand(time(0));
    vector <thread> threads;
    vector<int> ports;
    bool test;
    int cant = 0;
    for (size_t i = 0; i < 9; i++)
    {
        test = false;
        int port = 1025 + (rand()%128);
        if(ports.size() != 0){
            while (test == false){
                for (int i = 0; i < ports.size(); i++){
                    if(port == ports[i]){
                      
                        cant++;        
                    }
                }
                if (cant != 0){
                    cant = 0;
                    port = 1025 + (rand()%128);
                    
                }
                else{
                    test = true;
                }
            }
        }  
        ports.push_back(port);
        threads.push_back(thread(clients, port));
        sleep(2);
    }

    for (unsigned int i = 0; i < 9; i++)
	{
		threads[i].join();
	}
    
    return 0;
}
