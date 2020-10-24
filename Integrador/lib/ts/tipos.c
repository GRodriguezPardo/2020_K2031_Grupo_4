#include <string.h>
#include "../../headers/ts.h"

//*transiciones[5]; -> Notar que los asteriscos estan siempre al principio

short tipoPuntero(char* str) {
	short tipo = 0;
	for(int i = 0; i < strlen(str) && (str[i] == ' ' || str[i] == ' '); i++)
		if(str[i] == '*')
			tipo++;

	return tipo;
}

short encontrarTipo(char* especificadores) {
	if(strstr("char", especificadores))
		return 0;
	else if(strstr("signed char", especificadores))
		return 1;
	else if(strstr("unsigned char", especificadores))
		return 2;
	else if(strstr("unsigned short", especificadores))
		return 4;
	else if(strstr("short", especificadores))
		return 3;
	else if(strstr("long long", especificadores))
		return 9;
	else if(strstr("unsigned long long", especificadores))
		return 10;
	else if(strstr("unsigned long", especificadores))
		return 8;
	else if(strstr("long double", especificadores))
		return 13;
	else if(strstr("long", especificadores))
		return 7;
	else if(strstr("unsigned int", especificadores))
		return 6;
	else if(strstr("int", especificadores))
		return 5;
	else if(strstr("float", especificadores))
		return 11;
	else if(strstr("double", especificadores))
		return 12;
}