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

%token <cadena> TIPO_DATO SIZEOF SHIFT_RIGHT SHIFT_LEFT GE_OP LE_OP OR_OP AND_OP NE_OP EQ_OP INC_OP DEC_OP
%token <cadena> CALIFICADOR_TIPO CLASE_ALMACENAMIENTO IDENTIFIER UNARY_OPERATOR STRING ENUM
%token <cadena> SUM_ASSIGN SUB_ASSIGN DIV_ASSIGN MUL_ASSIGN MOD_ASSIGN PTR_ARROW STRUCT_UNION ELLIPSIS
%token <caracter> CONSTANTE_CARACTER
%token IF ELSE SWITCH WHILE DO FOR CASE DEFAULT CONTINUE BREAK RETURN GOTO

%token <ival> DECIMAL_CONSTANT
%token <ival> HEX_CONSTANT
%token <ival> OCTAL_CONSTANT
%token <fval> CONSTANTE_REAL

%type <cadena> expresion expresion_asignacion expresion_condicional operador_asignacion expresion_logical_or expresion_logical_and
%type <cadena> expresion_o_inclusivo expresion_o_excluyente expresion_y expresion_igualdad expresion_relacional expresion_corrimiento
%type <cadena> expresion_aditiva expresion_multiplicativa expresion_conversion expresion_unaria unary_op expresion_sufijo lista_argumentos
%type <cadena> expresion_primaria constante

%union {
  long ival;
  double fval;
  char cadena[500];
  char caracter;
}

%start unidad_traduccion

%%
/*

	EXPRESION

*/

expresion
	:	expresion_asignacion {printf("%s", $1);}
	|	expresion ',' expresion_asignacion
;

expresion_asignacion
	:	expresion_condicional
	|	expresion_unaria operador_asignacion expresion_asignacion {sprintf($$ + strlen($$), " %s %s", $2, $3);}
;

// Aca falta el ternario
expresion_condicional
	:	expresion_logical_or
;

operador_asignacion
	:	'='	{strcpy($$, "=");}
	|	SUM_ASSIGN {strcpy($$, "+=");}
	|	SUB_ASSIGN {strcpy($$, "-=");}
	|	MUL_ASSIGN {strcpy($$, "*=");}
	|	DIV_ASSIGN {strcpy($$, "/=");}
	|	MOD_ASSIGN {strcpy($$, "%=");}
;

expresion_logical_or
	:	expresion_logical_and
	|	expresion_logical_or OR_OP expresion_logical_and {sprintf($$ + strlen($$), " %s %s", "||", $3);}
;

expresion_logical_and
	:	expresion_o_inclusivo
	|	expresion_logical_and AND_OP expresion_o_inclusivo {sprintf($$ + strlen($$), " %s %s", "&&", $3);}
;

expresion_o_inclusivo
	:	expresion_o_excluyente
	|	expresion_o_excluyente '|' expresion_y {sprintf($$ + strlen($$), " | %s", $3);}
;

expresion_o_excluyente
	:	expresion_y
	|	expresion_o_excluyente '^' expresion_y {sprintf($$ + strlen($$), " ^ %s", $3);}
;

expresion_y
	:	expresion_igualdad
	|	expresion_y '&' expresion_igualdad {sprintf($$ + strlen($$), " & %s", $3);}
;

expresion_igualdad
	:	expresion_relacional
	|	expresion_igualdad EQ_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "==", $3);}
	|	expresion_igualdad NE_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "!=", $3);}
;

expresion_relacional
	:	expresion_corrimiento
	|	expresion_relacional '<' expresion_corrimiento {sprintf($$ + strlen($$), " < %s", $3);}
	|	expresion_relacional '>' expresion_corrimiento {sprintf($$ + strlen($$), " > %s", $3);}
	|	expresion_relacional LE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", "<=", $3);}
	|	expresion_relacional GE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", ">=", $3);}
;

expresion_corrimiento
	:	expresion_aditiva
	|	expresion_corrimiento SHIFT_LEFT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", "<<", $3);}
	|	expresion_corrimiento SHIFT_RIGHT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", ">>", $3);}
;

expresion_aditiva
	:	expresion_multiplicativa
	|	expresion_aditiva '+' expresion_multiplicativa {sprintf($$ + strlen($$), " + %s", $3);}
	|	expresion_aditiva '-' expresion_multiplicativa {sprintf($$ + strlen($$), " - %s", $3);}
;

