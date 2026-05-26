#include <stdio.h>
#include "structs.h"


struct Zona crear_Zona (char *nombre, int codigo, char *tematica, struct tiempo *apertura, struct tiempo *cierre, int atracciones_max){
    struct Zona nuevaZona;
    nuevaZona.nombre = strdup(nombre);
    nuevaZona.codigo = codigo;
    nuevaZona.tematica = strdup(tematica);
    nuevaZona.horaApertura = apertura;
    nuevaZona.horaCierre = cierre;
    nuevaZona.atracciones_max = atracciones_max;
    nuevaZona.headAtracciones = NULL;
 
    return nuevaZona;
};


void Zona eliminar_Zona(struct Zona z){
    free(z.nombre);
    free(z.tematica);
    free(z.horaApertura);
    free(z.horaCierre);
    struct NodoAtraccion *actual = z.headAtracciones;
    while (actual){ 
        struct NodoAtraccion *temp = actual;
        actual = actual->sig;
        liberarAtraccion(temp->datos);
        free(temp);
    }
    z.headAtracciones = NULL;
};


struct Zona *buscar_Zona(struct NodoZonas *head, int id) {
    struct NodoZonas *actual = head;
    while (actual) {
        if (actual->datos->id == id)
            return actual->datos;
        actual = actual->sig;
    }
    return NULL;
}

void modificar_Zona(struct Zona *z) {
    int opcion;
    printf("¿Qué desea modificar?\n");
    printf("1. Nombre\n2. Temática\n3. Capacidad máxima\n");
    printf("4. Hora apertura\n5. Hora cierre\n6. Máx. atracciones\n");
    printf("Opción: "); scanf("%d", &opcion);

    switch (opcion) {
        case 1: {
            char nuevo[100];
            printf("Nuevo nombre: "); scanf(" %99[^\n]", nuevo);
            free(z->nombre);
            z->nombre = strdup(nuevo);
            break;
        }
        case 2: {
            char nuevo[100];
            printf("Nueva temática: "); scanf(" %99[^\n]", nuevo);
            free(z->tematica);
            z->tematica = strdup(nuevo);
            break;
        }
        case 3:
            printf("Nueva capacidad máxima: ");
            scanf("%d", &z->cap_max);
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
            printf("Nuevo máx. atracciones: ");
            scanf("%d", &z->atracciones_max);
            break;
        default:
            printf("Opción inválida.\n");
    }
    printf("Zona actualizada.\n");
};

