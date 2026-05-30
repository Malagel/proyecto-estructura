#include "structs.h"

#ifndef ZONAS_H
#define ZONAS_H

struct Zona *obtener_zona_por_id(struct NodoZonas *head_zonas, int id_zona);
int agregar_zona(struct NodoZonas **head_zonas, const char *nombre, const char *tematica, 
                 int cap_max, int hora_apertura, int hora_cierre, int min_apertura, 
                 int min_cierre, int max_atracciones);
int eliminar_zona(struct NodoZonas **head_zonas, int id);
int agregar_o_remover_visitantes_zona(struct NodoZonas *head_zonas, int id, int visitantes);

#endif
