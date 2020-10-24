#include <stdlib.h>
#include <string.h>
#include "../headers/funciones.h"

void crearNodo(NodoFuncion** tail, char* especificadores, char* declarador) {
	NodoFuncion* newNode = (NodoFuncion*) malloc(sizeof(NodoFuncion));

	newNode->especificadores = malloc(strlen(especificadores) + 1);
	newNode->declarador = malloc(strlen(declarador) + 1);

	strcpy(newNode->especificadores, especificadores);
	strcpy(newNode->declarador, declarador);

	newNode->linea = line;

	newNode->siguiente = NULL;

	if(*tail != NULL)
		(*tail)->siguiente = newNode;

	*tail = newNode;
}

void printearFuncion(NodoFuncion* head) {
	if(head != NULL) {
		printf("\n\n");
		printf("\t-----------------\n");
		printf("\t  FUNCIONES\n");
		printf("\t-----------------\n\n");

		amarillo(0);
		printf("\t- Linea");
		printf("     ");
		rojo(1);
		printf("- Tipo dato");
		printf("     ");
		verde(0);
		printf("- Declaracion\n\n");

		while(head != NULL) {
			printf("\t");
			imprimirLinea(head->linea);
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
}