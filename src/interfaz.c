#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filas.h"
#include "utils.h"
#include "visitantes.h"

extern int ENTRADA_GENERAL;
extern int ENTRADA_INFANTIL;
extern int ENTRADA_FAMILIAR;
extern int ENTRADA_PRIORITARIA;

int escoger_opcion(void) {
    char linea[100], basura;
    int asignados, numero;

    while (1) {
        printf(">> ");
        if (fgets(linea, sizeof(linea), stdin) != NULL) { 
            asignados = sscanf(linea, "%d %c", &numero, &basura);
            if (asignados == 1) 
                return numero; 
        }
        
        printf("[ERROR] Entrada no permitida. Intente de nuevo.\n\n");
    }
}


void menu_inicializar_parque(struct Parque *parque) {
    char linea[256];
    char token[30];
    char basura;
    int cap_max_buf;
    int general_buf;
    int infantil_buf;
    int familiar_buf;
    int prioritaria_buf;
    int asignados;
    int es_valido;

    es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                       ++ BIENVENIDO A IBCLANDIA ++\n"
        "=========================================================================\n"
        "     Para empezar a manejar el sistema el día de hoy, debe ingresar\n"
        "          los siguientes parámetros en una sola línea y en orden.\n"
        "                  Escriba 'salir' para cerrar el programa\n"
        "=========================================================================\n\n"

        " - Los parámetros son (separados por espacios):\n"
        "[1] Capacidad Máxima del Parque\n"
        "[2] Precio entrada GENERAL\n"
        "[3] Precio entrada INFANTIL\n"
        "[4] Precio entrada FAMILIAR\n"
        "[5] Precio entrada PRIORITARIA\n"
        "Por ejemplo: '200000 5000 2500 10000 8000'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "salir") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %d %d %d %d %c", 
                           &cap_max_buf, &general_buf, &infantil_buf, 
                           &familiar_buf, &prioritaria_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 5) {
            printf("[ERROR] Datos incompletos. Se procesaron solo %d de los 5 campos requeridos.\n\n", asignados);
            continue;
        }

        if (asignados > 5) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (cap_max_buf <= 0 || general_buf < 0 || infantil_buf < 0 || familiar_buf < 0 || prioritaria_buf < 0) {
            printf("[ERROR] Valores inválidos. La capacidad debe ser positiva y los precios no pueden ser negativos.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (parque != NULL) {
        parque->cap_max = cap_max_buf;
        
        parque->raiz_visitantes = NULL;
        parque->head_zonas = NULL;
        parque->head_entradas = NULL;

        ENTRADA_GENERAL = general_buf;
        ENTRADA_INFANTIL = infantil_buf;
        ENTRADA_FAMILIAR = familiar_buf;
        ENTRADA_PRIORITARIA = prioritaria_buf;

        printf("\n[SISTEMA] ¡Parque inicializado con éxito!\n");
        printf("Capacidad Máxima configurada: %d visitantes.\n", parque->cap_max);
        printf("Precios de las entradas actualizados en los registros globales.\n\n");
    } else {
        printf("\n[ERROR] El puntero al parque provisto es nulo (NULL). No se pudo inicializar.\n\n");
    }

    printf("Presione ENTER para continuar...");
    while (getchar() != '\n');


}

void mostrar_menu_principal(void) {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                          ++ SISTEMA IBCLANDIA ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                           Para salir escriba '0'\n"
        "=========================================================================\n\n"

        "- CONTROL Y MANEJO DE PERSONAS\n"
        "[1] Control y Compra de Entradas\n"
        "[2] Manejo de Visitantes en el Parque\n"
        "[3] Control Filas de Atracciones\n\n" 

        "- GESTIÓN DEL PARQUE\n"
        "[4] Gestionar Zonas\n"
        "[5] Gestionar Atracciones\n\n"
        
        "- REPORTES Y ESTADÍSTICAS\n"
        "[6] Ver Reporte General Del Día\n"
        "[7] Ver Reporte Sobre Atracciones Actual\n\n"

        "- INFORMACIÓN DEL PARQUE\n"
        "[8] Ver Atracciones Actuales\n"
        "[9] Ver Zonas Actuales\n"
        "[10] Ver Visitantes en el Parque\n"
        "[11] Ver Entradas Compradas\n"
        "[12] Ver Filas de Atracción\n\n"
    
        "=========================================================================\n\n"
    );
}


