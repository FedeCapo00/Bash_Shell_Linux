#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sigNum) {
	printf("Segnale ricevuto: %d\n",sigNum);
}

int main() {
	sigset_t signals, pendingSignals;

	sigemptyset(&signals);
	sigaddset(&signals, SIGINT);

	sigprocmask(SIG_BLOCK, &signals, NULL);

	printf("Provo invio...\n");

	//prova a commentare questa linea sotto
	kill(getpid(),SIGINT);

	sigpending(&pendingSignals);
	if (sigismember(&pendingSignals, SIGINT)) {
		printf("SIGINT pendente.\n");
		handler(SIGINT);
	} else {
		printf("Nessun segnale pendente.\n");
	}

	sigprocmask(SIG_UNBLOCK, &signals, NULL);

	return 0;
}
