all: clean client server
# sudo apt-get install libjson-c-dev

CFLAGS += $(shell pkg-config --cflags json-c) #-I/usr/include/json-c
LDFLAGS += $(shell pkg-config --libs json-c)

# initially: gcc client.c -o client -I=/usr/include/json-c --sysroot=/

client:
	gcc client.c -o client.bin $(CFLAGS) $(LDFLAGS)
server:
	gcc server.c -o server.bin $(CFLAGS) $(LDFLAGS)

clean:
	rm -f client.bin server.bin temp.json