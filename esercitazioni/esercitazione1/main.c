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
#include <stdbool.h>

#define MAX_FOGLIE 10
pid_t foglie_pid[MAX_FOGLIE];
int cont = 1;

/* checkDirectory(...) riceve in input PATH e verifica se esiste e se si hanno i permessi d'accesso in lettura.
 * Valore di ritorno:
    * 0: SUCCESS
    * 1: BAD ADDRESS
    * 2: NOT EXISTS
    * 3: PERMISSION DENIED 
*/
int checkDirectory (char *pathCheck) {
    int toRet = 0;
    struct stat sb; // Per esplorare il filesystem

    if( stat(pathCheck, &sb) == -1 ) {
        // Bad Address
        if(errno == EFAULT) toRet = 1;

        // Component of path does not exist, or path is an empty string. 
        //if(errno == ENOENT) toRet = 2;

        // Search permission is denied 
        if(errno == EACCES) toRet = 3;
    }

    return toRet;
}

void handler(int sigNum) {
    if (sigNum == 10) {
        // manda un segnale SIGUSR2 (0 indica di inviare il segnale a tutti i processi nello stesso gruppo)
        kill(0,SIGUSR2);

        // kill una foglia casuale
        kill(foglie_pid[cont],SIGTERM);
        cont--;
    } else if (sigNum == 15) {
        // kill manager e tutte le foglie
        for (int i=0; i != cont; i++) {
            kill(foglie_pid[i],SIGTERM);
        }
    } else if (sigNum == 14) {
        // blocca i segnali SIGALRM
    }
}

int main(int argc, char ** argv) {

    if ( argc != 3 ) {
        perror("3");
    } else {
        char* target = argv[1];
        int n = atoi(argv[2]);

        if ( !(n > 1 && n <= 10 )) {
            perror("4");
        } else {
            if (checkDirectory(target) == 0) {

                // "0755" read and execute access to everyone
                int fd_File = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0755);

                if (fd_File == -1) {
                    perror("6");
                } else {

                    // Scrivo sul file il pid del target seguito da \n
                    int target_pid = getpid();
                    int num_digits = snprintf(NULL, 0, "%d", target_pid);
                    char target_pid_toSend[num_digits + 1];

                    sprintf(target_pid_toSend, "%d\n", target_pid);
                    write(fd_File, target_pid_toSend, strlen(target_pid_toSend));

                    // Genero 1 figlio "manager"
                    pid_t manager = fork();
            
                    if (manager == -1) { 
                        perror("6"); 
                        exit(EXIT_FAILURE); 
                    } else if (manager > 0) { 
                        // Qui sono nel padre
                        //kill(getpid(),SIGTERM);
                        pause();
                    } else if (manager == 0) { 
                        // Qui sono nel manager 

                        // Gestione dei segnali ricevuti dal manager
                        signal(SIGUSR1, handler);
                        signal(SIGTERM, handler);
                        signal(SIGALRM, handler);

                        // Scrivo il PID del manager
                        int manager_pid = getpid();
                        int num_digits = snprintf(NULL, 0, "%d", manager_pid);
                        char manager_pid_toSend[num_digits + 1];
                        
                        foglie_pid[0] = manager_pid;

                        sprintf(manager_pid_toSend, "%d\n", manager_pid);
                        write(fd_File, manager_pid_toSend, strlen(manager_pid_toSend));

                        for ( int i=0; i < n; i++) {
                            pid_t foglia = fork();
                            if (foglia == -1) { 
                                perror("6"); 
                                exit(EXIT_FAILURE); 
                            } else if (foglia > 0) { 
                                // Qui sono nel padre
                                return(0);
                            } else if (foglia == 0) { 
                                printf("%d\n", getpid());
                                // Scrivo il PID del nodo foglia
                                int foglia_pid = getpid();

                                // Salvo pid della foglia
                                foglie_pid[cont] = foglia_pid;
                                cont++;

                                int num_digits = snprintf(NULL, 0, "%d", foglia_pid);
                                char foglia_pid_toSend[num_digits + 1];
                                
                                sprintf(foglia_pid_toSend, "%d\n", foglia_pid);
                                write(fd_File, foglia_pid_toSend, strlen(foglia_pid_toSend));
                            }
                        }
                    }
                }
            } else {
                perror("6");
            }
        }
    }

    return 0;
}