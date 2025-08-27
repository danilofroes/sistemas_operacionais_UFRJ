#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <num>\n", argv[0]);
        return 1;
    }

    printf("O pid do pai eh %d\n",getpid());

    int num_processes = atoi(argv[1]);
    pid_t pid;

    for (int i = 0; i < num_processes; i++) {
        pid = fork();
    }

    wait(NULL);

    sleep(3600);

    return 0;
}