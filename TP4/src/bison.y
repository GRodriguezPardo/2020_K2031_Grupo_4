%{
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int yylex();
int yyerror();

char buffer[500];
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


%type <cadena> declarator direct_declarator init_declarator init_declarator_list declaration_specifiers declaration
%type <ival> constant_expression

%%

declaration:
		declaration_specifiers init_declarator_list ';' {printf("%s %s;", $1, $2);}
;

declaration_specifiers:
		STORAGE_CLASS declaration_specifiers {sprintf($$ + strlen($1), " %s", $2);}
	|	TYPE_SPECIFIER declaration_specifiers {sprintf($$ + strlen($1), " %s", $2);}
	|	TYPE_QUALIFIER declaration_specifiers {sprintf($$ + strlen($1), " %s", $2);}
	|	TYPE_SPECIFIER
	|	TYPE_QUALIFIER
	|	STORAGE_CLASS
;

init_declarator_list:
		init_declarator
	|	init_declarator_list ',' init_declarator {sprintf($$ + strlen($1), ", %s", $3);}
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
	|	direct_declarator '[' constant_expression ']' {sprintf($$ + strlen($1), "[%ld]", $3);}
	|	direct_declarator '[' ']' {strcat($$, "[]");}
	|	direct_declarator '(' ')' {strcat($$, "()");}
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