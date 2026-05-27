#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

#define FILA_OK 1
#define FILA_ERROR 0

 struct NodoFila *crear_nodo_fila(int ids_grupo[], int tam_grupo) {
    struct NodoFila *nuevo;
    int i;

    if (ids_grupo == NULL || tam_grupo <= 0 || tam_grupo > 10) {
        return NULL;
    }

    nuevo = (struct NodoFila *) malloc(sizeof(struct NodoFila));
    if (nuevo == NULL) {
        return NULL;
    }

    for (i = 0; i < 10; i++) {
        nuevo->ids_grupo[i] = -1;
    }

    for (i = 0; i < tam_grupo; i++) {
        nuevo->ids_grupo[i] = ids_grupo[i];
    }

    nuevo->tam_grupo = tam_grupo;
    nuevo->sig = NULL;

    return nuevo;
}

 int estado_es_operativo(char *estado) {
    if (estado == NULL) {
        return FILA_ERROR;
    }

    return strcmp(estado, "operativa") == 0;
}

 int limite_cola_permite(struct Fila *fila, int max_cola, int tam_grupo);


void inicializar_fila(struct Fila *fila) {
    if (fila == NULL) {
        return;
    }

    fila->frente = NULL;
    fila->final = NULL;
}

int fila_vacia(struct Fila *fila) {
    if (fila == NULL || fila->frente == NULL) {
        return FILA_OK;
    }

    return FILA_ERROR;
}

int agregar_grupo_fila(struct Fila *fila, int ids_grupo[], int tam_grupo) {
    struct NodoFila *nuevo;

    if (fila == NULL || ids_grupo == NULL || tam_grupo <= 0 || tam_grupo > 10) {
        return FILA_ERROR;
    }

    nuevo = crear_nodo_fila(ids_grupo, tam_grupo);
    if (nuevo == NULL) {
        return FILA_ERROR;
    }

    if (fila->frente == NULL) {
        fila->frente = nuevo;
        fila->final = nuevo;
    } else {
        fila->final->sig = nuevo;
        fila->final = nuevo;
    }

    return FILA_OK;
}

int quitar_grupo_fila(struct Fila *fila, int ids_salida[], int *tam_salida) {
    struct NodoFila *aux;
    int i;

    if (fila == NULL || fila->frente == NULL) {
        return FILA_ERROR;
    }

    aux = fila->frente;

    if (ids_salida != NULL) {
        for (i = 0; i < aux->tam_grupo; i++) {
            ids_salida[i] = aux->ids_grupo[i];
        }
    }

    if (tam_salida != NULL) {
        *tam_salida = aux->tam_grupo;
    }

    fila->frente = aux->sig;

    if (fila->frente == NULL) {
        fila->final = NULL;
    }

    free(aux);
    return FILA_OK;
}

int contar_grupos_fila(struct Fila *fila) {
    struct NodoFila *actual;
    int total;

    if (fila == NULL) {
        return 0;
    }

    actual = fila->frente;
    total = 0;

    while (actual != NULL) {
        total++;
        actual = actual->sig;
    }

    return total;
}

int contar_personas_fila(struct Fila *fila) {
    struct NodoFila *actual;
    int total;

    if (fila == NULL) {
        return 0;
    }

    actual = fila->frente;
    total = 0;

    while (actual != NULL) {
        total = total + actual->tam_grupo;
        actual = actual->sig;
    }

    return total;
}

int calcular_ciclos_necesarios(int personas, int capacidad_por_ciclo) {
    int ciclos;

    if (personas <= 0 || capacidad_por_ciclo <= 0) {
        return 0;
    }

    ciclos = personas / capacidad_por_ciclo;

    if (personas % capacidad_por_ciclo != 0) {
        ciclos++;
    }

    return ciclos;
}

int calcular_minutos_fila(struct Fila *fila, int capacidad_por_ciclo, int duracion_ciclo) {
    int personas;
    int ciclos;

    if (fila == NULL || capacidad_por_ciclo <= 0 || duracion_ciclo <= 0) {
        return 0;
    }

    personas = contar_personas_fila(fila);
    ciclos = calcular_ciclos_necesarios(personas, capacidad_por_ciclo);

    return ciclos * duracion_ciclo;
}

