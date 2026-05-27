#ifndef ZONAS_H
#define ZONAS_H

#include "structs.h"

struct Zona crear_zona(char *nombre, int id, char *tematica,
                       struct Tiempo apertura, struct Tiempo cierre,
                       int cap_max, int atracciones_max);
struct Zona crear_Zona(char *nombre, int codigo, char *tematica,
                       struct Tiempo *apertura, struct Tiempo *cierre,
                       int atracciones_max);
void eliminar_zona(struct Zona *z);
void eliminar_Zona(struct Zona *z);

/* Busqueda, modificacion y listado */
struct Zona *buscar_Zona(struct NodoZonas *head, int id);
void modificar_Zona(struct Zona *z);
void listar_zona(struct Zona *z);
void listar_zonas(struct NodoZonas *head);

#endif

