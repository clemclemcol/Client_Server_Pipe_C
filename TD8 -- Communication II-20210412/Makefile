CC=gcc

.PHONY: all

all: client server

server: server.c
	$(CC) -o $@ $^

client: client.c
	$(CC) -o $@ $^
