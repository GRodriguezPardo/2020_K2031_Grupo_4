#include <stdlib.h>
#include "../headers/funciones.h"

void agregarError(NodoError** tail, int linea) {
	NodoError* nuevoNodo = (NodoError*) malloc(sizeof(NodoError));

	nuevoNodo->linea = linea;
	nuevoNodo->siguiente = NULL;

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
			printf("\tLinea");
			imprimirLinea(head->linea);
			printf("\n");
			head = head->siguiente;
		}
	}
}