void vaciar_fila(struct Fila *fila) {
    int ids_aux[10];
    int tam_aux;

    if (fila == NULL) {
        return;
    }

    while (fila->frente != NULL) {
        quitar_grupo_fila(fila, ids_aux, &tam_aux);
    }
}

void mostrar_fila(struct Fila *fila) {
    struct NodoFila *actual;
    int posicion;
    int i;

    if (fila == NULL || fila->frente == NULL) {
        printf("La fila esta vacia.\n");
        return;
    }

    actual = fila->frente;
    posicion = 1;

    while (actual != NULL) {
        printf("Grupo %d | Personas: %d | Entradas: ", posicion, actual->tam_grupo);

        for (i = 0; i < actual->tam_grupo; i++) {
            printf("%d", actual->ids_grupo[i]);

            if (i < actual->tam_grupo - 1) {
                printf(", ");
            }
        }

        printf("\n");

        posicion++;
        actual = actual->sig;
    }
}


int atraccion_operativa(struct Atraccion *atraccion) {
    if (atraccion == NULL) {
        return FILA_ERROR;
    }

    return estado_es_operativo(atraccion->estado);
}

 int limite_cola_permite(struct Fila *fila, int max_cola, int tam_grupo) {
    int personas_actuales;

    if (max_cola <= 0) {
        return FILA_OK;
    }

    personas_actuales = contar_personas_fila(fila);

    if (personas_actuales + tam_grupo > max_cola) {
        return FILA_ERROR;
    }

    return FILA_OK;
}

int agregar_grupo_atraccion(struct Atraccion *atraccion, int ids_grupo[], int tam_grupo, int es_prioritaria) {
    struct Fila *fila_destino;
    int max_cola;

    if (atraccion == NULL || ids_grupo == NULL || tam_grupo <= 0 || tam_grupo > 10) {
        return FILA_ERROR;
    }

    if (!atraccion_operativa(atraccion)) {
        printf("No se puede agregar a la fila: la atraccion no esta operativa.\n");
        return FILA_ERROR;
    }

    if (atraccion->cap_max > 0 && tam_grupo > atraccion->cap_max) {
        printf("No se puede agregar al grupo: supera la capacidad por ciclo de la atraccion.\n");
        return FILA_ERROR;
    }

    if (es_prioritaria) {
        fila_destino = &atraccion->cola_prioritaria;
        max_cola = atraccion->max_cola_prioritaria;
    } else {
        fila_destino = &atraccion->cola_general;
        max_cola = atraccion->max_cola_general;
    }

    if (!limite_cola_permite(fila_destino, max_cola, tam_grupo)) {
        printf("No se puede agregar al grupo: la fila alcanzo su capacidad maxima.\n");
        return FILA_ERROR;
    }

    return agregar_grupo_fila(fila_destino, ids_grupo, tam_grupo);
}

int quitar_grupo_atraccion(struct Atraccion *atraccion, int es_prioritaria, int ids_salida[], int *tam_salida) {
    if (atraccion == NULL) {
        return FILA_ERROR;
    }

    if (es_prioritaria) {
        return quitar_grupo_fila(&atraccion->cola_prioritaria, ids_salida, tam_salida);
    }

    return quitar_grupo_fila(&atraccion->cola_general, ids_salida, tam_salida);
}

int contar_personas_atraccion(struct Atraccion *atraccion) {
    int total;

    if (atraccion == NULL) {
        return 0;
    }

    total = contar_personas_fila(&atraccion->cola_prioritaria);
    total = total + contar_personas_fila(&atraccion->cola_general);

    return total;
}

int contar_grupos_atraccion(struct Atraccion *atraccion) {
    int total;

    if (atraccion == NULL) {
        return 0;
    }

    total = contar_grupos_fila(&atraccion->cola_prioritaria);
    total = total + contar_grupos_fila(&atraccion->cola_general);

    return total;
}

