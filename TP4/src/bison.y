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
%type <cadena> expresion_primaria constante nombre_tipo

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

expresion_primaria
	:	IDENTIFIER
	|	constante
	|	STRING
	|	'(' expresion ')' {sprintf($$ + strlen($$), "(%s)", $2);}
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

expresion_unaria
	:	expresion_sufijo
	|	INC_OP expresion_unaria {sprintf($$ + strlen($$), "++%s", $2);}
	|	DEC_OP expresion_unaria {sprintf($$ + strlen($$), "--%s", $2);}
	|	unary_op expresion_conversion {sprintf($$ + strlen($$), "%s %s", $1, $2);}
	|	SIZEOF expresion_unaria {sprintf($$ + strlen($$), "sizeof %s", $2);}
	|	SIZEOF '(' nombre_tipo ')'	{sprintf($$ + strlen($$), "sizeof(%s)", $3);}
;

unary_op
	:	'&' {strcat($$, "$");}
	|	'*' {strcpy($$, "*");}
	|	'+' {strcat($$, "+");}
	|	'-' {strcat($$, "-");}
	|	'~' {strcat($$, "~");}
	|	'!' {strcat($$, "!");}
;

expresion_conversion
	:	expresion_unaria
	|	'(' nombre_tipo ')' expresion_conversion {sprintf($$ + strlen($$), "(%s) %s", $2, $4);}
;

expresion_multiplicativa
	:	expresion_conversion
	|	expresion_multiplicativa '*' expresion_conversion {sprintf($$ + strlen($$), " * %s", $3);}
	|	expresion_multiplicativa '/' expresion_conversion {sprintf($$ + strlen($$), " / %s", $3);}
	|	expresion_multiplicativa '%' expresion_conversion {sprintf($$ + strlen($$), " %c %s", 37, $3);}
;

expresion_aditiva
	:	expresion_multiplicativa
	|	expresion_aditiva '+' expresion_multiplicativa {sprintf($$ + strlen($$), " + %s", $3);}
	|	expresion_aditiva '-' expresion_multiplicativa {sprintf($$ + strlen($$), " - %s", $3);}
;

expresion_corrimiento
	:	expresion_aditiva
	|	expresion_corrimiento SHIFT_LEFT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", "<<", $3);}
	|	expresion_corrimiento SHIFT_RIGHT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", ">>", $3);}
;

expresion_relacional
	:	expresion_corrimiento
	|	expresion_relacional '<' expresion_corrimiento {sprintf($$ + strlen($$), " < %s", $3);}
	|	expresion_relacional '>' expresion_corrimiento {sprintf($$ + strlen($$), " > %s", $3);}
	|	expresion_relacional LE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", "<=", $3);}
	|	expresion_relacional GE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", ">=", $3);}
;

expresion_igualdad
	:	expresion_relacional
	|	expresion_igualdad EQ_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "==", $3);}
	|	expresion_igualdad NE_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "!=", $3);}
;

expresion_y
	:	expresion_igualdad
	|	expresion_y '&' expresion_igualdad {sprintf($$ + strlen($$), " & %s", $3);}
;

expresion_o_excluyente
	:	expresion_y
	|	expresion_o_excluyente '^' expresion_y {sprintf($$ + strlen($$), " ^ %s", $3);}
;

expresion_o_inclusivo
	:	expresion_o_excluyente
	|	expresion_o_excluyente '|' expresion_y {sprintf($$ + strlen($$), " | %s", $3);}
;

expresion_logical_and
	:	expresion_o_inclusivo
	|	expresion_logical_and AND_OP expresion_o_inclusivo {sprintf($$ + strlen($$), " %s %s", "&&", $3);}
;

expresion_logical_or
	:	expresion_logical_and
	|	expresion_logical_or OR_OP expresion_logical_and {sprintf($$ + strlen($$), " %s %s", "||", $3);}
;

