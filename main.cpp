#include "Album.h"
#include <iostream>
using namespace std;
int opcion;

int main() {
    // Crear álbum vacío, solo se llenará con las favoritas
    Album miAlbum("Favoritas del Usuario", "Varios Artistas", "");

    // Cargar las favoritas del usuario desde su archivo (con rutas completas)
    miAlbum.cargarFavoritasUsuario("C:\\Users\\Manuu\\Documents\\spotify\\users\\user1\\favoritas.txt");

    // Empezar a reproducir la primera favorita
    miAlbum.iniciarReproduccion();

    // Menú simple para controlar reproducción

    cout << "\n1. Pausar  2. Reanudar  3. Siguiente favorita  0. Salir\n> ";
    while (true) {
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }

        if (opcion == 1) miAlbum.pausar();
        else if (opcion == 2) miAlbum.reanudar();
        else if (opcion == 3) miAlbum.siguienteFavorita();
        else if (opcion == 0) {
            miAlbum.detener();
            cout << "👋 Saliendo del reproductor..." << endl;
            break;
        }
    }

    return 0;
}
