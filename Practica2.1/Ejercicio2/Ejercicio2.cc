#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>


int main(int argc, char** argv) //argv[1] indica la direccion
{
    struct addrinfo infoaddres;
    struct addrinfo * sockaddr;

    memset((void*) &infoaddres, 0, sizeof(struct addrinfo));    //Reservamos memoria

    infoaddres.ai_family = AF_UNSPEC; //Mira para IPv4 y para IPv6
    infoaddres.ai_socktype = 0; //Puede ser TCP o UDP

    //Probamos a ve si aparece la direccion en la red
    int info = getaddrinfo(argv[1], argv[2], &infoaddres, &sockaddr); //HTTP porque es por red¿?

    if(info != 0)   //Ha habido un error y acaba el programa
    { 
        std::cout << "Error: Name or service not known\n";
        return -1;
    }
    
    int sock = socket( sockaddr->ai_family, sockaddr->ai_socktype,0);

    if(sock == -1)   //Ha habido un error y acaba el programa
    { 
        std::cout << "Error: socket\n";
        return -1;
    }

    bind(sock, sockaddr->ai_addr, sockaddr->ai_addrlen);
    
    while(true){
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        char buffer[80];
        struct sockaddr client;
        socklen_t clienteleng;

        int bytes= recvfrom(sock, (void*) buffer, 80, 0, &client, &clienteleng);

        getnameinfo(&client, clienteleng, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

        sendto(sock, buffer, bytes, 0, &client, clienteleng);
    }

    freeaddrinfo(sockaddr);   //Limpiamos la memoria

    return 0; 
}