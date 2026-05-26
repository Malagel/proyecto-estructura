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
    char *tipo;
    int codigo;
    int valor;
    char *estado;
    char *fecha_ingreso;
};

struct Zona{
    char *nombre;
    int codigo;
    char *tematica;
    struct tiempo *hora_apertura;
    struct tiempo *hora_cierre;
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