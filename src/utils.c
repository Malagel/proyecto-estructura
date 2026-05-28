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