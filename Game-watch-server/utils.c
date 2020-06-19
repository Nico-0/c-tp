/*
 * utils.c		SERVIDOR
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#include"utils.h"
int global = 1;
#define RETRY_WAIT 20

void iniciar_servidor(void)
{
	int socket_servidor;

    struct addrinfo hints, *servinfo, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    for (p=servinfo; p != NULL; p = p->ai_next)
    {
        if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            close(socket_servidor);
            printf("ERROR DE BIND\n");
            exit(1);
            continue;
        }
        break;
    }

	listen(socket_servidor, SOMAXCONN);

    freeaddrinfo(servinfo);

    while(1)
    	esperar_cliente(socket_servidor);
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*)serve_client,&socket_cliente);
	pthread_detach(thread);

}

void serve_client(int* socket)
{
	printf("cliente conectado (%d), esperando cod_op\n", global);
	global++;
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	printf("se recibio la cod op: %d\n", cod_op);
	if(cod_op != 1){
		printf("cod_op erronea\n");
		pthread_exit(NULL);
	}
	process_request(cod_op, *socket);

}

void process_request(int cod_op, int cliente_fd) {
	int size;
	void* msg;
		switch (cod_op) {
		case MENSAJE:
			msg = recibir_mensaje(cliente_fd, &size);
			devolver_mensaje(msg, size, cliente_fd);
			free(msg);
			break;
		case 0:
			pthread_exit(NULL);
		case -1:
			pthread_exit(NULL);
		}
}

void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;
	int result;

	printf("esperando recibir tamanio del mensaje\n");
	recv_with_retry(socket_cliente, size, sizeof(int), MSG_WAITALL);
	printf("se solicito recibir un tamanio de mensaje de: %d\n", *size);
	buffer = malloc(*size);
	result = recv_with_retry(socket_cliente, buffer, *size, MSG_WAITALL);
	printf("Se recibieron %d bytes de %d\n", result, *size);
	if(result == -1){
		printf("Se recibio -1\n");
		pthread_exit(NULL);
	}
	else if(result<*size){
		printf("Se reciben pocos bytes\n");
		pthread_exit(NULL);
	}
	printf("mensaje recibido: %s\n", (char*)buffer);
	printf("(Se habian recibido %d bytes de %d)\n", result, *size);

	return buffer;
}

void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

void devolver_mensaje(void* payload, int size, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = size;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	printf("Intentando devolver mensaje\n");
	if(send_with_retry(socket_cliente, a_enviar, bytes, MSG_NOSIGNAL) == -1)
		printf("Error al enviar (-1)\n");
	else
		printf("Enviado todos los bytes\n");

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

int32_t send_with_retry(int32_t socket, void* a_enviar, size_t bytes, int32_t flag){

	int32_t result = 0;
	int32_t current_bytes;
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
		current_bytes += result;
		if(current_bytes < bytes){
			printf("Se enviaron %d de %d bytes\n", current_bytes, bytes);
			printf("Reintentando en %d segundos, por vez %d\n", RETRY_WAIT, i);
			sleep(RETRY_WAIT);
			i++;
		}

	}
	printf("Se enviaron %d de %d bytes\n", current_bytes, bytes);
	return current_bytes;
}



int32_t recv_with_retry(int32_t socket, void* a_recibir, size_t bytes, int32_t flag){

	int32_t result = 0;
	int32_t current_bytes;
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
