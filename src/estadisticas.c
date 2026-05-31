#include <stdio.h>

#include "structs.h"
#include "visitantes.h"
#include "atracciones.h"
#include "entradas.h"
#include "filas.h"
#include "zonas.h"
#include "utils.h"

void ver_reporte_general_dia(struct Parque *parque) {
    struct Atraccion *mayor_pico; 
    struct Atraccion *mas_visitada;
    int total_dentro_parque, total_visitantes, mayor_espera, recaudacion;

    limpiar_pantalla();

    if (parque == NULL) {
        printf("[ERROR]: No se puede generar el reporte (Estructura del parque no inicializada).\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    mas_visitada = obtener_atraccion_mas_visitada(parque);
    mayor_pico = obtener_atraccion_mayor_pico(parque);

    if (mas_visitada == NULL || mayor_pico == NULL || mayor_pico->cap_max == 0) {
        printf("[ERROR]: No se puede generar el reporte (Datos insuficientes o inválidos en las atracciones).\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    mayor_espera = ((mayor_pico->pico_cola_general + mayor_pico->pico_cola_prioritaria) / mayor_pico->cap_max) * mayor_pico->duracion;

    recaudacion = calcular_recaudacion_entradas(parque);
    total_dentro_parque = total_personas_dentro_parque(parque);
    
    total_visitantes = total_personas_diario_parque(parque);

    printf("=========================================================================\n");
    printf("                      REPORTE GENERAL DIARIO DEL PARQUE\n");
    printf("=========================================================================\n\n");
    printf(" Flujo de Visitantes:\n");
    printf("  - Total de visitas registradas hoy  : %d\n", total_visitantes);
    printf("  - Personas activas dentro del parque: %d\n", total_dentro_parque);
    printf("-------------------------------------------------------------------------\n");
    printf(" Auditoría Financiera:\n");
    printf("  - Total recaudado por entradas      : $ %d\n", recaudacion);
    printf("-------------------------------------------------------------------------\n");
    printf(" Estadisticas de Atracciones:\n");
    printf("  - Atraccion mas concurrida          : %s (%d visitantes)\n", 
           mas_visitada->nombre, mas_visitada->visitantes_totales);
    printf("  - Atraccion con mayor pico de cola  : %s\n", 
           mayor_pico->nombre);
    printf("  - Tiempo maximo de espera calculado : %d min\n", 
           mayor_espera);
    printf("=========================================================================\n\n");
    
    printf("Presione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_reporte_atracciones_actual(struct NodoZonas *head_zonas) {
    struct NodoZonas *zona_act;
    struct NodoAtraccion *atr_act;
    
    struct Atraccion *top_atracciones[10];
    struct Zona *top_zonas[10];
    
    int num_top;
    int i;
    int j;
    int pos;

    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("\n[ERROR] No hay zonas registradas.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    num_top = 0;
    for (i = 0; i < 10; i++) {
        top_atracciones[i] = NULL;
        top_zonas[i] = NULL;
    }

    zona_act = head_zonas;

    while (zona_act != NULL) {
        if (zona_act->datos != NULL) {
            atr_act = zona_act->datos->head_atracciones;
            
            while (atr_act != NULL) {
                if (atr_act->datos != NULL) {
                    
                    pos = -1;
                    for (i = 0; i < 10; i++) {
                        if (top_atracciones[i] == NULL) {
                            pos = i;
                            break;
                        }
                        if (atr_act->datos->visitantes_totales > top_atracciones[i]->visitantes_totales) {
                            pos = i;
                            break;
                        }
                    }

                    if (pos != -1) {
                        for (j = 9; j > pos; j--) {
                            top_atracciones[j] = top_atracciones[j - 1];
                            top_zonas[j] = top_zonas[j - 1];
                        }
                        top_atracciones[pos] = atr_act->datos;
                        top_zonas[pos] = zona_act->datos;

                        if (num_top < 10) {
                            num_top++;
                        }
                    }
                }
                atr_act = atr_act->sig;
            }
        }
        zona_act = zona_act->sig;
    }

    if (num_top == 0) {
        printf("No hay atracciones registradas o con datos válidos en el parque.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    printf("\n=================================================================================\n");
    printf("                      RANKING TOP 10 ATRACCIONES MÁS VISITADAS                   \n");
    printf("=================================================================================\n");
    printf("%-4s | %-22s | %-15s | %-15s | %-10s\n", "Pos", "Atraccion", "Tematica", "Zona", "Visitas");
    printf("---------------------------------------------------------------------------------\n");

    for (i = 0; i < num_top; i++) {
        printf("%-4d | %-22s | %-15s | %-15s | %-10d\n",
               i + 1,
               top_atracciones[i]->nombre ? top_atracciones[i]->nombre : "Sin Nombre",
               top_atracciones[i]->tematica ? top_atracciones[i]->tematica : "Sin Tematica",
               top_zonas[i]->nombre ? top_zonas[i]->nombre : "Sin Zona",
               top_atracciones[i]->visitantes_totales);
    }
    printf("=================================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_atracciones_actuales(struct NodoZonas *head_zonas) {
    struct NodoZonas *zona_act;
    struct NodoAtraccion *atr_act;
    int contador_atracciones;
    
    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("Error: La lista de zonas del parque esta vacia.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    contador_atracciones = 0;
    zona_act = head_zonas;

    printf("\n====================================================================\n");
    printf("                    LISTADO GLOBAL DE ATRACCIONES                   \n");
    printf("====================================================================\n");
    printf("%-6s | %-25s | %s\n", "ID", "Nombre de la Atraccion", "Estado");
    printf("--------------------------------------------------------------------\n");

    while (zona_act != NULL) {
        if (zona_act->datos != NULL) {
            atr_act = zona_act->datos->head_atracciones;
            
            while (atr_act != NULL) {
                if (atr_act->datos != NULL) {
                    
                    printf("%-6d | %-25s | %s\n", 
                           atr_act->datos->id, 
                           atr_act->datos->nombre ? atr_act->datos->nombre : "Sin Nombre",
                           atr_act->datos->estado ? atr_act->datos->estado : "Sin Estado");
                    
                    contador_atracciones++;
                }
                atr_act = atr_act->sig;
            }
        }
        zona_act = zona_act->sig;
    }

    printf("--------------------------------------------------------------------\n");
    printf(" Total de atracciones actuales en el parque: %d\n", contador_atracciones);
    printf("====================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_zonas_actuales(struct NodoZonas *head_zonas) {
    struct NodoZonas *actual;
    int contador_zonas;
    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("[ERROR] La lista de zonas del parque esta vacía.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    contador_zonas = 0;
    actual = head_zonas;

    printf("\n================================================================================\n");
    printf("                            LISTADO GLOBAL DE ZONAS                             \n");
    printf("================================================================================\n");
    printf("%-6s | %-25s | %-20s | %s\n", "ID", "Nombre de la Zona", "Temática", "Visitantes Actuales");
    printf("--------------------------------------------------------------------------------\n");

    while (actual != NULL) {
        if (actual->datos != NULL) {
            
            printf("%-6d | %-25s | %-20s | %d\n",
                   actual->datos->id,
                   actual->datos->nombre ? actual->datos->nombre : "Sin Nombre",
                   actual->datos->tematica ? actual->datos->tematica : "Sin Temática",
                   actual->datos->visitantes_actuales);
            
            contador_zonas++;
        }
        actual = actual->sig;
    }

    printf("--------------------------------------------------------------------------------\n");
    printf(" Total de zonas actualmente registradas: %d\n", contador_zonas);
    printf("================================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

static void recorrer_e_imprimir_visitantes(struct NodoVisitantes *raiz, int *contador) {
    if (raiz == NULL) {
        return;
    }

    recorrer_e_imprimir_visitantes(raiz->izq, contador);

    if (raiz->datos != NULL) {
        printf("[%04d] %-18.18s ", 
               raiz->datos->id, 
               raiz->datos->nombre ? raiz->datos->nombre : "Sin Nombre");

        (*contador)++;

        if (*contador % 3 == 0) {
            printf("\n");
        } else {
            printf("| ");
        }
    }

    recorrer_e_imprimir_visitantes(raiz->der, contador);
}

void ver_visitantes_parque(struct NodoVisitantes *raiz_visitantes) {
    int total_visitantes;
    limpiar_pantalla();

    if (raiz_visitantes == NULL) {
        printf("[ERROR] El parque no tiene visitantes registrados en este momento.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    total_visitantes = 0;

    printf("\n===============================================================================\n");
    printf("                          REGISTRO GLOBAL DE VISITANTES                      \n");
    printf("===============================================================================\n");

    recorrer_e_imprimir_visitantes(raiz_visitantes, &total_visitantes);

    if (total_visitantes % 3 != 0) {
        printf("\n");
    }

    printf("-------------------------------------------------------------------------------\n");
    printf(" Total de visitantes actualmente en el parque: %d\n", total_visitantes);
    printf("===============================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_entradas_compradas(struct NodoEntradas *head_entradas) {
    struct NodoEntradas *actual;
    int contador_entradas;
    limpiar_pantalla();

    if (head_entradas == NULL) {
        printf("[ERROR] No hay entradas registradas en el sistema.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    contador_entradas = 0;
    actual = head_entradas;

    printf("\n============================================================\n");
    printf("                REGISTRO GLOBAL DE ENTRADAS                 \n");
    printf("============================================================\n");
    printf("%-6s | %-15s | %-8s | %-12s\n", "ID", "Tipo", "Valor", "Estado");
    printf("------------------------------------------------------------\n");

    while (actual != NULL) {
        if (actual->entrada != NULL) {
            
            /* Se imprime la información alineada incluyendo el estado */
            printf("%-6d | %-15s | %-8d | %-12s\n", 
                   actual->entrada->id, 
                   actual->entrada->tipo ? actual->entrada->tipo : "Sin Tipo",
                   actual->entrada->valor,
                   actual->entrada->estado ? actual->entrada->estado : "Sin Estado");
            
            contador_entradas++;
        }
        actual = actual->sig;
    }

    printf("------------------------------------------------------------\n");
    printf(" Total de entradas emitidas: %d\n", contador_entradas);
    printf("============================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}
void ver_filas_atracciones(struct NodoZonas *head_zonas) {
    struct NodoZonas *zona_act;
    struct NodoAtraccion *atr_act;
    struct NodoFila *act_fila;
    int primero;
    int k;
    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("[ERROR] No hay zonas registradas.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    zona_act = head_zonas;

    while (zona_act != NULL) {
        if (zona_act->datos != NULL) {
            atr_act = zona_act->datos->head_atracciones;
            
            while (atr_act != NULL) {
                if (atr_act->datos != NULL) {
                    
                    printf("[%03d] %s\n", 
                           atr_act->datos->id, 
                           atr_act->datos->nombre ? atr_act->datos->nombre : "Sin Nombre");

                    printf("  Prio: ");
                    act_fila = atr_act->datos->cola_prioritaria.frente;
                    primero = 1;
                    
                    while (act_fila != NULL) {
                        for (k = 0; k < act_fila->tam_grupo; k++) {
                            if (!primero) {
                                printf(" -> ");
                            }
                            printf("%d", act_fila->ids_grupo[k]);
                            primero = 0;
                        }
                        act_fila = act_fila->sig;
                    }
                    if (primero) {
                        printf("(Vacia)");
                    }
                    printf("\n");

                    printf("  Gen : ");
                    act_fila = atr_act->datos->cola_general.frente;
                    primero = 1;
                    
                    while (act_fila != NULL) {
                        for (k = 0; k < act_fila->tam_grupo; k++) {
                            if (!primero) {
                                printf(" -> ");
                            }
                            printf("%d", act_fila->ids_grupo[k]);
                            primero = 0;
                        }
                        act_fila = act_fila->sig;
                    }
                    if (primero) {
                        printf("(Vacia)");
                    }
                    
                    printf("\n----------------------------------------\n");
                }
                atr_act = atr_act->sig;
            }
        }
        zona_act = zona_act->sig;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}