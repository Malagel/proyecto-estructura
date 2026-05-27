#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

/* ═══════════════════════════════════════════════════════════════════
 * Función    : contarGrupos
 * Descripción: Cuenta la cantidad de grupos (NodoFila) en una fila.
 *              Auxiliar usada por listarAtracciones y tiempoEspera.
 * Retorno    : int -> número de grupos en la fila
 * ═══════════════════════════════════════════════════════════════════ */
int contarGrupos(struct Fila *f) {
    int cont = 0;
    struct NodoFila *actual = f->frente;
    while (actual) {
        cont++;
        actual = actual->sig;
    }
    return cont;
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : contar_Atracciones
 * Descripción: Cuenta cuántas atracciones tiene actualmente una zona.
 *              Auxiliar usada por agregar_atraccion.
 * Retorno    : int -> número de atracciones en la lista
 * ═══════════════════════════════════════════════════════════════════ */
int contar_Atracciones(struct Zona *z) {
    int cont = 0;
    struct NodoAtraccion *actual = z->head_atracciones;
    while (actual) {
        cont++;
        actual = actual->sig;
    }
    return cont;
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : crearAtraccion
 * Descripción: Reserva memoria para una nueva atracción, inicializa
 *              todos sus campos y deja las filas vacías.
 * Retorno    : struct Atraccion* -> puntero a la nueva atracción,
 *              NULL si falla el malloc
 * ═══════════════════════════════════════════════════════════════════ */
struct Atraccion *crearAtraccion(char *nombre, char *estado, char *tematica,
                                  int duracion, int edad_min, float altura_min,
                                  int cap_max, int max_cola_gral,
                                  int max_cola_prior) {
    struct Atraccion *a = malloc(sizeof(struct Atraccion));
    if (!a) { perror("Error al crear atraccion"); return NULL; }

    a->nombre               = strdup(nombre);
    a->estado               = strdup(estado);
    a->tematica             = strdup(tematica);
    a->duracion             = duracion;
    a->edad_min             = edad_min;
    a->altura_min           = altura_min;
    a->cap_max              = cap_max;
    a->visitantes_totales   = 0;
    a->max_cola_general     = max_cola_gral;
    a->max_cola_prioritaria = max_cola_prior;

    /* Inicializar filas vacías */
    a->cola_general.frente     = NULL;
    a->cola_general.final      = NULL;
    a->cola_prioritaria.frente = NULL;
    a->cola_prioritaria.final  = NULL;

    return a;
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : agregar_atraccion
 * Descripción: Agrega una atracción al final de la lista enlazada
 *              de una zona, respetando el límite atracciones_max.
 * Retorno    : 1 si se agregó correctamente, 0 si la zona está llena
 * ═══════════════════════════════════════════════════════════════════ */
int agregar_atraccion(struct Zona *z, struct Atraccion *a) {

    /* Verificar que haya espacio en la zona */
    if (contar_Atracciones(z) >= z->atracciones_max) {
        printf("Zona '%s' llena (máx %d).\n", z->nombre, z->atracciones_max);
        return 0;
    }

    /* Crear nodo contenedor */
    struct NodoAtraccion *nuevo = malloc(sizeof(struct NodoAtraccion));
    if (!nuevo) { perror("Error al crear nodo atraccion"); return 0; }
    nuevo->datos = a;
    nuevo->sig   = NULL;

    /* Insertar al final de la lista */
    if (z->head_atracciones == NULL) {
        /* Lista vacía: el nuevo nodo es la cabeza */
        z->head_atracciones = nuevo;
    } else {
        /* Recorrer hasta el último nodo */
        struct NodoAtraccion *actual = z->head_atracciones;
        while (actual->sig) actual = actual->sig;
        actual->sig = nuevo;
    }
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : eliminarAtraccion
 * Descripción: Busca una atracción por nombre, la desenlaza de la
 *              lista y libera toda su memoria (filas, nodos, datos).
 * Retorno    : void
 * ═══════════════════════════════════════════════════════════════════ */
void eliminarAtraccion(struct Zona *z, char *nombre) {

    /* Validar entradas */
    if (!z || !nombre) {
        printf("Error: zona o nombre inválido.\n");
        return;
    }

    if (!z->head_atracciones) {
        printf("La zona '%s' no tiene atracciones.\n", z->nombre);
        return;
    }

    struct NodoAtraccion *actual = z->head_atracciones;
    struct NodoAtraccion *previo = NULL;

    /* Recorrer la lista buscando por nombre */
    while (actual) {
        if (strcmp(actual->datos->nombre, nombre) == 0) {

            /* Desenlazar: distinguir si es cabeza o nodo intermedio */
            if (previo == NULL)
                z->head_atracciones = actual->sig;
            else
                previo->sig = actual->sig;

            /* Liberar nodos de cola general */
            struct NodoFila *nf = actual->datos->cola_general.frente;
            while (nf) {
                struct NodoFila *temp = nf;
                nf = nf->sig;
                free(temp);
            }
            actual->datos->cola_general.frente = NULL;
            actual->datos->cola_general.final  = NULL;

            /* Liberar nodos de cola prioritaria */
            nf = actual->datos->cola_prioritaria.frente;
            while (nf) {
                struct NodoFila *temp = nf;
                nf = nf->sig;
                free(temp);
            }
            actual->datos->cola_prioritaria.frente = NULL;
            actual->datos->cola_prioritaria.final  = NULL;

            /* Liberar campos dinámicos y struct */
            free(actual->datos->nombre);
            free(actual->datos->estado);
            free(actual->datos->tematica);
            free(actual->datos);
            free(actual);

            printf("Atracción '%s' eliminada correctamente.\n", nombre);
            return;
        }
        previo = actual;
        actual = actual->sig;
    }

    printf("Atracción '%s' no encontrada en zona '%s'.\n", nombre, z->nombre);
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : modificarAtraccion
 * Descripción: Menú interactivo para modificar un campo específico
 *              de una atracción. Libera memoria anterior en campos
 *              de tipo char* antes de asignar el nuevo valor.
 * Retorno    : void
 * ═══════════════════════════════════════════════════════════════════ */
void modificarAtraccion(struct Atraccion *a) {

    if (!a) { printf("Error: atracción inválida.\n"); return; }

    int opcion;
    printf("\n¿Qué desea modificar?\n");
    printf("1. Nombre\n2. Estado\n3. Temática\n4. Duración (min)\n");
    printf("5. Edad mínima\n6. Altura mínima\n7. Capacidad por ciclo\n");
    printf("8. Máx. grupos cola general\n9. Máx. grupos cola prioritaria\n");
    printf("Opción: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1: {
            char nuevo[100];
            printf("Nuevo nombre: ");
            scanf(" %99[^\n]", nuevo);
            free(a->nombre);
            a->nombre = strdup(nuevo);
            break;
        }
        case 2: {
            char nuevo[30];
            printf("Estados: operativa / mantenimiento / cerrada / fuera_de_servicio\n");
            printf("Nuevo estado: ");
            scanf(" %29[^\n]", nuevo);
            if (strcmp(nuevo, "operativa")        != 0 &&
                strcmp(nuevo, "mantenimiento")     != 0 &&
                strcmp(nuevo, "cerrada")           != 0 &&
                strcmp(nuevo, "fuera_de_servicio") != 0) {
                printf("Estado inválido. No se realizaron cambios.\n");
                return;
            }
            free(a->estado);
            a->estado = strdup(nuevo);
            break;
        }
        case 3: {
            char nuevo[100];
            printf("Nueva temática: ");
            scanf(" %99[^\n]", nuevo);
            free(a->tematica);
            a->tematica = strdup(nuevo);
            break;
        }
        case 4:
            printf("Nueva duración (min): ");
            scanf("%d", &a->duracion);
            if (a->duracion <= 0) { printf("Inválida, se asigna 1.\n"); a->duracion = 1; }
            break;
        case 5:
            printf("Nueva edad mínima: ");
            scanf("%d", &a->edad_min);
            break;
        case 6:
            printf("Nueva altura mínima (m): ");
            scanf("%f", &a->altura_min);
            break;
        case 7:
            printf("Nueva capacidad por ciclo: ");
            scanf("%d", &a->cap_max);
            if (a->cap_max <= 0) { printf("Inválida, se asigna 1.\n"); a->cap_max = 1; }
            break;
        case 8:
            printf("Nuevo máx. cola general: ");
            scanf("%d", &a->max_cola_general);
            break;
        case 9:
            printf("Nuevo máx. cola prioritaria: ");
            scanf("%d", &a->max_cola_prioritaria);
            break;
        default:
            printf("Opción inválida.\n");
            return;
    }
    printf("Atracción actualizada correctamente.\n");
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : listarAtracciones
 * Descripción: Imprime los datos de todas las atracciones de una
 *              zona, incluyendo estado, restricciones y grupos en
 *              espera en ambas colas.
 * Retorno    : void
 * ═══════════════════════════════════════════════════════════════════ */
void listarAtracciones(struct Zona *z) {

    if (!z) { printf("Error: zona inválida.\n"); return; }

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
        int espera = contarGrupos(&a->cola_general)
                   + contarGrupos(&a->cola_prioritaria);

        printf("[%d] %-22s | Estado : %-20s\n",  i++, a->nombre, a->estado);
        printf("    Temática  : %-20s | Duración : %d min\n",  a->tematica, a->duracion);
        printf("    Edad mín  : %d años       | Altura mín: %.2f m\n", a->edad_min, a->altura_min);
        printf("    Cap/ciclo : %-5d          | Visitantes: %d\n", a->cap_max, a->visitantes_totales);
        printf("    En espera : %d grupos (gral: %d | prior: %d)\n",
               espera,
               contarGrupos(&a->cola_general),
               contarGrupos(&a->cola_prioritaria));
        printf("──────────────────────────────────────────────────\n");

        actual = actual->sig;
    }
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : buscar_Atraccion_Por_Nombre
 * Descripción: Recorre la lista de atracciones de una zona buscando
 *              coincidencia exacta con el nombre dado.
 * Retorno    : struct Atraccion* -> atracción encontrada, NULL si no
 * ═══════════════════════════════════════════════════════════════════ */
struct Atraccion *buscar_Atraccion_Por_Nombre(struct Zona *z, char *nombre) {
    if (!z || !nombre) return NULL;

    struct NodoAtraccion *actual = z->head_atracciones;
    while (actual) {
        if (strcmp(actual->datos->nombre, nombre) == 0)
            return actual->datos;
        actual = actual->sig;
    }
    return NULL;
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : buscar_Atraccion_Por_Estado
 * Descripción: Recorre la lista de atracciones de una zona y retorna
 *              la primera que coincida con el estado dado.
 * Retorno    : struct Atraccion* -> primera atracción encontrada,
 *              NULL si no hay coincidencia
 * ═══════════════════════════════════════════════════════════════════ */
struct Atraccion *buscar_Atraccion_Por_Estado(struct Zona *z, char *estado) {
    if (!z || !estado) return NULL;

    struct NodoAtraccion *actual = z->head_atracciones;
    while (actual) {
        if (strcmp(actual->datos->estado, estado) == 0)
            return actual->datos;
        actual = actual->sig;
    }
    return NULL;
}