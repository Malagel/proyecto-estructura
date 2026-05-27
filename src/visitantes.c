#include <string.h>

int contar_visitantes_dia(struct NodoVisitantes *visitantes, char* fecha_actual){
    if(visitantes == NULL || visitantes->datos == NULL){
        return 0;
    }
    int cont = 0;

    if(strcmp(visitantes->datos->entrada->fecha_ingreso, fecha_actual) == 0){
        cont ++; /*Sumamos ya que si entro el dia que queremos */
    }
    /*Aqui sumamos lo que se encuentra en el lado izquierdo del arbol y derecho, con el nodo principal. Y return al valor */
    return cont + contar_visitantes_dia(visitantes->izq, fecha_actual) + contar_visitantes_dia(visitantes->der, fecha_actual);
}

/*Funcion para ver cuantas personas estan dentro del parque*/
int personas_dentro_parque(struct NodoVisitantes *visitantes, char *fecha_actual) {
    if (visitantes == NULL || visitantes->datos == NULL) {
        return 0;
    }
    int cont = 0;

    if (strcmp(visitantes->datos->entrada->estado, "utilizada") == 0) {
        if(strcmp(visitantes-datos->entrada->fecha_ingreso, fecha_actual) == 0){
            cont++; /*Sumamos ya que la entrada es utilizada*/
        }
    }
    return cont + personas_dentro_parque(visitantes->izq) + personas_dentro_parque(visitantes->der);
}

int falta_para_cap_max(struct Parque *parque, char *fecha_actual){
    if(parque->raiz_visitantes == NULL){
        return parque->cap_max;
    }
    int personas_dentro = personas_dentro_parque(parque->NodoVisitantes, fecha_actual);

    /*Devolvemos la diferencia entre la capacidad maxima y las personas que estan adentro*/
    return parque->cap_max - personas_dentro; 
}

/*Seccion para agregar visitantes*/

int agregar_visitante(struct Parque *parque, struct Visitante *nuevo, char *fecha_actual){
    struct NodoVisitantes *nuevo_n;
    struct NodoVisitantes *actual;
    struct NodoVisitantes *nodo;
    int comp;
    
    if(parque == NULL || nuevo == NULL){
        return 0; /*0 es false*/
    }

    if(falta_para_cap_max(parque, fecha_actual) == 0){ 
        return 0;
    }

    /*Asignamos la memoria*/
    nuevo_n = (struct NodoVisitantes *)malloc(sizeof(struct NodoVisitantes));
    if(nuevo_n == NULL){
        return 0;
    }
    nuevo_n->datos = nuevo;
    nuevo_n->izq = NULL;
    nuevo_n->der = NULL;

    /*Agregamos el visitante en caso de que el arbol este vacio*/
    if(parque->raiz_visitantes == NULL){
        parque->raiz_visitantes = nuevo_n;
        return 1;
    }
    actual = parque->raiz_visitantes;
    nodo = NULL;

    while(actual != NULL){
        nodo = actual;
        comp = strcmp(nuevo->rut, actual->datos->rut);

        if(comp < 0){
            actual = actual->izq; /*Nos movemos a la izquierda*/
        } else if(comp > 0){
            actual = actual->der; /*Nos movemos a la derecha*/
        }else{
            free(nuevo_n)
            return 0;
        }
    }
    if(comp < 0){
        nodo->izq = nuevo_n;
    }else{
        nodo->der = nuevo_n;
    }
    return 1; 
}