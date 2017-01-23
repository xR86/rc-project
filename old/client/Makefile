#Makefile for client
SHELL = /bin/bash #use bash syntax
#SILENT_EA = 'source ~/.bash_aliases && source $HOME/.bash_aliases && echo "[silent]aliases sourced --ok."'

#alias rc-client='cd /home/xr-86/httpreq/client/ ; ./client' 
#current_loc=$$(pwd) | stuff | $$current_loc
#shopt -s expand_aliases ;
# echo `${BASH_ALIASES[silent_ea]}`  sau $(SILENT_EA)
#source ~/.bash_aliases && source $$HOME/.bash_aliases && echo "[silent]aliases sourced --ok."


all: compile alias

compile: client.c
	gcc client.c -o client.bin

alias: 
	echo "alias rc-client='cd /home/xr-86/httpreq/client/ ; ./client.bin'" >> ~/.bash_aliases ; \

clean:
	unalias rc-client
