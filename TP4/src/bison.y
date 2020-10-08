%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./headers/funciones.h"
int yylex();
int yyerror();

NodoDeclaracion* head_declaraciones = NULL;
NodoDeclaracion* tail_declaraciones = NULL;

NodoFuncion* head_funcion = NULL;
NodoFuncion* tail_funcion = NULL;

FILE* yyin;

#ifdef BDEBUG
	int yydebug = 1;
#endif

%}

%token <cadena> TIPO_DATO SIZEOF SHIFT_RIGHT SHIFT_LEFT GE_OP LE_OP OR_OP AND_OP NE_OP EQ_OP INC_OP DEC_OP
%token <cadena> CALIFICADOR_TIPO CLASE_ALMACENAMIENTO IDENTIFIER STRING ENUM
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
%type <cadena> expresion_primaria constante declaracion especificadores_declaracion especificadores_declaracion_2 lista_declaradores
%type <cadena> inicializador_declarador especificador_tipo especificador_struct_union lista_declaraciones_struct declaracion_struct lista_especificador_calificador
%type <cadena> opt_lista_especificador_calificador lista_declaradores_struct declarador_struct especificador_enum lista_enumerador
%type <cadena> enumerador declarador declarador_directo puntero puntero_2 opt_puntero lista_calificador_tipo lista_tipo_parametro 
%type <cadena> lista_parametro declaracion_parametro opt_declaracion_parametro lista_identificadores declarador_abstracto lista_bloque_codigo
%type <cadena> declarador_abstracto_directo inicializador lista_inicializador sentencia sentencia_etiquetada sentencia_compuesta
%type <cadena> sentencia_expresion sentencia_seleccion opcion_else sentencia_iteracion sentencia_salto unidad_traduccion declaracion_externa
%type <cadena> declaracion_funcion expresion_constante lista_argumentos_2 opt_declaracion nombre_tipo opt_nombre_tipo bloque_codigo lista_declaraciones

// %type <cadena> lista_sentencia

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

expresion_primaria:
		IDENTIFIER
	|	constante
	|	STRING
	|	'(' expresion ')' {sprintf($$ + strlen($$), "(%s)", $2);}
;

/*
	en la regla 1 se va a printear dos veces. Esto es esperado de bison ya que es una expresion
	adentro de otra expresion
*/

expresion_sufijo:
		expresion_primaria
	|	expresion_sufijo '[' expresion ']' {sprintf($$ + strlen($$), "[%s]", $3);}
	|	expresion_sufijo '(' lista_argumentos_2 ')' {sprintf($$ + strlen($$), "(%s)", $3);}
	|	expresion_sufijo '.' IDENTIFIER {sprintf($$ + strlen($$), ".%s", $3);}
	|	expresion_sufijo PTR_ARROW IDENTIFIER {sprintf($$ + strlen($$), "->%s", $3);}
	|	expresion_sufijo INC_OP {strcat($$, "++");}
	|	expresion_sufijo DEC_OP {strcat($$, "--");}
;

lista_argumentos:
		expresion_asignacion
	|	lista_argumentos ',' expresion_asignacion {sprintf($$ + strlen($$), "%s, %s", $1, $3);}
;

lista_argumentos_2:
		{memset($$, 0, sizeof($$));}
	|	lista_argumentos
;

expresion_unaria:
		expresion_sufijo
	|	INC_OP expresion_unaria {sprintf($$ + strlen($$), "++%s", $2);}
	|	DEC_OP expresion_unaria {sprintf($$ + strlen($$), "--%s", $2);}
	|	unary_op expresion_conversion {sprintf($$ + strlen($$), "%s %s", $1, $2);}
	|	SIZEOF expresion_unaria {sprintf($$ + strlen($$), "sizeof %s", $2);}
	|	SIZEOF '(' nombre_tipo ')'	{sprintf($$ + strlen($$), "sizeof(%s)", $3);}
;

unary_op:
		'&' {strcat($$, "$");}
	|	'*' {strcpy($$, "*");}
	|	'+' {strcat($$, "+");}
	|	'-' {strcat($$, "-");}
	|	'~' {strcat($$, "~");}
	|	'!' {strcat($$, "!");}
