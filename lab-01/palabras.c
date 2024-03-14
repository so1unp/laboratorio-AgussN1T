#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{	
	printf("ingrese un texto... \n");	
	char caracter;
	while(1)
	{
	  caracter = getchar();
	  
		if(caracter == ' ')
		{
	  		printf("\n");
		}
		else
		{
			printf("%c",caracter);
		}
	  
	}

    exit(EXIT_SUCCESS);
}
