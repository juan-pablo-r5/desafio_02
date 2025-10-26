#ifndef USUARIO_H
#define USUARIO_H

#include "Album.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

class Usuario {
private:
    char* username;
    char* password;
    bool premiumStatus;
    char* fechaInscripcion;
    char* rutaUsuario; // Carpeta del usuario
    Cancion** favoritos;
    int cantidadFavoritos;
    int maxFavoritos;
    Usuario* siguiendo;

public:
    Usuario(const char* _username, const char* _password, bool _premium,
            const char* _fecha, const char* _rutaBase);
    ~Usuario();

    bool login(const char* user, const char* pass) const;
    bool checkPremium() const;

    void agregarFavorito(const char* rutaCancion);
    void eliminarFavorito(const char* nombreCancion);
    void imprimirFavoritos() const;

    void guardarFavoritos() const;
    void cargarFavoritos();
    const char* getRutaFavoritos() const;

    void seguirListaFavoritos(const Usuario* otro);
    const char* getUsername() const { return username; }
    bool esPremium() const { return premiumStatus; }
};

#endif


