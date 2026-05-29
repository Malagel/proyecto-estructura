#include <stdio.h>
#include "utils.h"
#include "visitantes.h"

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

void bienvenida_y_visitantes_parque() {
    printf(
        "Bienvenido a IBCLANDIA!\n"
        "Para empezar seleccione la capacidad máxima del parque:\n"
    );
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
        "[9] Ver Zonas Actuales\n\n"
    
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
    int valor_buf;
    char basura;
    int asignados;
    int es_valido;
    char *fecha_actual;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                           ++ COMPRAR ENTRADA ++\n"
        "=========================================================================\n"
        " Para comprar una entrada y agregarla, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "               Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: tipo de entrada y valor de la entrada\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] Los tipos de entrada son: 'general', 'infantil', 'familiar', y 'vip'\n"
        "[4] Por ejemplo: 'vip 15000'\n\n"

        "=========================================================================\n\n"
    );

    es_valido = 0;

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer la entrada por teclado.\n\n");
            continue;
        }

        asignados = sscanf(linea, "%29s %d %c", tipo_buf, &valor_buf, &basura);

        if (asignados >= 1 && strcmp(tipo_buf, "volver") == 0) {
            return;
        }

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ninguna entrada válida. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el segundo parámetro. Debe ingresar el precio después del tipo.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió demasiados parámetros (máximo 2).\n\n");
            continue;
        }

        if (strcmp(tipo_buf, "general") != 0 && 
            strcmp(tipo_buf, "infantil") != 0 && 
            strcmp(tipo_buf, "familiar") != 0 && 
            strcmp(tipo_buf, "vip") != 0) {
            
            printf("[ERROR] El tipo '%s' no es valido.\n", tipo_buf);
            printf("[INFO] Tipos permitidos estrictamente en minusculas: [general, infantil, familiar, vip]\n\n");
            continue;
        }

        if (valor_buf <= 0) {
            printf("[ERROR] El precio (%d) no es valido. Debe ser un monto mayor a cero.\n\n", valor_buf);
            continue;
        }

        es_valido = 1;
    }

    if (comprar_entrada(entradas, tipo_buf, valor_buf)) {
        printf("\n[SISTEMA] Entrada registrada y comprada con éxito\n");
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
        printf("\n[SISTEMA] Estado de la entrada %d actualizado a '%s' con exito\n", id_buf, estado_buf);
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

void menu_agregar_visitante(struct Parque *parque) {
    char linea[200];
    char nombre_buf[40];
    char rut_buf[15];
    int edad_buf;
    float altura_buf;
    char basura;
    int asignados;
    int es_valido;
    char *fecha_sistema;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                            ++ AGREGAR VISITANTE ++\n"
        "=========================================================================\n"
        "      Para agregar a un visitante, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: nombre, RUT, edad, y altura (metros)\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] El nombre no debe contener espacios (ver ejemplo)"
        "[4] Por ejemplo: 'Juan_Perez 15936475-1 45 1.72'\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {

        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", nombre_buf) == 1) {
            if (strcmp(nombre_buf, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%39s %14s %d %f %c", nombre_buf, rut_buf, &edad_buf, &altura_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ninguna entrada. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 4) {
            printf("[ERROR] Datos incompletos. Asegurese de ingresar los 4 campos obligatorios.\n");
            continue;
        }

        if (asignados > 4) {
            printf("[ERROR] Entrada inválida. Escribió parametros de más al final de la linea.\n\n");
            continue;
        }

        if (edad_buf < 0 || edad_buf > 120) {
            printf("[ERROR] La edad ingresada (%d) no es valida para el ingreso al parque.\n\n", edad_buf);
            continue;
        }

        if (altura_buf <= 0.40f || altura_buf > 2.50f) {
            printf("[ERROR] La altura (%.2fm) es incoherente. Ingrese el valor en metros (Ej: 1.75).\n\n", altura_buf);
            continue;
        }

        es_valido = 1;
    }

    if (agregar_visitante(parque, nombre_buf, rut_buf, edad_buf, altura_buf)) {
        printf("\n[SISTEMA] Visitante registrado exitosamente en el parque\n");
    } else {
        printf("\n[ERROR] El sistema no pudo agregar al visitante.\n");
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void menu_eliminar_visitante(struct Parque *parque) {
    char linea[100];
    char rut_buf[40];
    char basura;
    int asignados;
    int es_valido;
    size_t largo_rut;

    es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                        ++ ELIMINAR VISITANTE ++\n"
        "=========================================================================\n"
        "      Para agregar a un visitante, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] El parámetro es únicamente el RUT del visitante\n"
        "[2] La eliminación es permanente\n"
        "[3] Por ejemplo: '15936475-1'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", rut_buf) == 1) {
            if (strcmp(rut_buf, "volver") == 0) {
                return; 
            }
        }

        asignados = sscanf(linea, "%39s %c", rut_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectá ningun ingreso. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Ingrese ÚNICAMENTE el RUT del visitante.\n\n");
            continue;
        }

        largo_rut = strlen(rut_buf);

        if (largo_rut > 10) {
            printf("[ERROR] El RUT ingresado es demasiado largo (%lu caracteres).\n", (unsigned long)largo_rut);
            continue;
        }

        if (largo_rut < 7) {
            printf("[ERROR] El RUT ingresado es ridículamente corto. Verifique los datos.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (eliminar_visitante(parque, rut_buf)) {
        printf("\n[SISTEMA] El visitante con RUT %s fue eliminado exitosamente\n", rut_buf);
    } else {
        printf("\n[ERROR] No se pudo eliminar: El RUT %s no esta registrado en el parque.\n", rut_buf);
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
    char token_control[30];
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
    struct Atraccion *atr_encontrada;

    es_valido = 0;
    tam_grupo = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                        ++ AGREGAR GRUPO A FILA ++\n"
        "=========================================================================\n"
        " Para agregar un grupo a la atracción, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: IDs del grupo, tipo de fila, ID de atracción\n"
        "[2] Los ID de los visitantes van separados por un espacio. Máximo 10.\n"
        "[3] Los tipo de fila pueden ser 'general' o 'prioritaria'\n"
        "[4] Por ejemplo: '1 30 9 12 general 14'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        print(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token_control) == 1) {
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
                printf("[ERROR] Excedió el límite. El grupo puede contener un maximo de 10 visitantes.\n\n");
                continue;
            }
        }

        asignados = sscanf(ptr_linea, "%29s %d %c", tipo_buf, &id_atraccion, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Falta especificar el tipo de fila (general/prioritaria) y el ID de la atraccion.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el último parámetro: Ingrese el ID de la atraccion de destino.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada invalida. Detectados elementos desconocidos al final de la linea.\n\n");
            continue;
        }

        if (strcmp(tipo_buf, "general") != 0 && strcmp(tipo_buf, "prioritaria") != 0) {
            printf("[ERROR] Tipo de fila '%s' no valido.\n", tipo_buf);
            printf("[INFO] Debe escribir exactamente 'general' o 'prioritaria' en minúsculas.\n\n");
            continue;
        }

        if (id_atraccion <= 0) {
            printf("[ERROR] El ID de la atraccion debe ser un numero positivo.\n\n");
            continue;
        }

        atr_encontrada = buscar_atraccion(parque->head_zonas, id_atraccion);
        if (atr_encontrada == NULL) {
            printf("[ERROR] La atraccion con ID %d no existe en el parque.\n\n", id_atraccion);
            continue;
        }

        es_valido = 1;
    }

    es_prioritaria = (strcmp(tipo_buf, "prioritaria") == 0) ? 1 : 0;

    if (agregar_grupo_atraccion(parque->raiz_visitantes, atr_encontrada, ids_grupo, tam_grupo, es_prioritaria)) {
        printf("\n[SISTEMA] ¡Grupo de %d visitantes enviado a la fila '%s' con exito!\n", tam_grupo, tipo_buf);
    } else {
        printf("\n[ERROR] El sistema denegó la inserción del grupo a la atracción.\n");
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void menu_avanzar_fila_atraccion(struct NodoZonas *head_zonas) {
    char linea[100];
    char token[30];
    char basura;
    int id_atraccion;
    int asignados;
    int es_valido;
    struct Atraccion *atr_encontrada;

    es_valido = 0;
    id_atraccion = 0;
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
        "[4] Por ejemplo: '23'\n\n"

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
                return;
            }
        }

        asignados = sscanf(linea, "%d %c", &id_atraccion, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numerico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada invalida. Escribio argumentos de mas al final de la linea.\n\n");
            continue;
        }

        if (id_atraccion <= 0) {
            printf("[ERROR] ID invalido. El identificador debe ser un numero entero positivo.\n\n");
            continue;
        }

        atr_encontrada = buscar_atraccion_en_zonas(head_zonas, id_atraccion);
        if (atr_encontrada == NULL) {
            printf("[ERROR] La atraccion con ID %d no existe en el sistema del parque.\n\n", id_atraccion);
            continue;
        }

        es_valido = 1;
    }

    if (atender_ciclo_atraccion(atr_encontrada)) {
        printf("\n[SISTEMA] Ciclo ejecutado con éxito. La fila de espera ha avanzado.\n");
    } else {
        printf("\n[ERROR] No se pudo avanzar la fila.\n");
    }

    printf("Presione ENTER para continuar...");
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
        "[2] Remover Atracción de Zona\n"
        "[3] Mover Atracción a Distinta Zona\n"
        "[4] Modificar Estado de Atracción\n\n" 

        "=========================================================================\n\n"
    );
}

void menu_agregar_atraccion_zona(struct NodoZonas *head_zonas) {
    limpiar_pantalla();

}

void menu_remover_atraccion_zona(struct NodoZonas *head_zonas) {
    limpiar_pantalla();

}

void menu_mover_atraccion_distinta_zona(struct NodoZonas *head_zonas) {
    limpiar_pantalla();

}

void menu_modificar_estado_atraccion(struct NodoZonas *head_zonas) {
    limpiar_pantalla();

}