//***********************************************************************************************************//
#include "libreriaDaemon.h"
//***********************************************************************************************************//
/* main(int argc, char** argv): 
    * genera un processo e un file con nome [PID].txt
    * riceve come parametro in argv[1] il [PATH assoluto] del file [key.txt] per aprire Message Queue
*/
int main(int argc, char** argv){
    int fd;
    int toRet = 0;
    // Salvo il PATH assoluto del file key.txt
    strcpy(pathAssolutoIPC, argv[1]);
    //strncpy(pathAssolutoIPC, argv[1], sizeof(pathAssolutoIPC) - 1);
    //pathAssolutoIPC[sizeof(pathAssolutoIPC) - 1] = '\0';
    // Installazione del gestore per il segnale SIGUSR1
    signal(SIGUSR1, sigusr1_handler);
    // Installazione del gestore per il segnale SIGUSR2
    signal(SIGUSR2, sigusr2_handler);
    // Recupero il PID del processo
    sprintf(filename, "%d.txt", getpid());
    // Creo file con nome [PID].txt
    fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0755);
        if(fd == -1){ perror("Errore nella creazione del file"); exit(EXIT_FAILURE); }
    close(fd);
    // Lo mantengo in memoria in busy waiting
    while(1);
    return(toRet);
}
//***********************************************************************************************************//