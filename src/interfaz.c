#include <stdio.h>
#include "utils.h"

int escoger_opcion(void) {
    char linea[100], basura;
    int asignados, numero;

    while (1) {
        printf("Escoge una opción: ");
        if (fgets(linea, sizeof(linea), stdin) != NULL) { 
            asignados = sscanf(linea, "%d %c", &numero, &basura);
        }

        if (asignados == 1) 
            return numero; 
        
        printf("[ERROR] Entrada no permitida. Intente de nuevo.\n\n");
    }
}

void mostrar_menu_principal(void) {
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
        "[6] Ver Reporte General Histórico\n"
        "[7] Ver Reporte Sobre Atracciones Actual\n\n"

        "- INFORMACIÓN DEL PARQUE\n"
        "[8] Ver Atracciones Actuales\n"
        "[9] Ver Zonas Actuales\n"
        "[10] Ver Visitantes Totales en el Parque\n\n"

    
        "=========================================================================\n\n"
    );
}


void mostrar_submenu_entradas() {
    printf(
        "=========================================================================\n"
        "                              ++ ENTRADAS ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"

        "[1] Comprar Entrada\n"
        "[2] Cambiar Estado de Entrada\n\n"

        "=========================================================================\n\n"

    );
}

void menu_comprar_entrada(entradas) {
    char linea[150];
    char tipo_buf[30];
    int valor_buf;
    char basura;
    int asignados;
    int es_valido;
    char *fecha_actual;

    printf(
        "=========================================================================\n"
        "                           ++ COMPRAR ENTRADA ++\n"
        "=========================================================================\n"
        " Para comprar una entrada y agregarla, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n" 
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n"

        "- INFORMACIÓN\n"
        "[1] Los parámetros son: tipo de entrada y valor de la entrada\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] Los tipos de entrada son: 'general', 'infantil', 'familiar', 'vip'\n"
        "[4] Por ejemplo: 'vip 15000'\n"

        "=========================================================================\n\n"
    );

    fecha_actual = obtener_fecha_actual(); 
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

    if (comprar_entrada(entradas, tipo_buf, valor_buf, fecha_actual)) {
        printf("\n[SISTEMA] ¡Entrada registrada y comprada con exito!\n");
    } else {
        printf("\n[ALERTA] El sistema rechazo la operacion de compra.\n");
    }

}

void mostrar_submenu_visitantes() {
    printf(
        "=========================================================================\n"
        "                             ++ VISITANTES ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"
        
        "[1] Agregar Visitante al Parque\n"
        "[2] Eliminar Visitante del Parque"
        "[3] Mover Visitantes a Zona\n"
        "[4] Buscar Visitante Por Nombre o RUT\n\n"

        "=========================================================================\n\n"

    );
}

void mostrar_submenu_filas() {
    printf(
        "=========================================================================\n"
        "                         ++ FILAS DE ATRACCIÓN ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"
        
        "[1] Agregar Grupo a la Fila Prioritaria\n"
        "[2] Agregar Grupo a la Fila General\n"
        "[3] Avanzar Fila de Atracción\n"
        "[4] Ver Estado de Filas de Atracción\n\n"

        "=========================================================================\n\n"
    );
}

void mostrar_submenu_zonas() {
    printf(
        "=========================================================================\n"
        "                                ++ ZONAS ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n"

        "[1] Agregar Zona al Parque\n"
        "[2] Aumentar Cantidad de Visitantes en Zona\n"
        "[3] "

        "=========================================================================\n\n"

    );

}

void mostrar_submenu_atracciones() {

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