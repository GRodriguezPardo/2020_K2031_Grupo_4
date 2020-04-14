#include <stdio.h>   //Libreria de C base
#include <ctype.h>   //Para ahorrar el tener que poner minusculas y mayusculas (funcion toupper)
#include <string.h>  //Para la funcion de pedir ubicacion de archivos (no son hacks, no usa nada en el programa "real")
#include <windows.h> //Para limpiar la pantalla y agilizar el menu (temas esteticos del .exe)

//Sugiero leer desde el main hacia arriba, yendo a las funciones que se llamen en su orden

void menu (){   //Un menu muy bonico :)

	system("cls");
    printf("ANALIZADOR DE CONSTANTES ENTERAS EN C\n\n");									//Un titulo, por que no?
	printf("1-Abrir archivo de texto con las constantes\n");
	printf("2-Crear archivo de guardado de resultados\n");
	printf("3-Comenzar el analisis\n");
	printf("4-Ver esquema del automata finito a utilizar\n");
	printf("5-Analisis paso a paso\n");
	printf("0-Salir\n");
	printf("Opcion: ");
}

void opcionElegida (int opcion){     //Probando un nuevo estilo de menu, que limpia la pantalla al elegir una opcion
	system("cls");
	printf("Opcion Elegida: %i\n",opcion);
}

void sinComentarios (int a, char b[], int c, int d, int e[][6]){       //Para cuando se elige el analisis normalito (opcion 3) :)
	return;                                                         
}

void imprimirTabla (int estado, int columna, int tt[][6]){       //Imprime la tabla de transicion. Los parametros son para resaltar el estado actual en el analisis paso a paso

    int i=0;
    int j=0;
    printf("\n      (0)  (1-7) (8-9) (A-F)  (x)  (Otro)\n");
    for (i=0; i<7; i++){
        printf(" Q%i | ",i);
        for(j=0; j<6; j++){
            if(i==estado && j==columna-1){
                printf("(%i)   ", tt[i][j]);
             }
             else{
                 printf(" %i    ", tt[i][j]);
             }
         }
     printf("\n");
     }
     printf("\n");
}

void activarComentarios (int c, char constanteLeida[], int estado, int columna, int tt[][6]){    //Muestra la constante leida, el estado actual y la tabla de transicion resaltando el nuevo estado segun lo leido
	
	//No asustarse, son puros printf (salidas por pantalla)
	printf("\n\n");
	system("PAUSE");
	system("cls");
	printf("Constante leida por ahora: %s\n", constanteLeida);                                                 //Va imprimiendo la constante total
	
	if (c!=',' && c!='|'){
		printf("\nSe leyo '%c', lo cual corresponde a la columna %i de la tabla",c,columna);                   //Indica el ultimo caracter, y sus consecuencias
		printf("\nEl estado anterior era %i (designa la fila)", estado);
		printf("\nEntonces el nuevo estado es: %i  (Resaltado entre parentesis)\n\n", tt[estado][columna-1]);
        imprimirTabla (estado, columna, tt);
	}
	else {
		if(c=='|'){  printf("\nSe llego al final de archivo, con lo cual la constante finalizo");  }
		else{ 	  	 printf("\nSe leyo '%c', con lo cual la constante finalizo",c); }
		printf("\nEstado final: %i", estado);
		printf("\n\nConclusion:  ");
	}
}

void continuar (){    //Una pausa nomas, detalles "esteticos" (para que el abominable clear_screen no domine al mundo evitando que se lea lo que dice en pantalla)
	printf("\n\n-----------------------------------------\n");
	system("PAuSE");
}

