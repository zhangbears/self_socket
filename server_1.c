#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define MAX 4096
void *receive_msg(void *fd);
void *sendout_msg(void *fd);

int main()
{
    int serverfd,confd;                 //接收socket对象建立后的文件描述符
    pthread_t thread_read,thread_write; //读，写线程
    struct sockaddr_in serveraddr;             //socket地址
    char send_msg[MAX],recv_msg[MAX];   //发送和接受信息数组
    int byte;                           //字节数

    //创建socket对象
    if((serverfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket create error!\n");
        exit(EXIT_FAILURE);
    }

    //初始化地址簇
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(6666);
   // serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定地址簇
    if(bind(serverfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)) == -1)
    {
        perror("bind error!\n");
        exit(EXIT_FAILURE);
    }

    //监听端口
    if(listen(serverfd,10) == -1)
    {
        perror("listen error!\n");
        exit(EXIT_FAILURE);
    }

    if((confd = accept(serverfd,(struct sockaddr *)NULL,NULL)) == -1)
    {
        perror("accept error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_create(&thread_read,NULL,receive_msg,&confd);
    while(1)                 
    {                        
        memset(send_msg,0,MAX);
        printf("self:");     
        scanf("%s",send_msg);
        send(confd,send_msg,sizeof(send_msg),0);
    }   
    close(confd);
    close(serverfd);
}

void *receive_msg(void *fd)
{
    char recv_msg[MAX];
    int byte;
    while(1)
    {
        memset(recv_msg,0,MAX);
        byte = recv(*(int *)fd,recv_msg,MAX,0);
        if(byte > 0)
            printf("\e[32mclient:%s\n\e[0m",recv_msg);
    }
    printf("receive message init over!\n");
    return ((void *)0);
}

