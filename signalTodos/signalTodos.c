/*
 ============================================================================
 Name        : signalTodos.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define cantidad_hilos 4

typedef struct
{
	pthread_cond_t broadcast;
} t_semaforos;


pthread_t hilo[cantidad_hilos];

pthread_mutex_t mutex;
pthread_cond_t condicion; //que no este vacia la cola, y que tenga mensajes que no envio

bool elemento_disponible;

void funcion(void* numero){

    pthread_mutex_lock(&mutex);
       while (!elemento_disponible)	//si cumple la condicion, pasa de largo, y sigue ejecutando el programa. Si no cumple, se bloquea.
           pthread_cond_wait(&condicion, &mutex);
       /* do something that requires holding the mutex and condition is true */
    pthread_mutex_unlock(&mutex);

      //cuando haya elementos disponibles, luego de algun broadcast, el thread va a ejecutar
	printf("Soy el thread %d\n", (int) numero);
}


int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */

	t_semaforos* semis = malloc(sizeof(t_semaforos));
	t_semaforos* semos = malloc(sizeof(t_semaforos));

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&(semis->broadcast), NULL);

	semos = semis;
	condicion = semos->broadcast;

	elemento_disponible = false;

	int j;
	for(j=0; j<cantidad_hilos; j++){
    if(pthread_create(&hilo[j], NULL, (void*)funcion, (void*) j ))
    	printf("ERROR AL CREAR HILO\n");
	}

	sleep(1); //esperar que entren los hilos

    pthread_mutex_lock(&mutex);
    /* do something that might make condition true */
    pthread_cond_broadcast(&condicion);
    pthread_mutex_unlock(&mutex);

    printf("Hice broadcast, pero la condicion era falsa\n");
    printf("Va denuevo:\n");

	elemento_disponible = true;


    pthread_mutex_lock(&mutex);
    /* do something that might make condition true */
    pthread_cond_broadcast(&condicion);
    pthread_mutex_unlock(&mutex);




	for(j=0; j<cantidad_hilos; j++)
    pthread_join(hilo[j], NULL);


    printf("Termine!\n");

    sleep(1);




	return EXIT_SUCCESS;
}
