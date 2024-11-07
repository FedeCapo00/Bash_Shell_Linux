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

    // Invio del primo messaggio
    struct msgbuf message1;
    message1.mtype = 1;
    strcpy(message1.mtext, "Ciao, mondo!");

    if (msgsnd(queueId, &message1, sizeof(message1.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Primo messaggio inviato con successo.\n");

    // Invio del secondo messaggio
    struct msgbuf message2;
    message2.mtype = 1;
    strcpy(message2.mtext, "Salve!");

    if (msgsnd(queueId, &message2, sizeof(message2.mtext), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    printf("Secondo messaggio inviato con successo.\n");

    // Ricezione di entrambi i messaggi
    struct msgbuf receivedMessage;
    for (int i = 0; i < 2; ++i) {
        if (msgrcv(queueId, &receivedMessage, sizeof(receivedMessage.mtext), 1, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Messaggio ricevuto: %s\n", receivedMessage.mtext);
    }

    // Rimozione della coda dei messaggi
    if (msgctl(queueId, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("Coda dei messaggi rimossa con successo.\n");

    return 0;
}
