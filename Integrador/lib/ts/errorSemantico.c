#include "../../headers/funciones.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void crearErrorSemantico(char* str) {
	hayError = true;
	char* causa = malloc(12 + strlen(str) + 1);
	strcpy(causa, "semantico - ");
	strcat(causa, str);

	agregarError(&tail_errores, line, causa);

	if(head_errores == NULL)
		head_errores = tail_errores;
}