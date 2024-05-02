#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variable global que incrementan los hilos.
static long glob = 0;


void increment_glob(int loops)
{
    int loc, j;

    // incrementa glob
    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }
}

void* funcionHilos(void * s){
    
    long num = (long)s;

    increment_glob(num);
    pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
    long loops;

    // Controla numero de argumentos.
    if (argc != 2) {
        fprintf(stderr, "Uso: %s ciclos\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    loops = atoi(argv[1]);

    // Verifica argumentos.
    if (loops < 1) {
        fprintf(stderr, "Error: ciclos debe ser mayor a cero.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t hilos[2];

    pthread_create(&hilos[0],NULL,funcionHilos,(void *)loops);
    pthread_create(&hilos[1],NULL,funcionHilos,(void *)loops);

    pthread_join(hilos[0],NULL);
    pthread_join(hilos[1],NULL);

    printf("%ld\n", glob);

    exit(EXIT_SUCCESS);
}
