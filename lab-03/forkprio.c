#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>
int busywork(void)
{
    struct tms buf;
    for (;;) {
        times(&buf);
    }
}

void signal_handler(int signum) {
   struct rusage usage;
   getrusage(RUSAGE_SELF, &usage);
   printf("Child %d (nice %2d):\t%3li\n", getpid(), getpriority(PRIO_PROCESS, getpid()), usage.ru_utime.tv_sec + usage.ru_stime.tv_sec);
}

int main(int argc, char *argv[])
{
   int cantHijos = atoi(argv[1]);
    int hijos[cantHijos];
    int i;

    signal(SIGTERM, signal_handler);

    printf("Hola, soy el padre con PID=%d\n", getpid());

    for (i = 0; i < cantHijos; i++) {
        hijos[i] = fork();

        if (hijos[i] < 0) {
            perror("Error al hacer fork");
            exit(EXIT_FAILURE);
        } else if (hijos[i] == 0) {
            printf("Proceso hijo %d creado con PID=%d\n", i, getpid());
            busywork();
        }
    }
    
    sleep(3);
    
    for(i=0 ; i<cantHijos; i++){
        kill(hijos[i],SIGTERM);
    }

    printf("Proceso: %d terminado\n", getpid());


    exit(EXIT_SUCCESS);
}
