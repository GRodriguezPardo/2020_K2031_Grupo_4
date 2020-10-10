_HEADERS = bison.tab.h funciones.h
HEAD_DIR = ./headers
HEADERS = $(patsubst %,$(HEAD_DIR)/%,$(_HEADERS))

_LIB =
LDIR = ./lib

_OBJ = lex.yy.o bison.tab.o concatenarString.o archivosComentarios.o declaraciones.o colores.o funciones.o sentencias.o
ODIR = ./compile
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

CFLAGS = -c -o
BFLAG += -d
DGFLAG +=

CC = gcc

build: message makeFolder flex $(OBJ)
	@echo "\n\n===== COMPILACION FINAL =====\n"
	$(CC) $(OBJ) $(HEADERS)
	@echo "\n\n===== PROGRAMA COMPILADO CON EXITO ====="

debug: BFLAG += --debug
debug: DGFLAG += -D BDEBUG
debug: build

message:
	@echo "\n========== INICIANDO COMPILACION ========="

makeFolder:
	@if [ ! -d headers ]; then \
		mkdir headers; \
		echo "- Se creo carpeta headers"; \
	fi
	@echo "\n\n- CREANDO CARPETA compile"
	@if [ ! -d compile ]; then \
		mkdir compile; \
		echo "-  CARPETA CREADA"; \
	else \
		echo "-  CARPETA COMPILE EXISTENTE"; \
	fi

flex: bison
	@echo "\n\n===== GENERANDO ARCHIVO FLEX =====\n"
	flex ./src/lex.l
	$(CC) $(CFLAGS) ./compile/lex.yy.o lex.yy.c -lfl
	@echo "\n===== ARCHIVO FLEX GENERADO =====\n"

./compile/lex.yy.o:
	flex
	
bison:
	@echo "\n\n===== GENERANDO ARCHIVO BISON =====\n"
	bison $(BFLAG) ./src/bison.y
	mv bison.tab.h headers
	@echo "-  Moviendo headers"
	$(CC) $(DGFLAG) $(CFLAGS) ./compile/bison.tab.o bison.tab.c -ly
	@echo "\n===== ARCHIVO BISON GENERADO =====\n"

./compile/bison.tab.o:
	bison

./compile/%.o: $(LDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) $@ $<

output:
	bison -v ./src/bison.y

.PHONY: clean

# -rf esta para que no haya errores si el archivo no existe

cleanHeaders:
	@echo "===== LIMPIANDO =====\n"
	rm -rf ./headers/*.h.gch

clean: cleanHeaders
	rm -rf ./compile/*
	rm -rf bison.tab.c
	rm -rf ./headers/bison.tab.h
	rm -rf lex.yy.c
	rm -rf a.out
	rm -rf bison.output