import paramiko

import json

# ssh could also be named client
ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

#hosts = ['127.0.0.1', '192.168.100.7']

hosts_file = open('hosts.json', 'r')
hosts = json.load( hosts_file )


''' file dump
print '---hosts types available:'
for host in hosts:
	print host

print '---enumerating hosts: '
for host in hosts["password-only"]:
	# print host
	print 'tag', host['tag']
	print 'address', host['address']
	print 'username', host['username']
	print 'password', host['password']
'''


for host in hosts["password-only"]:
	print '-----sending to ' + host['tag'] + '------------'

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