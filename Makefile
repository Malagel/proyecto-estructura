CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -O2

SRC_DIR = src
TARGET = parque

OBJS = $(SRC_DIR)/main.o \
       $(SRC_DIR)/atracciones.o \
       $(SRC_DIR)/entradas.o \
       $(SRC_DIR)/estadisticas.o \
       $(SRC_DIR)/filas.o \
       $(SRC_DIR)/interfaz.o \
       $(SRC_DIR)/utils.o \
       $(SRC_DIR)/visitantes.o \
       $(SRC_DIR)/zonas.o

GLOBAL_DEPS = $(SRC_DIR)/structs.h

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/main.o: $(SRC_DIR)/main.c $(GLOBAL_DEPS)
$(SRC_DIR)/atracciones.o: $(SRC_DIR)/atracciones.c $(SRC_DIR)/atracciones.h $(GLOBAL_DEPS)
$(SRC_DIR)/estadisticas.o: $(SRC_DIR)/estadisticas.c $(SRC_DIR)/estadisticas.h $(GLOBAL_DEPS)
$(SRC_DIR)/filas.o: $(SRC_DIR)/filas.c $(SRC_DIR)/filas.h $(GLOBAL_DEPS)
$(SRC_DIR)/interfaz.o: $(SRC_DIR)/interfaz.c $(SRC_DIR)/interfaz.h $(GLOBAL_DEPS)
$(SRC_DIR)/utils.o: $(SRC_DIR)/utils.c $(SRC_DIR)/utils.h $(GLOBAL_DEPS)
$(SRC_DIR)/visitantes.o: $(SRC_DIR)/visitantes.c $(SRC_DIR)/visitantes.h $(GLOBAL_DEPS)
$(SRC_DIR)/zonas.o: $(SRC_DIR)/zonas.c $(SRC_DIR)/zonas.h $(GLOBAL_DEPS)
$(SRC_DIR)/entradas.o: $(SRC_DIR)/entradas.c $(SRC_DIR)/entradas.h $(GLOBAL_DEPS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)