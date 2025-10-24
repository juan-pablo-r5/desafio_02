#include "Publicidad.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const int MAX_PUBLICIDADES = 50;

// Selecciona publicidad ponderada según prioridad (1,2,3) sin repetir consecutiva
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
    srand(time(NULL));
    Publicidad** lista = new Publicidad*[MAX_PUBLICIDADES];
    int cantidad = 0;
    Publicidad* ultima = nullptr;
    int opcion = 0;

    do {
        cout << "\n=== GESTOR DE PUBLICIDAD UdeATunes ===\n";
        cout << "1. Agregar publicidad\n";
        cout << "2. Mostrar una publicidad aleatoria (ponderada)\n";
        cout << "3. Mostrar todas las publicidades cargadas\n";
        cout << "0. Salir\n> ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            if (cantidad >= MAX_PUBLICIDADES) {
                cout << " Limite maximo de 50 publicidades alcanzado.\n";
                continue;
            }

            string tipo, mensaje;
            int duracion;

            cout << "Tipo de publicidad (C / B / AAA): ";
            getline(cin, tipo);
            cout << "Duración (segundos): ";
            cin >> duracion;
            cin.ignore();
            cout << "Mensaje (max 500 caracteres): ";
            getline(cin, mensaje);

            lista[cantidad++] = new Publicidad(tipo.c_str(), duracion, mensaje.c_str());
            cout << "Publicidad agregada correctamente.\n";
        }
        else if (opcion == 2) {
            if (cantidad == 0) {
                cout << " No hay publicidades cargadas.\n";
                continue;
            }

            Publicidad* seleccionada = seleccionarPublicidad(lista, cantidad, ultima);
            if (seleccionada) {
                seleccionada->imprimirPublicidad();
                ultima = seleccionada;
            }
        }
        else if (opcion == 3) {
            cout << "\n--- LISTA DE PUBLICIDADES ---\n";
            if (cantidad == 0) {
                cout << "No hay publicidades registradas.\n";
                continue;
            }
            for (int i = 0; i < cantidad; ++i) {
                cout << i + 1 << ". ";
                lista[i]->imprimirPublicidad();
            }
        }

    } while (opcion != 0);

    // Liberar memoria
    for (int i = 0; i < cantidad; ++i)
        delete lista[i];
    delete[] lista;

    cout << "\nPrograma finalizado correctamente.\n";
    return 0;
}

