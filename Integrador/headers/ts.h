#include <stdbool.h>

/*
LOS TIPOS DE DATOS SE IDENTIFICAN CON UN NUMERO

Los parentesis indican opcional

char -> 0
signed char ->1
unsigned char -> 2
(signed) short (int)? -> 3
unsigned short (int)? -> 4
(signed) int -> 5
unsigned (int) -> 6
(signed) long (int) -> 7
unsigned long (int) -> 8
(signed) long long (int) -> 9
unsigned long long (int) -> 10
float -> 11
double -> 12
long double -> 13

*/

/*
	DECLARACIONES
*/



typedef struct ts_iden {
	short tipo;
	short puntero;
	char* identificador;
	struct ts_iden* siguiente;
} ts_iden;

typedef struct ts_func {
	short tipo;
	short puntero;
	char* identificador;
	ts_iden* args;
	struct ts_func* siguiente;
} ts_func;

void analizarDeclaracion(ts_iden**, ts_iden**, char*, char*);

/*
	TIPOS
*/

short tipoPuntero(char*);
short encontrarTipo(char*);