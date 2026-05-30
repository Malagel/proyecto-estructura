#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "visitantes.h"

void vaciar_cola_nodos(struct Fila *fila) {
    struct NodoFila *actual;
    struct NodoFila *siguiente;

    if (fila == NULL) {
        return;
    }

    actual = fila->frente;
    
    while (actual != NULL) {
        siguiente = actual->sig;
        free(actual);
        actual = siguiente;
    }

    fila->frente = NULL;
    fila->final = NULL;
}

int vaciar_filas_atraccion(struct NodoZonas *head_zonas, int id_atraccion) {
    struct Atraccion *atraccion;

    atraccion = buscar_atraccion_por_id(head_zonas, id_atraccion);
    if (atraccion == NULL) {
        return -1; 
    }

    vaciar_cola_nodos(&(atraccion->cola_general));
    vaciar_cola_nodos(&(atraccion->cola_prioritaria));

    return 0; 
}

int agregar_grupo_fila(struct NodoVisitantes *raiz_visitantes, struct Atraccion *atraccion, int *ids_grupo, int tam_grupo, int es_prioritaria) {
    int i;
    struct Visitante *v;
    struct NodoFila *nuevo_nodo;
    struct Fila *fila_destino;
    struct NodoFila *actual;
    int personas_en_cola;

    if (tam_grupo < 1 || tam_grupo > 10) {
        return -6;
    }

    if (atraccion == NULL || atraccion->estado == NULL || strcmp(atraccion->estado, "operativa") != 0) {
        return -1;
    }

    for (i = 0; i < tam_grupo; i++) {
        v = buscar_visitante_por_id(raiz_visitantes, ids_grupo[i]);
        
        if (v == NULL) {
            return -2;
        }
        
        if (v->edad < atraccion->edad_min || v->altura < atraccion->altura_min) {
            return -3;
        }
        
        if (es_prioritaria) {
            if (v->entrada == NULL || v->entrada->tipo == NULL || strcmp(v->entrada->tipo, "prioritaria") != 0) {
                return -4;
            }
        }
    }

    nuevo_nodo = (struct NodoFila *)malloc(sizeof(struct NodoFila));
    if (nuevo_nodo == NULL) {
        return -5;
    }

    nuevo_nodo->tam_grupo = tam_grupo;
    nuevo_nodo->sig = NULL;
    
    for (i = 0; i < 10; i++) {
        if (i < tam_grupo) {
            nuevo_nodo->ids_grupo[i] = ids_grupo[i];
        } else {
            nuevo_nodo->ids_grupo[i] = 0;
        }
    }

    if (es_prioritaria) {
        fila_destino = &(atraccion->cola_prioritaria);
    } else {
        fila_destino = &(atraccion->cola_general);
    }

    if (fila_destino->final == NULL) {
        fila_destino->frente = nuevo_nodo;
        fila_destino->final = nuevo_nodo;
    } else {
        fila_destino->final->sig = nuevo_nodo;
        fila_destino->final = nuevo_nodo;
    }

    atraccion->visitantes_totales += tam_grupo;

    personas_en_cola = 0;
    actual = fila_destino->frente;
    while (actual != NULL) {
        personas_en_cola += actual->tam_grupo;
        actual = actual->sig;
    }

    if (es_prioritaria) {
        if (personas_en_cola > atraccion->pico_cola_prioritaria) {
            atraccion->pico_cola_prioritaria = personas_en_cola;
        }
    } else {
        if (personas_en_cola > atraccion->pico_cola_general) {
            atraccion->pico_cola_general = personas_en_cola;
        }
    }

    return 0;
}

int avanzar_fila_atraccion(struct NodoZonas *head_zonas, int id_atraccion) {
    struct Atraccion *atraccion;
    int cap_disponible;
    int turno_prioritaria;
    int p_bloqueada;
    int g_bloqueada;
    int encontrado;
    
    struct Fila *fila_actual;
    int *bloqueada_actual;
    int *bloqueada_otra;
    struct NodoFila *temp;
    struct NodoFila *anterior;
    struct NodoFila *actual;

    atraccion = buscar_atraccion_por_id(head_zonas, id_atraccion);
    if (atraccion == NULL) {
        return -1;
    }

    if (atraccion->cola_prioritaria.frente == NULL && atraccion->cola_general.frente == NULL) {
        return -2;
    }

    cap_disponible = atraccion->cap_max;
    turno_prioritaria = 1; 
    p_bloqueada = 0;       
    g_bloqueada = 0;       

    while (cap_disponible > 0 && (!p_bloqueada || !g_bloqueada)) {
        
        if (turno_prioritaria) {
            fila_actual = &(atraccion->cola_prioritaria);
            bloqueada_actual = &p_bloqueada;
            bloqueada_otra = &g_bloqueada;
        } else {
            fila_actual = &(atraccion->cola_general);
            bloqueada_actual = &g_bloqueada;
            bloqueada_otra = &p_bloqueada;
        }

        if (fila_actual->frente == NULL || *bloqueada_actual) {
            *bloqueada_actual = 1; 
            if (*bloqueada_otra) {
                break;
            }
            turno_prioritaria = !turno_prioritaria; 
            continue;
        }

        /* CASO A: El grupo en el frente de la fila cabe perfectamente en el espacio disponible */
        if (fila_actual->frente->tam_grupo <= cap_disponible) {
            temp = fila_actual->frente;
            cap_disponible -= temp->tam_grupo;
            
            fila_actual->frente = temp->sig;
            if (fila_actual->frente == NULL) {
                fila_actual->final = NULL;
            }
            free(temp);

            turno_prioritaria = !turno_prioritaria;
        } 
        
        /* CASO B: El grupo del frente excede la capacidad actual. Buscamos un grupo menor atrás. */
        else {
            anterior = fila_actual->frente;
            actual = fila_actual->frente->sig;
            encontrado = 0;

            while (actual != NULL) {
                if (actual->tam_grupo <= cap_disponible) {
                    encontrado = 1;
                    break;
                }
                anterior = actual;
                actual = actual->sig;
            }

            if (encontrado) {
                cap_disponible -= actual->tam_grupo;
                
                /* El grupo se "cuela" hacia adelante: lo desenlazamos de su posición intermedia */
                anterior->sig = actual->sig;
                if (actual == fila_actual->final) {
                    fila_actual->final = anterior;
                }
                free(actual);

                turno_prioritaria = !turno_prioritaria;
            } else {
                *bloqueada_actual = 1;
                turno_prioritaria = !turno_prioritaria;
            }
        }
    }

    return 0;
}