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

const char* Cancion::getNombre() const {
    static char info[512];
    imprimirInfo();
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
    estadoSimulado = DETENIDO;
    historialReproduccion = nullptr;
    tamHistorial = 0;
}

Album::~Album() {
    detener();
    for (int i = 0; i < cantidadCanciones; i++)
        delete canciones[i];
    delete[] canciones;
    delete[] historialReproduccion;
    delete[] nombre;
    delete[] artista;
    delete[] ruta;
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
    // 1. Declara la variable
    int MAX_HISTORIAL;

    if (cantidadCanciones == 0) {
        cout << "No hay canciones para reproducir." << endl;
        return;
    }
    detener();
    estadoSimulado = REPRODUCIENDO;

    // 2. Asígnale el valor
    if(!playlist){
        MAX_HISTORIAL = 4;
    }
    else{
        MAX_HISTORIAL = 6;
    }

    if (tamHistorial < MAX_HISTORIAL) {
        int* nuevoHistorial = new int[tamHistorial + 1];
        for (int i = 0; i < tamHistorial; ++i) {
            nuevoHistorial[i] = historialReproduccion[i];
        }
        nuevoHistorial[tamHistorial] = actual;
        delete[] historialReproduccion;
        historialReproduccion = nuevoHistorial;
        tamHistorial++;

    } else {
        for (int i = 0; i < MAX_HISTORIAL - 1; ++i) {
            historialReproduccion[i] = historialReproduccion[i + 1];
        }
        historialReproduccion[MAX_HISTORIAL - 1] = actual; // Escribe en [5]
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

void Album::siguiente(bool random, bool playlist) {
    if (cantidadCanciones == 0) {
        cout << "No hay canciones en el álbum." << endl;
        return;
    }

    if (!random) {
        int inicio = actual;
        actual = (actual + 1) % cantidadCanciones;

        if (actual == inicio && cantidadCanciones > 1) {
            cout << "Fin de la lista." << endl;
        }

        reproducir(playlist);

    } else {

        if (cantidadCanciones == 1) {
            reproducir(playlist);
            return;
        }

        int nuevoIndice;
        do {
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

bool Album::estaReproduciendo() const {
    return estadoSimulado == REPRODUCIENDO;
}

void Album::anterior(bool playlist) {
    if (cantidadCanciones == 0) {
        cout << "⚠️ No hay canciones en el álbum." << endl;
        return;
    }

    if (tamHistorial < 2) {
        cout << "No hay una canción anterior en el historial." << endl;
        return;
    }

    tamHistorial--;

    int indiceAnterior = historialReproduccion[tamHistorial - 1];
    tamHistorial--;

    actual = indiceAnterior;
    reproducir(playlist);
}

void Album::cargarCancionesDesdeTxt(const char* rutaFavoritas) {
    FILE* archivo = fopen(rutaFavoritas, "r");
    if (!archivo) {
        cout << "No se pudo abrir: " << rutaFavoritas << endl;
        return;
    }

    char buffer[512];

    while (fgets(buffer, sizeof(buffer), archivo)) {
        buffer[strcspn(buffer, "\r\n")] = 0;

        // --- INICIO DE LA COMPROBACIÓN IMPORTANTE ---
        fs::path ruta_path(buffer);

        // 1. ¡Comprobar si el archivo de audio EXISTE!
        if (!fs::exists(ruta_path)) {
            cout << "Advertencia: No se encontro el archivo de audio: " << buffer << endl;
            continue; // Saltar a la siguiente línea del .txt
        }

        // 2. Comprobar que es un archivo y no una carpeta
        if (!fs::is_regular_file(ruta_path)) {
            cout << "Advertencia: La ruta no es un archivo: " << buffer << endl;
            continue;
        }
        // --- FIN DE LA COMPROBACIÓN IMPORTANTE ---

        // Si llegamos aquí, el .wav SÍ existe.
        // Ahora el resto de la lógica (parseo, portadas) tiene sentido.

        char* nombreCancionParseado = nullptr;
        char* nombreArtistaParseado = nullptr;
        char* nombreAlbumParseado = nullptr;
        char* rutaPortadaFinal = nullptr;

        try {
            // fs::path ruta_path(buffer); // (Ya lo hicimos arriba)
            std::string nombreCancionStd = ruta_path.stem().string();
            // ... (el resto de tu lógica de parseo de nombres) ...
            nombreCancionParseado = new char[nombreCancionStd.length() + 1];
            strcpy(nombreCancionParseado, nombreCancionStd.c_str());

            fs::path album_path = ruta_path.parent_path();
            std::string nombreAlbumStd = album_path.filename().string();
            nombreAlbumParseado = new char[nombreAlbumStd.length() + 1];
            strcpy(nombreAlbumParseado, nombreAlbumStd.c_str());

            fs::path artista_path = album_path.parent_path();
            std::string nombreArtistaStd = artista_path.filename().string();
            nombreArtistaParseado = new char[nombreArtistaStd.length() + 1];
            strcpy(nombreArtistaParseado, nombreArtistaStd.c_str());

            // ... (el resto de tu lógica de buscar portada) ...
            std::string rutaPortadaStd = "Portada Desconocida";
            fs::path portadaCheck = album_path / "portada.png";
            if (fs::exists(portadaCheck)) {
                rutaPortadaStd = portadaCheck.string();
            } else {
                portadaCheck = album_path / "folder.png";
                if (fs::exists(portadaCheck)) {
                    rutaPortadaStd = portadaCheck.string();
                } else {
                    portadaCheck = album_path / "album.png";
                    if (fs::exists(portadaCheck)) {
                        rutaPortadaStd = portadaCheck.string();
                    }
                }
            }
            rutaPortadaFinal = new char[rutaPortadaStd.length() + 1];
            strcpy(rutaPortadaFinal, rutaPortadaStd.c_str());

        } catch (const fs::filesystem_error& e) {
            // ... (Tu fallback) ...
        }

        agregarCancion(nombreCancionParseado, nombreArtistaParseado, nombreAlbumParseado, rutaPortadaFinal, 0.0f);
        // ... (Tu limpieza de delete[]) ...
        delete[] nombreCancionParseado;
        delete[] nombreArtistaParseado;
        delete[] nombreAlbumParseado;
        delete[] rutaPortadaFinal;

        canciones[cantidadCanciones - 1]->marcarFavorita(true);
    }
    fclose(archivo);
    actual = 0;
}

void Album::cargarBibliotecaCompleta(const char* rutaRepositorio) {
    cout << "Escaneando la biblioteca completa (esto puede tardar)..." << endl;

    try {
        // Itera recursivamente por todas las carpetas y subcarpetas
        for (const auto& entry : fs::recursive_directory_iterator(rutaRepositorio)) {

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
