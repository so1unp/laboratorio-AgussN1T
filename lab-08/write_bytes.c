#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    size_t bufSize, numBytes, thisWrite, totWritten;
    char *buf;
    int syncOption, fd, openFlags;

    if (argc != 5) {
        fprintf(stderr, "Uso: %s num-bytes buf-size sync archivo\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numBytes = atoi(argv[1]);
    bufSize = atoi(argv[2]);
    syncOption = atoi(argv[3]);

    buf = malloc(bufSize);
    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    openFlags = O_CREAT | O_WRONLY;

    fd = open(argv[4], openFlags, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for (totWritten = 0; totWritten < numBytes; totWritten += thisWrite) {
        thisWrite = (numBytes - totWritten) > bufSize ? bufSize : (numBytes - totWritten);

        if (write(fd, buf, thisWrite) != thisWrite) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        if (syncOption == 1) {
            if (fsync(fd) == -1) {
                perror("fsync");
                exit(EXIT_FAILURE);
            }
        } else if (syncOption == 2) {
            if (fdatasync(fd) == -1) {
                perror("fdatasync");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    free(buf);
    exit(EXIT_SUCCESS);
}