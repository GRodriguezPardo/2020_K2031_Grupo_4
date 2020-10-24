#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../headers/ts.h"
#include "../../headers/funciones.h"

// Verifica que el identificador no este asignado
_Bool identificadorLibre(ts_iden* head, char* identificador) {
	while(head != NULL) {
		if(strcmp(head->identificador, identificador) == 0)
			return false;

		head = head->siguiente;
	}

	return true;
}

void agregarVariable(ts_iden** tail, short tipo, short puntero, char* identificador) {
	ts_iden* nuevoNodo = (ts_iden*) malloc(sizeof(ts_iden));
	nuevoNodo->identificador = (char*) malloc(strlen(identificador) + 1);
	strcpy(nuevoNodo->identificador, identificador);
	nuevoNodo->tipo = tipo;
	nuevoNodo->puntero = puntero;

	if(*tail != NULL)
		(*tail)->siguiente = nuevoNodo;
	else
		*tail = nuevoNodo;

	nuevoNodo->siguiente = NULL;
}

void sacarAsteriscos(char** str) {
	int i = 0;

	while((*str)[i] == '*' || (*str)[i] == ' ')
		i++;

	char* aux = *str; // Para no perder la referencia a ese espacio de memoria
	*str = malloc(strlen(aux) + 1 - i);
	strcpy(*str, aux + i);
}

/*
	ESTA FUNCION NO RECONOCE BIEN LOS ARRAYS
*/

void analizarDeclaracion(ts_iden** head_iden, ts_iden** tail_iden, char* especificadores, char* declaraciones) {
	short tipo = encontrarTipo(especificadores);
	short puntero = tipoPuntero(declaraciones);
	sacarAsteriscos(&declaraciones);

	char* decla = strtok(declaraciones, ",");

	while(decla != NULL) {
		if(identificadorLibre(*head_iden, decla)) {

			_Bool esFuncion = strstr(decla, "(") != NULL;

			if(esFuncion) {

			} else {
				agregarVariable(tail_iden, tipo, puntero, decla);
				if(*head_iden == NULL)
					*head_iden = *tail_iden;
			}
		} else
			crearErrorSintactico("Doble declaracion de identificador");

		decla = strtok(NULL, ",");
	}
}