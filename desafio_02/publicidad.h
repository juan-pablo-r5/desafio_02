
#ifndef PUBLICIDAD_H
#define PUBLICIDAD_H

#include <iostream>
#include <cstring>
using namespace std;

class Publicidad {
private:
    char* tipo;      // "C", "B", "AAA"
    int duracion;    // segundos
    char* mensaje;
    int prioridad;   // 1x, 2x, 3x

public:
    Publicidad(const char* _tipo, int _duracion, const char* _mensaje);
    ~Publicidad();

    void imprimirPublicidad() const;
    int getPrioridad() const { return prioridad; }
    const char* getMensaje() const { return mensaje; }
};

#endif // PUBLICIDAD_H

