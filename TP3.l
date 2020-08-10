%option noyywrap

%{
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include "superfunciones.c"

Node* listaCadenas=NULL;
Node* listaId=NULL;
Node2* listaPalRes=NULL;
Node* listaHexa=NULL;
Node* listaOctal=NULL;
Node2* listaDec=NULL;
Node2* listaReal=NULL;
Node2* listaCar=NULL;
Node* listaOp=NULL;
Node3* listaComent=NULL;
int linea = 1;
Node* listaNoRec=NULL;

%}

COMENTARIOS_UNA_LINEA (\/\/.*)
COMENTARIOS_MULTILINEA (\/\*[^(\*\/)]*\*\/)
DIGITO_DECIMAL [0-9]
DIGITO_OCTAL [0-7]
DIGITO_HEXADECIMAL [0-9a-fA-F]
PALABRAS_RESERVADAS auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|int|long|register|return|short|signed|static|struct|switch|typedef|union|unsigned|void|volatile|while 
IDENTIFICADOR [a-zA-Z_][a-zA-Z0-9_]*
SUFIJO [uU][lL]?|[lL][uU]?
SUFIJO_REAL [fF][lL]?|[lL][fF]?
LITERAL_CADENA \"([^"]|(\\\"))*\" 
SIZEOF sizeof
CONSTANTE_REAL [0-9]*\.[0-9]+|[0-9]+\.
PARTE_EXPONENCIAL [eE][-+]?[0-9]+
CONSTANTE_CARACTER '[^'\\]'
SECUENCIA_ESCAPE_SIMPLE '\\['"?\\abfnrtv]'
SECUENCIA_ESCAPE_OCTAL '\\[0-7]{1,3}'
SECUENCIA_ESCAPE_HEXADECIMAL '\\x[a-fA-F0-9]{1,2}'
PUNTUACION1 \[|\]|\(|\)|\{|\}
PUNTUACION2 \.|->|\+\+|--|&|\*|\+|-|~|!|\/|% 
PUNTUACION3 <<|>>|<|>|<=|>=|==|!=|\^|\||&&|\|\||=|\*=|\/=
PUNTUACION4 %=|\+=|-=|<<=|>>=|&=|\^=|\|=|\?
PUNTUACION5 :|;|\.\.\.|,|#|##
SALTO_LINEA \n
ESPACIOyTAB \40|\t

%%

\#include(\40)*(<[^\n\\/?*":<>|]*>|\"[^\n\\/?*":<>|]*\") {;}
\#define(\40)+{IDENTIFICADOR}(\(({IDENTIFICADOR},)*{IDENTIFICADOR}\))?((\40)+[^\n\\\40]*)? {;}
{ESPACIOyTAB} {;}
{PALABRAS_RESERVADAS} {char* nuevoPalRes = (char*)malloc(yyleng+1);
                       strcpy(nuevoPalRes, yytext);
                       listaPalRes = agregarPalRes(nuevoPalRes, listaPalRes);}
{SIZEOF} {char* nuevoSizeOf = (char*)malloc(yyleng+1);
          strcpy(nuevoSizeOf, yytext);
          listaPalRes = agregarPalRes(nuevoSizeOf, listaPalRes);
          listaOp = agregarId(nuevoSizeOf, listaOp);}
{COMENTARIOS_UNA_LINEA} {char* nuevoComentario = (char*)malloc(yyleng+1);
                         strcpy(nuevoComentario, yytext);
                         listaComent = agregarAListaComentarios(nuevoComentario, 0, listaComent);}
{COMENTARIOS_MULTILINEA} {char* nuevoComentario = (char*)malloc(yyleng+1);
                          strcpy(nuevoComentario, yytext);
                          listaComent = agregarAListaComentarios(nuevoComentario, 1, listaComent);
                          linea += lineasComentariosMultilinea(nuevoComentario, yyleng);}
