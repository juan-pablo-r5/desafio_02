#include "Album.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h> // Para SetConsoleOutputCP
#include <chrono>
#include <thread>

using namespace std;

int main() {
    // Configuración inicial
    SetConsoleOutputCP(65001); // Para manejar tildes
    srand(time(NULL));

    // Variables de control
    int opcionCarga = 0;
    int opcionMenu = 0;
    bool modeb = false; // false = Secuencial, true = Aleatorio
    bool playlist = false;



    Album miAlbum("Reproductor", "Varios Artistas", "");

    cout << "Bienvenido a udeATunes" << endl;
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
        playlist=true;
    } else {
        cout << "Opcion invalida. Saliendo." << endl;
        return 1;
    }


    int modoOperacion = 0;
    cout << "\n------------------------------------------\n";
    cout << "Seleccione el modo de operacion:\n";
    cout << "1. Modo Interactivo (control manual)\n";
    cout << "2. Modo de Prueba Automatica (5 canciones, 3s cada una)\n";
    cout << "> ";
    cin >> modoOperacion;

    if (modoOperacion == 1){
        miAlbum.iniciarReproduccion(modeb,playlist);


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
            miAlbum.siguiente(modeb,playlist);
        } else if (opcionMenu == 4) {
            miAlbum.anterior(playlist);
        } else if (opcionMenu == 5) {
            cout << "\n...Simulando fin de cancion, pasando a la siguiente..." << endl;
            miAlbum.detener();
            miAlbum.siguiente(modeb,playlist); // Igual aquí
        } else if (opcionMenu == 0) {
            miAlbum.detener();
            cout << "Saliendo del reproductor..." << endl;
            break;
        }
    }
    }
    else if (modoOperacion == 2){
        const int K = 5; // Número de canciones a reproducir
        const int segundosEspera = 3;
        cout << "\nIniciando prueba automatica..." << endl;
        cout << "Se reproduciran " << K << " canciones, cambiando cada " << segundosEspera << " segundos." << endl;
        miAlbum.iniciarReproduccion(modeb,playlist);
        for (int i = 1; i < K; ++i) {
            cout << "\n(" << i << "/" << K-1 << ") Esperando " << segundosEspera << " segundos..." << endl;

            // Pausa la ejecución por 3 segundos
            std::this_thread::sleep_for(std::chrono::seconds(segundosEspera));

            cout << "[Temporizador] Cambiando a la siguiente cancion..." << endl;
            miAlbum.siguiente(modeb,playlist);; // Cambia de canción
        }

        cout << "\nPrueba finalizada. Se han reproducido " << K << " canciones. Deteniendo..." << endl;
        miAlbum.detener();
    }

    else {
        cout << "Opcion invalida. Saliendo." << endl;
    }

    return 0;
}

