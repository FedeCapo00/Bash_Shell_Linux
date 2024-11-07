#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <limits.h>
#include <libgen.h>

#define MAX_QUEUE_SIZE 100

typedef struct {
	int front, rear;
	int array[MAX_QUEUE_SIZE];
} Queue;

Queue myQueue;
char stringToSend[128];
char *path;

void writePidFile();
void writeStringToFile();
char * extract ();
void initializeQueue(Queue *queue);
int isEmpty(Queue *queue);
int isFull(Queue *queue);
void enqueue(Queue *queue, int element);
int dequeue(Queue *queue);
int random_dequeue(Queue *queue);
int count_queue(Queue *queue);

void handler(int sigNum) {

	if (sigNum == 10) {
    		// SIGUSR1
		int child_pid = fork();
		if (child_pid == 0) {
			int num_pid = getpid();
			enqueue(&myQueue, getpid());
			char op = '+';
			sprintf(stringToSend, "%c%d\n", op, num_pid);
			writeStringToFile();
		} else if (child_pid == -1){
			perror("Fork error.\n");
			exit(EXIT_FAILURE);
		} else {
			int status;
        		if (waitpid(child_pid, &status, 0) == -1) {
            			perror("Waitpid error.\n");
            			exit(EXIT_FAILURE);
        		}
		}

    	} else if (sigNum == 12) {
		// SIGUSR2
        	int num_pid = getpid();
        	char op = '-';

		writeStringToFile();

		if (isEmpty(&myQueue)) {
			strcat(stringToSend, "0");
               		writeStringToFile();
                	exit(EXIT_SUCCESS);
		} else {
			sprintf(stringToSend, "%c%d\n", op, num_pid);
			int rand_pid = random_dequeue(&myQueue);
			if (kill(rand_pid, SIGTERM) == -1) {
        			perror("Kill error.\n");
        			exit(EXIT_FAILURE);
			}
		}

	} else if (sigNum == 2) {
		// SIGINT
		int num_fig = count_queue(&myQueue);
		sprintf(stringToSend, "%d\n", num_fig);
		writeStringToFile();
		exit(EXIT_SUCCESS);
	}
}

int main (int argc, char *argv[]) {
	initializeQueue(&myQueue);
	enqueue(&myQueue, getpid());
	path = argv[1];
	writePidFile();

	signal(SIGUSR1,handler);
	signal(SIGUSR2,handler);
	signal(SIGINT,handler);

	while(1);

	return 0;
}

void writePidFile(){
	int pf;
	pf = open(path, O_WRONLY | O_CREAT | O_APPEND);

        if (pf == -1) {
	        fprintf(stderr,"stderr: %s\n", strerror(errno));
        }
        else {
		char value[128];
		sprintf(value, "%d\n", getpid());

		write(pf, value, strlen(value));
		printf("[server:%d]\n",getpid());
              	close(pf);
        }
	return;
}

// Function to write into the file stringToSend
void writeStringToFile() {
	char *returnValue = extract();

	if (returnValue != NULL) {
    		size_t len = strlen(returnValue);
    		if (len > 0 && returnValue[len - 1] == '\n') {
	       	 	returnValue[len - 1] = '\0';
    		}

    		int pf;
    		pf = open(path, O_WRONLY | O_CREAT | O_APPEND);

    		if (pf == -1) {
        		fprintf(stderr, "stderr: %s\n", strerror(errno));
        		perror("Errore apertura file.\n");
    		} else {
        		write(pf, stringToSend, strlen(stringToSend)+1);
        		printf("[server: %s]\n", returnValue);
        		close(pf);
    		}
	}
	free(returnValue);
	return;
}

// Function to extract the specific pid
char *extract() {
	char *returnValue = NULL;
	int i = 0;

	while (stringToSend[i] != '\0' && stringToSend[i] != '\n') {
		if (stringToSend[i] == '+' || stringToSend[i] == '-') {
			int length = strlen(&stringToSend[i]);
            		returnValue = (char *)malloc((length + 1) * sizeof(char));
            		if (returnValue == NULL) {
                		perror("Memory allocation error.\n");
                		exit(EXIT_FAILURE);
            		}

	            	strcpy(returnValue, &stringToSend[++i]);
			return returnValue;
        	} else {
    	        	i++;
        	}
    	}

	return returnValue;
}

// Function to initialize the queue
void initializeQueue(Queue *queue) {
    queue->front = -1;
    queue->rear = -1;
}

// Function to check if the queue is empty
int isEmpty(Queue *queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// Function to check if the queue is full
int isFull(Queue *queue) {
    return (queue->rear + 1) % MAX_QUEUE_SIZE == queue->front;
}

// Function to enqueue an element into the queue
void enqueue(Queue *queue, int element) {
    if (isFull(queue)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }

    if (isEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    }

    queue->array[queue->rear] = element;
    //printf("Enqueued: %d\n", element);
}

// Function to dequeue an element from the queue
int dequeue(Queue *queue) {
    int element;

    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1; // Assuming -1 is not a valid element in the queue
    }

    element = queue->array[queue->front];

    if (queue->front == queue->rear) {
        // Last element in the queue, reset front and rear
        initializeQueue(queue);
    } else {
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
    }

    //printf("Dequeued: %d\n", element);
    return element;
}

// Function to dequeue a random member of the queue
int random_dequeue(Queue *queue) {
    int element;

    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1;
    }

    // Randomly choose a position to dequeue
    int randomPosition = rand() % (queue->rear - queue->front + 1) + queue->front;

    element = queue->array[randomPosition];

    // Swap the chosen element with the last element in the queue
    int temp = queue->array[randomPosition];
    queue->array[randomPosition] = queue->array[queue->rear];
    queue->array[queue->rear] = temp;

    if (queue->front == queue->rear) {
        initializeQueue(queue);
    } else {
        queue->rear = (queue->rear - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    }

    //printf("Dequeued from random position: %d\n", element);
    return element;
}

// Function calculate the number of elements in the queue
int count_queue(Queue *queue) {
    if (isEmpty(queue)) {
        return 0;
    } else {
        return (queue->rear - queue->front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE + 1;
    }
}
