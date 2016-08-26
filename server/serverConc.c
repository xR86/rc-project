/* concurrent TCP server
   
   Initial server implementation from Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
    (based on [Retele de Calculatoare,S.Buraga & G.Ciobanu, 2003])

   Initial purpose: Server waits for a "name" from multiple clients and returns to clients
   "Hello <<name>>" accordingly; multiplexing made with select().
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h> /*for open*/
#include <fcntl.h> /*for open*/

/*ANSI colours*/
#include "../api/colours.h"

/*file utilities*/
#include "../api/utils.h"

/*extra messages - flag verbose at server startup with "--verbose"*/
int verbose = 0;

/*CLcounter = client counter*/
int CLcounter = 23;

/*Buffer size pt sendfile*/
#define BUFSIZ 3000


/* used port for server */
#define PORT 2728

extern int errno;	/* returned error by some calls */

/* function that converts the client IP address in char array */
char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* client IP */
  strcpy (str, inet_ntoa (address.sin_addr));	
  /* client port */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}




/*generate RSA private key for securing transaction*/
int gen_privkey(){
  pid_t pid;	/* child process PID */
  int status;	/* child termination status */

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      return 0;/*not ok - exit(1)*/
    }
  else if (pid)			/* parent process */
    {
      if (wait (&status) < 0)
	{
	  perror ("wait()");
	}
      FILE * checkfile;
      checkfile = fopen("key-priv.txt","r");
      if(isEmpty(checkfile)){ return 0; }
	if(verbose==1){
      	   printf (ANSI_CYAN "Command has been executed.\n" ANSI_RESET);
	}
      return 1;/*ok - exit(0)*/
    }
  else	/* child process */
    {
      execlp ("openssl",
	      /* command to execute (will search in PATH folders) */
	      "openssl",		/* argv[0] */
	      "genrsa",	/* argv[1]*/
	      "-out",	/* argv[2]*/
	      "key-priv.txt",/* argv[3]*/
	      "2048",	/* argv[4]*/
	      NULL);
      /* if we get here, execlp failed */
      if(verbose==1){
      	printf (ANSI_CYAN "Execution error!\n" ANSI_RESET);
      }
      return 0;/*not ok - exit(1)*/
    }
}

/*generate RSA public key for securing transaction*/
int gen_pubkey(){
  pid_t pid;	/* child process PID */
  int status;	/* child termination status */

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      return 0;/*not ok - exit(1)*/
    }
  else if (pid)			/* parent process */
    {
      if (wait (&status) < 0)
	{
	  perror ("wait()");
	}
	system("sed -i '/writing RSA key/d' ./pub_checker_file ; cp pub_checker_file key.pub; rm pub_checker_file;");
      	FILE * checkfile;
      	checkfile = fopen("key.pub","r");
      	if(isEmpty(checkfile)){ return 0; }
	  if(verbose==1){
      	   printf (ANSI_CYAN "Command has been executed.\n" ANSI_RESET);
	}
      return 1;/*ok - exit(0)*/
    }
  else	/* child process */
    {
	int fd = open("pub_checker_file", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	dup2(fd, 1);
	close(fd);
      execlp ("openssl",
	      /* command to execute (will search in PATH folders) */
	      "openssl",		/* argv[0] */
	      "rsa",	/* argv[1]*/
	      "-in",	/* argv[2]*/
	      "key-priv.txt",/* argv[3]*/
	      "-pubout", /* argv[4] -  > key.pub not working*/
	      NULL);
      /* if we get here, execlp failed */
      if(verbose==1){
      	printf (ANSI_CYAN "Execution error!\n" ANSI_RESET);
      }
      return 0;/*not ok - exit(1)*/
    }
}

