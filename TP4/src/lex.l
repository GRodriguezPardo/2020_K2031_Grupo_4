%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headers/bison.tab.h"
%}
DIGITO_DECIMAL [0-9] 
DIGITO_OCTAL [0-7] 
DIGITO_HEXADECIMAL [a-fA-F0-9] 
LETRA [a-zA-Z]
IDENTIFICADOR ({LETRA}|"_")({LETRA}|{DIGITO_DECIMAL}|"_")*

TIPO_DATO "void"|"char"|"short"|"int"|"long"|"float"|"double"|"signed"|"unsigned"|"struct"|"union"|"enum"
STORAGE_CLASS "auto"|"register"|"static"|"extern"|"typedef"|"const"|"volatile"
ESTRUCTURA_CONTROL "break"|"else"|"switch"|"case"|"return"|"continue"|"for"|"default"|"goto"|"do"|"if"|"while"

OPERADORES_ARIMETICOS "-"|"+"|"/"|"%"
OPERADORES_UNARIOS "++"|"--"
OPERADORES_RELACIONALES "=="|"!="|"<"|">"|"<="|">="
OPERADORES_LOGICOS "&&"|"||"|"!"
OPERADORES_BIT "^"|"~"|"<<"|">>"
OPERADORES_ASIGNACION "="|"+="|"-+"|"*="|"/="|"%="|"<<="|">>="|"&="|"^="|"|="
OPERADOR_OTROS "&"|"*"|"->"

CONSTANTE_REAL1 ({DIGITO_DECIMAL}+(e|E)(-|\+)?{DIGITO_DECIMAL}+(f|F|l|L)?)
CONSTANTE_REAL2 (({DIGITO_DECIMAL}*?\.{DIGITO_DECIMAL}+)|({DIGITO_DECIMAL}+\.))((E|e)(-|\+)?{DIGITO_DECIMAL}+)?(f|F|l|L)?
CARACTERES_PUNTUACION "#"|"$"|"("|")"|","|":"|"?"|"@"|"["|"]"|"_"|"{"|"}"|";"
CARACTERES_ESCAPE "\\a"|"\\b"|"\\e"|"\\f"|"\\n"|"\\r"|"\\t"|"\\v"|"\\"|"\\'"|"\""|"\\?"|"\\0"
%%
"/*"[^"/*""*/"]*"*/" {
      
      }

{TIPO_DATO} {
      strcpy(yylval.cadena, yytext);
      return DATA_TYPE;
}
{OPERADORES_UNARIOS} {
      strcpy(yylval.cadena, yytext);
      return UNARY_OPERATOR;
}
{STORAGE_CLASS} {
      strcpy(yylval.cadena, yytext);
      return DATA_QUALIFIER;
}
{ESTRUCTURA_CONTROL} {}

[1-9]{DIGITO_DECIMAL}* {}
0{DIGITO_OCTAL}* {}
0[xX]{DIGITO_HEXADECIMAL}+ {}
{IDENTIFICADOR} {
      strcpy(yylval.cadena, yytext);
      return IDENTIFIER;
}

{OPERADORES_ARIMETICOS} {}
{OPERADORES_LOGICOS} {}
{OPERADORES_RELACIONALES} {}
{OPERADORES_BIT} {}
{OPERADORES_ASIGNACION} {}
{OPERADOR_OTROS} {}

\"([^"]|"\\\"")*\" {}
'(.|{CARACTERES_ESCAPE})' {}

"//".* {}

"#define ".* {}

"#include ".* {}

{CONSTANTE_REAL1}|{CONSTANTE_REAL2} {}
{CARACTERES_PUNTUACION} {
      return yytext[0];
}


" "|\9|"\t" {
      // No hacemos nada
}

. {
      
}


%%