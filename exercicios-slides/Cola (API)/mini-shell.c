#include <stdio.h>
#include <string.h>

#define OK 0
#define ERRO_CRIAR_ARQUIVO 1

#define MAX_BUFFER 256

int main() {
    FILE *ptr_file;
    char buffer[MAX_BUFFER];

    ptr_file = fopen("log.txt", "a");

    if (ptr_file == NULL) {
        perror("Erro ao abrir o log.txt");
        return ERRO_CRIAR_ARQUIVO;
    }

    printf("Mini-Shell de Log. Digite 'exit' para sair.\n");

    for (;;) {
        printf("> ");

        if (fgets(buffer, MAX_BUFFER, stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        fprintf(ptr_file, "%s\n", buffer);
        fflush(ptr_file);
    }

    fclose(ptr_file);
    printf("Log salvo. Encerrando mini-shell!\n");

    return OK;
}