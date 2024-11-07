#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

void myHandler(int sigNum);

int main(int argc, char *argv[]) {
/*	primo esercizio:
	fork();
	fork();
	fork();
	printf("hello\n");

	secondo esercizio:
	int fork();
	if (fork == 0) {
		printf("I'm the child, my PID is: ");
		printf("%d\n",getpid());
	}
	else {
		printf("I'm the parent, my PID is: ");
		printf("%d\n", getpid());
	}

	terzo esercizio (del prof):
	int fid = fork(), st;

	srand(time(NULL));
	int r = rand()%256;

	if (fid == 0) {
		printf("Child... (%d)",r);
		fflush(stdout);
		sleep(3);
		printf("Done!\n");
		exit(r);
	} else {
		printf("Parent...\n");
		int wid = wait(&st);
		printf("...child's id: %d == %d (std=&d)\n",fid,wid,WEXITSTATUS(st));
	}

	quarto esercizio:
	if (argc < 2) {
		fprintf(stderr,"Usage: %s program [args..]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int outputFile = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (outputFile == -1) {
		perror("Errore nell'apertura del file");
		exit(EXIT_FAILURE);
	}

	pid_t childPid = fork();
	if(childPid == -1) {
		perror("Errore nella fork");
		exit(EXIT_FAILURE);
	}

	if (child == 0)
		printf("Sono il figlio.");
	else if (child == 1)
		printf("Sono il padre.");
*/

	//printf("sigNum of SIGINT: %p\n",signal(SIGINT,myHandler));
	//printf("sigNum of SIGTSTP: %p\n",signal(SIGTSTP,myHandler));

	return (0);
}
