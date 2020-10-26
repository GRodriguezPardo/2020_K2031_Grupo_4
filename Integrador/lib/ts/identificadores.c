#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../headers/ts.h"
#include "../../headers/funciones.h"

char* pos; // Usada por separarDeclaraciones()

_Bool identificadorLibre(ts_iden*, char*);
void agregarVariable(ts_iden**, short, short, char*);
void sacarAsteriscos(char**);
void iterarHastaSeparador(char**, char);
char* separarDeclaraciones(char*);
_Bool obtenerInformacionArray(char*, short*, int**);

_Bool obtenerInformacionArray(char* decla, short* dimArray, int** arrSize) {
	*dimArray = 0;
	*arrSize = NULL;

	while(decla[0] != '\0') {

		if(decla[0] == '[') {
			int dimension = 0;
			(*dimArray)++;
			char aux[strlen(decla) + 1];
			strcpy(aux, decla); // strtok modifica la cadena, asi que la copiamos
			char* dim = strtok(aux, "]");
			/*
				Si strtok devuelve solo "[", es decir, no esta definido el largo del array
				Ej: a[][] -> Error semantico porque el segundo array si o si tiene que tener definido el largo
			*/
			if(strlen(dim) == 1 && *dimArray > 1) { 
				crearErrorSemantico("Largo del array no definido");
				return false; // Cortamos aca
			} else {
				dim++; // Avanzamos 1 posicion para descartar el '['
				dimension = atoi(dim);
			}

			if(*arrSize == NULL)
				*arrSize = malloc(sizeof(int));
			else
				*arrSize = realloc(*arrSize, sizeof(int) * (*dimArray));

			(*arrSize)[*dimArray - 1] = dimension;
		}

		decla++;
	}
	return true;
}

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

	// La posibilidad de error sintactico en la definicion de array esta dentro de la funcion obtenerInformacionArray
	if(obtenerInformacionArray(identificador, &(nuevoNodo->dimArray), &(nuevoNodo->arrSize))) {
		int i = 0;
		while(i < strlen(identificador)) {
			if(identificador[i] == '[') {
				identificador[i] = '\0';
				// El strcpy va a copiar solo hasta i. No preocuparse por la memoria "perdida" porque es stack memory (se limpia sola)
				break;
			}
			i++;
		}

		nuevoNodo->identificador = (char*) malloc(strlen(identificador) + 1);
		strcpy(nuevoNodo->identificador, identificador);
		nuevoNodo->tipo = tipo;
		nuevoNodo->puntero = puntero;
		nuevoNodo->siguiente = NULL;

		if(*tail != NULL)
			(*tail)->siguiente = nuevoNodo;

		*tail = nuevoNodo;
	}
}

void sacarAsteriscos(char** str) {
	int i = 0;

	while((*str)[i] == '*' || (*str)[i] == ' ')
		i++;

	char* aux = *str; // Para no perder la referencia a ese espacio de memoria
	*str = malloc(strlen(aux) + 1 - i);
	strcpy(*str, aux + i);
}

void iterarHastaSeparador(char** aux, char extra) {
	while((*aux)[0] != ',' && (*aux)[0] !=';' && (*aux)[0] != extra) {
		if((*aux)[0] == '(') {
			while((*aux)[0] != ')')
				(*aux)++;
		}
		(*aux)++;
	}
}

/*
	Esta funcion es basicamente un strtok pero que separa por ',', declaraciones de funciones y asginaciones
	Por ej: si le pasas a, func(char), b[], c = 5;
	En cada llamado devuelve (en orden)
	a
	func(char)
	b[]
	c
	NULL -> Fin
*/
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

				if(*head_iden == NULL)
					*head_iden = *tail_iden;
			} else
				error = true;
		}

		if(error)
			crearErrorSemantico("Doble declaracion de identificador");

		decla = separarDeclaraciones(NULL);
	}
}