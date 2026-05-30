#include "structs.h"

#ifndef VISITANTES_H
#define VISITANTES_H

struct Visitante* buscar_visitante_por_id(struct NodoVisitantes *raiz, int id);

int total_personas_diario_parque(struct Parque *parque);
int total_personas_dentro_parque(struct Parque *parque);

int agregar_visitante(struct Parque *parque, struct Entrada *entrada, char *nombre, char *rut, int edad, float altura);
int eliminar_visitante(struct NodoVisitantes **raiz_visitantes, int id);

#endif