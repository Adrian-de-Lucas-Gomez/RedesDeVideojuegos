#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>
#include <unistd.h>
#include <time.h>

//Metodo que te devuelve la fecha u hora segun que le pidas
int sendInfo(const char *letra, int socket, sockaddr client, socklen_t clienteleng){
    char buffer[80] = {};
    time_t tiempo;
    struct tm * infoTiempo;

    tiempo = time(NULL);
    infoTiempo = localtime(&tiempo);

    strftime(buffer, sizeof(buffer), letra, infoTiempo);

    return sendto(socket, buffer, sizeof(buffer), 0, &client, clienteleng);
}

int main(int argc, char** argv) //argv[1] es la direccion y argv[2] es el puerto
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

    bool funcionando=true;

    while(funcionando){
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        char buffer[80];
        struct sockaddr client;
        socklen_t clienteleng = sizeof(struct sockaddr);

        getnameinfo(&client, clienteleng, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << "Se conectaron desde: " << host << ":" << service << "\n";
        
        int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, &client, &clienteleng);

        if(bytes == -1){
            std::cout << "Se ha producido un error al recibir el mensaje\n";
            return -1;
        }
        
        std::cout << "Comando: " << buffer[0] << "\n";

        int bytesSend = 0;

        switch(buffer[0]){  //El comando que nos llega
                case 't':
                bytesSend = sendInfo("%T", sock, client, clienteleng);                
            break;
                case 'd':
                bytesSend = sendInfo("%D", sock, client, clienteleng); 
            break;
                case 'q':
                bytesSend = 0;
                std::cout << "Fin de conexion\n";
                funcionando=false;
            break;
        }

        if(bytesSend < 0){
            std::cout << "No se pudo enviar la respuesta\n";
            return -1;
        }
    }
    close(sock);

    return 0; 
}