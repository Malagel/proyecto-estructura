struct Parque{
    int cap_max;
    struct NodoVisitantes *headVisitante;
};

struct NodoVisitantes{
    struct visitante *V;
    struct NodoVisitantes *sig;
};

struct visitante{
    char *tipo;
    char *nombre;
    char *rut;
    struct entradas *entrada;
};

struct entradas{
    char *tipo;
    int codigo;
    int valor;
    char *estado;
    char *fechaIngreso;
};

struct Zona{
    char *nombre;
    int codigo;
    char *tematica;
    struct tiempo *horaApertura;
    struct tiempo *horaCierre;
    int atracciones_max;
    struct NodoAtraccion *headAtracciones;
};

struct tiempo{
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
    struct Fila *colaPobres;
    struct Fila *colaRicos;
};

struct NodoFila{
    int idvisitante;
    struct NodoFila *sig;
};

struct Fila{
    struct NodoFila *frente;
    struct NodoFila *final;
};