;

expresion_conversion:
		expresion_unaria
	|	'(' nombre_tipo ')' expresion_conversion {sprintf($$ + strlen($$), "(%s) %s", $2, $4);}
;

expresion_multiplicativa:
		expresion_conversion
	|	expresion_multiplicativa '*' expresion_conversion {sprintf($$ + strlen($$), " * %s", $3);}
	|	expresion_multiplicativa '/' expresion_conversion {sprintf($$ + strlen($$), " / %s", $3);}
	|	expresion_multiplicativa '%' expresion_conversion {sprintf($$ + strlen($$), " %c %s", 37, $3);}
;

expresion_aditiva:
		expresion_multiplicativa
	|	expresion_aditiva '+' expresion_multiplicativa {sprintf($$ + strlen($$), " + %s", $3);}
	|	expresion_aditiva '-' expresion_multiplicativa {sprintf($$ + strlen($$), " - %s", $3);}
;

expresion_corrimiento:
		expresion_aditiva
	|	expresion_corrimiento SHIFT_LEFT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", "<<", $3);}
	|	expresion_corrimiento SHIFT_RIGHT expresion_aditiva {sprintf($$ + strlen($$), " %s %s", ">>", $3);}
;

expresion_relacional:
		expresion_corrimiento
	|	expresion_relacional '<' expresion_corrimiento {sprintf($$ + strlen($$), " < %s", $3);}
	|	expresion_relacional '>' expresion_corrimiento {sprintf($$ + strlen($$), " > %s", $3);}
	|	expresion_relacional LE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", "<=", $3);}
	|	expresion_relacional GE_OP expresion_corrimiento {sprintf($$ + strlen($$), " %s %s", ">=", $3);}
;

expresion_igualdad:
		expresion_relacional
	|	expresion_igualdad EQ_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "==", $3);}
	|	expresion_igualdad NE_OP expresion_relacional {sprintf($$ + strlen($$), " %s %s", "!=", $3);}
;

expresion_y:
		expresion_igualdad
	|	expresion_y '&' expresion_igualdad {sprintf($$ + strlen($$), " & %s", $3);}
;

expresion_o_excluyente:
		expresion_y
	|	expresion_o_excluyente '^' expresion_y {sprintf($$ + strlen($$), " ^ %s", $3);}
;

expresion_o_inclusivo:
		expresion_o_excluyente
	|	expresion_o_excluyente '|' expresion_y {sprintf($$ + strlen($$), " | %s", $3);}
;

expresion_logical_and:
		expresion_o_inclusivo
	|	expresion_logical_and AND_OP expresion_o_inclusivo {sprintf($$ + strlen($$), " %s %s", "&&", $3);}
;

expresion_logical_or:
		expresion_logical_and
	|	expresion_logical_or OR_OP expresion_logical_and {sprintf($$ + strlen($$), " %s %s", "||", $3);}
;

expresion_condicional:
		expresion_logical_or
	|	expresion_logical_or '?' expresion ':' expresion_condicional
;

expresion_asignacion:
		expresion_condicional
	|	expresion_unaria operador_asignacion expresion_asignacion {sprintf($$ + strlen($$), " %s %s", $2, $3);}
;

operador_asignacion:
		'='	{strcpy($$, "=");}
	|	SUM_ASSIGN {strcpy($$, "+=");}
	|	SUB_ASSIGN {strcpy($$, "-=");}
	|	MUL_ASSIGN {strcpy($$, "*=");}
	|	DIV_ASSIGN {strcpy($$, "/=");}
	|	MOD_ASSIGN {strcpy($$, "%=");}
;

expresion:
		expresion_asignacion
	|	expresion ',' expresion_asignacion	{sprintf($$ + strlen($$), ", %s", $3);}
;

expresion_constante:
	 expresion_condicional
;

// Ver lo del enum. Si es solo identificador, entonces expresion_primaria ya es suficiente
constante:
		DECIMAL_CONSTANT {sprintf($$, "%ld", $1);}
	|	CONSTANTE_CARACTER
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

