typedef struct Literal_Cadena {
    char* cadena;
    int len;
    struct Literal_Cadena* sig;
} Node;

typedef struct Palabra_Reservada {
    char* cadena;
    struct Palabra_Reservada* sig;
} Node2;

typedef struct Comentario {
    char* cadena;
    int tipo; //0 una linea, 1 multilinea
    struct Comentario* sig;
} Node3;

Node2* agregarPalRes(char* palabra, Node2* head) {
    Node2* new_node;
    new_node = (Node2*) malloc(sizeof(Node2));
    new_node -> cadena = palabra;
    if(head == NULL) {
        head = new_node;
        new_node -> sig = NULL;
    } else {
        new_node -> sig = head;
        head = new_node;
    }
return head;
}

void imprimirPalRes(Node2* head,FILE* f){
	if(head==NULL){return;}
    if(head->sig != NULL) {
      imprimirPalRes(head -> sig,f);
   }
   printf(" .\t%s\n", head -> cadena);
   fprintf(f, " .\t%s\n", head -> cadena);
}


Node* agregarCadena(char* palabra, int longitud, Node *head) {
    Node* new_node;
    new_node = (Node*) malloc(sizeof(Node));
    new_node -> cadena = palabra;
    new_node -> len = longitud;
    if(head == NULL) {
        head = new_node;
        new_node -> sig = NULL;
    } else {
        new_node -> sig = head;
        head = new_node;
    }
return head;
}

void imprimirCadenas(Node* head,FILE* f){
	if(head==NULL){return;}
    if(head->sig != NULL) {
      imprimirCadenas(head -> sig,f);
   }
   printf(" .\t%-30s\t%d\n", head -> cadena, head -> len);
   fprintf(f, " .\t%-30s\t%d\n", head -> cadena, head -> len);
}

Node* buscarId(char* palabra, Node* head){
	Node* new_node = head;
	while(new_node != NULL){
		if(strcmp(new_node->cadena,palabra)){
			new_node = new_node->sig;
		}else{
			return new_node;
		}
	}
	return new_node;
}

Node3* agregarAListaComentarios(char* comentario, int categoria, Node3* head) {
    Node3* new_node;
    new_node = (Node3*) malloc(sizeof(Node3));
    new_node -> cadena = comentario;
    new_node -> tipo = categoria;
    if(head == NULL) {
        head = new_node;
        new_node -> sig = NULL;
    } else {
        new_node -> sig = head;
        head = new_node;
    }
    return head;
}

void imprimirComentarios(Node3* head, FILE* f){
    if(head==NULL){return;}
    int i;
    if(head->sig != NULL) {
      imprimirComentarios(head -> sig,f);
   }
    if(head -> tipo){
        int len = strlen(head->cadena);
        printf(" .\tMultilinea\t");
        fprintf(f, " .\tMultilinea\t");
        for(i=0;i<len;i++){
          if(head->cadena[i]=='\n'){
               printf("\n .\t\t\t");
               fprintf(f,"\n .\t\t\t");
          }else{
               printf("%c",head->cadena[i]);
               fprintf(f, "%c",head->cadena[i]);
          }
        }
        printf("\n");
        fprintf(f,"\n");
        
   } else {
        printf(" .\tUna sola linea\t%s\n", head -> cadena);
        fprintf(f, " .\tUna sola linea\t%s\n", head -> cadena);
   }
}

int lineasComentariosMultilinea(char* comentario, int longitud){
    int acum = 0;
    int i = 0;
    while(i < longitud){
        if(comentario[i] == '\n'){acum++;}
        i++;
    }
    return acum;
}

Node* agregarAListaId(char* palabra, Node* head){ //AGREGAR ORDENADO
    Node* new_node; 
    new_node = (Node*)malloc(sizeof(Node));
    Node* aux = head;
    Node* ant = NULL;
    new_node -> cadena = palabra;
    new_node -> len = 1;
    while(aux!=NULL && (strcmp(aux -> cadena, palabra) == 1)) {
        ant = aux;
        aux = aux -> sig;
    }
    if(ant==NULL){
        head = new_node;
    } else {
        ant -> sig = new_node;
    }
    new_node -> sig = aux;
    return head;
}

