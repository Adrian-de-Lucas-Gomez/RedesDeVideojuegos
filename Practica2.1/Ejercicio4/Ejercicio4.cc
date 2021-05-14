#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>
#include <unistd.h>

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

    listen(sock, 6);    //Se podrian meter 6 conexiones teóricamente
    //Solo gestiona la primera conexion que llega :'(

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    struct sockaddr client;
    socklen_t clienteleng = sizeof(struct sockaddr);

    //Esperamos a que alguien se conecte
    int clientSock = accept(sock, (struct sockaddr *) &client, &clienteleng);
     
    getnameinfo(&client, clienteleng, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
    std::cout << "Se conectaron desde: " << host << ":" << service << "\n";  

    bool funcionando=true;

    //Bucle donde vamos a ir reenviando lo que llegue
    while(funcionando){
        char buffer[80];
        
        int bytes = recvfrom(clientSock, buffer, sizeof(buffer), 0, &client, &clienteleng);
        
        if(bytes == -1){
            std::cout << "Se ha producido un error al recibir el mensaje\n";
            return -1;
        }

        if(bytes == 0){
            std::cout << "Se ha ternimado la conexion\n";
            funcionando= false;
            break;
        }

        int bytesSend = send(clientSock, buffer, bytes, 0);
        std::cout << "Reenviando el mensaje recibido\n";
        
        if(bytesSend < 0){
            std::cout << "No se pudo enviar el echo\n";
            return -1;
        }
    }
    close(sock);

    return 0; 
}