declaracion:
		especificadores_declaracion opt_declaracion {
			agregarDeclaracion(&tail_declaraciones, $1, $2);

			if(head_declaraciones == NULL)
				head_declaraciones = tail_declaraciones;
		}
;

opt_declaracion:
		';'		{strcpy($$, ";");}
	|	lista_declaradores ';'	{strcat($$, ";");}
;

// HAY QUE LIMPIAR EL ARRAY EN LAS REGLAS VACIAS

especificadores_declaracion:
		CLASE_ALMACENAMIENTO especificadores_declaracion_2	{sprintf($$, "%s %s", $1, $2);}
	|	especificador_tipo especificadores_declaracion_2	{sprintf($$, "%s %s", $1, $2);}
	|	CALIFICADOR_TIPO especificadores_declaracion_2	{sprintf($$, "%s %s", $1, $2);}
;

especificadores_declaracion_2:
		{memset($$, 0, sizeof($$));}
	|	especificadores_declaracion
;

lista_declaradores:
		inicializador_declarador
	|	lista_declaradores ',' inicializador_declarador		{sprintf($$ + strlen($$), ", %s", $3);}
;

inicializador_declarador:
		declarador
	|	declarador '=' inicializador	{sprintf($$ + strlen($$), " = %s", $3);}
;

especificador_tipo:
		TIPO_DATO
	|	especificador_struct_union
	|	especificador_enum
;

//	|	TIPO_TYPEDEF  -> Se necesita TS para implementar

especificador_struct_union:
		STRUCT_UNION IDENTIFIER '{' lista_declaraciones_struct '}'	{sprintf($$ + strlen($$), " %s {%s}", $2, $4);}
	|	STRUCT_UNION '{' lista_declaraciones_struct '}'		{sprintf($$ + strlen($$), "{%s}", $3);}
	|	STRUCT_UNION IDENTIFIER		{sprintf($$ + strlen($$), " %s", $2);}
;

lista_declaraciones_struct:
		declaracion_struct
	|	lista_declaraciones_struct declaracion_struct	{sprintf($$ + strlen($$), " %s", $2);}
;

declaracion_struct:
		lista_especificador_calificador lista_declaradores_struct ';'	{sprintf($$ + strlen($$), " %s;", $2);}
;

lista_especificador_calificador:
		especificador_tipo opt_lista_especificador_calificador	{sprintf($$ + strlen($$), " %s", $2);}
	|	CALIFICADOR_TIPO opt_lista_especificador_calificador	{sprintf($$ + strlen($$), " %s", $2);}
;

opt_lista_especificador_calificador:
		{memset($$, 0, sizeof($$));}
	|	lista_declaraciones_struct
;

lista_declaradores_struct:
		declarador_struct
	|	lista_declaradores_struct ',' declarador_struct	{sprintf($$ + strlen($$), ", %s", $3);}
;

declarador_struct:
		declarador
	|	':' expresion_constante	{sprintf($$, ": %s", $2);}
	|	declarador ':' expresion_constante	{sprintf($$ + strlen($$), ": %s", $3);}
;

especificador_enum:
		ENUM '{' lista_enumerador '}'	{sprintf($$ + strlen($$), "{%s}", $3);}
	|	ENUM IDENTIFIER '{' lista_enumerador '}'	{sprintf($$ + strlen($$), "{%s}", $4);}
	|	ENUM IDENTIFIER	{sprintf($$ + strlen($$), " %s", $2);}
;

lista_enumerador:
		enumerador
	|	lista_enumerador ',' enumerador	{sprintf($$ + strlen($$), ", %s", $3);}
;

enumerador:
		IDENTIFIER
	|	IDENTIFIER '=' expresion_constante {sprintf($$, "%s = %s", $1, $3);}
;

declarador:
		puntero declarador_directo {sprintf($$ + strlen($$), " %s", $2);}
	|	declarador_directo
;

declarador_directo:
		IDENTIFIER	
	|	'(' declarador ')'	{sprintf($$, "(%s)", $2);}
	|	declarador_directo '[' expresion_constante ']'	{sprintf($$ + strlen($$), "[%s]", $3);}
	|	declarador_directo '[' ']'		{strcat($$, "[]");}
	|	declarador_directo '(' lista_tipo_parametro ')'	{sprintf($$ + strlen($$), "(%s)", $3);}
	|	declarador_directo '(' lista_identificadores ')'	{sprintf($$ + strlen($$), "(%s)", $3);}
	|	declarador_directo '(' ')'	{strcat($$, "()");}
