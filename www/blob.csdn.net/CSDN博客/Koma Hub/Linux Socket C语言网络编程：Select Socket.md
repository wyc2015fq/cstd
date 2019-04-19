# Linux Socket C语言网络编程：Select Socket - Koma Hub - CSDN博客
2018年07月11日 22:05:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：137
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/select-socket.md#select-socket)Select Socket
- 你需要包含`#include <sys/select.h>`
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/select-socket.md#%E7%BB%93%E6%9E%9C)结果
```
$ ./server &
$ ./client 127.0.0.1
accpet connection~
accpet a new client: 127.0.0.1:23261
this is a test.
reading the socket~~~ 
clint[0] send message: this is a test.
this is a test.
```
当然，你也可以再打开一个终端，然后运行客户端代码，结果在本文的最后部分
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/select-socket.md#%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%BB%A3%E7%A0%81serverc)服务器代码`server.c`
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
#defineMAX_LINE2048
#defineLISTENQ20intmain(int argc , char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready , client[FD_SETSIZE];
	
	ssize_t n, ret;
		
	fd_set rset , allset;
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
	/*(4) 设置select*/
	maxfd = listenfd;
	maxi = -1;
	for(i=0 ; i<FD_SETSIZE ; ++i)
	{
		client[i] = -1;
	}//forFD_ZERO(&allset);
	FD_SET(listenfd , &allset);
	/*(5) 进入服务器接收请求死循环*/while(1)
	{
		rset = allset;
		nready = select(maxfd+1 , &rset , NULL , NULL , NULL);
		
		if(FD_ISSET(listenfd , &rset))
		{
			/*接收客户端的请求*/
			clilen = sizeof(cliaddr);
			printf("\naccpet connection~\n");
			if((connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen)) < 0)
			{
				perror("accept error.\n");
				exit(1);
			}//if		printf("accpet a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr) , cliaddr.sin_port);
			/*将客户链接套接字描述符添加到数组*/for(i=0 ; i<FD_SETSIZE ; ++i)
			{
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}//if
			}//forif(FD_SETSIZE == i)
			{
				perror("too many connection.\n");
				exit(1);
			}//ifFD_SET(connfd , &allset);
			if(connfd > maxfd)
				maxfd = connfd;
			if(i > maxi)
				maxi = i;
			if(--nready < 0)
				continue;
		}//iffor(i=0; i<=maxi ; ++i)
		{
			if((sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd , &rset))
			{
				/*处理客户请求*/printf("\nreading the socket~~~ \n");
				
				bzero(buf , MAX_LINE);
				if((n = read(sockfd , buf , MAX_LINE)) <= 0)
				{
					close(sockfd);
					FD_CLR(sockfd , &allset);
					client[i] = -1;
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
	}//while
}
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/select-socket.md#%E5%AE%A2%E6%88%B7%E7%AB%AF%E4%BB%A3%E7%A0%81clientc)客户端代码`client.c`
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
/*采用select的客户端消息处理函数*/voidstr_cli2(FILE* fp , int sockfd)
{
	int maxfd;
	fd_set rset;
	/*发送和接收缓冲区*/char sendline[MAX_LINE] , recvline[MAX_LINE];
	FD_ZERO(&rset);
	while(1)
	{
		/*将文件描述符和套接字描述符添加到rset描述符集*/FD_SET(fileno(fp) , &rset);	
		FD_SET(sockfd , &rset);
		maxfd = max(fileno(fp) , sockfd) + 1;
		select(maxfd , &rset , NULL , NULL , NULL);
		
		if(FD_ISSET(fileno(fp) , &rset))
		{
			if(fgets(sendline , MAX_LINE , fp) == NULL)
			{
				printf("read nothing~\n");
				close(sockfd); /*all done*/return ;
			}//if
			sendline[strlen(sendline) - 1] = '\0';
			write(sockfd , sendline , strlen(sendline));
		}//ifif(FD_ISSET(sockfd , &rset))
		{
			if(readline(sockfd , recvline , MAX_LINE) == 0)
			{
				
				perror("handleMsg: server terminated prematurely.\n");
				exit(1);			
			}//ifif(fputs(recvline , stdout) == EOF)
			{
				perror("fputs error");
				exit(1);
			}//if
		}//if	
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
    }//if/*调用普通消息处理函数*/str_cli(sockfd);	
	/*调用采用select技术的消息处理函数*///str_cli2(stdin , sockfd);exit(0);
}
## [](https://github.com/Rtoax/Knowledge/blob/master/Socket/select-socket.md#makefile)`Makefile`
ALL:
	gcc server.c -o server -lm
	gcc client.c -o client -lm
- 在不同的终端下测试的结果：
![](https://img-blog.csdn.net/20180711220438466?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
