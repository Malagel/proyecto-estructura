#ifndef FILAS_H
#define FILAS_H

#include "structs.h"

/* Manejo basico de filas */
void inicializar_fila(struct Fila *fila);
int fila_vacia(struct Fila *fila);
int agregar_grupo_fila(struct Fila *fila, int ids_grupo[], int tam_grupo);
int quitar_grupo_fila(struct Fila *fila, int ids_salida[], int *tam_salida);
int contar_grupos_fila(struct Fila *fila);
int contar_personas_fila(struct Fila *fila);
void vaciar_fila(struct Fila *fila);
void mostrar_fila(struct Fila *fila);

/* Calculos de espera */
int calcular_ciclos_necesarios(int personas, int capacidad_por_ciclo);
int calcular_minutos_fila(struct Fila *fila, int capacidad_por_ciclo, int duracion_ciclo);

/* Manejo de filas asociadas a una atraccion */
int atraccion_operativa(struct Atraccion *atraccion);
int agregar_grupo_atraccion(struct Atraccion *atraccion, int ids_grupo[], int tam_grupo, int es_prioritaria);
int quitar_grupo_atraccion(struct Atraccion *atraccion, int es_prioritaria, int ids_salida[], int *tam_salida);
int contar_personas_atraccion(struct Atraccion *atraccion);
int contar_grupos_atraccion(struct Atraccion *atraccion);
int calcular_espera_atraccion(struct Atraccion *atraccion);
int calcular_espera_general_atraccion(struct Atraccion *atraccion);
int calcular_espera_prioritaria_atraccion(struct Atraccion *atraccion);
int atender_ciclo_atraccion(struct Atraccion *atraccion);
void suspender_filas_atraccion(struct Atraccion *atraccion, int vaciar_filas);
void mostrar_resumen_filas_atraccion(struct Atraccion *atraccion);

#endif
