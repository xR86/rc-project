/* servTCPCSel.c - Exemplu de server TCP concurent 
   
   Asteapta un "nume" de la clienti multipli si intoarce clientilor sirul
   "Hello nume" corespunzator; multiplexarea intrarilor se realizeaza cu select().
   
   Cod sursa preluat din [Retele de Calculatoare,S.Buraga & G.Ciobanu, 2003] si modificat de 
   Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
   
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
#include <sys/stat.h> /*pt open*/
#include <fcntl.h> /*pt open*/

/*ANSI colours*/
#define ANSI_RED     "\x1b[31m"
#define ANSI_GREEN   "\x1b[32m"
#define ANSI_YELLOW  "\x1b[33m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN    "\x1b[36m"
#define ANSI_RESET   "\x1b[0m"

/*extra messages - flag verbose at server startup with "--verbose"*/
int verbose = 0;

int counter = 23;

/*Buffer size pt sendfile*/
#define BUFSIZ 3000


/* portul folosit */
#define PORT 2728

extern int errno;	/* eroarea returnata de unele apeluri */

/* functie de convertire a adresei IP a clientului in sir de caractere */
char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* adresa IP a clientului */
  strcpy (str, inet_ntoa (address.sin_addr));	
  /* portul utilizat de client */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}

/*check if file empty -> used in check_openssl()*/
int isEmpty(FILE *file)
{
    long savedOffset = ftell(file);
    fseek(file, 0, SEEK_END);

    if (ftell(file) == 0)
    {
        return 1;
    }

    fseek(file, savedOffset, SEEK_SET);
    return 0;
}

/*check if openssl is installed*/
int check_openssl(){
  pid_t pid;	/* PID-ul procesului copil */
  int status;	/* starea de terminare a procesului copil */

  //printf ("Vom executa comanda...\n");

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      return 0;/*not ok - exit(1)*/
    }
  else if (pid)	/* parinte */
    {
      if (wait (&status) < 0)
	{
	  perror ("wait()");
	}
      FILE * checkfile;
      checkfile = fopen("openssl_checker_file","r");
      if(isEmpty(checkfile)){ return 0; }
      //printf ("Comanda a fost executata.\n");
      return 1;/*ok - exit(0)*/
    }
  else	/* fiu */
    {
	int fd = open("openssl_checker_file", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	dup2(fd, 1);
	close(fd);
      execlp ("which",
	      /* comanda de executat (se va cauta in directoarele din PATH) */
	      "which",		/* argv[0] */
	      "openssl",	/* argv[1]*/
	      NULL);
      /* daca ajungem aici inseamna ca nu s-a putut executa */
      //printf ("Eroare de executie!\n");
      return 0;/*not ok - exit(1)*/
    }
}

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
	system("sed -i '/writing RSA key/d' ./pub_checker_file ; cp pub_checker_file key.pub; rm pub_checker_file;");
      	FILE * checkfile;
      	checkfile = fopen("key.pub","r");
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

/* programul */
int
main (int argc, char *argv[])
{
  struct sockaddr_in server;	/* structurile pentru server si clienti */
  struct sockaddr_in from;
  fd_set readfds;		/* multimea descriptorilor de citire */
  fd_set actfds;		/* multimea descriptorilor activi */
  struct timeval tv;		/* structura de timp pentru select() */
  int sd, client, wsd;		/* descriptori de socket */
  int optval=1; 			/* optiune folosita pentru setsockopt()*/ 
  int fd;			/* descriptor folosit pentru 
				   parcurgerea listelor de descriptori */
  int nfds;			/* numarul maxim de descriptori */
  int len;			/* lungimea structurii sockaddr_in */

if (argc == 2){
	if(strcmp(argv[1], "--verbose") == 0){
		verbose = 1;
	}
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


  /* creare socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }
  /*creare socket pt download*/
  if ((wsd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }

  /*setam pentru socket optiunea SO_REUSEADDR */ 
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  /* pregatim structurile de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server] Eroare la listen().\n");
      return errno;
    }
  
  /* completam multimea de descriptori de citire */
  FD_ZERO (&actfds);		/* initial, multimea este vida */
  FD_SET (sd, &actfds);		/* includem in multime socketul creat */

  tv.tv_sec = 1;		/* se va astepta un timp de 1 sec. */
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
      		  printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",fd);
      		  fflush (stdout);
      		  close (fd);		/* inchidem conexiunea cu clientul */
      		  FD_CLR (fd, &actfds);/* scoatem si din multime */
      		  
      		 }
  	   }
  	}			/* for */
}				/* while */
}				/* main */

/* realizeaza primirea si retrimiterea unui mesaj unui client */
int accept_PK(int fd)
{
  char buffer[700];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */
  char msg[500];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client

  bytes = read (fd, msg, sizeof (buffer));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  printf ("[server]Mesajul a fost receptionat...%s\n", msg);

  
  ssize_t nrd;
  int fd1 = open(msg[0], buffer, 500, O_RDONLY);
  char * temp = "client_pk_";
  strcat(temp, counter);
  int fd2 = open("client_pk_", O_CREAT | O_WRONLY);
  while(nrd=read(fd1,buffer,500)){
	write(fd2,buffer,nrd);
  }

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

/* realizeaza primirea si retrimiterea unui mesaj unui client */
int sayHello(int fd)
{
  char buffer[100];		/* mesajul */
  int bytes;			/* numarul de octeti cititi/scrisi */
  char msg[100];		//mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client

  bytes = read (fd, msg, sizeof (buffer));
  if (bytes < 0)
    {
      perror ("Eroare la read() de la client.\n");
      return 0;
    }
  printf ("[server]Mesajul a fost receptionat...%s\n", msg);
      /*
     web_socket = socket(AF_INET, SOCK_STREAM, 0);
     memset(&server_address, 0, sizeof(struct sockaddr_in));
     inet_pton(AF_inet, "166.111.1.1", &server_address.sin_addr);
     server_address.sin_family = AF_INET;
     server_address.sin_port = htons(8080);
     indicator = connect(web_socket, (struct sockaddr *) &server_address, sizeof(server_address))
*/

  /*pregatim mesajul de raspuns */
  bzero(msgrasp,100);
  strcat(msgrasp,"Hello ");
  strcat(msgrasp,msg);
      
  printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);
      
  if (bytes && write (fd, msgrasp, bytes) < 0)
    {
      perror ("[server] Eroare la write() catre client.\n");
      return 0;
    }
  
  return bytes;
}
