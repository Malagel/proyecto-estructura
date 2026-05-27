#ifndef ATRACCIONES_H
#define ATRACCIONES_H

#include "structs.h"

char *copiar_texto(const char *texto);
int estado_atraccion_valido(const char *estado);
void liberar_datos_atraccion(struct Atraccion *a);
int contarGrupos(struct Fila *f);
int contar_Atracciones(struct Zona *z);

struct Atraccion *crearAtraccion(char *nombre, char *estado, char *tematica,
                                 int duracion, int edad_min, float altura_min,
                                 int cap_max, int max_cola_gral,
                                 int max_cola_prior);
int agregar_atraccion(struct Zona *z, struct Atraccion *a);
void eliminarAtraccion(struct Zona *z, char *nombre);
int cambiarEstadoAtraccion(struct Atraccion *a, char *nuevo_estado, int vaciar_si_no_operativa);
void modificarAtraccion(struct Atraccion *a);
void listarAtracciones(struct Zona *z);
struct Atraccion *buscar_Atraccion_Por_Nombre(struct Zona *z, char *nombre);
struct Atraccion *buscar_Atraccion_Por_Estado(struct Zona *z, char *estado);

/* Funciones puente hacia filas */
int agregarGrupoFilaAtraccion(struct Atraccion *a, int ids_grupo[], int tam_grupo, int es_prioritaria);
int quitarGrupoFilaAtraccion(struct Atraccion *a, int es_prioritaria, int ids_salida[], int *tam_salida);
int atenderAtraccion(struct Atraccion *a);
int tiempoEsperaAtraccion(struct Atraccion *a);
void mostrarFilasAtraccion(struct Atraccion *a);
void vaciarFilasAtraccion(struct Atraccion *a);

#endif
