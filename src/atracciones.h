#ifndef ATRACCIONES_H
#define ATRACCIONES_H

#include "structs.h"


char *copiar_texto(const char *texto);
int   estado_atraccion_valido(const char *estado);
int   atraccion_operativa(struct Atraccion *a);

void liberar_datos_atraccion(struct Atraccion *a);

struct Atraccion *crear_atraccion(char *nombre, char *estado, char *tematica,
                                  int duracion, int edad_min, float altura_min,
                                  int cap_max);
int  agregar_atraccion(struct Zona *z, struct Atraccion *a);
void eliminar_atraccion(struct Zona *z, char *nombre);

struct Atraccion    *buscar_atraccion_por_nombre(struct Zona *z, char *nombre);
struct NodoAtraccion *obtener_atracciones_zona(const struct Zona *z);
int                   contar_atracciones(struct Zona *z);

int cambiar_estado_atraccion(struct Atraccion *a, const char *nuevo_estado);

int cambiar_nombre_atraccion(struct Atraccion *a, const char *nuevo_nombre);
int cambiar_tematica_atraccion(struct Atraccion *a, const char *nueva_tematica);
int cambiar_duracion_atraccion(struct Atraccion *a, int nueva_duracion);
int cambiar_edad_minima_atraccion(struct Atraccion *a, int nueva_edad);
int cambiar_altura_minima_atraccion(struct Atraccion *a, float nueva_altura);
int cambiar_capacidad_atraccion(struct Atraccion *a, int nueva_cap);

void actualizar_pico_cola_general(struct Atraccion *a, int tam_actual);
void actualizar_pico_cola_prioritaria(struct Atraccion *a, int tam_actual);

#endif
