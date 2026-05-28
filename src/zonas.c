#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "atracciones.h"
#include "zonas.h"

struct Zona *crear_zona(int id, char *nombre, char *tematica,
                        struct Tiempo apertura, struct Tiempo cierre,
                        int cap_max, int atracciones_max) {
    struct Zona *z;

    z = (struct Zona *) malloc(sizeof(struct Zona));
    if (z == NULL) {
        return NULL;
    }

    z->id                  = id;
    z->nombre              = copiar_texto(nombre);
    z->tematica            = copiar_texto(tematica);
    z->cap_max             = cap_max;
    z->visitantes_actuales = 0;
    z->hora_apertura       = apertura;
    z->hora_cierre         = cierre;
    z->atracciones_max     = atracciones_max;
    z->head_atracciones    = NULL;

    if (z->nombre == NULL || z->tematica == NULL) {
        free(z->nombre);
        free(z->tematica);
        free(z);
        return NULL;
    }

    return z;
}

void eliminar_zona(struct Zona *z) {
    struct NodoAtraccion *actual;
    struct NodoAtraccion *temp;

    if (z == NULL) {
        return;
    }

    actual = z->head_atracciones;

    while (actual != NULL) {
        temp   = actual;
        actual = actual->sig;

        liberar_datos_atraccion(temp->datos);
        free(temp);
    }

    free(z->nombre);
    free(z->tematica);
    free(z);
}
struct Zona *buscar_zona(struct NodoZonas *head, int id) {
    struct NodoZonas *actual;

    actual = head;

    while (actual != NULL) {
        if (actual->datos != NULL && actual->datos->id == id) {
            return actual->datos;
        }

        actual = actual->sig;
    }

    return NULL;
}

int cambiar_nombre_zona(struct Zona *z, const char *nuevo_nombre) {
    char *copia;

    if (z == NULL || nuevo_nombre == NULL || nuevo_nombre[0] == '\0') return 0;

    copia = copiar_texto(nuevo_nombre);
    if (copia == NULL) return 0;

    free(z->nombre);
    z->nombre = copia;
    return 1;
}

int cambiar_tematica_zona(struct Zona *z, const char *nueva_tematica) {
    char *copia;

    if (z == NULL || nueva_tematica == NULL || nueva_tematica[0] == '\0') return 0;

    copia = copiar_texto(nueva_tematica);
    if (copia == NULL) return 0;

    free(z->tematica);
    z->tematica = copia;
    return 1;
}

int cambiar_cap_max_zona(struct Zona *z, int nueva_cap) {
    if (z == NULL || nueva_cap < 0) return 0;
    z->cap_max = nueva_cap;
    return 1;
}

int cambiar_horario_zona(struct Zona *z, struct Tiempo apertura, struct Tiempo cierre) {
    if (z == NULL) return 0;
    z->hora_apertura = apertura;
    z->hora_cierre   = cierre;
    return 1;
}

int cambiar_max_atracciones_zona(struct Zona *z, int nuevo_max) {
    if (z == NULL || nuevo_max < 0) return 0;
    z->atracciones_max = nuevo_max;
    return 1;
}
