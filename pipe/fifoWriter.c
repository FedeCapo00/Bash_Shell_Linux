#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {

	int fd;
	char * fifoName = "/tmp/fifo1";
	char str1[80], * str2 = "I’m a writer";

	// Create pipe
	mkfifo(fifoName,S_IRUSR|S_IWUSR);

	// Open WRITE side
	fd = open(fifoName, O_WRONLY);

	// Write and close it
	write(fd, str2, strlen(str2)+1);
	close(fd);

	// Open READ side
	fd = open(fifoName, O_RDONLY);

	// Read from the pipe
	read(fd, str1, sizeof(str1));
	printf("Reader is writing: %s\n", str1);

	close(fd);
	return 0;
}