;

puntero:
		'*' opt_puntero	{sprintf($$, "* %s", $2);}
;

puntero_2:
		{memset($$, 0, sizeof($$));}
	|	puntero
;

opt_puntero:
		lista_calificador_tipo puntero_2	{sprintf($$ + strlen($$), " %s", $2);}
	|	puntero_2
;

lista_calificador_tipo:
		CALIFICADOR_TIPO
	|	lista_calificador_tipo CALIFICADOR_TIPO {sprintf($$ + strlen($$), " %s", $2);}
;

lista_tipo_parametro:
		lista_parametro
	|	lista_parametro ',' ELLIPSIS	{strcat($$, ", ...");}
;

lista_parametro:
		declaracion_parametro
	|	lista_parametro ',' declaracion_parametro	{sprintf($$ + strlen($$), ", %s", $3);}
;

declaracion_parametro:
		especificadores_declaracion opt_declaracion_parametro	{sprintf($$ + strlen($$), " %s", $2);}
;

opt_declaracion_parametro:
		{memset($$, 0, sizeof($$));}
	|	declarador
	|	declarador_abstracto
;

lista_identificadores:
		IDENTIFIER
	|	lista_identificadores ',' IDENTIFIER	{sprintf($$ + strlen($$), ", %s", $3);}
;

nombre_tipo:
		lista_especificador_calificador opt_nombre_tipo 	{sprintf($$ + strlen($$), " %s", $2);}
;

//nombre_tipo:	 {memset($$, 0, sizeof($$));}
//;

opt_nombre_tipo:
		{memset($$, 0, sizeof($$));}
	| declarador_abstracto
;

declarador_abstracto:
		puntero
	|	declarador_abstracto_directo
	|	puntero declarador_abstracto_directo {sprintf($$ + strlen($$), " %s", $2);}
;

declarador_abstracto_directo:
	 '(' declarador_abstracto ')'	{sprintf($$, "(%s)", $2);}
	| '[' ']'	{strcpy($$, "[]");}
	| '[' expresion_constante ']'		{sprintf($$, "[%s]", $2);}
	| declarador_abstracto_directo '[' ']'		{strcat($$, "[]");}
	| declarador_abstracto_directo '[' expresion_constante ']'	{sprintf($$ + strlen($$), "[%s]", $3);}
	| '(' ')'	{strcpy($$, "()");}
	| '(' lista_tipo_parametro ')'	{sprintf($$ + strlen($$), "(%s)", $2);}
	| declarador_abstracto_directo '(' ')'	{strcat($$, "()");}
	| declarador_abstracto_directo '(' lista_tipo_parametro ')' {sprintf($$ + strlen($$), "(%s)", $3);}
;

inicializador:
		expresion_asignacion
	|	'{' lista_inicializador '}'	{sprintf($$, "{ %s }", $2);}
	|	'{' lista_inicializador ',' '}' {sprintf($$, "{ %s , }", $2);}
;

lista_inicializador:
		inicializador
	|	lista_inicializador ',' inicializador {sprintf($$ + strlen($$), " , %s", $3);}
;

/*

	FIN DECLARACIONES

*/

/*

	SENTENCIAS

*/

sentencia:
		sentencia_etiquetada
	|	sentencia_compuesta
	|	sentencia_expresion
	|	sentencia_seleccion
	|	sentencia_iteracion
	|	sentencia_salto
;

sentencia_etiquetada:
		IDENTIFIER ':' sentencia
	|	CASE expresion_constante ':' sentencia
	|	DEFAULT ':' sentencia
;

sentencia_compuesta:
		'{' '}'	{strcpy($$, "{memset($$, 0, sizeof($$));}");}
	|	'{' lista_bloque_codigo '}' {sprintf($$, "{%s}", $2);}
	/*|	'{' lista_sentencia '}'	{sprintf($$, "{%s}", $2);}
	|	'{' lista_declaraciones '}'	{sprintf($$, "{%s}", $2);}
	|	'{' lista_declaraciones lista_sentencia '}'	{sprintf($$, "{%s %s}", $2, $3);}*/
