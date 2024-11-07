CC = gcc
STD = gnu90
CFLAGS = -Wall -g
CFILES = server.c client.c
OBJECTS = server.o client.o
BINARY_SERVER = server
BINARY_CLIENT = client
BINARY = app

all: $(BINARY_SERVER) $(BINARY_CLIENT) $(BINARY)

$(BINARY): app.o
	$(CC) -o $@ $^

$(BINARY_SERVER): server.o
	$(CC) -o $@ $^

$(BINARY_CLIENT): client.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $<

clean:
	rm -rf $(BINARY_SERVER) $(BINARY_CLIENT) $(OBJECTS) app
