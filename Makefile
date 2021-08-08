all: client server

client:
	gcc client.c -o client

server:
	gcc server.c -o server

clean:
	rm -f client server