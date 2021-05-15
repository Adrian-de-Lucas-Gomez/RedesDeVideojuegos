class ThreadMessage{
    private:

    int sendInfo(const char *letra, int socket, sockaddr client, socklen_t clienteleng){
    char buffer[80] = {};
    time_t tiempo;
    struct tm * infoTiempo;

    tiempo = time(NULL);
    infoTiempo = localtime(&tiempo);

    strftime(buffer, sizeof(buffer), letra, infoTiempo);

    return sendto(socket, buffer, sizeof(buffer), 0, &client, clienteleng);
    
    }

    public:
    ThreadMessage(const int &socketUsed){
        //Es como el update que haciamos en el ejercicio2
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        char buffer[80];
        struct sockaddr client;
        socklen_t clienteleng = sizeof(struct sockaddr);

        std::cout << "El thread " << std::this_thread::get_id() << " se ha activado\n";

        getnameinfo(&client, clienteleng, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << "Se conectaron desde: " << host << ":" << service << "\n";
    
        
        bool funcionando=true;
        int bytes;

        while (funcionando){
               
            bytes = recvfrom(socketUsed, buffer, sizeof(buffer), 0, &client, &clienteleng);

            std::cout << "tras recibir\n";

            if(bytes == -1){
                std::cout << "Se ha producido un error al recibir el mensaje\n";
                //return -1;
            }
            //std::cout << bytes << " bytes de " << client << "\n";
            std::cout << "Comando: " << buffer[0] << "\n";

            int bytesSend = 0;

            switch(buffer[0]){  //El comando que nos llega
                case 't':
                    bytesSend = sendInfo("%T", socketUsed, client, clienteleng);                
                break;

                case 'd':
                    bytesSend = sendInfo("%D", socketUsed, client, clienteleng); 
                break;

                case 'q':
                    bytesSend = 0;
                    std::cout << "Fin de conexion\n";
                    funcionando=false;
                break;
                
                default:
                    std::cout << "Comando desconocido\n";
                break;
            }

            if(bytesSend < 0){
                std::cout << "No se pudo enviar la respuesta\n";
                //return -1;
            }
            std::cout << "El thread " << std::this_thread::get_id() << " se fue a dormir";

            sleep(5);

            std::cout << "El thread " << std::this_thread::get_id() << " se acaba de despertar";
        }
    }
};