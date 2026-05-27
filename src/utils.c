#include <stdio.h>

void limpiar_pantalla(void) {
    printf("\033[2J\033[H");
}

void copiar_string(char *destino, const char *origen, size_t tamano_destino) {
    size_t i;

    if (tamano_destino == 0) {
        return;
    }

    for (i = 0; i < tamano_destino - 1 && origen[i] != '\0'; i++) {
        destino[i] = origen[i];
    }

    destino[i] = '\0';
}