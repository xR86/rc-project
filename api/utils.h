
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
  pid_t pid;  /* PID-ul procesului copil */
  int status; /* starea de terminare a procesului copil */

  //printf ("Vom executa comanda...\n");

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      return 0;/*not ok - exit(1)*/
    }
  else if (pid) /* parinte */
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
  else  /* fiu */
    {
  int fd = open("openssl_checker_file", O_WRONLY|O_CREAT|O_TRUNC, 0666);
  dup2(fd, 1);
  close(fd);
      execlp ("which",
        /* comanda de executat (se va cauta in directoarele din PATH) */
        "which",    /* argv[0] */
        "openssl",  /* argv[1]*/
        NULL);
      /* daca ajungem aici inseamna ca nu s-a putut executa */
      //printf ("Eroare de executie!\n");
      return 0;/*not ok - exit(1)*/
    }
}
