#include "Album.h"
#include <iostream>
using namespace std;

int main() {
    Album miAlbum("Rock Clasico", "Varios Artistas",
                  "C:\\Users\\Manuu\\Documents\\spotify\\music_repository");

    miAlbum.reproducirDesdeArchivo("C:\\Users\\Manuu\\Documents\\spotify\\favoritas.txt");
    cout << "\n1. Pausar\n2. Reanudar\n3. Siguiente Cancion\n0. Salir\n> ";

    int opcion = -1;
    while (true) {

        if (!(cin >> opcion)) {
            cin.clear();
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
