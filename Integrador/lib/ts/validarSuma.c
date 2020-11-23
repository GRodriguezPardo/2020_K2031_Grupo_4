#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../headers/ts.h"
#include "../../headers/funciones.h"

short tipoIdentificador(tablaSimbolos, char*, short*);
_Bool seAnaliza(char*);
_Bool analizarOperando(char*, short*, short*, tablaSimbolos);

void validarSuma(tablaSimbolos ts, char* op1, char* op2) {

    char* operando1 = (char*)malloc(strlen(op1) + 1);
    char* operando2 = (char*)malloc(strlen(op2) + 1);
    strcpy(operando1, op1);
    strcpy(operando2, op2);

    if(!seAnaliza(operando1) || !seAnaliza(operando2))
        return;

    short esPuntero1;
    short esPuntero2;

    short tipo1;
    short tipo2;

    if(!analizarOperando(operando1, &esPuntero1, &tipo1, ts) || !analizarOperando(operando2, &esPuntero2, &tipo2, ts))
        return;

    // Pueden ser negativos (se estaria desreferenciando mal)
    if(esPuntero1 && esPuntero2)
        crearErrorSemantico("No se pueden sumar dos punteros");
    else if (!esCompatible(tipo1, tipo2))
        crearErrorSemantico("Los operandos no son compatibles");
    else if(esPuntero1 < 0 || esPuntero2 < 0)
        crearErrorSemantico("Se desreferencia una variable que no es puntero");

    free(operando1);
    free(operando2);

}

_Bool seAnaliza(char* arg) {
	if(arg[0] == '(') // Expresion con parentesis
		return false;

	// Salteamos posible desreferenciacion de punteros
	while(arg[0] == '*' || arg[0] == ' ')
		arg++;

	char* ops[] = {"+", "-", "/", "*", "%", "=", "!", ">", "<"};

	for(int i = 0; i < 9; i++) {
		if(strstr(arg, ops[i]) != NULL)
			return false;
	}

	return true;
}

_Bool analizarOperando(char* arg, short* puntero, short* tipo, tablaSimbolos ts) {
    *tipo = esConstante(arg[0], puntero);

    if(*tipo == 0) {
        char* esFuncion = strstr(arg, "(");

        if(esFuncion != NULL) {
            esFuncion[0] = '\0';
            esFuncion++;
            free(esFuncion);

            ts_func* nodoFunc = buscarFuncion(arg, ts.head_func);

            if(nodoFunc == NULL) {
                crearErrorSemantico("Funcion no declarada");
                return false;
            }
            *puntero = nodoFunc->puntero;
            *tipo = nodoFunc->tipo;
        } else {
            short punteroExtra = 0;
            punteroExtra -= obtenerPunteroArray(arg);
            punteroExtra -= tipoPuntero(arg, true);

            // Sacamos corchetes y asteriscos

            char* corchete = strstr(arg, "[");

            if(corchete != NULL) {
                corchete[0] = '\0';
                corchete++;
                free(corchete);
            }

            while(arg[0] == '*' || arg[0] == ' ')
                arg++;

            ts_iden* nodoIden = buscarVariable(arg, ts.head_iden);

            if(nodoIden == NULL) {
                crearErrorSemantico("Variable no declarada");
                return false;
            }
            *puntero = nodoIden->puntero + punteroExtra;
            *tipo = nodoIden->tipo;        
        }
    }
    return true;
}