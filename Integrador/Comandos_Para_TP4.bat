bison -d ./src/bison.y
flex ./src/lex.l
move bison.tab.h .\headers\bison.tab.h
gcc -D WINCOMP bison.tab.c lex.yy.c ./lib/archivosComentarios.c ./lib/colores.c  ./lib/declaraciones.c ./lib/errores.c ./lib/sentencias.c ./lib/funciones.c ./lib/str_func.c ./lib/ts/errorSemantico.c ./lib/ts/tipos.c ./lib/ts/ts_declaraciones.c ./lib/ts/ts_llamadasFunciones.c ./lib/ts/ts_definicionFunciones.c ./lib/ts/validarSuma.c -o Analizador_Sintactico