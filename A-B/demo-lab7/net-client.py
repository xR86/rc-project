#!/usr/bin/python           # This is client.py file

import socket               # Import socket module
import sys

s = socket.socket()         # Create a socket object
# host = socket.gethostname() # Get local machine name
# port = 12345                # Reserve a port for your service.

if len(sys.argv) != 3:
	print 'Sintaxa: %s <adresa_server> <port>\n' % sys.argv[0]
	sys.exit()

host = sys.argv[1]
port = int(sys.argv[2])

s.connect((host, port))
name = raw_input('[client]Introduceti un nume: ')
s.send(name)

print "[client]Mesajul primit este: %s\n"% s.recv(1024)
s.close                     # Close the socket when done