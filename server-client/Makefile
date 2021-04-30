CC=gcc
CFLAGS=-Wall

all: server.o client.o
	$(CC) client.o -o client
	$(CC) server.o -o server

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c 

clean:
	rm -rf *.o client server