//***********************************************************************************************************************************************************
#include "libreriaProgram.h"
//***********************************************************************************************************************************************************
/* main():
    * controlla che la chiamata sia completa dei parametri richiesti (PATH, NUM di processi da generare).
    * controlla che il PATH esista
    * Valore di ritorno:
        *  0: Success
        *  1: Chiamata errata ./app [PATH/] [NUM]
        *  2: Lunghezza PATH non valida
        *  3: NUM è minore o uguale a 0 o supera il limite massimo
        *  4: PATH inesistente
        *  5: Move directory PATH fallito
        *  6: Creazione cartella INFO fallita
        *  7: Move directory PATH/INFO fallita
        *  8: Creazione file key.txt fallita
        *  9: Recupero path file key.txt fallito
        * 10: Caricamento in memoria daemon exec() fallito
        * 11: MSG Queue ftok() fallita
        * 12: MSG Queue msgid() fallita
        * 13: MSG Queue msgsnd() fallita
        * 14: MSG Queue errore generico
*/
int main(int argc, char **argv){
    int toRet = 0;
    int numChild = 0;
    int controlVar;
    char bufferPidFather[PIDLEN];
    if(argc == 3){
        // Controllo lunghezza PATH
        if(strlen(argv[1]) > PATH_MAXLEN){
            // Stampo errore
            printErrorCode(&toRet, 2, "-------------------------------------\n[PATH] eccede la lunghezza massima consentita!\n-------------------------------------\n", NULL);
        }else{
            // Controllo NUM figli
            numChild = atoi(argv[2]);
            if(numChild <= 0 || numChild > CHILDREN_MAX){
                // Stampo errore
                printErrorCode(&toRet, 3, "-------------------------------------\n[NUM] minore o uguale a 0!\n-------------------------------------\n", NULL);
            }else{
                // Controllo che PATH sia esistente
                controlVar = checkDirectory(argv[1]);
                if(controlVar == 0){
                    // Mi sposto nella directory ricevuta in input
                    if(chdir(argv[1]) == -1){
                        // Stampo errore
                        printErrorCode(&toRet, 5, "-------------------------------------\nCambio directory /%s fallita\n-------------------------------------\n", argv[1]);
                    }else{
                        // Creo sottocartella [PATH USER]/INFO/
                        controlVar = mkdir(DIR_NAME, 0755); 
                        if(controlVar == -1 && errno != EEXIST){
                            // Stampo errore
                            printErrorCode(&toRet, 6, "-------------------------------------\nCreazione cartella /%s fallita\n-------------------------------------\n", DIR_NAME);
                        }else{
                            printf("PID %d\n", getpid());
                            sleep(10);
                            // Mi sposto nella cartella ricevuta in input
                            if(chdir(DIR_NAME) == -1){
                                // Stampo errore
                                printErrorCode(&toRet, 7, "-------------------------------------\nCambio directory /%s fallita\n-------------------------------------\n", DIR_NAME);
                            }else{
                                // Creo file INFO/KEY.TXT
                                int fd_File = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0755);
                                if(fd_File == -1){
                                    // Stampo errore
                                    printErrorCode(&toRet, 8, "-------------------------------------\nCreazione file INFO/%s fallita\n-------------------------------------\n", FILE_NAME);
                                }else{
                                    char cwd[100];
                                    if(getcwd(cwd, sizeof(cwd)) != NULL){
                                        // Concateno al path della cartella corrente il file [key.txt]
                                        strcat(cwd, "/key.txt");
                                        // Ottengo il PID del processo Father
                                        FATHER_PID = getpid();
                                        sprintf(bufferPidFather, "%d\n", FATHER_PID);
                                        // Scrivo nel file PATH/INFO/key.txt il PID del processo Father
                                        controlVar = write(fd_File, bufferPidFather, strlen(bufferPidFather));
                                        close(fd_File);
                                        // Creazione dei processi figli e generazione dei file associati
                                        for(int i = 1; i <= numChild; i++){
                                            if(fork() == 0){
                                                int varFork = fork();
                                                if(varFork == 0){
                                                    // Carico il programma che genera processo daemon e file di testo [daemonPID].txt
                                                    execlp("/tmp/esame1/daemon", "daemon", cwd, NULL);
                                                    // Stampo errore
                                                    printErrorCode(&toRet, 10, "-------------------------------------\nChiamata execl() fallita\n-------------------------------------\n", NULL);
                                                }else{
                                                    // Stampo in ordine i PID dei processi generati
                                                    fprintf(stdout, "%d ", varFork);
                                                    if(i == numChild) fprintf(stdout, "\n");
                                                    exit(EXIT_SUCCESS);
                                                }
                                            }else{
                                                // Termino tutte le chiamate al processo padre tranne l'ultimo che va in sleep
                                                if(i == numChild) sleep(2);
                                                wait(NULL);
                                            }
                                        }
                                        // Funzione che genera la message queue e scrive il PID del processo principale
                                        controlVar = generaMSG_Queue(cwd, 32);
                                        // Controllo il tipo di errore restituito da MSG_Queue
                                        if(controlVar == 0){
                                            for(int i=0; i<numChild; i++){
                                                char operVar[5];
                                                do{
                                                    printf("Stampo QUEUE (y/n)? ");
                                                    char* varDump = fgets(operVar, 5, stdin);
                                                }while(strcmp(operVar, "y\n") != 0);
                                                // Stampo il contenuto della msg-queue
                                                stampaMessageQueue(cwd, 32);
                                            }
                                        }else{
                                            switch(controlVar){
                                                case 1:
                                                    // Stampo errore
                                                    printErrorCode(&toRet, 11, "-------------------------------------\nMSG_QUEUE ftok() fallita\n-------------------------------------\n", NULL);
                                                break;
                                                case 2:
                                                    // Stampo errore
                                                    printErrorCode(&toRet, 12, "-------------------------------------\nMSG_QUEUE msgid() fallita\n-------------------------------------\n", NULL);
                                                break;
                                                case 3:
                                                    // Stampo errore
                                                    printErrorCode(&toRet, 13, "-------------------------------------\nMSG_QUEUE msgsnd() fallita\n-------------------------------------\n", NULL);
                                                break;
                                                default:
                                                    // Stampo errore
                                                    printErrorCode(&toRet, 14, "-------------------------------------\nMSG_QUEUE errore generico\n-------------------------------------\n", NULL);
                                                break;
                                            }
                                        }
                                    }else{
                                        printErrorCode(&toRet, 9, "-------------------------------------\nRecupero [%s/key.txt] fallito\n-------------------------------------\n", argv[1]);
                                    }
                                }
                            }
                        }
                    }
                }else{
                    // Stampo errore
                    printErrorCode(&toRet, 4, "-------------------------------------\n[%s] inesistente o problemi con i permessi!\n-------------------------------------\n", argv[1]);
                }
            }
        }
    }else{
        // Stampo errore
        printErrorCode(&toRet, 1, "-------------------------------------\nUsage:\n\t./app [PATH/] [NUM]\n-------------------------------------\n", NULL);
    }
    return(toRet);
}