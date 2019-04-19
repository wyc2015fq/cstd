# Linux Socket C语言网络编程：UDP Socket - Koma Hub - CSDN博客
2018年07月11日 21:52:35[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：83
## UDP简介
- UDP与TCP相比，UDP是没有链接的协议，可以把他比作电子邮件，收信方不需要确认就可以接收邮件（不需要握手），这一点以TCP协议不同。
下面简单介绍GitHub上的开源项目TCP-Socket
结果是这样的
$ ./server &
$ ./client 127.0.0.1
this is a test
this is a test
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/udp-socket.md#%E6%9C%8D%E5%8A%A1%E5%99%A8%E7%AB%AF%E4%BB%A3%E7%A0%81serverc)服务器端代码`server.c`
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/ip.h>constint SERV_PORT = 6000;
constint MAXLINE = 2048;
voiddg_echo(int sockfd , struct sockaddr *pcliaddr , socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];
	for( ; ;)
	{
		len = clilen;
		if((n = recvfrom(sockfd , mesg , MAXLINE , 0 , pcliaddr , &len)) <0)
		{
			perror("recvfrom error");
			exit(1);
		}//ifif((n = sendto(sockfd , mesg , n , 0 , pcliaddr , len)) < 0)
		{
			perror("sendto error");
			exit(1);
		}//if
	}//for
}
intmain(int argc , char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr , cliaddr;
	bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	if((sockfd = socket(AF_INET , SOCK_DGRAM , 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}//ifif(bind(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)))
	{
		perror("bind error");
		exit(1);
	}//ifdg_echo(sockfd , (struct sockaddr *)&cliaddr , sizeof(cliaddr));	
}
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/udp-socket.md#%E5%AE%A2%E6%88%B7%E7%AB%AF%E4%BB%A3%E7%A0%81clientc)客户端代码`client.c`
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>constint SERV_PORT = 6000;
constint MAXLINE = 2048;
voiddg_cli(FILE *fp , int sockfd , conststruct sockaddr *pservaddr , socklen_t servlen)
{
	int n;
	char sendline[MAXLINE] , recvline[MAXLINE+1];
	
	while(fgets(sendline , MAXLINE , fp) != NULL)
	{
		if(sendto(sockfd , sendline , strlen(sendline) , 0 , pservaddr ,  servlen) < 0)
		{
			perror("sendto error");
			exit(1);
		}//ifif( ( n = recvfrom(sockfd , recvline , MAXLINE , 0 , NULL , NULL)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}//if
		recvline[n] = '\0';
		fputs(recvline , stdout);
	}//while
}
intmain(int argc , char **argv)
{
	int sockfd , t;
	struct sockaddr_in servaddr;
	if(argc != 2)
	{
		perror("usage: udpcli <IPaddress>");
		exit(1);
	}//ifbzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	if((t = inet_pton(AF_INET , argv[1], &servaddr.sin_addr)) <= 0)
	{
		perror("inet_pton error");
		exit(1);
	}//ifif((sockfd = socket(AF_INET , SOCK_DGRAM , 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}//ifdg_cli(stdin , sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) ;
	exit(0);
} 
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/udp-socket.md#makefile)`Makefile`
ALL:
	gcc server.c -o server -lm
	gcc client.c -o client -lm
- 代码来自GitHub帐号，此为学习笔记，源代码请查看GitHub
