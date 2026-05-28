#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "filas.h"

#define ATRACCION_OK 1
#define ATRACCION_ERROR 0



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

int contar_grupos(struct Fila *f) {
    return contar_grupos_fila(f);
}

/* ═══════════════════════════════════════════════════════════════════
 * Función    : contar_Atracciones
 * Descripción: Cuenta cuántas atracciones tiene actualmente una zona.
 *              Auxiliar usada por agregar_atraccion.
 * Retorno    : int -> número de atracciones en la lista
 * ═══════════════════════════════════════════════════════════════════ */

int contar_atracciones(struct Zona *z) {
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

struct Atraccion *crear_atraccion(char *nombre, char *estado, char *tematica,
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

 //ACTUALIZA A POR ID NO POR NOMBRE

void eliminar_atraccion(struct Zona *z, char *nombre) {
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

int cambiar_estado_atraccion(struct Atraccion *a, const char *nuevo_estado) {
    if (a == NULL || nuevo_estado == NULL) return 0;

    const char *estados_validos[] = {
        "operativa", "mantenimiento", "cerrada", "fuera_de_servicio"
    };
    int n = sizeof(estados_validos) / sizeof(estados_validos[0]);

    for (int i = 0; i < n; i++) {
        if (strcmp(nuevo_estado, estados_validos[i]) == 0) {
            char *copia = copiar_texto(nuevo_estado);
            if (copia == NULL) return 0;
            free(a->estado);
            a->estado = copia;
            return 1;
        }
    }

    return 0;
}

void actualizar_pico_cola_general(struct Atraccion *a, int tam_actual) {
    if (a == NULL) return;
    if (tam_actual > a->max_cola_general)
        a->max_cola_general = tam_actual;
}

void actualizar_pico_cola_prioritaria(struct Atraccion *a, int tam_actual) {
    if (a == NULL) return;
    if (tam_actual > a->max_cola_prioritaria)
        a->max_cola_prioritaria = tam_actual;
}

int cambiar_nombre_atraccion(struct Atraccion *a, const char *nuevo_nombre) {
    if (a == NULL || nuevo_nombre == NULL || nuevo_nombre[0] == '\0') return 0;
    char *copia = copiar_texto(nuevo_nombre);
    if (copia == NULL) return 0;
    free(a->nombre);
    a->nombre = copia;
    return 1;
}

int cambiar_tematica_atraccion(struct Atraccion *a, const char *nueva_tematica) {
    if (a == NULL || nueva_tematica == NULL || nueva_tematica[0] == '\0') return 0;
    char *copia = copiar_texto(nueva_tematica);
    if (copia == NULL) return 0;
    free(a->tematica);
    a->tematica = copia;
    return 1;
}

int cambiar_duracion_atraccion(struct Atraccion *a, int nueva_duracion) {
    if (a == NULL || nueva_duracion <= 0) return 0;
    a->duracion = nueva_duracion;
    return 1;
}

int cambiar_edad_minima_atraccion(struct Atraccion *a, int nueva_edad) {
    if (a == NULL || nueva_edad < 0) return 0;
    a->edad_min = nueva_edad;
    return 1;
}

int cambiar_altura_minima_atraccion(struct Atraccion *a, float nueva_altura) {
    if (a == NULL || nueva_altura < 0.0f) return 0;
    a->altura_min = nueva_altura;
    return 1;
}

int cambiar_capacidad_atraccion(struct Atraccion *a, int nueva_cap) {
    if (a == NULL || nueva_cap <= 0) return 0;
    a->cap_max = nueva_cap;
    return 1;
}

struct NodoAtraccion *obtener_atracciones_zona(const struct Zona *z) {
    if (z == NULL) return NULL;
    return z->head_atracciones;
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