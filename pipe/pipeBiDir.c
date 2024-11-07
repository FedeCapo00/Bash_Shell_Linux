#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ 0
#define WRITE 1

int main() {
    char cont = '3';
    int pipe1[2], pipe2[2];
    char buf[50];

    // Create two unnamed pipes
    pipe(pipe1);
    pipe(pipe2);

    int p2 = !fork();
    if (p2) {
        close(pipe1[WRITE]);
        close(pipe2[READ]);

        // Read from pipe
        int r = read(pipe1[READ], &buf, 50);
        close(pipe1[READ]);

        printf("P2 received: '%c'\n", cont);

        cont = '2';

        // Write to pipe
        write(pipe2[WRITE], &cont, 1);
        close(pipe2[WRITE]);
    } else {
        close(pipe1[READ]);
        close(pipe2[WRITE]);

        cont = '1';

        // Write to pipe
        write(pipe1[WRITE], &cont, 1);
        close(pipe1[WRITE]);

        // Read from pipe
        int r = read(pipe2[READ], &buf, 50);
        close(pipe2[READ]);
        printf("P1 received: '%c'\n", buf[0]);
    }
    while (wait(NULL) > 0);

    return 0;
}
