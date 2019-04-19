# Linux 多播（组播）例程 - maopig的专栏 - CSDN博客
2011年11月22日 13:42:45[maopig](https://me.csdn.net/maopig)阅读数：1249标签：[linux																[struct																[socket																[通讯																[测试																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=通讯&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[网络](https://blog.csdn.net/maopig/article/category/869532)
 ser.c 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#define BUFLEN 255
int
main (int argc, char **argv) 
{  
    struct sockaddr_in peeraddr,ia;   
    int sockfd;  
    char recmsg[BUFLEN + 1]; 
    unsigned int socklen, n; 
    struct ip_mreq mreq; 
    /* 创建 socket 用于UDP通讯 */ 
    sockfd = socket (AF_INET, SOCK_DGRAM, 0); 
    if (sockfd < 0)
    {          
        printf ("socket creating err in udptalk\n");          
        exit (1);        
    } 
    /* 设置要加入组播的地址 */ 
    bzero(&mreq, sizeof (struct ip_mreq)); 
    inet_pton(AF_INET,"224.0.1.2",&ia.sin_addr);
    /* 设置组地址 */ 
    bcopy (&ia.sin_addr.s_addr, &mreq.imr_multiaddr.s_addr, sizeof (struct in_addr));
    /* 设置发送组播消息的源主机的地址信息 */ 
    mreq.imr_interface.s_addr = htonl (INADDR_ANY);  
    /* 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 */ 
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,sizeof (struct ip_mreq)) == -1)
    {     
        perror ("setsockopt");      
        exit (-1);   
    }
    socklen = sizeof (struct sockaddr_in); 
    memset (&peeraddr, 0, socklen); 
    peeraddr.sin_family = AF_INET;
    peeraddr.sin_port = htons (7838);
    inet_pton(AF_INET, "224.0.1.2", &peeraddr.sin_addr); 
    /* 绑定自己的端口和IP信息到socket上 */ 
    if (bind(sockfd, (struct sockaddr *) &peeraddr,sizeof (struct sockaddr_in)) == -1)
    {      
        printf ("Bind error\n");      
        exit (0);    
    }
    /* 循环接收网络上来的组播消息 */ 
    for (;;)
    {     
        bzero (recmsg, BUFLEN + 1);     
        n = recvfrom (sockfd, recmsg, BUFLEN, 0, (struct sockaddr *) &peeraddr, &socklen);
        if (n < 0)
        {      
            printf ("recvfrom err in udptalk!\n");      
            exit (4);    
        }
        else{      
        /* 成功接收到数据报 */ 
            recmsg[n] = 0;      
            printf ("peer:%s", recmsg);    
        }
    }
}
cli.c
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFLEN 255
int main (int argc, char **argv) 
{
    struct sockaddr_in peeraddr, myaddr; 
    int sockfd;  
    char recmsg[BUFLEN + 1];
    unsigned int socklen;
    socklen = sizeof (struct sockaddr_in);  
    /* 创建 socket 用于UDP通讯 */ 
    sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {      
        printf ("socket creating error\n");     
        exit (1);   
    }  
    /* 设置对方的端口和IP信息 */ 
    memset (&peeraddr, 0, socklen); 
    peeraddr.sin_family = AF_INET;  
    peeraddr.sin_port = htons (7838);
    inet_pton (AF_INET, "224.0.1.2", &peeraddr.sin_addr);
/* 设置自己的端口和IP信息 */ 
    memset (&myaddr, 0, socklen); 
    myaddr.sin_family = AF_INET; 
    myaddr.sin_port = htons (23456); 
    myaddr.sin_addr.s_addr = INADDR_ANY;
    /* 绑定自己的端口和IP信息到socket上 */ 
    if (bind (sockfd, (struct sockaddr *) &myaddr,     sizeof (struct sockaddr_in)) == -1)
    {     
        printf ("Bind error\n");     
        exit (0);  
    }
    /* 循环接受用户输入的消息发送组播消息 */ 
    for (;;)
    {      
        /* 接受用户输入 */ 
        bzero (recmsg, BUFLEN + 1);     
        if (fgets (recmsg, BUFLEN, stdin) == (char *) EOF)    
            exit (0);
        /* 发送消息 */ 
        if (sendto(sockfd, recmsg, strlen (recmsg), 0, (struct sockaddr *) &peeraddr, sizeof (struct sockaddr_in)) < 0)
        {      
            printf ("sendto error!\n");      
            exit (3);    
        }
        printf ("'%s' send ok\n", recmsg);
    } 
}
运行方式：./ser 
　　　　./cli
在属于同一子网的不同linux主机上运行测试成功！

