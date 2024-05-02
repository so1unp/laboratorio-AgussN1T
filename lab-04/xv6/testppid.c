#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int ppid = getppid();
    printf(1, "Parent PID: %d\n", ppid);
    exit();
}