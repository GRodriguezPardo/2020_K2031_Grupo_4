#include <string.h>
#include "../../headers/ts.h"

//*transiciones[5]; -> Notar que los asteriscos estan siempre al principio

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