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
// Variabile che contiene il nome del file associato al processo PID
char filename[20];
char pathAssolutoIPC[100];
static int num = 32;
// Struttura per Messagge Queue 
struct mesg_buffer{
    long m_type;
    char m_text[1024];
} messageSend;
//***********************************************************************************************************************************************************
// Funzion e che scrive SIGUSER1 nel file associato al processo
void writePidFile();
// Funzione che scrive PID nella messageQueue
void writeQueue();
//***********************************************************************************************************************************************************
// Funzione di gestione del segnale SIGUSR1: se riceve un SIGUSR1 il processo scrive sul file associato il proprio PID + 'a capo'
void sigusr1_handler(){ writePidFile(); }
// Funzione di gestione del segnale SIGUSR2: se riceve un SIGUSR2 il processo scrive sulla messageQueue il proprio PID e tipo pari a 1
void sigusr2_handler(){ writeQueue(); }
//***********************************************************************************************************************************************************
/* main(int argc, char** argv): 
    * genera un processo e un file con nome [PID].txt
    * riceve come parametro in argv[1] il [PATH assoluto] del file [key.txt] per aprire Message Queue
*/
int main(int argc, char** argv){
    int fd;
    int toRet = 0;
    // Salvo il PATH assoluto del file key.txt
    strcpy(pathAssolutoIPC, argv[1]);
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
    while(1); // Lo mantengo in memoria in busy waiting
    return(toRet);
}
//***********************************************************************************************************************************************************
/* sendQueue():
    * invocata quando ricevo un SIGNAL SIGUSR2
    * scrive SIGUSR1 nel file [PID].txt associato al processo [PID]
*/  
void writeQueue(){
    key_t key;
    int msgid;
    // Genera key-token univoco usando PATH assoluto key.txt e num=32
    key = ftok(pathAssolutoIPC, num);
    if(key == -1){
        perror("ftok() failed");
    }else{
        // Ottengo la message queue creata dal processo principale
        msgid = msgget(key, 0666 | IPC_CREAT);
        // Rimuovo la queue se esiste, se la decommento non funziona più la comunicazione
        //msgctl(msgid, IPC_RMID, NULL);
        //msgid = msgget(key, 0666 | IPC_CREAT);
        if(msgid == -1){
            perror("msgget() failed");
        }else{
            // Scrivo il PID del processo principale e tipo uguale a 1 e setto il msg-type a 1
            sprintf(messageSend.m_text, "%d", getpid()); 
            messageSend.m_type = 1;
            // Invio il messaggio nella queue
            if(msgsnd(msgid, &messageSend, sizeof(messageSend.m_text), 0) < 0){ perror("msgsnd() failed"); }
        }
    }
}
//***********************************************************************************************************************************************************
/* writePidFile():
    * invocata quando ricevo un SIGNAL SIGUSR1
    * scrive SIGUSR1 nel file [PID].txt associato al processo [PID]
*/
void writePidFile(){
    int fd_File, controlVar;
    // Apro il file associato al processo e scrivo SIGUSR1
    fd_File = open(filename, O_WRONLY | O_APPEND, 0755);
        if(fd_File == -1){ perror("open() file pid failed"); exit(EXIT_FAILURE); }
        controlVar = write(fd_File, "SIGUSR1\n", strlen("SIGUSR1\n")+1);
        if(controlVar == -1){ perror("write() file pid failed"); exit(EXIT_FAILURE); }
    close(fd_File);
}
//***********************************************************************************************************************************************************

