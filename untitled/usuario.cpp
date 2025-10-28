#include "Usuario.h"

Usuario::Usuario(const char* _username, const char* _password, bool _premium,
               const char* _rutaBase)
    : premiumStatus(_premium), cantidadFavoritos(0), maxFavoritos(10000), siguiendo(nullptr) {

    username = strdup(_username);
    password = strdup(_password);
    favoritos = new Cancion*[maxFavoritos];

    // Crear carpeta del usuario si no existe
    fs::path carpeta = fs::path(_rutaBase) / username;
    if (!fs::exists(carpeta)) {
        fs::create_directories(carpeta);
    }

    string ruta = (carpeta / "favoritas.txt").string();
    rutaUsuario = strdup(ruta.c_str());
}

Usuario::~Usuario() {
    guardarFavoritos();
    delete[] username;
    delete[] password;
    delete[] fechaInscripcion;
    delete[] rutaUsuario;
    delete[] favoritos;
}

bool Usuario::login(const char* user, const char* pass) const {
    return strcmp(username, user) == 0 && strcmp(password, pass) == 0;
}

bool Usuario::checkPremium() const {
    return premiumStatus;
}

const char* Usuario::getRutaFavoritos() const {
    return rutaUsuario;
}

void Usuario::agregarFavorito(const char* rutaCancion) {
    if (cantidadFavoritos >= maxFavoritos) {
        cout << "Limite de favoritos alcanzado.\n";
        return;
    }

    for (int i = 0; i < cantidadFavoritos; ++i) {
        if (strcmp(favoritos[i]->getRuta(), rutaCancion) == 0) {
            cout << "La cancion ya esta en favoritos.\n";
            return;
        }
    }

    // Crear objeto Cancion solo con nombre y ruta
    fs::path p(rutaCancion);
    string nombre = p.stem().string();
    string album = p.parent_path().filename().string();
    string artista = p.parent_path().parent_path().filename().string();

    favoritos[cantidadFavoritos++] = new Cancion(
        nombre.c_str(), artista.c_str(), album.c_str(), rutaCancion, 0.0f);

    guardarFavoritos();
    cout << "Cancion agregada a favoritos.\n";
}

void Usuario::eliminarFavorito(const char* nombreCancion) {
    for (int i = 0; i < cantidadFavoritos; ++i) {
        if (strcmp(favoritos[i]->getNombre(), nombreCancion) == 0) {
            delete favoritos[i];
            favoritos[i] = favoritos[cantidadFavoritos - 1];
            cantidadFavoritos--;
            guardarFavoritos();
            cout << " Cancion eliminada de favoritos.\n";
            return;
        }
    }
    cout << " Cancion no encontrada.\n";
}

void Usuario::imprimirFavoritos() const {
    cout << "\nLista de favoritos de " << username << ":\n";
    if (cantidadFavoritos == 0) {
        cout << "(Vacia)\n";
        return;
    }
    for (int i = 0; i < cantidadFavoritos; ++i) {
        favoritos[i]->imprimirInfo();
    }
}

void Usuario::guardarFavoritos() const {
    ofstream out(rutaUsuario);
    if (!out.is_open()) {
        cout << "Error guardando lista de favoritos.\n";
        return;
    }

    for (int i = 0; i < cantidadFavoritos; ++i) {
         out << favoritos[i]->getRuta() << endl;
    }

    out.close();
}

void Usuario::cargarFavoritos() {
    cantidadFavoritos = 0;
    ifstream in(rutaUsuario);
    if (!in.is_open()) return;

    string ruta;
    while (getline(in, ruta)) {
        if (ruta.empty()) continue;
        agregarFavorito(ruta.c_str());
    }

    in.close();
}

void Usuario::seguirListaFavoritos(const Usuario* otro) {
    cout << username << " ahora sigue la lista de " << otro->getUsername() << endl;
    ifstream in(otro->getRutaFavoritos());
    if (!in.is_open()) {
        cout << "No se pudo abrir la lista del otro usuario.\n";
        return;
    }
    string ruta;
    while (getline(in, ruta)) {
        agregarFavorito(ruta.c_str());
    }
    in.close();
}
