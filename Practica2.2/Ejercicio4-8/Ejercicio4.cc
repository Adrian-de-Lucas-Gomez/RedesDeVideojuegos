#include <iostream>
#include "Socket.h"
#include "Socket.cc"

int main(int argc, char** argv){

    Socket prueba(argv[1], argv[2]);
    prueba.bind();
    
    std::cout << prueba << "\n";

    char aux;
    std::cin >> aux;    //Para que no se me cierre

    return 0;
}