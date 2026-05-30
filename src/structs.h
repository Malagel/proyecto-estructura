#ifndef STRUCTS_H
#define STRUCTS_H

/* ========================================================================== */
/* 1. DECLARACIONES */
/* ========================================================================== */

struct Entrada;
struct Visitante;
struct Zona;
struct Atraccion;
struct NodoVisitantes;
struct NodoEntradas;
struct NodoZonas;
struct NodoAtraccion;
struct NodoFila;

/* ========================================================================== */
/* 2. ESTRUCTURAS INDEPENDIENTES Y HOJA */
/* ========================================================================== */

struct Tiempo {
    int hora;
    int minutos;
};

struct Entrada {
    int id;
    char *tipo; /* ["general", "infantil", "familiar", "vip"] */
    int valor;
    char *estado; /* ["activa", "utilizada", "vencida", "anulada"] */
};

struct NodoFila {
    int ids_grupo[10]; /* ids_grupo[0] es el representante del grupo, la id del visitante */
    int tam_grupo;
    struct NodoFila *sig;
};

struct Fila {
    struct NodoFila *frente;
    struct NodoFila *final;
};

struct Visitante {
    int id;
    char *nombre;
    char rut[11];
    int edad;
    float altura;
    struct Entrada *entrada;
};

/* ========================================================================== */
/* 3. ESTRUCTURAS DEPENDIENTES */
/* ========================================================================== */

struct Zona {
    int id;
    char *nombre;
    char *tematica; 

    int cap_max;
    int visitantes_actuales;
    
    struct Tiempo hora_apertura;
    struct Tiempo hora_cierre;
    
    int atracciones_max;
    struct NodoAtraccion *head_atracciones;
};

struct Atraccion {
    int id;
    char *nombre;
    char *estado; /* ["operativa", "mantenimiento", "cerrada", "fuera_de_servicio"] */
    char *tematica;
    int duracion; /* En minutos */
    int cap_max; /* Capacidad por ciclo */
    
    int edad_min;
    float altura_min;
    
    struct Fila cola_general;
    struct Fila cola_prioritaria;
    
    int visitantes_totales;
    int pico_cola_general;
    int pico_cola_prioritaria;
};

/* ========================================================================== */
/* 4. NODOS */
/* ========================================================================== */

struct NodoVisitantes {
    struct Visitante *datos;
    struct NodoVisitantes *izq, *der;
};

struct NodoEntradas {
    struct Entrada *entrada;
    struct NodoEntradas *sig;
};

struct NodoZonas {
    struct Zona *datos;
    struct NodoZonas *sig;
};

struct NodoAtraccion {
    struct Atraccion *datos;
    struct NodoAtraccion *sig;
};

/* ========================================================================== */
/* 5. PARQUE */
/* ========================================================================== */

struct Parque {
    int cap_max;
    struct NodoVisitantes *raiz_visitantes; /* árbol binario ordenado por id del visitante */
    struct NodoEntradas *head_entradas; /* Simplemente enlazada */
    struct NodoZonas *head_zonas; /* Simplemente enlazada */
};

#endif