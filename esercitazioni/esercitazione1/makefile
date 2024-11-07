# Define variables
CC = gcc
STD = gnu90
CFLAGS = -Wall -g
CFILES = main.c
OBJECT = main.o
name ?= program

# Default target
.DEFAULT_GOAL := $(name)

$(name): $(OBJECT)
	$(CC) -o $(name) $(OBJECT)

# Define targets and their dependencies
%.o: %.c
	$(CC) -c -o $@ $<

# Target for cleaning generated files
clean:
	rm -rf $(OBJECT) $(name) 
	clear
