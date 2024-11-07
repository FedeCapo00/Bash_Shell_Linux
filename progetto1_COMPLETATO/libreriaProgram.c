//****************************************************************************************************************//
#include "libreriaProgram.h"
//****************************************************************************************************************//
// Variabile globale dove salvo il PID del processo principale
pid_t FATHER_PID;
//****************************************************************************************************************//
// Strutture dati per l'invio e ricezione dei messaggio per la message queue
struct mesg_buffer messageReceive, messageSend;
//****************************************************************************************************************//
/* printErrorCode(int *toRet, int codError, char* msgError, char* msgParameter):
    * toRet: passaggio per riferimento della variabile da restituire a fine esecuzione
    * codError: codice d'errore scelto dal programmatore
    * msgError: messaggio autoesplicativo dell'errore
    * msgParameter: eventuali parametri da stampare su stdout (se NULL non ci sono parametri)
*/
void printErrorCode(int *toRet, int codError, char* msgError, char* msgParameter){
    // Salvo il valore codError nella variabile passata per riferimento toRet
    *toRet = codError;
    if(msgParameter != NULL){
        fprintf(stderr, msgError, msgParameter);
    }else{
        fprintf(stderr, msgError, NULL);
    }
    return;
}
//****************************************************************************************************************//
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
//****************************************************************************************************************//
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
//****************************************************************************************************************//
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
//****************************************************************************************************************//