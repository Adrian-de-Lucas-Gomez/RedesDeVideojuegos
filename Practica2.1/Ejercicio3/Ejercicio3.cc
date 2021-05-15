#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>


int main(int argc, char** argv) //argv[1] direccion, argv[2] puerto usado y argv[3] comando a mandar
{
    struct addrinfo infoaddres;
    struct addrinfo * sockaddr, *aux;

    memset((void*) &infoaddres, 0, sizeof(struct addrinfo));    //Reservamos memoria

    infoaddres.ai_family = AF_INET; //Mira para IPv4
    infoaddres.ai_socktype = SOCK_DGRAM; //UDP

    //Probamos a ve si aparece la direccion en la red
    int info = getaddrinfo(argv[1], argv[2], &infoaddres, &sockaddr); 

    int sock;
    
    for(aux = sockaddr; aux != NULL; aux = aux->ai_next){    //Buscamos cual es una valida

        sock = socket(aux->ai_family, aux->ai_socktype, 0); //creamos un socket
        
        if(sock == -1) 
            continue;   //Que pase a la siguiente entrada que esta no ha valido
        
        int connection = connect(sock, aux->ai_addr, aux->ai_addrlen);
        if(connection != -1){
            break;  //Si la conexion fue correcta nos salimos
        }
    }
    if(aux == NULL){
        std::cout << "El socket no fue valido\n";
        return -1;
    }

    freeaddrinfo(sockaddr); //Liberamos la informacion ya que ya no nos es necesario

    bind(sock, sockaddr->ai_addr, sockaddr->ai_addrlen);
    char buffer[80];

    struct sockaddr client;
    socklen_t clienteleng = sizeof(struct sockaddr);

    sendto(sock, argv[3], sizeof(argv[3]), 0, NULL, 0); //Envio del comando dado
    
    int recievedBytes= recvfrom(sock, buffer, sizeof(buffer), 0, &client, & clienteleng);

    //Comprobamos que llegue la informacion bien
    if(recievedBytes < 0){
        std::cout << "El mensaje no llegó bien\n";
        return -1;
    }

    //Si llega bien
    std::cout << buffer << "\n";

    return 0;
}