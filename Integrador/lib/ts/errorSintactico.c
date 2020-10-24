#include "../../headers/funciones.h"
#include <string.h>
#include <stdlib.h>

void crearErrorSintactico(char* str) {
	hayError = 1;
	char* causa = malloc(12 + strlen(str) + 1);
	strcpy(causa, "semantico - ");
	strcat(causa, str);

	agregarError(&tail_errores, line, causa);
}