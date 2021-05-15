#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>
#include <unistd.h>

#include <thread>
#include "ThreadMessageTCP.cc"

int main(int argc, char** argv) //argv[1] indica la direccion
{
    struct addrinfo infoaddres;
    struct addrinfo * sockaddr;

    memset((void*) &infoaddres, 0, sizeof(struct addrinfo));    //Reservamos memoria

    //infoaddres.ai_flags = AI_PASSIVE;
    infoaddres.ai_family = AF_INET; //Mira para IPv4 y para IPv6
    infoaddres.ai_socktype = SOCK_STREAM; //TCP

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

    

    if(listen(sock, 6) != 0){   //Para comprobar si lo ha activado bien
        std::cout << "Falló el listen\n";
        return -1;
    }

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    struct sockaddr client;
    socklen_t clienteleng = sizeof(struct sockaddr);

    

    bool funcionando=true;

    //Bucle donde vamos a ir reenviando lo que llegue
    while(funcionando){

         //Esperamos a que alguien se conecte
        int clientSock = accept(sock, (struct sockaddr *) &client, &clienteleng);
     
        getnameinfo(&client, clienteleng, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << "Se conectaron desde: " << host << ":" << service << "\n";  

        //Tratamiento de la conexion desde una thread hija
        ThreadMessageTCP* handler= new ThreadMessageTCP(clientSock);

        //Creamos una thread con una funcion lambda (primero lo maneja y luego se destruye)
        std::thread hilo = std::thread([&handler](){ handler->manageConnection();  delete handler;});
        hilo.detach();
    }

    return 0; 
}