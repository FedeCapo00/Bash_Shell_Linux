#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void * my_fun(void * param){

	// Per aspettare senza usare la sleep()
	//for (long unsigned i = 0; i < (0x9FFF0000); i++);

	printf("This is a thread that received %d\n", *(int *)param);
	return (void *)3;
}

int main(void){

	pthread_t t_id1;
	pthread_t t_id2;
	pthread_t t_id3;
	pthread_t t_id4;

	int arg = 10;

	pthread_create(&t_id1, NULL, my_fun, (void *)&arg);
	printf("Executed thread with id %ld\n",t_id1);
	pthread_join(t_id1, NULL);
//	pthread_cancel(t_id1);

	// Modifico arg (se non fosse stato cancellato, avrebbe lo stesso valore per tutti i threads)
	arg = 20;
	pthread_create(&t_id2, NULL, my_fun, (void *)&arg);
	printf("Executed thread with id %ld\n",t_id2);
//	pthread_cancel(t_id2);

	sleep(1);

	// Join tra threads
	arg = 30;
	pthread_create(&t_id3, NULL, my_fun, (void *)&arg);

	printf("Executed thread with id %ld\n", (long)t_id3);
	pthread_join(t_id3, NULL);
    	arg = 40;

    	pthread_create(&t_id4, NULL, my_fun, (void *)&arg);
   	printf("Executed thread with id %ld\n", (long)t_id4);

    	// Attendere il completamento del thread t_id2 prima di terminare il thread principale
    	pthread_join(t_id4, NULL);

	if (pthread_equal(t_id3,t_id4))
		printf("Equal: %ld - %ld \n",t_id3,t_id4);
	else
		printf("t_id3: %ld \n t_id4: %ld",t_id3,t_id4);

	// Termino TUTTI i threads attivi
	pthread_exit((void *)42);

	return 0;
}
