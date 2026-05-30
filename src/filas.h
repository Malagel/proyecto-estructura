#include "structs.h"

#ifndef FILAS_H
#define FILAS_H

int agregar_grupo_fila(struct NodoVisitantes *raiz_visitantes, struct Atraccion *atraccion, int *ids_grupo, int tam_grupo, int es_prioritaria);
int vaciar_filas_atraccion(struct NodoZonas *head_zonas, int id_atraccion);
int avanzar_fila_atraccion(struct NodoZonas *head_zonas, int id_atraccion);

#endif
