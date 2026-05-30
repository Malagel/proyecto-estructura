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
        
        case -3:
            printf("\n[ERROR] El Parque llegó a su límite máximo.\n");
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
    char linea[256];
    char token_control[40];
    char tipo_buf[30];
    char basura;
    int ids_grupo[10];

    char *ptr_linea;
    int caracteres_leidos;
    int asignados;
    int tam_grupo;
    int id_atraccion;
    int es_prioritaria;
    int es_valido;
    int id_extra;
    int resultado_logico;
    struct Atraccion *atr_encontrada;

    es_valido = 0;
    tam_grupo = 0;
    id_atraccion = 0;
    atr_encontrada = NULL;

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
        "[2] Máximo 10 IDs de integrantes pueden estar en el grupo.\n"
        "[3] Todos los parámetros deben estar separados por un espacio\n"
        "[4] Los tipo de fila pueden ser 'general' o 'prioritaria'\n"
        "[5] Por ejemplo: '1 30 9 12 general 14'\n\n"

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

        ptr_linea = linea;
        tam_grupo = 0;

        while (tam_grupo < 10 && sscanf(ptr_linea, "%d%n", &ids_grupo[tam_grupo], &caracteres_leidos) == 1) {
            tam_grupo++;
            ptr_linea += caracteres_leidos;
        }

        if (tam_grupo == 0) {
            printf("[ERROR] No se detectó ninguna ID de visitante al inicio. Intente de nuevo.\n\n");
            continue;
        }

        if (tam_grupo == 10) {
            if (sscanf(ptr_linea, "%d", &id_extra) == 1) {
                printf("[ERROR] Excedió el límite. El grupo puede contener un máximo de 10 visitantes.\n\n");
                continue;
            }
        }

        asignados = sscanf(ptr_linea, "%29s %d %c", tipo_buf, &id_atraccion, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Falta especificar el tipo de fila (general/prioritaria) y el ID de la atracción.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el último parámetro: Ingrese el ID de la atracción de destino.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Detectados elementos desconocidos al final de la línea.\n\n");
            continue;
        }

        if (strcmp(tipo_buf, "general") != 0 && strcmp(tipo_buf, "prioritaria") != 0) {
            printf("[ERROR] Tipo de fila '%s' no válido.\n", tipo_buf);
            printf("[INFO] Debe escribir exactamente 'general' o 'prioritaria' en minúsculas.\n\n");
            continue;
        }

        if (id_atraccion <= 0) {
            printf("[ERROR] El ID de la atracción debe ser un número entero positivo.\n\n");
            continue;
        }

        if (parque != NULL) {
            atr_encontrada = buscar_atraccion(parque->head_zonas, id_atraccion);
        }
        
        if (atr_encontrada == NULL) {
            printf("[ERROR] La atracción con ID %d no existe en el sistema del parque.\n\n", id_atraccion);
            continue;
        }

        es_valido = 1;
    }

    es_prioritaria = (strcmp(tipo_buf, "prioritaria") == 0) ? 1 : 0;

    resultado_logico = agregar_grupo_fila(parque->raiz_visitantes, atr_encontrada, ids_grupo, tam_grupo, es_prioritaria);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] Grupo de %d visitantes enviado exitosamente a la fila '%s'\n", tam_grupo, tipo_buf);
            break;
            
        case -1:
            printf("\n[ERROR] Operación rechazada: La atracción seleccionada no se encuentra operativa actualmente.\n");
            break;
            
        case -2:
            printf("\n[ERROR] Validación fallida: Una o más IDs ingresadas no corresponden a visitantes registrados.\n");
            break;
            
        case -3:
            printf("\n[ERROR] Restricción física: Uno o más integrantes del grupo no cumplen con la edad o altura mínima.\n");
            break;
            
        case -4:
            printf("\n[ERROR] Acceso denegado: Algún integrante no posee una entrada prioritaria cuando el canal lo requiere.\n");
            break;
            
        case -5:
            printf("\n[ERROR] Error crítico: No hay suficiente espacio en la memoria dinámica del sistema.\n");
            break;
            
        case -10:
            printf("\n[ERROR] Tamaño inválido: La cantidad de integrantes proporcionada no es permitida para este grupo.\n");
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}