expresion_multiplicativa
	:	expresion_conversion
	|	expresion_multiplicativa '*' expresion_conversion {sprintf($$ + strlen($$), " * %s", $3);}
	|	expresion_multiplicativa '/' expresion_conversion {sprintf($$ + strlen($$), " / %s", $3);}
	|	expresion_multiplicativa '%' expresion_conversion {sprintf($$ + strlen($$), " %c %s", 37, $3);}
;

expresion_conversion
	:	expresion_unaria
	|	'(' TIPO_DATO ')' expresion_conversion {sprintf($$ + strlen($$), "(%s) %s", $2, $4);}
;

expresion_unaria
	:	expresion_sufijo
	|	INC_OP expresion_unaria {sprintf($$ + strlen($$), "++%s", $2);}
	|	DEC_OP expresion_unaria {sprintf($$ + strlen($$), "--%s", $2);}
	|	unary_op expresion_conversion {sprintf($$ + strlen($$), "%s %s", $1, $2);}
	|	SIZEOF expresion_unaria {sprintf($$ + strlen($$), "sizeof %s", $2);}
	|	SIZEOF '(' TIPO_DATO ')'	{sprintf($$ + strlen($$), "sizeof(%s)", $3);}
;

unary_op
	:	'&' {strcat($$, "$");}
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
expresion_sufijo
	:	expresion_primaria
	|	expresion_sufijo '[' expresion ']' {sprintf($$ + strlen($$), "[%s]", $3);}
	|	expresion_sufijo '(' lista_argumentos ')' {sprintf($$ + strlen($$), "(%s)", $3);}
	|	expresion_sufijo '.' IDENTIFIER {sprintf($$ + strlen($$), ".%s", $3);}
	|	expresion_sufijo PTR_ARROW IDENTIFIER {sprintf($$ + strlen($$), "->%s", $3);}
	|	expresion_sufijo INC_OP {strcat($$, "++");}
	|	expresion_sufijo DEC_OP {strcat($$, "--");}
;

lista_argumentos
	:	expresion_asignacion
	|	lista_argumentos ',' expresion_asignacion {sprintf($$ + strlen($$), "%s, %s", $1, $3);}
;

expresion_primaria
	:	IDENTIFIER
	|	constante
	|	STRING
	|	'(' expresion ')' {sprintf($$ + strlen($$), "(%s)", $2);}
;

expresion_constante
	: expresion_condicional
;

// Ver lo del enum. Si es solo identificador, entonces expresion_primaria ya es suficiente
constante
	:	DECIMAL_CONSTANT {sprintf($$, "%ld", $1);}
	|	CONSTANTE_CARACTER {sprintf($$, "%c", $1);}
	|	CONSTANTE_REAL {sprintf($$, "%lf", $1);}
;

/*

FIN EXPRESION

*/

/*
	DECLARACIONES
*/

declaracion
	: especificadores_declaracion lista_declaradores_2
;

especificadores_declaracion
	:	CLASE_ALMACENAMIENTO especificadores_declaracion_2
	|	TIPO_DATO especificadores_declaracion_2
	|	CALIFICADOR_TIPO especificadores_declaracion_2
;

especificadores_declaracion_2: /* Nada */
	|	especificadores_declaracion
;

lista_declaradores_2: /* Nada */
	|	lista_declaradores
;

lista_declaradores
	:	declarador
	|	lista_declaradores ',' declarador
;

declarador
	:	decla
	|	decla '=' inicializador
;

inicializador
	:	expresion_asignacion
	|	'{' lista_inicializadores '}'
	|	'{' lista_inicializadores ',' '}'
;

lista_inicializadores
	:	inicializador
	|	lista_inicializadores ',' inicializador
;

especificador_struct_union
	:	STRUCT_UNION iden_o_nada '{' lista_declaraciones_struct '}'
	|	STRUCT_UNION IDENTIFIER
;

lista_declaraciones_struct
	:	declaracion_struct
	|	lista_declaraciones_struct declaracion_struct
;

declaracion_struct
	:	lista_calificadores declaradores_struct ';'
;

lista_calificadores
	:	TIPO_DATO lista_calificadores_2
	|	CALIFICADOR_TIPO lista_calificadores_2
;

lista_calificadores_2: /* Nada */
	|	lista_calificadores
;

declaradores_struct
	:	decla_struct
	|	declaradores_struct ',' decla_struct
;

// Falta una parte aca
decla_struct
	:	decla
;

decla
	:	puntero declarador_directo
	|	declarador_directo
;

puntero
	:	'*' lista_calificadores_tipo_2
	|	'*' lista_calificadores_tipo_2 puntero
;

