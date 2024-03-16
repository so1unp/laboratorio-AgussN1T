#include <stdio.h>
#include <stdlib.h>

#define TAMANIO_MAXIMO 20

int main(int argc, char *argv[])
{	
	
	int histograma[TAMANIO_MAXIMO] = {0};	
	char c;
	int tamanioPalabra = 0;
	
	while((c = getchar()) != EOF)
	{
		if(c != '\n' && c!= ' ' && c!= '\t')
		{
			tamanioPalabra ++;
		}
		else
		{
			histograma[tamanioPalabra] ++;
			tamanioPalabra = 0;		
		}
		
	}
	//dibujar el histograma
	int i;
	int j;
	for(i = 1; i<=TAMANIO_MAXIMO; i++)
	{
		printf("%d", i);
		for(j = 0; j < histograma[i]; j++)
		{
			putchar('*');
		}
		putchar('\n');
	}
	

    exit(EXIT_SUCCESS);
}
