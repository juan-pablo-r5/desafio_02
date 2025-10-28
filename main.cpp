#include "Album.h"
#include "usuario.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include "publicidad.h"
#define MAX_PUBLICIDADES 50


using namespace std;


struct DatosUsuario {
    string nombre;
    string contrasena;
    bool premium;
};

// --- Prototipos de nuestras nuevas funciones ---
bool configurarCargaInicial(Album& miAlbum, bool& modeb, bool& playlist, const char* username, const char* contrasena,bool premium );
void ejecutarModoInteractivo(Album& miAlbum, bool modeb, bool playlist, bool premium, string username);
void ejecutarModoPrueba(Album& miAlbum, bool modeb, bool playlist, bool premium);
bool verificarUsuario(const string& rutaArchivo, const string& nombre, const string& contrasena, DatosUsuario& datos);
Publicidad* seleccinarPublicidad(Publicidad** lista, int cantidad, Publicidad* ultima);

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

bool configurarCargaInicial(Album& miAlbum, bool& modeb, bool& playlist, string username, string contrasena, bool premium) {
    const char* rutaBaseUsuarios = "C:\\Users\\Usuario\\des_02\\users";
    const char* ruta_repo= "C:\\Users\\Usuario\\des_02\\music_repository";


    // Inicializar objeto Usuario
    Usuario usuario(username.c_str(), contrasena.c_str(), premium, rutaBaseUsuarios);


    int opcionCarga = 0;
    cout << "\nBienvenido " << username << " a udeATunes" << endl;
    cout << "1. Sección Favoritos" << endl;
    cout << "2. Biblioteca Completa (music_repository)" << endl;
    cout << "> ";
    cin >> opcionCarga;
    cin.ignore();

    if (opcionCarga == 1) {
        int opcionFavoritos;
        cout << "\n1. Agregar a Favoritos"
             << "\n2. Eliminar de Favoritos"
             << "\n3. Escuchar Mis Favoritos"
             << "\n4. Seguir lista de otro usuario"
             << "\n> ";
        cin >> opcionFavoritos;
        cin.ignore();

        switch (opcionFavoritos) {
        case 1: { // Agregar
            string rutaCancion;
            cout << "\nRuta de la canción a agregar: ";
            getline(cin, rutaCancion);
            usuario.agregarFavoritoPorID(rutaCancion.c_str(),ruta_repo);
            break;
        }
        case 2: { // Eliminar
            string nombreCancion;
            cout << "\nNombre de la canción a eliminar: ";
            getline(cin, nombreCancion);
            usuario.eliminarFavoritoPorID(nombreCancion.c_str());
            break;
        }
        case 3: { // Escuchar favoritos
            char rutaFavoritas[512];
            sprintf(rutaFavoritas, "%s\\%s\\favoritas.txt", rutaBaseUsuarios, username.c_str());

            cout << "\nCargando lista de favoritos..." << endl;
            long long iteraciones = 0;
            miAlbum.cargarCancionesDesdeTxt(rutaFavoritas, iteraciones);

            int mode;
            cout << "\nModo de reproducción (1. Secuencial / 2. Aleatorio): ";
            cin >> mode;
            modeb = (mode == 2);
            playlist = false;

            cout << "Reproduciendo lista de favoritos en modo "
                 << (modeb ? "aleatorio" : "secuencial") << endl;

            miAlbum.iniciarReproduccion(modeb, playlist);
            break;
        }
        case 4: { // Seguir lista de otro usuario
            string otroUsuario;
            cout << "\nIngrese el nombre del usuario que desea seguir: ";
            getline(cin, otroUsuario);

            Usuario otro(otroUsuario.c_str(), "placeholder", false, rutaBaseUsuarios);
            usuario.seguirListaFavoritos(&otro);
            break;
        }
        default:
            cout << "\nOpción inválida.\n";
            return false;
        }
        return true;
    }
    else if (opcionCarga == 2) {
        cout << "\nCargando biblioteca completa..." << endl;
        long long iteraciones = 0;
        miAlbum.cargarBibliotecaCompleta("C:\\Users\\Usuario\\des_02\\music_repository", iteraciones);

        cout << "\nBiblioteca cargada en modo aleatorio.\n";
        modeb = true;
        playlist = true;
        return true;
    }
    else {
        cout << "Opcion invalida. Saliendo.\n";
        return false;
    }
}


