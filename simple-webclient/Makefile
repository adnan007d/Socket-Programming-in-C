CC=gcc
CFLAGS=-Wall

all:client

client: tcp_client.o
	$(CC) tcp_client.o -o client

tcp_client.o: tcp_client.c
	$(CC) $(CFLAGS) -c tcp_client.c 

clean:
	rm -rf *.o client