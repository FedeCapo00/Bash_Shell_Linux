//***********************************************************************************************************************************************************
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
//***********************************************************************************************************************************************************
#define DIM_BUFFER_LONG 1024
#define DIM_BUFFER_SHORT 64
#define TRUE 1
#define FALSE 0
// Lunghezza massima gestita dei percorsi nel file-system
#define PATH_MAXLEN 60
// Numero massimo di figli gestibili
#define CHILDREN_MAX 10
// Lunghezza massima di stringa che rappresenti un pid
#define PIDLEN 8
// Nome cartella da creare
#define DIR_NAME "INFO"
// Nome file di testo da creare
#define FILE_NAME "key.txt"
//***********************************************************************************************************************************************************
// PID processo principale
pid_t FATHER_PID;
// Struttura per Messagge Queue 
struct mesg_buffer{ 
    long m_type;
    char m_text[DIM_BUFFER_LONG];
} messageReceive, messageSend;
//***********************************************************************************************************************************************************
int checkDirectory(char *);
int generaMSG_Queue(char *, int);
void stampaMessageQueue(char *, int);
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
        * 10: MSG Queue ftok() fallita
        * 11: MSG Queue msgid() fallita
        * 12: MSG Queue msgsnd() fallita
        * 13: MSG Queue errore generico
