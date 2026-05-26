#include studio.h>
#include "structs.h"


int agregar_atraccion (struct Zona *z, struct Atraccion *a){
    if contar_Atracciones(z) >= z->atracciones_max {
        return 0; // No se puede agregar más atracciones
    }
        struct NodoAtraccion *nuevo = malloc(sizeof(struct NodoAtraccion));
        nuevo->datos = a;
        nuevo->sig = NULL;

        if(z->headAtracciones == NULL) {
        z->headAtracciones = nuevo;
    } else {
        struct NodoAtraccion *actual = z->head_Atracciones;
        while (actual->sig) actual = actual->sig;
        actual->sig = nuevo;
    }
    return 1;
};


int contar_Atracciones(struct Zona *z) {
    int cont = 0;
    struct NodoAtraccion *actual = z->head_Atracciones;
    while (actual){
         cont++; actual = actual->sig; 
        }
    return cont;
    };


struct Atraccion *buscar_Atraccion_Por_Nombre(struct Zona *z, char *nombre) {
    if (!z || !nombre) return NULL;

    struct NodoAtraccion *actual = z->head_Atracciones;
    while (actual) {
        if (strcmp(actual->datos->nombre, nombre) == 0)
            return actual->datos;
        actual = actual->sig;
    }
    return NULL;
}

struct Atraccion *buscar_Atraccion_Por_Estado(struct Zona *z, char *estado) {
    if (!z || !estado) return NULL;

    struct NodoAtraccion *actual = z->head_Atracciones;
    while (actual) {
        if (strcmp(actual->datos->estado, estado) == 0)
            return actual->datos;
        actual = actual->sig;
    }
    return NULL;
}