void ejecutarModoInteractivo(Album& miAlbum, bool modeb, bool playlist, bool premium) {
    miAlbum.iniciarReproduccion(modeb, playlist);

    int opcionMenu = 0;
    int numPublicidad=1;

    size_t memoria_locales_funcion = sizeof(opcionMenu) + sizeof(modeb) + sizeof(playlist);

    while (true) {
        cout << "\n------------------------------------------\n";
        cout << "1. Pausar\n2. Reanudar\n3. Siguiente\n4. Anterior\n0. Salir\n> ";
        if (!(cin >> opcionMenu)) { /* ... manejo de error ... */ continue; }
        //publicidad
        const char* rutaPublicidades = "C:\\Users\\Usuario\\des_02\\publicidades.txt";
        Publicidad** listaPublicidades = new Publicidad*[MAX_PUBLICIDADES];
        int cantidadPublicidades = Publicidad::cargarDesdeArchivo(rutaPublicidades, listaPublicidades, MAX_PUBLICIDADES);

        if (cantidadPublicidades == 0) {
            cout << "No se cargaron publicidades.\n";
            return;
        }



        switch (opcionMenu) {
        case 1: miAlbum.pausar(); break;
        case 2: miAlbum.reanudar(); break;
        case 3: {
            long long iteraciones = 0;
            numPublicidad++;
            miAlbum.siguiente(modeb, playlist, iteraciones);
            size_t memoriaTotal = miAlbum.getMemoriaConsumida() + memoria_locales_funcion + sizeof(iteraciones);
            cout << "\n================ METRICAS ================\n";
            cout << "a. Iteraciones Requeridas: " << iteraciones << "\n";
            cout << "b. Memoria Total Consumida: " << memoriaTotal << " bytes\n";
            cout << "==========================================\n";
            if (!premium && numPublicidad % 2 == 0) {
                cout << "\n Mostrando publicidad...\n";
                Publicidad* ultima = nullptr;
                Publicidad* seleccionada = seleccionarPublicidad(listaPublicidades, cantidadPublicidades, ultima);
                if (seleccionada) {
                    seleccionada->imprimirPublicidad();
                }
            }
            break;
        }
        case 4: {
            if (!premium){
                cout<< "no puedes retroceder cuenta estandar"<<endl;
                    break;

            }
            else{
                long long iteraciones = 0;
                miAlbum.anterior(playlist, iteraciones);
                size_t memoriaTotal = miAlbum.getMemoriaConsumida() + memoria_locales_funcion + sizeof(iteraciones);
                cout << "\n================ METRICAS ================\n";
                cout << "a. Iteraciones Requeridas: " << iteraciones << "\n";
                cout << "b. Memoria Total Consumida: " << memoriaTotal << " bytes\n";
                cout << "==========================================\n";
                break; // <-- ¡CRUCIAL!
            }

        }
        case 0:
            miAlbum.detener();
            cout << "Saliendo del reproductor..." << endl;
            return;
        default:
            cout << "Opcion no valida." << endl;
            break;
        }

        cout << "\n Fin de la reproduccion.\n";
        for (int i = 0; i < cantidadPublicidades; ++i)
            delete listaPublicidades[i];
        delete[] listaPublicidades;
    }
}

void ejecutarModoPrueba(Album& miAlbum, bool modeb, bool playlist, bool premium) {
    const int K = 5;
    const int segundosEspera = 3;
    size_t memoria_locales_base = sizeof(modeb) + sizeof(playlist) + sizeof(K) + sizeof(segundosEspera);

    const char* rutaPublicidades = "C:\\Users\\Usuario\\des_02\\publicidades.txt";
    Publicidad** listaPublicidades = new Publicidad*[MAX_PUBLICIDADES];
    int cantidadPublicidades = Publicidad::cargarDesdeArchivo(rutaPublicidades, listaPublicidades, MAX_PUBLICIDADES);

    if (cantidadPublicidades == 0) {
        cout << "No se cargaron publicidades.\n";
        return;
    }

    cout << "\nIniciando prueba automatica..." << endl;
    cout << "Se reproduciran " << K << " canciones, cambiando cada " << segundosEspera << " segundos." << endl;
    miAlbum.iniciarReproduccion(modeb, playlist);

    for (int i = 1; i < K; ++i) {
        cout << "\n(" << i << "/" << K - 1 << ") Esperando " << segundosEspera << " segundos..." << endl;

        // --- CORRECCIÓN AQUÍ ---
        // Volvemos a usar la versión de chrono para el temporizador.

        if (!premium && i % 2 == 0) {
            cout << "\n Mostrando publicidad...\n";
            Publicidad* ultima = nullptr;
            Publicidad* seleccionada = seleccionarPublicidad(listaPublicidades, cantidadPublicidades, ultima);
            if (seleccionada) {
                seleccionada->imprimirPublicidad();
            }
        }
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

bool verificarUsuario(const string& rutaArchivo, const string& nombre, const string& contrasena, DatosUsuario& datos) {
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        cout << " No se pudo abrir el archivo: " << rutaArchivo << endl;
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t p1 = linea.find(';');
        size_t p2 = linea.find(';', p1 + 1);

        if (p1 == string::npos || p2 == string::npos)
            continue;

        string nombreArchivo = linea.substr(0, p1);
        string passArchivo = linea.substr(p1 + 1, p2 - p1 - 1);
        bool premium = stoi(linea.substr(p2 + 1)) == 1;

        if (nombreArchivo == nombre && passArchivo == contrasena) {
            datos.nombre = nombreArchivo;
            datos.contrasena = passArchivo;
            datos.premium = premium;
            return true;
        }
    }

    return false;
}
int main() {
    // Configuración inicial
    SetConsoleOutputCP(65001);
    srand(time(NULL));

    // Variables de estado del programa
    bool modeb = false;
    bool playlist = false;

    string username, contrasena;
    const string rutaSudo = "C:\\Users\\Usuario\\des_02\\sudo.txt";

    cout << "=== LOGIN UDEATUNES ===\n";
    cout << "Usuario: ";
    getline(cin, username);
    cout << "Contraseña: ";
    getline(cin, contrasena);

    DatosUsuario usuarios;

    if (verificarUsuario(rutaSudo, username, contrasena, usuarios)) {
        cout << "\n Bienvenido, " << usuarios.nombre << "!\n";
        cout << "Tipo de cuenta: " << (usuarios.premium ? "Premium" : "Estandar") << endl;
    } else {
        cout << "\n Usuario o contrasena incorrectos.\n";
        return false;
    }


    Album miAlbum("Reproductor", "Varios Artistas", "");

    // 1. Cargar la música (dele_gamos a la función auxiliar)
    if (!configurarCargaInicial(miAlbum, modeb, playlist, username, contrasena, usuarios.premium)) {
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
        ejecutarModoInteractivo(miAlbum, modeb, playlist, usuarios.premium);
    } else if (modoOperacion == 2) {
        ejecutarModoPrueba(miAlbum, modeb, playlist, usuarios.premium);
    } else {
        cout << "Opcion invalida. Saliendo." << endl;
    }

    return 0;
}
