# Linux Socket C语言网络编程：Poll Socket [code from GitHub, for study] - Koma Hub - CSDN博客
2018年07月14日 21:48:40[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：81
个人分类：[C/C++																[Socket																[计算机网络](https://blog.csdn.net/Rong_Toa/article/category/7569489)](https://blog.csdn.net/Rong_Toa/article/category/7792858)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
## two files in one directory
- `server.c`
- `client.c`
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/poll-socket.md#code-from-github-for-study)code from GitHub, for study
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/poll-socket.md#serverc)`server.c`
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<time.h>
#include<sys/socket.h>
#include<poll.h>
#include<limits.h>/*for OPEN_MAX*/
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#ifndef OPEN_MAX
#defineOPEN_MAX1024
#endif
#ifndef INFTIM
#defineINFTIM -1
#endif
#definePORT8888
#defineMAX_LINE2048
#defineLISTENQ20intmain(int argc , char **argv)
{
	int i, maxi, listenfd, connfd, sockfd;
	int nready;
	
	ssize_t n, ret;
		
	struct pollfd client[OPEN_MAX];
	char buf[MAX_LINE];
	socklen_t clilen;
	struct sockaddr_in servaddr , cliaddr;
	/*(1) 得到监听描述符*/
	listenfd = socket(AF_INET , SOCK_STREAM , 0);
	/*(2) 绑定套接字*/bzero(&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);
	bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr));
	/*(3) 监听*/listen(listenfd , LISTENQ);
	/*(4) 设置poll*/
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for(i=1 ; i<OPEN_MAX ; ++i)
	{
		client[i].fd = -1;
	}//for
	maxi = 0;
	/*(5) 进入服务器接收请求死循环*/while(1)
	{
		nready = poll(client , maxi+1 , INFTIM);
		
		if(client[0].revents & POLLRDNORM)
		{
			/*接收客户端的请求*/
			clilen = sizeof(cliaddr);
			printf("\naccpet connection~\n");
			if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0)
			{
				perror("accept error.\n");
				exit(1);
			}//if		printf("accpet a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr) , cliaddr.sin_port);
			
			/*将客户链接套接字描述符添加到数组*/for(i=1 ; i<OPEN_MAX ; ++i)
			{
				if(client[i].fd < 0)
				{
					client[i].fd = connfd;
					break;
				}//if
			}//forif(OPEN_MAX == i)
			{
				perror("too many connection.\n");
				exit(1);
			}//if/*该描述符等待的事件*/
			client[i].events = POLLRDNORM;
			if(i > maxi)
				maxi = i;
			if(--nready < 0)
				continue;
		}//iffor(i=1; i<=maxi ; ++i)
		{
			if((sockfd = client[i].fd) < 0)
				continue;
			/*该链接描述符实际发生的事件*/if(client[i].revents & (POLLRDNORM | POLLERR))
			{
				/*处理客户请求*/printf("\nreading the socket~~~ \n");
				
				bzero(buf , MAX_LINE);
				if((n = read(sockfd , buf , MAX_LINE)) <= 0)
				{
					close(sockfd);				
					client[i].fd = -1;
				}//ifelse{
					printf("clint[%d] send message: %s\n", i , buf);
					if((ret = write(sockfd , buf , n)) != n)
					{
						printf("error writing to the sockfd!\n");
						break;
					}//if
				}//elseif(--nready <= 0)
					break;
			}//if
		}//for
	}//whileexit(0);
}
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/poll-socket.md#clientc)`client.c`
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<time.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#definePORT8888
#defineMAX_LINE2048intmax(int a , int b)
{
	return a > b ? a : b;
}
/*readline函数实现*/ssize_treadline(int fd, char *vptr, size_t maxlen)
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
/*普通客户端消息处理函数*/voidstr_cli(int sockfd)
{
	/*发送和接收缓冲区*/char sendline[MAX_LINE] , recvline[MAX_LINE];
	while(fgets(sendline , MAX_LINE , stdin) != NULL)	
	{
		write(sockfd , sendline , strlen(sendline));
		bzero(recvline , MAX_LINE);
		if(readline(sockfd , recvline , MAX_LINE) == 0)
		{
			perror("server terminated prematurely");
			exit(1);
		}//ifif(fputs(recvline , stdout) == EOF)
		{
			perror("fputs error");
			exit(1);
		}//ifbzero(sendline , MAX_LINE);
	}//while
}
intmain(int argc , char **argv)
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
    }//if/*(3) 发送链接服务器请求*/if(connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }//if/*调用消息处理函数*/str_cli(sockfd);	
	exit(0);
}
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/poll-socket.md#complied--run)complied & run
```
$ gcc server.c -o server
$ gcc client.c -o client
$ ./server &
$ ./client 127.0.0.1
```
