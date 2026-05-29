#include "structs.h"

#ifndef VISITANTES_H
#define VISITANTES_H

int contar_visitantes_dia(struct NodoVisitantes *visitantes);
int personas_dentro_parque(struct NodoVisitantes *visitantes);
int falta_para_cap_max(struct Parque *parque);

int agregar_visitante(struct Parque *parque, char *nombre, char *rut, int edad, float altura);
int eliminar_visitante(struct NodoVisitantes **raiz_visitantes, char *rut);


#endif