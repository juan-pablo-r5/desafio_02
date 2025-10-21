#ifndef ALBUM_H
#define ALBUM_H

class Cancion {
private:
    char* nombre;
    char* artista;
    char* ruta;
    float duracion;
    bool favorita;

public:
    Cancion();
    Cancion(const char* _nombre, const char* _artista, const char* _ruta, float _duracion);
    ~Cancion();

    void marcarFavorita(bool fav);
    void imprimirInfo() const;

    const char* getNombre() const;
    const char* getRuta() const;
};

class Album {
private:
    char* nombre;
    char* artista;
    char* ruta; // ruta base del álbum
    Cancion** canciones;
    int cantidadCanciones;
    int actual; // índice de la canción actual

public:
    Album(const char* _nombre, const char* _artista, const char* _ruta);
    ~Album();

    void agregarCancion(const char* nombre, const char* artista, const char* ruta, float duracion);

    // Reproducción
    void reproducir();          // Reproduce la canción actual
    void pausar();              // Pausa la reproducción
    void reanudar();            // Reanuda la canción pausada
    void detener();             // Detiene y cierra la canción
    void siguiente();           // Pasa a la siguiente canción y la reproduce
    void anterior();            // Pasa a la anterior canción y la reproduce

    void mostrarCanciones() const;

    void reproducirDesdeArchivo(const char* rutaTxt); // Agrega canciones desde archivo y reproduce la primera
    void siguienteFavorita();
};

#endif // ALBUM_H
