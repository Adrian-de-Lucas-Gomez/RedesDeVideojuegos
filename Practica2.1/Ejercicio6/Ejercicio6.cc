#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <thread>

#include "ThreadMessage.cc"

int main(int argc, char** argv)
{
    struct addrinfo infoaddres;
    struct addrinfo * sockaddr;

    memset((void*) &infoaddres, 0, sizeof(struct addrinfo));    //Reservamos memoria

    //infoaddres.ai_flags = AI_PASSIVE;
    infoaddres.ai_family = AF_INET; //Mira para IPv4
    infoaddres.ai_socktype = SOCK_DGRAM; //UDP

    //Probamos a ve si aparece la direccion en la red
    int info = getaddrinfo(argv[1], argv[2], &infoaddres, &sockaddr); //HTTP porque es por red¿?

    if(info != 0)   //Ha habido un error y acaba el programa
    { 
        std::cout << "Error: Servicio desconocido\n";
        return -1;
    }
    
    int sock = socket( sockaddr->ai_family, sockaddr->ai_socktype,0);

    if(sock == -1)   //Ha habido un error y acaba el programa
    { 
        std::cout << "Error: socket\n";
        return -1;
    }

    bind(sock, sockaddr->ai_addr, sockaddr->ai_addrlen);
    freeaddrinfo(sockaddr); 

    //Una vez inicializado hay que crear los hilos posibles
    std::thread hilos[6];


    for(int i=0; i< 6; i++){    //Vamos a crear 6 threads
        //Los hilos hijos se crean con una funcion lambda
        hilos[i]=std::thread([&](){ ThreadMessage msg = ThreadMessage(sock); });
    }

    bool funcionando=true;
    //Comprobamos si hay que terminar la ejecicion
    while(funcionando){
        char salida;
        std::cin >> salida;

        if(salida == 'q'){
            funcionando = false;
        }
    }

    //Esperamos a que acabe cada uno de los hilos hijos
    for(int i=0; 1<5; i++){
        hilos[i].join();
    }

    close(sock);

    return 0; 
}