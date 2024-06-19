#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre de archivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat fileStat;

    if (stat(argv[1], &fileStat) < 0) {
        perror("Error al obtener información del archivo");
        exit(EXIT_FAILURE);
    }

    printf("ID del usuario propietario: %d\n", fileStat.st_uid);

    printf("Tipo de archivo: ");
    if (S_ISREG(fileStat.st_mode)) {
        printf("Archivo regular\n");
    } else if (S_ISDIR(fileStat.st_mode)) {
        printf("Directorio\n");
    } else if (S_ISCHR(fileStat.st_mode)) {
        printf("Archivo de dispositivo de caracteres\n");
    } else if (S_ISBLK(fileStat.st_mode)) {
        printf("Archivo de dispositivo de bloques\n");
    } else if (S_ISFIFO(fileStat.st_mode)) {
        printf("FIFO\n");
    } else if (S_ISLNK(fileStat.st_mode)) {
        printf("Enlace simbólico\n");
    } else if (S_ISSOCK(fileStat.st_mode)) {
        printf("Socket\n");
    } else {
        printf("Desconocido\n");
    }

    printf("Número de i-nodo: %ld\n", (long)fileStat.st_ino);

    printf("Tamaño en bytes: %lld\n", (long long)fileStat.st_size);

    printf("Número de bloques: %lld\n", (long long)fileStat.st_blocks);

    printf("Tiempo de última modificación: %s", ctime(&fileStat.st_mtime));

    printf("Tiempo de último acceso: %s", ctime(&fileStat.st_atime));

    exit(EXIT_SUCCESS);
}