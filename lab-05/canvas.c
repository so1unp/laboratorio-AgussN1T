#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define HEIGHT 25 // Altura en caracteres de la pizarra
#define WIDTH 25  // Ancho en caracteres de la pizarra

struct canvas
{
    char canvas[HEIGHT * WIDTH];
};

typedef struct canvas canvas_t;

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-w canvas mensaje x y [h|v]: escribe el mensaje en el canvas indicado en la posición (x,y) de manera [h]orizontal o [v]ertical.\n");
    fprintf(stderr, "\t-p canvas: imprime el canvas indicado.\n");
    fprintf(stderr, "\t-c canvas: crea un canvas con el nombre indicado.\n");
    fprintf(stderr, "\t-d canvas: elimina el canvas indicado.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

void create_canvas(char *nombre)
{
    int fd = shm_open(nombre, O_CREAT | O_RDWR, 0622);

    if (fd < 0)
        exit(EXIT_FAILURE);

    ftruncate(fd, sizeof(canvas_t));
    close(fd);
}

void delete_canvas(char *nombre)
{
    shm_unlink(nombre);
}

void add_canvas(char *nombre, char *input, int x, int y)
{
    int fd = shm_open(nombre, O_RDWR, 0622);
    canvas_t *canvas = (canvas_t *)mmap(NULL, sizeof(canvas_t), PROT_WRITE, MAP_SHARED, fd, 0);

    int i;
    for (i = 0; i < strlen(input); i++)
    {
        canvas->canvas[(((HEIGHT * x) + y) + i)%(HEIGHT * WIDTH)] = input[i];
    }

    close(fd);
}

void print_canvas(char *nombre)
{
    int fd = shm_open(nombre, O_RDWR, 0622);

    canvas_t *canvas = (canvas_t *)mmap(NULL, sizeof(canvas_t), PROT_WRITE, MAP_SHARED, fd, 0);

    int i;
    int j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (canvas->canvas[((HEIGHT * i) + j)] == '\0')
                putchar('#');
            else
                putchar(canvas->canvas[((HEIGHT * i) + j)]);
        }
        putchar('\n');
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-')
    {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch (option)
    {
    case 'w':
        add_canvas(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]));
        printf("Escribe %s en el canvas %s en la posición (%d, %d).\n", argv[3], argv[2], atoi(argv[4]), atoi(argv[5]));
        break;
    case 'p':
        print_canvas(argv[2]);
        break;
    case 'c':
        create_canvas(argv[2]);
        break;
    case 'd':
        delete_canvas(argv[2]);
        break;
    case 'h':
        usage(argv);
        break;
    default:
        fprintf(stderr, "Comando desconocido\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
