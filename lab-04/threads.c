#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int tiempo;

void* funcionHilos(void * s){
    unsigned int sleepTime = (rand() % tiempo + 1);
    
    long i = (long) s; 

    printf("Hilo %ld Voy a dormir: %d\n", i, sleepTime);
    sleep(sleepTime);

    pthread_exit((void *)(intptr_t) sleepTime);
}


int main(int argc, char* argv[])
{
    int cantidadHilos = atoi(argv[1]);
    tiempo = atoi(argv[2]);
    pthread_t thread_ids[cantidadHilos];

    long i;
    for(i=0;i<cantidadHilos;i++){
        pthread_create(&thread_ids[i],NULL,funcionHilos,(void *)i);
    }

    void * value;
    for(i=0;i<cantidadHilos;i++){
        pthread_join(thread_ids[i], &value);
        printf("Tiempo que durmio el hilo %ld: %ld\n", i, (long)value);
    }


    exit(EXIT_SUCCESS);
}