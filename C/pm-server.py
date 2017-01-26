# Paramiko OpenSSH Server

# General Dependencies
import json
import sys

# Connection
import paramiko

import socket
import signal, time



terminate = False

# ssh could also be named client
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

hosts_file = open('hosts.json', 'r')
hosts = json.load( hosts_file )


''' file dump
print '---enumerating hosts: '
for host in hosts:
	print host['tag'], host['type']
	print host['address']
	print host['username']
	print host['password']
	print '---'
'''

def host_handler(hosts):
	global ssh
	print '-----sending to ' + host['tag'] + '------------'

	if host['type'] != 'password-only':
		print 'type not supported: ', host['type']
		return

	if ssh.get_transport():
		print ssh.get_transport()
		print ssh.get_transport().is_active()

	try:
		ssh.connect(
			host['address'], 
			username = host['username'], 
		    password = host['password'],
		    timeout = 5
		)

		stdin, stdout, stderr = ssh.exec_command('ls -l')
	# (BadHostKeyException, AuthenticationException, SSHException, socket.error)
	except Exception as e:
		print e

	# stdout.readlines()
	#stdin.write('check\n')
	#stdin.flush()
	data = stdout.read().splitlines()
	# print data

	for line in data:
	    if line.split(':')[0]:
	        print line

	stdout.flush()


def signal_handling(signum,frame):
	#global terminate
	#terminate = True
	print('You pressed Ctrl+C!')
	sys.exit(0)



if __name__ == '__main__':

	signal.signal(signal.SIGINT,signal_handling)

	s = socket.socket()         # Create a socket object
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

	host = '127.0.0.1' 			# if using getHostName, will not work, even with python client
	port = 2028                 # Reserve a port for your service.
	s.bind((host, port))        # Bind to the port

	s.listen(5)                 # Now wait for client connection.

	
	while True:
		print 'Accepting clients'
		c, addr = s.accept()		# Establish connection with client.
		print 'Got connection from', addr
		hosts = c.recv(1000)
		print 'hosts: ', hosts
		c.send('ACK')


		for host in json.loads(hosts):
			host_handler(host)
		c.close()					# Close the connection
		if terminate:
			print "Graceful shutdown ..."
			break

	hosts_file.close()