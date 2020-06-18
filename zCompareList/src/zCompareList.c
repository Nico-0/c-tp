/*
 ============================================================================
 Name        : zCompareList.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/collections/list.h>

void encontrar_numero(t_list *lista, void* numero_buscado, t_list* elementos_coincidentes) {
	bool _soy_el_numero_buscado(void *p) {
		return p == numero_buscado;
	}
	if(list_any_satisfy(lista, _soy_el_numero_buscado)){
		list_add(elementos_coincidentes, numero_buscado);
	}
}

void no_contenido(t_list* lista_enviados, void* elemento_lista_global, t_list* resultado) {
	bool _soy_el_numero_buscado(void *p) {
		return p == elemento_lista_global;
	}
	if(!list_any_satisfy(lista_enviados, _soy_el_numero_buscado)){
		list_add(resultado, elemento_lista_global);
	}
}

t_list* interseccion_lista(t_list* unaLista, t_list* otraLista, t_list* resultado){
	list_clean(resultado);
	t_list* lista = list_duplicate(unaLista);
	void* elemento;

	while(list_size(lista) != 0){
		elemento = list_remove(lista, 0);
		encontrar_numero(otraLista, elemento, resultado);
	}

	if(list_size(lista) == 0)
		list_destroy(lista);
	else
		printf("ERROR no se vacio la lista\n");

	return resultado;
}

//asigna en la lista resultado, los elementos que estan en la primera, pero no en la segunda
t_list* no_enviados_lista(t_list* lista_global, t_list* lista_enviados, t_list* resultado){
	list_clean(resultado);
	t_list* lista = list_duplicate(lista_global);
	void* elemento;

	while(list_size(lista) != 0){
		elemento = list_remove(lista, 0);
		no_contenido(lista_enviados, elemento, resultado);
	}

	if(list_size(lista) == 0)
		list_destroy(lista);
	else
		printf("ERROR no se vacio la lista\n");

	return resultado;
}

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

	//creo dos listas para comparar

	t_list* lista_1 = list_create();
	t_list* lista_2 = list_create();

	bool condicion;
	int i;

	list_add(lista_1, 3);
	list_add(lista_1, 4);
	list_add(lista_1, 5);

	list_add(lista_2, 2);
	list_add(lista_2, 5);
	list_add(lista_2, 3);


	int numero = list_get(lista_1, 0);
	printf("Contenido de la lista: %d\n", numero);
	numero = list_get(lista_1, 1);
	printf("Contenido de la lista: %d\n", numero);
	numero = list_get(lista_1, 2);
	printf("Contenido de la lista: %d\n", numero);
	condicion = (numero == NULL);
	printf("El contenido de la posicion 2 es NULL: %d\n", condicion);
	numero = list_get(lista_1, 3);
	printf("Contenido de la lista: %d\n", numero);
	condicion = (numero == NULL);
	printf("El contenido de la posicion 3 es NULL: %d\n", condicion);	//ojo que si tengo un elemento 0, lo va a considerar NULL
	numero = list_size(lista_1);
	printf("Tamanio de la lista_1: %d\n", numero);

/*	Si es una posicion que no existe, list_get devuelve null
	for(i=4; i<999; i++){
		numero = list_get(lista_1, i);
		printf("Contenido de la lista: %d\n", numero);
		condicion = (numero == NULL);
		printf("El contenido de la posicion %d es NULL: %d\n", i, condicion);
	}
*/

	t_list* resultado = list_create();
	interseccion_lista(lista_1, lista_2, resultado);
	imprimir_lista(resultado, "Prueba");
	list_clean(resultado);
	no_enviados_lista(lista_1, lista_2, resultado);
	imprimir_lista(resultado, "a");
	list_clean(resultado);
	no_enviados_lista(lista_2, lista_1, resultado);
	imprimir_lista(resultado, "b");
























/*

	while(elemento lista_2 != null)
	tomo el primer elemento de la lista_2
	me fijo si pertenece a la lista global
	si pertenece, lo agrego a mi nueva lista
	en cualquier caso, aumento la posicion del elemento en 1.

		t_link_element *elemento = colaAEnviar->head;
		uint32_t posicion = 0;

		t_list* ids_a_enviar = NULL;

		while(elemento != NULL){
			if (falta_enviar_msj(colaEnviados, ((t_pending*)elemento->data)->ID_mensaje))
				list_add(ids_a_enviar, ((t_pending*)elemento->data)->ID_mensaje);
			elemento = elemento->next;
			posicion++;
		}

		return ids_a_enviar;

*/

//dado un elemento, si cumple la condicion
/*	 int _soy_1(void *p) {
	  return p == 1;
	 }
	// list_find(lista, (void*) _soy_1);
*/
//pasado un elemento generico, si cumple la condicion


//dado un elemento, decir si esta contenido en una lista
//	int meme = 3;
	//uso list_any_satisfy, le paso la lista, y la condicion debe ser == a ese elemento


//devolver una lista con los elementos que contiene la primera, pero la segunda no
	//uso list filter
/*
	t_list* lista_filtrada = list_create();

	t_link_element* elemento = lista_2->head;

	while(elemento->data != NULL){

	}

    bool _pertenece_a_lista_1(t_list* lista_2) {
        return person->age <= 24;
    }
*/

//devolver una lista con los elementos que contienen ambos
	//uso list_filter






//devolver un el primer elemento de la lista que este contenida en la otra
	//uso list_find




	/*
	int id = t_mensaje->ID_mensaje;

	    bool soy_mismo_mensaje(void *mensaje){
	        return ((t_pending)mensaje)->ID_mensaje == id;
	    }

	    list_any_satisfy(cola, soy_mismo_mensaje);
	*/

	return EXIT_SUCCESS;
}
