/*
 ============================================================================
 Name        : zListTest.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>

void imprimir_lista(t_list* lista, char* nombre){
	int tamanio = list_size(lista);
	int i;
	void* contenido;

	for(i=0; i<tamanio; i++){
		contenido = list_get(lista, i);
		printf("%s[%d] vale: %d\n", nombre, i, contenido);
	}

}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	t_list* lista_1 = list_create();

	int i;

	list_add(lista_1, 3);
	list_add(lista_1, 4);
	list_add(lista_1, 5);

	imprimir_lista(lista_1, "Primera");

	list_add_in_index(lista_1, 1, 15);

	imprimir_lista(lista_1, "Segunda");

	return EXIT_SUCCESS;
}
