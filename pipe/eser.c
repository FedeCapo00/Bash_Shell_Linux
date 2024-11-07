#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CHILDREN 10

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numero_di_figli>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_children = atoi(argv[1]);
    if (num_children <= 0 || num_children > MAX_CHILDREN) {
        fprintf(stderr, "Il numero di figli deve essere compreso tra 1 e %d\n", MAX_CHILDREN);
        exit(EXIT_FAILURE);
    }

    // Creazione della pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Errore nella creazione della pipe");
        exit(EXIT_FAILURE);
    }

    // Creazione dei figli
    for (int i = 0; i < num_children; ++i) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Errore nella fork");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Codice eseguito dal figlio

            // Chiusura del lato di scrittura della pipe nel figlio
            close(pipe_fd[0]);

            // Generazione di un numero casuale
            srand(getpid());
            int random_number = rand() % 100;

            // Comunicazione del numero casuale al genitore
            write(pipe_fd[1], &random_number, sizeof(int));

            // Chiusura del lato di scrittura della pipe nel figlio
            close(pipe_fd[1]);

            exit(EXIT_SUCCESS);
        }
    }

    // Codice eseguito dal genitore

    // Chiusura del lato di scrittura della pipe nel genitore
    close(pipe_fd[1]);

    int sum = 0;
    int received_number;

    // Lettura dei numeri inviati dai figli
    for (int i = 0; i < num_children; ++i) {
        read(pipe_fd[0], &received_number, sizeof(int));
        sum += received_number;
    }

    // Chiusura del lato di lettura della pipe nel genitore
    close(pipe_fd[0]);

    // Attesa dei figli
    for (int i = 0; i < num_children; ++i) {
        wait(NULL);
    }

    // Stampa della somma su stdout
    printf("La somma di tutti i numeri generati dai figli è: %d\n", sum);

    return 0;
}
