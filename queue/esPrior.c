#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define NUM_PRIORITIES 5

struct msgbuf {
    long mtype;  // Tipo del messaggio (rappresenta la priorità)
    char mtext[1024];
};

int main() {
    key_t queueKeys[NUM_PRIORITIES];
    int queueIds[NUM_PRIORITIES];

    // Creazione o apertura delle code dei messaggi per ogni priorità
    for (int i = 0; i < NUM_PRIORITIES; ++i) {
        if ((queueKeys[i] = ftok("/tmp/unique", i + 1)) == -1) {
            perror("ftok");
            exit(EXIT_FAILURE);
        }

        if ((queueIds[i] = msgget(queueKeys[i], 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
            perror("msgget");
            exit(EXIT_FAILURE);
        }

        printf("Coda di priorità %d creata o aperta con successo.\n", i);
    }

    // Invio di messaggi con diverse priorità
    struct msgbuf message;
    for (int i = 0; i < NUM_PRIORITIES; ++i) {
        message.mtype = i + 1;
        snprintf(message.mtext, sizeof(message.mtext), "Messaggio di priorità %d", i);

        if (msgsnd(queueIds[i], &message, sizeof(message.mtext), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        printf("Messaggio di priorità %d inviato con successo.\n", i);
    }

    // Ricezione e stampa dei messaggi in ordine di priorità
    for (int i = 0; i < NUM_PRIORITIES; ++i) {
        while (msgrcv(queueIds[i], &message, sizeof(message.mtext), i, IPC_NOWAIT) != -1) {
            printf("Messaggio di priorità %d ricevuto: %s\n", i, message.mtext);
        }
    }

    // Rimozione delle code dei messaggi
    for (int i = 0; i < NUM_PRIORITIES; ++i) {
        if (msgctl(queueIds[i], IPC_RMID, NULL) == -1) {
            perror("msgctl");
            exit(EXIT_FAILURE);
        }

        printf("Coda di priorità %d rimossa con successo.\n", i);
    }

    return 0;
}
