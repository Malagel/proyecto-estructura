struct Parque {
    int cap_max;
    struct NodoVisitantes *raiz_visitantes; /* árbol binario ordenado por rut */
    struct NodoZonas *head_zonas; /* Simplemente enlazada */
};

struct NodoVisitantes {
    struct Visitante *datos;
    struct NodoVisitantes *izq, *der;
};

struct Visitante {
    char *nombre;
    char rut[12];
    int edad;
    float altura;

    struct Entrada *entrada;
};

struct Entrada {
    int id;
    char *tipo;
    int valor;
    char *estado; /* ["activa", "utilizada", "vencida", "anulada"] */
    char fecha_ingreso[11]; /* Formato: DD-MM-YYYY */
};

struct NodoZonas {
    struct Zona *datos;
    struct NodoZonas *sig;
};

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

struct Tiempo {
    int hora;
    int minutos;
};

struct NodoAtraccion {
    struct Atraccion *datos;
    struct NodoAtraccion *sig;
};

struct Atraccion {
    char *nombre;
    char *estado; /* ["operativa", "mantenimiento", "cerrada", "fuera_de_servicio"] */
    char *tematica;
    int duracion; /* En minutos */

    int edad_min;
    float altura_min;

    struct Fila cola_general;
    struct Fila cola_prioritaria;

    int cap_max; /* Capacidad por ciclo */
    int visitantes_totales;

    int max_cola_general;
    int max_cola_prioritaria;
};

struct NodoFila {
    int ids_grupo[10]; /* ids_grupo[0] es el representante del grupo, la id de su entrada */
    int tam_grupo;
    struct NodoFila *sig;
};

struct Fila {
    struct NodoFila *frente;
    struct NodoFila *final;
};