#include "Publicidad.h"

Publicidad::Publicidad(const char* _tipo, int _duracion, const char* _mensaje)
    : duracion(_duracion) {
    tipo = strdup(_tipo);
    mensaje = strdup(_mensaje);

    if (strcmp(_tipo, "AAA") == 0) prioridad = 3;
    else if (strcmp(_tipo, "B") == 0) prioridad = 2;
    else prioridad = 1;
}

Publicidad::~Publicidad() {
    delete[] tipo;
    delete[] mensaje;
}

void Publicidad::imprimirPublicidad() const {
    cout << "\n--- PUBLICIDAD ---\n";
    cout << "Tipo: " << tipo << " | Duración: " << duracion << " seg\n";
    cout << "Mensaje: " << mensaje << "\n";
}

// --- Nuevo método: carga desde archivo ---
int Publicidad::cargarDesdeArchivo(const char* ruta, Publicidad** lista, int maxPublicidades) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        cout << " No se pudo abrir el archivo de publicidades: " << ruta << endl;
        return 0;
    }

    string linea;
    int contador = 0;

    while (getline(archivo, linea) && contador < maxPublicidades) {
        if (linea.empty()) continue;

        size_t p1 = linea.find(';');
        size_t p2 = linea.find(';', p1 + 1);
        if (p1 == string::npos || p2 == string::npos) continue;

        string tipo = linea.substr(0, p1);
        int duracion = stoi(linea.substr(p1 + 1, p2 - p1 - 1));
        string mensaje = linea.substr(p2 + 1);

        lista[contador++] = new Publicidad(tipo.c_str(), duracion, mensaje.c_str());
    }

    archivo.close();
    cout << " Se cargaron " << contador << " publicidades desde el archivo.\n";
    return contador;
}

