CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -O2

SRC_DIR = src
TARGET = parque

OBJS = $(SRC_DIR)/main.o \
       $(SRC_DIR)/atracciones.o \
       $(SRC_DIR)/entrada.o \
       $(SRC_DIR)/estadisticas.o \
       $(SRC_DIR)/filas.o \
       $(SRC_DIR)/interfaz.o \
       $(SRC_DIR)/utils.o \
       # En la captura no se visualiza entrada.h; se asume que depende de structs.h
$(SRC_DIR)/entrada.o: $(SRC_DIR)/entrada.c $(GLOBAL_DEPS)
$(SRC_DIR)/estadisticas.o: $(SRC_DIR)/estadisticas.c $(SRC_DIR)/estadisticas.h $(GLOBAL_DEPS)
$(SRC_DIR)/filas.o: $(SRC_DIR)/filas.c $(SRC_DIR)/filas.h $(GLOBAL_DEPS)
$(SRC_DIR)/interfaz.o: $(SRC_DIR)/interfaz.c $(SRC_DIR)/interfaz.h $(GLOBAL_DEPS)
$(SRC_DIR)/utils.o: $(SRC_DIR)/utils.c $(SRC_DIR)/utils.h $(GLOBAL_DEPS)
$(SRC_DIR)/visitantes.o: $(SRC_DIR)/visitantes.c $(SRC_DIR)/visitantes.h $(GLOBAL_DEPS)
$(SRC_DIR)/zonas.o: $(SRC_DIR)/zonas.c $(SRC_DIR)/zonas.h $(GLOBAL_DEPS)

# ==========================================================================
# REGLA DE LIMPIEZA
# ==========================================================================
.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)