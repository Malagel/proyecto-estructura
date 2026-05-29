#ifndef ZONAS_H
#define ZONAS_H

#include "structs.h"

struct Zona *crear_zona(int id, char *nombre, char *tematica,
                        struct Tiempo apertura, struct Tiempo cierre,
                        int cap_max, int atracciones_max);
void eliminar_zona(struct Zona *z);

struct Zona *buscar_zona(struct NodoZonas *head, int id);

int cambiar_nombre_zona(struct Zona *z, const char *nuevo_nombre);
int cambiar_tematica_zona(struct Zona *z, const char *nueva_tematica);
int cambiar_cap_max_zona(struct Zona *z, int nueva_cap);
int cambiar_horario_zona(struct Zona *z, struct Tiempo apertura, struct Tiempo cierre);
int cambiar_max_atracciones_zona(struct Zona *z, int nuevo_max);

#endif
