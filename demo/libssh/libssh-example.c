#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>

void free_channel(ssh_channel channel) {
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
}

void free_session(ssh_session session) {
    ssh_disconnect(session);
    ssh_free(session);
}

void error(ssh_session session) {
    fprintf(stderr, "Error: %s\n", ssh_get_error(session));
    free_session(session);
    exit(-1);
}

int main() {

    ssh_session session;
    ssh_channel channel;
    int rc, port = 22;
    char buffer[1024];
    unsigned int nbytes;

    printf("Session...\n");
    session = ssh_new();
    if (session == NULL) exit(-1);

    ssh_options_set(session, SSH_OPTIONS_HOST, "localhost"); //localhost //192.168.100.2
    ssh_options_set(session, SSH_OPTIONS_PORT, &port);
    ssh_options_set(session, SSH_OPTIONS_USER, "xR86"); //xR86 //labsi

    printf("Connecting...\n");
    rc = ssh_connect(session);
    if (rc != SSH_OK) error(session);

    printf("Password Autentication...\n");
    rc = ssh_userauth_password(session, "xR86", "parola");
    //rc = ssh_userauth_password(session, NULL, "parola");
    //rc = ssh_userauth_publickey_auto(session, "xR86", NULL);
    if (rc != SSH_AUTH_SUCCESS) error(session);

    printf("Channel...\n");
    channel = ssh_channel_new(session);
    if (channel == NULL) exit(-1);

    printf("Opening...\n");
    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) error(session);

    printf("Executing remote command...\n");
    rc = ssh_channel_request_exec(channel, "ls -l");
    if (rc != SSH_OK) error(session);

    printf("Received:\n");
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        fwrite(buffer, 1, nbytes, stdout);
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    free_channel(channel);
    free_session(session);

    return 0;
}