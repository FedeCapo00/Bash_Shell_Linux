#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int get_last_pid(char * path);

void handler(int signo) {
	if (signo == 2) {
		exit(EXIT_SUCCESS);
	}
}

int main (int argc, char *argv[]) {
        char * path = argv[1];
	char pid_letto[256];
        FILE * pf;

	signal(SIGINT, handler);

	while (1) {
	        pf = fopen(path, "r");

        	if (pf != NULL) {
			if (fgets(pid_letto, sizeof(pid_letto), pf) != NULL) {
				printf("[client] server: %s", pid_letto);
                		fclose(pf);
                		break; // Esci dal ciclo se hai letto con successo il PID
			}
			fclose(pf);
        	}
	}

	int contatore = 0;
	char input_tast;
	printf("Digita un comando oppure premi INVIO: ");

	while (contatore >= 0 && contatore <= 10 ) {
    		scanf("%c", &input_tast);
		if (input_tast == '+') {
			contatore = contatore + 1;
			int pid_server = get_last_pid(path);
			kill(pid_server, SIGUSR1);
		} else if (input_tast == '-') {
			contatore = contatore - 1;
			int pid_server = get_last_pid(path);
			kill(pid_server, SIGUSR2);
		}
		 else if (input_tast == '\n'){
			break;
		}
		printf("[client] %d\n", contatore);
	}

        return 0;
}

int get_last_pid(char * path) {
	FILE *inputFile = fopen(path, "r");
	if (inputFile == NULL) {
		perror("Error opening input file");
		return 1;
	}

	fseek(inputFile, 0, SEEK_END);

	long currentPosition = ftell(inputFile);
	char lastLine[128];
	lastLine[0] = '\0';

	// Move backward until a newline character is found or the beginning of the file is reached
	while (currentPosition > 0) {
		currentPosition--;
	        fseek(inputFile, currentPosition, SEEK_SET);
		char currentChar = fgetc(inputFile);
		if (currentChar == '\n') {
			fgets(lastLine, strlen(lastLine), inputFile);
			break;
		}
	}
	fclose(inputFile);

	int lastPid;
	sscanf(lastLine, "%d", &lastPid);
	return lastPid;
}
