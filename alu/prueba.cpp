#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <vector>
#include <semaphore.h>
#include <mutex>
#include <sys/wait.h>
#include <cassert>
#include <time.h>       /* time */
#define PORT 5030
#define MENSAJE_MAXIMO  256
#define MAX_CLIENTS 10
#define CLIENT_READY "CL_OK\n"
#define CANT_CLIENTS 9

using namespace std;
#include <thread>
using namespace std;

int main(int argc, char* argv[])
{
    vector<vector<int>> matriz;
    int numero = 0;
    for (size_t i = 0; i < 3 ; i++)
    {
        vector<int> value;
        for (size_t j = 0; j < 3; j++)
        {
            value.push_back(j);
            numero++;
        }
        matriz.push_back(value);
    }
    for (size_t i = 0; i < 3 ; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            printf("%d", matriz[i][j]);
        }
        printf("\n");
    }
}
