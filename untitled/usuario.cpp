
#include "Usuario.h"

Usuario::Usuario(const char* _username, const char* _password, bool _premium,
                 const char* _fecha)
    : premiumStatus(_premium), cantidadFavoritos(0), maxFavoritos(10000), siguiendo(nullptr) {
    username = strdup(_username);
    password = strdup(_password);
    fechaInscripcion = strdup(_fecha);
    favoritos = new Cancion*[maxFavoritos];
}

Usuario::~Usuario() {
    delete[] username;
    delete[] password;
    delete[] fechaInscripcion;
    delete[] favoritos;
}

bool Usuario::login(const char* user, const char* pass) const {
    return strcmp(username, user) == 0 && strcmp(password, pass) == 0;
}

bool Usuario::checkPremium() const {
    return premiumStatus;
}

void Usuario::agregarFavorito(Cancion* c) {
    if (cantidadFavoritos >= maxFavoritos) {
        cout << "Límite de favoritos alcanzado.\n";
        return;
    }
    for (int i = 0; i < cantidadFavoritos; ++i) {
        if (strcmp(favoritos[i]->getNombre(), c->getNombre()) == 0) {
            cout << "La canción ya está en favoritos.\n";
            return;
        }
    }
    favoritos[cantidadFavoritos++] = c;
    cout << "Canción agregada a favoritos.\n";
}

void Usuario::eliminarFavorito(const char* nombre) {
    for (int i = 0; i < cantidadFavoritos; ++i) {
        if (strcmp(favoritos[i]->getNombre(), nombre) == 0) {
            favoritos[i] = favoritos[cantidadFavoritos - 1];
            cantidadFavoritos--;
            cout << "Canción eliminada de favoritos.\n";
            return;
        }
    }
    cout << "Canción no encontrada en favoritos.\n";
}

void Usuario::seguirListaFavoritos(Usuario* otro) {
    if (!otro->checkPremium()) {
        cout << "Solo se pueden seguir listas de usuarios premium.\n";
        return;
    }
    siguiendo = otro;
    for (int i = 0; i < otro->cantidadFavoritos && cantidadFavoritos < maxFavoritos; ++i) {
        agregarFavorito(otro->favoritos[i]);
    }
    cout << "Ahora sigues la lista de favoritos de " << otro->getUsername() << ".\n";
}

void Usuario::imprimirFavoritos() const {
    cout << "\n--- Lista de Favoritos de " << username << " ---\n";
    if (cantidadFavoritos == 0) {
        cout << "No hay canciones favoritas.\n";
        return;
    }
    for (int i = 0; i < cantidadFavoritos; ++i) {
        favoritos[i]->imprimirInfo();
    }
}
