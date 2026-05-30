#ifndef INTERFAZ_H
#define INTERFAZ_H

extern int ENTRADA_GENERAL;
extern int ENTRADA_INFANTIL;
extern int ENTRADA_FAMILIAR;
extern int ENTRADA_PRIORITARIA;

int escoger_opcion(void);

void menu_inicializar_parque(struct Parque *parque);
void mostrar_menu_principal(void);

void mostrar_submenu_entradas(void);
void menu_comprar_entrada(struct NodoEntradas **entradas);
void menu_cambiar_estado_entrada(struct NodoEntradas **entradas);

void mostrar_submenu_visitantes(void);
void menu_agregar_visitante(struct Parque *parque);
void menu_eliminar_visitante(struct Parque *parque);

void mostrar_submenu_filas(void);
void menu_agregar_grupo_fila_prioritaria(struct NodoZonas *head_zonas);
void menu_agregar_grupo_fila_general(struct NodoZonas *head_zonas);
void menu_avanzar_fila_atraccion(struct NodoZonas *head_zonas);
void menu_ver_estado_filas_atraccion(struct NodoZonas *head_zonas);

void mostrar_submenu_zonas(void);
void menu_agregar_zona_al_parque(struct NodoZonas *head_zonas);
void menu_agregar_visitante_zona(struct NodoZonas *head_zonas);
void menu_remover_visitante_zona(struct NodoZonas *head_zonas);

void mostrar_submenu_atracciones(void);
void menu_agregar_atraccion_zona(struct NodoZonas *head_zonas);
void menu_remover_atraccion_zona(struct NodoZonas *head_zonas);
void menu_mover_atraccion_distinta_zona(struct NodoZonas *head_zonas);
void menu_modificar_estado_atraccion(struct NodoZonas *head_zonas);


#endif