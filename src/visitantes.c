#include <string.h>


/*Funcion para contar los visitantes de todo el dia*/
int contar_visitantes_dia(struct NodoVisitantes *visitantes, char *fecha_buscada){
    if(visitantes == NULL || visitantes->datos == NULL){
        return 0;
    }

    int cont = 0;

    if(strcmp(visitantes-datos->entrada->fecha_ingreso, fecha_buscada) == 0){
        cont ++; /*Sumamos ya que si entro el dia que queremos */
    }


    /*Aqui sumamos lo que se encuentra en el lado izquierdo del arbol y derecho, con el nodo principal. Y return al valor */
    return cont + contar_visitantes_dia(visitantes->izq, fecha_buscada) + contar_visitantes_dia(visitantes->der, fecha_buscada);
};

/*Funcion para ver cuantas personas estan dentro del parque*/
int personas_dentro_parque(struct NodoVisitantes *visitantes) {
    if (visitantes == NULL || visitantes->datos == NULL) {
        return 0;
    }

    int cont = 0;

    if (strcmp(visitantes->datos->entrada->estado, "utilizada") == 0) {
        cont++; /*Sumamos ya que la entrada es utilizada*/
    }

    /*Ahora la misma suma que en la función anterior*/
    return cont + personas_dentro_parque(visitantes->izq) + personas_dentro_parque(visitantes->der);
};

/*Funcion para ver cuanto falta para llegar a la capacidad maxima del parque*/
int falta_para_cap_max(struct Parque *parque){
    if(parque->raiz_visitantes == NULL){
        return parque->cap_max;
    }

    int personas_dentro = personas_dentro_parque(parque->NodoVisitantes);

    return parque->cap_max - personas_dentro; 
    /*Devolvemos la diferencia entre la capacidad maxima y las personas que estan adentro*/
}