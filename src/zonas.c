#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "atracciones.h"
#include "zonas.h"
#include "utils.h"

struct Zona *obtener_zona_por_id(struct NodoZonas *head_zonas, int id_zona) {
    struct NodoZonas *actual;

    if (head_zonas == NULL) {
        return NULL;
    }

    actual = head_zonas;

    while (actual != NULL) {
        if (actual->datos != NULL && actual->datos->id == id_zona)
            return actual->datos; 
        actual = actual->sig;
    }

    return NULL;
}   

int agregar_zona(struct NodoZonas **head_zonas, const char *nombre, const char *tematica, 
                 int cap_max, int hora_apertura, int hora_cierre, int min_apertura, 
                 int min_cierre, int max_atracciones) {
    
    int nueva_id;
    int id_ocupada;
    struct NodoZonas *actual;
    struct NodoZonas *nuevo_nodo;
    struct Zona *nueva_zona;

    if (head_zonas == NULL) {
        return -1; /* ERR PUNTERO RAIZ NULO */
    }

    nueva_id = 1;
    id_ocupada = 1;
    while (id_ocupada) {
        id_ocupada = 0;
        actual = *head_zonas;
        while (actual != NULL) {
            if (actual->datos != NULL && actual->datos->id == nueva_id) {
                id_ocupada = 1;
                break; 
            }
            actual = actual->sig;
        }
        if (id_ocupada) {
            nueva_id++;
        }
    }

    nuevo_nodo = (struct NodoZonas *)malloc(sizeof(struct NodoZonas));
    nueva_zona = (struct Zona *)malloc(sizeof(struct Zona));

    if (nuevo_nodo == NULL || nueva_zona == NULL) {
        free(nuevo_nodo);
        free(nueva_zona);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    nueva_zona->nombre = copiar_string(nombre);
    nueva_zona->tematica = copiar_string(tematica);

    if (nueva_zona->nombre == NULL || nueva_zona->tematica == NULL) {
        free(nueva_zona->nombre);
        free(nueva_zona->tematica);
        free(nueva_zona);
        free(nuevo_nodo);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    nueva_zona->id = nueva_id;
    nueva_zona->cap_max = cap_max;
    nueva_zona->visitantes_actuales = 0;
    nueva_zona->atracciones_max = max_atracciones;
    nueva_zona->head_atracciones = NULL;

    nueva_zona->hora_apertura.hora = hora_apertura;
    nueva_zona->hora_apertura.minutos = min_apertura;
    nueva_zona->hora_cierre.hora = hora_cierre;
    nueva_zona->hora_cierre.minutos = min_cierre;

    nuevo_nodo->datos = nueva_zona;
    nuevo_nodo->sig = *head_zonas;
    *head_zonas = nuevo_nodo;

    return 0;
}

int eliminar_zona(struct NodoZonas **head_zonas, int id) {
    struct NodoZonas **enlace;
    struct NodoZonas *a_eliminar;
    struct NodoAtraccion *act_atr;
    struct NodoAtraccion *sig_atr;
    struct NodoFila *act_fila;
    struct NodoFila *sig_fila;

    if (head_zonas == NULL || *head_zonas == NULL) {
        return -1; /* LISTA VACIA */
    }

    enlace = head_zonas;
    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL && (*enlace)->datos->id == id) {
            break;
        }
        enlace = &((*enlace)->sig);
    }

    if (*enlace == NULL) {
        return -2; /* ZONA NO ENCONTRADA */
    }

    a_eliminar = *enlace;
    *enlace = a_eliminar->sig;

    if (a_eliminar->datos != NULL) {
        
        act_atr = a_eliminar->datos->head_atracciones;
        while (act_atr != NULL) {
            sig_atr = act_atr->sig; 
            if (act_atr->datos != NULL) {
                
                act_fila = act_atr->datos->cola_general.frente;
                while (act_fila != NULL) {
                    sig_fila = act_fila->sig;
                    free(act_fila);
                    act_fila = sig_fila;
                }

                act_fila = act_atr->datos->cola_prioritaria.frente;
                while (act_fila != NULL) {
                    sig_fila = act_fila->sig;
                    free(act_fila);
                    act_fila = sig_fila;
                }

                free(act_atr->datos->nombre);
                free(act_atr->datos->tematica);
                free(act_atr->datos->estado);

                free(act_atr->datos);
            }
            
            free(act_atr);
            act_atr = sig_atr;
        }

        free(a_eliminar->datos->nombre);
        free(a_eliminar->datos->tematica);
        free(a_eliminar->datos);
    }

    free(a_eliminar);

    return 0;
}

int agregar_o_remover_visitantes_zona(struct NodoZonas *head_zonas, int id, int visitantes) {
    struct NodoZonas *actual;
    int nuevo_total;
    
    struct Tiempo hora_actual;
    int mins_actuales;
    int mins_apertura;
    int mins_cierre;

    if (head_zonas == NULL) {
        return -1; /* LISTA NULA */
    }

    actual = head_zonas;

    while (actual != NULL) {
        if (actual->datos != NULL && actual->datos->id == id) {
            
            if (visitantes > 0) {
                hora_actual = obtener_hora_actual();
                
                mins_actuales = (hora_actual.hora * 60) + hora_actual.minutos;
                mins_apertura = (actual->datos->hora_apertura.hora * 60) + actual->datos->hora_apertura.minutos;
                mins_cierre = (actual->datos->hora_cierre.hora * 60) + actual->datos->hora_cierre.minutos;
                
                if (mins_actuales < mins_apertura || mins_actuales >= mins_cierre) {
                    return -5; /* ZONA CERRADA POR HORARIO */
                }
            }

            nuevo_total = actual->datos->visitantes_actuales + visitantes;

            if (nuevo_total > actual->datos->cap_max) {
                return -3; /* SUPERA CANTIDAD MAXIMA */
            }

            if (nuevo_total < 0) {
                return -4; /* VISITANTES MENOR A 0 */
            }

            actual->datos->visitantes_actuales = nuevo_total;
            return 0; /* ÉXITO */
        }
        actual = actual->sig;
    }

    return -2; /* ZONA NO ENCONTRADA */
}