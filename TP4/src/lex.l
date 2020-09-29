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
STORAGE_CLASS "auto"|"register"|"static"|"extern"|"typedef"
ESTRUCTURA_CONTROL "break"|"else"|"switch"|"case"|"return"|"continue"|"for"|"default"|"goto"|"do"|"if"|"while"
CALIFICADOR_TIPO "const"|"volatile"

SINGLE_OPERATOR "&"|"*"|"^"|"~"|"="|"-"|"+"|"/"|"%"|"<"|">"

CONSTANTE_REAL1 ({DIGITO_DECIMAL}+(e|E)(-|\+)?{DIGITO_DECIMAL}+(f|F|l|L)?)
CONSTANTE_REAL2 (({DIGITO_DECIMAL}*?\.{DIGITO_DECIMAL}+)|({DIGITO_DECIMAL}+\.))((E|e)(-|\+)?{DIGITO_DECIMAL}+)?(f|F|l|L)?
CARACTERES_PUNTUACION "#"|"$"|"("|")"|","|":"|"?"|"@"|"["|"]"|"_"|"{"|"}"|";"
CARACTERES_ESCAPE "\\a"|"\\b"|"\\e"|"\\f"|"\\n"|"\\r"|"\\t"|"\\v"|"\\"|"\\'"|"\""|"\\?"|"\\0"
%%
"/*"[^"/*""*/"]*"*/" {
      
      }
{TIPO_DATO} {
      strcpy(yylval.cadena, yytext);
      return TYPE_SPECIFIER;
}

{STORAGE_CLASS} {
      strcpy(yylval.cadena, yytext);
      return STORAGE_CLASS;
}
{CALIFICADOR_TIPO} {
      strcpy(yylval.cadena, yytext);
      return TYPE_QUALIFIER;
}

{SINGLE_OPERATOR} {
      return yytext[0]; 
}

{ESTRUCTURA_CONTROL} {

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
{IDENTIFICADOR} {
      strcpy(yylval.cadena, yytext);
      return IDENTIFIER;
}


\"([^"]|"\\\"")*\" {
      strcpy(yylval.cadena, yytext);
      return STRING;
}
'(.|{CARACTERES_ESCAPE})' {
      return yytext[1];
}

"//".* {}

"#define ".* {}

"#include ".* {}

{CONSTANTE_REAL1}|{CONSTANTE_REAL2} {
      yylval.fval = strtod(yytext, NULL);
      return FLOAT_CONSTANT;
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

" "|\9|"\t" {
      // No hacemos nada
}

"\n" {
      // Esto es para poder trabajar en consola
      return 0;
}

. {
      
}


%%