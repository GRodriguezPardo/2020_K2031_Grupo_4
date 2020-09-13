%{
#include <math.h>
#include <stdio.h>
#include <ctype.h>
int yylex();
int yyerror();
%}

%token <cadena> TYPE_SPECIFIER
%token <cadena> UNARY_OPERATOR
%token <cadena> IDENTIFIER
%token <cadena> TYPE_QUALIFIER
%token <cadena> STORAGE_CLASS

%token <ival> DECIMAL_CONSTANT
%token <ival> HEX_CONSTANT
%token <ival> OCTAL_CONSTANT
%token <fval> FLOAT_CONSTANT

%token <cadena> STRING

%union {
  long ival;
  double fval;
  char cadena[500];
}


%%

declaration:
		declaration_specifiers init_declarator_list ';'
;

declaration_specifiers:
		STORAGE_CLASS declaration_specifiers
	|	TYPE_SPECIFIER declaration_specifiers
	|	TYPE_QUALIFIER declaration_specifiers
	|	TYPE_SPECIFIER
	|	TYPE_QUALIFIER
	|	STORAGE_CLASS
;

init_declarator_list:
		init_declarator
	|	init_declarator_list ',' init_declarator
;

/*ACA FALTA UNA PARTE DE declarator=initializer*/
init_declarator:
		declarator
;

/*FALTA LA PARTE DE pointer*/
declarator:
		direct_declarator
;

direct_declarator:
		IDENTIFIER
	|	'(' declarator ')'
	|	direct_declarator '[' constant_expression ']'
	|	direct_declarator '[' ']'
	|	direct_declarator '(' ')'
;

/*Implementado para probar pero no es la BNF correcta*/

constant_expression:
 		DECIMAL_CONSTANT
 	|	HEX_CONSTANT
	|	OCTAL_CONSTANT
;

%%

int yyerror (char *s) {
  printf ("%s\n", s);
}

void main() {
   yyparse();
}