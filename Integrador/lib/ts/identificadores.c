#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../headers/ts.h"
#include "../../headers/funciones.h"

// Verifica que el identificador no este asignado
_Bool identificadorLibre(ts_iden* head, char* identificador) {
	while(head != NULL && head != 0x8) {
		//if(strcmp(head->identificador, identificador) == 0)
		//	return false;

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
	nuevoNodo->siguiente = NULL;

	if(*tail != NULL)
		(*tail)->siguiente = nuevoNodo;

	*tail = nuevoNodo;
}

void sacarAsteriscos(char** str) {
	int i = 0;

	while((*str)[i] == '*' || (*str)[i] == ' ')
		i++;

	char* aux = *str; // Para no perder la referencia a ese espacio de memoria
	*str = malloc(strlen(aux) + 1 - i);
	strcpy(*str, aux + i);
}

char* pos; // Usada por separarDeclaraciones()

void iterarHastaSeparador(char** aux, char extra) {
	while((*aux)[0] != ',' && (*aux)[0] !=';' && (*aux)[0] != extra) {
		if((*aux)[0] == '(') {
			while((*aux)[0] != ')')
				(*aux)++;
		}
		(*aux)++;
	}
}

char* separarDeclaraciones(char* str) {
	if(str != NULL)
		pos = str;

	char* aux = pos;

	if(aux[0] == ';' || aux[0] == '\0')
		return NULL;

	iterarHastaSeparador(&aux, '=');

	char* ret = malloc(aux - pos + 1);
	strncpy(ret, pos, aux - pos);
	ret = trimStr(ret);
	
	iterarHastaSeparador(&aux, -1); // Si se paro en un '=' va a seguir hasta la proxima coma o punto y coma

	if(aux[0] == ',')
		aux++;

	pos = aux;
	return ret;
}

void ts_analizarDeclaracion(ts_iden** head_iden, ts_iden** tail_iden, char* especificadores, char* declaraciones) {
	short tipo = encontrarTipo(especificadores);
	short puntero = tipoPuntero(declaraciones);
	sacarAsteriscos(&declaraciones);

	char* decla = separarDeclaraciones(declaraciones);

	while(decla != NULL) {
		_Bool esFuncion = strstr(decla, "(");
		_Bool error = false;

		if(esFuncion) {

		} else {
			if(identificadorLibre(*head_iden, decla)) {
				agregarVariable(tail_iden, tipo, puntero, decla);
 				ts_iden* aux = *head_iden;

				if(*head_iden == NULL)
					*head_iden = *tail_iden;
			} else
				error = true;
		}

		if(error)
			crearErrorSintactico("Doble declaracion de identificador");

		decla = separarDeclaraciones(NULL);
	}
}