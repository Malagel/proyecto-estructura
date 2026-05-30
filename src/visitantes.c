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

struct Visitante* crear_visitante(char *nombre_ingresado, char *rut_ingresado, int edad_ingresada, float altura_ingresada) {
    struct Visitante *nuevo_v;

    static int generador_id = 1; 

    nuevo_v = (struct Visitante *)malloc(sizeof(struct Visitante));

    if (nuevo_v != NULL) {
        nuevo_v->nombre = (char *)malloc((strlen(nombre_ingresado) + 1) * sizeof(char));
        
        if (nuevo_v->nombre != NULL) {
            strcpy(nuevo_v->nombre, nombre_ingresado);
        }

        strncpy(nuevo_v->rut, rut_ingresado, 11);
        nuevo_v->rut[11] = '\0'; 
        
        nuevo_v->edad = edad_ingresada;
        nuevo_v->altura = altura_ingresada;
        
        nuevo_v->id = generador_id; 
        generador_id++; 
        
        nuevo_v->entrada = NULL; 
    }

    return nuevo_v; 
}

int agregar_visitante(struct Parque *parque, char *nombre, char *rut, int edad, float altura) {
    struct NodoVisitantes *nuevo_n;
    struct NodoVisitantes *actual;
    struct NodoVisitantes *nodo;
    struct Visitante *nuevo;
    int comp;

    if (parque == NULL) {
        return 0; 
    }

    nuevo = crear_visitante(nombre, rut, edad, altura);

    if (nuevo == NULL) {
        return 0;
    }

    if (falta_para_cap_max(parque) <= 0) { 
        free(nuevo->nombre);
        free(nuevo);
        return 0;
    }

    nuevo_n = (struct NodoVisitantes *)malloc(sizeof(struct NodoVisitantes));
    if (nuevo_n == NULL) {
        free(nuevo->nombre);
        free(nuevo);
        return 0;
    }
    nuevo_n->datos = nuevo;
    nuevo_n->izq = NULL;
    nuevo_n->der = NULL;

    if (parque->raiz_visitantes == NULL) {
        parque->raiz_visitantes = nuevo_n;
        return 1;
    }

    actual = parque->raiz_visitantes;
    nodo = NULL;

    while (actual != NULL) {
        nodo = actual;
        comp = nuevo->id - actual->datos->id;

        if (comp < 0) {
            actual = actual->izq; 
        } else if (comp > 0) {
            actual = actual->der; 
        } else {
            free(nuevo_n);
            free(nuevo->nombre);
            free(nuevo);
            return 0;
        }
    }

    if (comp < 0) {
        nodo->izq = nuevo_n;
    } else {
        nodo->der = nuevo_n;
    }
    
    return 1; 
}

int eliminar_visitante(struct NodoVisitantes **raiz_visitantes, char *rut) {
    struct NodoVisitantes *actual;
    struct NodoVisitantes *n;
    struct NodoVisitantes *ns;
    struct NodoVisitantes *sucesor;
    struct NodoVisitantes *n_sucesor;
    int comp, vef = 0;

    if (raiz_visitantes == NULL || *raiz_visitantes == NULL || rut == NULL) {
        return -1;
    }
    
    actual = *raiz_visitantes;
    n = NULL;

    while (actual != NULL && vef != 1) {
        comp = strcmp(rut, actual->datos->rut);
        
        if (comp == 0) {
            vef = 1;
        } else {
            n = actual;
            if (comp < 0) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }
        }
    }
    
    if (actual == NULL) {
        return -1; 
    }

    if (actual->izq == NULL || actual->der == NULL) {
        if (actual->izq == NULL) {
            ns = actual->der;
        } else {
            ns = actual->izq;
        }

        if (n == NULL) {
            *raiz_visitantes = ns;
        } else if (n->izq == actual) {
            n->izq = ns;
        } else {
            n->der = ns;
        }

        free(actual->datos->nombre);
        free(actual->datos);
        free(actual);
    } 
    else {
        n_sucesor = actual;
        sucesor = actual->der;

        while (sucesor->izq != NULL) {
            n_sucesor = sucesor;
            sucesor = sucesor->izq;
        }

        free(actual->datos->nombre);
        free(actual->datos);

        actual->datos = sucesor->datos;

        if (n_sucesor == actual) {
            n_sucesor->der = sucesor->der;
        } else {
            n_sucesor->izq = sucesor->der;
        }
        free(sucesor);
    }
    
    return 1; 
}