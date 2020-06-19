/*
 * utils.c		SERVIDOR
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#include"utils.h"
int global = 1;

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

	printf("esperando recibir tamanio del mensaje\n");
	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	printf("se solicito recibir un tamanio de mensaje de: %d\n", *size);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);
	printf("mensaje recibido: %s\n", (char*)buffer);

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
	if(send(socket_cliente, a_enviar, bytes, 0) == -1)
		printf("Error al enviar\n");
	else
		printf("Enviado\n");

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

