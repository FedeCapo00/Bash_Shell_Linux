#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	pid_t child_pid = fork();

	if (child_pid == 0) {
		printf("Child process: PID=%d, PGID=%d\n",getpid(),getpgrp());
		setpgid(0,0);
		//printf("%s\n", strerror(errno));
		printf("Child process after setpgid: PID=%d, PGID=%d\n",getpid(),getpgrp());

		pid_t child2_pid = fork();
		if (child2_pid == 0) {
			printf("Child2 process: PID=%d, PGID=%d\n",getpid(),getpgrp());
			setpgid(0,0);
			printf("Child2 process after setpgid: PID=%d, PGID=%d\n",getpid(),getpgrp());
		}
		else
			printf("Parent2 process: PID=%d, PGID=%d\n",getpid(),getpgrp());
	}
	else {
		wait(NULL);
		printf("Parent process: PID=%d, PGID=%d\n",getpid(),getpgrp());
	}

	return 0;
}
