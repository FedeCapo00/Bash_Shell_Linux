#-*- Makefile -*-
CC = gcc
INCDIRS = -I
OPT = O1
CFLAGS = -Wall -g $(INCDIRS) $(OPT)
CFILES = program.c generaDaemonFile.c
OBJECTS = program.o generaDaemonFile.o
BINARY_PROGRAM = program
BINARY_DAEMON = daemon
BINARY = app

all: $(BINARY_PROGRAM) $(BINARY_DAEMON)

$(BINARY_PROGRAM): program.o
	$(CC) -o $@ $^

$(BINARY_DAEMON): generaDaemonFile.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -rf $(BINARY_PROGRAM) $(BINARY_DAEMON) $(OBJECTS)
	rm -rf tmp/INFO/ *.txt
