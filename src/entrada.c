#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

void comprar_entrada(struct Parque *parque, int id, char *tipo, int valor, char *fecha){
    struct NodoEntradas *actual;
    int i;

    actual = parque->head_entradas;
    while(actual != NULL){
        if (actual->entrada->id==id){
            return;
        }
        actual=actual->sig;
    }

    struct Entrada *nueva_entrada = (struct Entrada*) malloc (sizeof(struct Entrada));
    struct NodoEntradas *nuevo_nodo = (struct NodoEntradas*)malloc(sizeof(struct NodoEntradas));

    if (nueva_entrada == NULL || nuevo_nodo == NULL){
        if (nueva_entrada != NULL) free(nueva_entrada);
        if (nuevo_nodo != NULL) free(nuevo_nodo);
        return;
    }

    nueva_entrada->id = id;
    nueva_entrada->valor=valor;
    nueva_entrada->tipo=tipo;
    nueva_entrada->estado="activa";

    
    for (i=0; i<10 && fecha[i] != '\0'; i++){
        nueva_entrada->fecha_ingreso[i]=fecha[i];
    }
    nueva_entrada->fecha_ingreso[i]='\0';

    nuevo_nodo->entrada=nueva_entrada;
    nuevo_nodo->sig = parque->head_entradas;
    parque->head_entradas = nuevo_nodo;
}




int cambiar_estado_entrada(struct Parque *parque, int id_entrada, char *nuevo_estado) {
    struct NodoEntradas *actual = parque->head_entradas;
    
    while (actual != NULL) {
        if (actual->entrada->id == id_entrada) {
            actual->entrada->estado = nuevo_estado;
            return 1;
        }
        actual = actual->sig;
    }
    
    return 0;
}
