%{
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int yylex();
int yyerror();

char buffer[500];
%}

%token <cadena> TYPE_SPECIFIER SIZEOF LEFT_OP RIGHT_OP GE_OP LE_OP OR_OP AND_OP NE_OP EQ_OP INC_OP DEC_OP
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


%type <cadena> declarator direct_declarator init_declarator init_declarator_list declaration_specifiers declaration identifier_list type_qualifier_list pointer
%type <cadena> assignment_expression conditional_expression logical_or_expression logical_and_expression inclusive_or_expression exclusive_or_expression
%type <cadena> and_expression equality_expression relational_expression shift_expression additive_expression multiplicative_expression cast_expression unary_expression

%%



/*
	EXPRESIONES
*/


/*
	FIN PARTE EXPRESIONES
*/


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

declarator:
		direct_declarator
	|	pointer direct_declarator {strcat($$, $2);}
;

// Aca faltaria el parameter_type_list
direct_declarator:
		IDENTIFIER
	|	'(' declarator ')'
	|	direct_declarator '[' conditional_expression ']' {sprintf($$ + strlen($1), "[%ld]", $3);}
	|	direct_declarator '[' ']' {strcat($$, "[]");}
	|	direct_declarator '(' ')' {strcat($$, "()");}
	|	direct_declarator '(' identifier_list ')' {sprintf($$ + strlen($$), "(%s)", $3);} 
;

pointer:
		'*' {strcpy($$, "*");}
	|	'*' type_qualifier_list {sprintf($$ + strlen($$), "*%s", $2);}
	|	'*' pointer {sprintf($$ + strlen($$), "*%s", $2);}
	|	'*' type_qualifier_list pointer {sprintf($$ + strlen($$), "*%s %s", $2, $3);}
;


type_qualifier_list:
		TYPE_QUALIFIER
	|	type_qualifier_list TYPE_QUALIFIER {sprintf($$ + strlen($$), " %s", $2);}
;

identifier_list:
		IDENTIFIER
	|	identifier_list ',' IDENTIFIER {sprintf($$ + strlen($$), ", %s", $3);}
;

%%

int yyerror (char *s) {
  printf ("%s\n", s);
}

void main() {
   yyparse();
}