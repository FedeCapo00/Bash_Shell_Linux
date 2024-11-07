#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

extern int errno;

int main(void) {
	int sys = kill(3443,SIGUSR1);
	if (sys == -1) {
		fprintf(stderr, "errno = %d\n", errno);
		perror("Error printed.\n");
		fprintf(stderr, "strerror: %s\n", strerror(errno));
	} else {
		printf("Signal sent.\n");
	}
}
