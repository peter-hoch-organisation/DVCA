#include <stdio.h>

#define VERSION "1.00"

void LoadLib() {
	printf("[LIB] lib v%s is loaded\n", VERSION);
}

void Draw() {
	__asm__("jmp *%esp\n\t"
			"jmp *%eax\n\t"
			"pop %eax\n\t"
			"pop %eax\n\t"
			"ret");
}

void Process(char *input) {
	char Buffer[1000];
	strcpy(Buffer, input);
}