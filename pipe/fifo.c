#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(void){

	char * fifoName = "/tmp/fifo1";

	// Create pipe
	mkfifo(fifoName,S_IRUSR|S_IWUSR);
	perror("Created?");

	if (fork() == 0){
		// Open READ side ... stuck
		open(fifoName,O_RDONLY);
		printf("Open read\n");
	} else {
		sleep(3);

		// Open WRITE side
		open(fifoName,O_WRONLY);
		printf("Open write\n");
	}

	return 0;
}
