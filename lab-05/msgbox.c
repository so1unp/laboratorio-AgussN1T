#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define USERNAME_MAXSIZE    15  // Máximo tamaño en caracteres del nombre del remitente.
#define TXT_SIZE            100 // Máximo tamaño del texto del mensaje.
#define QUEUE_PERMISSIONS 0660
/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg {
    char sender[USERNAME_MAXSIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;

/**
 * Imprime información acerca del uso del programa.
 */
void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-s queue mensaje: escribe el mensaje en queue.\n");
    fprintf(stderr, "\t-r queue: imprime el primer mensaje en queue.\n");
    fprintf(stderr, "\t-a queue: imprime todos los mensaje en queue.\n");
    fprintf(stderr, "\t-l queue: vigila por mensajes en queue.\n");
    fprintf(stderr, "\t-c queue: crea una cola de mensaje queue.\n");
    fprintf(stderr, "\t-d queue: elimina la cola de mensajes queue.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

void create_queue(const char *queue_name) {
    mqd_t mq = mq_open(queue_name, O_CREAT | O_RDONLY, QUEUE_PERMISSIONS, NULL);
    if (mq == (mqd_t)-1) {
        perror("Error creando la cola de mensajes");
    } else {
        printf("Cola de mensajes %s creada.\n", queue_name);
        mq_close(mq);
    }
}

void delete_queue(const char *queue_name) {
    if (mq_unlink(queue_name) == -1) {
        perror("Error eliminando la cola de mensajes");
    } else {
        printf("Cola de mensajes %s eliminada.\n", queue_name);
    }
}

void send_message(const char *queue_name, const char *message) {
    mqd_t mq = mq_open(queue_name, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("Error abriendo la cola de mensajes");
        return;
    }

    msg_t msg;
    if (getlogin_r(msg.sender, USERNAME_MAXSIZE) != 0) {
        perror("Error obteniendo el nombre de usuario");
        mq_close(mq);
        return;
    }

    strncpy(msg.text, message, TXT_SIZE);

    if (mq_send(mq, (const char*)&msg, sizeof(msg_t), 0) == -1) {
        perror("Error enviando el mensaje");
    } else {
        printf("Mensaje enviado a la cola %s.\n", queue_name);
    }

    mq_close(mq);
}

void receive_message(const char *queue_name) {
    mqd_t mq = mq_open(queue_name, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("Error abriendo la cola de mensajes");
        return;
    }

    msg_t msg;
    if (mq_receive(mq, (char*)&msg, sizeof(msg_t), NULL) == -1) {
        perror("Error recibiendo el mensaje");
    } else {
        printf("Mensaje de %s: %s\n", msg.sender, msg.text);
    }

    mq_close(mq);
}

void print_all_messages(const char *queue_name) {
    mqd_t mq = mq_open(queue_name, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("Error abriendo la cola de mensajes");
        return;
    }

    msg_t msg;
    while (mq_receive(mq, (char*)&msg, sizeof(msg_t), NULL) != -1) {
        printf("Mensaje de %s: %s\n", msg.sender, msg.text);
    }

    if (errno != EAGAIN) {
        perror("Error recibiendo el mensaje");
    }

    mq_close(mq);
}

void listen_for_messages(const char *queue_name) {
    mqd_t mq = mq_open(queue_name, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("Error abriendo la cola de mensajes");
        return;
    }

    msg_t msg;
    while (1) {
        if (mq_receive(mq, (char*)&msg, sizeof(msg_t), NULL) != -1) {
            printf("Mensaje de %s: %s\n", msg.sender, msg.text);
        } else {
            perror("Error recibiendo el mensaje");
            sleep(1); // Esperar un rato antes de intentar nuevamente
        }
    }

    mq_close(mq);
}

void print_queue_info(const char *queue_name) {
    mqd_t mq = mq_open(queue_name, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("Error abriendo la cola de mensajes");
        return;
    }

    struct mq_attr attr;
    if (mq_getattr(mq, &attr) == -1) {
        perror("Error obteniendo los atributos de la cola");
    } else {
        printf("Información de la cola %s:\n", queue_name);
        printf("\tNúmero máximo de mensajes: %ld\n", attr.mq_maxmsg);
        printf("\tTamaño máximo del mensaje: %ld\n", attr.mq_msgsize);
        printf("\tNúmero actual de mensajes: %ld\n", attr.mq_curmsgs);
    }

    mq_close(mq);
}


int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];
    
    switch(option) {
        case 's':
            if (argc < 4) {
                fprintf(stderr, "Error: Faltan parámetros para -s\n");
                usage(argv);
                exit(EXIT_FAILURE);
            }
            send_message(argv[2], argv[3]);
            break;
        case 'r':
            if (argc < 3) {
                fprintf(stderr, "Error: Falta parámetro para -r\n");
                usage(argv);
                exit(EXIT_FAILURE);
            }
            receive_message(argv[2]);
            break;
        case 'a':
            if (argc < 3) {
                fprintf(stderr, "Error: Falta parámetro para -a\n");
                usage(argv);
                exit(EXIT_FAILURE);
            }
            print_all_messages(argv[2]);
            break;
        case 'l':
            if (argc < 3) {
                fprintf(stderr, "Error: Falta parámetro para -l\n");
                usage(argv);
                exit(EXIT_FAILURE);
            }
            listen_for_messages(argv[2]);
            break;
        case 'c':
            if (argc < 3) {
                fprintf(stderr, "Error: Falta parámetro para -c\n");
                usage(argv);
                exit(EXIT_FAILURE);
            }
            create_queue(argv[2]);
            break;
        case 'd':
            if (argc < 3) {
                fprintf(stderr, "Error: Falta parámetro para -d\n");
                usage(argv);
                exit(EXIT_FAILURE);
            }
            delete_queue(argv[2]);
            break;
        case 'i':
            if (argc < 3) {
                fprintf(stderr, "Error: Falta parámetro para -i\n");
                usage(argv);
                exit(EXIT_FAILURE);
            }
            print_queue_info(argv[2]);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
            usage(argv);
            exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
    
    
    
    exit(EXIT_SUCCESS);
}
/*
    switch(option) {
        case 's':
            printf("Enviar %s a la cola %s\n", argv[3], argv[2]);
            break;
        case 'r':
            printf("Recibe el primer mensaje en %s\n", argv[2]);
            break;
        case 'a':
            printf("Imprimer todos los mensajes en %s\n", argv[2]);
            break;
        case 'l':
            printf("Escucha indefinidamente por mensajes\n");
            break;
        case 'c':
            printf("Crea la cola de mensajes %s\n", argv[2]);
            break;
        case 'd':
            printf("Borra la cola de mensajes %s\n", argv[2]);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
    */