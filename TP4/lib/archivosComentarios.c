#include "../headers/funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* format;

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

void printearMensajeFinal() {
   printf("\n\n\n\n\n");

   if(hayError == 1) {
      rojo(1);
      printf("ANALISIS FINALIZADO CON ERRORES");
   } else {
      verde(1);
      printf("[======================================] 100%c", 37);
      printf("\nANALISIS FINALIZADO CORRECTAMENTE");
   }
   blanco();
}

void calcularCaracteres() {
   char temp[6]; // El rango de int es [0-65535] + '\0'
   sprintf(temp, "%d", line);
   format = malloc(3 + strlen(temp)); // Nunca va a ser mas de 8 caracteres. Es % + [0-65535] + s
   sprintf(format, "%c%dd", 37, (int) strlen(temp));
}

void imprimirLinea(int linea) {
   amarillo(0);
   printf(format, linea);
   blanco();
}