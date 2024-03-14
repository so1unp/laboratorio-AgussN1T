#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Agregar código aquí.
    //
    //printf("Hola Mundo \n");

	int i;

	for(i=1;i<argc;i++)
	{
		printf("%s \n",argv[i]);
	}
   
 // Termina la ejecución del programa.
    exit(EXIT_SUCCESS);
}
