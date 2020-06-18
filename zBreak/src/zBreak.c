/*
 ============================================================================
 Name        : zBreak.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	int i = 0;

	while(1){
		i++;
		if(i>8){
			if(i>10){
				break;
			}
		}
	}
	puts("fin");

	return EXIT_SUCCESS;
}
