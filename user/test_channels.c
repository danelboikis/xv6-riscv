#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int cd = channel_create();
    if (cd < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }
    printf("Channel created\n");
    if (fork() == 0) {
        if (channel_put(cd, 42) < 0) {
            printf("Failed to put data in channel\n");
            exit(1);
        }
        if (channel_put(cd, 43) < 0) { // Sleeps until cleared
            printf("Failed to put data in channel\n");
            exit(1);
        }
        sleep(100);
        printf("Child woke up\n");
        if (channel_destroy(cd)) {
            printf("Failed to destroy channel\n");
            exit(1);
        }
        //printf("Channel destroyed\n");
    } 
    else {
        int data;
        if (channel_take(cd, &data) < 0) { // 42
            printf("Failed to take data from channel\n");
            exit(1);
        }
        printf("Data: %d\n", data);

        if (channel_take(cd, &data)) { // 43
            printf("Failed to take data from channel\n");
            exit(1);
        }
        printf("Data: %d\n", data);
        
        if (channel_take(cd, &data)) { // Sleep until child destroys channel
            printf("Failed to take data from channel\n");
            exit(1);
        }
        printf("Data: %d\n", data);
    }
    exit(0);
}