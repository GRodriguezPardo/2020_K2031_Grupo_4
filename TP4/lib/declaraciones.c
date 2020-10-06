#include <stdlib.h>
#include <string.h>
#include "../headers/funciones.h"

// Recordar que la linea es una variable global

void agregarDeclaracion(NodoDeclaracion** tail, char* especificadores, char* declaracion) {
	NodoDeclaracion* nuevoNodo = (NodoDeclaracion*) malloc(sizeof(NodoDeclaracion));

	nuevoNodo->especificadores = (char*) malloc(strlen(especificadores) + 1);
	nuevoNodo->declaracion = (char*) malloc(strlen(declaracion) + 1);
	nuevoNodo->linea = line;

	strcpy(nuevoNodo->especificadores, especificadores);
	strcpy(nuevoNodo->declaracion, declaracion);

	nuevoNodo->siguiente = NULL;

	if(*tail == NULL)
		*tail = nuevoNodo;
	else {
		(*tail)->siguiente = nuevoNodo;
		*tail = nuevoNodo;
	}
}

void printearDeclaraciones(NodoDeclaracion* head) {
	printf("\n\n");
	printf("-----------------\n");
	printf("  DECLARACIONES\n");
	printf("-----------------\n\n");
	while(head != NULL) {
		printf("\033[0;33m	");
		printf("Linea %d | ", head->linea);
		printf("\033[1;31m");
		printf("%s ", head->especificadores);
		printf("\033[0;32m");
		printf("%s\n", head->declaracion);
		head = head->siguiente;
	}
}