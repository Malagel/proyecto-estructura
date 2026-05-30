#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "atracciones.h"
#include "zonas.h"

struct Zona *obtener_zona_por_id(struct NodoZonas *head_zonas, int id_zona) {
    struct NodoZonas *actual;

    if (head_zonas == NULL) {
        return NULL;
    }

    actual = head_zonas;

    while (actual != NULL) {
        if (actual->datos != NULL && actual->datos->id == id_zona)
            return actual->datos; 
        actual = actual->sig;
    }

    return NULL;
}   