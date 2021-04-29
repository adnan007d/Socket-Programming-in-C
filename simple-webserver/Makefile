CC=gcc
CFLAGS=-Wall

all: server.o
	$(CC) $(CFLAGS) server.o -o server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

clean:
	rm -rf *.o server
