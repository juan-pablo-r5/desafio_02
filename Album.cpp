#include "Album.h"
#include <iostream>
#include <cstring>
#include <filesystem>
#include <cstdlib>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

Cancion::Cancion() {
    nombre = nullptr;
    artista = nullptr;
    album = nullptr;
    ruta = nullptr;
    duracion = 0.0f;
    favorita = false;
}

Cancion::Cancion(const char* _nombre, const char* _artista, const char* _album, const char* _ruta, float _duracion) {
    nombre = new char[strlen(_nombre) + 1];
    strcpy(nombre, _nombre);
    artista = new char[strlen(_artista) + 1];
    strcpy(artista, _artista);
    album = new char[strlen(_album) + 1];
    strcpy(album, _album);
    ruta = new char[strlen(_ruta) + 1];
    strcpy(ruta, _ruta);
    duracion = _duracion;
    favorita = false;
}

Cancion::~Cancion() {
    delete[] nombre;
    delete[] artista;
    delete[] album;
    delete[] ruta;
}

void Cancion::marcarFavorita(bool fav) {
    favorita = fav;
}

void Cancion::imprimirInfo() const {
    const char* n = (nombre) ? nombre : "(sin nombre)";
    const char* a = (artista) ? artista : "(artista desconocido)";
    const char* al = (album) ? album : "(album desconocido)";
    const char* rut = (ruta) ? ruta : "(portada desconocida)";
    cout << "Artista: " << a << " | Cancion: " << n << " | Album: " << al << " | Ruta: "<< rut << endl;
}

size_t Cancion::getMemoriaConsumida() const {
    size_t memoria = sizeof(Cancion); // Tamaño de los miembros fijos (punteros, float, bool)
    if (nombre) memoria += strlen(nombre) + 1; // +1 por el carácter nulo '\0'
    if (artista) memoria += strlen(artista) + 1;
    if (album) memoria += strlen(album) + 1;
    if (ruta) memoria += strlen(ruta) + 1;
    return memoria;
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
    estadoSimulado = DETENIDO;
    historialReproduccion = nullptr;
    tamHistorial = 0;
}

Album::~Album() {
    detener();
    for (int i = 0; i < cantidadCanciones; i++)
        delete canciones[i];
    delete[] canciones;
    delete[] nombre;
    delete[] artista;
    delete[] ruta;
    delete[] historialReproduccion;
}

void Album::agregarCancion(const char* nombre, const char* artista, const char* album, const char* ruta, float duracion) {
    Cancion** nuevo = new Cancion*[cantidadCanciones + 1];
    for (int i = 0; i < cantidadCanciones; i++)
        nuevo[i] = canciones[i];
    nuevo[cantidadCanciones] = new Cancion(nombre, artista, album, ruta, duracion);
    delete[] canciones;
    canciones = nuevo;
    cantidadCanciones++;
}

size_t Album::getMemoriaConsumida() const {
    size_t memoria = sizeof(Album); // Tamaño de los miembros fijos del álbum
    if (nombre) memoria += strlen(nombre) + 1;
    if (artista) memoria += strlen(artista) + 1;
    if (ruta) memoria += strlen(ruta) + 1;

    // Sumar la memoria del array de punteros a canciones
    if (canciones) {
        memoria += cantidadCanciones * sizeof(Cancion*);
    }

    // Sumar la memoria de CADA objeto Cancion individualmente
    for (int i = 0; i < cantidadCanciones; ++i) {
        if (canciones[i]) {
            memoria += canciones[i]->getMemoriaConsumida();
        }
    }

    // Sumar la memoria del historial
    if (historialReproduccion) {
        memoria += tamHistorial * sizeof(int);
    }

    return memoria;
}

void Album::iniciarReproduccion(bool random, bool playlist) {
    if (cantidadCanciones == 0) {
        cout << "No hay canciones para iniciar." << endl;
        return;
    }

    if (!random) {
        actual = 0;
        reproducir(playlist);
    } else {

        if (cantidadCanciones == 1) {
            actual = 0; // Solo hay una opción
        } else {
            actual = rand() % cantidadCanciones;
        }

        reproducir(playlist);
        // --- FIN DE LA LÓGICA CORREGIDA ---
    }
}

