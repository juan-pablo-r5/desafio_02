#ifndef ALBUM_H
#define ALBUM_H

// (Clase Cancion sin cambios, ya estaba bien)
class Cancion {
private:
    char* nombre;
    char* artista;
    char* album;
    char* ruta;
    float duracion;
    bool favorita;
public:
    Cancion();
    Cancion(const char* _nombre, const char* _artista, const char* _album, const char* _ruta, float _duracion);
    ~Cancion();
    void marcarFavorita(bool fav);
    void imprimirInfo() const;
    const char* getNombre() const;
    const char* getRuta() const;
    bool esFavorita() const { return favorita; }
};

class Album {
private:
    char* nombre;
    char* artista;
    char* ruta;
    Cancion** canciones;
    int cantidadCanciones;
    int actual;

    // --- MIEMBROS REDUNDANTES ELIMINADOS ---
    // vector<Cancion*> favoritasUsuario;
    // std::vector<int> historialReproduccion;
    // int actualFavorita = -1;

    int* historialReproduccion; // El que sí usamos
    int tamHistorial;

    enum EstadoSimulado {
        DETENIDO,
        REPRODUCIENDO,
        PAUSADO
    };
    EstadoSimulado estadoSimulado;

public:
    Album(const char* _nombre, const char* _artista, const char* _ruta);
    ~Album();

    void agregarCancion(const char* nombre, const char* artista, const char* album, const char* ruta, float duracion);

    // Reproducción
    void iniciarReproduccion(bool random);
    void reproducir(bool playlist);
    void pausar();
    void reanudar();
    void detener();
    void siguiente(bool random, bool playlist);
    void anterior(bool random);

    void mostrarCanciones() const;
    void cargarCancionesDesdeTxt(const char *rutaFavoritas);
    bool estaReproduciendo() const;
    void cargarBibliotecaCompleta(const char *rutaRepositorio);
    void iniciarReproduccion(bool random, bool playlist);
    void anterior(bool random, bool playlist);
};

#endif // ALBUM_H
