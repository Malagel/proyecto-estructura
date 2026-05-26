#include <string.h>

int contar_visitantes_dia(struct NodoVisitantes *visitantes, char *fecha_buscada){
    if(visitantes == NULL || visitantes->v == NULL){
        return 0;
    }

    int cont = 0;

    if(stcrmp(visitantes->v->entrada->fecha_ingreso, fecha_buscada) == 0){
        cont ++; /*Sumamos ya que si entro el dia que queremos */
    }


    /*Aqui sumamos lo que se encuentra en el lado izquierdo del arbol y derecho, con el nodo principal. Y return al valor */
    return contador + contar_visitantes_dia(visitantes->izq, fecha_buscada) + contar_visitantes_dia(visitantes->der, fecha_buscada);
};