int calcular_espera_atraccion(struct Atraccion *atraccion) {
    if (atraccion == NULL) {
        return 0;
    }

    return calcular_minutos_fila(&atraccion->cola_prioritaria, atraccion->cap_max, atraccion->duracion)
         + calcular_minutos_fila(&atraccion->cola_general, atraccion->cap_max, atraccion->duracion);
}

int calcular_espera_general_atraccion(struct Atraccion *atraccion) {
    int personas_prioridad;
    int personas_general;
    int ciclos;

    if (atraccion == NULL || atraccion->cap_max <= 0 || atraccion->duracion <= 0) {
        return 0;
    }

    
    personas_prioridad = contar_personas_fila(&atraccion->cola_prioritaria);
    personas_general = contar_personas_fila(&atraccion->cola_general);

    ciclos = calcular_ciclos_necesarios(personas_prioridad + personas_general, atraccion->cap_max);

    return ciclos * atraccion->duracion;
}

int calcular_espera_prioritaria_atraccion(struct Atraccion *atraccion) {
    int personas_prioridad;
    int ciclos;

    if (atraccion == NULL || atraccion->cap_max <= 0 || atraccion->duracion <= 0) {
        return 0;
    }

    personas_prioridad = contar_personas_fila(&atraccion->cola_prioritaria);
    ciclos = calcular_ciclos_necesarios(personas_prioridad, atraccion->cap_max);

    return ciclos * atraccion->duracion;
}


 int atender_desde_fila(struct Fila *fila, int *capacidad_restante) {
    int atendidos;
    int ids_aux[10];
    int tam_aux;

    atendidos = 0;

    if (fila == NULL || capacidad_restante == NULL) {
        return 0;
    }

    while (fila->frente != NULL && *capacidad_restante > 0) {
        if (fila->frente->tam_grupo > *capacidad_restante) {
            break;
        }

        tam_aux = 0;
        quitar_grupo_fila(fila, ids_aux, &tam_aux);

        atendidos = atendidos + tam_aux;
        *capacidad_restante = *capacidad_restante - tam_aux;
    }

    return atendidos;
}

int atender_ciclo_atraccion(struct Atraccion *atraccion) {
    int capacidad_restante;
    int atendidos;

    if (atraccion == NULL || atraccion->cap_max <= 0) {
        return 0;
    }

    if (!atraccion_operativa(atraccion)) {
        printf("No se puede atender: la atraccion no esta operativa.\n");
        return 0;
    }

    capacidad_restante = atraccion->cap_max;
    atendidos = 0;

    
    atendidos = atendidos + atender_desde_fila(&atraccion->cola_prioritaria, &capacidad_restante);
    atendidos = atendidos + atender_desde_fila(&atraccion->cola_general, &capacidad_restante);

   
    atraccion->visitantes_totales = atraccion->visitantes_totales + atendidos;

    return atendidos;
}

void suspender_filas_atraccion(struct Atraccion *atraccion, int vaciar_filas) {
    if (atraccion == NULL) {
        return;
    }

    if (vaciar_filas) {
        vaciar_fila(&atraccion->cola_general);
        vaciar_fila(&atraccion->cola_prioritaria);
    }
}

void mostrar_resumen_filas_atraccion(struct Atraccion *atraccion) {
    if (atraccion == NULL) {
        printf("Atraccion no valida.\n");
        return;
    }

    printf("Atraccion: %s\n", atraccion->nombre);
    printf("Estado: %s\n", atraccion->estado);
    printf("Personas en fila general: %d\n", contar_personas_fila(&atraccion->cola_general));
    printf("Personas en fila prioritaria: %d\n", contar_personas_fila(&atraccion->cola_prioritaria));
    printf("Personas totales esperando: %d\n", contar_personas_atraccion(atraccion));
    printf("Tiempo espera general aprox: %d minutos\n", calcular_espera_general_atraccion(atraccion));
    printf("Tiempo espera prioritaria aprox: %d minutos\n", calcular_espera_prioritaria_atraccion(atraccion));
    printf("Visitantes atendidos en el dia: %d\n", atraccion->visitantes_totales);
}