/* main body */
int
main (int argc, char *argv[])
{
  struct sockaddr_in server;	/* server and client structure */
  struct sockaddr_in from;
  fd_set readfds;		/* read file descriptors set */
  fd_set actfds;		/* active file descriptors set */
  struct timeval tv;		/* time structure for select() */
  int sd, client, wsd;		/* socket descriptors */
  int optval=1; 			/* option used for setsockopt()*/ 
  int fd;			/* descriptor used for 
				   iterating descriptor lists */
  int nfds;			/* maximum number of descriptors */
  int len;			/* length of sockaddr_in structure */

if (argc == 2){
	if(strcmp(argv[1], "--verbose") == 0){
		verbose = 1;
	}
}

/*check openssl installed*/
int FLG_oSSL = check_openssl();
if(FLG_oSSL==1){
	if(verbose == 1){
   		printf (ANSI_CYAN "[server] openssl is installed\n" ANSI_RESET);
	}
}
else if(FLG_oSSL==0){
	perror (ANSI_RED "[server] openssl is not installed\n" ANSI_RESET);
	printf(ANSI_RED "[server] Do you wish to install it now ? (Y/N): " ANSI_RESET);
	char check;
	scanf("%s", check);
	if(strcmp(check, "Y")==0 || strcmp(check, "y")==0){
		printf(ANSI_RED "[server] run sudo apt-get install openssl\n" ANSI_RESET);
		exit(0);
	}
	else if(strcmp(check, "N")==0 || strcmp(check, "n")==0){
		exit(0);
	}
	else{
		printf("[server] invalid installation response\n");	
	}
	exit(0);
}
else{
	perror (ANSI_RED "[server] check_openssl returns invalid value.\n" ANSI_RESET);
	exit(0);
}

/*private key body*/
int FLG_privKEY = gen_privkey();
if(FLG_privKEY==1){
	if(verbose == 1){
   		printf (ANSI_CYAN "[server] private openssl RSA key has been generated\n" ANSI_RESET);
	}
}
else if(FLG_privKEY==0){
	printf(ANSI_RED "[server] private key generation failed" ANSI_RESET);
	exit(0);
}
else{
	printf(ANSI_RED "[server] private key generation returns invalid response\n" ANSI_RESET);
	exit(0);
}

/*public key body*/
int FLG_pubKEY = gen_pubkey();
if(FLG_pubKEY==1){
	if(verbose == 1){
   		printf (ANSI_CYAN "[server] public openssl RSA key has been generated\n" ANSI_RESET);
	}
}
else if(FLG_pubKEY==0){
	printf(ANSI_RED "[server] public key generation failed" ANSI_RESET);
	exit(0);
}
else{
	printf(ANSI_RED "[server] public key generation returns invalid response\n" ANSI_RESET);
	exit(0);
}


  /* create socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] socket() error.\n");
      return errno;
    }
  /*create download socket*/
  if ((wsd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] socket() error.\n");
      return errno;
    }

  /*set socket option SO_REUSEADDR */ 
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  /* prepare data structure */
  bzero (&server, sizeof (server));

  /* set server structure */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  /* socket attach */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] bind() error\n");
      return errno;
    }

  /* server listens for clients */
  if (listen (sd, 5) == -1)
    {
      perror ("[server] listen() error\n");
      return errno;
    }
  
  /* set file descriptor set */
  FD_ZERO (&actfds);		/* initially, set is empty */
  FD_SET (sd, &actfds);		/* we include the newly-created socket in the set */

  tv.tv_sec = 1;		/* 1s wait time. */
  tv.tv_usec = 0;
  
  /* valoarea maxima a descriptorilor folositi */
  nfds = sd;

  printf ("[server] Asteptam la portul %d...\n", PORT);
  fflush (stdout);
        
  /* servim in mod concurent clientii... */
  while (1)
  {
    /* ajustam multimea descriptorilor activi (efectiv utilizati) */
    bcopy ((char *) &actfds, (char *) &readfds, sizeof (readfds));

    /* apelul select() */
    if (select (nfds+1, &readfds, NULL, NULL, &tv) < 0)
  	{
  	  perror ("[server] Eroare la select().\n");
  	  return errno;
  	}
    /* vedem daca e pregatit socketul pentru a-i accepta pe clienti */
    if (FD_ISSET (sd, &readfds))
  	{
  	  /* pregatirea structurii client */
  	  len = sizeof (from);
  	  bzero (&from, sizeof (from));

  	  /* a venit un client, acceptam conexiunea */
  	  client = accept (sd, (struct sockaddr *) &from, &len);

  	  /* eroare la acceptarea conexiunii de la un client */
  	  if (client < 0)
  	  {
	      perror ("[server] Eroare la accept().\n");
	      continue;
  	  }

      if (nfds < client) /* ajusteaza valoarea maximului */
        nfds = client;
              
  	  /* includem in lista de descriptori activi si acest socket */
  	  FD_SET (client, &actfds);

  	  printf("[server] S-a conectat clientul cu descriptorul %d, de la adresa %s.\n",client, conv_addr (from));
  	  fflush (stdout);
  	}

    /* vedem daca e pregatit vreun socket client pentru a trimite raspunsul */
    for (fd = 0; fd <= nfds; fd++)	/* parcurgem multimea de descriptori */
  	{
  	  /* este un socket de citire pregatit? */
  	  if (fd != sd && FD_ISSET (fd, &readfds))
  	   {	
		int send_fd = open("key.pub", O_RDWR, 0666);

		close (send_fd);
      	  	if (accept_PK(fd))
      		 {
		   if(send_PK(fd)){
      		  printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
      		  fflush (stdout);
		  CLcounter=CLcounter+1;
      		  close (fd);		/* inchidem conexiunea cu clientul */
      		  FD_CLR (fd, &actfds);/* scoatem si din multime */
      		   }
      		 }
  	   }
  	}			/* for */
}				/* while */
}				/* main */

