#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "filas.h"
#include "utils.h"

struct Atraccion *buscar_atraccion_por_id(struct NodoZonas *head_zonas, int id_atraccion) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion *atraccion_actual;

    if (head_zonas == NULL) {
        return NULL;
    }

    zona_actual = head_zonas;

    while (zona_actual != NULL) {
        if (zona_actual->datos != NULL) {
            
            atraccion_actual = zona_actual->datos->head_atracciones;
            while (atraccion_actual != NULL) {
                
                if (atraccion_actual->datos != NULL && atraccion_actual->datos->id == id_atraccion)
                    return atraccion_actual->datos; 
                atraccion_actual = atraccion_actual->sig;
            }
        }
        zona_actual = zona_actual->sig;
    }

    return NULL;
}

struct Atraccion *obtener_atraccion_mayor_pico(struct Parque *parque) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion *atraccion_actual;
    struct Atraccion *atraccion_max;
    int max_pico;
    int suma_picos;

    if (parque == NULL || parque->head_zonas == NULL) {
        return NULL;
    }

    max_pico = -1;
    atraccion_max = NULL;
    
    zona_actual = parque->head_zonas;
    while (zona_actual != NULL) {
        
        if (zona_actual->datos != NULL) {
            
            atraccion_actual = zona_actual->datos->head_atracciones;
            while (atraccion_actual != NULL) {
                
                if (atraccion_actual->datos != NULL) {
                    suma_picos = atraccion_actual->datos->pico_cola_general + 
                                 atraccion_actual->datos->pico_cola_prioritaria;

                    if (suma_picos > max_pico) {
                        max_pico = suma_picos;
                        atraccion_max = atraccion_actual->datos;
                    }
                }
                
                atraccion_actual = atraccion_actual->sig;
            }
        }
        
        zona_actual = zona_actual->sig;
    }

    return atraccion_max;
}

struct Atraccion *obtener_atraccion_mas_visitada(struct Parque *parque) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion *atraccion_actual;
    struct Atraccion *atraccion_max;
    int max_visitantes;

    if (parque == NULL || parque->head_zonas == NULL) {
        return NULL;
    }

    max_visitantes = -1;
    atraccion_max = NULL;

    zona_actual = parque->head_zonas;
    while (zona_actual != NULL) {

        if (zona_actual->datos != NULL) {
            
            atraccion_actual = zona_actual->datos->head_atracciones;
            while (atraccion_actual != NULL) {

                if (atraccion_actual->datos != NULL) {
                    
                    if (atraccion_actual->datos->visitantes_totales > max_visitantes) {
                        max_visitantes = atraccion_actual->datos->visitantes_totales;
                        atraccion_max = atraccion_actual->datos;
                    }
                }

                atraccion_actual = atraccion_actual->sig;
            }
        }

        zona_actual = zona_actual->sig;
    }

    return atraccion_max;
}

int agregar_atraccion(struct NodoZonas *head_zonas, struct Zona *zona, const char *nombre, 
                      const char *tematica, int duracion, int cap_max, int edad_min, float altura_min) {
    
    int contador_atracciones;
    int nueva_id;
    int id_ocupada;
    struct NodoZonas *zona_aux;
    struct NodoAtraccion *actual;
    struct NodoAtraccion *nuevo_nodo;
    struct Atraccion *nueva_atraccion;

    if (head_zonas == NULL || zona == NULL) {
        return -1; /* PUNTERO NULO */
    }

    if (zona->tematica == NULL || tematica == NULL || strcmp(zona->tematica, tematica) != 0) {
        return -2; /* TEMATICA INCOMPATIBLE */
    }

    contador_atracciones = 0;
    actual = zona->head_atracciones;
    while (actual != NULL) {
        contador_atracciones++;
        actual = actual->sig;
    }

    if (contador_atracciones >= zona->atracciones_max) {
        return -3; /*SIN ESPACIO*/
    }

    nueva_id = 1;
    id_ocupada = 1;

    while (id_ocupada) {
        id_ocupada = 0;
        zona_aux = head_zonas;

        while (zona_aux != NULL) {
            if (zona_aux->datos != NULL) {
                actual = zona_aux->datos->head_atracciones;
                
                while (actual != NULL) {
                    if (actual->datos != NULL && actual->datos->id == nueva_id) {
                        id_ocupada = 1;
                        break;
                    }
                    actual = actual->sig;
                }
            }
            
            if (id_ocupada) {
                break; 
            }
            zona_aux = zona_aux->sig;
        }

        if (id_ocupada) {
            nueva_id++;
        }
    }

    nuevo_nodo = (struct NodoAtraccion *)malloc(sizeof(struct NodoAtraccion));
    nueva_atraccion = (struct Atraccion *)malloc(sizeof(struct Atraccion));

    if (nuevo_nodo == NULL || nueva_atraccion == NULL) {
        free(nuevo_nodo);
        free(nueva_atraccion);
        return -4; /* ERROR MEMORIA */
    }

    nueva_atraccion->nombre = copiar_string(nombre);
    nueva_atraccion->tematica = copiar_string(tematica);
    nueva_atraccion->estado = copiar_string("operativa");

    if (nueva_atraccion->nombre == NULL || nueva_atraccion->tematica == NULL || nueva_atraccion->estado == NULL) {
        free(nueva_atraccion->nombre);
        free(nueva_atraccion->tematica);
        free(nueva_atraccion->estado);
        free(nueva_atraccion);
        free(nuevo_nodo);
        return -4; /* ERROR MEMORIA */
    }

    nueva_atraccion->id = nueva_id;
    nueva_atraccion->duracion = duracion;
    nueva_atraccion->cap_max = cap_max;
    nueva_atraccion->edad_min = edad_min;
    nueva_atraccion->altura_min = altura_min;

    nueva_atraccion->cola_general.frente = NULL;
    nueva_atraccion->cola_general.final = NULL;
    nueva_atraccion->cola_prioritaria.frente = NULL;
    nueva_atraccion->cola_prioritaria.final = NULL;

    nueva_atraccion->visitantes_totales = 0;
    nueva_atraccion->pico_cola_general = 0;
    nueva_atraccion->pico_cola_prioritaria = 0;

    nuevo_nodo->datos = nueva_atraccion;
    nuevo_nodo->sig = zona->head_atracciones;
    zona->head_atracciones = nuevo_nodo;

    return 0; /* EXITO */
}

