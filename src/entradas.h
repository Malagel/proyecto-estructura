#ifndef ENTRADAS_H
#define ENTRADAS_H

struct Entrada *buscar_entrada_por_id(struct NodoZonas *head_zonas, int id);

int calcular_recaudacion_entradas(struct Parque *parque);
int comprar_entrada(struct NodoEntradas **entradas, char *tipo, int valor);
int cambiar_estado_entrada(struct NodoEntradas **entradas, int id_entrada, char *nuevo_estado);
int eliminar_entrada(struct NodoEntradas **entradas, int id_entrada);

#endif