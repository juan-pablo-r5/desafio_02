#include "Album.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <cstring>
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

const char* Cancion::getNombre() const {
    return nombre;
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
    cout << "▶ Reproduciendo: " << canciones[actual]->getNombre() << endl;
}

void Album::pausar() {
    mciSendStringA("pause miCancion", NULL, 0, NULL);
    cout << "⏸ Pausando canción..." << endl;
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

void Album::siguienteFavorita() {
    if (cantidadCanciones == 0) return;

    int inicio = actual;
    do {
        actual = (actual + 1) % cantidadCanciones;
        if (canciones[actual]->favorita) {
            reproducir();
            return;
        }
    } while (actual != inicio);

    cout << "⚠️ No hay más canciones favoritas." << endl;
}

void Album::reproducirDesdeArchivo(const char* rutaTxt) {
    FILE* archivo = fopen(rutaTxt, "r");
    if (!archivo) {
        cout << "❌ No se pudo abrir el archivo: " << rutaTxt << endl;
        return;
    }

    char buffer[1024];
    if (!fgets(buffer, sizeof(buffer), archivo)) {
        fclose(archivo);
        cout << "❌ El archivo está vacío o tiene error de lectura." << endl;
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
            cout << "⚠️ No se encontró: " << rutaCompleta << endl;
        }

        token = strtok(nullptr, ",\n\r");
    }

    cout << "\n✅ Reproducción desde archivo completada.\n";
}
