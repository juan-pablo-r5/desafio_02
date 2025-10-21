#include "Album.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <random>

using namespace std;

// -------- Clase Cancion ---------
Cancion::Cancion() {
    nombre = nullptr;
    artista = nullptr;
    ruta = nullptr;
    duracion = 0.0f;
    favorita = false;
}

Cancion::Cancion(const char* _nombre, const char* _artista, const char* _ruta, float _duracion) {
    nombre = new char[strlen(_nombre) + 1];
    strcpy(nombre, _nombre);

    artista = new char[strlen(_artista) + 1];
    strcpy(artista, _artista);

    ruta = new char[strlen(_ruta) + 1];
    strcpy(ruta, _ruta);

    duracion = _duracion;
    favorita = false;
}

Cancion::~Cancion() {
    delete[] nombre;
    delete[] artista;
    delete[] ruta;
}

void Cancion::marcarFavorita(bool fav) {
    favorita = fav;
}

void Cancion::imprimirInfo() const {
    cout << "Canción: " << nombre
         << " | Artista: " << artista
         << " | Duración: " << duracion << " min"
         << (favorita ? " | ★ Favorita" : "") << endl;
}

namespace fs = std::filesystem;
const char* Cancion::getNombre() const {
    static char info[512];


    if (!ruta) {
        snprintf(info, sizeof(info), "Artista: (desconocido) | Cancion: (desconocido) | Album: (desconocido)");
        return info;
    }

    try {
        fs::path ruta_path(ruta);

        std::string nombreCancion = ruta_path.stem().string();

        // La ruta del álbum es el directorio padre de la canción
        fs::path album_path = ruta_path.parent_path();
        std::string nombreAlbum = album_path.filename().string();

        // La ruta del artista es el directorio padre del álbum
        fs::path artista_path = album_path.parent_path();
        std::string nombreArtista = artista_path.filename().string();

        snprintf(info, sizeof(info), "Artista: %s | Cancion: %s | Album: %s",
                 nombreArtista.c_str(),
                 nombreCancion.c_str(),
                 nombreAlbum.c_str());

    } catch (const fs::filesystem_error& e) {
        snprintf(info, sizeof(info), "Error al procesar la ruta: %s", e.what());
    }

    return info;
}
const char* Cancion::getRuta() const {
    return ruta;
}

// -------- Clase Album ---------
Album::Album(const char* _nombre, const char* _artista, const char* _ruta) {
    nombre = new char[strlen(_nombre) + 1];
    strcpy(nombre, _nombre);

    artista = new char[strlen(_artista) + 1];
    strcpy(artista, _artista);

    ruta = new char[strlen(_ruta) + 1];
    strcpy(ruta, _ruta);

    canciones = nullptr;
    cantidadCanciones = 0;
    actual = 0;
}

Album::~Album() {
    detener(); // detener cualquier reproducción antes de destruir
    for (int i = 0; i < cantidadCanciones; i++)
        delete canciones[i];
    delete[] canciones;

    delete[] nombre;
    delete[] artista;
    delete[] ruta;
}

void Album::agregarCancion(const char* nombre, const char* artista, const char* ruta, float duracion) {
    Cancion** nuevo = new Cancion*[cantidadCanciones + 1];
    for (int i = 0; i < cantidadCanciones; i++)
        nuevo[i] = canciones[i];

    nuevo[cantidadCanciones] = new Cancion(nombre, artista, ruta, duracion);
    delete[] canciones;
    canciones = nuevo;
    cantidadCanciones++;
}

