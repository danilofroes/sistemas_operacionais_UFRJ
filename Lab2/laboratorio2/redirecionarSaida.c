#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(void){
	pid_t child_pid = fork();
	int fd;
	char *filename = "saida.out";
	if (child_pid == 0) {
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		dup2(fd, STDOUT_FILENO);
		execlp("/bin/ping","ping","localhost","-c","10",NULL);
		return 0;
	}
	else {
		wait(NULL);
		return 0;
	}
}
