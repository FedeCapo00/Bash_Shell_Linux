#ifndef _LIBRERIAPROGRAM_H
#define _LIBRERIAPROGRAM_H
//************************************************************************//
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
//************************************************************************//
#define DIM_BUFFER_LONG 1024
#define DIM_BUFFER_SHORT 64
#define TRUE 1
#define FALSE 0
// Lunghezza massima gestita dei percorsi nel file-system
#define PATH_MAXLEN 60
// Numero massimo di figli gestibili
#define CHILDREN_MAX 10
// Lunghezza massima di stringa che rappresenti un pid
#define PIDLEN 8
// Nome cartella da creare
#define DIR_NAME "INFO"
// Nome file di testo da creare
#define FILE_NAME "key.txt"
//************************************************************************//
// PID processo principale
extern pid_t FATHER_PID;
// Struttura per Messagge Queue
struct mesg_buffer{ 
    long m_type;
    char m_text[DIM_BUFFER_LONG];
};
//************************************************************************//
// Funzione che controlla il path passato come parametro esiste o meno
int checkDirectory(char *);
// Funzione che genera la message queue, scrive il PID del processo padre
int generaMSG_Queue(char *, int);
// Funzione che stampa il contenuto della message queue
void stampaMessageQueue(char *, int);
// Funzione per la stampa e gestione degli errori
void printErrorCode(int *, int, char*, char*);
//************************************************************************//
#endif