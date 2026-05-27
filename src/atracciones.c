#include <studio.h>
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


/*Funcion para eliminar Atracciones*/

void eliminarAtraccion(struct Zona *z, char *nombre) {

    if (!z || !nombre) {
        printf("Error: zona o nombre inválido.\n");
        return;
    }

    if (!z->head_atracciones) {
        printf("La zona '%s' no tiene atracciones.\n", z->nombre);
        return;
    }

    struct NodoAtraccion *actual = z->head_atracciones;
    struct NodoAtraccion *previo   = NULL;

    /* Recorrer la lista buscando la atracción por nombre */
    while (actual) {
        if (strcmp(actual->datos->nombre, nombre) == 0) {

            /* ── Desenlazar el nodo de la lista ── */
            if (previo == NULL)

                /* Era el primer nodo: mover cabeza al siguiente */
                z->head_atracciones = actual->sig;
            else
                /* Nodo intermedio o final: saltar el nodo actual */
                previo->sig = actual->sig;

            struct NodoFila *nf = actual->datos->cola_general.frente;
            while (nf) {
                struct NodoFila *temp = nf;
                nf = nf->sig;
                free(temp);
            }
            actual->datos->cola_general.frente = NULL;
            actual->datos->cola_general.final  = NULL;

            nf = actual->datos->cola_prioritaria.frente;
            while (nf) {
                struct NodoFila *temp = nf;
                nf = nf->sig;
                free(temp);
            }
            actual->datos->cola_prioritaria.frente = NULL;
            actual->datos->cola_prioritaria.final  = NULL;

            /* ── Liberar campos dinámicos ── */
            free(actual->datos->nombre);
            free(actual->datos->estado);
            free(actual->datos->tematica);

            free(actual->datos);
            free(actual);

            printf("Atracción '%s' eliminada correctamente.\n", nombre);
            return;
        }

        previo   = actual;
        actual = actual->sig;
    }

    printf("Atracción '%s' no encontrada en la zona '%s'.\n",
           nombre, z->nombre);
}


void modificarAtraccion(struct Atraccion *a) {

    if (!a) {
        printf("Error: atracción inválida.\n");
        return;
    }

    int opcion;
    printf("\n¿Qué desea modificar?\n");
    printf("1. Nombre\n");
    printf("2. Estado\n");
    printf("3. Temática\n");
    printf("4. Duración (min)\n");
    printf("5. Edad mínima\n");
    printf("6. Altura mínima\n");
    printf("7. Capacidad por ciclo\n");
    printf("8. Máx. grupos cola general\n");
    printf("9. Máx. grupos cola prioritaria\n");
    printf("Opción: ");
    scanf("%d", &opcion);

    switch (opcion) {

        case 1: {
            /* Liberar nombre anterior y asignar el nuevo */
            char nuevo[100];
            printf("Nuevo nombre: ");
            scanf(" %99[^\n]", nuevo);
            free(a->nombre);
            a->nombre = strdup(nuevo);
            break;
        }

        case 2: {
            /* Validar que el estado sea uno de los permitidos */
            char nuevo[30];
            printf("Estados válidos: operativa / mantenimiento / "
                   "cerrada / fuera_de_servicio\n");
            printf("Nuevo estado: ");
            scanf(" %29[^\n]", nuevo);

            /* Verificar que el estado ingresado sea válido */
            if (strcmp(nuevo, "operativa")          != 0 &&
                strcmp(nuevo, "mantenimiento")       != 0 &&
                strcmp(nuevo, "cerrada")             != 0 &&
                strcmp(nuevo, "fuera_de_servicio")   != 0) {
                printf("Estado inválido. No se realizaron cambios.\n");
                return;
            }
            free(a->estado);
            a->estado = strdup(nuevo);
            break;
        }

        case 3: {
            /* Liberar temática anterior y asignar la nueva */
            char nuevo[100];
            printf("Nueva temática: ");
            scanf(" %99[^\n]", nuevo);
            free(a->tematica);
            a->tematica = strdup(nuevo);
            break;
        }

        case 4:
            /* Duración en minutos, debe ser positiva */
            printf("Nueva duración (min): ");
            scanf("%d", &a->duracion);
            if (a->duracion <= 0) {
                printf("Duración inválida.\n");
                a->duracion = 1;
            }
            break;

        case 5:
            /* Edad mínima, debe ser positiva */
            printf("Nueva edad mínima: ");
            scanf("%d", &a->edad_min);
            break;

        case 6:
            /* Altura mínima en metros */
            printf("Nueva altura mínima (m): ");
            scanf("%f", &a->altura_min);
            break;

        case 7:
            /* Capacidad por ciclo, debe ser mayor a 0 */
            printf("Nueva capacidad por ciclo: ");
            scanf("%d", &a->cap_max);
            if (a->cap_max <= 0) {
                printf("Capacidad inválida.\n");
                a->cap_max = 1;
            }
            break;

        case 8:
            /* Límite de grupos en cola general */
            printf("Nuevo máx. cola general: ");
            scanf("%d", &a->max_cola_general);
            break;

        case 9:
            /* Límite de grupos en cola prioritaria */
            printf("Nuevo máx. cola prioritaria: ");
            scanf("%d", &a->max_cola_prioritaria);
            break;

        default:
            printf("Opción inválida.\n");
            return;
    }

    printf("Atracción actualizada correctamente.\n");
}



void listarAtracciones(struct Zona *z) {

    /* Validar zona */
    if (!z) {
        printf("Error: zona inválida.\n");
        return;
    }

    if (!z->head_atracciones) {
        printf("La zona '%s' no tiene atracciones registradas.\n", z->nombre);
        return;
    }

    printf("\n══════════════════════════════════════════════════════\n");
    printf(" Atracciones de la zona: %s\n", z->nombre);
    printf("══════════════════════════════════════════════════════\n");

    struct NodoAtraccion *actual = z->head_atracciones;
    int i = 1;

    while (actual) {
        struct Atraccion *a = actual->datos;

        /* Calcular grupos totales en espera */
        int espera = contarGrupos(&a->cola_general) + contarGrupos(&a->cola_prioritaria);

        printf("[%d] %-22s | Estado : %-20s\n", i++, a->nombre, a->estado);
        printf("    Temática  : %-20s | Duración : %d min\n",
               a->tematica, a->duracion);
        printf("    Edad mín  : %d años       | Altura mín: %.2f m\n",
               a->edad_min, a->altura_min);
        printf("    Cap/ciclo : %-5d          | Visitantes: %d\n",
               a->cap_max, a->visitantes_totales);
        printf("    En espera : %d grupos (gral: %d | prior: %d)\n",
               espera,
               contarGrupos(&a->cola_general),
               contarGrupos(&a->cola_prioritaria));
        printf("──────────────────────────────────────────────────\n");

        actual = actual->sig;
    }
}



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