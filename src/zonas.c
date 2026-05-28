#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "atracciones.h"
#include "zonas.h"

static char *copiar_texto_zona(const char *texto) {
    char *copia;
    size_t largo;

    if (texto == NULL) {
        texto = "";
    }

    largo = strlen(texto) + 1;
    copia = (char *) malloc(largo * sizeof(char));

    if (copia == NULL) {
        return NULL;
    }

    strcpy(copia, texto);
    return copia;
}

struct Zona crear_zona(char *nombre, int id, char *tematica,
                       struct Tiempo apertura, struct Tiempo cierre,
                       int cap_max, int atracciones_max) {
    struct Zona nueva_zona;

    nueva_zona.id = id;
    nueva_zona.nombre = copiar_texto_zona(nombre);
    nueva_zona.tematica = copiar_texto_zona(tematica);
    nueva_zona.cap_max = cap_max;
    nueva_zona.visitantes_actuales = 0;
    nueva_zona.hora_apertura = apertura;
    nueva_zona.hora_cierre = cierre;
    nueva_zona.atracciones_max = atracciones_max;
    nueva_zona.head_atracciones = NULL;

    if (nueva_zona.cap_max < 0) {
        nueva_zona.cap_max = 0;
    }

    if (nueva_zona.atracciones_max < 0) {
        nueva_zona.atracciones_max = 0;
    }

    return nueva_zona;
}

struct Zona crear_Zona(char *nombre, int codigo, char *tematica,
                       struct Tiempo *apertura, struct Tiempo *cierre,
                       int atracciones_max) {
    struct Tiempo hora_apertura;
    struct Tiempo hora_cierre;

    hora_apertura.hora = 0;
    hora_apertura.minutos = 0;
    hora_cierre.hora = 0;
    hora_cierre.minutos = 0;

    if (apertura != NULL) {
        hora_apertura = *apertura;
    }

    if (cierre != NULL) {
        hora_cierre = *cierre;
    }

    return crear_zona(nombre, codigo, tematica, hora_apertura, hora_cierre, 0, atracciones_max);
}

void eliminar_zona(struct Zona *z) {
    struct NodoAtraccion *actual;
    struct NodoAtraccion *temp;

    if (z == NULL) {
        return;
    }

    actual = z->head_atracciones;

    while (actual != NULL) {
        temp = actual;
        actual = actual->sig;

        liberar_datos_atraccion(temp->datos);
        free(temp);
    }

    free(z->nombre);
    free(z->tematica);

    z->nombre = NULL;
    z->tematica = NULL;
    z->visitantes_actuales = 0;
    z->head_atracciones = NULL;
}

void eliminar_Zona(struct Zona *z) {
    eliminar_zona(z);
}

struct Zona *buscar_Zona(struct NodoZonas *head, int id) {
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

void modificar_Zona(struct Zona *z) {
    int opcion;
    char nuevo[100];
    char *copia;

    if (z == NULL) {
        printf("Zona invalida.\n");
        return -1;
    }

    printf("Que desea modificar?\n");
    printf("1. Nombre\n2. Tematica\n3. Capacidad maxima\n");
    printf("4. Hora apertura\n5. Hora cierre\n6. Max. atracciones\n");
    printf("Opcion: ");

    if (scanf("%d", &opcion) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    switch (opcion) {
        case 1:
            printf("Nuevo nombre: ");
            scanf(" %99[^\n]", nuevo);

            copia = copiar_texto_zona(nuevo);
            if (copia == NULL) {
                printf("No se pudo actualizar el nombre.\n");
                return;
            }

            free(z->nombre);
            z->nombre = copia;
            break;

        case 2:
            printf("Nueva tematica: ");
            scanf(" %99[^\n]", nuevo);

            copia = copiar_texto_zona(nuevo);
            if (copia == NULL) {
                printf("No se pudo actualizar la tematica.\n");
                return;
            }

            free(z->tematica);
            z->tematica = copia;
            break;

        case 3:
            printf("Nueva capacidad maxima: ");
            scanf("%d", &z->cap_max);
            if (z->cap_max < 0) {
                z->cap_max = 0;
            }
            break;

        case 4:
            printf("Nueva hora apertura (HH MM): ");
            scanf("%d %d", &z->hora_apertura.hora, &z->hora_apertura.minutos);
            break;

        case 5:
            printf("Nueva hora cierre (HH MM): ");
            scanf("%d %d", &z->hora_cierre.hora, &z->hora_cierre.minutos);
            break;

        case 6:
            printf("Nuevo max. atracciones: ");
            scanf("%d", &z->atracciones_max);
            if (z->atracciones_max < 0) {
                z->atracciones_max = 0;
            }
            break;

        default:
            printf("Opcion invalida.\n");
            return;
    }

    printf("Zona actualizada.\n");
}

void listar_zona(struct Zona *z) {
    int cantidad_atracciones;

    if (z == NULL) {
        printf("Zona invalida.\n");
        return;
    }

    cantidad_atracciones = contar_Atracciones(z);

    printf("ID: %d\n", z->id);
    printf("Nombre: %s\n", z->nombre);
    printf("Tematica: %s\n", z->tematica);
    printf("Capacidad maxima: %d\n", z->cap_max);
    printf("Visitantes actuales: %d\n", z->visitantes_actuales);
    printf("Horario: %02d:%02d - %02d:%02d\n",
           z->hora_apertura.hora, z->hora_apertura.minutos,
           z->hora_cierre.hora, z->hora_cierre.minutos);
    printf("Atracciones: %d/%d\n", cantidad_atracciones, z->atracciones_max);
}

void listar_zonas(struct NodoZonas *head) {
    struct NodoZonas *actual;
    int numero;

    actual = head;
    numero = 1;

    if (actual == NULL) {
        printf("No hay zonas registradas.\n");
        return;
    }

    while (actual != NULL) {
        printf("\nZona %d\n", numero);
        printf("--------------------\n");
        listar_zona(actual->datos);

        actual = actual->sig;
        numero++;
    }
}
