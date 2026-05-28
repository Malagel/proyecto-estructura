#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void limpiar_pantalla(void) {
    printf("\033[2J\033[H");
}

char *copiar_string(const char *origen) {
    char *destino;
    size_t tamano;

    if (origen == NULL) {
        return NULL;
    }

    tamano = strlen(origen) + 1;

    destino = (char *)malloc(tamano);

    if (destino != NULL) {
        strcpy(destino, origen); 
    }

    return destino; 
}

char* obtener_fecha_actual(char *buffer) {
    if (buffer == NULL) {
        return NULL;
    }

    time_t tiempo_actual = time(NULL);
    struct tm *info_tiempo = localtime(&tiempo_actual);

    if (info_tiempo == NULL) {
        buffer[0] = '\0';
        return NULL;
    }

    strftime(buffer, 11, "%d-%m-%Y", info_tiempo);

    return buffer;
}