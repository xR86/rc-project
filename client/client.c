/* cliTCP.c - Exemplu de client TCP 
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009

*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h> /*pt open*/
#include <fcntl.h> /*pt open*/

/*ANSI colours*/
#include "../api/colours.h"

/*file utilities*/
#include "../api/utils.h"

/*extra messages - flag verbose at server startup with "--verbose"*/
int verbose = 0;

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;




/*generate RSA private key for securing transaction*/
int gen_privkey(){
  pid_t pid;	/* PID-ul procesului copil */
  int status;	/* starea de terminare a procesului copil */

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      return 0;/*not ok - exit(1)*/
    }
  else if (pid)			/* parinte */
    {
      if (wait (&status) < 0)
	{
	  perror ("wait()");
	}
      FILE * checkfile;
      checkfile = fopen("key-priv.txt","r");
      if(isEmpty(checkfile)){ return 0; }
	if(verbose==1){
      	   printf (ANSI_CYAN "Comanda a fost executata.\n" ANSI_RESET);
	}
      return 1;/*ok - exit(0)*/
    }
  else	/* fiu */
    {
      execlp ("openssl",
	      /* comanda de executat (se va cauta in directoarele din PATH) */
	      "openssl",		/* argv[0] */
	      "genrsa",	/* argv[1]*/
	      "-out",	/* argv[2]*/
	      "key-priv.txt",/* argv[3]*/
	      "2048",	/* argv[4]*/
	      NULL);
      /* daca ajungem aici inseamna ca nu s-a putut executa */
      if(verbose==1){
      	printf (ANSI_CYAN "Eroare de executie!\n" ANSI_RESET);
      }
      return 0;/*not ok - exit(1)*/
    }
}

/*generate RSA public key for securing transaction*/
int gen_pubkey(){
  pid_t pid;	/* PID-ul procesului copil */
  int status;	/* starea de terminare a procesului copil */

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      return 0;/*not ok - exit(1)*/
    }
  else if (pid)			/* parinte */
    {
      if (wait (&status) < 0)
	{
	  perror ("wait()");
	}
	system("sed -i '/writing RSA key/d' ./pub_checker_file ; cp pub_checker_file key-cl.pub; rm pub_checker_file;");
      	FILE * checkfile;
      	checkfile = fopen("key-cl.pub","r");
      	if(isEmpty(checkfile)){ return 0; }
	  if(verbose==1){
      	   printf (ANSI_CYAN "Comanda a fost executata.\n" ANSI_RESET);
	}
      return 1;/*ok - exit(0)*/
    }
  else	/* fiu */
    {
	int fd = open("pub_checker_file", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	dup2(fd, 1);
	close(fd);
      execlp ("openssl",
	      /* comanda de executat (se va cauta in directoarele din PATH) */
	      "openssl",		/* argv[0] */
	      "rsa",	/* argv[1]*/
	      "-in",	/* argv[2]*/
	      "key-priv.txt",/* argv[3]*/
	      "-pubout", /* argv[4] -  > key.pub not working*/
	      NULL);
      /* daca ajungem aici inseamna ca nu s-a putut executa */
      if(verbose==1){
      	printf (ANSI_CYAN "Eroare de executie!\n" ANSI_RESET);
      }
      return 0;/*not ok - exit(1)*/
    }
}

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 

  /* exista toate argumentele in linia de comanda? */
  if (argc == 4){
	if(strcmp(argv[3], "--verbose") == 0){
		verbose = 1;
	}
	else{ 
		printf("Doar --verbose poate fi admis ca ca a treia optiune");	
		return -1;
	}
  }
  else if (argc != 3)
    {
      printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }


/*check openssl installed*/
int FLG_oSSL = check_openssl();
if(FLG_oSSL==1){
	if(verbose == 1){
   		printf (ANSI_CYAN "[server] openssl este instalat\n" ANSI_RESET);
	}
}
else if(FLG_oSSL==0){
	perror (ANSI_RED "[server] openssl nu este instalat.\n" ANSI_RESET);
	printf(ANSI_RED "[server] Doriti sa instalati acum ? (Y/N): " ANSI_RESET);
	char check;
	scanf("%s", check);
	if(strcmp(check, "Y")==0 || strcmp(check, "y")==0){
		printf(ANSI_RED "[server] rulati sudo apt-get install openssl\n" ANSI_RESET);
		exit(0);
	}
	else if(strcmp(check, "N")==0 || strcmp(check, "n")==0){
		exit(0);
	}
	else{
		printf("[server] raspuns instalare inexistent\n");	
	}
	exit(0);
}
else{
	perror (ANSI_RED "[server] check_openssl returneaza valoare inexistenta.\n" ANSI_RESET);
	exit(0);
}

/*bloc cheie privata*/
int FLG_privKEY = gen_privkey();
if(FLG_privKEY==1){
	if(verbose == 1){
   		printf (ANSI_CYAN "[server] s-a generat cheia privata openssl RSA\n" ANSI_RESET);
	}
}
else if(FLG_privKEY==0){
	printf(ANSI_RED "[server] generare cheie privata ESUATA " ANSI_RESET);
	exit(0);
}
else{
	printf(ANSI_RED "[server] generare cheie privata  returneaza valoare inexistenta.\n" ANSI_RESET);
	exit(0);
}

/*bloc cheie publica*/
int FLG_pubKEY = gen_pubkey();
if(FLG_pubKEY==1){
	if(verbose == 1){
   		printf (ANSI_CYAN "[server] s-a generat cheia publica openssl RSA\n" ANSI_RESET);
	}
}
else if(FLG_pubKEY==0){
	printf(ANSI_RED "[server] generare cheie publica ESUATA " ANSI_RESET);
	exit(0);
}
else{
	printf(ANSI_RED "[server] generare cheie publica returneaza valoare inexistenta.\n" ANSI_RESET);
	exit(0);
}

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] Eroare la socket().\n");
      return errno;
    }
  

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
	

   char msg[700];// mesajul trimis - o cheie publica (la una privata de 2048 -> 1700) va fi de 451

  /****** schimbul de chei */
  bzero (msg, 700);
  printf ("[client] se realizeaza schimbul de chei\n ");
  int send_fd = open("key.pub", O_RDWR, 0666);
  read (send_fd, msg, 700);
  close(send_fd);
  /* trimiterea mesajului la server */
  if (write (sd, msg, 700) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
/* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, msg, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  /* afisam mesajul primit */
  printf ("[client]Mesajul primit este: %s\n", msg);

  if (accept_PK(sd)){
	printf("[client] client -> cheie publica primita \n");
  }

  /* inchidem conexiunea, am terminat */
  close (sd);
}

int accept_PK(int fd)
{
  char buffer[700];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */
  char msg[700];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client
printf ("[server]Mesajul din cahce...%s\n", msg);
  bzero (msg, sizeof (msg));
  bytes = read (fd, msg, sizeof (buffer));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  printf ("[server]Mesajul a fost receptionat...%s\n", msg);

  /*se creeaza un fisier unic in care se stocheaza cheia publica a unui client*/
  char temp[20] = "server_pk";

  FILE * pkFile;
  pkFile = fopen(temp, "w");
  fprintf(pkFile, "%s", msg);
  fclose(pkFile);

  /*pregatim mesajul de raspuns */
  bzero(msgrasp,100);
  strcat(msgrasp,"client -> cheie publica primita");
  printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
      
  if (bytes && write (fd, msgrasp, bytes) < 0)
    {
      perror ("[server] Eroare la write() catre client.\n");
      return 0;
    }
  
  return bytes;
}

