/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

void* string_to_void(char* string, uint32_t* size);
char* void_to_string(void* buffer, uint32_t size);
void* generate_void(uint32_t size);
char* generate_repeat(int tamano, int* size);
void recibir_loggear(int socket_servidor);

int main(int argc, char* argv[])
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int socket_servidor;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	//Loggear "soy un log"
	log_info(logger, "soy un log");

	config = leer_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger, "ip del archivo: %s", ip);
	log_info(logger, "puerto del archivo: %s", puerto);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	socket_servidor = crear_conexion(ip, puerto);
	log_info(logger, "conexion creada\n");

	//enviar mensaje
	if(argc == 1){
		printf("se va a enviar mensaje por default\n");
		char* mensaje = "Te envio este mensaje pipi\n";
		enviar_mensaje(mensaje, socket_servidor, strlen(mensaje) + 1);
	}
	else if(argc == 2){
		if(strcmp(argv[1], "loop") == 0){
			int i = 1;
			while(1){

				printf("------%d----------se va a enviar en loop un mensaje: %d\n", i, i);

				char* mensaje = "Prueba total de conexion 2.0\n";
				enviar_mensaje(mensaje, socket_servidor, strlen(mensaje) + 1);


				recibir_loggear(socket_servidor);
				i++;
				sleep(5);
				socket_servidor = crear_conexion(ip, puerto);
			}
		}
		else{
			int tamano = atoi(argv[1]);
			printf("se va a enviar un mensaje repetido la cantidad de veces: %d\n", tamano);

			int* string_size = malloc(sizeof(int));
			char* mensaje = generate_repeat(tamano, string_size);

			printf("pre envio, string %d bytes\n", *string_size);
			enviar_mensaje(mensaje, socket_servidor, *string_size + 1);
			printf("Se envio el mensaje: %s\n", mensaje);
			free(mensaje);
		}
	}
	else if(argc == 3){
		if(strcmp(argv[1], "rand") == 0){
			int tamano = atoi(argv[2]);
			printf("se va a enviar un mensaje random de tamanio: %d\n", tamano);

			void* data = generate_void(tamano);
			char* mensaje = void_to_string(data, tamano);
			free(data);

			printf("pre envio\n");
			enviar_mensaje(mensaje, socket_servidor, tamano + 1);
			printf("Se envio el mensaje: %s\n", mensaje);
			free(mensaje);
		}
		else{
			printf("\nerror de primer argumento\n");
			exit(1);
		}
	}
	else{
		printf("\nerror de cantidad de argumentos\n");
		exit(1);
	}


	recibir_loggear(socket_servidor);

	terminar_programa(socket_servidor, logger, config);
}

void recibir_loggear(int socket_servidor){
	//recibir mensaje
	char* buffer;
	printf("intentando recibir cod_op\n");
	int cod_op;
		if(recv_with_retry(socket_servidor, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		printf("error\n");
			else
		printf("se recibio la cod op: %d\n", cod_op);

		int size;
	printf("esperando recibir tamanio del mensaje\n");
	recv_with_retry(socket_servidor, &size, sizeof(int), MSG_WAITALL);
	printf("se solicito recibir un tamanio de mensaje de: %d\n", size);
	buffer = malloc(size);
	recv_with_retry(socket_servidor, buffer, size, MSG_WAITALL);



	//loguear mensaje recibido
	printf("mensaje recibido: %s\n", buffer);

	free(buffer);
}

//TODO DONE
t_log* iniciar_logger(void)
{
	return log_create("log.log", "tp0", true, LOG_LEVEL_INFO);

}

//TODO DONE
t_config* leer_config(void)
{
	return config_create("tp0.config");
}

//TODO DONE
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_info(logger, "fin del programa\n");
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
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

	printf("Void generado\n");
	return data;
}

char* generate_repeat(int tamano, int* string_size){
	int i;
	char* string = string_new();
	string_append(&string, "start-");
	for(i=0; i<tamano; i++){
		string_append(&string, "123-");
	}
	string_append(&string, "end");
	*string_size = strlen(string);

	printf("String generado\n");
	return string;
}


