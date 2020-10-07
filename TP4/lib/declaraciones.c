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

	if(*tail != NULL)
		(*tail)->siguiente = nuevoNodo;

	*tail = nuevoNodo;
}

void printearDeclaraciones(NodoDeclaracion* head) {
	printf("\n\n");
	printf("-----------------\n");
	printf("  DECLARACIONES\n");
	printf("-----------------\n\n");
	while(head != NULL) {
		amarillo(0);
		printf("Linea %d | ", head->linea);
		rojo(1);
		printf("%s ", head->especificadores);
		verde(0);
		printf("%s\n", head->declaracion);
		head = head->siguiente;
		blanco();
	}
}