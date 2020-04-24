#include <stdio.h>
#include <stdbool.h>

int main (){
	
	FILE* fr = fopen("entradas.txt", "r");
	FILE* fw = fopen("salidas.txt", "w");
	char car;
	bool valido;
	
	int estado = 0;
	int col;
	int tt[6][6] = {{1,5,5,-1,-1,-1},{2,2,-1,-1,3,-1},{2,2,-1,-1,-1,-1},{4,4,4,4,-1,-1},{4,4,4,4,-1,-1},{5,5,5,-1,-1,-1}};
	
	car = fgetc(fr);
	while (!feof(fr)){
		valido = 1;
		estado=0;
		while (car!=','&&(!feof(fr))){
			switch (car){
				case '0':
					col = 0;
					break;
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					col = 1;
					break;
				case '8':
				case '9':
					col = 2;
					break;
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
					col = 3;
					break;
				case 'x':
				case 'X':
					col = 4;
					break;
				default:
					col = 5;
					break;
			}
			if (estado!=(-1)){estado = tt[estado][col];}
			if(estado==(-1)){valido=0;}
			car = fgetc(fr);
		}
		if (estado==0||estado==3){valido=0;}
		if (valido){
			if (estado==1||estado==2){fputs("OCTAL, ",fw);}
			if (estado==4){fputs("HEXA, ",fw);}
			if (estado==5){fputs("DECIMAL, ",fw);}
		}else{
			fputs("NO RECONOCIDO, ",fw);
		}
		car = fgetc(fr);
	}
	fclose (fr);
	fclose (fw);
}
