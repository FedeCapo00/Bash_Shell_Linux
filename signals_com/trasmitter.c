#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(stderr,"Usage: %s <pid <integer>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	union sigval value;
	value.sival_int = atoi(argv[2]);

	if (sigqueue(atoi(argv[1]), SIGUSR1, value) == -1) {
		perror("Error sending signals");
		exit(EXIT_FAILURE);
	}

	while(1);

	return 0;
}
