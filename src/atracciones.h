#include "structs.h"

#ifndef ATRACCIONES_H
#define ATRACCIONES_H

struct Atraccion *buscar_atraccion_por_id(struct NodoZonas *head_zonas, int id_atraccion);
struct Atraccion *obtener_atraccion_mayor_pico(struct Parque *parque);
struct Atraccion *obtener_atraccion_mas_visitada(struct Parque *parque);

int agregar_atraccion(struct NodoZonas *head_zonas, struct Zona *zona, const char *nombre, 
                    const char *tematica, int duracion, int cap_max, int edad_min, float altura_min);
int eliminar_atraccion(struct NodoZonas *head_zonas, int id_atraccion);
int mover_atraccion(struct NodoZonas *head_zonas, struct Zona *zona_objetivo, int id_atraccion);
int cambiar_estado_atraccion(struct NodoZonas *head_zonas, int id_atraccion, const char *nuevo_estado);

#endif
