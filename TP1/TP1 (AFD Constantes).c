#include <stdio.h>   //Libreria de C base
#include <ctype.h>   //Para ahorrar el tener que poner minusculas y mayusculas (funcion toupper)
#include <string.h>  //Para la funcion de pedir ubicacion de archivos (no son hacks, no usa nada en el programa "real")
#include <windows.h> //Para limpiar la pantalla y agilizar el menu (temas esteticos del .exe)

void menu (){   //Un menu muy bonico :)

	system("cls");
    printf("ANALIZADOR DE CONSTANTES ENTERAS EN C\n\n");									//Un titulo, por que no?
	printf("1-Abrir archivo de texto con las constantes\n");
	printf("2-Crear archivo de guardado de resultados\n");
	printf("3-Comenzar el analisis\n");
	printf("0-Salir\n");
	printf("Opcion: ");
}

void opcionElegida (int opcion){     //Probando un nuevo estilo de menu, que limpia la pantalla al elegir una opcion
	system("cls");
	printf("Opcion Elegida: %i\n",opcion);
}

void continuar (){    //Una pausa nomas, detalles "esteticos"
	printf("\n\n-----------------------------------------\n");
	system("PAuSE");
}

void limpiarFlujoEntrada (){    //Para que no tome un \n como un char que tenes que introducir (y lo saltee como consecuencia)
	int nl;
	while ( nl = getchar() != '\n' );     //nl (newline) va a reasignarse hasta encontrar un \n y entonces cumplio su funcion (que no se almacene en una variable a usar)
}

void pedirubicacion (char rutaArchivo[], int abroOGuardo){  //Por si queres abrir o guardar un archivo en otra carpeta o con otro nombre
    
    char opcion;
    char opcion2;
    char nombreArchivo[100];                                //Variables que se van a necesitar
    
    limpiarFlujoEntrada();                                  //Para entrar a esta funcion puse la opcion 1 o 2 y despues Enter, con lo que en el flujo hay guardado un \n. Quiero sacarlo  >:(
    
    printf ("\nDesea ingresar el nombre del archivo? (default: \"Constantes\") (S/N) -> ");
    opcion = getc (stdin);
    opcion = toupper(opcion);                               //Evitar los casos en minuscula como paso anteriormente (mas abajo). 
    
    if (opcion=='S'){
        printf ("Nombre del Archivo: ");
        limpiarFlujoEntrada();                              //Elimino el \n  >:(
        gets(nombreArchivo);
	}
    else { 
    	if (abroOGuardo == 1){                              //Para evitar complicaciones, otro parametro que decide si es el archivo que se lee o el que se guarda
    		strcpy (nombreArchivo, "Constantes");
		}
		else {
			strcpy (nombreArchivo, "ResultadosV2");
		}
        
        limpiarFlujoEntrada();                              //Malditos \n  >:(
	}
    
    
    printf ("\nDesea ingresar la ubicacion del archivo? (default: ubicacion del .exe) (S/N) -> ");
    opcion2 = getc (stdin);
    opcion2 = toupper(opcion2);                             //Lo mismo que arriba, pero para la direccion (por ejemplo "C:/users/Fulano/Documentos". Es lo que copiarias si clickeas arriba, donde esta...
    														//... la direccion del archivo en el explorador de Windows
    if (opcion2=='S'){
        printf ("ubicacion del Archivo: ");
        limpiarFlujoEntrada();;                             //Guerra contra los  \n
        gets(rutaArchivo);
        strcat (rutaArchivo, "/");							//La barrita que separa la ubicacion del nombre (que al copiarlo del explorador de Windows no se pone)
	}
    else {
        rutaArchivo[0]='\0'; }
    
    strcat (rutaArchivo, nombreArchivo);
    strcat (rutaArchivo, ".txt");						    //Se juntan direccion y nombre y viven felices por siempre. De hijo tienen a la extension .txt  :D   Hermoso final
}

