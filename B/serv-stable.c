//server.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

/* portul folosit */
#define PORT 2026
#define PORTPY 2028

/* codul de eroare returnat de anumite apeluri */
extern int errno;

void handler(int sig)
{
  pid_t pid;

  pid = wait(NULL);
  printf("Pid %d exited.\n", pid);
}

int main ()
{
  struct sockaddr_in server;  // structura folosita de server
  struct sockaddr_in from;  
  struct sockaddr_in server2;  
  char msg[100];    //mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client
  int sd, sdPy;     //descriptorul de socket 

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket() - server.\n");
      return errno;
    }

  if ((sdPy = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket() - python.\n");
      return errno;
    }
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  bzero (&server2, sizeof(server2));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;  
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);


    server2.sin_family = AF_INET;
    server2.sin_addr.s_addr = inet_addr("127.0.0.1");
    server2.sin_port = htons (PORTPY);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  /* servim in mod iterativ clientii... */
  while (1)
    {
      int client;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      client = accept (sd, (struct sockaddr *) &from, &length);

      /* eroare la acceptarea conexiunii de la un client */
      if (client < 0)
      {
        perror ("[server]Eroare la accept().\n");
        continue;
      }

      int child;
      int status;

      if( (child=fork()) == -1) perror("Err...fork");
      else if(child) { //parinte
        //signal(SIGCHLD, SIG_IGN);
        signal(SIGCHLD, handler);

        /*if (wait (&status, WNOHANG) < 0)
        {
          perror ("wait()");
        }*/

      }
      else{ //copil
        /* s-a realizat conexiunea, se astepta mesajul */
        bzero (msg, 1000);
        printf ("[server]Asteptam mesajul...\n");
        fflush (stdout);
        
        /* citirea mesajului */
        if (read (client, msg, 1000) <= 0)
        {
          perror ("[server]Eroare la read() de la client.\n");
          close (client); /* inchidem conexiunea cu clientul */
          continue;   /* continuam sa ascultam */
        }
    
        printf ("[server]Mesajul a fost receptionat...%s\n", msg);

        fflush(stdin);
        fflush(stdout);
        fflush(stderr);

        /*pregatim mesajul de raspuns */
        bzero(msgrasp,4);
        strcat(msgrasp,"ACK");
        
        printf("[server]Trimitem mesajul inapoi...%s\n",msgrasp);

        /* returnam mesajul clientului */
        if (write (client, msgrasp, 4) <= 0)
        {
          perror ("[server]Eroare la write() catre client.\n");
          continue;   /* continuam sa ascultam */
        }
        else
            printf ("[server]Mesajul a fost transmis cu succes.\n");




    
        
        printf ("[server]Mesajul a fost receptionat...%s\n", msg);

        if (connect (sdPy, (struct sockaddr *) &server2,sizeof (struct sockaddr)) == -1)
          {
            perror ("[client]Eroare la connect().\n");
            return errno;
          }

        
        if (write (sdPy, msg, 1000) <= 0)
          {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
          }

        bzero(msgrasp,4);
        if (read (sdPy, msgrasp, 4) < 0)
          {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
          }

        printf ("[client]Mesajul primit este: %s\n", msgrasp);

        // close (sdPy);


        /* am terminat cu acest client, inchidem conexiunea */
        close (client);
        exit(0);
      }
    }       /* while */
}       /* main */