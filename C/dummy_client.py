#!/usr/bin/python           # This is client.py file

import socket               # Import socket module
import sys

s = socket.socket()         # Create a socket object


host = '127.0.0.1'
port = 2028

s.connect((host, port))
#name = raw_input('[client]Test: ')

f = open('../A/hosts.json', 'r')
print 'Sending hosts.json'
data = f.read()

s.send(data)

print "[client]Mesajul primit este: %s\n"% s.recv(1024)
s.close                     # Close the socket when done