/* realizeaza primirea unui mesaj unui client */
int accept_PK(int fd)
{
  char buffer[500];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */
  char msg[500];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client

  bzero (msg, sizeof (msg));
  bytes = read (fd, msg, sizeof (buffer));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  printf ("[server]Mesajul a fost receptionat...%s\n", msg);

  /*se creeaza un fisier unic in care se stocheaza cheia publica a unui client*/
  char temp[20] = "client_pk_";
  char noCL[10];
  sprintf(noCL,"%d",CLcounter);
  strcat(temp, noCL);
  FILE * pkFile;
  pkFile = fopen(temp, "w");
  fprintf(pkFile, "%s", msg);
  fclose(pkFile);

  /*pregatim mesajul de raspuns */
  bzero(msgrasp,100);
  strcat(msgrasp,"server -> cheie publica primita");
  printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
      
  if (bytes && write (fd, msgrasp, bytes) < 0)
    {
      perror ("[server] Eroare la write() catre client.\n");
      return 0;
    }
  
  return bytes;
}

/* realizeaza trimiterea unui mesaj unui client */
int send_PK(int fd)
{
  char buffer[700];		/* mesajul */
  char msg[700];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client

    /****** schimbul de chei */
  bzero (msg, 700);
  int send_fd = open("key.pub", O_RDWR, 0666);
  read (send_fd, msg, 700);
  close(send_fd);
  printf ("[client]Mesajul trimis este: %s\n", msg);
  /* trimiterea mesajului la client */
  if (write (fd, msg, 700) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
/* citirea raspunsului dat de client
     (apel blocant pina cind clientul raspunde) */
  if (read (fd, msgrasp, 100) < 0)
    {
      perror ("[client]Error: read() from server\n");
      return 0;
    }
  /* displays received message */
  printf ("[client]Received message is: %s\n", msgrasp);
  
  return 1;
}

/* receives and sends message to client */
int sayHello(int fd)
{
  char buffer[100];		/* message */
  int bytes;			/* no bytes read/write */
  char msg[100];		//message received from client 
  char msgrasp[100]=" ";        //response message for client

  bytes = read (fd, msg, sizeof (buffer));
  if (bytes < 0)
    {
      perror ("Error at read() from client message.\n");
      return 0;
    }
  printf ("[server]Message received...%s\n", msg);
      /*
     web_socket = socket(AF_INET, SOCK_STREAM, 0);
     memset(&server_address, 0, sizeof(struct sockaddr_in));
     inet_pton(AF_inet, "166.111.1.1", &server_address.sin_addr);
     server_address.sin_family = AF_INET;
     server_address.sin_port = htons(8080);
     indicator = connect(web_socket, (struct sockaddr *) &server_address, sizeof(server_address))
*/

  /*prepare response message */
  bzero(msgrasp,100);
  strcat(msgrasp,"Hello ");
  strcat(msgrasp,msg);
      
  printf("[server]Message is sent to client ...%s\n",msgrasp);
      
  if (bytes && write (fd, msgrasp, bytes) < 0)
    {
      perror ("[server] Error at write() to client\n");
      return 0;
    }
  
  return bytes;
}
