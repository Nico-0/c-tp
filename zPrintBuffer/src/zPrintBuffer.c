/*
 ============================================================================
 Name        : zPrintBuffer.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdint.h>
#include <commons/txt.h>

void* string_to_void(char* string, uint32_t* size);
char* void_to_string(void* buffer, uint32_t size);
void* generate_void(uint32_t size);

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	//guardar string a void*, pero sin el \0

	char* string = "12345";
	int tamano = strlen(string);
	printf("tamano del string: %d\n", tamano);
	void* buffer = malloc(tamano);
	memcpy(buffer, string, tamano);
	printf("Guardado el string sin /0 en el buffer\n", tamano);
	//printf("Si imprimo el buffer da: %s (puede dar bien porque aparecen /0 random, pero valgrind chilla)\n", buffer);


	//convertir void* a string

	char* mensaje = malloc(tamano + 1);
	memcpy(mensaje, buffer, tamano);
	printf("Copio un caracter random al final del string, valgrind chilla?\n");
	char caracter = 65;
	memcpy(mensaje + tamano, &caracter, 1);
	//printf("Imprimo el mensaje, da: %s. (si, valgrind chilla)\n", mensaje);
	printf("Reemplazo el caracter por fin de string, valgrind no chilla\n");
	caracter = '\0';
	memcpy(mensaje + tamano, &caracter, 1);
	printf("Imprimo el mensaje, da: %s. Valgrind ya no chilla\n", mensaje);

	free(buffer);
	free(mensaje);

	//generar char random de tamano variable.
	//generar char random

	char nombre[20];
	nombre[0] = 65;
	nombre[1] = 66;
	nombre[2] = 67;
	nombre[3] = 68;
//mayusculas	65-90
//numeros		30-39
//minusculas	97-122



	//conseguir bytes random de dev/random sin que valgrind chille

	uint32_t count = 10000;

	void* data = generate_void(count);

	char* resultado = void_to_string(data, count);
	free(data);
	printf("Me quedo: ");
	printf("%s", resultado);
	printf("\n");

	FILE* file_dump = txt_open_for_append("dump.txt");
	txt_write_in_file(file_dump, resultado);
	txt_close_file(file_dump);

	free(resultado);

	return EXIT_SUCCESS;
}


void* string_to_void(char* string, uint32_t* size){

	*size = strlen(string);		//afuera de la funcion voy a necesitar saber el tamano del buffer
	void* buffer = malloc(*size);
	memcpy(buffer, string, *size);
	//falta hacer fuera de la funcion el free del string
	return buffer;
}

char* void_to_string(void* buffer, uint32_t size){
						//necesito el size del buffer para crear el string
	char* string = malloc(size + 1);
	memcpy(string, buffer, size);
	char caracter = '\0';
	memcpy(string + size, &caracter, 1);
	//falta hacer fuera de la funcion el free del buffer
	return string;
}

void* generate_void(uint32_t size){

	void* data = malloc(size);
	FILE* random;
	random = fopen("/dev/urandom", "r");
	fread(data, 1, size, random);
	fclose(random);

	return data;
}
