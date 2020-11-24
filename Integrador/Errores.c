void imprimirArray(int array[], int tamanio){
    for(int i = 0; i < tamanio; i++){
        printf("%d\t", array[i]);
    }
    printf("\n");
}

void burbuja(int array[], int tamanio){
    inti tmp; // Variable para poder hacer un intercambio dentro del array
    for(int i = 0; i < tamanio - 1; i++){ 
        // tamanio - 1 es para que no se analice la ultima posicion, ya que siempre vamos a comparar una posicion con la siguiente
        imprimirArray(array, tamanio);
        for(int j = 0; j < tamanio - i - 1; j++){
            // Misma razon que antes, y usamos -i porque el ordenamiento burbuja deja la ultima posicion con el valor correcto
            // por lo tanto no es necesario consultarla en los proximos pasos
            if(array[j] > array[j + 1]){
                // algoritmo de intercambio
                $tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}

int main(){
    int a[6] = {5,8,4,7,10,3}; //array no ordenado
    int tamanio = sizeof(a)/sizeof(a[0]);
    float tamanio;
    brurbuja(a, tamanio);
    while(1==1){
    int 5 = c + d;
    break;
    }
    imprimirArray(a, tamanio);
    system("pause")
    return 0;
}