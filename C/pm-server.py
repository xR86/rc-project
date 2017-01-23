import paramiko

import json
import sys

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

for host in hosts:
	print '-----sending to ' + host['tag'] + '------------'

	if host['type'] != 'password-only':
		print 'type not supported: ', host['type']
		continue

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


	# type(stdin)
	# stdout.readlines()

	#stdin.write('check\n')
	#stdin.flush()
	data = stdout.read().splitlines()
	# print data

	for line in data:
	    if line.split(':')[0]:
	        print line

	stdout.flush()


hosts_file.close()