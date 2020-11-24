#include <stdlib.h>
#include <string.h>
#include "../../headers/funciones.h"
#include "../../headers/ts.h"

_Bool verCaracter(char c, char** ptr, short* parentesis) {
	_Bool listo = false;
	switch(c) {
		case ')':
			(*parentesis)++;
			break;

		case '(':
			(*parentesis)--;
			break;

		case '\"':
			while((*ptr)[0] != '\"')
				(*ptr)++;
			break;

		case ',':
			if(*parentesis == 0)
				listo = true;

			break;
	}

	return listo;
}

// Es similar a strtok y separarDeclaraciones, pero era mas facil crear otra funcion la verdad

char* separarArgumento(char** args) {
	char* ptr = *args;
	short parentesis = 0;

	if(strlen(ptr) == 0)
		return NULL;

	while(ptr[0] != '\0') {
		if(verCaracter(ptr[0], &ptr, &parentesis))
			break;

		ptr++;
	}
	char* ret = malloc(ptr - *args + 1);
	strncpy(ret, *args, ptr - *args);
	
	if(ptr[0] == '\0')
		*args = ptr;
	else
		*args = ptr + 1;

	ret = trimStr(ret);
	return ret;
}

void analizarArgumento(char* arg, ts_iden** args, ts_iden* head_iden, ts_func* head_func) {
	// Primero vemos si se desreferencia un puntero o se pasa por referencia
	short punteroExtra = 0;

	printf("\n%s\n", arg);

	while(arg[0] == '*' || arg[0] == '&' || arg[0] == ' ') {
		if(arg[0] == '*')
			punteroExtra--;
		else if(arg[0] == '&')
			punteroExtra++;

		arg++;
	}

	printf("\n%s\n", arg);

	punteroExtra -= obtenerPunteroArray(arg); // Por si se accede al array. Ej: hacer a[5] seria acceder al array en 5

	/*
		Hacer, por ejemplo, a[2] es lo mismo que *(a + 2)
	*/

	int i = 0;
	while(arg[i] != '\0') {
		// Cambiamos el fin de string
		if(arg[i] == '[') 
			arg[i] = '\0';

		i++;
	}

	short constPunt;
	short isConst = esConstante(arg[0], &constPunt);
	_Bool tipoCorrecto = true;
	_Bool punteroCorrecto = true;

	printf("\n%d\n", isConst);

	if(isConst) {
		if(!esCompatible(isConst, (*args)->tipo))
			tipoCorrecto = false;
		else if(constPunt != (*args)->puntero)
			punteroCorrecto = false;
	} else if(strstr(arg, "(") != NULL) { // es otra llamada de funcion
		int i = 0;
		while(i < strlen(arg) + 1 && arg[i] != '(')
			i++;
		arg[i] = '\0';
		ts_func* funcion = buscarFuncion(arg, head_func);

		if(funcion == NULL)
			crearErrorSemantico("Funcion no declarada");
		else {
			if(!esCompatible(funcion->tipo, (*args)->tipo))
				tipoCorrecto = false;
		}

	} else {
		ts_iden* var = buscarVariable(arg, head_iden);

		if(var == NULL) { // Variable no declarada
			char* msg = malloc(24 + strlen(arg) + 1);
			strcpy(msg, "Variable no declarada");
			strcat(msg, " - ");
			strcat(msg, arg);
			crearErrorSemantico(msg);
			return;
		}

		if(!esCompatible(var->tipo, (*args)->tipo))
			tipoCorrecto = false;
		
		if((*args)->puntero != var->puntero + punteroExtra)
			punteroCorrecto = false;
	}

	if(!tipoCorrecto)
		crearErrorSemantico("Tipo de argumento incorrecto");

	if(!punteroCorrecto)
		crearErrorSemantico("Punteros no coinciden");

	*args = (*args)->siguiente;
}

void ts_analizarLlamada(char* iden, char* yylval_args, ts_iden* head_iden, ts_func* head_func) {
	ts_func* nodoFuncion = buscarFuncion(iden, head_func);

	if(nodoFuncion == NULL) {
		crearErrorSemantico("Funcion no declarada");
		return;
	}

	// Copiamos para no reemplazar el yylval
	char* args = malloc(strlen(yylval_args) + 1);
	strcpy(args, yylval_args);
	ts_iden* argsFuncion = nodoFuncion->head_args;

	char* arg = separarArgumento(&args);
	while(arg != NULL) {
		printf("\n%s - %s\n", arg, args);
		analizarArgumento(arg, &argsFuncion, head_iden, head_func);
		arg = separarArgumento(&args);
	}
}