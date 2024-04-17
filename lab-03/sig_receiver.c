#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void signal_handler(int signum)
{

    printf("Se recibió la señal %d %s \n", signum, strsignal(signum));
}

int main(void)
{
    
    /*
    struct sigaction sig;
    sig.sa_handler = signal_handler;

    sigaction(signum, &sig, NULL);
    */
   
    int sig;
    __pid_t pid = getpid();

    printf("Mi PID es: %d\n", pid);

    for (sig = 1; sig < 255; sig++)
    {
        signal(sig, signal_handler);
    }

    while (1)
    {
        pause();
    }

    exit(EXIT_SUCCESS);
}