void definirTipoConstante (char rutaArchivoA[], char rutaArchivoG[]){
	
	//Automata Finito: los valores de la tabla son los estados, las columnas son las entradas y las filas son las salidas para un mismo estado:
	
	int tablaTransicion[7][6]={{2,1,1,6,6,6},                     //Primer columna: entro 0
							  {1,1,1,6,6,6},                      //Segunda columna: entro del 1 al 7
							  {3,3,6,6,4,6},                      //Tercer columna: entro 8 o 9
							  {3,3,6,6,6,6},                      //Cuarta columna: entro de la "a" a la "f"
							  {5,5,5,5,6,6},					  //Quinta columna: entro "x"
							  {5,5,5,5,6,6},                      //Sexta columna: entro un valor diferente al resto (Ejemplo, "j")
							  {6,6,6,6,6,6}};
	
	FILE* f = fopen (rutaArchivoA, "r");                          //Abro ambos archivos
	FILE* g = fopen (rutaArchivoG, "w");   
	
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

	
	while(!feof(f)){                                        			        //El analisis finaliza cuando se termina de leer el archivo
		while (c!=',' && !feof(f)){                         			        //El caracter ',' es lo que indica el fin de la constante
			constanteLeida[i]=c;                            			        //Va cargando la constante completa
			i++;
			
			switch (toupper(c)){								   			    //Para evitar los casos en minuscula (no afecta a la variable 'c')
				case '0':
					estado=tablaTransicion[estado][0];        				    //Entro un cero, corresponde a la primer columna (indice 0) de la tabla, la fila es el estado en el cual se encontraba antes
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					estado=tablaTransicion[estado][1];			                //Entro un numero del 0-7, corresponde a la segunda columna (1)
					break;
				case '8':
				case '9':
					estado=tablaTransicion[estado][2];			                //Entro un 8 o un 9, corresponde a la tercer columna (2)
					break;
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					estado=tablaTransicion[estado][3];			                //Entro una letra de la 'a' a la 'f', corresponde a la cuarta columna (3)
					break;
				case 'X':
					estado=tablaTransicion[estado][4];			                //Entro una 'x', corresponde a la quinta columna (4)
					break;
				default:
					estado=tablaTransicion[estado][5];			                //Entro un caracter no valido, corresponde a la sexta columna (5). Esta columna y su estado (5) son de errores. Se entra y no se sale
			}
			c = getc(f);                                                        //Realiza un avance dentro de la cadena en progreso
		}
		constanteLeida[i]='\0';									                //Finaliza la constante
		i=0;													                //Reinicia el indice para la siguiente cadena
		estadoFinal=estado;										                //Establece el estado final (mejor dicho, ultimo estado. Puede que no sea final)
		estado=0;												                //Reinicia el estado para la siguiente cadena
	
		switch (estadoFinal){									                //Cada estado corresponde a un tipo de constante diferente:
			case 1:
                printf ("\nLa constante %s corresponde al tipo Decimal", constanteLeida);
				fprintf (g, "La constante %s corresponde al tipo Decimal\n", constanteLeida); 
				break;
			case 2:
                printf ("\nLa constante %s corresponde al valor 0 en cualquier tipo de constante entera", constanteLeida);
				fprintf (g, "La constante %s corresponde al valor 0 en cualqueir tipo de constante entera\n", constanteLeida);
				break;
			case 3:
                printf ("\nLa constante %s corresponde al tipo Octal", constanteLeida);
				fprintf (g, "La constante %s corresponde al tipo Octal\n", constanteLeida);
				break;
			case 5:
                printf ("\nLa constante %s corresponde al tipo Hexadecimal", constanteLeida);
				fprintf (g, "La constante %s corresponde al tipo Hexadecimal\n", constanteLeida);
				break;
			case 6:
			case 4:
                printf ("\nLa constante %s no corresponde a ningun tipo de constante entera", constanteLeida);
				fprintf (g, "La constante %s no corresponde a ningun tipo de constante entera\n", constanteLeida);
				break;
		}
		c=getc(f);											      //Paso entre una cadena y otra
	}
	fclose(f);													  //Cierro ambos archivos
	fclose(g);
	return;
}

int main (){    //Pues el main, que mas?

    int opcion;
	menu();
	scanf("%i", &opcion);
	opcionElegida(opcion);
	
	char rutaArchivoA[300];
	char rutaArchivoG[300];																	//Rutas de los archivos
	
	while(opcion!=0){																		//Con el 0 finaliza el programa
	
		switch(opcion){
			case 1:
				pedirubicacion(rutaArchivoA, 1);
				printf("\nRuta Final: %s", rutaArchivoA);
				continuar();																//Una pausa, para que cuando termine la funcion no se reinicie el menu de una
				break;
			case 2:
				pedirubicacion(rutaArchivoG, 2);
				printf("\nRuta Final: %s", rutaArchivoG);
				continuar();
				break;
			case 3:
				definirTipoConstante(rutaArchivoA, rutaArchivoG);
				continuar();
				break;
			default:
				printf("\nNo ha introducido una opcion valida. Intente de nuevo.");
				continuar();
		}
		menu();
		scanf("%i", &opcion);
		opcionElegida(opcion);
	}
    return 0;
}
