#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "structs.h"

void limpiar_pantalla(void) {
    printf("\033[2J\033[3J\033[H");
    fflush(stdout);
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


struct Tiempo obtener_hora_actual(void) {
    time_t tiempo_segundos;
    struct tm *tiempo_local;
    struct Tiempo hora_sistema;

    tiempo_segundos = time(NULL);

    tiempo_local = localtime(&tiempo_segundos);

    if (tiempo_local != NULL) {
        hora_sistema.hora = tiempo_local->tm_hour;
        hora_sistema.minutos = tiempo_local->tm_min;
    } else {
        hora_sistema.hora = 0;
        hora_sistema.minutos = 0;
    }

    return hora_sistema;
}