#include "Album.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

// --- Prototipos de nuestras nuevas funciones ---
bool configurarCargaInicial(Album& miAlbum, bool& modeb, bool& playlist, const char* username);
void ejecutarModoInteractivo(Album& miAlbum, bool modeb, bool playlist);
void ejecutarModoPrueba(Album& miAlbum, bool modeb, bool playlist);

// --- Implementación de las funciones auxiliares ---
bool configurarCargaInicial(Album& miAlbum, bool& modeb, bool& playlist, const char* username) {
    int opcionCarga = 0;
    cout << "Bienvenido " << username << " a udeATunes" << endl;
    cout << "1. Seccion Favoritos" << endl;
    cout << "2. Biblioteca Completa (music_repository)" << endl;
    cout << "> ";
    cin >> opcionCarga;

    size_t memoria_locales = sizeof(opcionCarga) + sizeof(modeb) + sizeof(playlist) + sizeof(username);

    if (opcionCarga == 1) {
        int opcionFavoritos;
        cout << "1. Editar Favoritos\n2. Seguir otra lista\n3. Escuchar Mis Favoritos\n> ";
        cin >> opcionFavoritos;

        if (opcionFavoritos == 3) {
            char ruta_favoritas_c[512];
            sprintf(ruta_favoritas_c, "C:\\Users\\Manuu\\Documents\\spotify\\users\\%s\\favoritas.txt", username);

            cout << "\nCargando favoritos..." << endl;
            {
                long long iteraciones = 0;
                miAlbum.cargarCancionesDesdeTxt(ruta_favoritas_c, iteraciones);

                size_t memoriaTotal = miAlbum.getMemoriaConsumida() + memoria_locales + sizeof(iteraciones) + sizeof(ruta_favoritas_c);
                cout << "\n================ METRICAS DE CARGA ================\n";
                cout << "a. Iteraciones Requeridas: " << iteraciones << "\n";
                cout << "b. Memoria Total Consumida: " << memoriaTotal << " bytes\n";
                cout << "=================================================\n";
            }

            int mode;
            cout << "\nEscoja su modo de reproduccion: 1. Secuencial    2. Aleatorio : ";
            cin >> mode;
            modeb = (mode == 2);
            cout << "Usted escogio modo " << (modeb ? "aleatorio\n" : "secuencial\n");
            return true;
        } else {
            cout << "Funcion no implementada" << endl;
            return false;
        }
    } else if (opcionCarga == 2) {
        {
            long long iteraciones = 0;
            miAlbum.cargarBibliotecaCompleta("C:\\Users\\Manuu\\Documents\\spotify\\music_repository", iteraciones);

            size_t memoriaTotal = miAlbum.getMemoriaConsumida() + memoria_locales + sizeof(iteraciones);
            cout << "\n================ METRICAS DE CARGA ================\n";
            cout << "a. Iteraciones Requeridas: " << iteraciones << "\n";
            cout << "b. Memoria Total Consumida: " << memoriaTotal << " bytes\n";
            cout << "=================================================\n";
        }
        cout << "\nBiblioteca cargada en modo aleatorio por defecto.\n";
        modeb = true;
        playlist = true;
        return true;
    } else {
        cout << "Opcion invalida. Saliendo." << endl;
        return false;
    }
}
void ejecutarModoInteractivo(Album& miAlbum, bool modeb, bool playlist) {
    miAlbum.iniciarReproduccion(modeb, playlist);
    int opcionMenu = 0;
    size_t memoria_locales_funcion = sizeof(opcionMenu) + sizeof(modeb) + sizeof(playlist);

    while (true) {
        cout << "\n------------------------------------------\n";
        cout << "1. Pausar\n2. Reanudar\n3. Siguiente\n4. Anterior\n0. Salir\n> ";
        if (!(cin >> opcionMenu)) { /* ... manejo de error ... */ continue; }

        switch (opcionMenu) {
        case 1: miAlbum.pausar(); break;
        case 2: miAlbum.reanudar(); break;
        case 3: {
            long long iteraciones = 0;
            miAlbum.siguiente(modeb, playlist, iteraciones);
            size_t memoriaTotal = miAlbum.getMemoriaConsumida() + memoria_locales_funcion + sizeof(iteraciones);
            cout << "\n================ METRICAS ================\n";
            cout << "a. Iteraciones Requeridas: " << iteraciones << "\n";
            cout << "b. Memoria Total Consumida: " << memoriaTotal << " bytes\n";
            cout << "==========================================\n";
            break; // <-- ¡CRUCIAL!
        }
        case 4: {
            long long iteraciones = 0;
            miAlbum.anterior(playlist, iteraciones);
            size_t memoriaTotal = miAlbum.getMemoriaConsumida() + memoria_locales_funcion + sizeof(iteraciones);
            cout << "\n================ METRICAS ================\n";
            cout << "a. Iteraciones Requeridas: " << iteraciones << "\n";
            cout << "b. Memoria Total Consumida: " << memoriaTotal << " bytes\n";
            cout << "==========================================\n";
            break; // <-- ¡CRUCIAL!
        }
        case 0:
            miAlbum.detener();
            cout << "Saliendo del reproductor..." << endl;
            return;
        default:
            cout << "Opcion no valida." << endl;
            break;
        }
    }
}
void ejecutarModoPrueba(Album& miAlbum, bool modeb, bool playlist) {
    const int K = 5;
    const int segundosEspera = 3;
    size_t memoria_locales_base = sizeof(modeb) + sizeof(playlist) + sizeof(K) + sizeof(segundosEspera);

    cout << "\nIniciando prueba automatica..." << endl;
    cout << "Se reproduciran " << K << " canciones, cambiando cada " << segundosEspera << " segundos." << endl;
    miAlbum.iniciarReproduccion(modeb, playlist);

    for (int i = 1; i < K; ++i) {
        cout << "\n(" << i << "/" << K - 1 << ") Esperando " << segundosEspera << " segundos..." << endl;

        // --- CORRECCIÓN AQUÍ ---
        // Volvemos a usar la versión de chrono para el temporizador.
        std::this_thread::sleep_for(std::chrono::seconds(segundosEspera));

        cout << "[Temporizador] Cambiando a la siguiente cancion..." << endl;

        {
            long long iteraciones = 0;
            miAlbum.siguiente(modeb, playlist, iteraciones);
            size_t memoriaTotal = miAlbum.getMemoriaConsumida() + memoria_locales_base + sizeof(i) + sizeof(iteraciones);

            cout << "\n================ METRICAS ================\n";
            cout << "a. Iteraciones Requeridas: " << iteraciones << "\n";
            cout << "b. Memoria Total Consumida: " << memoriaTotal << " bytes\n";
            cout << "==========================================\n";
        }
    }

    cout << "\nPrueba finalizada. Deteniendo..." << endl;
    miAlbum.detener();
}

