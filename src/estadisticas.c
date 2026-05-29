#include "structs.h"
#include "visitantes.h"

funcion_reporte_general_dia(struct Parque *parque) {
    int visitantes_totales, visitantes_dentro, visitantes_para_capacidad_maxima;
    
    visitantes_totales = contar_visitantes_dia(parque->raiz_visitantes);
    visitantes_dentro = personas_dentro_parque(parque->raiz_visitantes);
    visitantes_para_capacidad_maxima = falta_para_cap_max(parque);

    

}