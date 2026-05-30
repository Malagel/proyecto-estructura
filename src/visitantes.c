#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "structs.h"

int contar_visitantes_dia(struct NodoVisitantes *visitantes){
    if(visitantes == NULL || visitantes->datos == NULL){
        return 0;
    }
    int cont = 0;

    if (visitantes->datos->entrada != NULL) {
        cont ++;
    }

    return cont + contar_visitantes_dia(visitantes->izq) + contar_visitantes_dia(visitantes->der);
}

int personas_dentro_parque(struct NodoVisitantes *visitantes) {
    if (visitantes == NULL || visitantes->datos == NULL) {
        return 0;
    }
    int cont = 0;

    if(visitantes->datos->entrada == NULL){
        return 0;
    }

    if (strcmp(visitantes->datos->entrada->estado, "utilizada") == 0) {
        cont++; 
    }

    return cont + personas_dentro_parque(visitantes->izq) + personas_dentro_parque(visitantes->der);
}

int falta_para_cap_max(struct Parque *parque){
    int personas_dentro;
    if(parque->raiz_visitantes == NULL){
        return parque->cap_max;
    }
    personas_dentro = personas_dentro_parque(parque->raiz_visitantes);

    return parque->cap_max - personas_dentro; 
}

static int id_existe_en_arbol(struct NodoVisitantes *raiz, int id) {
    struct NodoVisitantes *actual;
    actual = raiz;
    
    while (actual != NULL) {
        if (actual->datos != NULL) {
            if (id == actual->datos->id) {
                return 1; /* ID encontrada */
            }
            if (id < actual->datos->id) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }
        } else {
            break;
        }
    }
    return 0; /* ID libre */
}

int agregar_visitante(struct Parque *parque, struct Entrada *entrada, char *nombre, char *rut, int edad, float altura) {
    int nueva_id;
    struct NodoVisitantes **enlace;
    struct NodoVisitantes *nuevo_nodo;
    struct Visitante *nuevo_visitante;

    if (parque == NULL) {
        return -1; /* ERR PARQUE NULO */
    }

    nueva_id = 1;
    while (id_existe_en_arbol(parque->raiz_visitantes, nueva_id)) {
        nueva_id++;
    }

    nuevo_nodo = (struct NodoVisitantes *)malloc(sizeof(struct NodoVisitantes));
    nuevo_visitante = (struct Visitante *)malloc(sizeof(struct Visitante));

    if (nuevo_nodo == NULL || nuevo_visitante == NULL) {
        free(nuevo_nodo);
        free(nuevo_visitante);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    nuevo_visitante->nombre = copiar_string(nombre);
    if (nuevo_visitante->nombre == NULL) {
        free(nuevo_visitante);
        free(nuevo_nodo);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    strncpy(nuevo_visitante->rut, rut, 10);
    nuevo_visitante->rut[10] = '\0';

    nuevo_visitante->id = nueva_id;
    nuevo_visitante->edad = edad;
    nuevo_visitante->altura = altura;
    nuevo_visitante->entrada = entrada;
    nuevo_visitante->entrada->estado = copiar_string("utilizada");

    nuevo_nodo->datos = nuevo_visitante;
    nuevo_nodo->izq = NULL;
    nuevo_nodo->der = NULL;

    enlace = &parque->raiz_visitantes;
    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL) {
            if (nueva_id < (*enlace)->datos->id) {
                enlace = &((*enlace)->izq);
            } else {
                enlace = &((*enlace)->der);
            }
        } else {
            break;
        }
    }

    *enlace = nuevo_nodo;

    return 0;
}

int eliminar_visitante(struct NodoVisitantes **raiz_visitantes, int id) {
    struct NodoVisitantes **enlace;
    struct NodoVisitantes *a_eliminar;
    struct NodoVisitantes **sucesor;
    struct NodoVisitantes *nodo_sucesor;

    if (raiz_visitantes == NULL || *raiz_visitantes == NULL) {
        return -1; /* ERR ARBOL VACIO O NULO */
    }

    enlace = raiz_visitantes;
    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL) {
            if (id == (*enlace)->datos->id) {
                break; 
            }
            if (id < (*enlace)->datos->id) {
                enlace = &((*enlace)->izq);
            } else {
                enlace = &((*enlace)->der);
            }
        } else {
            return -3; /* ERR ESTRUCTURA CORRUPTA */
        }
    }

    if (*enlace == NULL) {
        return -2; /* ERR VISITANTE NO ENCONTRADO */
    }

    a_eliminar = *enlace;

    if (a_eliminar->izq == NULL) {
        *enlace = a_eliminar->der;
        
        if (a_eliminar->datos != NULL) {
            free(a_eliminar->datos->nombre);
            free(a_eliminar->datos);
        }
        free(a_eliminar);
    } 
    else if (a_eliminar->der == NULL) {
        *enlace = a_eliminar->izq;
        
        if (a_eliminar->datos != NULL) {
            free(a_eliminar->datos->nombre);
            free(a_eliminar->datos);
        }
        free(a_eliminar);
    } 
    else {
        sucesor = &(a_eliminar->der);
        while ((*sucesor)->izq != NULL) {
            sucesor = &((*sucesor)->izq);
        }
        
        nodo_sucesor = *sucesor;
        
        if (a_eliminar->datos != NULL) {
            free(a_eliminar->datos->nombre);
            free(a_eliminar->datos);
        }
        
        a_eliminar->datos = nodo_sucesor->datos;
        
        *sucesor = nodo_sucesor->der;
        
        free(nodo_sucesor);
    }

    return 0;
}