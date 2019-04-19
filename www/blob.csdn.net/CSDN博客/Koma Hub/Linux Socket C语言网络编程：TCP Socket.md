# Linux Socket C语言网络编程：TCP Socket - Koma Hub - CSDN博客
2018年07月10日 22:46:46[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：70
个人分类：[Socket																[TCP/IP																[计算机网络](https://blog.csdn.net/Rong_Toa/article/category/7569489)](https://blog.csdn.net/Rong_Toa/article/category/7792859)](https://blog.csdn.net/Rong_Toa/article/category/7792858)
## TCP/IP Socket套接字C语言编程
- 源代码尽供学习交流
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/tcp-socket.md#%E5%A4%B4%E6%96%87%E4%BB%B6cnofigh)头文件`cnofig.h`
/* * config.h 包含该tcp/ip套接字编程所需要的基本头文件，与server.c client.c位于同一目录下*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>constint MAX_LINE = 2048;
constint PORT = 6000;
constint BACKLOG = 10;
constint LISTENQ = 6666;
constint MAX_CONNECT = 20;
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/tcp-socket.md#%E6%9C%8D%E5%8A%A1%E5%99%A8%E7%AB%AF%E4%BB%A3%E7%A0%81serverc)服务器端代码`server.c`
/* * server.c为服务器端代码*/
#include"config.h"intmain(int argc , char **argv)
{
	/*声明服务器地址和客户链接地址*/struct sockaddr_in servaddr , cliaddr;
	/*声明服务器监听套接字和客户端链接套接字*/int listenfd , connfd;
	pid_t childpid;
	/*声明缓冲区*/char buf[MAX_LINE];
	socklen_t clilen;
	/*(1) 初始化监听套接字listenfd*/if((listenfd = socket(AF_INET , SOCK_STREAM , 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}//if/*(2) 设置服务器sockaddr_in结构*/bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //表明可接受任意IP地址
	servaddr.sin_port = htons(PORT);
	/*(3) 绑定套接字和端口*/if(bind(listenfd , (struct sockaddr*)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("bind error");
		exit(1);
	}//if/*(4) 监听客户请求*/if(listen(listenfd , LISTENQ) < 0)
	{
		perror("listen error");
		exit(1);
	}//if/*(5) 接受客户请求*/for( ; ; )
	{
		clilen = sizeof(cliaddr);
		if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0 )
		{
			perror("accept error");
			exit(1);
		}//if//新建子进程单独处理链接if((childpid = fork()) == 0) 
		{
			close(listenfd);
			//str_echossize_t n;
			char buff[MAX_LINE];
			while((n = read(connfd , buff , MAX_LINE)) > 0)
			{
				write(connfd , buff , n);
			}
			exit(0);
		}//ifclose(connfd);
	}//for/*(6) 关闭监听套接字*/close(listenfd);
}
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/tcp-socket.md#%E5%AE%A2%E6%88%B7%E7%AB%AF%E4%BB%A3%E7%A0%81clientc)客户端代码`client.c`
/* * client.c为客户端代码*/
#include"config.h"/*readline函数实现*/ssize_treadline(int fd, char *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;
	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = read(fd, &c,1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} elseif (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} elsereturn(-1);		/* error, errno set by read() */
	}
	*ptr = 0;	/* null terminate like fgets() */return(n);
}
intmain(int argc , char ** argv)
{
	/*声明套接字和链接服务器地址*/int sockfd;
	struct sockaddr_in servaddr;
	/*判断是否为合法输入*/if(argc != 2)
	{
		perror("usage:tcpcli <IPaddress>");
		exit(1);
	}//if/*(1) 创建套接字*/if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}//if/*(2) 设置链接服务器地址结构*/bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if(inet_pton(AF_INET , argv[1] , &servaddr.sin_addr) < 0)
	{
		printf("inet_pton error for %s\n",argv[1]);
		exit(1);
	}//if/*(3) 发送链接服务器请求*/if( connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}//if/*(4) 消息处理*/char sendline[MAX_LINE] , recvline[MAX_LINE];
	while(fgets(sendline , MAX_LINE , stdin) != NULL)	
	{
		write(sockfd , sendline , strlen(sendline));
		if(readline(sockfd , recvline , MAX_LINE) == 0)
		{
			perror("server terminated prematurely");
			exit(1);
		}//ifif(fputs(recvline , stdout) == EOF)
		{
			perror("fputs error");
			exit(1);
		}//if
	}//while/*(5) 关闭套接字*/close(sockfd);
}
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/tcp-socket.md#makefile)`Makefile`
ALL:
	gcc server.c -o server -lm
	gcc client.c -o client -lm
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/tcp-socket.md#%E8%BF%90%E8%A1%8C%E5%AE%9E%E7%8E%B0%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8E%E5%AE%A2%E6%88%B7%E6%9C%BA%E4%B9%8B%E9%97%B4%E7%9A%84%E8%BF%9B%E7%A8%8B%E9%97%B4%E9%80%9A%E4%BF%A1)运行，实现服务器与客户机之间的进程间通信
$ ./server &
$ ./client 127.0.0.1
this is a test
this is a test
当然，在两个终端中也是可以的，如下图：
![](https://img-blog.csdn.net/20180710224601842?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
