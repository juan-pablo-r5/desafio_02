#include "Album.h"
#include <iostream>
using namespace std;
int opcion;
bool modeb=false;

int main() {
    // Crear álbum vacío, solo se llenará con las favoritas
    Album miAlbum("Favoritas del Usuario", "Varios Artistas", "");

    // Cargar las favoritas del usuario desde su archivo (con rutas completas)
    miAlbum.cargarFavoritasUsuario("C:\\Users\\Manuu\\Documents\\spotify\\users\\user1\\favoritas.txt");

    //Escoger modo de reproduccion
    int mode;
    cout<<"Escoja su modo de reproduccion: 1. Secuencial   2. Aleatorio : ";
    cin>>mode;
    if(mode==2){
        cout<<"\nUsted escogio modo aleatorio\n";
        modeb=true;
    }
    else if(mode==1){
        cout<<"\nUsted escogio modo secuencual\n";
    }

    else{
        cout<<"\nOpcion incorrecta (secuencial agregado por defecto)\n";
    }

    // Empezar a reproducir la primera favorita
    miAlbum.iniciarReproduccion(modeb);

    // Menú simple para controlar reproducción

    cout << "\n1. Pausar  2. Reanudar  3. Siguiente favorita 4. Anterior Favorita  0. Salir\n> ";
    while (true) {
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }

        if (opcion == 1) miAlbum.pausar();
        else if (opcion == 2) miAlbum.reanudar();
        else if (opcion == 3) miAlbum.siguienteFavorita(modeb);
        else if (opcion == 4) miAlbum.atrasFavorita(modeb);
        else if (opcion == 0) {
            miAlbum.detener();
            cout << "Saliendo del reproductor..." << endl;
            break;
        }

        if (!miAlbum.estaReproduciendo() && opcion!=1 && opcion != 2) {
            cout << "\nCancion terminada, pasando a la siguiente..." << endl;
            miAlbum.siguienteFavorita(modeb);
        }
    }

    return 0;
}