void Album::iniciarReproduccion(bool random) {
    if (cantidadCanciones == 0) {
        cout << "⚠️ No hay canciones para iniciar." << endl;
        return;
    }

    if (!random) {
        // --- MODO SECUENCIAL: Empezar desde el principio ---
        actual = 0;
        reproducir();

    } else {
        // --- MODO ALEATORIO: Elegir una favorita al azar para empezar ---

        // 1. Encontrar todas las canciones favoritas.
        std::vector<int> indicesFavoritas;
        for (int i = 0; i < cantidadCanciones; ++i) {
            if (canciones[i]->esFavorita()) {
                indicesFavoritas.push_back(i);
            }
        }

        if (indicesFavoritas.empty()) {
            cout << "⚠️ No se encontraron canciones favoritas para iniciar en modo aleatorio." << endl;
            return;
        }

        // 2. Elegir una al azar de la lista.
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, indicesFavoritas.size() - 1);

        // A diferencia de 'siguienteFavorita', aquí no necesitamos evitar
        // la canción actual, porque ninguna está sonando.
        actual = indicesFavoritas[distrib(gen)];
        reproducir();
    }
}

// -------- Reproducción con MCI ---------
void Album::reproducir() {
    if (cantidadCanciones == 0) {
        cout << "No hay canciones para reproducir." << endl;
        return;
    }

    detener(); // cerrar canción anterior si existe

    const char* alias = "miCancion";
    char comando[512];

    sprintf(comando, "open \"%s\" type waveaudio alias %s", canciones[actual]->getRuta(), alias);
    mciSendStringA(comando, NULL, 0, NULL);

    mciSendStringA("play miCancion", NULL, 0, NULL);
    historialReproduccion.push_back(actual);
    cout << "Reproduciendo: " << canciones[actual]->getNombre() << endl;
}

void Album::pausar() {
    mciSendStringA("pause miCancion", NULL, 0, NULL);
    cout << "Pausando cancion..." << endl;
}

void Album::siguienteFavorita(bool random) {
    if (cantidadCanciones == 0) {
        cout << "No hay canciones en el álbum." << endl;
        return;
    }

    if (!random) {
        // --- MODO SECUENCIAL (Salto de +1) ---
        int inicio = actual;
        do {
            actual = (actual + 1) % cantidadCanciones;
            if (canciones[actual]->esFavorita()) {
                reproducir();
                return;
            }
        } while (actual != inicio);

        cout << "No hay más canciones favoritas." << endl;

    } else {
        // --- MODO ALEATORIO (Elegir de una lista) ---

        // 1. Primero, creamos una lista con los índices de TODAS las favoritas.
        std::vector<int> indicesFavoritas;
        for (int i = 0; i < cantidadCanciones; ++i) {
            if (canciones[i]->esFavorita()) {
                indicesFavoritas.push_back(i);
            }
        }

        if (indicesFavoritas.empty()) {
            cout << "No se encontraron canciones favoritas." << endl;
            return;
        }

        // Si solo hay una favorita, no tiene sentido buscar otra.
        if (indicesFavoritas.size() == 1) {
            actual = indicesFavoritas[0];
            reproducir();
            return;
        }

        // 2. Elegimos un nuevo índice al azar de esa lista de favoritas.
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, indicesFavoritas.size() - 1);

        int nuevoIndice;
        do {
            // Elegimos un índice aleatorio de nuestra lista de opciones válidas.
            nuevoIndice = indicesFavoritas[distrib(gen)];
        } while (nuevoIndice == actual); // Evitamos repetir la misma canción.

        actual = nuevoIndice;
        reproducir();
    }
}
void Album::reanudar() {
    mciSendStringA("resume miCancion", NULL, 0, NULL);
    cout << "▶ Reanudando canción..." << endl;
}

void Album::detener() {
    mciSendStringA("stop miCancion", NULL, 0, NULL);
    mciSendStringA("close miCancion", NULL, 0, NULL);
    // cout << "⏹ Deteniendo reproducción..." << endl;
}

void Album::siguiente() {
    if (cantidadCanciones == 0) return;

    // Avanzar índice de canción
    actual = (actual + 1) % cantidadCanciones;

    // Reproducir la nueva canción
    reproducir();
}

