
/*check if file is empty -> used in check_openssl()*/
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
  pid_t pid;  /* PID of child process */
  int status; /* termination status of child process */

  if ((pid = fork ()) < 0)
    {
      perror ("fork()");
      return 0;/*not ok - exit(1)*/
    }
  else if (pid) /* parent */
    {
      if (wait (&status) < 0)
  {
    perror ("wait()");
  }
      FILE * checkfile;
      checkfile = fopen("openssl_checker_file","r");
      if(isEmpty(checkfile)){ return 0; }
      //printf ("Command executed.\n");

      return 1;/*ok - exit(0)*/
    }
  else  /* child */
    {
  int fd = open("openssl_checker_file", O_WRONLY|O_CREAT|O_TRUNC, 0666);
  dup2(fd, 1);
  close(fd);
      execlp ("which",
        /* command to execute (will search for folders in PATH) */
        "which",    /* argv[0] */
        "openssl",  /* argv[1]*/
        NULL);
      /* will get here if execlp failed */
      printf ("Execution error !\n");

      return 0;/*not ok - exit(1)*/
    }
}