void Album::reproducir(bool playlist) {
    if (cantidadCanciones == 0) { /* ... */ return; }

    detener();
    estadoSimulado = REPRODUCIENDO;

    const int MAX_HISTORIAL = playlist ? 6 : 4;

    // Lógica para manejar el historial con un array dinámico
    if (tamHistorial < MAX_HISTORIAL) {
        // Si hay espacio, creamos un nuevo array más grande
        int* nuevoHistorial = new int[tamHistorial + 1];
        for (int i = 0; i < tamHistorial; ++i) {
            nuevoHistorial[i] = historialReproduccion[i];
        }
        delete[] historialReproduccion;
        historialReproduccion = nuevoHistorial;
        historialReproduccion[tamHistorial] = actual;
        tamHistorial++;
    } else {
        // Si está lleno, deslizamos los elementos hacia la izquierda
        for (int i = 0; i < MAX_HISTORIAL - 1; ++i) {
            historialReproduccion[i] = historialReproduccion[i + 1];
        }
        // Y añadimos el nuevo al final
        historialReproduccion[MAX_HISTORIAL - 1] = actual;
    }

    cout << "Reproduciendo: ";
    canciones[actual]->imprimirInfo();
}

void Album::pausar() {
    // (Sin cambios)
    if (estadoSimulado == REPRODUCIENDO) {
        estadoSimulado = PAUSADO;
        cout << "Pausando cancion..." << endl;
    }
    else if (estadoSimulado == PAUSADO){
        cout << "La cancion ya fue pausada!" << endl;
    }
}

void Album::siguiente(bool random, bool playlist, long long& iteraciones) {
    if (cantidadCanciones == 0) return;

    if (!random) {
        iteraciones++; // Cuenta como una operación/iteración
        actual = (actual + 1) % cantidadCanciones;
        reproducir(playlist);
    } else {
        if (cantidadCanciones == 1) {
            reproducir(playlist);
            return;
        }
        int nuevoIndice;
        do {
            iteraciones++; // <-- INCREMENTAMOS DENTRO DEL BUCLE
            nuevoIndice = rand() % cantidadCanciones;
        } while (nuevoIndice == actual);
        actual = nuevoIndice;
        reproducir(playlist);
    }
}

void Album::reanudar() {
    // (Sin cambios)
    if (estadoSimulado == PAUSADO) {
        estadoSimulado = REPRODUCIENDO;
        cout << "Reanudando canción..." << endl;
    }
}

void Album::detener() {
    estadoSimulado = DETENIDO;
}

void Album::mostrarCanciones() const {
    cout << "\n--- Lista de canciones del álbum: " << nombre << " ---" << endl;
    for (int i = 0; i < cantidadCanciones; i++) {
        cout << i + 1 << ". ";
        canciones[i]->imprimirInfo();
    }
}


void Album::anterior(bool playlist, long long& iteraciones) {
    if (tamHistorial < 2) {
        cout << "No hay una cancion anterior en el historial." << endl;
        return;
    }
    iteraciones++;

    // La canción actual es la última del historial. La anterior es la penúltima.
    int indiceAnterior = historialReproduccion[tamHistorial - 2];

    // Reducimos el tamaño del historial en 2 para eliminar la actual y la anterior,
    // ya que 'reproducir' volverá a añadir la nueva canción actual.
    tamHistorial -= 2;

    actual = indiceAnterior;
    reproducir(playlist);
}

void Album::cargarCancionesDesdeTxt(const char* rutaFavoritas, long long& iteraciones) {
    FILE* archivo = fopen(rutaFavoritas, "r");
    if (!archivo) { /* ... */ return; }

    char buffer[512]; // La ruta completa leída del archivo
    while (fgets(buffer, sizeof(buffer), archivo)) {
        iteraciones++;
        buffer[strcspn(buffer, "\r\n")] = 0; // Limpiar saltos de línea

        char temp[512];
        strcpy(temp, buffer); // Copiamos para poder modificarla

        // Extraer nombre de la canción
        char nombreCancion[128] = "Desconocido";
        char* p = strrchr(temp, '\\');
        if (p) {
            strcpy(nombreCancion, p + 1);
            char* dot = strrchr(nombreCancion, '.');
            if (dot) *dot = '\0'; // Quitar extensión
            *p = '\0'; // Cortar la cadena 'temp'
        }

        // Extraer nombre del álbum
        char nombreAlbum[128] = "Desconocido";
        p = strrchr(temp, '\\');
        if (p) {
            strcpy(nombreAlbum, p + 1);
            *p = '\0'; // Cortar de nuevo
        }

        // Lo que queda es el artista
        char nombreArtista[128] = "Desconocido";
        p = strrchr(temp, '\\');
        if (p) {
            strcpy(nombreArtista, p + 1);
        }

        // La ruta de la portada es un placeholder
        const char* rutaPortada = "Portada Desconocida";

        // buffer contiene la ruta original del archivo de audio, la usamos para la canción
        agregarCancion(nombreCancion, nombreArtista, nombreAlbum, rutaPortada, 0.0f);
        canciones[cantidadCanciones - 1]->marcarFavorita(true);
    }
    fclose(archivo);
}

