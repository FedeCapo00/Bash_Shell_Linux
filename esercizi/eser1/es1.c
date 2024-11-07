#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 10

int array[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int globalSum = 0;

// Struttura dati per passare i parametri al thread
typedef struct {
    int* array;
    int startIndex;
    int endIndex;
} ThreadData;

// Funzione eseguita dal thread per calcolare la somma in una parte dell'array
void* calculateSum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int localSum = 0;

    for (int i = data->startIndex; i < data->endIndex; ++i) {
        localSum += data->array[i];
    }

    // Aggiungi la somma parziale alla somma globale
    globalSum += localSum;

    // Termina il thread
    pthread_exit(NULL);
}

int main() {
   //init array con random
    srand(time(NULL));
    int r = rand()%10;

    pthread_t thread1, thread2;
    ThreadData data1, data2;

    // Imposta i parametri per il primo thread
    data1.array = array;
    data1.startIndex = 0;
    data1.endIndex = ARRAY_SIZE / 2;

    // Imposta i parametri per il secondo thread
    data2.array = array;
    data2.startIndex = ARRAY_SIZE / 2;
    data2.endIndex = ARRAY_SIZE;

    // Crea i due thread
    pthread_create(&thread1, NULL, calculateSum, (void*)&data1);
    pthread_create(&thread2, NULL, calculateSum, (void*)&data2);

    // Attendi la terminazione dei due thread
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Stampa la somma totale
    printf("La somma totale degli elementi dell'array è: %d\n", globalSum);

    return 0;
}
