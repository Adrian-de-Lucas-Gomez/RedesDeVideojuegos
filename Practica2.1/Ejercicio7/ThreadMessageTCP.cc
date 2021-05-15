class ThreadMessageTCP{
    private:
    int socket;     //De donde vienen los mensajes y a donde los mandamos

    public:

    ThreadMessageTCP(int sock){
        socket = sock;
    }

    int manageConnection(){
        bool funcionando= true;
        while(funcionando){

            char buffer[80];
        
            int bytes = recv(socket, buffer, sizeof(buffer), 0);
        
            if(bytes == -1){
                std::cout << "Se ha producido un error al recibir el mensaje\n";
                return -1;
            }

            else if(bytes > 0){
                int bytesSend = send(socket, buffer, bytes, 0);
                std::cout << "Reenviando el mensaje recibido\n";
        
                if(bytesSend < 0){
                    std::cout << "No se pudo enviar el echo\n";
                    return -1;
                }
            }
        }
        return 0;
    }
};