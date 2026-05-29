#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "filas.h"

#define ATRACCION_OK    -1
#define ATRACCION_ERROR 0


int estado_atraccion_valido(const char *estado) {
    if (estado == NULL) {
        return ATRACCION_ERROR;
    }

    if (strcmp(estado, "operativa")         == 0) return ATRACCION_OK;
    if (strcmp(estado, "mantenimiento")     == 0) return ATRACCION_OK;
    if (strcmp(estado, "cerrada")           == 0) return ATRACCION_OK;
    if (strcmp(estado, "fuera_de_servicio") == 0) return ATRACCION_OK;

    return ATRACCION_ERROR;
}

int atraccion_operativa(struct Atraccion *a) {
    if (a == NULL) {
        return ATRACCION_ERROR;
    }

    return strcmp(a->estado, "operativa") == 0;
}

void liberar_datos_atraccion(struct Atraccion *a) {
    if (a == NULL) {
        return;
    }

    vaciar_fila(&a->cola_general);
    vaciar_fila(&a->cola_prioritaria);

    free(a->nombre);
    free(a->estado);
    free(a->tematica);
    free(a);
}

struct Atraccion* buscar_atraccion_en_todas_las_zonas(struct NodoZonas *head_zonas, int id) {
    struct NodoZonas *zona_actual = head_zonas;
    struct NodoAtraccion *atraccion_actual;

    while (zona_actual != NULL) {
        atraccion_actual = zona_actual->datos->head_atracciones;

        while (atraccion_actual != NULL) {
            if (atraccion_actual->datos != NULL) {
                if (atraccion_actual->datos->id == id) {
                    return atraccion_actual->datos;
                }
            }
            atraccion_actual = atraccion_actual->sig;
        }

        zona_actual = zona_actual->sig;
    }

    return NULL;
}

struct Atraccion *crear_atraccion(char *nombre, char *estado, char *tematica,
                                  int duracion, int edad_min, float altura_min,
                                  int cap_max) {
    struct Atraccion *a;

    if (!estado_atraccion_valido(estado)) {
        return NULL;
    }

    a = (struct Atraccion *) malloc(sizeof(struct Atraccion));
    if (a == NULL) {
        return NULL;
    }

    a->nombre   = copiar_texto(nombre);
    a->estado   = copiar_texto(estado);
    a->tematica = copiar_texto(tematica);

    if (a->nombre == NULL || a->estado == NULL || a->tematica == NULL) {
        free(a->nombre);
        free(a->estado);
        free(a->tematica);
        free(a);
        return NULL;
    }

    a->duracion          = duracion;
    a->edad_min          = edad_min;
    a->altura_min        = altura_min;
    a->cap_max           = cap_max;
    a->visitantes_totales = 0;

    a->pico_cola_general = 0;
    a->pico_cola_prioritaria = 0;

    inicializar_fila(&a->cola_general);
    inicializar_fila(&a->cola_prioritaria);

    return a;
}

int agregar_atraccion(struct Zona *z, struct Atraccion *a) {
    struct NodoAtraccion *nuevo;
    struct NodoAtraccion *actual;

    if (z == NULL || a == NULL) {
        return ATRACCION_ERROR;
    }

    if (contar_atracciones(z) >= z->atracciones_max) {
        return ATRACCION_ERROR;
    }

    nuevo = (struct NodoAtraccion *) malloc(sizeof(struct NodoAtraccion));
    if (nuevo == NULL) {
        return ATRACCION_ERROR;
    }

    nuevo->datos = a;
    nuevo->sig   = NULL;

    if (z->head_atracciones == NULL) {
        z->head_atracciones = nuevo;
    } else {
        actual = z->head_atracciones;
        while (actual->sig != NULL) {
            actual = actual->sig;
        }
        actual->sig = nuevo;
    }

    return ATRACCION_OK;
}

void eliminar_atraccion(struct Zona *z, char *nombre) {
    struct NodoAtraccion *actual;
    struct NodoAtraccion *previo;

    if (z == NULL || nombre == NULL) {
        return;
    }

    actual = z->head_atracciones;
    previo = NULL;

    while (actual != NULL) {
        if (actual->datos != NULL && strcmp(actual->datos->nombre, nombre) == 0) {
            if (previo == NULL) {
                z->head_atracciones = actual->sig;
            } else {
                previo->sig = actual->sig;
            }

            liberar_datos_atraccion(actual->datos);
            free(actual);
            return;
        }

        previo = actual;
        actual = actual->sig;
    }
}

