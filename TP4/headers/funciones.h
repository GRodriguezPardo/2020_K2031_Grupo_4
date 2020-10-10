#include <stdio.h>

void setupFiles(FILE**);
void contarLineas(char*);

void concatenarStrings(char**, int, ...);
void copiarStrings(char**, int, ...);

void printearMensajeFinal();
void calcularCaracteres();
void imprimirLinea(int);
// Algunas variables globales
 
extern unsigned int line;
extern short hayError; // Un booleano seria perfecto pero no vale la pena importar la libreria, asi que usamos un short
extern char* format; // Para formateo de la salida. Ver funcion calcularCaracteres()

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
	int linea;
	struct NodoFuncion* siguiente;
} NodoFuncion;

void crearNodo(NodoFuncion**, char*, char*);
void terminarFuncion(NodoFuncion*);
void printearFuncion(NodoFuncion*);

/*
	SENTENCIAS
*/

typedef struct NodoSentencia {
	char* tipo;
	int linea;
	struct NodoSentencia* siguiente;
} NodoSentencia;

void agregarSentencia(NodoSentencia**, NodoSentencia**, char*, int);
void printearSentencia(NodoSentencia*);

/*
	ERRORES
*/

typedef struct NodoError {
	int linea;
	struct NodoError* siguiente;
} NodoError;

void agregarError(NodoError**, int);
void printearErrores(NodoError*);