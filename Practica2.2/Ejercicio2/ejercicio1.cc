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
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
        int fileDescriptor = open("SavedPlayer.txt", O_RDONLY | O_CREAT)

        if(fileDescriptor < 0){
            std::cout << "Hubo un problema al abrir/crear el archivo\n";
            return -1;
        }
        //Escribimos primero la posicion en X Y
        //write();

        //Escribimos el nombre del player
        //write();

        close(fileDescriptor);
    }

    int from_bin(char * data)
    {
        //
        return 0;
    }


public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // 1. Serializar el objeto one_w
    // 2. Escribir la serialización en un fichero
    // 3. Leer el fichero
    // 4. "Deserializar" en one_r
    // 5. Mostrar el contenido de one_r

    return 0;
}

