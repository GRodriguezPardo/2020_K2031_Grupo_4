#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../headers/ts.h"
#include "../../headers/funciones.h"

_Bool identificadorLibre(tablaSimbolos, char*);
void agregarVariable(ts_iden**, short, short, char*);
void sacarAsteriscos(char**);
void iterarHastaSeparador(char**, char);
char* separarDeclaraciones(char**);
_Bool obtenerInformacionArray(char* decla, short* dimArray, int** arrSize);

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
_Bool identificadorLibre(tablaSimbolos ts, char* identificador) {
	_Bool libre = true;

	ts_iden* head_iden = ts.head_iden;
	while(head_iden != NULL && libre) {
		if(strcmp(head_iden->identificador, identificador) == 0)
			libre = false;
		head_iden = head_iden->siguiente;
	}

	ts_func* head_func = ts.head_func;
	while(head_func != NULL && libre) {
		if(strcmp(head_func->identificador, identificador) == 0)
			libre = false;
		head_func = head_func->siguiente;
	}

	return libre;
}

void agregarVariable(ts_iden** tail, short tipo, short puntero, char* identificador) {
	ts_iden* nuevoNodo = (ts_iden*) malloc(sizeof(ts_iden));

	// La posibilidad de error sintactico en la definicion de array esta dentro de la funcion obtenerInformacionArray
	if(obtenerInformacionArray(identificador, &(nuevoNodo->dimArray), &(nuevoNodo->arrSize))) {
		// Este while saca los [][] en caso de que haya (si es un array) para dejar solo el identificador
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
char* separarDeclaraciones(char** str) {
	if(*str != NULL) { // Por las dudas
		char* aux = *str;

		if(aux[0] == ';' || aux[0] == '\0')
			return NULL;

		iterarHastaSeparador(&aux, '=');

		char* ret = malloc(aux - *str + 1);
		strncpy(ret, *str, aux - *str);
		ret = trimStr(ret);
		
		iterarHastaSeparador(&aux, -1); // Si se paro en un '=' va a seguir hasta la proxima coma o punto y coma

		if(aux[0] == ',')
			aux++;

		*str = aux;
		return ret;
	} else
		return NULL;
}

void agregarFuncion(ts_func** tail, short tipo, short puntero, char* declaraciones, char* identificador) {
	ts_func* nuevoNodo = (ts_func*) malloc(sizeof(ts_func));
	nuevoNodo->tipo = tipo;
	nuevoNodo->puntero = puntero;
	nuevoNodo->identificador = (char*) malloc(strlen(identificador) + 1);
	strcpy(nuevoNodo->identificador, identificador);
	// Adaptamos las declaraciones para poder usar la funcion separarDeclaraciones
	int i = 0;
	while(declaraciones[i] != ')')
		i++;
	declaraciones[i] = ';';

	//printf("\n\n--- %s", declaraciones);

	// Analizamos cada argumento de la funcion
	char* decla = separarDeclaraciones(&declaraciones);
	/*
	Dentro de cada nodo de funcion hay una lista de argumentos
	*/

	while(decla != NULL) {
		ts_iden* nuevaVar = (ts_iden*) malloc(sizeof(ts_iden));
		obtenerInformacionArray(decla, &(nuevaVar->dimArray), &(nuevaVar->arrSize));
		nuevaVar->puntero = tipoPuntero(decla, false);
		nuevaVar->tipo = encontrarTipo(decla);
		nuevaVar->siguiente = NULL;

		if(nuevoNodo->tail_args != NULL)
			(nuevoNodo->tail_args)->siguiente = nuevaVar;
		else
			nuevoNodo->head_args = nuevaVar;

		nuevoNodo->tail_args = nuevaVar;

		decla = separarDeclaraciones(&declaraciones);
	}

	if(*tail != NULL)
		(*tail)->siguiente = nuevoNodo;

	*tail = nuevoNodo;
}

void ts_analizarDeclaracion(tablaSimbolos* ts, char* especificadores, char* declaraciones) {
	short tipo = encontrarTipo(especificadores);
	short puntero = tipoPuntero(declaraciones, true);
	sacarAsteriscos(&declaraciones);

	char* decla = separarDeclaraciones(&declaraciones);

	while(decla != NULL) {
		_Bool esFuncion = strstr(decla, "(");
		_Bool error = false;

		if(esFuncion) {
			// strtok modifica el string asi que hay que copiarla
			char* aux = malloc(strlen(decla) + 1);
			strcpy(aux, decla);
			char* identificador = strtok(aux, "(");
			char* args = identificador + strlen(identificador) + 1;
			if(identificadorLibre(*ts, identificador)) {
				agregarFuncion(&(ts->tail_func), tipo, puntero, args, identificador);

				if(ts->head_func == NULL)
					ts->head_func = ts->tail_func;
			} else
				error = true;
		} else {
			if(identificadorLibre(*ts, decla)) {
				agregarVariable(&(ts->tail_iden), tipo, puntero, decla);

				if(ts->head_iden == NULL)
					ts->head_iden = ts->tail_iden;
			} else
				error = true;
		}

		if(error)
			crearErrorSemantico("Doble declaracion de identificador");

		decla = separarDeclaraciones(&declaraciones);
	}
}