#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main () {
	int fd[2];
	char buf[50];
	pipe(fd);

	int p2 = !fork();
	if (p2) {
		//chiusura lato scrittura
		close(fd[1]);
		int r = read(fd[0], &buf, 50);
		close(fd[0]);
		printf("Buf: '%s'\n",buf);
	} else {
		//chiusura lato lettura
		close(fd[0]);
		write(fd[1], "writing", 8);
		close(fd[1]);
	}
	while(wait(NULL)>0);

	return 0;
}
