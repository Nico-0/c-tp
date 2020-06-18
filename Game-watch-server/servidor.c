/*
 * servidor.c
 *
 *  Created on: 3 mar. 2019
 *      Author: utnso
 */

#include "servidor.h"

int main(void)
{
	if((config = config_create("tp0-server.config")) == NULL)
		printf("ERROR DE CONFIG\n");

	IP = config_get_string_value(config, "IP");
	PUERTO = config_get_string_value(config, "PUERTO");

	iniciar_servidor();

	return EXIT_SUCCESS;
}


