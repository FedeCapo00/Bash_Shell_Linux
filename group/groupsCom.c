#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void handler (int sigNum) {
	printf("[%d,%d] sig%d received.\n", getpid(), getpgid(0), sigNum);
	sleep(1);
	exit(0);
}

int main () {
	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);

	int ancestor = getpid();
	int group1 = fork();
	int group2;

	if (getpid() != ancestor) {
		setpgid(0, getpid());
		fork();
		fork();
	}
	else {
		group2 = fork();

		if (getpid() != ancestor) {
			setpgid(0,getpid());
			fork();
			fork();
		}
	}

	if (getpid() == ancestor) {
		printf("[%d]Ancestor and I'll send signals.\n", getpid());
		sleep(1);
		kill(-group2,SIGUSR2);
		kill(-group1,SIGUSR1);
	} else {
		printf("[%d,%d]Child waiting signals.\n",getpid(),getpgid(0));
		while(1);
	 }
	while(wait(NULL)>0);
	printf("All children terminated.\n");
}
