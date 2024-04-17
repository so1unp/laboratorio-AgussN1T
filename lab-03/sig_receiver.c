#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void signal_handler(int signum)
{

    printf("Se recibió la señal %d %s \n", signum, strsignal(signum));
}
struct sigaction sig;

int main(void)
{
    
    sig.sa_handler = signal_handler;

    //sigaction(signum, &sig, NULL);
   
    int signum;
    __pid_t pid = getpid();

    printf("Mi PID es: %d\n", pid);

    for (signum = 1; signum < 255; signum++)
    {
        sigaction(signum, &sig, NULL);
    }

    while (1)
    {
        pause();
    }

    exit(EXIT_SUCCESS);
}
