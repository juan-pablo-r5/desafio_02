#ifndef PUBLICIDAD_H
#define PUBLICIDAD_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

class Publicidad {
private:
    char* tipo;
    int duracion;
    char* mensaje;
    int prioridad;

public:
    Publicidad(const char* _tipo, int _duracion, const char* _mensaje);
    ~Publicidad();

    void imprimirPublicidad() const;
    int getPrioridad() const { return prioridad; }
    const char* getMensaje() const { return mensaje; }

    static int cargarDesdeArchivo(const char* ruta, Publicidad** lista, int maxPublicidades);
};

#endif