void Album::anterior() {
    if (cantidadCanciones == 0) return;

    // Retroceder índice de canción
    actual = (actual - 1 + cantidadCanciones) % cantidadCanciones;

    // Reproducir la nueva canción
    reproducir();
}

void Album::mostrarCanciones() const {
    cout << "\n--- Lista de canciones del álbum: " << nombre << " ---" << endl;
    for (int i = 0; i < cantidadCanciones; i++) {
        cout << i + 1 << ". ";
        canciones[i]->imprimirInfo();
    }
}

bool Album::estaReproduciendo() const {
    char estado[128];
    // Preguntamos a MCI por el estado del alias "miCancion"
    mciSendStringA("status miCancion mode", estado, sizeof(estado), NULL);

    // Si el estado es "playing", la función strcmp devuelve 0
    return strcmp(estado, "playing") == 0;
}

void Album::atrasFavorita(bool random) {
    if (cantidadCanciones == 0) {
        cout << "⚠️ No hay canciones en el álbum." << endl;
        return;
    }

    if (!random) {
        int inicio = actual;
        do {
            // Usamos la fórmula segura para retroceder
            if(actual!=0){
            actual = (actual - 1 + cantidadCanciones) % cantidadCanciones;
            }
            if (canciones[actual]->esFavorita()) {
                reproducir();
                return;
            }
        } while (actual != inicio);

        cout << "⚠️ No hay más canciones favoritas." << endl;

    } else {

        if (historialReproduccion.size() < 2) {
            cout << "⚠️ No hay una canción anterior en el historial." << endl;
            return;
        }

        historialReproduccion.pop_back(); // Quitamos la canción actual
        int indiceAnterior = historialReproduccion.back(); // Obtenemos la anterior
        historialReproduccion.pop_back(); // La quitamos para no volver a añadirla

        actual = indiceAnterior;
        reproducir();
    }
}


void Album::reproducirDesdeArchivo(const char* rutaTxt) {
    FILE* archivo = fopen(rutaTxt, "r");
    if (!archivo) {
        cout << "No se pudo abrir el archivo: " << rutaTxt << endl;
        return;
    }

    char buffer[1024];
    if (!fgets(buffer, sizeof(buffer), archivo)) {
        fclose(archivo);
        cout << "El archivo está vacío o tiene error de lectura." << endl;
        return;
    }
    fclose(archivo);

    char* token = strtok(buffer, ",\n\r");
    while (token != nullptr) {
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end > token && (*end == ' ' || *end == '\r' || *end == '\n')) *end-- = '\0';

        char rutaCompleta[512];
        sprintf(rutaCompleta, "%s\\%s.wav", ruta, token);

        FILE* test = fopen(rutaCompleta, "r");
        if (test) {
            fclose(test);
            agregarCancion(token, "Desconocido", rutaCompleta, 0.0f);
            if (cantidadCanciones == 1) {
                actual = 0; // la primera canción
                reproducir();
            }
        } else {
            cout << "No se encontró: " << rutaCompleta << endl;
        }

        token = strtok(nullptr, ",\n\r");
    }

    cout << "\nReproducción desde archivo completada.\n";
}

void Album::cargarFavoritasUsuario(const char* rutaFavoritas) {
    FILE* archivo = fopen(rutaFavoritas, "r");
    if (!archivo) {
        cout << "❌ No se pudo abrir: " << rutaFavoritas << endl;
        return;
    }

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), archivo)) {
        buffer[strcspn(buffer, "\r\n")] = 0; // eliminar salto de línea

        // Extraer nombre de la canción de la ruta
        const char* nombreArchivo = strrchr(buffer, '\\');
        if (!nombreArchivo) nombreArchivo = buffer;
        else nombreArchivo++; // saltar la barra

        // Agregar la canción al álbum como favorita
        agregarCancion(nombreArchivo, "Desconocido", buffer, 0.0f);
        canciones[cantidadCanciones - 1]->marcarFavorita(true);
    }

    fclose(archivo);
    actual = 0; // empezar desde la primera favorita
}