lista_calificadores_tipo_2
	:	lista_calificadores_tipo
	|	/* Nada */
;

lista_calificadores_tipo
	:	CALIFICADOR_TIPO
	|	lista_calificadores_tipo CALIFICADOR_TIPO
;

declarador_directo
	:	IDENTIFIER
	|	'(' decla ')'
	|	declarador_directo '[' expresion_constante_2 ']'
	|	declarador_directo '(' lista_tipos_parametro ')'
	|	declarador_directo '(' lista_identificadores_2 ')'
;

lista_tipos_parametro
	:	lista_parametros
	|	lista_parametros ',' ELLIPSIS
;

lista_parametros
	:	declaracion_parametro
	|	lista_parametros ',' declaracion_parametro
;

declaracion_parametro
	:	especificadores_declaracion decla
	|	especificadores_declaracion declarador_abstracto_2
;

lista_identificadores_2: /* Nada */
	|	lista_identificadores
;

lista_identificadores
	:	IDENTIFIER
	|	lista_identificadores ',' IDENTIFIER
;

especificador_enum
	:	ENUM iden_o_nada '{' lista_enumeradores '}'
	|	ENUM IDENTIFIER
;

lista_enumeradores
	:	enumerador
	|	lista_enumeradores ',' enumerador
;

// constante_enumeracion es tan solo IDENTIFIER
enumerador
	:	IDENTIFIER
	|	IDENTIFIER '=' expresion_constante
;

nombre_typedef
	: IDENTIFIER
;

nombre_tipo
	: lista_calificadores declarador_abstracto_2
;

declarador_abstracto_2
	:	declarador_abstracto
	|	/* Nada */
;

declarador_abstracto
	:	puntero
	|	puntero_2 declarador_abstracto_directo
;

puntero_2
	:	puntero
	|	/* Nada */
;

declarador_abstracto_directo
	:	'(' declarador_abstracto_directo ')'
	|	declarador_abstracto_directo_2 opt_declarador_abstracto_directo
;

opt_declarador_abstracto_directo
	:	'[' expresion_constante_2 ']'
	|	'(' lista_tipos_parametro_2 ')'
;

declarador_abstracto_directo_2: /* Nada */
	|	declarador_abstracto	
;

expresion_constante_2
	:	expresion_constante
	|	/* Nada */
;

lista_tipos_parametro_2
	:	lista_tipos_parametro
	|	/* Nada */
;

iden_o_nada
	:	IDENTIFIER
	|	/* Nada */
;
/*
	FIN DECLARACIONES
*/

sentencia
	:	sentencia_expresion
	|	sentencia_compuesta
	|	sentencia_seleccion
	|	sentencia_iteracion
	|	sentencia_etiquetada
	|	sentencia_salto
;

sentencia_expresion
	: expresion_2 ';'
;

expresion_2
	:	expresion
	|	/* Nada */
;

sentencia_compuesta
	: '{' lista_declaraciones_2 lista_sentencias_2 '}'
;

lista_declaraciones_2: /* Nada */
	|	lista_declaraciones
;

lista_sentencias_2: /* Nada */
	|	lista_sentencias
;

lista_declaraciones
	:	declaracion
	|	lista_declaraciones declaracion
;

lista_sentencias
	:	sentencia
	|	lista_sentencias sentencia
;

sentencia_seleccion
	:	IF '(' expresion ')' sentencia opcion_else
	|	SWITCH '(' expresion ')' sentencia
;

opcion_else
	: ELSE sentencia
;

sentencia_iteracion
	:	WHILE '(' expresion ')' sentencia
	|	DO sentencia WHILE '(' expresion ')'
	|	FOR '(' expresion_2 ';' expresion_2 ';' expresion_2 ')' sentencia
;

sentencia_etiquetada
	:	CASE expresion_constante ":" sentencia
	|	DEFAULT ':' sentencia
	|	IDENTIFIER ':' sentencia
;

sentencia_salto
	:	CONTINUE ';'
	|	BREAK ';'
	|	RETURN expresion_2 ';'
	|	GOTO IDENTIFIER ';'
;

unidad_traduccion
	:	declaracion_externa {printf("%s", yylval.cadena);}
	|	unidad_traduccion declaracion_externa
;

declaracion_externa
	:	definicion_funcion
	|	declaracion
;

definicion_funcion
	:	especificadores_declaracion_2 decla lista_declaraciones_2 sentencia_compuesta
;
%%

int yyerror (char *s) {
  printf ("%s\n", s);
}

void main() {
   yyparse();
}