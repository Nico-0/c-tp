/*
 ============================================================================
 Name        : zCambioReferencia.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

	typedef struct
	{

		uint32_t id;
		uint32_t size_nombre;
		uint32_t posX;
		uint32_t posY;

	} t_catch;

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	t_catch* struct_1 = malloc(sizeof(t_catch));
	struct_1->id = 15;

	t_catch* struct_2 = struct_1;

	printf("El valor es: %d\n", struct_2->id);









	return EXIT_SUCCESS;
}
