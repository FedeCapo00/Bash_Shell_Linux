#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void cstHandler (int sigNum) {
	/*if (sigNum == 14)
		printf("[%d]received ALARM!\n",getpid());
	else*/

	printf("[%d]received sigNum: %d\n",getpid(),sigNum);
}

/*
short cnt = 0;
void countHandler (int sigNum) {
	printf("ALARM!\n");
	cnt++;
}
*/

int main()  {
/*
	// Es1
	signal(SIGALRM,cstHandler);

	int child = fork();

	if (!child)
		while(1);

	printf("[%d]sending ALARM to %d in 3 sec\n",getpid(),child);
	sleep(3);
	kill(child,SIGALRM);

	printf("[%d]sending SIGTERM to %d in 3 sec\n",getpid(),child);
	sleep(3);
	kill(child,SIGTERM);

	while(wait(NULL)>0);

	// Es2
	signal(SIGCHLD,cstHandler);
	int child2 = fork();
	if (!child2)
		return 0;
	while(wait(NULL)>0)

	// Es3
	signal(SIGALRM,countHandler);
	alarm(0);  // Clear any pending alarm
	alarm(5);  // Set alarm to 5 sec
	printf("Seconds remaining to previous alarm %d\n",alarm(2));
	while(cnt<1);

	// Es4
	signal(SIGINT,cstHandler);

	printf("Waiting for a signal...\n");

	while(1) {
		sleep(1);
	}

	// Es5
	signal(SIGCHLD,cstHandler);

	int child = fork();
	if (child == 0) {
		// Child process
		printf("Child process running...\n");
		sleep(2);
		exit(0);
	} else if (child > 0) {
		// Parent process
		printf("Parent process waiting child to terminate...\n");
		wait(NULL);
		printf("Parent process exiting\n");
	} else {
		// Fork failed
		perror("Fork failed");
		return 1;
	}
*/
	// Maschere:
	signal(SIGINT,cstHandler);

	sigset_t mask;
	sigemptyset(&mask);

	sigaddset(&mask,SIGUSR1);

	sigprocmask(SIG_BLOCK, &mask, NULL);

	printf("Waiting for signals...\n");

	sleep(5);

	sigset_t currentMask;
	sigprocmask(SIG_SETMASK, NULL, &currentMask);

	if (sigismember(&currentMask, SIGINT))
		printf("SIGINT is blocked\n");
	else
		printf("SIGINT is not blocked\n");

	sigdelset(&mask,SIGINT);
	sigprocmask(SIG_SETMASK, &mask, NULL);

	printf("Waiting for signals...\n");

	sleep(10);

	return (0);
}
