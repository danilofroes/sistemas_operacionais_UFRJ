#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

//O Pid de destino serah uma variavel global, para facilitar o trabalho
pid_t pidDestino;

void signal_handler(int sigNumber) {
    if (sigNumber == SIGTERM)
        sleep(1);

    printf("Recebi um sinal, enviando pong...\n");
    kill(pidDestino, SIGTERM);
}

int main(int argc, char ** argv) {
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        printf("\nNao consigo tratar SIGINT\n");
    }

    char pidDestinoString[10];
    pid_t meuPid = getpid();

    printf("Meu pid eh (pong): %d \n",meuPid);

    printf("Digite o pid do outro processo e aperte ENTER\n");

    fgets(pidDestinoString, 9, stdin);
    pidDestino = (pid_t) atoi(pidDestinoString);

    kill(pidDestino, SIGTERM);

    while(1)
        sleep(1);

    return 0;
}