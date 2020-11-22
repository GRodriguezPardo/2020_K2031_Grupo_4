#include <stdlib.h>
#include <string.h>
#include "../headers/funciones.h"

void agregarError(NodoError** tail, int linea, char* tipo) {
	NodoError* nuevoNodo = (NodoError*) malloc(sizeof(NodoError));

	nuevoNodo->linea = line;
	nuevoNodo->siguiente = NULL;
	nuevoNodo->tipo = malloc(strlen(tipo) + 1);
	strcpy(nuevoNodo->tipo, tipo);

	if(*tail != NULL)
		(*tail)->siguiente = nuevoNodo;

	*tail = nuevoNodo;
}

void printearErrores(NodoError* head) {
	if(head != NULL) {
		rojo(1);
		printf("\n\n\t-----------\n");
		printf("\t  ERRORES\n");
		printf("\t-----------\n\n");

		while(head != NULL) {
			printf("\t");
			imprimirLinea(head->linea);
			printf(" | ");
			rojo(1);
			printf("Error %s", head->tipo);
			printf("\n");
			head = head->siguiente;
		}
		blanco();
	}
}