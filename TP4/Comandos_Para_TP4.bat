bison -d ./src/bison.y
flex ./src/lex.l
move bison.tab.h .\headers\bison.tab.h
gcc -D WINCOMP bison.tab.c lex.yy.c ./lib/archivosComentarios.c ./lib/colores.c ./lib/concatenarString.c ./lib/declaraciones.c ./lib/errores.c ./lib/sentencias.c ./lib/funciones.c -o Analizador_Sintactico