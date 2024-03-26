#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{	
	printf("ingrese un texto... \n");	
	char caracter = 's';
	while(caracter = getchar()!= EOF)
	{
	  
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
