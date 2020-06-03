#include <stdio.h>   //Libreria de C base 
#include <string.h>  //Para el ingreso de la expresion (funcion gets)
#include <windows.h> //Para limpiar la pantalla y agilizar el menu (temas estaticos del .exe)
#include <stdlib.h>  //Funcion malloc (para la creacion de nodos de la pila)

void menu (){     //Un menu muy bonico :)
	system("cls");
    printf("ANALIZADOR DE EXPRESIONES ARITMETICAS C\n\n");
	printf("1-Ingresar la expresion a analizar\n");
    printf("2-Mostrar la expresion a analizar\n");
	printf("3-Analizar la expresion ingresada\n");
	printf("0-Salir\n");
	printf("Opcion: ");
}

void opcionElegida (int opcion){     //Limpia la pantalla al elegir una opcion y que sea mas dinamico y facil de seguir
	system("cls");					 //"Clean Screen", reinicia la pantalla del cmd
	printf("Opcion Elegida: %i\n",opcion);
}

void continuar (){    //Una pausa para que la limpieza de pantalla no suceda antes de lo querido
	printf("\n\n-----------------------------------------\n");
	system("PAUSE");	//Pausa el programa hasta que toques cualquier tecla
}


typedef struct Status{    //Definicion de la estructura Status (cada valor de la tabla de la transicion)
    int siguienteEstado;  //Nomero del nuevo estado que toma el AFP
    char pushPila[3];	  //Caracteres que tiene que pushear a la pila asociada al AFP
} Status;

typedef struct Nodo {	  //Definicion de la estuctura Nodo para la pila
	char elem;			  //La pila contiene caracteres ('R' o '$')
	struct Nodo* sig;	  //El puntero al siguiente nodo
} Nodo ;

Status tablaTransicion[2][4][6]= { 
									{
										{ {3,'$'}, {1,'$'}, {3,'$'}, {0,'$','R'}, {3,'$'}, {3,'$'} },		//Esta primer matriz corresponde cuando en la pila hay un '$'
										{ {1,'$'}, {1,'$'}, {0,'$'}, {3  , '\0'}, {3,'$'}, {3,'$'} },		//Las filas son los estados, las columnas las entradas
										{ {3,'$'}, {3,'$'}, {0,'$'}, {3  , '\0'}, {3,'$'}, {3,'$'} },		//Ver tabla del AFP para entender mejor
										{ {3,'$'}, {3,'$'}, {3,'$'}, {3  , '\0'}, {3,'$'}, {3,'$'} },
									},
									{
										{ {3,'R'}, {1,'R'}, {3,'R'}, {0,'R','R'}, {3,'R' }, {3,'R'} },		//Esta segunda matriz corresponde cuando en la pila hay 'R'
										{ {1,'R'}, {1,'R'}, {0,'R'}, {3  , '\0'}, {2,'\0'}, {3,'R'} },
										{ {3,'R'}, {3,'R'}, {0,'R'}, {3  , '\0'}, {2,'\0'}, {3,'R'} },
										{ {3,'R'}, {3,'R'}, {3,'R'}, {3  , '\0'}, {3,'R' }, {3,'R'} },
									}
};

void push (Nodo** pila, char valor){   //El topico push para pilas, el doble asterisco es por tema de que en C no hay parametos por referencia en si. Puntero a puntero.
	Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));    //C es distinto a C++, esto seria el equivalente a "new Nodo"
	nuevo->elem = valor;
	nuevo->sig = *pila;
	*pila = nuevo;
}

char pop (Nodo** pila){		//El topico pop para pilas
	Nodo* aux = *pila;
	char valor = aux->elem;
	*pila = aux->sig;
	free (aux);       		//En C no existe la funcion "delete", en vez de eso esta "free"
	return valor;
}

char* ingresarExpresion(char* exp){
	
	fflush (stdin);									//Variables de control
	int tamanio = 1;
	int i = 0;
	char control = '\0';
	
	exp = (char*)malloc(tamanio);							//Reserva de memoria inicial para la expresion
	
	printf("\nIngrese expresion para analizar...\n");
	
	while (control != EOF && control != '\n'){				//Ingreso y asignacion de memoria dinamico de la expresion
		control = getc(stdin);
		if(i >= tamanio){
			exp = realloc(exp,++tamanio);
		}
		exp[i++] = control;
	}
	
	exp[--i] = '\0';										//Caracter nulo al final de la cadena (caracter de fin de cadena)
	
	return exp;
}