Node* agregarId(char* palabra, Node* head){
	Node* new_node = buscarId(palabra, head);
	if(new_node==NULL){
		head=agregarAListaId(palabra, head);
	}else{
		++new_node->len;
	}
	return head;
}

void imprimirId(Node* head,FILE* f){
	if(head==NULL){return;}
    if(head->sig != NULL) {
      imprimirId(head -> sig,f);
   }
   printf(" .\t%-10s\t%d\n", head -> cadena, head -> len);
   fprintf(f, " .\t%-10s\t%d\n", head -> cadena, head -> len);
}

Node* agregarConstante(char* palabra, int longitud, Node *head) {
    Node* new_node;
    new_node = (Node*) malloc(sizeof(Node));
    new_node -> cadena = palabra;
    new_node -> len = longitud;
    if(head == NULL) {
        head = new_node;
        new_node -> sig = NULL;
    } else {
        new_node -> sig = head;
        head = new_node;
    }
return head;
}

int hexaADec(char* cadena, int len){
	int i,j,aux,potencia;
	int acum=0;
	for(i=len,j=0;i>2;i--,j++){
		switch(cadena[i-1]){
			case '0'...'9':
				aux = cadena[i-1]-48;
				break;
			case 'A'...'F':
				aux = cadena[i-1]-55;
				break;
			case 'a'...'f':
				aux = cadena[i-1]-87;
				break;
			case 'u':
			case 'U':
			case 'l':
			case 'L':	
				aux = 0;
				j--;
				break;
		}
		aux*=pow(16,j);
		acum+=aux;
	}
	return acum;
}

int octalADec(char* cadena, int len){
    int i,j,aux;
    int acum=0;
    for(i=len,j=0;i>1;i--,j++){
        switch(cadena[i-1]){
            case '0'...'7':
                aux = cadena[i-1]-48;
                break;
            case 'u':
            case 'U':
            case 'l':
            case 'L':    
                aux = 0;
                j--;
                break;
        }
        aux *= pow(8, j);
        acum += aux;
    }
    return acum;
}

void imprimirOctal(Node* head,FILE* f){
	if(head==NULL){return;}
    if(head->sig != NULL) {
      imprimirOctal(head -> sig,f);
   }
   int decimal = octalADec(head->cadena, head->len);
   printf(" .\t%-10s\t%-10d\n", head -> cadena, decimal);
   fprintf(f, " .\t%-10s\t%-10d\n", head -> cadena, decimal);
}

void imprimirHexa(Node* head,FILE* f){
	if(head==NULL){return;}
    if(head->sig != NULL) {
      imprimirHexa(head -> sig,f);
   }
   int decimal = hexaADec(head->cadena, head->len);
   printf(" .\t%-10s\t%-10d\n", head -> cadena, decimal);
   fprintf(f, " .\t%-10s\t%-10d\n", head -> cadena, decimal);
}

Node2* agregarConstanteDec(char* palabra, Node2 *head) {
    Node2* new_node;
    new_node = (Node2*) malloc(sizeof(Node2));
    new_node -> cadena = palabra;
    if(head == NULL) {
        head = new_node;
        new_node -> sig = NULL;
    } else {
        new_node -> sig = head;
        head = new_node;
    }
return head;
}

int imprimirDec(Node2* head,FILE* f){
	int acum=0;
	if(head==NULL){return 0;}
    if(head->sig != NULL) {
      acum = imprimirDec(head -> sig,f);
   }
   printf(" .\t%d\n", atoi(head -> cadena));
   fprintf(f, " .\t%d\n", atoi(head -> cadena));
   return atoi(head->cadena)+acum;
}

void imprimirReal(Node2* head,FILE* f){
	if(head==NULL){return;}
    if(head->sig != NULL) {
      imprimirReal(head -> sig,f);
   }
   double numero = atof(head -> cadena);
   double entera,fraccional;
   fraccional = modf(numero, &entera);
   printf(" .\t%10lf\t\t%lf\n", entera, fraccional);
   fprintf(f, " .\t%10lf\t\t%lf\n", entera, fraccional);
}
