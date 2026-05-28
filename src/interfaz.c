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
        "                           Para salir escribe '0'\n"
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
        "[9] Ver Zonas Actuales\n\n"
        
        "- BUSCAR INFORMACIÓN\n"
        "[10] Buscar Visitante\n\n"
    );
}


void mostrar_submenu_entradas() {
    printf(
        "=========================================================================\n"
        "                              ++ ENTRADAS ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                       Para volver atrás escribe '0'\n"
        "=========================================================================\n\n"

        "[1] Comprar Entrada\n"
        "[2] Cambiar Estado de Entrada\n\n"
    );
}