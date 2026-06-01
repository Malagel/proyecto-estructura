## COMPILACIÓN

El archivo incluido está compilado en Linux, por lo que no es compatible con otros sistemas operativos. Para compilar manualmente usar comando make si está instalada, o simplemente usar el siguiente comando con GCC :

    gcc -Wall -Wextra -ansi -pedantic -O2 -o parque src/main.c src/atracciones.c src/entradas.c src/estadisticas.c src/filas.c src/interfaz.c src/utils.c src/visitantes.c src/zonas.c