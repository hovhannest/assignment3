all: server client

server:
	cc server.c -o server

client:
	cc client.c -o client

run: 
	./server 2345 &
	./client 127.0.0.1 2345