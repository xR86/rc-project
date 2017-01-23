#Makefile for server
SHELL = /bin/bash #use bash syntax
#SILENT_EA = 'source ~/.bash_aliases && source $HOME/.bash_aliases && echo "[silent]aliases sourced --ok."'

#alias rc-server='cd /home/xr-86/httpreq/server/ ; ./server' 
#current_loc=$$(pwd) | stuff | $$current_loc
#shopt -s expand_aliases ;
# echo `${BASH_ALIASES[silent_ea]}`  sau $(SILENT_EA)
#source ~/.bash_aliases && source $$HOME/.bash_aliases && echo "[silent]aliases sourced --ok."


all: compile alias

compile: serverConc.c
	gcc serverConc.c -o server.bin

alias: 
	echo "alias rc-server='cd /home/xr-86/httpreq/server/ ; ./server.bin'" >> ~/.bash_aliases ; \
	
clean:
	unalias rc-server
