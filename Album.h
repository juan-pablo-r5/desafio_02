#ifndef ALBUM_H
#define ALBUM_H

#include <vector> // Necesario para el historial y simplificaciones futuras
#include <cstdlib>

// --- CLASE CANCION (AÑADIMOS MÉTODO DE MEMORIA) ---
class Cancion {
private:
    char* nombre;
    char* artista;
    char* album;
    char* ruta; // Asumo que esta es la ruta de la PORTADA
    float duracion;
    bool favorita;
public:
    Cancion();
    Cancion(const char* _nombre, const char* _artista, const char* _album, const char* _ruta, float _duracion);
    ~Cancion();

    void marcarFavorita(bool fav);
    void imprimirInfo() const;
    const char* getNombre() const;
    const char* getRuta() const;// Asumo que esto debería devolver la RUTA DEL AUDIO, no de la portada

    bool esFavorita() const { return favorita; }

    // --- NUEVA FUNCIÓN PARA MÉTRICAS ---
    size_t getMemoriaConsumida() const;
};


// --- CLASE ALBUM (ACTUALIZADA PARA MÉTRICAS) ---
class Album {
private:
    char* nombre;
    char* artista;
    char* ruta;
    Cancion** canciones;
    int cantidadCanciones;
    int actual;

    int* historialReproduccion; // <-- AÑADIR ESTO
    int tamHistorial;

    enum EstadoSimulado { DETENIDO, REPRODUCIENDO, PAUSADO };
    EstadoSimulado estadoSimulado;

public:
    Album(const char* _nombre, const char* _artista, const char* _ruta);
    ~Album();

    void agregarCancion(const char* nombre, const char* artista, const char* album, const char* ruta, float duracion);

    // --- FUNCIONES DE REPRODUCCIÓN (MODIFICADAS PARA MÉTRICAS) ---
    void iniciarReproduccion(bool random, bool playlist); // Se queda igual, llama a reproducir
    void reproducir(bool playlist); // Interna, no necesita contador
    void pausar();
    void reanudar();
    void detener();

    // Firmas actualizadas para aceptar el contador de iteraciones
    void siguiente(bool random, bool playlist, long long& iteraciones);
    void anterior(bool playlist, long long& iteraciones);

    // --- FUNCIONES DE CARGA (MODIFICADAS PARA MÉTRICAS) ---
    void cargarCancionesDesdeTxt(const char* rutaFavoritas, long long& iteraciones);
    void cargarBibliotecaCompleta(const char* rutaRepositorio, long long& iteraciones);

    void mostrarCanciones() const;

    // --- NUEVA FUNCIÓN PARA MÉTRICAS ---
    size_t getMemoriaConsumida() const;
};

#endif // ALBUM_H