void mostrarExpresion (char* expresion){    //Muestra por pantalla la expresion ingresada (o su equivalente sin espacios innecesarios si ya se analiza)
    printf("\nExpresion ingresada: %s", expresion);
}

void pushearCaracteres (Nodo** pila, char caracAPushear[]){		//Pushea a la pila los caracteres indicados por la Tabla de Transicion
    int i=0;
    while (caracAPushear[i]!='\0'){			//Pushea uno por uno los caracteres que debe
        push (pila, caracAPushear[i]);
        i++;
    }
}

void vaciarPila (Nodo** pila) {    //Libera el espacio utilizado por la pila. Se realiza al final de cada analisis de expresion
	while( *pila != NULL ) {
		pop (pila);
	}
}

char* indicadorError (int indiceError, int len){
	char* indicadorDeError = (char *)malloc((len+3)*sizeof(char));
	int i = 1;
	for (i; i < len+1; i++){
		indicadorDeError[i] = '_';
	}
	indicadorDeError[0] = '(';
	indicadorDeError[len+1] = ')';
	indicadorDeError[len+2] = '\0';
	indicadorDeError[indiceError+1] = '^';
	return indicadorDeError;
}

void imprimirError (char* expresion, int indiceError, int tipoError){    //Indica donde ocurrio el error (caracter que lo origini y su posicion en la expresion)
	int len = strlen(expresion);
	printf ("\nLa expresion ingresada no fue valida\n");

	switch (tipoError){
		case 0:
			if (expresion == ""){ printf ("La expresion fue nula");}
			else                { printf ("Expresion incompleta" );}
			break;
		case 1:
			printf ("Quedaron parentesis pendientes");
			break;
		case 2:
			printf ("\nAusencia de operacion entre numeros");
			break;
		case 3:
			printf("El error ocurrio en el caracter '%c', posicion %d de: %s", expresion[indiceError], indiceError+1, expresion);
			printf("\n                                                   %s", indicadorError (indiceError, len));
			break;
		case 4:
			printf("El error ocurrio en el caracter '%c', posicion %d de: %s", expresion[indiceError], indiceError+1, expresion);
			printf("\n                                                   %s", indicadorError (indiceError, len));
	}
}

void quitarEspacios (char* expresion){     //Elimina los espacios de la expresion, exceptuando aquellos que si se quitan salvan un error sintactico.

	int len = strlen (expresion);       	//Longitud de la expresion
	char expresionAux[100]="";				//Expresion donde se va a ir cargando la resultante, sin espacios
	int saqueUnEspacio = 0;					//Funciona como bandera, indica si en el paso previo saque un espacio (1) o no (0)
	int i=0;								//Indice de array para la expresion original
	int k=0;								//Indice de array para la expresion auxiliar
	
	for (i; i<len; i++){					//Recorre la expresion original
		if (expresion[i] != ' '){			//Si encuentra algo distinto a un parantesis, tiene las siguientes opciones: (mas abajo)

			if (saqueUnEspacio == 1 && isdigit(expresion[i]) && isdigit(expresionAux[k-1])){
				expresionAux[k] = ' ';
				k++;						//Si habia sacado un espacio, leyo un caracter numerico y lo ultimo guardado en el auxiliar era tambien un numero...
			}		
									//... ese espacio origina un error sintactico, asi que no lo quito para que en el analisis salte error.
			expresionAux[k]=expresion[i];	//Cargo el caracter leido en la expresion auxiliar
			saqueUnEspacio = 0;				//Reseteo el "bool" 
		}
		else {
			saqueUnEspacio = 1;				//Se leyo un espacio, lo indico en la bandera
			k--;							//Ya que no introduzco nada a la expresion auxiliar, contrarresto su crecimiento
		}
		k++;								//Aumenta el indice del auxiliar
	}
	strcpy (expresion, expresionAux);
}

void avanzarDeEstado (Status* nuevoEstado, int* estado, Nodo** pila, int cimaPilaTT, int entrada) {  //Realizo los cambios a las variables relacionadas con los estados y su respectiva accion
	*nuevoEstado   = tablaTransicion[cimaPilaTT][*estado][entrada];		//Guardo el estado correspondiente, que depende de cimaPila, el estado anterior y la entrada
    *estado        = (*nuevoEstado).siguienteEstado;					//Asigno la parte numerica del estado
    pushearCaracteres (pila, (*nuevoEstado).pushPila);    				//Realizo la accion que indica el nuevoEstado
}

