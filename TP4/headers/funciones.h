#include <stdio.h>

void setupFiles(FILE**);
void contarLineas(char*);

void concatenarStrings(char**, int, ...);
void copiarStrings(char**, int, ...);
extern int line;

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