// Aca falta el ternario
expresion_condicional
	:	expresion_logical_or
	|	expresion_logical_or '?' expresion ':' expresion_condicional
;

expresion_asignacion
	:	expresion_condicional
	|	expresion_unaria operador_asignacion expresion_asignacion {sprintf($$ + strlen($$), " %s %s", $2, $3);}
;

operador_asignacion
	:	'='	{strcpy($$, "=");}
	|	SUM_ASSIGN {strcpy($$, "+=");}
	|	SUB_ASSIGN {strcpy($$, "-=");}
	|	MUL_ASSIGN {strcpy($$, "*=");}
	|	DIV_ASSIGN {strcpy($$, "/=");}
	|	MOD_ASSIGN {strcpy($$, "%=");}
;

expresion
	:	expresion_asignacion {printf("%s", $1);}
	|	expresion ',' expresion_asignacion
;

expresion_constante
	: expresion_condicional
;

// Ver lo del enum. Si es solo identificador, entonces expresion_primaria ya es suficiente
constante
	:	DECIMAL_CONSTANT {sprintf($$, "%ld", $1);}
	|	CONSTANTE_CARACTER {sprintf($$, "%c", $1);}
	|	CONSTANTE_REAL {sprintf($$, "%lf", $1);}
	|	OCTAL_CONSTANT {sprintf($$, "%ld", $1);}
	|	HEX_CONSTANT {sprintf($$, "%ld", $1);}
;

/*

FIN EXPRESION

*/

/*

	DECLARACIONES

*/

declaracion
	:	especificadores_declaracion ';'
	|	especificadores_declaracion lista_declaradores ';'
;

especificadores_declaracion
	:	CLASE_ALMACENAMIENTO
	|	CLASE_ALMACENAMIENTO especificadores_declaracion
	|	especificador_tipo
	|	especificador_tipo especificadores_declaracion
	|	CALIFICADOR_TIPO
	|	CALIFICADOR_TIPO especificadores_declaracion
;

lista_declaradores
	:	inicializador_declarador
	|	lista_declaradores ',' inicializador_declarador
;

inicializador_declarador
	:	declarador
	|	declarador '=' inicializador
;

especificador_tipo
	:	TIPO_DATO
	|	especificador_struct_union
	|	especificador_enum
	|	nombre_tipo
;

especificador_struct_union
	:	STRUCT_UNION IDENTIFIER '{' lista_declaraciones_struct '}'
	|	STRUCT_UNION '{' lista_declaraciones_struct '}'
	|	STRUCT_UNION IDENTIFIER
;

lista_declaraciones_struct
	:	declaracion_struct
	|	lista_declaraciones_struct ',' declaracion_struct
;

declaracion_struct
	:	lista_especificador_calificador lista_declaradores_struct ';'
;

lista_especificador_calificador
	:	especificador_tipo lista_declaraciones_struct
	|	especificador_tipo
	|	CALIFICADOR_TIPO lista_declaraciones_struct
	|	CALIFICADOR_TIPO
;

lista_declaradores_struct
	:	declarador_struct
	|	lista_declaradores_struct ',' declarador_struct
;

declarador_struct
	:	declaracion
	|	':' expresion_constante
	|	declarador ':' expresion_constante
;

especificador_enum
	:	ENUM '{' lista_enumerador '}'
	|	ENUM IDENTIFIER '{' lista_enumerador '}'
	|	ENUM IDENTIFIER
;

lista_enumerador
	:	enumerador
	|	lista_enumerador ',' enumerador
;

enumerador
	:	IDENTIFIER
	|	IDENTIFIER '=' expresion_constante
;

declarador
	:	puntero declarador_directo
	|	declarador_directo
;

declarador_directo
	:	IDENTIFIER
	|	'(' declarador ')'
	|	declarador_directo '[' expresion_constante ']'
	|	declarador_directo '[' ']'
	|	declarador_directo '(' lista_tipo_parametro ')'
	|	declarador_directo '(' lista_identificadores ')'
	|	declarador_directo '(' ')'