[1-9]{DIGITO_DECIMAL}*({SUFIJO})? {char* nuevoDec = (char*)malloc(yyleng+1);
                                  strcpy(nuevoDec, yytext);
                                  listaDec = agregarConstanteDec(nuevoDec, listaDec);}
0{DIGITO_OCTAL}*({SUFIJO})? {char* nuevoOctal = (char*)malloc(yyleng+1);
                             strcpy(nuevoOctal, yytext);
                             listaOctal = agregarConstante(nuevoOctal, yyleng, listaOctal);}
0[xX]{DIGITO_HEXADECIMAL}+({SUFIJO})? {char* nuevoHexa = (char*)malloc(yyleng+1);
                                      strcpy(nuevoHexa, yytext);
                                      listaHexa = agregarConstante(nuevoHexa, yyleng, listaHexa);}
{CONSTANTE_REAL}{PARTE_EXPONENCIAL}?{SUFIJO_REAL}? {char* nuevoReal = (char*)malloc(yyleng+1);
                                                   strcpy(nuevoReal, yytext);
                                                   listaReal = agregarConstanteDec(nuevoReal, listaReal);}
[0-9]+{PARTE_EXPONENCIAL}{SUFIJO_REAL}? {char* nuevoReal = (char*)malloc(yyleng+1);
                                        strcpy(nuevoReal, yytext);
                                        listaReal = agregarConstanteDec(nuevoReal, listaReal);;}
{CONSTANTE_CARACTER} {char* nuevoCar = (char*)malloc(yyleng+1);
                      strcpy(nuevoCar, yytext);
                      listaCar = agregarPalRes(nuevoCar, listaCar);}
{SECUENCIA_ESCAPE_SIMPLE} {char* nuevoCar = (char*)malloc(yyleng+1);
                           strcpy(nuevoCar, yytext);
                           listaCar = agregarPalRes(nuevoCar, listaCar);}
{SECUENCIA_ESCAPE_OCTAL} {char* nuevoCar = (char*)malloc(yyleng+1);
                          strcpy(nuevoCar, yytext);
                          listaCar = agregarPalRes(nuevoCar, listaCar);}
{SECUENCIA_ESCAPE_HEXADECIMAL} {char* nuevoCar = (char*)malloc(yyleng+1);
                                strcpy(nuevoCar, yytext);
                                listaCar = agregarPalRes(nuevoCar, listaCar);}
{PUNTUACION1}|{PUNTUACION2}|{PUNTUACION3}|{PUNTUACION4}|{PUNTUACION5} {char* nuevoOp = (char*)malloc(yyleng+1);
                                                                       strcpy(nuevoOp, yytext);
                                                                       listaOp = agregarId(nuevoOp, listaOp);}
{IDENTIFICADOR} {char* nuevoId = (char*)malloc(yyleng+1);
                  strcpy(nuevoId, yytext);
                  listaId = agregarId(nuevoId, listaId);}
{LITERAL_CADENA} {char* nuevoLitCadena = (char*)malloc(yyleng+1);
                  strcpy(nuevoLitCadena, yytext);
                  listaCadenas = agregarCadena(nuevoLitCadena, (yyleng - 2), listaCadenas);}
{SALTO_LINEA} {linea++;}
. {char* nuevoNoRec = (char*)malloc(yyleng+1);
   strcpy(nuevoNoRec, yytext);
   listaNoRec = agregarCadena(nuevoNoRec, linea, listaNoRec);}

%%



