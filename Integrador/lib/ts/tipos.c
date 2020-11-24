#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../../headers/ts.h"

//*transiciones[5]; -> Notar que los asteriscos estan siempre al principio

ts_func* buscarFuncion(char* iden, ts_func* head_func) {
	while(head_func != NULL && strcmp(iden, head_func->identificador) != 0)
		head_func = head_func->siguiente;
	return head_func;
}

ts_iden* buscarVariable(char* iden, ts_iden* head_iden) {

	while(head_iden != NULL && strcmp(iden, head_iden->identificador) != 0)
		head_iden = head_iden->siguiente;
	return head_iden;
}

short obtenerPunteroArray(char* iden) {
	short puntero = 0;
	for(int i = 0; i < strlen(iden); i++)
		if(iden[i] == '[')
			puntero++;

	return puntero;
}

short tipoPuntero(char* str, _Bool desdeInicio) {
	short tipo = 0;

	if(desdeInicio) {
		for(int i = 0; i < strlen(str) && (str[i] == ' ' || str[i] == '*'); i++)
			if(str[i] == '*')
				tipo++;
	} else {
		for(int i = strlen(str); i > 0 && (str[i] == ' ' || str[i] == '*'); i--)
			if(str[i] == '*')
				tipo++;
	}

	return tipo;
}

// Si los tipos son compatibles pero diferentes seria ideal tirar un warning
_Bool esCompatible(short tipo1, short tipo2) {
	if(tipo1 == tipo2)
		return true;
	else if(tipo1 >= 1 && tipo1 <= 3 && tipo2 >= 1 && tipo2 <= 3)
		return true;
	else if(tipo1 >= 4 && tipo1 <= 13 && tipo2 >= 4 && tipo2 <= 13)
		return true;
	else
		return false;
}

short esConstante(char c, short* punt) {
	*punt = 0;

	if(c == '\'')
		return 1;
	else if(c == '\"') {
		*punt = 1;
		return 1;
	} else if(c >= 48 && c <= 57)
		return 6;
	else
		return 0;
}

short encontrarTipo(char* especificadores) {
	if(strstr(especificadores, "char"))
		return 1;
	else if(strstr(especificadores, "signed char"))
		return 2;
	else if(strstr(especificadores, "unsigned char"))
		return 3;
	else if(strstr(especificadores, "unsigned short"))
		return 5;
	else if(strstr(especificadores, "short"))
		return 4;
	else if(strstr(especificadores, "long long"))
		return 10;
	else if(strstr(especificadores, "unsigned long long"))
		return 11;
	else if(strstr(especificadores, "unsigned long"))
		return 9;
	else if(strstr(especificadores, "long double"))
		return 14;
	else if(strstr(especificadores, "long"))
		return 8;
	else if(strstr(especificadores, "unsigned"))
		return 7;
	else if(strstr(especificadores, "int"))
		return 6;
	else if(strstr(especificadores, "float"))
		return 12;
	else if(strstr(especificadores, "double"))
		return 13;
	else if(strstr(especificadores, "void"))
		return 0;
}

void sacarAsteriscos(char** str) {
	int i = 0;

	while((*str)[i] == '*' || (*str)[i] == ' ')
		i++;

	char* aux = *str; // Para no perder la referencia a ese espacio de memoria
	*str = malloc(strlen(aux) + 1 - i);
	strcpy(*str, aux + i);
}