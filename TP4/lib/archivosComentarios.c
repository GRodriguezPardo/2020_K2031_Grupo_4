#include "../headers/funciones.h"
#include <stdio.h>

void setupFiles(FILE** yyin) {
   char input[50];

   printf("Ingrese nombre del archivo a leer (recuerde ingresar formato):\n> ");
   scanf("%s", input);
   *yyin = fopen(input, "r");

   while(*yyin == NULL) {
      printf("\nError al abrir archivo. Vuelva a (recuerde ingresar formato):\n> ");
      scanf("%s", input);
      *yyin = fopen(input, "r");
   }
}

void contarLineas(char* yytext) {
   int i = 0;

   while(yytext[i] != '\0') {
      if(yytext[i] == '\n')
         line++;

      i++;
   }
}