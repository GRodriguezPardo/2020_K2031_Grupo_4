#include <stdio.h>

/*
	CMD no reconoce los colores
*/

#ifndef WINCOMP
	void rojo(int bold) {
		printf("\033[%d;31m", bold != 0);
	}

	void verde(int bold) {
		printf("\033[%d;32m", bold != 0);
	}

	void amarillo(int bold) {
		printf("\033[%d;33m", bold != 0);
	}

	void azul(int bold) {
		printf("\033[%d;34m", bold != 0);
	}

	void magenta(int bold) {
		printf("\033[%d;35m", bold != 0);
	}

	void cyan(int bold) {
		printf("\033[%d;36m", bold != 0);
	}

	void blanco() {
		printf("\033[0m");
	}
#else
	void rojo(int bold) {}

	void verde(int bold) {}

	void amarillo(int bold) {}

	void azul(int bold) {}

	void magenta(int bold) {}

	void cyan(int bold) {}

	void blanco() {}
#endif