void Album::cargarBibliotecaCompleta(const char* rutaRepositorio, long long& iteraciones) {
    cout << "Escaneando la biblioteca completa (esto puede tardar)..." << endl;

    try {
        // Itera recursivamente por todas las carpetas y subcarpetas
        for (const auto& entry : fs::recursive_directory_iterator(rutaRepositorio)) {
            iteraciones++;

            // Comprueba si es un archivo regular
            if (entry.is_regular_file()) {
                std::string extension = entry.path().extension().string();

                // Asumimos que quieres cargar archivos .wav
                // (Puedes añadir más, ej: || extension == ".mp3")
                if (extension == ".wav") {

                    std::string rutaCompleta = entry.path().string();

                    // --- REUTILIZAMOS TU LÓGICA DE PARSEO ---
                    // (Copiada de cargarCancionesDesdeTxt)

                    char* nombreCancionParseado = nullptr;
                    char* nombreArtistaParseado = nullptr;
                    char* nombreAlbumParseado = nullptr;
                    char* rutaPortadaFinal = nullptr;

                    try {
                        fs::path ruta_path(rutaCompleta);
                        std::string nombreCancionStd = ruta_path.stem().string();
                        nombreCancionParseado = new char[nombreCancionStd.length() + 1];
                        strcpy(nombreCancionParseado, nombreCancionStd.c_str());

                        fs::path album_path = ruta_path.parent_path();
                        std::string nombreAlbumStd = album_path.filename().string();
                        nombreAlbumParseado = new char[nombreAlbumStd.length() + 1];
                        strcpy(nombreAlbumParseado, nombreAlbumStd.c_str());

                        fs::path artista_path = album_path.parent_path(); // Padre del álbum
                        std::string nombreArtistaStd = artista_path.filename().string();
                        nombreArtistaParseado = new char[nombreArtistaStd.length() + 1];
                        strcpy(nombreArtistaParseado, nombreArtistaStd.c_str());

                        fs::path rutaPortadaGenerada = ruta_path.replace_extension(".png");

                        std::string rutaPortadaStd = "Portada Desconocida";

                        fs::path portadaCheck = album_path / "portada.png"; // 1. Buscar cover.png
                        if (fs::exists(portadaCheck)) {
                            rutaPortadaStd = portadaCheck.string();
                        } else {
                            portadaCheck = album_path / "folder.png"; // 2. Buscar folder.png
                            if (fs::exists(portadaCheck)) {
                                rutaPortadaStd = portadaCheck.string();
                            } else {
                                portadaCheck = album_path / "album.png"; // 3. Buscar album.png
                                if (fs::exists(portadaCheck)) {
                                    rutaPortadaStd = portadaCheck.string();
                                }
                            }
                        }

                        // Guardar la ruta final (sea la portada o el fallback)
                        rutaPortadaFinal = new char[rutaPortadaStd.length() + 1];
                        strcpy(rutaPortadaFinal, rutaPortadaStd.c_str());
                    } catch (const fs::filesystem_error& e) {
                        // ... (Tu lógica de fallback) ...
                        const char* fallbackNombre = "Nombre Desconocido";
                        nombreCancionParseado = new char[strlen(fallbackNombre) + 1];
                        strcpy(nombreCancionParseado, fallbackNombre);
                        nombreAlbumParseado = new char[strlen("Album Desconocido") + 1];
                        strcpy(nombreAlbumParseado, "Album Desconocido");
                        nombreArtistaParseado = new char[strlen("Artista Desconocido") + 1];
                        strcpy(nombreArtistaParseado, "Artista Desconocido");
                        const char* fallbackRuta = "Portada Desconocida";
                        rutaPortadaFinal = new char[strlen(fallbackRuta) + 1];
                        strcpy(rutaPortadaFinal, fallbackRuta);
                    }

                    // Añadimos la canción
                    agregarCancion(nombreCancionParseado, nombreArtistaParseado, nombreAlbumParseado, rutaPortadaFinal, 0.0f);

                    // IMPORTANTE: No la marcamos como favorita
                    // canciones[cantidadCanciones - 1]->marcarFavorita(true);

                    delete[] nombreCancionParseado;
                    delete[] nombreArtistaParseado;
                    delete[] nombreAlbumParseado;
                    delete[] rutaPortadaFinal;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        cout << "Error escaneando el repositorio: " << e.what() << endl;
    }

    cout << "Escaneo completo. Se cargaron " << cantidadCanciones << " canciones." << endl;
}



