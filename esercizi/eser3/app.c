//-------- Librerie:
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

//--------- Prototipi:
int check_path(char * path);

//--------- Main:
int main (int argc, char *argv[]) {

        if (argc != 3) {
                fprintf(stderr, "Usage: %s <server/client> <path>\n", argv[0]);
                return 1;
        }

	char *path = argv[2];
	char basePath[] = "/home/capo/esercizi/eser3";
	char bufferTmp[strlen(path)+strlen(basePath)+1];
	snprintf(bufferTmp, sizeof(bufferTmp), "%s%s", basePath, path);

	if (check_path(bufferTmp) != 0) {
		fprintf(stderr, "The path %s is invalid, with error %d\n",argv[2],check_path(path));
		return 1;
	} else {
	        if (strcmp(argv[1], "server") == 0) {
       	         	execlp("/home/capo/esercizi/eser3/server", "./server", bufferTmp, NULL);
       	 	} else if (strcmp(argv[1], "client") == 0) {
                	execlp("/home/capo/esercizi/eser3/client", "./client", bufferTmp, NULL);
       	 	} else {
			fprintf(stderr, "Invalid argument: %s\n", argv[1]);
                	return 1;
        	}
	}


        return 0;
}

//------- Funzioni:
int check_path(char *path){
    errno = 0;
    int toRet = 0;
    struct stat sb;

    if(stat(path, &sb) == -1){
        // Bad Address
        if(errno == EFAULT) toRet = 1;
        // Component of path does not exist, or path is an empty string.
        if(errno == ENOENT) toRet = 2;
        // Search permission is denied
        if(errno == EACCES) toRet = 3;
    }

    return toRet;
}
