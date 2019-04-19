# UDP套接字编程 - yixianfeng41的专栏 - CSDN博客
2017年02月17日 12:41:52[第2梦](https://me.csdn.net/yixianfeng41)阅读数：408标签：[UDP-通信																[UDP-Socket																[UDP套接字编程](https://so.csdn.net/so/search/s.do?q=UDP套接字编程&t=blog)](https://so.csdn.net/so/search/s.do?q=UDP-Socket&t=blog)](https://so.csdn.net/so/search/s.do?q=UDP-通信&t=blog)
个人分类：[Linux/Unix](https://blog.csdn.net/yixianfeng41/article/category/6006179)
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
## UDP简介
UDP—用户数据报协议，是一个简单的面向数据报的运输层协议。UDP不提供可靠性，它只是把应用程序传给IP层的数据报发送出去，但是并不能保证它们能到达目的地。由于UDP在传输数据报前不用在客户和服务器之间建立一个连接，且没有超时重发等机制，故而传输速度很快.
其编程通信框架如下： 
![这里写图片描述](https://img-blog.csdn.net/20170217122418098)
编程实现大致跟TCP一样，主要注意的是，不用listen,不用accept,数据类型是数据报类型（SOCK_DGRAM）,以及接收和发送函数多了三个参数。
```
ssize_t recvfrom(int sockfd,void *buf,size_t nbytes,int flag,struct sockaddr *from,socklen_t * addrlen);
ssize_t sento(int sockfd,const void *buf,size_t nbytes,int flag,struct sockaddr *to,socklen_t * addrlen);
```
前面三个参数跟tcp一样得含义，后面三个参数，一般flages设置为零就行，接着是要发送或接受的地址，以及socklen.看看实际代码：
## UDP Socket编程
服务端：
```cpp
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 9899
#define BUF_SIZE 1205
int main(int argc,char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr,cliaddr;
    socklen_t sockaddrlen=sizeof(cliaddr);
    char buf[BUF_SIZE];
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket error");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(PORT);
    if(bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
    {
        perror("bind error");
        exit(1);
    }
    while(1)
    {
        int n=recvfrom(sockfd,buf,BUF_SIZE,0,(struct sockaddr *)&cliaddr,&sockaddrlen);
        printf("msg lenth: %d ",n);
        if(n>0)
        {
            puts(buf);
            buf[n+1]=0;
            sendto(sockfd,buf,n+1,0,(struct sockaddr *)&cliaddr,sockaddrlen);
        }
        else if (n<0)
        {
            perror("recv error");
            break;
        }
    }
    close(sockfd);
    return 0;
}
```
客户端：
```cpp
//
// Created by Administrator on 2017-06-12.
//
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    socklen_t socklen=sizeof(struct sockaddr_in);
    char message[1000] , server_reply[2000];
    //Create socket
    sock = socket(AF_INET ,SOCK_DGRAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 9899 );
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        //Send some data
        if( sendto(sock , message , strlen(message)+1 , 0,(struct sockaddr *)&server, socklen) < 0)
        {
            puts("Send failed");
            close(sock);
            break;
        }
        //Receive a reply from the server
        int rc=recvfrom(sock , server_reply , 2000 , 0,(struct sockaddr *)&server, &socklen);
        if( rc< 0)
        {
            puts("recv failed");
            break;
        }
        else if(rc>0)
        {
            puts("Server reply :");
            puts(server_reply);
        }
    }
    close(sock);
    return 0;
}
```
参考资料： 
《UNIX网络编程 卷一》 
[linux socket编程之TCP与UDP](http://www.cnblogs.com/magicbox/archive/2012/02/15/2363875.html)
[Linux UDP C/S例子](http://www.cnblogs.com/uvsjoh/archive/2013/01/01/2841764.html)
