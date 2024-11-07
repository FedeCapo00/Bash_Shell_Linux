#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sigNum) {
	printf("Segnale ricevuto: %d\n",sigNum);
}

int main() {
	struct sigaction action;
	sigset_t pendingSignals;

	//inizializzo la struttura sigaction
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	if (sigaction(SIGINT, &action, NULL) == -1) {
		perror("Errore durante installazione segnali.\n");
		exit(EXIT_FAILURE);
	}

	sigfillset(&pendingSignals);
	sigprocmask(SIG_BLOCK, &pendingSignals, NULL);

	printf("Invio segnale...\n");
	kill(getpid(), SIGINT);

	sigprocmask(SIG_UNBLOCK, &pendingSignals, NULL);

	return 0;
}
