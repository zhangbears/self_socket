#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX 4096

void *send_msg(void *fd);
void *recv_msg(void *fd);

int main()
{
    int clientfd;
    struct sockaddr_in serveraddr;
    pthread_t thread_read,thread_write;
    char r_msg[MAX];
    int byte;

    if((clientfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket error!\n");
        exit(EXIT_FAILURE);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(6666);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(connect(clientfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) == -1)
    {
        perror("connect error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&thread_write,NULL,send_msg,&clientfd);
    while(1)
    {
        memset(r_msg,0,MAX);
        byte = recv(clientfd,r_msg,MAX,0);
        if(byte > 0)
        {
            printf("\nserver:%s\n",r_msg);
        }
    }
    close(clientfd);
}

void *send_msg(void *fd)
{
    char s_msg[MAX];
    while(1)
    {
        memset(s_msg,0,MAX);
        printf("self:");
        scanf("%s",s_msg);
        send(*(int *)fd,s_msg,sizeof(s_msg),0);
    }
    return ((void *)0);
}