void mostrar_submenu_entradas() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                              ++ ENTRADAS ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"

        "[1] Comprar Entrada\n"
        "[2] Eliminar Entrada del Sistema\n"
        "[3] Cambiar Estado de Entrada\n\n"

        "=========================================================================\n\n"
    );
}

void menu_comprar_entrada(struct NodoEntradas **entradas) {
    char linea[150];
    char tipo_buf[30];
    int cantidad_buf;
    char basura;
    int asignados;
    int es_valido;
    
    int i;
    int precio_base;
    int precio_ticket;
    int exitos;
    char *fecha_actual;

    precio_base = 0;
    exitos = 0;
    es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                          ++ COMPRAR ENTRADA ++\n"
        "=========================================================================\n"
        " Para comprar una entrada y agregarla, rellene los siguientes parámetros \n"
        "                        in el orden que se indica.\n" 
        "               Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Tipo de Entrada y Cantidad a Comprar\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] Tipos de entrada: 'general', 'infantil', 'familiar', y 'prioritaria'\n"
        "[4] Comprar la entrada familiar permite el ingreso gratuito de hasta otros\n"
        "    tres miembros. Por lo que la cantidad máxima es de 4.\n"
        "[5] Por ejemplo: 'prioritaria 4'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer la entrada por teclado.\n\n");
            continue;
        }

        asignados = sscanf(linea, "%29s %d %c", tipo_buf, &cantidad_buf, &basura);

        if (asignados >= 1 && strcmp(tipo_buf, "volver") == 0) {
            return;
        }

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ninguna entrada válida. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el segundo parámetro. Debe ingresar la cantidad después del tipo.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió demasiados parámetros (máximo 2).\n\n");
            continue;
        }

        if (strcmp(tipo_buf, "general") != 0 && 
            strcmp(tipo_buf, "infantil") != 0 && 
            strcmp(tipo_buf, "familiar") != 0 && 
            strcmp(tipo_buf, "prioritaria") != 0) {
            
            printf("[ERROR] El tipo '%s' no es valido.\n", tipo_buf);
            printf("[INFO] Tipos permitidos estrictamente en minúsculas: [general, infantil, familiar, prioritaria]\n\n");
            continue;
        }

        if (cantidad_buf <= 0) {
            printf("[ERROR] La cantidad (%d) no es válida. Debe ser mayor a cero.\n\n", cantidad_buf);
            continue;
        }

        if (strcmp(tipo_buf, "familiar") == 0 && cantidad_buf > 4) {
            printf("[ERROR] La cantidad máxima permitida para entradas de tipo 'familiar' es de 4.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (strcmp(tipo_buf, "general") == 0) {
        precio_base = ENTRADA_GENERAL;
    } else if (strcmp(tipo_buf, "infantil") == 0) {
        precio_base = ENTRADA_INFANTIL;
    } else if (strcmp(tipo_buf, "familiar") == 0) {
        precio_base = ENTRADA_FAMILIAR;
    } else if (strcmp(tipo_buf, "prioritaria") == 0) {
        precio_base = ENTRADA_PRIORITARIA;
    }

    for (i = 0; i < cantidad_buf; i++) {
        if (strcmp(tipo_buf, "familiar") == 0 && i > 0) {
            precio_ticket = 0;
        } else {
            precio_ticket = precio_base;
        }

        if (comprar_entrada(entradas, tipo_buf, precio_ticket)) {
            exitos++;
        }
    }

    if (exitos == cantidad_buf) {
        printf("\n[SISTEMA] ¡%d entrada(s) de tipo '%s' registrada(s) y comprada(s) con éxito!\n", exitos, tipo_buf);
    } else if (exitos > 0) {
        printf("\n[ALERTA] Operación parcial: Solo se registraron %d de %d entradas solicitadas.\n", exitos, cantidad_buf);
    } else {
        printf("\n[ALERTA] El sistema rechazó la operacion de compra.\n");
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void menu_eliminar_entrada(struct NodoEntradas **entradas) {
    char linea[100];
    char token[30];
    int id_buf;
    char basura;
    int asignados;
    int es_valido;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                           ++ ELIMINAR ENTRADA ++\n"
        "=========================================================================\n"
        " Para eliminar una entrada del parque, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "               Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] El parámetro es únicamente la ID de la entrada\n"
        "[2] Por ejemplo: '345'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");
        
        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                printf("\n[SISTEMA] Operacion cancelada.\n");
                printf("Presione ENTER para continuar...");
                return; 
            }
        }

        asignados = sscanf(linea, "%d %c", &id_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numerico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada invalida. Escribio argumentos de mas.\n\n");
            continue;
        }

        if (id_buf <= 0) {
            printf("[ERROR] ID invalido. Los identificadores deben ser mayores a cero.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (eliminar_entrada(entradas, id_buf)) {
        printf("\n[SISTEMA] Entrada con ID %d eliminada exitosamente.\n", id_buf);
    } else {
        printf("\n[ERROR] No se pudo eliminar: No se encontro ninguna entrada con el ID %d.\n", id_buf);
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');

}

void menu_cambiar_estado_entrada(struct NodoEntradas **entradas) {
    char linea[150];
    char token_control[30];
    char estado_buf[30];
    int id_buf;
    char basura;
    int asignados;
    int es_valido;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                     ++ CAMBIAR ESTADO DE ENTRADA ++\n"
        "=========================================================================\n"
        " Para cambiar el estado de la entrada, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID de la entrada, y el nuevo estado\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] Los tipos de entrada son: 'activa', 'utilizada', 'vencida', y 'anulada'\n"
        "[4] Por ejemplo: '25 anulada'\n\n"

        "=========================================================================\n\n"
    );

    es_valido = 0;

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %29s %c", &id_buf, estado_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. El primer parámetro debe ser el ID numérico de la entrada.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el segundo parámetro. Debe ingresar el nuevo estado despues del ID.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió parámetros de más (máximo 2).\n\n");
            continue;
        }

        if (id_buf <= 0) {
            printf("[ERROR] El ID de la entrada (%d) no es valido. Debe ser mayor a cero.\n\n", id_buf);
            continue;
        }

        if (strcmp(estado_buf, "activa") != 0 && 
            strcmp(estado_buf, "utilizada") != 0 && 
            strcmp(estado_buf, "vencida") != 0 && 
            strcmp(estado_buf, "anulada") != 0) {
            
            printf("[ERROR] El estado '%s' no es valido.\n", estado_buf);
            printf("[INFO] Estados permitidos: [activa, utilizada, vencida, anulada]\n\n");
            continue;
        }


        es_valido = 1;
    }

    if (cambiar_estado_entrada(entradas, id_buf, estado_buf)) {
        printf("\n[SISTEMA] Estado de la entrada %d actualizado a '%s' con éxito\n", id_buf, estado_buf);
    } else {
        printf("\n[ERROR] No se encontró ninguna entrada con el ID %d o no se pudo modificar.\n", id_buf);
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}


