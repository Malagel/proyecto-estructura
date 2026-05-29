#include "structs.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comprar_entrada(struct NodoEntradas **entradas, char *tipo, int valor) {
    struct NodoEntradas *actual;
    struct Entrada *nueva_entrada;
    struct NodoEntradas *nuevo_nodo;
    int nuevo_id = 1;

    if (entradas == NULL || tipo == NULL) return -1;

    actual = *entradas;
    while (actual != NULL) {
        if (actual->entrada->id >= nuevo_id)
            nuevo_id = actual->entrada->id + 1;
        actual = actual->sig;
    }

    nueva_entrada = (struct Entrada *)malloc(sizeof(struct Entrada));
    nuevo_nodo = (struct NodoEntradas *)malloc(sizeof(struct NodoEntradas));

    if (nueva_entrada == NULL || nuevo_nodo == NULL) {
        if (nueva_entrada != NULL) free(nueva_entrada);
        if (nuevo_nodo != NULL) free(nuevo_nodo);
        return -1;
    }

    nueva_entrada->id = nuevo_id;
    nueva_entrada->valor = valor;

    nueva_entrada->tipo = copiar_string(tipo);
    if (nueva_entrada->tipo == NULL) {
        free(nueva_entrada);
        free(nuevo_nodo);
        return -1;
    }

    nueva_entrada->estado = copiar_string("activa");
    if (nueva_entrada->estado == NULL) {
        free(nueva_entrada->tipo);
        free(nueva_entrada);
        free(nuevo_nodo);
        return -1;
    }

    nuevo_nodo->entrada = nueva_entrada;
    nuevo_nodo->sig = *entradas;
    *entradas = nuevo_nodo;

    return nuevo_id;
}

int cambiar_estado_entrada(struct NodoEntradas **entradas, int id_entrada, char *nuevo_estado) {
    struct NodoEntradas *actual;
    char *copia_estado;

    if (entradas == NULL || nuevo_estado == NULL) return -1;

    actual = *entradas;
    while (actual != NULL) {
        if (actual->entrada->id == id_entrada) {
            copia_estado = copiar_string(nuevo_estado);
            if (copia_estado == NULL) return -1;
            free(actual->entrada->estado);
            actual->entrada->estado = copia_estado;
            return 1;
        }
        actual = actual->sig;
    }

    return -1;
}

int eliminar_entrada(struct NodoEntradas **entradas, int id_entrada) {
    struct NodoEntradas *actual;
    struct NodoEntradas *anterior;

    if (entradas == NULL) return -1;

    actual = *entradas;
    anterior = NULL;

    while (actual != NULL) {
        if (actual->entrada->id == id_entrada) {
            if (anterior == NULL)
                *entradas = actual->sig;
            else
                anterior->sig = actual->sig;

            free(actual->entrada->tipo);
            free(actual->entrada->estado);
            free(actual->entrada);
            free(actual);
            return 1;
        }
        anterior = actual;
        actual = actual->sig;
    }

    return -1;
}