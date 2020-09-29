%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int yylex();
int yyerror();

#ifdef BDEBUG
	int yydebug = 1;
#endif

%}

%token <cadena> TYPE_SPECIFIER SIZEOF SHIFT_RIGHT SHIFT_LEFT GE_OP LE_OP OR_OP AND_OP NE_OP EQ_OP INC_OP DEC_OP
%token <cadena> TYPE_QUALIFIER STORAGE_CLASS IDENTIFIER UNARY_OPERATOR STRING
%token <cadena> SUM_ASSIGN SUB_ASSIGN DIV_ASSIGN MUL_ASSIGN MOD_ASSIGN PTR_ARROW

%token <ival> DECIMAL_CONSTANT
%token <ival> HEX_CONSTANT
%token <ival> OCTAL_CONSTANT
%token <fval> FLOAT_CONSTANT

%type <cadena> expresion expresion_asignacion expresion_condicional operador_asignacion expresion_logical_or expresion_logical_and
%type <cadena> expresion_o_inclusivo expresion_o_excluyente expresion_y expresion_igualdad expresion_relacional expresion_corrimiento
%type <cadena> expresion_aditiva expresion_multiplicativa expresion_conversion expresion_unaria unary_op expresion_sufijo lista_argumentos
%type <cadena> expresion_primaria constante

%union {
  long ival;
  double fval;
  char cadena[500];
}


%%
/*

	EXPRESION

*/

expresion:
		expresion_asignacion {printf("%s", $1);}
	|	expresion ',' expresion_asignacion
;

expresion_asignacion:
		expresion_condicional
	|	expresion_unaria operador_asignacion expresion_asignacion {sprintf($$ + strlen($$), " %s %s", $2, $3);}
;

// Aca falta el ternario
expresion_condicional:
		expresion_logical_or
;

operador_asignacion:	
		'='	{strcpy($$, "=");}
	|	SUM_ASSIGN {strcpy($$, "+=");}
	|	SUB_ASSIGN {strcpy($$, "-=");}
	|	MUL_ASSIGN {strcpy($$, "*=");}
	|	DIV_ASSIGN {strcpy($$, "/=");}
	|	MOD_ASSIGN {strcpy($$, "%=");}
;

expresion_logical_or:
		expresion_logical_and
	|	expresion_logical_or OR_OP expresion_logical_and {sprintf($$ + strlen($$), " %s %s", "||", $3);}
;

expresion_logical_and:
		expresion_o_inclusivo
	|	expresion_logical_and AND_OP expresion_o_inclusivo {sprintf($$ + strlen($$), " %s %s", "&&", $3);}
;

expresion_o_inclusivo:
		expresion_o_excluyente
	|	expresion_o_excluyente '|' expresion_y {sprintf($$ + strlen($$), " | %s", $3);}
;

expresion_o_excluyente:
		expresion_y
	|	expresion_o_excluyente '^' expresion_y {sprintf($$ + strlen($$), " ^ %s", $3);}
;

expresion_y:
		expresion_igualdad
	|	expresion_y '&' expresion_igualdad {sprintf($$ + strlen($$), " & %s", $3);}
;

expresion_igualdad:
		expresion_relacional
	|	expresion_igualdad EQ_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "==", $3);}
	|	expresion_igualdad NE_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "!=", $3);}
;

expresion_relacional:
		expresion_corrimiento
		|	expresion_relacional '<' expresion_corrimiento {sprintf($$ + strlen($$), " < %s", $3);}
		|	expresion_relacional '>' expresion_corrimiento {sprintf($$ + strlen($$), " > %s", $3);}
	|	expresion_relacional LE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", "<=", $3);}
	|	expresion_relacional GE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", ">=", $3);}
;

expresion_corrimiento:
		expresion_aditiva
	|	expresion_corrimiento SHIFT_LEFT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", "<<", $3);}
	|	expresion_corrimiento SHIFT_RIGHT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", ">>", $3);}
;

expresion_aditiva:
		expresion_multiplicativa
	|	expresion_aditiva '+' expresion_multiplicativa {sprintf($$ + strlen($$), " + %s", $3);}
	|	expresion_aditiva '-' expresion_multiplicativa {sprintf($$ + strlen($$), " - %s", $3);}
;

expresion_multiplicativa:
		expresion_conversion
	|	expresion_multiplicativa '*' expresion_conversion {sprintf($$ + strlen($$), " * %s", $3);}
	|	expresion_multiplicativa '/' expresion_conversion {sprintf($$ + strlen($$), " / %s", $3);}
	|	expresion_multiplicativa '%' expresion_conversion {sprintf($$ + strlen($$), " %c %s", 37, $3);}
;

expresion_conversion:
		expresion_unaria
	|	'(' TYPE_SPECIFIER ')' expresion_conversion {sprintf($$ + strlen($$), "(%s) %s", $2, $4);}
;

expresion_unaria:
		expresion_sufijo
	|	INC_OP expresion_unaria {sprintf($$ + strlen($$), "++%s", $2);}
	|	DEC_OP expresion_unaria {sprintf($$ + strlen($$), "--%s", $2);}
	|	unary_op expresion_conversion {sprintf($$ + strlen($$), "%s %s", $1, $2);}
	|	SIZEOF expresion_unaria {sprintf($$ + strlen($$), "sizeof %s", $2);}
	|	SIZEOF '(' TYPE_SPECIFIER ')'	{sprintf($$ + strlen($$), "sizeof(%s)", $3);}
;

unary_op:
		'&' {strcat($$, "$");}
	|	'*' {strcpy($$, "*");}
	|	'+' {strcat($$, "+");}
	|	'-' {strcat($$, "-");}
	|	'~' {strcat($$, "~");}
	|	'!' {strcat($$, "!");}
;

/*
	en la regla 1 se va a printear dos veces. Esto es esperado de bison ya que es una expresion
	adentro de otra expresion
*/
expresion_sufijo:
		expresion_primaria
	|	expresion_sufijo '[' expresion ']' {sprintf($$ + strlen($$), "[%s]", $3);}
	|	expresion_sufijo '(' lista_argumentos ')' {sprintf($$ + strlen($$), "(%s)", $3);}
	|	expresion_sufijo '.' IDENTIFIER {sprintf($$ + strlen($$), ".%s", $3);}
	|	expresion_sufijo PTR_ARROW IDENTIFIER {sprintf($$ + strlen($$), "->%s", $3);}
	|	expresion_sufijo INC_OP {strcat($$, "++");}
	|	expresion_sufijo DEC_OP {strcat($$, "--");}
;

lista_argumentos:
		expresion_asignacion
	|	lista_argumentos ',' expresion_asignacion {sprintf($$ + strlen($$), "%s, %s", $1, $3);}
;

expresion_primaria:
		IDENTIFIER
	|	constante
	|	STRING
	|	'(' expresion ')' {sprintf($$ + strlen($$), "(%s)", $2);}
;

// Para debugging, no es correcto
constante:
	DECIMAL_CONSTANT {sprintf($$, "%ld", $1);}
;

/*

FIN EXPRESION

*/

%%

int yyerror (char *s) {
  printf ("%s\n", s);
}

void main() {
   yyparse();
}