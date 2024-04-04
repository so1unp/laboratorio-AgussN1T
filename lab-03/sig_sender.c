#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
    __pid_t pid = getpid();

    printf("Mi PID es: %d\n", pid);

    exit(EXIT_SUCCESS);
}