;

puntero
	:	'*'
	|	'*' lista_calificador_tipo
	|	'*' puntero
	|	'*' lista_calificador_tipo puntero
;

lista_calificador_tipo
	:	CALIFICADOR_TIPO
	|	lista_calificador_tipo CALIFICADOR_TIPO
;

lista_tipo_parametro
	:	lista_parametro
	|	lista_parametro ',' ELLIPSIS
;

lista_parametro
	:	declaracion_parametro
	|	lista_parametro ',' declaracion_parametro
;

declaracion_parametro
	:	especificadores_declaracion declarador
	|	especificadores_declaracion declarador_abstracto
	|	especificadores_declaracion
;

lista_identificadores
	:	IDENTIFIER
	|	lista_identificadores ',' IDENTIFIER
;

nombre_tipo
	:	lista_especificador_calificador
	|	lista_especificador_calificador declarador_abstracto
;

declarador_abstracto
	:	puntero
	|	declarador_abstracto_directo
	|	puntero declarador_abstracto_directo
;

declarador_abstracto_directo
	: '(' declarador_abstracto ')'
	| '[' ']'
	| '[' expresion_constante ']'
	| declarador_abstracto_directo '[' ']'
	| declarador_abstracto_directo '[' expresion_constante ']'
	| '(' ')'
	| '(' lista_tipo_parametro ')'
	| declarador_abstracto_directo '(' ')'
	| declarador_abstracto_directo '(' lista_tipo_parametro ')'
;

inicializador
	:	expresion_asignacion
	|	'{' lista_inicializador '}'
	|	'{' lista_inicializador ',' '}'
;

lista_inicializador
	:	inicializador
	|	lista_inicializador ',' inicializador
;

/*

	FIN DECLARACIONES

*/

/*

	SENTENCIAS

*/

sentencia
	:	sentencia_etiquetada
	|	sentencia_compuesta
	|	sentencia_expresion
	|	sentencia_seleccion
	|	sentencia_iteracion
	|	sentencia_salto
;

sentencia_etiquetada
	:	IDENTIFIER ':' sentencia
	|	CASE expresion_constante ':' sentencia
	|	DEFAULT ':' sentencia
;

sentencia_compuesta
	:	'{' '}'
	|	'{' lista_sentencia '}'
	|	'{' lista_declaraciones '}'
	|	'{' lista_declaraciones lista_sentencia '}'
;

lista_declaraciones
	:	declaracion
	|	lista_declaraciones declaracion
;

lista_sentencia
	:	sentencia
	|	lista_sentencia sentencia
;

sentencia_expresion
	:	';'
	|	expresion ';'
;

sentencia_seleccion
	:	IF '(' expresion ')' sentencia
	|	IF '(' expresion ')' sentencia ELSE sentencia
	|	SWITCH '(' expresion ')' sentencia
;

sentencia_iteracion
	:	WHILE '(' expresion ')' sentencia
	|	DO sentencia WHILE '(' expresion ')' ';'
	|	FOR '(' sentencia_expresion sentencia_expresion ')' sentencia
	|	FOR '(' sentencia_expresion sentencia_expresion expresion ')' sentencia
;

sentencia_salto
	:	GOTO IDENTIFIER ';'
	|	CONTINUE ';'
	|	BREAK ';'
	|	RETURN ';'
	|	RETURN expresion ';'
;

unidad_traduccion
	:	declaracion_externa
	|	unidad_traduccion declaracion_externa
;

declaracion_externa
	:	declaracion_funcion
	|	declaracion
;

declaracion_funcion
	:	especificadores_declaracion declarador lista_declaraciones sentencia_compuesta
	|	especificadores_declaracion declarador sentencia_compuesta
	|	declarador lista_declaraciones sentencia_compuesta
	|	declarador sentencia_compuesta
;

%%

int yyerror (char *s) {
  printf ("%s\n", s);
}

void main() {
   yyparse();
}