#ifndef _LIBRERIADAEMON_H
#define _LIBRERIADAEMON_H
//************************************************************************************//
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
//************************************************************************************//
// Esporto le variabili così che siano accessibili dai file .c
// Nome del file di testo associato al processo daemon [PID].txt
extern char filename[20];
// Path dove è situata la message queue
extern char pathAssolutoIPC[100];
// Numero da passare per la funzione ftok()
extern int num;
// Struttura dati per Message Queue
struct mesg_buffer {
    long m_type;
    char m_text[1024];
};
//************************************************************************************//
// Funzione che scrive SIGUSER1 nel file associato al processo
void writePidFile();
// Funzione che scrive PID nella messageQueue
void writeQueue();
// Funzioni di gestione per SIGNAL SIGUSR1
void sigusr1_handler();
// Funzioni di gestione per SIGNAL SIGUSR2
void sigusr2_handler();
//************************************************************************************//
#endif