*/
int main(int argc, char **argv){
    int toRet = 0;
    int numChild = 0;
    int controlVar;
    char bufferPidFather[PIDLEN];
    if(argc == 3){
        // Controllo lunghezza PATH
        if(strlen(argv[1]) > PATH_MAXLEN){
            toRet = 2;
            fprintf(stderr, "-------------------------------------\n[PATH] eccede la lunghezza massima consentita!\n-------------------------------------\n");
        }else{
            // Controllo NUM figli
            numChild = atoi(argv[2]);
            if(numChild <= 0 || numChild > CHILDREN_MAX){
                toRet = 3;
                fprintf(stderr, "-------------------------------------\n[NUM] minore o uguale a 0!\n-------------------------------------\n");
            }else{
                // Controllo che PATH sia esistente
                controlVar = checkDirectory(argv[1]);
                if(controlVar == 0){
                    // Mi sposto nella directory ricevuta in input
                    if(chdir(argv[1]) == -1){
                        toRet = 5;
                        fprintf(stderr, "- ------------------------------------\nCambio directory /%s fallita\n-------------------------------------\n", argv[1]);
                    }else{
                        // Creo sottocartella [PATH USER]/INFO/
                        controlVar = mkdir(DIR_NAME, 0755); 
                        if(controlVar == -1 && errno != EEXIST){
                            toRet = 6;
                            fprintf(stderr, "-------------------------------------\nCreazione cartella /%s fallita\n-------------------------------------\n", DIR_NAME);
                        }else{
                            // Mi sposto nella cartella ricevuta in input
                            if(chdir(DIR_NAME) == -1){
                                toRet = 7;
                                fprintf(stderr, "-------------------------------------\n Cambio directory /%s fallita\n-------------------------------------\n", DIR_NAME);
                            }else{
                                // Creo file INFO/KEY.TXT
                                int fd_File = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0755);
                                if(fd_File == -1){
                                    toRet = 8;
                                    fprintf(stderr, "-------------------------------------\nCreazione file INFO/%s fallita\n-------------------------------------\n", FILE_NAME);
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
                                                    execlp("/home/capo/esercizi/daemon", "daemon", cwd, NULL);
                                                    perror("Errore execl()");
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
                                                    toRet = 10;
                                                    fprintf(stderr, "-------------------------------------\nMSG_QUEUE ftok() fallita\n-------------------------------------\n");
                                                break;
                                                case 2:
                                                    toRet = 11;
                                                    fprintf(stderr, "-------------------------------------\nMSG_QUEUE msgid() fallita\n-------------------------------------\n");
                                                break;
                                                case 3:
                                                    toRet = 12;
                                                    fprintf(stderr, "-------------------------------------\nMSG_QUEUE msgsnd() fallita\n-------------------------------------\n");
                                                break;
                                                default:
                                                    toRet = 13;
                                                    fprintf(stderr, "-------------------------------------\nMSG_QUEUE errore generico\n-------------------------------------\n");
                                                break;
                                            }
                                        }
                                    }else{
                                        toRet = 9;
                                        fprintf(stderr, "-------------------------------------\nRecupero PATH key.txt fallito\n-------------------------------------\n");
                                    }
                                }
                            }
                        }
                    }
                }else{
                    toRet = 4;
                    fprintf(stderr, "-------------------------------------\n[PATH] inesistente o problemi con i permessi!\n-------------------------------------\n");
                }
            }
        }
    }else{
        toRet = 1;
        fprintf(stderr, "-------------------------------------\nUsage:\n\t./app [PATH/] [NUM]\n-------------------------------------\n");
    }
    return(toRet);
}
//***********************************************************************************************************************************************************
/* checkDirectory(...) riceve in input PATH e verifica se esiste e se si hanno i permessi d'accesso in lettura.
 * Valore di ritorno:
    * 0: SUCCESS
    * 1: BAD ADDRESS
    * 2: NOT EXISTS
    * 3: PERMISSION DENIED 
*/
int checkDirectory(char *pathCheck){
    int toRet = 0;
    struct stat sb; // Per esplorare il filesystem
    if(stat(pathCheck, &sb) == -1){
        // Bad Address
        if(errno == EFAULT) toRet = 1;
        // Component of path does not exist, or path is an empty string. 
        if(errno == ENOENT) toRet = 2;
        // Search permission is denied 
        if(errno == EACCES) toRet = 3;
    }
    return toRet;
}
//***********************************************************************************************************************************************************
/* generaMSG_Queue():
    * crea una Message Queue (eliminandola se esiste)
    * usa come chiave per ftok() [PATH ASSOLUTO key.txt] e num: 32
    * scrivo nella Message Queue il [PID] processo principale e tipo pari ad 1
    * Valori di ritorno:
        * 0: SUCCESS  
        * 1: ftok() fallita
        * 2: msgget() fallita
        * 3: msgsnd() fallita
*/
int generaMSG_Queue(char* path, int num){
    int toRet = 0;
    key_t key; 
    int msgid;
    // Genera key-token univoco usando PATH assoluto key.txt e num=32
    key = ftok(path, num);
    if(key == -1){
        toRet = 1;
    }else{
        // Crea message queue e ritorna l'id queue
        msgid = msgget(key, 0666 | IPC_CREAT);
        // Rimuovo la queue se esiste, se decommento, non funziona più la comunicazione
        //msgctl(msgid, IPC_RMID, NULL);
        //msgid = msgget(key, 0666 | IPC_CREAT);
        if(msgid == -1){
            toRet = 2;
        }else{
            // Scrivo il PID del processo principale e tipo uguale a 1
            sprintf(messageSend.m_text, "%d", FATHER_PID); 
            messageSend.m_type = 1;
            // Invio il messaggio nella msg-queue
            if(msgsnd(msgid, &messageSend, sizeof(messageSend.m_text), 0) < 0){ toRet = 3; }
        }
    }
    return(toRet);
}
//***********************************************************************************************************************************************************
/* stampaMessageQueue(char *path, int num):
    * funzione che stampa la message queue per verificare il corretto funzionamento
    * path: percorso assoluto del file [key.txt] da usare per ftok()
    * num: intero usato da ftok()
*/
void stampaMessageQueue(char *path, int num){
    long msgtyp = 1;
    key_t key;
    int msgid;
    // Genera key-token univoco usando PATH assoluto key.txt e num=32
    key = ftok(path, num);
    if(key != -1){
        // Crea message queue e ritorna l'id queue
        msgid = msgget(key, 0666 | IPC_CREAT);
        if(msgid != -1){
            int loop = 1;
            /* msgrsv():
                * si blocca quando riceve un signal, ciclo controllando il tipo di errore
                * se errno == EINTR, allora ha ricevuto un SIGNAL e la msg-queue è bloccata rieseguo la lettura
            */
            while(loop){
                // Cerco di ricevere il messaggio inviato sulla msg-queue
                errno = 0;
                int status = msgrcv(msgid, &messageReceive, sizeof(messageReceive.m_text), msgtyp, 0);
                if(status < 0){
                    switch(errno){
                        case 0: loop = 0;
                        case E2BIG: perror("mtext > msgsz"); break;
                        case EACCES: perror("Permission error"); break;
                        case EINVAL: perror("ID not correct"); break;
                        case EINTR: break; // ricevuto SIGNAL, restart della msgrcv()
                    }
                }else{
                    printf("[%s]\n", messageReceive.m_text);
                }
            }
        }
    }
    return;
}
//***********************************************************************************************************************************************************
