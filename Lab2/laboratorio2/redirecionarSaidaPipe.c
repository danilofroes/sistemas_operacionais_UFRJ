
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void read_from_pipe(int file) {
	FILE *stream;
	int c;
	stream = fdopen (file, "r");

	while ((c = fgetc (stream)) != EOF)
		putchar (c);

	fclose (stream);
}

int main(void) {
	pid_t child_pid = fork();
	int mypipe[2];

	if (child_pid == 0) {

		close (mypipe[0]);
		dup2(mypipe[1], STDOUT_FILENO);
		execlp("/bin/ping","ping","localhost","-c","10",NULL);

		return 0;
	}

	else {
		wait(NULL);
		close (mypipe[1]);
		read_from_pipe (mypipe[0]);

	    return 0;
	}
}