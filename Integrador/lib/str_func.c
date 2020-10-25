#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/funciones.h" 

int strMax;

/*

concatenarStrings -> concatena

copiarStrings -> sobreescribe la posicion de memoria dada

ojo con el ellipsis (...). Los argumentos hay que pasarlos asi:

formato1, str1, formato2, str2

un llamado a la funcion podria ser

concatenarStrings(&posicion, 30, 2, " %s", "string1", "\n\n %s 2 \n", "string2");

*/

char* trimStr(char* str) {
    while(str[0] == ' ' && str[0] != '\0')
      str++;

    char* final = str + strlen(str) - 1;

    while(final[0] == ' ' && final > str)
        final--;

    char* ret = malloc(final - str + 2);
    strncpy(ret, str, final - str + 1);
  
    return ret;
}

void concatenarStrings(char** address, int args, ...) {
  va_list valist;
  va_start(valist, args);
 
  for(int i = 0; i < args; i++) {
    char* format = va_arg(valist, char*);
    char* str = va_arg(valist, char*);
    int addrLength = strlen(*address);

    if(strlen(str) + 1 > strMax - addrLength) {
      int n = strlen(str) + 250;
      *address = realloc(*address, strlen(str) + 250);
      strMax += strlen(str) + 250;
    }

    sprintf(*address + addrLength, format, str);
  }

  va_end(valist);
}

void copiarStrings(char** address, int args, ...) {
  memset(*address, 0, strMax); // Limpiamos la posicion primero

  va_list valist;
  va_start(valist, args);

  for(int i = 0; i < args; i++) {
    char* format = va_arg(valist, char*);
    char* str = va_arg(valist, char*);

    concatenarStrings(address, strMax, 1, format, str);
  }
}