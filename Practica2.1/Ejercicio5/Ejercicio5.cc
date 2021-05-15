#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <iostream>


int main(int argc, char** argv) //argv[1] direccion, argv[2] puerto usado
{
    struct addrinfo infoaddres;
    struct addrinfo * sockaddr, *aux;

    memset((void*) &infoaddres, 0, sizeof(struct addrinfo));    //Reservamos memoria

    infoaddres.ai_family = AF_INET; //Mira para IPv4
    infoaddres.ai_socktype = SOCK_STREAM; //TCP

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

    bool funcionando=true;

    while(funcionando){

        char mensajeSaliente[80];

        std::cin >> mensajeSaliente;

        if(mensajeSaliente[0] == 'Q' && mensajeSaliente[1] == '\0'){  //Solo se esta enviando el fin de conexion
            funcionando = false;    //Simplemente acabamos el programa
        }

        send(sock, mensajeSaliente, sizeof(mensajeSaliente), 0); //Envio del mensaje escrito

        char mensajeEntrante[80];
        //Recibimos la respuesta del servidor
        int recievedBytes= recv(sock, mensajeEntrante, sizeof(mensajeSaliente), 0);
        
        //Comprobamos que llegue la informacion bien
        if(recievedBytes < 0){
            std::cout << "El mensaje no llegó bien\n";
            return -1;
        }
        //Comprobar si la conexion se cerró
        if(recievedBytes == 0){
            std::cout << "La conexion terminó\n";
            return 0;
        }

        //Si llega bien se lo mostramos
        std::cout << mensajeEntrante << "\n";
    }
    std::cout << "La conexion terminó\n";
    return 0;
}