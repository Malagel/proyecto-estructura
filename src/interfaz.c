#include <stdio.h>

void mostrar_menu_principal() {
    printf(
        "=========================================================================\n"
        "                          ++ SISTEMA IBCLANDIA ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n" 
        "                           Para salir escribe '0'\n"
        "=========================================================================\n"

        "- CONTROL Y MANEJO DE PERSONAS\n"
        "[1] Control y Compra de Entradas\n"
        "[2] Manejo de Visitantes en el Parque\n"
        "[3] Control Filas de Atracciones\n\n" 

        "- GESTIÓN DEL PARQUE"
        "[3] Gestionar Zonas\n"
        "[4] Gestionar Atracciones\n\n"
        
        "- REPORTES Y ESTADÍSTICAS\n"
        "[5] Ver Reporte General Histórico\n"
        "[6] Ver Reporte Sobre Atracciones Actual\n\n"

        "- INFORMACIÓN DEL PARQUE\n"
        "[7] Ver Atracciones Actuales\n"
        "[8] Ver Zonas Actuales\n\n"
        
        "- BUSCAR INFORMACIÓN\n"
        "[9] Buscar Visitante\n"
    );
}