void analizarExpresion (char* expresion){
	
    Nodo* pila = NULL;															//Inicializo la pila
    push (&pila, '$');															//Pusheo el simbolo que indica que esta vacia

	int i=0;                                                   				    //Para avanzar por la expresion                                       	    
	int estado=0;                                        			       		//El estado 0 es el incial, el valor va a ir cambiando a medida que se leen los caracteres
	char cimaPila;																//Se guadaran aqui los pop que se hagan sobre la pila para chequear el estado
	int cimaPilaTT;																//Dependiendo de cimaPila, cimaPilaTT sera "0" o "1" (es el indice para la matriz)
    Status nuevoEstado = {0,'$'};												//Se cargara el nuevo estado, tanto numero como accion a realizar
	Status estadoFinal;															//Para cuando termine el analisis (por temas de prolijidad existe, no hace falta)
	int error = 0;																//Un flag para cortar el analisis ante el primer error encontrado

	quitarEspacios (expresion);													//Quito los espacios innecesarios y "a salvo"
	
	while(expresion[i]!='\0' && error!=1){                                      //El analisis finaliza cuando se termina de leer la expresion o surge un error

            cimaPila = pop (&pila);												//Me fijo que hay en la cima de la pila
			if (cimaPila == '$'){ cimaPilaTT = 0;}								//Asigno su respectivo valor, para poder redirigirla a la matriz correspondiente
			else 				{ cimaPilaTT = 1;}

			switch (expresion[i]){								   			    //Segun que se leyo, ira a una columna u otra de la matriz
				case '0':
					avanzarDeEstado (&nuevoEstado, &estado, &pila, cimaPilaTT, 0);
					break;
				case '1' ... '9':
					avanzarDeEstado (&nuevoEstado, &estado, &pila, cimaPilaTT, 1);
					break;
				case '+':
				case '-':
				case '*':
				case '/':
					avanzarDeEstado (&nuevoEstado, &estado, &pila, cimaPilaTT, 2);
					break;
				case '(':
					avanzarDeEstado (&nuevoEstado, &estado, &pila, cimaPilaTT, 3);
                    break;
                case ')':
					avanzarDeEstado (&nuevoEstado, &estado, &pila, cimaPilaTT, 4);
					break;
				default:
					avanzarDeEstado (&nuevoEstado, &estado, &pila, cimaPilaTT, 5);
			}
				
			if (estado == 3){error=1;}											//Si entro al estado de rechazo, actualiza la flag de error
			i++;																//Realiza un avance dentro de la expresion
		}

		estadoFinal = nuevoEstado;										        //Establece el estado final (mejor dicho, ultimo estado. Puede que no sea final teoricamente)
	
		switch (estadoFinal.siguienteEstado){
			case 0:
				imprimirError (expresion, i-1, 0);
				break;
			case 1:																//Si termino en el estado q1 o q2 sin parentesis pendientes, es valida la expresion
			case 2:
                if (pop(&pila)=='$'){printf ("\nLa expresion ingresada fue valida");}
                else                {imprimirError (expresion, i-1,1);}			//Si quedaron parentesis pendientes, no es valida
				break;
			case 3:																//Cualquier otra opcion tampoco es valida
				if (expresion[i-1]==' ') { imprimirError (expresion, i-1,2);}
				else if (pop(&pila)=='$'){ imprimirError (expresion, i-1,3);}
				else				  	 { imprimirError (expresion, i-1,4);}
		}
    
	vaciarPila (&pila);															//Vacio la pila
	return;
}

int main (){    //Pues el main, que mas?

    int opcion;
	menu();
	scanf("%i", &opcion);
	opcionElegida(opcion);
	char* expresion=NULL;

	while(opcion!=0){																//Con el 0 finaliza el programa
		switch(opcion){
			case 1:
				expresion = ingresarExpresion (expresion);
				continuar();												        //Una pausa, para que cuando termine la funcion no se reinicie el menu de una
				break;
            case 2: 
                mostrarExpresion (expresion);
                continuar();
                break;
			case 3:
				analizarExpresion (expresion);
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
