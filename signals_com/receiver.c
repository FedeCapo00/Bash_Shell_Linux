#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void handler(int sigNum, siginfo_t * info, void * empty) {
	printf("Received int: %d\n", info->si_value.sival_int);
}

int main() {
	struct sigaction sa;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags |= SA_SIGINFO;

	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("Error setting up signal handler.\n");
		return 1;
	}

	while(1);

	return 0;
}
