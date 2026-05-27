#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "filas.h"

#define ATRACCION_OK 1
#define ATRACCION_ERROR 0


 char *copiar_texto(const char *texto) {
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

 int estado_atraccion_valido(const char *estado) {
    if (estado == NULL) {
        return ATRACCION_ERROR;
    }

    if (strcmp(estado, "operativa") == 0) {
        return ATRACCION_OK;
    }

    if (strcmp(estado, "mantenimiento") == 0) {
        return ATRACCION_OK;
    }

    if (strcmp(estado, "cerrada") == 0) {
        return ATRACCION_OK;
    }

    if (strcmp(estado, "fuera_de_servicio") == 0) {
        return ATRACCION_OK;
    }

    return ATRACCION_ERROR;
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

/* ═══════════════════════════════════════════════════════════════════
 * Función    : contarGrupos
 * Descripción: Cuenta la cantidad de grupos (NodoFila) en una fila.
 *              Auxiliar usada por listarAtracciones y tiempoEspera.
 * Retorno    : int -> número de grupos en la fila
 * ═══════════════════════════════════════════════════════════════════ */

int contarGrupos(struct Fila *f) {
    return contar_grupos_fila(f);
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : contar_Atracciones
 * Descripción: Cuenta cuántas atracciones tiene actualmente una zona.
 *              Auxiliar usada por agregar_atraccion.
 * Retorno    : int -> número de atracciones en la lista
 * ═══════════════════════════════════════════════════════════════════ */

int contar_Atracciones(struct Zona *z) {
    int cont;
    struct NodoAtraccion *actual;

    if (z == NULL) {
        return 0;
    }

    cont = 0;
    actual = z->head_atracciones;

    while (actual != NULL) {
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
    struct Atraccion *a;

    if (!estado_atraccion_valido(estado)) {
        printf("Estado invalido. Se usara 'operativa'.\n");
        estado = "operativa";
    }

    a = (struct Atraccion *) malloc(sizeof(struct Atraccion));
    if (a == NULL) {
        perror("Error al crear atraccion");
        return NULL;
    }

    a->nombre = copiar_texto(nombre);
    a->estado = copiar_texto(estado);
    a->tematica = copiar_texto(tematica);

    if (a->nombre == NULL || a->estado == NULL || a->tematica == NULL) {
        free(a->nombre);
        free(a->estado);
        free(a->tematica);
        free(a);
        return NULL;
    }

    if (duracion <= 0) {
        duracion = 1;
    }

    if (cap_max <= 0) {
        cap_max = 1;
    }

    if (max_cola_gral < 0) {
        max_cola_gral = 0;
    }

    if (max_cola_prior < 0) {
        max_cola_prior = 0;
    }

    a->duracion = duracion;
    a->edad_min = edad_min;
    a->altura_min = altura_min;
    a->cap_max = cap_max;
    a->visitantes_totales = 0;
    a->max_cola_general = max_cola_gral;
    a->max_cola_prioritaria = max_cola_prior;

    inicializar_fila(&a->cola_general);
    inicializar_fila(&a->cola_prioritaria);

    return a;
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : agregar_atraccion
 * Descripción: Agrega una atracción al final de la lista enlazada
 *              de una zona, respetando el límite atracciones_max.
 * Retorno    : 1 si se agregó correctamente, 0 si la zona está llena
 * ═══════════════════════════════════════════════════════════════════ */

int agregar_atraccion(struct Zona *z, struct Atraccion *a) {
    struct NodoAtraccion *nuevo;
    struct NodoAtraccion *actual;

    if (z == NULL || a == NULL) {
        printf("Error: zona o atraccion invalida.\n");
        return ATRACCION_ERROR;
    }

    if (contar_Atracciones(z) >= z->atracciones_max) {
        printf("Zona '%s' llena (max %d).\n", z->nombre, z->atracciones_max);
        return ATRACCION_ERROR;
    }

    nuevo = (struct NodoAtraccion *) malloc(sizeof(struct NodoAtraccion));
    if (nuevo == NULL) {
        perror("Error al crear nodo atraccion");
        return ATRACCION_ERROR;
    }

    nuevo->datos = a;
    nuevo->sig = NULL;

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

/* ═══════════════════════════════════════════════════════════════════
 * Función    : eliminarAtraccion
 * Descripción: Busca una atracción por nombre, la desenlaza de la
 *              lista y libera toda su memoria (filas, nodos, datos).
 * Retorno    : void
 * ═══════════════════════════════════════════════════════════════════ */

void eliminarAtraccion(struct Zona *z, char *nombre) {
    struct NodoAtraccion *actual;
    struct NodoAtraccion *previo;

    if (z == NULL || nombre == NULL) {
        printf("Error: zona o nombre invalido.\n");
        return;
    }

    if (z->head_atracciones == NULL) {
        printf("La zona '%s' no tiene atracciones.\n", z->nombre);
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

            printf("Atraccion '%s' eliminada correctamente.\n", nombre);
            return;
        }

        previo = actual;
        actual = actual->sig;
    }

    printf("Atraccion '%s' no encontrada en zona '%s'.\n", nombre, z->nombre);
}

int cambiarEstadoAtraccion(struct Atraccion *a, char *nuevo_estado, int vaciar_si_no_operativa) {
    char *copia;

    if (a == NULL || nuevo_estado == NULL) {
        return ATRACCION_ERROR;
    }

    if (!estado_atraccion_valido(nuevo_estado)) {
        printf("Estado invalido. No se realizaron cambios.\n");
        return ATRACCION_ERROR;
    }

    copia = copiar_texto(nuevo_estado);
    if (copia == NULL) {
        return ATRACCION_ERROR;
    }

    free(a->estado);
    a->estado = copia;

    
    if (strcmp(a->estado, "operativa") != 0) {
        suspender_filas_atraccion(a, vaciar_si_no_operativa);
    }

    return ATRACCION_OK;
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : modificarAtraccion
 * Descripción: Menú interactivo para modificar un campo específico
 *              de una atracción. Libera memoria anterior en campos
 *              de tipo char* antes de asignar el nuevo valor.
 * Retorno    : void
 * ═══════════════════════════════════════════════════════════════════ */

void modificarAtraccion(struct Atraccion *a) {
    int opcion;
    char nuevo_texto[100];
    char nuevo_estado[30];
    char *copia;

    if (a == NULL) {
        printf("Error: atraccion invalida.\n");
        return;
    }

    printf("\nQue desea modificar?\n");
    printf("1. Nombre\n2. Estado\n3. Tematica\n4. Duracion (min)\n");
    printf("5. Edad minima\n6. Altura minima\n7. Capacidad por ciclo\n");
    printf("8. Max. personas cola general\n9. Max. personas cola prioritaria\n");
    printf("Opcion: ");

    if (scanf("%d", &opcion) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    switch (opcion) {
        case 1:
            printf("Nuevo nombre: ");
            scanf(" %99[^\n]", nuevo_texto);
            copia = copiar_texto(nuevo_texto);
            if (copia == NULL) {
                printf("No se pudo actualizar el nombre.\n");
                return;
            }
            free(a->nombre);
            a->nombre = copia;
            break;

        case 2:
            printf("Estados: operativa / mantenimiento / cerrada / fuera_de_servicio\n");
            printf("Nuevo estado: ");
            scanf(" %29[^\n]", nuevo_estado);
            if (!cambiarEstadoAtraccion(a, nuevo_estado, 0)) {
                return;
            }
            break;

        case 3:
            printf("Nueva tematica: ");
            scanf(" %99[^\n]", nuevo_texto);
            copia = copiar_texto(nuevo_texto);
            if (copia == NULL) {
                printf("No se pudo actualizar la tematica.\n");
                return;
            }
            free(a->tematica);
            a->tematica = copia;
            break;

        case 4:
            printf("Nueva duracion (min): ");
            scanf("%d", &a->duracion);
            if (a->duracion <= 0) {
                printf("Invalida, se asigna 1.\n");
                a->duracion = 1;
            }
            break;

        case 5:
            printf("Nueva edad minima: ");
            scanf("%d", &a->edad_min);
            break;

        case 6:
            printf("Nueva altura minima (m): ");
            scanf("%f", &a->altura_min);
            break;

        case 7:
            printf("Nueva capacidad por ciclo: ");
            scanf("%d", &a->cap_max);
            if (a->cap_max <= 0) {
                printf("Invalida, se asigna 1.\n");
                a->cap_max = 1;
            }
            break;

        case 8:
            printf("Nuevo max. personas cola general: ");
            scanf("%d", &a->max_cola_general);
            if (a->max_cola_general < 0) {
                a->max_cola_general = 0;
            }
            break;

        case 9:
            printf("Nuevo max. personas cola prioritaria: ");
            scanf("%d", &a->max_cola_prioritaria);
            if (a->max_cola_prioritaria < 0) {
                a->max_cola_prioritaria = 0;
            }
            break;

        default:
            printf("Opcion invalida.\n");
            return;
    }

    printf("Atraccion actualizada correctamente.\n");
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : listarAtracciones
 * Descripción: Imprime los datos de todas las atracciones de una
 *              zona, incluyendo estado, restricciones y grupos en
 *              espera en ambas colas.
 * Retorno    : void
 * ═══════════════════════════════════════════════════════════════════ */

void listarAtracciones(struct Zona *z) {
    struct NodoAtraccion *actual;
    struct Atraccion *a;
    int i;
    int grupos_general;
    int grupos_prioridad;
    int personas_general;
    int personas_prioridad;

    if (z == NULL) {
        printf("Error: zona invalida.\n");
        return;
    }

    if (z->head_atracciones == NULL) {
        printf("La zona '%s' no tiene atracciones registradas.\n", z->nombre);
        return;
    }

    printf("\n======================================================\n");
    printf(" Atracciones de la zona: %s\n", z->nombre);
    printf("======================================================\n");

    actual = z->head_atracciones;
    i = 1;

    while (actual != NULL) {
        a = actual->datos;

        if (a != NULL) {
            grupos_general = contar_grupos_fila(&a->cola_general);
            grupos_prioridad = contar_grupos_fila(&a->cola_prioritaria);
            personas_general = contar_personas_fila(&a->cola_general);
            personas_prioridad = contar_personas_fila(&a->cola_prioritaria);

            printf("[%d] %-22s | Estado : %-20s\n", i, a->nombre, a->estado);
            printf("    Tematica  : %-20s | Duracion : %d min\n", a->tematica, a->duracion);
            printf("    Edad min  : %d anios      | Altura min: %.2f m\n", a->edad_min, a->altura_min);
            printf("    Cap/ciclo : %-5d          | Visitantes atendidos: %d\n", a->cap_max, a->visitantes_totales);
            printf("    Fila gral : %d grupos / %d personas | Espera aprox: %d min\n",
                   grupos_general,
                   personas_general,
                   calcular_espera_general_atraccion(a));
            printf("    Fila prior: %d grupos / %d personas | Espera aprox: %d min\n",
                   grupos_prioridad,
                   personas_prioridad,
                   calcular_espera_prioritaria_atraccion(a));
            printf("------------------------------------------------------\n");
        }

        i++;
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

/* ═══════════════════════════════════════════════════════════════════
 * Función    : buscar_Atraccion_Por_Estado
 * Descripción: Recorre la lista de atracciones de una zona y retorna
 *              la primera que coincida con el estado dado.
 * Retorno    : struct Atraccion* -> primera atracción encontrada,
 *              NULL si no hay coincidencia
 * ═══════════════════════════════════════════════════════════════════ */

struct Atraccion *buscar_Atraccion_Por_Estado(struct Zona *z, char *estado) {
    struct NodoAtraccion *actual;

    if (z == NULL || estado == NULL) {
        return NULL;
    }

    actual = z->head_atracciones;

    while (actual != NULL) {
        if (actual->datos != NULL && strcmp(actual->datos->estado, estado) == 0) {
            return actual->datos;
        }
        actual = actual->sig;
    }

    return NULL;
}


int agregarGrupoFilaAtraccion(struct Atraccion *a, int ids_grupo[], int tam_grupo, int es_prioritaria) {
    return agregar_grupo_atraccion(a, ids_grupo, tam_grupo, es_prioritaria);
}

int quitarGrupoFilaAtraccion(struct Atraccion *a, int es_prioritaria, int ids_salida[], int *tam_salida) {
    return quitar_grupo_atraccion(a, es_prioritaria, ids_salida, tam_salida);
}

int atenderAtraccion(struct Atraccion *a) {
    int atendidos;

    atendidos = atender_ciclo_atraccion(a);

    if (a != NULL) {
        printf("Se atendieron %d visitantes en la atraccion '%s'.\n", atendidos, a->nombre);
    }

    return atendidos;
}

int tiempoEsperaAtraccion(struct Atraccion *a) {
    return calcular_espera_atraccion(a);
}

void mostrarFilasAtraccion(struct Atraccion *a) {
    if (a == NULL) {
        printf("Atraccion invalida.\n");
        return;
    }

    mostrar_resumen_filas_atraccion(a);

    printf("\nFila prioritaria:\n");
    mostrar_fila(&a->cola_prioritaria);

    printf("\nFila general:\n");
    mostrar_fila(&a->cola_general);
}

void vaciarFilasAtraccion(struct Atraccion *a) {
    if (a == NULL) {
        return;
    }

    vaciar_fila(&a->cola_general);
    vaciar_fila(&a->cola_prioritaria);
}