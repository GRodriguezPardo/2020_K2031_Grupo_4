#include <stdlib.h>
#include <string.h>
#include "../headers/funciones.h"

void crearNodo(NodoFuncion** tail, char* especificadores, char* declarador) {
	NodoFuncion* newNode = (NodoFuncion*) malloc(sizeof(NodoFuncion));

	newNode->especificadores = malloc(strlen(especificadores) + 1);
	newNode->declarador = malloc(strlen(declarador) + 1);

	strcpy(newNode->especificadores, especificadores);
	strcpy(newNode->declarador, declarador);

	newNode->inicio = line;

	newNode->siguiente = NULL;

	if(*tail != NULL)
		(*tail)->siguiente = newNode;

	*tail = newNode;
}

void terminarFuncion(NodoFuncion* tail) {
	tail->final = line;
}

void printearFuncion(NodoFuncion* head) {
	printf("\n\n");
	printf("-----------------\n");
	printf("  FUNCIONES\n");
	printf("-----------------\n\n");

	while(head != NULL) {
		amarillo(0);
		printf("%d", head->inicio);
		blanco();
		printf(" | ");
		amarillo(0);
		printf("%d", head->final);
		blanco();
		printf(" | ");
		rojo(1);
		printf("%s", head->especificadores);
		verde(0);
		printf(" %s", head->declarador);
		blanco();
		printf("\n");
		head = head->siguiente;
	}
}