%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/bison.tab.h"
#include "./headers/funciones.h"

unsigned int line = 1;
short hayError = 0;
%}
DIGITO_DECIMAL [0-9] 
DIGITO_OCTAL [0-7] 
DIGITO_HEXADECIMAL [a-fA-F0-9] 
LETRA [a-zA-Z]
IDENTIFICADOR ({LETRA}|"_")({LETRA}|{DIGITO_DECIMAL}|"_")*

TIPO_DATO "void"|"char"|"short"|"int"|"long"|"float"|"double"|"signed"|"unsigned"
CLASE_ALMACENAMIENTO "auto"|"register"|"static"|"extern"|"typedef"
CALIFICADOR_TIPO "const"|"volatile"

SINGLE_OPERATOR "&"|"*"|"^"|"~"|"="|"-"|"+"|"/"|"%"|"<"|">"|"!"

CONSTANTE_REAL1 ({DIGITO_DECIMAL}+(e|E)(-|\+)?{DIGITO_DECIMAL}+(f|F|l|L)?)
CONSTANTE_REAL2 (({DIGITO_DECIMAL}*?\.{DIGITO_DECIMAL}+)|({DIGITO_DECIMAL}+\.))((E|e)(-|\+)?{DIGITO_DECIMAL}+)?(f|F|l|L)?
CARACTERES_PUNTUACION "#"|"("|")"|","|":"|"?"|"@"|"["|"]"|"_"|"{"|"}"|";"
CARACTERES_ESCAPE "\\a"|"\\b"|"\\e"|"\\f"|"\\n"|"\\r"|"\\t"|"\\v"|"\\"|"\\'"|"\""|"\\?"|"\\0"
%%
"/*"[^"/*""*/"]*"*/" {
      contarLineas(yytext);
      }

"enum" {
      return ENUM;
}

{TIPO_DATO} {
      strcpy(yylval.cadena, yytext);
      return TIPO_DATO;
}

{CLASE_ALMACENAMIENTO} {
      strcpy(yylval.cadena, yytext);
      return CLASE_ALMACENAMIENTO;
}
{CALIFICADOR_TIPO} {
      strcpy(yylval.cadena, yytext);
      return CALIFICADOR_TIPO;
}

{SINGLE_OPERATOR} {
      return yytext[0]; 
}

[1-9]{DIGITO_DECIMAL}* {
      yylval.ival = atoi(yytext);
      return DECIMAL_CONSTANT;
}
0{DIGITO_OCTAL}* {
      yylval.ival = strtol(yytext, NULL, 10);
      return OCTAL_CONSTANT;
}
0[xX]{DIGITO_HEXADECIMAL}+ {
      yylval.ival = strtol(yytext, NULL, 10);
      return HEX_CONSTANT;
}

\"([^"]|"\\\"")*\" {
      strcpy(yylval.cadena, yytext);
      return STRING;
}
'(.|{CARACTERES_ESCAPE})' {
      strcpy(yylval.cadena, yytext);
      return CONSTANTE_CARACTER;
}

"//".* {
}

"#define ".* {}

"#include ".* {}

{CONSTANTE_REAL1}|{CONSTANTE_REAL2} {
      yylval.fval = strtod(yytext, NULL);
      return CONSTANTE_REAL;
}
{CARACTERES_PUNTUACION} {
      return yytext[0];
}

"->" {return(PTR_ARROW);}
"sizeof" {return(SIZEOF);}
">>"  {return(SHIFT_RIGHT);}
"<<"  {return(SHIFT_LEFT);}
"++"  {return(INC_OP);}
"--"  {return(DEC_OP);}
"&&"	{return(AND_OP);}
"||"  {return(OR_OP);}
"<="  {return(LE_OP);}
">="  {return(GE_OP);}
"=="  {return(EQ_OP);}
"!="  {return(NE_OP);}
"+="  {return(SUM_ASSIGN);}       
"-="  {return(SUB_ASSIGN);}
"/="  {return(DIV_ASSIGN);}
"*="  {return(MUL_ASSIGN);}
"%="  {return(MOD_ASSIGN);}
"..." {return(ELLIPSIS);}
"if"  {return(IF);}
"else"      {return(ELSE);}
"switch"    {return(SWITCH);}
"while"     {return(WHILE);}
"do"        {return(DO);}
"for"       {return(FOR);}
"case"      {return(CASE);}
"default"   {return(DEFAULT);}
"continue"  {return(CONTINUE);}
"break"     {return(BREAK);}
"return"    {return(RETURN);}
"goto"      {return(GOTO);}

("struct"|"union") {
      strcpy(yylval.cadena, yytext);
      return STRUCT_UNION;
}

{IDENTIFICADOR} {
      strcpy(yylval.cadena, yytext);
      return IDENTIFIER;
}

" "|\9|"\t" {
      // No hacemos nada
}

"\n"|"\r\n"|"\r" {
      line++;
}

<<EOF>> {
      return 0;
}

. {
      hayError = 1;
      agregarError(&tail_errores, line);

      if(head_errores == NULL)
            head_errores = tail_errores;
}


%%