#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <signal.h>
#include <sys/resource.h>
int busywork(void)
{
    struct tms buf;
    for (;;)
    {
        times(&buf);
    }
}

void signal_handler(int signum)
{
    (void)signum;
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Child %d (nice %2d):\t%3li\n", getpid(), getpriority(PRIO_PROCESS, (id_t)getpid()), usage.ru_utime.tv_sec + usage.ru_stime.tv_sec);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Los parámetros enviados no son los que se esperaba\n");
        exit(0);
    }

    int cantHijos = atoi(argv[1]);
    int hijos[cantHijos];
    int tiempoDurmiendo = atoi(argv[2]);
    unsigned int prioridad = (unsigned)atoi(argv[3]);
    int i;

    struct sigaction sig;
    sig.sa_handler = signal_handler;

    sigaction(SIGTERM, &sig, NULL);

    printf("Hola, soy el padre con PID=%d\n", getpid());

    for (i = 0; i < cantHijos; i++)
    {
        hijos[i] = fork();

        if (hijos[i] < 0)
        {
            perror("Error al hacer fork");
            exit(EXIT_FAILURE);
        }
        else if (hijos[i] == 0)
        {
            printf("Proceso hijo %d creado con PID=%d\n", i, getpid());
            if (prioridad)
                nice(i);
            else
                nice(0);
            busywork();
        }
    }

    sleep((unsigned int)tiempoDurmiendo);

    for (i = 0; i < cantHijos; i++)
    {
        kill(hijos[i], SIGTERM);
    }

    printf("Proceso: %d terminado\n", getpid());

    exit(EXIT_SUCCESS);
}
