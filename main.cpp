#include "Album.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h> // Para SetConsoleOutputCP

using namespace std;

// (Asegúrate de tener 'cargarBibliotecaCompleta' en tu .h y .cpp)

int main() {
    // Configuración inicial
    SetConsoleOutputCP(65001); // Para manejar tildes
    srand(time(NULL));

    // Variables de control
    int opcionCarga = 0;
    int opcionMenu = 0;
    bool modeb = false; // false = Secuencial, true = Aleatorio

    Album miAlbum("Reproductor", "Varios Artistas", "");

    cout << "Bienvenido a Spotify (simulado)" << endl;
    cout << "1. Cargar solo mi playlist de Favoritos" << endl;
    cout << "2. Cargar la Biblioteca Completa (music_repository)" << endl;
    cout << "> ";
    cin >> opcionCarga;

    if (opcionCarga == 1) {
        miAlbum.cargarCancionesDesdeTxt("C:\\Users\\Manuu\\Documents\\spotify\\users\\user1\\favoritas.txt");
        int mode;
        cout << "\nEscoja su modo de reproduccion: 1. Secuencial   2. Aleatorio : ";
        cin >> mode;

        if (mode == 2) {
            cout << "Usted escogio modo aleatorio\n";
            modeb = true;
        } else {
            cout << "Usted escogio modo secuencial\n";
        }
    } else if (opcionCarga == 2) {
        miAlbum.cargarBibliotecaCompleta("C:\\Users\\Manuu\\Documents\\spotify\\music_repository");
        cout << "\nCargando biblioteca en modo aleatorio.\n";
        modeb=true;
    } else {
        cout << "Opcion invalida. Saliendo." << endl;
        return 1;
    }


    miAlbum.iniciarReproduccion(modeb);

    while (true) {
        cout << "\n------------------------------------------\n";
        cout << "1. Pausar\n";
        cout << "2. Reanudar\n";
        cout << "3. Siguiente\n";
        cout << "4. Anterior\n";
        cout << "5. (Simular fin de cancion y pasar a Siguiente)\n";
        cout << "0. Salir\n";
        cout << "> ";

        if (!(cin >> opcionMenu)) {
            cout << "Error: Por favor introduce solo numeros.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (opcionMenu == 1) {
            miAlbum.pausar();
        } else if (opcionMenu == 2) {
            miAlbum.reanudar();
        } else if (opcionMenu == 3) {
            // ¡Solo llamamos a 'siguiente' y funciona para todo!
            miAlbum.siguiente(modeb);
        } else if (opcionMenu == 4) {
            miAlbum.anterior(modeb);
        } else if (opcionMenu == 5) {
            cout << "\n...Simulando fin de cancion, pasando a la siguiente..." << endl;
            miAlbum.detener();
            miAlbum.siguiente(modeb); // Igual aquí
        } else if (opcionMenu == 0) {
            miAlbum.detener();
            cout << "Saliendo del reproductor..." << endl;
            break;
        }
    }

    return 0;
}
