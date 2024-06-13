#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int count = pscnt();
    printf(1, "Número de procesos en el sistema: %d\n", count);
    exit();
}