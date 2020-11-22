#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../headers/ts.h"
#include "../../headers/funciones.h"

short tipoIdentificador(tablaSimbolos, char*, short*);

void validarSuma(tablaSimbolos ts, char* op1, char* op2) {

    char* operando1 = (char*)malloc(strlen(op1) + 1);
    char* operando2 = (char*)malloc(strlen(op2) + 1);

    strcpy(operando1, op1);
    strcpy(operando2, op2);

    short esPuntero1;
    short esPuntero2;

    short tipo1 = esConstante(operando1[0], &esPuntero1);
    short tipo2 = esConstante(operando2[0], &esPuntero2);

    if(tipo1 == 0) {
        tipo1 = tipoIdentificador(ts, operando1, &esPuntero1);
        if(tipo1 == -1) {
            crearErrorSemantico("Variable no declarada");
            return;
        }
    }
    if(tipo2 == 0) {
        tipo2 = tipoIdentificador(ts, operando1, &esPuntero1);
        if(tipo2 == -1) {
            crearErrorSemantico("Variable no declarada");
            return;
        }
    }

    if(esPuntero1 && esPuntero2) {
        crearErrorSemantico("No se pueden sumar punteros");
        return;
    } else if (!esCompatible(tipo1, tipo2)) {
        crearErrorSemantico("Los operandos no son compatibles");
        return;
    }

}

short tipoIdentificador(tablaSimbolos ts, char* operando, short* puntero){

    ts_iden* nodoIden = buscarVariable(operando, ts.head_iden);

    if(nodoIden == NULL) {
        return -1;
    }

    *puntero = nodoIden->puntero + tipoPuntero(operando, true);

    return nodoIden->tipo;

}


