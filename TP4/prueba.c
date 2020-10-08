int main() {
    

    char estadoActual = '0'; //q0, q1, q2

    struct nodoPila* pila = (struct nodoPila*) malloc(sizeof(struct nodoPila));
    pila->elem = '$';
    pila-> siguiente = NULL;

    printf("Ingrese una operacion a analizar (o 'salida' para terminar)\n> ");
    scanf ("%[^\n]%*c", ingreso); //Ingreso del usuario
    iniciarAFP(transiciones, estados, indicesEstado);

    //do {
    //    analizarOperacion(ingreso, &estadoActual, transiciones, estados, indicesEstado, &pila);
    //    printf("Ingrese una operacion a analizar (o 'salida' para terminar)\n> ");
    //    scanf ("%[^\n]%*c", ingreso); //Ingreso del usuario
    //} while(strcmp(ingreso, "salida") != 0);

    while(strcmp(ingreso, "salida") != 0) {
        analizarOperacion(ingreso, &estadoActual, transiciones, estados, indicesEstado, &pila);
	printf("Ingrese una operacion a analizar (o 'salida' para terminar)\n>");
	scanf("%[^\n]%*c" ,ingreso);
    }

    return 0;
}

void limpiarAFP(struct nodoPila** pila, char* estadoActual) {
    *estadoActual = '0';

    while((*pila)->elem != '$') { // Detectamos error asi que limpiamos la pila
        struct nodoPila* aux = *pila;
        *pila = (*pila)->siguiente;
        free(aux);
    }
}

void analizarOperacion(char ingreso[], char* estadoActual, char* transiciones[], char* estados[][6], char* indicesEstado[], struct nodoPila** pila) {
    int i = 0;
    int error = 0; //flag

    while(ingreso[i] != '\0' && !error) { //Analizamos cada letra del ingreso
        if(ingreso[i] != ' ') {
            char cimaPila = pop(pila);
            int accesoEstados = obtenerIndice(*estadoActual, cimaPila, indicesEstado); //Indice al acceso de indicesEstado
            error = analizarCaracter(ingreso[i], transiciones, accesoEstados, estadoActual, pila, estados);
        }
        i++;
    }

    if(error || (*estadoActual != '1' && *estadoActual != '2')) {
        printf("\nOperacion incorrecta\n\n");

        printf("%s\n", ingreso);
        for(int j = 0; j < i; j++) 
            if(j == i-1)
                printf("^\n\n");
            else
                printf(" ");
            
        printf("Caracter no reconocido o faltante\n\n");
    } else if(pop(pila) != '$') {
        printf("\nOperacion incorrecta\n\n");
        printf("%s\n", ingreso);
        printf("\nParentesis faltante\n\n");
        
    } else
        printf("\nOperacion correcta\n\n");

    limpiarAFP(pila, estadoActual);
}

int analizarCaracter(char c, char* transiciones[], int accesoEstados, char* estadoActual, struct nodoPila** pila, char* estados[][6]) {
    int i = 0;
    int j = 0;

    for(int i = 0; i < 5; i++) {
        if(estados[i][accesoEstados] != "--") { //Si es la transicion nula no evaluamos
            int j = 0;

            while(transiciones[i][j] != '\0' && transiciones[i][j] != c) j++; //Leemos caracter a caracter hasta encontrar una coincidencia

            if(transiciones[i][j] == c) {
            	int k = 1;
                *estadoActual = estados[i][accesoEstados][0];               

                while(estados[i][accesoEstados][k] != '\0') { //En algunos casos hay que meter mas de 1 elemento a la pila, por eso el ciclo
                    push(pila, estados[i][accesoEstados][k]);
                    k++;
                }
                return 0;
            }
        }
    }

    return 1;
}

int obtenerIndice(char estadoActual, char cimaPila, char* indicesEstado[]) {
    int i = 0;

    while(i < 6 && ((indicesEstado[i][0]) != estadoActual || indicesEstado[i][1] != cimaPila)) i++;

    return i;
}

void iniciarAFP(char* transiciones[5], char* estados[5][6], char* indicesEstado[]) {
    transiciones[0] = "0";
    transiciones[1] = "123456789";
    transiciones[2] = "+-*/";
    transiciones[3] = "(";
    transiciones[4] = ")";

    indicesEstado[0] = "0$";
    indicesEstado[1] = "1$";
    indicesEstado[2] = "0R";
    indicesEstado[3] = "1R";
    indicesEstado[4] = "2R";
    indicesEstado[5] = "2$";

    char* rechazo = "--";

    // Configuramos todos los estados de rechazo
    estados[0][0] = estados[2][0] = estados[4][0] = rechazo;
    estados[3][1] = estados[4][1] = rechazo;
    estados[0][2] = estados[2][2] = estados[4][2] = rechazo;
    estados[3][3] = rechazo;
    estados[0][4] = estados[1][4] = estados[3][4] = rechazo;
    estados[0][5] = estados[1][5] = estados[3][5] = estados[4][5] = rechazo;

    //Configuramos el resto
    //e representa epsilon

    estados[1][0] = estados[0][1] = estados[1][1] = "1$";
    estados[3][0] = "0R$";
    estados[2][1] = estados[2][5] = "0$";
    estados[1][2] = estados[0][3] = estados[1][3] = "1R";
    estados[3][2] = "0RR";
    estados[2][3] = estados[2][4] = "0R";
    estados[4][3] = estados[4][4] = "2e";
}

char pop(struct nodoPila** pila) {
    if((*pila)->elem == '$')
        return '$';
    else {
        struct nodoPila* aux = *pila;
        *pila = (*pila)->siguiente;
        char ret = aux->elem;
        free(aux); //Importante liberar memoria del nodo

        return ret;
    }
}

void push(struct nodoPila** pila, char nuevoElem) {
    if(nuevoElem != '$' && nuevoElem != 'e') {
        struct nodoPila* nuevoNodo = (struct nodoPila*) malloc(sizeof(struct nodoPila)); //Creamos el nuevo nodo con malloc para poder liberarlo
        nuevoNodo->elem = nuevoElem;
        nuevoNodo->siguiente = *pila;
        *pila = nuevoNodo; 
    }
}