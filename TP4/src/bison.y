%{
#include <math.h>
#include <stdio.h>
#include <ctype.h>
int yylex();
int yyerror();
%}

%token <cadena> DATA_TYPE
%token <cadena> UNARY_OPERATOR
%token <cadena> IDENTIFIER
%token <cadena> DATA_QUALIFIER

%union {
  long ival;
  double fval;
  char cadena[500];
}


%%

/*
	DECLARACIONES
*/

declaration:
   	function_declaration
;

function_declaration:
   	data_qualifier DATA_TYPE IDENTIFIER '(' argument_list ')' ';' {printf("encontrado");}
;


argument_list:
		argument
	|	argument_list ',' argument
;

argument:
		DATA_TYPE IDENTIFIER
	|	nothing
;

/*
	TIPOS DE DATO
*/

data_qualifier:
		DATA_QUALIFIER
	|	nothing
;

nothing:
;

%%

int yyerror (char *s) {
  printf ("%s\n", s);
}

void main() {
   yyparse();
}