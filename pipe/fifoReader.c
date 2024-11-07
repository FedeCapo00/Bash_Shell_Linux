#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char *argv[]) {

	int fd;
	char * fifoName = "/tmp/fifo1";

	// Create pipe
	mkfifo(fifoName,S_IRUSR|S_IWUSR);

	char str1[80], * str2 = "I'm a reader";

	// Open READ side
	fd = open(fifoName , O_RDONLY);

	// Read and close
	read(fd, str1, 80);
	close(fd);

	printf("Writer is writing: %s\n", str1);

	// Open WRITE side
	fd = open(fifoName , O_WRONLY);;

	// Write and close
	write(fd, str2, strlen(str2)+1);
	close(fd);

	return 0;
}
