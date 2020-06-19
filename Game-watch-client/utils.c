/*
 * utils.c		CLIENTE
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

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
		printf("error");

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
	if((result = send(socket_cliente, a_enviar, bytes, MSG_NOSIGNAL)) == -1){
		printf("Error al enviar\n");
		exit(-1);
	}
	else{
		printf("El send dio %d bytes\n", result);
		printf("Enviado\n");
	}

	if(result<bytes){
		printf("No se reciben suficientes bytes\n");
		exit(-1);
	}

	printf("quepasa1\n");
	free(a_enviar);
//	free(paquete->buffer->stream);		//da segmentation fault
	free(paquete->buffer);
	free(paquete);
	printf("quepasa2\n");

}

//TODO
char* recibir_mensaje(int socket_cliente)
{

}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