void menu_avanzar_fila_atraccion(struct NodoZonas *head_zonas) {
    char linea[100];
    char token[30];
    char basura;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;
    struct Atraccion *atr_encontrada;

    es_valido = 0;
    id_atraccion_buf = 0;
    atr_encontrada = NULL;

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
        "[4] Por ejemplo:'23'\n\n"

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

        atr_encontrada = buscar_atraccion_por_id(head_zonas, id_atraccion_buf);
        if (atr_encontrada == NULL) {
            printf("[ERROR] No encontrada: No se encontró ninguna atracción con el ID %d.\n\n", id_atraccion_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = avanzar_fila_atraccion(head_zonas, id_atraccion_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] ¡Filas de la atracción %d avanzadas con éxito!\n", id_atraccion_buf);
            break;
            
        case -1:
            printf("\n[ERROR] No encontrada: La atracción con ID %d no pudo ser localizada en el sistema de filas.\n", id_atraccion_buf);
            break;
            
        case -2:
            printf("\n[ERROR] Operación omitida: Ambas filas (general y prioritaria) de la atracción %d están vacías.\n", id_atraccion_buf);
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("Presione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
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
        "[3] Agregar o Remover Visitantes de Zona\n"

        "=========================================================================\n\n"

    );

}

void menu_agregar_zona_al_parque(struct NodoZonas **head_zonas) {
    char linea[256];
    char token_control[40];
    char nombre_buf[40];
    char tematica_buf[30];
    char basura;

    int cap_max_buf;
    int h_ap_buf;
    int m_ap_buf;
    int h_ci_buf;
    int m_ci_buf;
    int max_atr_buf;

    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                              ++ AGREGAR ZONA ++\n"
        "=========================================================================\n"
        "         Para agregar una zona, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Nombre, Temática, Capacidad, Hora Apertura,\n" 
        "Hora Cierre, y Máximo de Atracciones\n"
        "[2] Deben estar separados por un espacio y los nombres juntos.\n"
        "[3] Los tiempos están en formato de 24 hrs."
        "[3] Por ejemplo: 'Tren_de_la_Muerte Terror 1000 1:00 5:30 10'\n\n"

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

        asignados = sscanf(linea, "%39s %29s %d %d:%d %d:%d %d %c",
                           nombre_buf, tematica_buf, &cap_max_buf, 
                           &h_ap_buf, &m_ap_buf, &h_ci_buf, &m_ci_buf, 
                           &max_atr_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 8) {
            printf("[ERROR] Datos incompletos o formato de hora incorrecto. Recuerde usar el formato HH:MM.\n");
            printf("[INFO] Se interpretaron con éxito sólo %d de los sub-parámetros.\n\n", asignados);
            continue;
        }

        if (asignados > 8) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (cap_max_buf <= 0 || max_atr_buf <= 0) {
            printf("[ERROR] La capacidad máxima y el máximo de atracciones deben ser números positivos.\n\n");
            continue;
        }

        if (h_ap_buf < 0 || h_ap_buf > 23 || m_ap_buf < 0 || m_ap_buf > 59) {
            printf("[ERROR] Hora de apertura inválida (%d:%02d). Ingrese un tiempo válido entre 00:00 y 23:59.\n\n", h_ap_buf, m_ap_buf);
            continue;
        }

        if (h_ci_buf < 0 || h_ci_buf > 23 || m_ci_buf < 0 || m_ci_buf > 59) {
            printf("[ERROR] Hora de cierre inválida (%d:%02d). Ingrese un tiempo válido entre 00:00 y 23:59.\n\n", h_ci_buf, m_ci_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = agregar_zona(head_zonas, nombre_buf, tematica_buf, cap_max_buf, 
                                    h_ap_buf, h_ci_buf, m_ap_buf, m_ci_buf, max_atr_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] ¡Zona '%s' de temática '%s' agregada con éxito al parque!\n", nombre_buf, tematica_buf);
            break;
            
        case -1:
            printf("\n[ERROR] Raíz nula: La estructura de control (head_zonas) no está inicializada.\n");
            break;
            
        case -2:
            printf("\n[ERROR] Error de memoria: No hay suficiente espacio dinámico en el sistema.\n");
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("Presione ENTER para regresar al menú principal..");
    while (getchar() != '\n');
}

void menu_eliminar_zona_del_parque(struct NodoZonas **head_zonas) {
    char linea[100];
    char token[30];
    char basura;
    int id_zona_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_zona_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                             ++ ELIMINAR ZONA ++\n"
        "=========================================================================\n"
        "       Para eliminar una zona, rellene los siguientes parámetros \n"
        "                        en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] El único parámetro es la ID de la Zona\n"
        "[2] Esto eliminará todas las atracciones con la zona del Parque\n"
        "[3] Por ejemplo: '21'\n\n"

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

        asignados = sscanf(linea, "%d %c", &id_zona_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numérico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_zona_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = eliminar_zona(head_zonas, id_zona_buf);

    switch (resultado_logico) {
        case 0:
            printf("\n[SISTEMA] Zona con ID %d y todas sus atracciones asociadas eliminadas con éxito.\n", id_zona_buf);
            break;
            
        case -1:
            printf("\n[ERROR] Lista vacía: No existen zonas registradas en el parque actualmente.\n");
            break;
            
        case -2:
            printf("\n[ERROR] No encontrada: No se encontró ninguna zona con el ID %d en el sistema.\n", id_zona_buf);
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("Presione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}

void menu_agregar_o_remover_visitante_zona(struct NodoZonas *head_zonas) {
    char linea[150];
    char token_control[40];
    char accion_buf[30];
    char basura;
    int cantidad_buf;
    int id_zona_buf;
    int asignados;
    int es_valido;
    int visitantes_param;
    int resultado_logico;

    es_valido = 0;
    cantidad_buf = 0;
    id_zona_buf = 0;
    visitantes_param = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                  ++ AGREGAR O REMOVER VISITANTE DE ZONA ++\n"
        "=========================================================================\n"
        " Para agregar o remover a un visitante, rellene los siguientes parámetros\n"
        "                        en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Acción, Cantidad, ID de la Zona.\n"
        "[2] Debe usar 'agregar' y 'remover' como acciones.\n"
        "[3] Por ejemplo: 'remover 23 2'\n\n"

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

        asignados = sscanf(linea, "%29s %d %d %c", accion_buf, &cantidad_buf, &id_zona_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 3) {
            printf("[ERROR] Datos incompletos. Debe ingresar la acción (agregar/remover), la cantidad y la ID de la zona.\n\n");
            continue;
        }

        if (asignados > 3) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (strcmp(accion_buf, "agregar") != 0 && strcmp(accion_buf, "remover") != 0) {
            printf("[ERROR] Acción '%s' no válida.\n", accion_buf);
            printf("[INFO] Use estrictamente las palabras 'agregar' o 'remover' en minúsculas.\n\n");
            continue;
        }

        if (cantidad_buf <= 0) {
            printf("[ERROR] La cantidad de visitantes debe ser un número entero positivo.\n\n");
            continue;
        }

        if (id_zona_buf <= 0) {
            printf("[ERROR] ID de la zona inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (strcmp(accion_buf, "agregar") == 0) {
        visitantes_param = cantidad_buf;
    } else {
        visitantes_param = -cantidad_buf;
    }

    resultado_logico = agregar_o_remover_visitantes_zona(head_zonas, id_zona_buf, visitantes_param);

    switch (resultado_logico) {
        case 0:
            if (visitantes_param > 0) {
                printf("\n[SISTEMA] Se agregaron %d visitantes con éxito a la zona ID %d.\n", cantidad_buf, id_zona_buf);
            } else {
                printf("\n[SISTEMA] Se removieron %d visitantes con éxito de la zona ID %d.\n", cantidad_buf, id_zona_buf);
            }
            break;
            
        case -1:
            printf("\n[ERROR] Lista nula: La estructura global de zonas no ha sido inicializada.\n");
            break;
            
        case -2:
            printf("\n[ERROR] No encontrada: La zona con ID %d no existe en el parque.\n", id_zona_buf);
            break;
            
        case -3:
            printf("\n[ERROR] Capacidad excedida: El ingreso de %d visitantes supera la capacidad máxima de la zona.\n", cantidad_buf);
            break;
            
        case -4:
            printf("\n[ERROR] Supera cantidad mínima: No se pueden remover %d visitantes porque el aforo quedaría menor a cero.\n", cantidad_buf);
            break;
            
        default:
            printf("\n[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("Presione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
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