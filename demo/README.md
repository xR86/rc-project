# rcproj
Computer Networks Project


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

### Useful: 
+ [github.com/msanders/wol.c](https://github.com/msanders/wol.c/blob/master/wol.c)
+ [libssh tutorial](http://api.libssh.org/master/libssh_tutorial.html), [libssh + public key](http://api.libssh.org/master/libssh_tutor_authentication.html)
+ [api.libssh.org/master/group__libssh__auth](http://api.libssh.org/master/group__libssh__auth.html)