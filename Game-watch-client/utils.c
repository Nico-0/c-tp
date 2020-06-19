/*
 * utils.c		CLIENTE
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"
#define RETRY_WAIT 20
#define RETRY_AMOUNT 3

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete *paquete, int *bytes)
{


}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error de connect\n");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente, int tamano)
{
	t_buffer *ptr_buffer = malloc(sizeof(t_buffer));
	ptr_buffer->size = tamano;
	ptr_buffer->stream = mensaje;
	printf("Se cargo el t_buffer\n");

	int bytes = ptr_buffer->size + sizeof(int) + sizeof(int);

	t_paquete *paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = ptr_buffer;
	printf("Se cargo el t_paquete\n");


	void* a_enviar = malloc(sizeof(int)*2 + ptr_buffer->size);
	int offset = 0;

	memcpy(a_enviar, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	int result;
	printf("Intentando enviar\n");
	//sleep(100);
	if((result = send_with_retry(socket_cliente, a_enviar, bytes, MSG_NOSIGNAL)) == -1){
		printf("Error al enviar\n");
		exit(-1);
	}
	else{
		printf("El send dio %d bytes\n", result);
		printf("Enviado (falta hacer la confirmacion de recepcion)\n");
	}

	if(result<bytes){
		printf("No se reciben suficientes bytes\n");
		exit(-1);
	}


	free(a_enviar);
//	free(paquete->buffer->stream);		//da segmentation fault
	free(paquete->buffer);
	free(paquete);


}

//TODO
char* recibir_mensaje(int socket_cliente)
{

}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

int32_t send_with_retry(int32_t socket, void* a_enviar, size_t bytes, int32_t flag){

	int32_t result = 0;
	int32_t current_bytes;
	int retry = RETRY_AMOUNT;
	int i = 1;

	current_bytes = result;
	while(current_bytes < bytes){

		result = send(socket, a_enviar + current_bytes, bytes - current_bytes, flag); //El send manda los bytes, no siempre puede asegurar si el otro proceso lo recibio.
		printf("El send dio: %d\n", result);
		if(result == -1){
			printf("ERRORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR de envio\n");
			printf("Se desconecto el proceso, hay que volver a accept-connect\n");
			return -1;
		}
		if(retry == 0){
			printf("Se acabaron los reintentos\n");
			pthread_exit(NULL);
		}
		current_bytes += result;
		if(current_bytes < bytes){
			printf("Se enviaron %d de %d bytes\n", current_bytes, bytes);
			printf("Reintentando en %d segundos, por vez %d\n", RETRY_WAIT, i);
			sleep(RETRY_WAIT);
			i++;
			retry--;
		}

	}
	printf("Se enviaron %d de %d bytes\n", current_bytes, bytes);
	return current_bytes;
}



int32_t recv_with_retry(int32_t socket, void* a_recibir, size_t bytes, int32_t flag){

	int32_t result = 0;
	int32_t current_bytes;
	int retry = RETRY_AMOUNT;
	int i = 1;

	current_bytes = result;
	while(current_bytes < bytes){ 	//en principio el flag MSG_WAITALL se va a quedar esperando a recibir t0do, no hace falta reintentar el recv
										//dice el man que si lo interrumpe una signal, va a recibir menos
										//asi que lo reintentamos igual
		result = recv(socket, a_recibir + current_bytes, bytes - current_bytes, flag);
		printf("El recv dio: %d\n", result);
		if(result == -1){
			printf("ERRORRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR de recv\n");
			printf("Se desconecto el proceso, hay que volver a accept-connect\n");
			return -1;
		}
		if(retry == 0){
			printf("Se acabaron los reintentos\n");
			pthread_exit(NULL);
		}
		current_bytes += result;
		if(current_bytes < bytes){
			printf("Se recibieron %d de %d bytes\n", current_bytes, bytes);
			printf("Reintentando en %d segundos, por vez %d\n", RETRY_WAIT, i);
			sleep(RETRY_WAIT);
			i++;
		}

	}
	printf("Se recibieron %d de %d bytes\n", current_bytes, bytes);
	return current_bytes;


	return result;
}
