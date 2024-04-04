#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <signal_number> <pid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int signal_number = atoi(argv[1]);
    __pid_t pid = atoi(argv[2]);

    if (kill(pid, signal_number) == -1) {
        perror("Error al enviar la señal");
        return EXIT_FAILURE;
    }

    printf("Se envió la señal %d al proceso con PID %d\n", signal_number, pid);

    return EXIT_SUCCESS;
}