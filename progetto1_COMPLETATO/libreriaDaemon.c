//***********************************************************************************************************//
#include "libreriaDaemon.h"
//***********************************************************************************************************//
// Definizione delle variabili usate da generaDaemonFile.c e libreriaDaemon.c
// Nome del file di testo associato al processo daemon [PID].txt
char filename[20];
// Path dove è situata la message queue
char pathAssolutoIPC[100];
// Numero da passare per la funzione ftok()
int num = 32;
//***********************************************************************************************************//
struct mesg_buffer messageSend;
//***********************************************************************************************************//
/* Funzione di gestione del segnale SIGUSR1: 
    * se riceve un SIGUSR1 il processo scrive sul file associato il proprio PID + 'a capo'
*/
void sigusr1_handler(){
    writePidFile();
}
//***********************************************************************************************************//
/* Funzione di gestione del segnale SIGUSR2: 
    * se riceve un SIGUSR2 il processo scrive sulla messageQueue il proprio PID e tipo pari a 1
*/
void sigusr2_handler(){
    writeQueue();
}
//***********************************************************************************************************//
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
//***********************************************************************************************************//
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
//***********************************************************************************************************//