int main() {
    // Configuración inicial
    SetConsoleOutputCP(65001);
    srand(time(NULL));

    // Variables de estado del programa
    bool modeb = false;
    bool playlist = false;
    char username[30];


    cin>>username;
    cout<<"Ingrese su usuario: "<<endl;



    Album miAlbum("Reproductor", "Varios Artistas", "");

    // 1. Cargar la música (dele_gamos a la función auxiliar)
    if (!configurarCargaInicial(miAlbum, modeb, playlist, username)) {
        return 1; // Si la carga falla o el usuario elige una opción no válida, salimos.
    }

    // 2. Elegir el modo de operación
    int modoOperacion = 0;
    cout << "\n------------------------------------------\n";
    cout << "Seleccione el modo de operacion:\n";
    cout << "1. Modo Interactivo (control manual)\n";
    cout << "2. Modo de Prueba Automatica (5 canciones, 3s cada una)\n";
    cout << "> ";
    cin >> modoOperacion;

    // 3. Ejecutar el modo elegido
    if (modoOperacion == 1) {
        ejecutarModoInteractivo(miAlbum, modeb, playlist);
    } else if (modoOperacion == 2) {
        ejecutarModoPrueba(miAlbum, modeb, playlist);
    } else {
        cout << "Opcion invalida. Saliendo." << endl;
    }

    return 0;
}
