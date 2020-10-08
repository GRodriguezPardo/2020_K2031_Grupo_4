Por como esta hecho el Makefile, hay que tener esto en cuenta: 
 
- Todos los archivos .c van en la carpeta **lib**. Hay que agregar **nombre-de-archivo.o** a la variable **_OBJ**
- Los **.h** van en la carpeta **headers** y hay que agregarlos a la variable **_HEADERS**

## Anotaciones importantes

El actual analizador sintactico no reconoce correctamente los tipos de datos declarados por TYPEDEF. Esto sucede porque, en esencia, estos tipos de dato declarados por TYPEDEF
son identificadores, pero al no poseer una TS (Tabla de Simbolos) es imposible diferenciar si es un identificador o un tipo de dato. Es por esto que esto no esta soportado en este TP