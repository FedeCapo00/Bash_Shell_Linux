#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_KEY 5678

struct msgbuf {
    long mtype;
    char mtext[1024];
};

int main() {
    key_t queueKey = QUEUE_KEY;
    int queueId;

    // Creazione o apertura della coda dei messaggi
    if ((queueId = msgget(queueKey, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Coda dei messaggi creata o aperta con successo.\n");

    // Invio di un messaggio
    struct msgbuf message;
    message.mtype = 1;  // Tipo del messaggio (può essere qualsiasi valore positivo)
    strcpy(message.mtext, "Ciao, mondo!");

    if (msgsnd(queueId, &message, sizeof(message.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    if (msgsnd(queueId, &message, sizeof(message.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Messaggio inviato con successo.\n");

    // Ricezione di un messaggio
    if (msgrcv(queueId, &message, sizeof(message.mtext), 1, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    printf("Messaggio ricevuto: %s\n", message.mtext);

    // Rimozione della coda dei messaggi
    if (msgctl(queueId, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("Coda dei messaggi rimossa con successo.\n");

    return 0;
}
