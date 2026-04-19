CC=gcc
CFLAGS=-Wall -pthread

all: server client

server:
	$(CC) $(CFLAGS) server/server.c -o server/server

client:
	$(CC) $(CFLAGS) client/client.c -o client/client

clean:
	rm -f server/server client/client