;

lista_bloque_codigo:
		bloque_codigo
	|	lista_bloque_codigo bloque_codigo {sprintf($$ + strlen($$), " %s", $2);}
;

bloque_codigo:
		sentencia
	|	declaracion
;

lista_declaraciones:
		declaracion
	|	lista_declaraciones declaracion	{sprintf($$ + strlen($$), " %s", $2);}
;

/*lista_sentencia:
		sentencia
	|	lista_sentencia sentencia	{sprintf($$, " %s", $2);}
;*/

sentencia_expresion:
		';'	{strcpy($$, ";");}
	|	expresion ';'	{strcat($$, ";");}
;

sentencia_seleccion:
		IF '(' expresion ')' sentencia opcion_else	{sprintf($$, "if (%s) %s %s", $3, $5, $6);}
	|	SWITCH '(' expresion ')' sentencia	{sprintf($$, "switch (%s) %s", $3, $5);}
;

opcion_else:
		{memset($$, 0, sizeof($$));}
	|	ELSE sentencia	{sprintf($$, "else %s", $2);}
;

sentencia_iteracion:
		WHILE '(' expresion ')' sentencia	{sprintf($$, "while(%s) %s", $3, $5);}
	|	DO sentencia WHILE '(' expresion ')' ';'	{sprintf($$, "do %s while (%s)", $2, $5);}
	|	FOR '(' sentencia_expresion sentencia_expresion ')' sentencia	{sprintf($$, "for(%s%s) %s", $3, $4, $6);}
	|	FOR '(' sentencia_expresion sentencia_expresion expresion ')' sentencia	{sprintf($$, "for(%s%s%s) %s", $3, $4, $5, $7);}
	|	FOR '(' declaracion sentencia_expresion ')' sentencia	{sprintf($$, "for(%s%s) %s", $3, $4, $6);}
	|	FOR '(' declaracion sentencia_expresion expresion ')' sentencia	{sprintf($$, "for(%s%s%s) %s", $3, $4, $5, $7);}
;

sentencia_salto:
		GOTO IDENTIFIER ';'	{sprintf($$, "goto %s;", $2);}
	|	CONTINUE ';'	{strcpy($$, "continue;");}
	|	BREAK ';'	{strcpy($$, "break;");}
	|	RETURN ';'	{strcpy($$, "return;");}
	|	RETURN expresion ';'		{sprintf($$, "return %s;", $2);}
;

unidad_traduccion:
		declaracion_externa
	|	unidad_traduccion declaracion_externa
;

declaracion_externa:
		declaracion_funcion
	|	declaracion
;

declaracion_funcion:
		especificadores_declaracion declarador <cadena>{
			crearNodo(&tail_funcion, $1, $2);

			if(head_funcion == NULL)
				head_funcion = tail_funcion;
		} 
		sentencia_compuesta <cadena>{terminarFuncion(tail_funcion);}
	|	declarador sentencia_compuesta {sprintf($$ + strlen($$), " %s", $2);}
	|	especificadores_declaracion declarador lista_declaraciones sentencia_compuesta {sprintf($$ + strlen($$), " %s %s %s", $2, $3, $4);}
	|	declarador lista_declaraciones sentencia_compuesta {sprintf($$ + strlen($$), " %s %s", $2, $3);}
;

%%

int yyerror (char *s) {
  printf ("\n\nLinea %d | Error: %s\n\n", line, yylval.cadena);
  hayError = 1;
}

//	Estas regla sirven de algo?
// 
// 

void main() {
	setupFiles(&yyin);
  	yyparse();
	printearFuncion(head_funcion);
	printearDeclaraciones(head_declaraciones);

	printearMensajeFinal();

	// Pausa que anda en windows y linux
	char pausa;
	scanf("\n\nANALISIS FINALIZADO. PRESIONE CUALQUIER BOTON PARA SALIR.\n\n%c", pausa);
}