void mostrar_submenu_visitantes() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                             ++ VISITANTES ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"
        
        "[1] Agregar Visitante al Parque\n"
        "[2] Eliminar Visitante del Parque\n\n"

        "=========================================================================\n\n"

    );
}

void menu_agregar_visitante(struct Parque *parque, struct NodoZonas *head_zonas) {
    char linea[256];
    char token_control[40];
    char nombre_buf[40];
    char rut_buf[15];
    char basura;
    
    int edad_buf;
    float altura_buf;
    int id_entrada_buf;
    
    int asignados;
    int es_valido;
    int resultado_logico;
    struct Entrada *entrada_visitante;

    es_valido = 0;
    entrada_visitante = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                          ++ AGREGAR VISITANTE ++\n"
        "=========================================================================\n"
        "      Para agregar a un visitante, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Nombre, RUT, Edad, Altura (mts) y la ID de Entrada\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] El nombre no debe contener espacios (ver ejemplo)\n"
        "[4] Por ejemplo: 'Juan_Perez 15936475-1 45 1.72 34'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%39s %14s %d %f %d %c", 
                           nombre_buf, rut_buf, &edad_buf, &altura_buf, &id_entrada_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 5) {
            printf("[ERROR] Datos incompletos. Se procesaron solo %d de los 5 campos requeridos.\n\n", asignados);
            continue;
        }

        if (asignados > 5) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (edad_buf < 0 || edad_buf > 120 || altura_buf <= 0.40f || altura_buf > 2.50f || id_entrada_buf <= 0) {
            printf("[ERROR] Valores numéricos incoherentes. Verifique edad, altura e ID de entrada.\n\n");
            continue;
        }

        entrada_visitante = buscar_entrada_por_id(head_zonas, id_entrada_buf);
        if (entrada_visitante == NULL) {
            printf("[ERROR] La entrada con ID %d no existe o no está registrada en el sistema.\n\n", id_entrada_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = agregar_visitante(parque, entrada_visitante, nombre_buf, rut_buf, edad_buf, altura_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] ¡Visitante '%s' registrado exitosamente con la entrada ID %d!\n", nombre_buf, id_entrada_buf);
            break;
            
        case -1:
            printf("\n[ERROR] Error del sistema: Estructura del Parque no inicializada (NULL).\n");
            break;
            
        case -2:
            printf("\n[ERROR] Error de memoria: No se pudo reservar espacio para el nuevo visitante.\n");
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void menu_eliminar_visitante(struct NodoVisitantes **raiz_visitantes) { 
    char linea[100];
    char token[30];
    char basura;
    int id_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                        ++ ELIMINAR VISITANTE ++\n"
        "=========================================================================\n"
        "      Para eliminar a un visitante, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] El parámetro es únicamente el ID del visitante\n"
        "[2] La eliminación es permanente\n"
        "[3] Por ejemplo: '67'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %c", &id_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numérico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = eliminar_visitante(raiz_visitantes, id_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] Visitante con ID %d eliminado exitosamente de los registros.\n", id_buf);
            break;
            
        case -1:
            printf("\n[ERROR] El árbol de visitantes está vacío o no ha sido inicializado (NULL).\n");
            break;
            
        case -2:
            printf("\n[ERROR] No se pudo eliminar: El visitante con ID %d no se encuentra registrado.\n", id_buf);
            break;
            
        case -3:
            printf("\n[ERROR] Estructura corrupta: Se detectó una anomalía crítica en la integridad del árbol.\n");
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void mostrar_submenu_filas() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                         ++ FILAS DE ATRACCIÓN ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"
        
        "[1] Agregar Grupo a la Fila\n"
        "[2] Avanzar Fila de Atracción\n\n"

        "=========================================================================\n\n"
    );
}

void menu_agregar_grupo_fila(struct Parque *parque) {

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                        ++ AGREGAR GRUPO A FILA ++\n"
        "=========================================================================\n"
        " Para agregar un grupo a la atracción, rellene los siguientes parámetros\n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: IDs del Grupo, Tipo de Fila, ID de Atracción\n"
        "[2] Los ID de los visitantes van separados por un espacio. Máximo 10.\n"
        "[3] Los tipo de fila pueden ser 'general' o 'prioritaria'\n"
        "[4] Por ejemplo: '1 30 9 12 general 14'\n\n"

        "=========================================================================\n\n"
    );


}

