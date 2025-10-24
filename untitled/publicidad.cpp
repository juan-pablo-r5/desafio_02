#include "publicidad.h"


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
    cout << "Tipo: " << tipo << " | Duracion: " << duracion << " seg\n";
    cout << "Mensaje: " << mensaje << "\n";
}

