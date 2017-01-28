# Paramiko OpenSSH Server

# General Dependencies
import json
import sys, os

# Connection
import paramiko

import socket
import signal

# Logging
import logging
from logging.handlers import RotatingFileHandler
import time as tm


# terminate = False #variable for SIGINT handler

# ssh could also be named client
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

hosts_file = open('hosts.json', 'r')
hosts = json.load( hosts_file )




# Function to configure logging format
def configure_logging():
	log = logging.getLogger('')
	log.setLevel(logging.DEBUG)
	format = logging.Formatter("%(asctime)s |%(name)s| %(levelname)s | >> %(message)s")

	ch = logging.StreamHandler(sys.stdout)
	ch.setFormatter(format)
	log.addHandler(ch)

	if not os.path.exists('logs/'):
		os.makedirs('logs/')
	time = tm.strftime('%Y-%m-%d-%H_%M_%S', tm.localtime( tm.time() ))
	fh = RotatingFileHandler('logs/log-%s.log' % time, maxBytes=(1048576*5), backupCount=7)
	fh.setFormatter(format)
	log.addHandler(fh)

def host_handler(hosts):
	global ssh
	print '-----sending to ' + host['tag'] + '------------'

	if host['type'] != 'password-only':
		print 'type not supported: ', host['type']
		return

	if ssh.get_transport():
		# print ssh.get_transport()
		print ssh.get_transport().is_active()

	try:
		ssh.connect(
			host['address'], 
			username = host['username'], 
		    password = host['password'],
		    timeout = 5
		)

		sys.stdin, sys.stdout, sys.stderr = ssh.exec_command('ls -l')
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
	configure_logging()

	logging.info('started server')

	signal.signal(signal.SIGINT,signal_handling)
	logging.info('initialized SIGINT handler') #TODO: put in else (of try-except)

	s = socket.socket()         # Create a socket object
	s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

	host = '127.0.0.1' 			# if using getHostName, will not work, even with python client
	port = 2028                 # Reserve a port for your service.
	s.bind((host, port))        # Bind to the port

	s.listen(5)                 # Now wait for client connection.

	
	while True:
		logging.info('Accepting clients')
		c, addr = s.accept()		# Establish connection with client.
		logging.info('Got connection from %s %s' % addr)
		hosts = c.recv(1000)
		logging.info('hosts file: %s' % hosts)
		c.send('ACK')


		for host in json.loads(hosts):
			''' file dump
			print '---enumerating hosts: '
			for host in hosts:
				print host['tag'], host['type']
				print host['address']
				print host['username']
				print host['password']
				print '---'
			'''
			host_handler(host)
		c.close()					# Close the connection
		if terminate:
			logging.info('Graceful shutdown ...')
			break

	hosts_file.close()