void menu_avanzar_fila_atraccion(struct NodoZonas *head_zonas) {
    
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                     ++ AVANZAR FILAS DE ATRACCION ++\n"
        "=========================================================================\n"
        "           Para avanzar las filas, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] El único parámetro es la ID de la atracción\n"
        "[2] Esto avanzará ambas filas dependiendo de su capacidad\n"
        "[4] Por ejemplo: '23'\n\n"

        "=========================================================================\n\n"
    );

   
}

void mostrar_submenu_zonas() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                                ++ ZONAS ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"

        "[1] Agregar Zona al Parque\n"
        "[2] Eliminar Zona Del Parque\n"
        "[3] Agregar Visitantes a Zona\n"
        "[4] Remover Visitantes de Zona\n\n"

        "=========================================================================\n\n"

    );

}

void menu_agregar_zona_al_parque(struct NodoZonas *head_zonas) {
    limpiar_pantalla();

}

void menu_agregar_visitante_zona(struct NodoZonas *head_zonas) {
    limpiar_pantalla();

}

void menu_remover_visitante_zona(struct NodoZonas *head_zonas) {
    limpiar_pantalla();

}



void mostrar_submenu_atracciones() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                             ++ ATRACCIONES ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"

        "[1] Agregar Atracción a Zona\n"
        "[2] Eliminar Atracción de Zona\n"
        "[3] Mover Atracción a Distinta Zona\n"
        "[4] Modificar Estado de Atracción\n\n" 

        "=========================================================================\n\n"
    );
}

