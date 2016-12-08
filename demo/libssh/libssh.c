#include <libssh/libssh.h> 
#include <stdlib.h>

#include <stdio.h>

int main()
{
  ssh_session my_ssh_session;
  int verbosity = SSH_LOG_PROTOCOL;
  int port = 22;
  my_ssh_session = ssh_new();
  if (my_ssh_session == NULL)
    exit(-1);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "localhost");
  ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
  //...
  printf("Hello");
  ssh_free(my_ssh_session);
}