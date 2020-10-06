struct casa {
   int a;
};

void setupFiles(int** yyin) {
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