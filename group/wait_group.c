#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main (void) {
	int group1 = fork();
	int group2;
	system("ps");
	if (group1 == 0) {
		setpgid(0,getpid());
		fork();
		fork();
		sleep(2);
		return 0;
	} else {
		group2 = fork();
		if (group2 == 0) {
			setpgid(0,getpid());
			fork();
			fork();
			sleep(4);
			return 0;
		}
		sleep(1);
		while(waitpid(-group1, NULL, 0)>0);
		printf("Children in %d terminated.\n",group1);
		while(waitpid(-group2, NULL, 0)>0);
		printf("Children in %d terminated.\n",group2);
	}
}
