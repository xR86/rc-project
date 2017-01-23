all: clean client server

client:
	gcc cliTCPConc.c -o client.bin
server:
	gcc servTCPConc.c -o server.bin
clean:
	rm -f client.bin server.bin