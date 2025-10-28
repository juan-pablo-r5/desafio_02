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
/*
#include "Usuario.h"
#include "Album.h"

#include "Publicidad.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

const int MAX_PUBLICIDADES = 50;

// Selecciona publicidad ponderada por prioridad (1,2,3)
Publicidad* seleccionarPublicidad(Publicidad** lista, int cantidad, Publicidad* ultima) {
    if (cantidad == 0) return nullptr;
    int totalPeso = 0;
    for (int i = 0; i < cantidad; ++i)
        totalPeso += lista[i]->getPrioridad();

    Publicidad* seleccion = nullptr;
    do {
        int random = rand() % totalPeso;
        int acumulado = 0;
        for (int i = 0; i < cantidad; ++i) {
            acumulado += lista[i]->getPrioridad();
            if (random < acumulado) {
                seleccion = lista[i];
                break;
            }
        }
    } while (seleccion == ultima && cantidad > 1);
    return seleccion;
}

int main() {

    const char* base = "C:\\Users\\Usuario\\Documents\\spotify\\users";

    Usuario user1("user1", "1234", true, "2025-10-20", base);
    Usuario user3("user3", "abcd", false, "2025-10-21", base);

    user1.cargarFavoritos();
    user3.cargarFavoritos();

    cout << "\n=== AGREGAR FAVORITOS ===\n";
    user1.agregarFavorito("C:\\Users\\Usuario\\Documents\\spotify\\music_repository\\artist1\\album1\\song1.wav");

    cout << "\n=== FAVORITOS USER1 ===\n";
    user1.imprimirFavoritos();

    cout << "\n=== USER1 SIGUE A USER3 ===\n";
    user1.seguirListaFavoritos(&user3);

    cout << "\n=== NUEVA LISTA USER1 ===\n";
    user1.imprimirFavoritos();

    srand(time(NULL));
    Publicidad** listaPublicidades = new Publicidad*[MAX_PUBLICIDADES];
    int cantidadPublicidades = 0;
    Publicidad* ultima = nullptr;

    // Cargar publicidades desde archivo
    cantidadPublicidades = Publicidad::cargarDesdeArchivo("publicidades.txt", listaPublicidades, MAX_PUBLICIDADES);
    if (cantidadPublicidades == 0) {
        cout << "No se cargaron publicidades. Verifica el archivo.\n";
        return 1;
    }

    cout << "\n=== SIMULADOR DE PUBLICIDAD UDEATUNES ===\n";
    bool esPremium;
    cout << "¿Eres usuario premium? (1 = Si, 0 = No): ";
    cin >> esPremium;

    int canciones = 0;
    int totalCanciones = 6;
    cout << "\nReproduciendo " << totalCanciones << " canciones simuladas...\n";

    for (int i = 1; i <= totalCanciones; ++i) {
        cout << "\n Reproduciendo canci0n " << i << "...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));

        if (!esPremium && i % 2 == 0) {  // Cada 2 canciones, publicidad
            cout << "\nMostrando publicidad (" << i / 2 << ")\n";
            Publicidad* seleccionada = seleccionarPublicidad(listaPublicidades, cantidadPublicidades, ultima);
            if (seleccionada) {
                seleccionada->imprimirPublicidad();
                ultima = seleccionada;
            }
        }
    }

    cout << "\n Fin de la reproduccion.\n";

    for (int i = 0; i < cantidadPublicidades; ++i)
        delete listaPublicidades[i];
    delete[] listaPublicidades;

    return 0;
}*/
