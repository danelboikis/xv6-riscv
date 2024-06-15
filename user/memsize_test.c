#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    printf("process size: %d\n", memsize());
    char *m = malloc(20000);
    printf("process size: %d\n", memsize());
    free(m);
    printf("process size: %d\n", memsize());
    exit(0, 0);
}