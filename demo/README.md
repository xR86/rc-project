# rcproj
Computer Networks Project

### Dockerfile / Vagrantfile

sudo docker build -t "test" .
sudo docker run -p 8081:8081 -d "test"

sudo docker build -t test .
sudo docker run -name test1 -i -t test

sudo docker pull salttest/ubuntu-12.04


----

`sudo apt-get install -y openssh-client openssh-server`

`sudo docker images`
`sudo docker run -i -t salttest/ubuntu-12.04 /bin/bash` - ubuntu:12.04 e shitty


`sudo docker inspect romantic_hoover | grep "IPAddress"`
`cat /etc/*release` - extra check


### Folder structure:
+ `client-htmlpy/` - htmlpy example for client
+ `extending/` - extending C with Python
+ `libssh/` - using a openssh-compliant library for C
  + follow install guide here: [official](https://github.com/substack/libssh/blob/master/INSTALL#L26-L29), [issue](https://askubuntu.com/questions/741351/trying-to-install-libssh-0-7-x-on-ubuntu)
  + tutorial [here](http://api.libssh.org/master/libssh_tutor_guided_tour.html)
  + configure openssh-server for client machines ([here](https://askubuntu.com/questions/51925/how-do-i-configure-a-new-ubuntu-installation-to-accept-ssh-connections), [extended](https://thepcspy.com/read/making-ssh-secure/)):
    + `sudo apt-get update`
	+ `sudo apt-get install openssh-server` (implicitly installs `openssh-client`, if **not** add it to the command)
	+ `sudo ufw allow 22`
	+ configure - [quick](https://help.ubuntu.com/14.04/serverguide/openssh-server.html), [detailed](https://help.ubuntu.com/community/SSH/OpenSSH/Configuring) (you can check the files location with `dpkg -L libssh-dev`)
	+ `ssh-keygen -t rsa`
	+ `ssh-copy-id remoteuser@remotehostIP` - will add the id to known hosts
  + `/etc/ssh/sshd_config/`
  + `sudo service ssh restart`
  + `paramiko-demo.py`:
  + install deps first ( [source](https://stackoverflow.com/questions/31569339/importerror-no-module-named-cryptography-hazmat-backends-boxsdk-on-mac) ):
    + `sudo apt-get install libffi-dev` ([what is that ?](https://sourceware.org/libffi/); needed for crypto package below)
    + `sudo pip install cryptography` (needed for `paramiko` package)
  + `pip install paramiko`

### Useful: 
+ [github.com/msanders/wol.c](https://github.com/msanders/wol.c/blob/master/wol.c)
+ [libssh tutorial](http://api.libssh.org/master/libssh_tutorial.html), [libssh + public key](http://api.libssh.org/master/libssh_tutor_authentication.html)
+ [api.libssh.org/master/group__libssh__auth](http://api.libssh.org/master/group__libssh__auth.html)
+ [PyQt standard icons](http://nukesaq88.hatenablog.com/entry/2013/04/12/005525)
+ Recursion in makefiles - [Recursive Make Considered Harmful](http://aegis.sourceforge.net/auug97.pdf)
+ file types comparison - [What is the equivalent of an “exe file”?](http://askubuntu.com/a/174356) - should use .bin, [.elf](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format), [a.out](https://en.wikipedia.org/wiki/A.out)