void esquemaAFD(){        //Aprendiendo a dibujar en C  :O
	printf("\n _____________________________________________________\n");
	printf("|                                                     |         Imaginar que los estados 1,3,5 y 6 tiene bucle\n");
	printf("|   Q0----------------->  Q1+                         |         Tambien imaginar que hay una arista\n");
	printf("|      \'-------.                                      |         desde todos los estados hasta Q6 (con valor \"Otro\")\n");
	printf("|                \'-----.                              |\n");
	printf("|                         Q2+ ----------------> Q3+   |         Aristas:\n");
	printf("|                            \'-------.                |         Q0-Q1: 1 al 9        Bucle Q1: 0 al 9\n");
	printf("|                                     \'-----.         |         Q0-Q2: 0             Bucle Q3: 0 al 7\n");
	printf("|   Q6+                   Q5+ <---------------- Q4    |         Q2-Q3: 0 al 7        Bucle Q5: 0 al 9 y A a F     \n");
	printf("|_____________________________________________________|         Q2-Q4: x o X         Bucle Q6: Cualquier otro caracter\n\n");
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

void definirTipoConstante (char rutaArchivoA[], char rutaArchivoG[], void (*comentarios)(int, char[], int, int, int[][6])){
	
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
					constanteLeida[i]='\0';                                     //Cierra momentaneamente la constante para poder imprimirla correctamente
					comentarios (c,constanteLeida,estado,1,tablaTransicion);    //Si es el analisis completo, muestra lo que hace el programa
					estado=tablaTransicion[estado][0];        				    //Entro un cero, corresponde a la primer columna (indice 0) de la tabla, la fila es el estado en el cual se encontraba antes
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					constanteLeida[i]='\0';                                     //Lo mismo que en case '0'  (y para el resto)
					comentarios (c,constanteLeida,estado,2,tablaTransicion);
					estado=tablaTransicion[estado][1];			                //Entro un numero del 0-7, corresponde a la segunda columna (1)
					break;
				case '8':
				case '9':
					constanteLeida[i]='\0';
					comentarios (c,constanteLeida,estado,3,tablaTransicion);
					estado=tablaTransicion[estado][2];			                //Entro un 8 o un 9, corresponde a la tercer columna (2)
					break;
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					constanteLeida[i]='\0';
					comentarios (c,constanteLeida,estado,4,tablaTransicion);
					estado=tablaTransicion[estado][3];			                //Entro una letra de la 'a' a la 'f', corresponde a la cuarta columna (3)
					break;
				case 'X':
					constanteLeida[i]='\0';
					comentarios (c,constanteLeida,estado,5,tablaTransicion);
					estado=tablaTransicion[estado][4];			                //Entro una 'x', corresponde a la quinta columna (4)
					break;
				default:
					constanteLeida[i]='\0';
					comentarios (c,constanteLeida,estado,6,tablaTransicion);
					estado=tablaTransicion[estado][5];			                //Entro un caracter no valido, corresponde a la sexta columna (5). Esta columna y su estado (5) son de errores. Se entra y no se sale
			}
			c = getc(f);                                                        //Realiza un avance dentro de la cadena en progreso
		}
		if(feof(f)){c='|';}													    //Para que el comentario de la ultima constante lo haga correctamente
		comentarios (c,constanteLeida,estado,6,tablaTransicion);
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

void analisisPasos (char rutaArchivoA[], char rutaArchivoG[]){		//Veamos que sale de esto (un intento de ver el progreso dentro del AFD)
	
	//Tenia ganas de poner un porcentaje de carga, estoy probando cosas con el clear screen :)
	int i = 0;
	char p = '%';
	
	for (i=0; i<101;i++){                                                   //Basicamente, incrementa 'i', lo imprime con un porcentaje al lado y reinicia la pantalla 
		system("cls");
		printf("\nBIENVENIDO AL ANALISIS PASO A PASO!\n\n");
		printf("Cargando...");
		printf("%i %c \n                                                                             (Mentira, pero asi es mas divertido)\n", i,p);
		Sleep(1);
		
	}
	
	printf("Cargado EXITOSO!!\n");
	system("PAUSE");
	system("cls");
	//Fin del porcentaje de carga falso
	
	printf("Bueno comenzamos primero con un grafico del automata finito que vamos a utilizar:\n");
	esquemaAFD();
	system("PAUSE");
	printf("\nAhora, presentamos la grandiosa tabla de transicion de dicho automata.\n\n");
	
	int tablaTransicion[7][6]={{2,1,1,6,6,6},                     //Primer columna: entro 0
							  {1,1,1,6,6,6},                      //Segunda columna: entro del 1 al 7
							  {3,3,6,4,6,6},                      //Tercer columna: entro 8 o 9
							  {3,3,6,6,6,6},                      //Cuarta columna: entro de la "a" a la "f"
							  {5,5,5,6,5,6},                      //Quinta columna: entro "x"
							  {5,5,5,6,5,6},                      //Sexta columna: entro  un valor diferente al resto (Ejemplo, "j")
							  {6,6,6,6,6,6}};
							  

	imprimirTabla (10,10,tablaTransicion);                        //Imprime la tabla. 10 y 10 para que no resalte ningun elemento (se ve mas arriba)
	system("PAUSE");
	system ("cls");

	printf("Comenzamos con el analisis\n");
	
	definirTipoConstante (rutaArchivoA, rutaArchivoG, activarComentarios);						//Llamo a la funcion de analisis con los comentarios activados
	
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
				definirTipoConstante(rutaArchivoA, rutaArchivoG, sinComentarios);
				continuar();
				break;
			case 4:
				esquemaAFD();                                                               
				continuar();
				break;
			case 5:
				analisisPasos (rutaArchivoA, rutaArchivoG);                                 //Imprime AFD, Tabla, y detalla lo que hace el programa por pantalla. Estaba aburrido, ok?
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
