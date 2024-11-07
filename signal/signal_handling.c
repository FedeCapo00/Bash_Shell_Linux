#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void handler(int sig) {
	printf("\nRicevuto il segnale %d\n", sig);
	exit(EXIT_SUCCESS);
}

int main () {
	sigset_t set;

	//init a vuoto
	sigemptyset(&set);
	if (sigemptyset(&set) == -1) {
		perror("Impossibile inizializzare la lista dei segnali!");
		return EXIT_FAILURE;
	}

	//add di type di segnali
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGTERM);
	if (sigaddset(&set, SIGINT) == -1 || sigaddset(&set, SIGTERM) == -1) {
		perror("Impossibile aggiungere segnali!");
		return EXIT_FAILURE;
	}

	//operazioni non bloccate
	sigprocmask(SIG_BLOCK, &set, NULL);
	printf("Maschera impostata per bloccare SIGINT...\n");
	printf("Ora sto eseguendo operazioni senza essere interrotto da SIGINT\n");
	sleep(3);
	printf("\nOperazioni terminate!\n");

	//sblocco SIGINT
	if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1) {
		perror("Impossibile sbloccare SIGINT");
		return EXIT_FAILURE;
	}
	printf("SIGINT ora è sbloccato\n");

	//collego segnale con handler
	signal(SIGINT, handler);

	printf("Waiting signals...\n");

	while(1) {
		sleep(1);
	}

	return EXIT_SUCCESS;
}
