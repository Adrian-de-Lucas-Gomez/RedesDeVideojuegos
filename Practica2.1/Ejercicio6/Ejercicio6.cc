#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <thread>

int sendInfo(const char *letra, int socket, sockaddr client, socklen_t clienteleng){
    char buffer[80] = {};
    time_t tiempo;
    struct tm * infoTiempo;

    tiempo = time(NULL);
    infoTiempo = localtime(&tiempo);

    strftime(buffer, sizeof(buffer), letra, infoTiempo);

    return sendto(socket, buffer, sizeof(buffer), 0, &client, clienteleng);
}

int main(int argc, char** argv) //argv[1] indica la direccion
{
    struct addrinfo infoaddres;
    struct addrinfo * sockaddr;

    memset((void*) &infoaddres, 0, sizeof(struct addrinfo));    //Reservamos memoria

    //infoaddres.ai_flags = AI_PASSIVE;
    infoaddres.ai_family = AF_INET; //Mira para IPv4 y para IPv6
    infoaddres.ai_socktype = SOCK_DGRAM; //Puede ser TCP o UDP

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
        hilos[i]=std::thread();
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

    

    while(funcionando){
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        char buffer[80];
        struct sockaddr client;
        socklen_t clienteleng = sizeof(struct sockaddr);

        getnameinfo(&client, clienteleng, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << "Se conectaron desde: " << host << ":" << service << "\n";
        //int cliente_sd = accept(sock, (struct sockaddr *) &client, &clienteleng);
        
        int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, &client, &clienteleng);

        if(bytes == -1){
            std::cout << "Se ha producido un error al recibir el mensaje\n";
            return -1;
        }
        //std::cout << bytes << " bytes de " << client << "\n";
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