struct Atraccion *buscar_atraccion_por_nombre(struct Zona *z, char *nombre) {
    struct NodoAtraccion *actual;

    if (z == NULL || nombre == NULL) {
        return NULL;
    }

    actual = z->head_atracciones;

    while (actual != NULL) {
        if (actual->datos != NULL && strcmp(actual->datos->nombre, nombre) == 0) {
            return actual->datos;
        }
        actual = actual->sig;
    }

    return NULL;
}

struct NodoAtraccion *obtener_atracciones_zona(const struct Zona *z) {
    if (z == NULL) return NULL;
    return z->head_atracciones;
}

int contar_atracciones(struct Zona *z) {
    struct NodoAtraccion *actual;
    int cont;

    if (z == NULL) {
        return 0;
    }

    cont   = 0;
    actual = z->head_atracciones;

    while (actual != NULL) {
        cont++;
        actual = actual->sig;
    }

    return cont;
}

int cambiar_estado_atraccion(struct Atraccion *a, const char *nuevo_estado) {
    char *copia;

    if (a == NULL || nuevo_estado == NULL) {
        return ATRACCION_ERROR;
    }

    if (!estado_atraccion_valido(nuevo_estado)) {
        return ATRACCION_ERROR;
    }

    copia = copiar_texto(nuevo_estado);
    if (copia == NULL) {
        return ATRACCION_ERROR;
    }

    free(a->estado);
    a->estado = copia;

    if (!atraccion_operativa(a)) {
        suspender_filas_atraccion(a, 0);
    }

    return ATRACCION_OK;
}

int cambiar_nombre_atraccion(struct Atraccion *a, const char *nuevo_nombre) {
    char *copia;

    if (a == NULL || nuevo_nombre == NULL || nuevo_nombre[0] == '\0') return ATRACCION_ERROR;

    copia = copiar_texto(nuevo_nombre);
    if (copia == NULL) return ATRACCION_ERROR;

    free(a->nombre);
    a->nombre = copia;
    return ATRACCION_OK;
}

int cambiar_tematica_atraccion(struct Atraccion *a, const char *nueva_tematica) {
    char *copia;

    if (a == NULL || nueva_tematica == NULL || nueva_tematica[0] == '\0') return ATRACCION_ERROR;

    copia = copiar_texto(nueva_tematica);
    if (copia == NULL) return ATRACCION_ERROR;

    free(a->tematica);
    a->tematica = copia;
    return ATRACCION_OK;
}

int cambiar_duracion_atraccion(struct Atraccion *a, int nueva_duracion) {
    if (a == NULL || nueva_duracion <= 0) return ATRACCION_ERROR;
    a->duracion = nueva_duracion;
    return ATRACCION_OK;
}

int cambiar_edad_minima_atraccion(struct Atraccion *a, int nueva_edad) {
    if (a == NULL || nueva_edad < 0) return ATRACCION_ERROR;
    a->edad_min = nueva_edad;
    return ATRACCION_OK;
}

int cambiar_altura_minima_atraccion(struct Atraccion *a, float nueva_altura) {
    if (a == NULL || nueva_altura < 0.0f) return ATRACCION_ERROR;
    a->altura_min = nueva_altura;
    return ATRACCION_OK;
}

int cambiar_capacidad_atraccion(struct Atraccion *a, int nueva_cap) {
    if (a == NULL || nueva_cap <= 0) return ATRACCION_ERROR;
    a->cap_max = nueva_cap;
    return ATRACCION_OK;
}

void actualizar_pico_cola_general(struct Atraccion *a, int tam_actual) {
    if (a == NULL) return;
    if (tam_actual > a->pico_cola_general)
        a->pico_cola_general = tam_actual;
}

void actualizar_pico_cola_prioritaria(struct Atraccion *a, int tam_actual) {
    if (a == NULL) return;
    if (tam_actual > a->pico_cola_prioritaria)
        a->pico_cola_prioritaria = tam_actual;
}