int main() {

    FILE* f = fopen("reporte.txt", "w+t");

    yyin = fopen("entrada.txt", "r+t");

    yylex();

    printf(" -------------------------------------------\n -------------- R E P O R T E --------------\n -------------------------------------------\n");
    fprintf(f," -------------------------------------------\n -------------- R E P O R T E --------------\n -------------------------------------------\n");
    printf(" LISTA DE IDENTIFICADORES:------------------\n .\n .\tID\t\tAPARICIONES\n .\n");
    fprintf(f," LISTA DE IDENTIFICADORES:------------------\n .\n .\tID\t\tAPARICIONES\n .\n");
    imprimirId(listaId,f);
    printf(" .\n");
    fprintf(f," .\n");
    
    printf(" LISTA DE LITERALES CADENA:-----------------\n .\n .\tCADENA\t\t\t\tLONGITUD\n .\n");
    fprintf(f," LISTA DE LITERALES CADENA:-----------------\n .\n .\tCADENA\t\t\t\tLONGITUD\n .\n");
    imprimirCadenas(listaCadenas,f);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE PALABRAS RESERVADAS:--------------\n .\n .\tPALABRA\n .\n");
    fprintf(f," LISTA DE PALABRAS RESERVADAS:--------------\n .\n .\tPALABRA\n .\n");
    imprimirPalRes(listaPalRes,f);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE CONSTANTES HEXA:------------------\n .\n .\tVALOR HEXA\tVALOR DECIMAL\n .\n");
    fprintf(f," LISTA DE CONSTANTES HEXA:------------------\n .\n .\tVALOR HEXA\tVALOR DECIMAL\n .\n");
    imprimirHexa(listaHexa,f);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE CONSTANTES OCTAL:-----------------\n .\n .\tVALOR OCTAL\tVALOR DECIMAL\n .\n");
    fprintf(f," LISTA DE CONSTANTES OCTAL:-----------------\n .\n .\tVALOR OCTAL\tVALOR DECIMAL\n .\n");
    imprimirOctal(listaOctal,f);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE CONSTANTES DECIMAL:---------------\n .\n .\tVALOR DECIMAL\n .\n");
    fprintf(f," LISTA DE CONSTANTES DECIMAL:---------------\n .\n .\tVALOR DECIMAL\n .\n");
    int totalDecimal = imprimirDec(listaDec,f);
    printf(" .\n .\tTOTAL ACUMULADO DE CONSTANTES DECIMALES:  %d\n", totalDecimal);
    fprintf(f," .\n .\tTOTAL ACUMULADO DE CONSTANTES DECIMALES:  %d\n", totalDecimal);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE CONSTANTES REALES:----------------\n .\n .\tPARTE ENTERA\t\tPARTE FRACCIONAL\n .\n");
    fprintf(f," LISTA DE CONSTANTES REALES:----------------\n .\n .\tPARTE ENTERA\t\tPARTE FRACCIONAL\n .\n");
    imprimirReal(listaReal,f);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE CONSTANTES CARACTER:--------------\n .\n .\tCARACTER\n .\n");
    fprintf(f," LISTA DE CONSTANTES CARACTER:--------------\n .\n .\tCARACTER\n .\n");
    imprimirPalRes(listaCar,f);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE C.P y OPERADORES:-----------------\n .\n .\tCARACTER\tAPARICIONES\n .\n");
    fprintf(f," LISTA DE C.P y OPERADORES:-----------------\n .\n .\tCARACTER\tAPARICIONES\n .\n");
    imprimirId(listaOp,f);
    printf(" .\n");
    fprintf(f," .\n");
    
    printf(" LISTA DE COMENTARIOS:-----------------------\n .\n .\tTIPO\t\tCOMENTARIO\n .\n");
    fprintf(f," LISTA DE COMENTARIOS:-----------------------\n .\n .\tTIPO\t\t\t\tCOMENTARIO\n .\n");
    imprimirComentarios(listaComent, f);
    printf(" .\n");
    fprintf(f," .\n");

    printf(" LISTA DE CARACTERES NO RECONOCIDOS:--------\n .\n .\tCARACTER\tLINEA\n .\n");
    fprintf(f," LISTA DE CARACTERES NO RECONOCIDOS:--------\n .\n .\tCARACTER\tLINEA\n .\n");
    imprimirCadenas(listaNoRec,f);
    printf(" .\n");
    fprintf(f," .\n");
    
    fclose(f);
    return 0;
}
