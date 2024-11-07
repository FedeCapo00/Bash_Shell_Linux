#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int errno;

int main (void) {
	FILE * pf;
	pf = fopen("nonExFile.boh","rb");

	if (pf == NULL) {
		fprintf(stderr, "errno 0 %d\n", errno);
		perror("Error printed");
		fprintf(stderr,"stderr: %s\n", strerror(errno));
	}
	else {
		fclose(pf);
	}
}
