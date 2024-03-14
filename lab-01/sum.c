#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i;
	int suma = 0;
	
	for(i=1;i<argc;i++)
	{
		suma+=atoi(argv[i]);
	}
	
	printf("El resultado es: %d \n", suma);
    exit(EXIT_SUCCESS);
}
