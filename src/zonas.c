#include <stdio.h>
#include "structs.h"


struct Zona crear_Zona (char *nombre, int codigo, char *tematica, struct tiempo *apertura, struct tiempo *cierre, int atracciones_max){
    struct Zona nuevaZona;
    nuevaZona.nombre = strdup(nombre);
    nuevaZona.codigo = codigo;
    nuevaZona.tematica = strdup(tematica);
    nuevaZona.horaApertura = apertura;
    nuevaZona.horaCierre = cierre;
    nuevaZona.atracciones_max = atracciones_max;
    nuevaZona.headAtracciones = NULL;
 
    return nuevaZona;
};


void Zona eliminar_Zona(struct Zona z){
    free(z.nombre);
    free(z.tematica);
    free(z.horaApertura);
    free(z.horaCierre);
    struct NodoAtraccion *actual = z.headAtracciones;
    while (actual){ 
        struct NodoAtraccion *temp = actual;
        actual = actual->sig;
        liberarAtraccion(temp->datos);
        free(temp);
    }
    z.headAtracciones = NULL;
};

int agregar_atraccion (struct Zona *z, struct Atraccion *a){
    if contarAtracciones(z) >= z->atracciones_max {
        return 0; // No se puede agregar más atracciones
    }
        struct NodoAtraccion *nuevo = malloc(sizeof(struct NodoAtraccion));
        nuevo->datos = a;
        nuevo->sig = NULL;

        if(z->headAtracciones == NULL) {
        z->headAtracciones = nuevo;
    } else {
        struct NodoAtraccion *actual = z->headAtracciones;
        while (actual->sig) actual = actual->sig;
        actual->sig = nuevo;
    }
    return 1;
};


int contarAtracciones(struct Zona *z) {
    int cont = 0;
    struct NodoAtraccion *actual = z->headAtracciones;
    while (actual){
         cont++; actual = actual->sig; 
        }
    return cont;
    };



/*struct Atraccion *buscarAtraccion(struct Zona *z, char *nombre) { 
*/