#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

void ver_filas_atracciones(struct NodoZonas *head_zonas);
void ver_entradas_compradas(struct NodoEntradas *head_entradas);
void ver_visitantes_parque(struct NodoVisitantes *raiz_visitantes);
void ver_zonas_actuales(struct NodoZonas *head_zonas);
void ver_atracciones_actuales(struct NodoZonas *head_zonas);
void ver_reporte_atracciones_actual(struct NodoZonas *head_zonas);
void ver_reporte_general_dia(struct Parque *parque);

#endif