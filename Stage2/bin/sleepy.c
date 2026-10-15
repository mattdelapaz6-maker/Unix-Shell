#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: sleepy <seconds>\n");
        return 1;
    }

    int seconds = atoi(argv[1]);

    printf("Sleepy started (PID %d), sleeping for %d seconds...\n", getpid(), seconds);

    sleep(seconds);

    printf("Sleepy finished (PID %d)\n", getpid());

    return 0;
}