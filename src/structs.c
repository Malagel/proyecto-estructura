struct Parque{
    int cap_max;
    struct NodoVisitantes *head_visitante;
};

struct NodoVisitantes{
    struct Visitante *v;
    struct NodoVisitantes *sig;
};

struct Visitante{
    char *tipo;
    char *nombre;
    char *rut;
    struct Entradas *entrada;
};

struct Entradas{
    int codigo;
    char *tipo;
    int valor;
    char *estado;
    char *fecha_ingreso;
};

struct Zona{
    int codigo;
    char *nombre;
    char *tematica;
    struct Tiempo *hora_apertura;
    struct Tiempo *hora_cierre;
    int atracciones_max;
    struct NodoAtraccion *head_atracciones;
};

struct Tiempo{
    int hora;
    int minutos;
};

struct NodoAtraccion{
    struct Atraccion *datos;
    struct NodoAtraccion *sig;
};

struct Atracción{
    char *nombre;
    int capacidad;
    int duracion;
    float altura;
    int edad;
    struct Fila *cola_pobres;
    struct Fila *cola_ricos;
};

struct NodoFila{
    int id_visitante;
    struct NodoFila *sig;
};

struct Fila{
    struct NodoFila *frente;
    struct NodoFila *final;
};