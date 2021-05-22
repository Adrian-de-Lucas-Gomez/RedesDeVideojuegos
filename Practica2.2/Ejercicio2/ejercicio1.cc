#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, MAX_LENGHT);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        int32_t dataSize = sizePlayer();
        alloc_data(dataSize);

        //Escribimos primero la posicion en X Y
        char *aux= _data;   //Lo guardamos en el array del jugador

        memcpy(aux, &x, sizeof(int16_t));
        //Movemos el puntero de escritura
        aux += sizeof(int16_t);
        memcpy(aux, &y, sizeof(int16_t));
        //Movemos el puntero de escritura
        aux += sizeof(int16_t);

        //Escribimos el nombre del player
        memcpy(aux, name, sizeof(char)*MAX_LENGHT );
        //Movemos el puntero de escritura
        aux += sizeof(char)*MAX_LENGHT;

        
    }

    int from_bin(char * data)
    {
        int32_t dataSize = sizePlayer();
        alloc_data(dataSize);

        //Ahora traemos la info de aux a name y X e Y

        memcpy(&x, data, sizeof(int16_t));
        //Movemos el puntero de escritura
        data += sizeof(int16_t);
        memcpy(&y, data, sizeof(int16_t));
        //Movemos el puntero de escritura
        data += sizeof(int16_t);

        //Escribimos el nombre del player
        memcpy(name, data, sizeof(char)*MAX_LENGHT);
        //Movemos el puntero de escritura
        data += sizeof(char)*MAX_LENGHT;

        return 0;
    }


public:
    static const size_t MAX_LENGHT=80;
    char name[MAX_LENGHT];

    int16_t x;
    int16_t y;

    int sizePlayer(){
        int tam= (sizeof(char)*MAX_LENGHT) + (2 * sizeof(int16_t));
        return tam;   
    }
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Jugador 1", 164, 612);

    int fileDescriptor = open("./SavedPlayer", O_TRUNC | O_CREAT | O_RDWR, 0666);

    if(fileDescriptor < 0){
        std::cout << "Hubo un problema al abrir/crear el archivo\n";
        return -1;
    }

    
    // 1. Serializar el objeto one_w
    one_w.to_bin();

    // 2. Escribir la serialización en un fichero
    write(fileDescriptor, one_w.data(), one_w.sizePlayer());
    close(fileDescriptor);

    // 3. Leer el fichero
    fileDescriptor = open("./SavedPlayer", O_RDONLY);
    char *deserializedBuffer= (char *)malloc(one_r.sizePlayer());
    read(fileDescriptor, deserializedBuffer, one_r.sizePlayer());
    close(fileDescriptor);

    // 4. "Deserializar" en one_r
    one_r.from_bin(deserializedBuffer);

    // 5. Mostrar el contenido de one_r
    std:: cout << "Player name: " << one_r.name << " X: " << one_r.x << " Y: " << one_r.y << std::endl;

    return 0;
}

