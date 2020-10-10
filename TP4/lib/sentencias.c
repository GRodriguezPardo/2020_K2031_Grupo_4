#include <stdlib.h>
#include <string.h>
#include "../headers/funciones.h"

void agregarSentencia(NodoSentencia** head, NodoSentencia** tail,char* tipo, int linea) {
	NodoSentencia* nuevoNodo = (NodoSentencia*) malloc(sizeof(NodoSentencia));

	nuevoNodo->linea = linea;
	nuevoNodo->tipo = malloc(strlen(tipo));
	strcpy(nuevoNodo->tipo, tipo);
	nuevoNodo->siguiente = NULL;

	if(*tail != NULL)
		(*tail)->siguiente = nuevoNodo;
		
	*tail = nuevoNodo;

	if(*head == NULL)
		*head = *tail; // El head va a ser NULL solo si todavia no hay sentencias
}

void printearSentencia(NodoSentencia* head) {
	if(head != NULL) {
		printf("\n\n");
		printf("\t--------------\n");
		printf("\t  SENTENCIAS\n");
		printf("\t--------------\n\n");

		amarillo(0);
		printf("\t- Linea");
		printf("     ");
		verde(0);
		printf("- Tipo de sentencia\n\n");

		while(head != NULL) {
			printf("\t");
			imprimirLinea(head->linea);
			printf(" | ");
			verde(1);
			printf("%s\n", head->tipo);
			blanco();
			head = head->siguiente;
		}
	}
}