#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <fcntl.h>

int main() {
    remove("/tmp/unique"); // Rimuovi il file
    int fd = creat("/tmp/unique", 0777); // Crea il file
    close(fd); // Chiudi il file descriptor

    key_t queue1Key = ftok("/tmp/unique", 1); // Ottieni una chiave unica

    if (queue1Key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    int queueId = msgget(queue1Key, 0777 | IPC_CREAT); // Crea la coda

    if (queueId == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    queueId = msgget(queue1Key, 0777); // Ottieni la coda esistente

    if (queueId == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Rimuovi una coda inesistente (fallirà)
    if (msgctl(queue1Key, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    // Rimuovi la coda esistente
    if (msgctl(queueId, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    // Prova ad ottenere una coda inesistente (fallirà)
    queueId = msgget(queue1Key, 0777);

    if (queueId == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Crea una nuova coda
    queueId = msgget(queue1Key, 0777 | IPC_CREAT);

    if (queueId == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Prova a creare una coda già esistente (fallirà)
    queueId = msgget(queue1Key, 0777 | IPC_CREAT | IPC_EXCL);

    if (queueId == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    return 0;
}
