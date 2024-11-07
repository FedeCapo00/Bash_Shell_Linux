#-*- Makefile -*-
# Compilatore
CC = gcc

# Parametri di ottimizzazione
INCDIRS = -I
OPT = -O1

# Opzioni di compilazione
CFLAGS = -Wall -g $(INCDIRS) $(OPT)

# Nomi dei file sorgenti programma
APP_SOURCE_FILES = program.c libreriaProgram.c

# Nome dell'eseguibile
OUTPUT = app

# Nomi dei file sorgenti daemon
DAEMON_SOURCE_FILES = generaDaemonFile.c libreriaDaemon.c
# Nome dell'eseguibile
DAEMON = daemon

all: $(OUTPUT) $(DAEMON)

$(OUTPUT): $(APP_SOURCE_FILES:.c=.o)
	$(CC) $^ -o $@

$(DAEMON): $(DAEMON_SOURCE_FILES:.c=.o)
	$(CC) $^ -o $@

%.o: %.c libreriaProgram.h libreriaDaemon.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OUTPUT) $(DAEMON) *.o
	rm -rf tmp/INFO/ *.txt