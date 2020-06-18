/*
 ============================================================================
 Name        : zPrintArchivo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/txt.h>
#include <commons/string.h>

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	//meter datos void*
	//dividir a la mitad dando la direccion de la mitad
	//imprimir en archivo la direccion principio, mitad, y fin.

	void* temp = malloc(21);
	strcpy(temp, "12345678901234567890");
	printf("temp: %s\n", (char*) temp);

	void* datos = malloc(21);
	memcpy(datos, temp, 19);
	char letra = 'p';
	char fin = '\0';
	memcpy(datos + 19, &letra, 1);
	memcpy(datos + 20, &fin, 1);
	printf("datos: %s\n", (char*) datos);

	void* medio = datos + 10;

	//agrego un fin a la mitad
	memcpy(medio, &letra, 1);
	printf("datos: %s\n", (char*) datos);

	FILE* archivo;
	archivo = txt_open_for_append("hola.txt");
	txt_write_in_file(archivo, (char*) datos);
	txt_write_in_file(archivo, "\n");
	char* prueba = string_from_format("%p", datos);
	txt_write_in_file(archivo, prueba);
	free(prueba);

	txt_write_in_file(archivo, "\n");
	txt_close_file(archivo);
	free(temp);
	free(datos);
	return EXIT_SUCCESS;
}
