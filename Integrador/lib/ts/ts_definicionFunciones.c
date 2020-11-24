#include <stdlib.h>
#include <string.h>
#include "../../headers/ts.h"
#include "../../headers/funciones.h"

void coincideConDeclaracion(ts_func* head_func, short tipo, short puntero, char* identificador, char* args, _Bool esDefinicion) {
    ts_func* nodoFunc = buscarFuncion(identificador, head_func);

    char* aux = args;
    args = strtok(aux, ")");

    if(nodoFunc == NULL) {
        crearErrorSemantico("Variable y funcion con el mismo nombre");
        return;
    } else if(nodoFunc->tipo != tipo) {
        crearErrorSemantico("Tipo de dato no coindide en la declaracion y la definicion");
        return;
    } else if(nodoFunc->puntero != puntero) {
        crearErrorSemantico("Puntero no coincide en la declaracion y la definicion");
        return;
    } else if(esDefinicion && nodoFunc->estaDefinida) {
        crearErrorSemantico("Doble definicion de funcion");
        return;
    } else if(!esDefinicion && !(nodoFunc->estaDefinida)) {
        crearErrorSemantico("Doble declaracion de funcion");
        return;
    }

    ts_iden* head_args = nodoFunc->head_args;
    char* arg = separarDeclaraciones(&args);

    while(arg != NULL && head_args != NULL) {
        short tipoArg = encontrarTipo(arg);
        short punteroArg = 0;
        punteroArg += obtenerPunteroArray(arg);
        
        for(int i = 0; i < strlen(arg); i++)
            if(arg[i] == '*')
                punteroArg++;

        if(tipoArg != head_args->tipo) {
            crearErrorSemantico("Tipo de dato en el argumento no coincide en el prototipo y la definicion");
            return;
        } else if(punteroArg != head_args->puntero) {
            crearErrorSemantico("Punteros no coinciden en el prototipo y la definicion");
            return;
        }
        arg = separarDeclaraciones(&args);
        head_args = head_args->siguiente;
    }

    if(arg != NULL || head_args != NULL)
        crearErrorSemantico("Cantidad de argumentos no coinciden");
}