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
    delete[] username;
    delete[] password;
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



void Usuario::agregarFavoritoPorID(const char* idCancion, const char* rutaRepositorio) {
    string idStr(idCancion);
    bool yaExiste = false;

    // --- Verificar si ya existe en favoritos ---
    ifstream in(rutaUsuario);
    if (in.is_open()) {
        string linea;
        while (getline(in, linea)) {
            if (linea.find(idStr) != string::npos) {
                yaExiste = true;
                break;
            }
        }
        in.close();
    }

    if (yaExiste) {
        cout << "  La canción con ID " << idCancion << " ya está en favoritos.\n";
        return;
    }

    // --- Buscar la canción en el repositorio ---
    bool encontrada = false;
    for (const auto& artista : fs::directory_iterator(rutaRepositorio)) {
        for (const auto& album : fs::directory_iterator(artista)) {
            for (const auto& archivo : fs::directory_iterator(album)) {
                string nombre = archivo.path().filename().string();
                if (nombre.find(idStr) != string::npos) {
                    encontrada = true;

                    // --- Asegurarse de que haya salto de línea al final ---
                    fstream out(rutaUsuario, ios::in | ios::out | ios::app);
                    if (!out.is_open()) {
                        cout << " Error al abrir el archivo de favoritos.\n";
                        return;
                    }

                    // Comprobar si el archivo termina sin salto de línea
                    out.seekp(0, ios::end);
                    if (out.tellp() > 0) {
                        out.seekp(-1, ios::end);
                        char lastChar;
                        out.get(lastChar);
                        if (lastChar != '\n') out << '\n';
                    }

                    // --- Agregar nueva canción con salto de línea ---
                    out << archivo.path().string() << '\n';
                    out.close();

                    cout << " Canción agregada a favoritos:\n" << archivo.path().string() << "\n";
                    break;
                }
            }
            if (encontrada) break;
        }
        if (encontrada) break;
    }

    if (!encontrada) {
        cout << " No se encontró ninguna canción con ID " << idCancion << " en el repositorio.\n";
    }
}

void Usuario::eliminarFavoritoPorID(const char* idCancion) {
    string idStr(idCancion);
    string rutaTemp = string(rutaUsuario) + ".tmp";

    ifstream in(rutaUsuario);
    ofstream out(rutaTemp);
    if (!in.is_open() || !out.is_open()) {
        cout << " Error al abrir los archivos.\n";
        return;
    }

    string linea;
    bool eliminada = false;

    while (getline(in, linea)) {
        if (linea.find(idStr) != string::npos) {
            cout << "  Canción con ID " << idCancion << " eliminada de favoritos.\n";
            eliminada = true;
            continue; // no copiamos esta línea
        }
        if (!linea.empty()) out << linea << "\n"; // mantener el resto
    }

    in.close();
    out.close();

    fs::remove(rutaUsuario);
    fs::rename(rutaTemp, rutaUsuario);

    if (!eliminada)
        cout << "  No se encontró ninguna canción con ID " << idCancion << ".\n";
}


void Usuario::cargarFavoritos() {
    cantidadFavoritos = 0;
    ifstream in(rutaUsuario);
    if (!in.is_open()) return;

    string ruta;
    while (getline(in, ruta)) {
        if (ruta.empty()) continue;
        //agregarFavorito(ruta.c_str());
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
        //agregarFavorito(ruta.c_str());
    }
    in.close();
}
