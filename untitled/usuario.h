/*
#ifndef USUARIO_H
#define USUARIO_H

//#include "Album.h"
#include <iostream>
#include <cstring>
using namespace std;

class Usuario {
private:
    char* username;
    char* password;
    bool premiumStatus;
    char* ciudad;
    char* pais;
    char* fechaInscripcion;
    Cancion** favoritos;
    int cantidadFavoritos;
    int maxFavoritos;
    Usuario* siguiendo;

public:
    Usuario(const char* _username, const char* _password, bool _premium,
            const char* _ciudad, const char* _pais, const char* _fecha);
    ~Usuario();

    bool login(const char* user, const char* pass) const;
    bool checkPremium() const;

    void agregarFavorito(Cancion* c);
    void eliminarFavorito(const char* nombre);
    void seguirListaFavoritos(Usuario* otro);
    void imprimirFavoritos() const;

    const char* getUsername() const { return username; }
    bool esPremium() const { return premiumStatus; }
};

#endif // USUARIO_H */

