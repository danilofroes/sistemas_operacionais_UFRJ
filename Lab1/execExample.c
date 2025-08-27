#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	pid_t child_pid = fork();

	if (child_pid == 0) {
		execlp("/criarFilhos","criarFilhos","2", NULL);
		return 0;
	}

	else {
		wait(NULL);
		printf("Task is done\n");
		return 0;
	}
}