int eliminar_atraccion(struct NodoZonas *head_zonas, int id_atraccion) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion **enlace;
    struct NodoAtraccion *a_eliminar;
    struct NodoFila *aux_fila;
    struct NodoFila *sig_fila;

    if (head_zonas == NULL) {
        return -1; /* ZONA NULA */
    }

    zona_actual = head_zonas;

    while (zona_actual != NULL) {
        if (zona_actual->datos != NULL) {
            
            enlace = &zona_actual->datos->head_atracciones;

            while (*enlace != NULL) {
                if ((*enlace)->datos != NULL && (*enlace)->datos->id == id_atraccion) {
                    
                    a_eliminar = *enlace;
                    *enlace = (*enlace)->sig;

                    if (a_eliminar->datos != NULL) {
                        
                        aux_fila = a_eliminar->datos->cola_general.frente;
                        while (aux_fila != NULL) {
                            sig_fila = aux_fila->sig;
                            free(aux_fila);
                            aux_fila = sig_fila;
                        }

                        aux_fila = a_eliminar->datos->cola_prioritaria.frente;
                        while (aux_fila != NULL) {
                            sig_fila = aux_fila->sig;
                            free(aux_fila);
                            aux_fila = sig_fila;
                        }

                        free(a_eliminar->datos->nombre);
                        free(a_eliminar->datos->tematica);
                        free(a_eliminar->datos->estado);

                        free(a_eliminar->datos);
                    }

                    free(a_eliminar);

                    return 0;
                }
                
                enlace = &((*enlace)->sig);
            }
        }
        zona_actual = zona_actual->sig;
    }

    return -2; /* ATRACCION NO ENCONTRADA */
}

int mover_atraccion(struct NodoZonas *head_zonas, struct Zona *zona_objetivo, int id_atraccion) {
    int contador_atracciones;
    struct NodoZonas *zona_actual;
    struct Zona *zona_inicio;
    struct NodoAtraccion *actual_atr;
    struct NodoAtraccion **enlace;
    struct NodoAtraccion *nodo_a_mover;

    if (head_zonas == NULL || zona_objetivo == NULL) {
        return -1; /* ERR_PUNTERO_NULO */
    }

    zona_inicio = NULL;
    zona_actual = head_zonas;

    while (zona_actual != NULL && zona_inicio == NULL) {
        if (zona_actual->datos != NULL) {
            actual_atr = zona_actual->datos->head_atracciones;
            
            while (actual_atr != NULL) {
                if (actual_atr->datos != NULL && actual_atr->datos->id == id_atraccion) {
                    zona_inicio = zona_actual->datos; 
                    break;
                }
                actual_atr = actual_atr->sig;
            }
        }
        zona_actual = zona_actual->sig;
    }

    if (zona_inicio == NULL) {
        return -4; /* ERR_ATRACCION_NO_ENCONTRADA */
    }

    if (zona_inicio == zona_objetivo) {
        return 0; /* EXITO */
    }

    if (zona_inicio->tematica == NULL || zona_objetivo->tematica == NULL || 
        strcmp(zona_inicio->tematica, zona_objetivo->tematica) != 0) {
        return -2; /* ERR_TEMATICA_INCOMPATIBLE */
    }

    contador_atracciones = 0;
    actual_atr = zona_objetivo->head_atracciones;
    while (actual_atr != NULL) {
        contador_atracciones++;
        actual_atr = actual_atr->sig;
    }

    if (contador_atracciones >= zona_objetivo->atracciones_max) {
        return -3; /* CAPACIDAD MAX */
    }

    enlace = &zona_inicio->head_atracciones;
    nodo_a_mover = NULL;

    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL && (*enlace)->datos->id == id_atraccion) {
            nodo_a_mover = *enlace;
            *enlace = (*enlace)->sig;
            break;
        }
        enlace = &((*enlace)->sig);
    }

    if (nodo_a_mover != NULL) {
        nodo_a_mover->sig = zona_objetivo->head_atracciones;
        zona_objetivo->head_atracciones = nodo_a_mover;
    }

    return 0; /* EXITO */
}

int cambiar_estado_atraccion(struct NodoZonas *head_zonas, int id_atraccion, const char *nuevo_estado) {
    struct Atraccion *atraccion;
    char *temp_estado;

    if (head_zonas == NULL) {
        return -1; /* ZONA NULA */
    }

    atraccion = buscar_atraccion_por_id(head_zonas, id_atraccion);
    if (atraccion == NULL) {
        return -2; /* ATRACCION NO ENCONTRADA */
    }

    temp_estado = copiar_string(nuevo_estado);
    if (temp_estado == NULL) {
        return -3; /* ERROR MEMORIA INSUFICIENTE */
    }

    free(atraccion->estado);
    atraccion->estado = temp_estado;

    if (strcmp(nuevo_estado, "cerrada") == 0 || strcmp(nuevo_estado, "fuera_de_servicio") == 0) {
        vaciar_filas_atraccion(head_zonas, id_atraccion);
    }

    return 0; /* EXITO */
}