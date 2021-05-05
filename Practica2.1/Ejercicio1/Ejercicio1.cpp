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

    hintsInfo.ai_family = AF_UNSPEC; //Mira para IPv4 y para IPv6
    hintsInfo.ai_socktype = 0; //Puede ser TCP o UDP

    //Probamos a ve si aparece la direccion en la red
    int info = getaddrinfo(argv[1], "http", &hintsInfo, &sockaddr); //HTTP porque es por red¿?

    if(info != 0)   //Ha habido un error y acaba el programa
    { 
        std::cout << "Error: Name or service not known\n";
        return -1;
    }
    
    //Miramos todas las direcciones que hemos encontrado y que se guaradaron en sockaddr
    for(struct addrinfo* i = sockaddr; i != NULL; i = i->ai_next){

        char host[NI_MAXHOST];  //Ponemos tamaño a los char que va a rellenar getnameinfo(3)
        char service[NI_MAXSERV];

        getnameinfo(i->ai_addr, i->ai_addrlen,
                       host, NI_MAXHOST,
                       service, NI_MAXSERV, NI_NUMERICHOST);

        std::cout << host << " " << i->ai_family << " " << i->ai_socktype << "\n";
    }

    freeaddrinfo(sockaddr);   //Limpiamos la memoria

    return 0; 
}