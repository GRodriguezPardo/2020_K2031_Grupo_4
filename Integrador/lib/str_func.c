#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../headers/funciones.h" 

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