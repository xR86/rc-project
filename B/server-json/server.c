//server.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <json-c/json.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_SIZE 10000


int main()
{
    int listenfd = 0, connfd = 0;   //related with the server
    struct sockaddr_in serv_addr;

    FILE *fp;
    fp=fopen("safe_temp.json", "w");
    //json_object * jobj;
    uint8_t buf[158], i;

    memset(&buf, '0', sizeof(buf));
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8888); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("binding\n");

    listen(listenfd, 5);
    printf("listening\n");
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    
    /* all previous code until
    printf("Reading from client\n"); */

    ssize_t r;

    char buff[MAX_SIZE];

    for (;;)
    {
        r = read(connfd, buff, MAX_SIZE);

        if (r == -1)
        {
            perror("read");
            return EXIT_FAILURE;
        }
        if (r == 0)
            break;

        printf("READ: %s\n", buff);
        fputs(buff, fp);
    }
    fclose(fp);

    return EXIT_SUCCESS;
}