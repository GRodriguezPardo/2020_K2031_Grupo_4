#include <stdio.h>  //Librería de C base
#include <ctype.h>  //Para ahorrar el tener que poner minúsculas y mayúsculas (función toupper)

void definirTipoConstante (){
	
	//Autómata Finito: los valores de la tabla son los estados, las columnas son las entradas y las filas son las salidas para un mismo estado:
	
	int tablaTransicion[7][6]={{2,1,1,6,6,6},                     //Primer columna: entró 0
							  {1,1,1,6,6,6},                      //Segunda columna: entró del 1 al 7
							  {3,3,6,6,4,6},                      //Tercer columna: entró 8 o 9
							  {3,3,6,6,6,6},                      //Cuarta columna: entró de la "a" a la "f"
							  {5,5,5,5,6,6},					  //Quinta columna: entró "x"
							  {5,5,5,5,6,6},                      //Sexta columna: entró un valor diferente al resto (Ejemplo, "j")
							  {6,6,6,6,6,6}};
	
	FILE* f = fopen ("Constantes.txt", "r");                          //Abro ambos archivos
	FILE* g = fopen ("ResultadosV1", "w");   
	
	//Inicio errores archivos
	if (f==NULL){
		printf("Error al abrir el archivo de lectura. Intente de nuevo.");
		return;
	}
	if (g==NULL){
		printf("Error al abrir el archivo de guardado. Intente de nuevo.");
		return;
	}
	//Fin errores archivos
	
	char constanteLeida[30];                                    			    //Para guardar la constante completa
	int i=0;                                                   				    //Para regular la cadena constanteLeida
	int c = getc (f);                                          	    
	int estado=0;                                              			        //El estado 0 es el incial, el valor va a ir cambiando a medida que se leen los caracteres
	int estadoFinal;                                         			        //Por temas de prolijidad. No hace realmente falta esta variable

	
	while(!feof(f)){                                        			        //El análisis finaliza cuando se termina de leer el archivo
		while (c!=',' && !feof(f)){                         			        //El caracter ',' es lo que indica el fin de la constante
			constanteLeida[i]=c;                            			        //Va cargando la constante completa
			i++;
			
			switch (toupper(c)){								   			    //Para evitar los casos en minúscula (no afecta a la variable 'c')
				case '0':
					estado=tablaTransicion[estado][0];        				    //Entró un cero, corresponde a la primer columna (indice 0) de la tabla, la fila es el estado en el cual se encontraba antes
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					estado=tablaTransicion[estado][1];			                //Entró un numero del 0-7, corresponde a la segunda columna (1)
					break;
				case '8':
				case '9':
					estado=tablaTransicion[estado][2];			                //Entró un 8 o un 9, corresponde a la tercer columna (2)
					break;
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					estado=tablaTransicion[estado][3];			                //Entró una letra de la 'a' a la 'f', corresponde a la cuarta columna (3)
					break;
				case 'X':
					estado=tablaTransicion[estado][4];			                //Entró una 'x', corresponde a la quinta columna (4)
					break;
				default:
					estado=tablaTransicion[estado][5];			                //Entró un caracter no valido, corresponde a la sexta columna (5). Esta columna y su estado (5) son de errores. Se entra y no se sale
			}
			c = getc(f);                                                        //Realiza un avance dentro de la cadena en progreso
		}
		constanteLeida[i]='\0';									                //Finaliza la constante
		i=0;													                //Reinicia el indice para la siguiente cadena
		estadoFinal=estado;										                //Establece el estado final (mejor dicho, último estado. Puede que no sea final)
		estado=0;												                //Reinicia el estado para la siguiente cadena
	
		switch (estadoFinal){									                //Cada estado corresponde a un tipo de constante diferente:
			case 1:
				fprintf (g, "La constante %s corresponde al tipo Decimal\n", constanteLeida); 
				break;
			case 2:
				fprintf (g, "La constante %s corresponde al valor 0 en cualqueir tipo de constante entera\n", constanteLeida);
				break;
			case 3:
				fprintf (g, "La constante %s corresponde al tipo Octal\n", constanteLeida);
				break;
			case 5:
				fprintf (g, "La constante %s corresponde al tipo Hexadecimal\n", constanteLeida);
				break;
			case 6:
			case 4:
				fprintf (g, "La constante %s no corresponde a ningun tipo de constante entera\n", constanteLeida);
				break;
		}
		c=getc(f);											      //Paso entre una cadena y otra
	}
	fclose(f);													  //Cierro ambos archivos
	fclose(g);
	return;
}

int main (){    //Pues el main, qué más?

    definirTipoConstante();
    return 0;
}