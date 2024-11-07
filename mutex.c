#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock;
pthread_t tid[2];
int counter = 0;

void* thr1(void* arg){
	// Lock del semaforo
	pthread_mutex_lock(&lock);

	counter = 1;

	printf("Thread 1 has started with counter %d\n",counter);

	for (long unsigned i = 0; i < (0x00FF0000); i++);

	counter += 1;

	// Unlock del semaforo
	pthread_mutex_unlock(&lock);
	printf("Thread 1 expects 2 and has: %d\n", counter);
}

void * thr2(void* arg) {
	pthread_mutex_lock(&lock);
	counter = 10;
	printf("Thread 2 has started with counter %d\n",counter);

	for (long unsigned i = 0; i < (0xFFF0000); i++);

	counter += 1;
	pthread_mutex_unlock(&lock);
	printf("Thread 2 expects 11 and has: %d\n", counter);
}

int main(void) {
	// Init semaforo
	pthread_mutex_init(&lock, NULL);

	// Crea due threads
	pthread_create(&(tid[0]), NULL, thr1,NULL);
	pthread_create(&(tid[1]), NULL, thr2,NULL);

	// Aspetta la fine di un singolo thread
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	// Elimino il semaforo
	pthread_mutex_destroy(&lock);

	// Elimino i threads
	pthread_exit((void *)42);
}
