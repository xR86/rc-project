/* exec.c

   Executia prin program a comenzii 'ls -a -l', folosind execlp()
   
   Autor: Sabin-Corneliu Buraga <busaco@infoiasi.ro> (c)1998-2001
   Versiunea: 1.0 (ultima actualizare: 04 iunie 2001)
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int
main ()
{
  pid_t pid;			/* PID-ul procesului copil */
  int status;			/* starea de terminare a procesului copil */

  printf ("Vom executa comanda...\n");

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      exit (1);
    }
  else if (pid)			/* parinte */
    {
      if (wait (&status) < 0)
	{
	  perror ("wait()");
	}
      printf ("Comanda a fost executata.\n");
      exit (0);
    }
  else				/* fiu */
    {
        execlp ("bash",
	      /* comanda de executat (se va cauta in directoarele din PATH) */
	      "bash",		/* argv[0] */
	      "-c",
	      "su | apt-get install openssl",	/* argv[1] */
	      NULL);
      /* vom folosi execlp() */
      /*execlp ("apt-get",
	      "apt-get",		
	      "install",		
	      "openssl",	
	      NULL);*/
      /* daca ajungem aici inseamna ca nu s-a putut executa */
      printf ("Eroare de executie!\n");
      exit (1);
    }
}
