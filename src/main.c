#include <stdio.h>

#include "structs.h"
#include "interfaz.h"
#include "utils.h"

void ejecutar_submenu_entradas(struct NodoEntradas **entradas);
void ejecutar_submenu_visitantes(struct Parque *parque);
void ejecutar_submenu_filas(struct Parque *parque);
void ejecutar_submenu_zonas(struct NodoZonas *head_zonas);
void ejecutar_submenu_atracciones(struct NodoZonas *head_zonas);

int main(void) {
    struct Parque *parque = NULL;
    int opcion_principal;
    int continuar_programa;

    continuar_programa = 1;

    parque = (struct Parque *)malloc(sizeof(*parque));
    menu_inicializar_parque(parque);

    while (continuar_programa) {
        limpiar_pantalla();
        mostrar_menu_principal();
        opcion_principal = escoger_opcion();

        switch (opcion_principal) {
            case 1:
                ejecutar_submenu_entradas(&(parque->head_entradas));
                break;
            case 2:
                ejecutar_submenu_visitantes(parque->raiz_visitantes);
                break;
            case 3:
                ejecutar_submenu_filas(parque);
                break;
            case 4:
                ejecutar_submenu_zonas(parque->head_zonas);
                break;
            case 5:
                ejecutar_submenu_atracciones(parque->head_zonas);
                break;
            case 6:
                ver_reporte_general_dia(parque);
                break;
            case 7:
                ver_reporte_atracciones_actual(parque->head_zonas);
                break;
            case 8:
                ver_atracciones_actuales(parque->head_zonas);
                break;
            case 9:
                ver_zonas_actuales(parque->head_zonas);
                break;
            case 10:
                ver_visitantes_parque(parque->raiz_visitantes);
                break;
            case 11:
                ver_entradas_compradas(parque->head_entradas);
                break;
            case 12:
                ver_filas_atracciones(parque->head_zonas);
            case 0:
                continuar_programa = 0;
                break;
            default:
                printf("\n[AVISO] Opcion invalida. Ingrese un numero entre 0 y 9.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }

    return 0;
}

void ejecutar_submenu_entradas(struct NodoEntradas **entradas) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        mostrar_submenu_entradas();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_comprar_entrada(entradas);
                break;
            case 2:
                menu_eliminar_entrada(entradas);
                break;
            case 3:
                menu_cambiar_estado_entrada(entradas);
                break;
            case 0:
                continuar = 0;
                break;
            default:
                printf("\n[AVISO] Opcion invalida en Submenu Entradas.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_visitantes(struct Parque *parque) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        mostrar_submenu_visitantes();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_visitante(parque);
                break;
            case 2:
                menu_eliminar_visitante(&parque->raiz_visitantes);
                break;
            case 0:
                continuar = 0;
                break;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_filas(struct Parque *parque) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        mostrar_submenu_filas();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_grupo_fila(parque);
                break;
            case 2:
                menu_avanzar_fila_atraccion(parque->head_zonas);
                break;
            case 0:
                continuar = 0;
                break;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_zonas(struct NodoZonas *head_zonas) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        mostrar_submenu_zonas();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_zona_al_parque(head_zonas);
                break;
            case 2:
                menu_eliminar_zona_del_parque(head_zonas);
                break;
            case 3:
                menu_agregar_o_remover_visitante_zona(head_zonas);
                break;
            case 0:
                continuar = 0;
                break;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_atracciones(struct NodoZonas *head_zonas) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        mostrar_submenu_atracciones();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_atraccion_zona(head_zonas);
                break;
            case 2:
                menu_eliminar_atraccion_zona(head_zonas);
                break;
            case 3:
                menu_mover_atraccion_distinta_zona(head_zonas);
                break;
            case 4:
                menu_modificar_estado_atraccion(head_zonas);
                break;
            case 0:
                continuar = 0;
                break;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}