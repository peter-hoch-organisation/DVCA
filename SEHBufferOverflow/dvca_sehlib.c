#include <stdio.h>
#include "dvca_sehlib.h"

#define VERSION "1.00"

void LoadLib() {
	printf("[LIB] lib v%s is loaded\n", VERSION);
}

void Draw() {
	__asm{  jmp esp
			jmp eax
			pop eax
			pop eax
			ret};
}