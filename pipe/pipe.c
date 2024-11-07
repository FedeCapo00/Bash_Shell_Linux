#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
	int fd[2];
	char buf[50];

	int esito = pipe(fd);

	if (esito == 0) {
		// Write to pipe
		write(fd[1], "writing", 8);

		// Read from pipe
		int r = read(fd[0], &buf, 50);
		printf("Last read %d. Received: '%s'\n", r, buf);

		r = read(fd[0], &buf, 50);
		printf("Last read %d. Received: 's'", r, buf);
	}

	/*
	int fd[2], cnt = 0;

	// Create unamed pipe using 2 file descriptors
	while (pipe(fd) == 0) {
		cnt++;
		printf("%d, %d, ", fd[0], fd[1]);
	}

	printf("\nOpened %d pipes, the error\n",cnt);

	int op = open("/tmp/tmp.txt",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	printf("File opened with fd %d\n", op);
	*/

	return 0;
}
