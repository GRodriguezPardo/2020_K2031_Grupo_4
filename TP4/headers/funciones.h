#include <stdio.h>

void setupFiles(FILE**);
void contarLineas(char*);

void concatenarStrings(char**, int, ...);
void copiarStrings(char**, int, ...);

void printearMensajeFinal();

extern int line;
extern short hayError;

/*
	COLORES
*/

void rojo(int);
void verde(int);
void amarillo(int);
void azul(int);
void magenta(int);
void cyan(int);
void blanco();

/*
	DECLARACIONES
*/

typedef struct NodoDeclaracion {
	char* especificadores;
	char* declaracion;
	int linea;
	struct NodoDeclaracion* siguiente;
} NodoDeclaracion;

void agregarDeclaracion(NodoDeclaracion**, char*, char*);
void printearDeclaraciones(NodoDeclaracion*);

/*
	FUNCIONES
*/

typedef struct NodoFuncion {
	char* especificadores;
	char* declarador;
	int inicio;
	int final;
	struct NodoFuncion* siguiente;
} NodoFuncion;

void crearNodo(NodoFuncion**, char*, char*);
void terminarFuncion(NodoFuncion*);
void printearFuncion(NodoFuncion*);