void menu_agregar_atraccion_zona(struct NodoZonas *head_zonas) {
    char linea[256];
    char token_control[40];
    char nombre_buf[40];
    char tematica_buf[30];
    char basura;
    
    int id_zona_buf;
    int duracion_buf;
    int cap_max_buf;
    int edad_min_buf;
    float altura_min_buf;
    
    int asignados;
    int es_valido;
    int resultado_logico;
    struct Zona *zona_destino;

    es_valido = 0;
    zona_destino = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                     ++ AGREGAR ATRACCIÓN A ZONA ++\n"
        "=========================================================================\n"
        "       Para agregar una atracción, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID Zona, Nombre_Sin_Espacios, Temática,\n"
        "Duración, Personas por Ciclo, Edad_Min, y Altura Mínima (metros)]\n"
        "[2] Por ejemplo: '3 Trueno_del_Dragon Aventura 4 24 10 1.20'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %39s %29s %d %d %d %f %c",
                           &id_zona_buf, nombre_buf, tematica_buf, &duracion_buf,
                           &cap_max_buf, &edad_min_buf, &altura_min_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningun ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 7) {
            printf("[ERROR] Datos incompletos. Se procesaron solo %d de los 7 campos requeridos.\n\n", asignados);
            continue;
        }

        if (asignados > 7) {
            printf("[ERROR] Entrada inválida. Detectados elementos desconocidos al final de la línea.\n\n");
            continue;
        }

        if (id_zona_buf <= 0 || duracion_buf <= 0 || cap_max_buf <= 0 || edad_min_buf < 0 || altura_min_buf < 0.0f) {
            printf("[ERROR] Valores numericos incoherentes. Verifique que las cantidades sean positivas.\n\n");
            continue;
        }

        zona_destino = obtener_zona_por_id(head_zonas, id_zona_buf);
        if (zona_destino == NULL) {
            printf("[ERROR] La zona con ID %d no existe en el sistema del parque.\n\n", id_zona_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = agregar_atraccion(head_zonas, zona_destino, nombre_buf, tematica_buf, 
                                         duracion_buf, cap_max_buf, edad_min_buf, altura_min_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] ¡Atraccion '%s' agregada con exito a la zona ID %d!\n", nombre_buf, id_zona_buf);
            break;
            
        case -2:
            printf("\n[ERROR] Incompatibilidad: La tematica '%s' no coincide con la identidad de la zona.\n", tematica_buf);
            break;
            
        case -3:
            printf("\n[ERROR] Capacidad excedida: La zona ID %d no tiene espacio fisico para mas atracciones.\n", id_zona_buf);
            break;
            
        default:
            printf("\n[ERROR] Operacion rechazada. Codigo de error no especificado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}

void menu_eliminar_atraccion_zona(struct NodoZonas *head_zonas) {
    char linea[100];
    char token[30];
    char basura;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_atraccion_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                    ++ ELIMINAR ATRACCIÓN DE ZONA ++\n"
        "=========================================================================\n"
        "     Para eliminar una atracción, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] El único parámetro es la ID de la atracción.\n"
        "[2] Por ejemplo: '3'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");
        
        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return; 
            }
        }

        asignados = sscanf(linea, "%d %c", &id_atraccion_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numérico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_atraccion_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = eliminar_atraccion(head_zonas, id_atraccion_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] Atracción con ID %d eliminada exitosamente del parque.\n", id_atraccion_buf);
            break;
            
        case -1:
            printf("\n[ERROR] Error de sistema: La estructura de Zonas es nula.\n");
            break;
            
        case -2:
            printf("\n[ERROR] No se pudo eliminar: No se encontró ninguna atracción con el ID %d.\n", id_atraccion_buf);
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("Presione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void menu_mover_atraccion_distinta_zona(struct NodoZonas *head_zonas) {
    char linea[150];
    char token[30];
    char basura;
    int id_objetivo_buf;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;
    struct Zona *zona_objetivo;

    es_valido = 0;
    id_objetivo_buf = 0;
    id_atraccion_buf = 0;
    zona_objetivo = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                      ++ MOVER ATRACCION A ZONA ++\n"
        "=========================================================================\n"
        "       Para mover una atracción, rellene los siguientes parámetros \n"
        "                        en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID Zona objetivo, ID Atracción.\n"
        "[2] Deben estar separados por espacios.\n"
        "[2] Por ejemplo: '3 23'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %d %c", &id_objetivo_buf, &id_atraccion_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 2) {
            printf("[ERROR] Datos incompletos. Debe ingresar la ID de la zona objetivo y la ID de la atracción.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_objetivo_buf <= 0 || id_atraccion_buf <= 0) {
            printf("[ERROR] IDs inválidas. Los identificadores deben ser números enteros positivos.\n\n");
            continue;
        }

        zona_objetivo = obtener_zona_por_id(head_zonas, id_objetivo_buf);
        if (zona_objetivo == NULL) {
            printf("[ERROR] La zona objetivo con ID %d no existe en el sistema del parque.\n\n", id_objetivo_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = mover_atraccion(head_zonas, zona_objetivo, id_atraccion_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] ¡Atracción con ID %d movida con éxito a la zona objetivo %d!\n", 
                   id_atraccion_buf, id_objetivo_buf);
            break;
            
        case -2:
            printf("\n[ERROR] Incompatibilidad: La temática de la atracción no coincide con la identidad de la zona objetivo.\n");
            break;
            
        case -3:
            printf("\n[ERROR] Capacidad excedida: La zona objetivo con ID %d llegó al máximo de su capacidad.\n", id_objetivo_buf);
            break;
            
        case -4:
            printf("\n[ERROR] No encontrada: No se encontró ninguna atracción con el ID %d en ninguna zona del parque.\n", id_atraccion_buf);
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("Presione ENTER para regresar al menu principal...");
    while (getchar() != '\n');

}

void menu_modificar_estado_atraccion(struct NodoZonas *head_zonas) {
    char linea[150];
    char token[30];
    char estado_buf[30];
    char basura;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_atraccion_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                   ++ MODIFICAR ESTADO DE ATRACCION ++\n"
        "=========================================================================\n"
        "     Para modificar una atracción, rellene los siguientes parámetros \n"
        "                        en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID de Atracción, y nuevo estado.\n"
        "[2] Los estados posibles son: 'operativa', 'mantenimiento', 'cerrada' y,\n" 
        "'fuera_de_servicio'.\n"
        "[3] Todo debe estar en minúsculas y sin espacios.\n"
        "[4] Por ejemplo: '42 mantenimiento'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %29s %c", &id_atraccion_buf, estado_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. El primer parámetro debe ser el ID numérico de la atracción.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Datos incompletos. Debe ingresar el nuevo estado después del ID.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_atraccion_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        if (strcmp(estado_buf, "operativa") != 0 && 
            strcmp(estado_buf, "mantenimiento") != 0 && 
            strcmp(estado_buf, "cerrada") != 0 && 
            strcmp(estado_buf, "fuera_de_servicio") != 0) {
            
            printf("[ERROR] El estado '%s' no es válido.\n", estado_buf);
            printf("[INFO] Estados permitidos: [operativa, mantenimiento, cerrada, fuera_de_servicio]\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = cambiar_estado_atraccion(head_zonas, id_atraccion_buf, estado_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] Estado de la atracción %d actualizado a '%s' con éxito\n", 
                   id_atraccion_buf, estado_buf);
            break;
            
        case -1:
            printf("\n[ERROR] Error de sistema: La estructura de Zonas es nula (NULL).\n");
            break;
            
        case -2:
            printf("\n[ERROR] No encontrada: No se encontró ninguna atracción con el ID %d en el parque.\n", id_atraccion_buf);
            break;
            
        case -3:
            printf("\n[ERROR] Error crítico: Falta memoria en el sistema para procesar el cambio.\n");
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("Presione ENTER para regresar al menu principal...");
    while (getchar() != '\n');

}