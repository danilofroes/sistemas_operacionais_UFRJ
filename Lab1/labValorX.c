#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main(void) {
	int x;
	pid_t child_pid = fork();
	srand(time(NULL) + child_pid);

	if (child_pid == 0) {
		x = rand() % 1000;
		printf("Eu sou o filho. O valor de x eh %d \n", x);
		return 0;
	}
	else {
		x = rand() % 1000;
		printf("Eu sou o pai. O valor de x eh  %d \n", x);
		wait(NULL);
		printf("Fim!\n");
		return 0;
	}
}
