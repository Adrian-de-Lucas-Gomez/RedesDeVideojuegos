#include "Chat.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char *aux= _data;   //Guardamos el puntero
    
    //Escribimos primero TYPE
        memcpy(aux, &type, sizeof(uint8_t));
        //Movemos el puntero de escritura
        aux += sizeof(uint8_t);

    //Escribimos el nombre del jugador
        memcpy(aux, &nick.c_str(), sizeof(char) * 8);  //Nombre de 8 caracteres
        //Movemos el puntero de escritura
        aux += sizeof(int16_t) * 8;

    //Escribimos el mensaje
        memcpy(aux, name, sizeof(char)*80 );    //Mensaje de 80 caracteres
        //Movemos el puntero de escritura
        aux += sizeof(char)*80;

}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char *aux= _data;   //Guardamos el puntero
    
    //Leemos primero TYPE
        memcpy(aux, &type, sizeof(uint8_t));
        //Movemos el puntero de escritura
        aux += sizeof(uint8_t);

    //Leemos el nombre del jugador
        nick = aux;
        aux += sizeof(int16_t) * 8;

    //Leemos el mensaje
        message = aux;
        aux += sizeof(char)*80;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        //Recibir Mensajes en y en función del tipo de mensaje
        ChatMessage mensaje;
        Socket *sock= &socket;

        socket.recv(mensaje, sock);

        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)
        switch(mensaje.type){
            case 0:
                std::cout << "Recibido LOGGIN de " << mensaje.nick << "\n";
                std::unique_ptr<Socket> cliente(sock);
                clients.push_back(std::move(cliente));
            break;

            case 1:
                std::cout << "Recibido MESSAGE de " << mensaje.nick << "\n";
                for(int i=0; i<clients.size; i++){
                    Socket aux = *clients[i].get();

                    if(!(aux == *sock)){    //Si no es la misma persona se lo reenvio
                        socket.send(mensaje, aux);
                    }
                }
            break;

            case 2:
                std::cout << "LOGOUT de " << mensaje.nick << "\n";

                bool desconexionRealizada=false;
                int i=0;

                while(!desconexionRealizada && i < clients.size()){
                    Socket aux = *clients[i].get();

                    if(aux == *sock){    //Si no es la misma persona se lo reenvio
                        socket.send(mensaje, aux);
                        clients.erase(clients.begin()+i);
                        desconexionRealizada = true;
                    }

                    i++;
                }
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    // Completar
}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
    }
}

