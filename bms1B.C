
#include <stdio.h>
#include <stdlib.h>
#include "lib/ecc.h"

void error_msg()
{
	fprintf(stderr, "Incorect input!");
}

int
main (int argc, char *argv[])
{
	if(argc != 2)
	{
		error_msg();
		return EXIT_FAILURE;
	}
	else
		 printf("něco %s",argv[1]);
	
	printf("Hello World!");
	return 0;
}