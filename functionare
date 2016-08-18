TODO0.0. install script bash_aliases & other stuff ...
0.1. makefile from client && server => make all
0.2. compile bash_aliases client && server => silent_ea
0.3. clean old alias: unalias rc-server ; unalias rc-client

1. [server] open: rc-server
	-> parameter allowed: --verbose
2. [server] checks if openssl is installed (check can be done either with running "openssl", running with --version parameter or with "which"):
	-> if not, install: apt-get install openssl
		-> handle exception if it can't install
	-> if yes, execute: openssl genrsa -out key-priv.txt 2048
		-> handle exception if it doesn't generate the private key

3. [server] create public key: openssl rsa -in key-priv.txt -pubout >key.pub
	-> handle exception if it doesn't generate the public key

4. [client] open: rc-client 127.0.0.1 <port>
5. [client] repeat steps 2 and 3 for client
6. key exchange, through socket: (registration) -> problem of sending key.pub of the server to the client (server message is ok, client receives (part of) personal public key - that was sent previously through sd -> flush should be made for socket ???)
	-> brute, for testing purposes: 
		- client: cp ../server/key.pub key-server.pub
		- server: cp ../client/key.pub key-client.pub
	-> how will client public keys be saved on the server ? (name policy -> fragment of MAC/IP appended to the key name)

7. [server] encryption of the message to send through: echo "download options: ..." | openssl rsautl -pubin -inkey key-client.pub -encrypt >fisier.bin

8. [server] send through socket to client
	-> brute, for testing purposes: cp ../server/fisier.bin fisier-server.bin
9. [client] decrypt: openssl rsautl -inkey key-priv.txt -decrypt <fisier-client.bin #output redirect isn't really necessary ? : >fisier-decriptat.txt
10. [client] choose download option, encrypt and send to server, server will send prompts to the client:
	option (wget or get): __
	depth(wget only)(not 0, 1-5): __
	files-only(Y/N): ___

wgetTODO11. search stackoverflow article with possible solutions, pass to execlp previously mentioned commands

GET-TODO11. parse for <a href="stuff">
		-> try to mimic depth and files-only from wget through GET


15? - make qt-webkit interface
