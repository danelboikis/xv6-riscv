#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 100

int
check_primality(int num) {
    // check if the number is prime
    for (int i = 2; i < num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    int num_checkers = 3;
    int cd1, cd2;
    if (argc > 1) {
        num_checkers = atoi(argv[1]);
    }

    cd1 = channel_create();
    if (cd1 < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }
    cd2 = channel_create();
    if (cd2 < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }

    for (int i = 0; i < num_checkers; i++) {
        if (fork() == 0) {
            int data;
            while (1) {
                if (channel_take(cd1, &data) < 0) {
                    printf("Checker %d shutting down\n", getpid());
                    break;
                }
                if (check_primality(data)) { 
                    if (channel_put(cd2, data) < 0) {
                        printf("Checker %d shutting down\n", getpid());
                        channel_destroy(cd1);
                        break;
                    }
                }
            }
            exit(0);
        }
    }

    if (fork() == 0) {
        int data;
        for(int i = 0; i < MAX; i++) {
            channel_take(cd2, &data);

            printf("%d. %d\n", i, data);
        }
        printf("Printer %d shutting down\n", getpid());
        channel_destroy(cd2);
        printf("channel2 destroyed\n");
        exit(0);
    }

    for (int i = 2;;i++) {
        if (channel_put(cd1, i) < 0) {
            break;
        }
    }

    wait(0);
    printf("Generator %d shutting down\n", getpid());

    printf("start again? ");
    // get 1 char from user
    char c[3];
    c[0] = '\0';
    c[1] = '\0';
    c[2] = '\0';
    while (c[0] != 'Y' && c[0] != 'N') {
        printf("Y/N?\n");
        gets(c, 3);
    }
    // check if buf is Y or N
    if (c[0] == 'Y') {
        exec(argv[0], argv);
    }
    
    
    exit(0);
}
