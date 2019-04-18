# Linux I/O复用 - Orisun - 博客园







# [Linux I/O复用](https://www.cnblogs.com/zhangchaoyang/articles/2681893.html)





## epoll

首先看个结构体

```
typedef union epoll_data
{
    void *ptr;
    int fd;
    uint32_t u32;
    uint64_t u64;
 } epoll_data_t;
  
 struct epoll_event
 {
    uint32_t events;  /* Epoll events */
    epoll_data_t data;    /* User data variable */
 }
```

struct epoll_event的成员events是个bit set，有几种类型：

EPOLLIN：关联的文件是用来读的

EPOLLOUT：关联的文件是用来写的

EPOLLET：Edge Trigger，与之对应的是Level Trigger，下面会详细介绍它们的区别。需要注意的是Level Trigger是默认模式，在我这边（linux-2.6.32）头文件sys/epoll.h中已经没有EPOLLLT的定义了，所以在代码中不要再显式地写EPOLLLT了，反正默认情况用的就是它。select和poll都相当于epoll中的Level Trigger模式。

定义两个变量，后面会用。

struct epoll_event  event, events[20];


epoll系列有3组函数：
- int  epfd=epoll_create(int size);　　　　//创建一个epoll实例。size表示建议内核开辟的空间。
- int nfds=epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);　　　　//准备好读/写的事件存放在参数events中，maxevents是同时监听的最大事件数，timeout是超时返回。
- 
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);　　　　　　//op的取值有：EPOLL_CTL_ADD、EPOLL_CTL_MOD、EPOLL_CTL_DEL，表示你要从监听集中添加、去除或修改某个文件描述符。


看个例子就知道该怎么用了：

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

int main(){
	int i,maxi,listenfd,connfd,sockfd,epfd,nfds;
	int n;
	int yes=1;
	char line[MAXLINE+1];
	socklen_t clilen;
	//声明epoll_event结构体变量，ev用于注册事件，数组用于回传要处理的事件
	struct epoll_event ev,events[20];
	//生成用于处理accept的epoll专用文件描述符
	epfd=epoll_create(256);
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	listenfd=socket(PF_INET,SOCK_STREAM,0);
	//设置套接口选项
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	//设置与要处理的事件相关的文件描述符
	ev.data.fd=listenfd;
	//设置要处理的事件类型
	ev.events=EPOLLET|EPOLLIN;
	//注册epoll事件
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	char *local_addr="127.0.0.1";
	inet_pton(AF_INET,local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=htons(SERV_PORT);
	bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	listen(listenfd,LISTENQ);
	maxi=0;
	while(1){
		//等待epoll事件的发生
		nfds=epoll_wait(epfd,events,20,500);
		//处理所发生的事件
		for(i=0;i<nfds;++i){
			if(events[i].data.fd==listenfd){
				clilen=sizeof(clientaddr);
				connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clilen);
				if(connfd<0){
					perror("connfd<0");
					exit(1);
				}
				char *str=inet_ntoa(clientaddr.sin_addr);
				printf("accept a connection from %s\n",str);
				//设置用于读操作的文件描述符
				ev.data.fd=connfd;
				//设置用于注册的读操作事件
				ev.events=EPOLLET|EPOLLIN;
				//注册ev
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
			}
			else if(events[i].events & EPOLLIN){
				sockfd = events[i].data.fd;
				printf("read:");
				if((n=read(sockfd,line,MAXLINE))<0){
					printf("read error,close connection\n");
					epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,&ev);
					close(sockfd);			
				}
				line[n]='\0';
				printf("|%s|\n",line);
			}
		}
	}
	return 0;
}


上述代码中，服务端建好套接字listenfd后开始监听它，并把它放到epoll中（第38行）。当有新的连接到来时，第53行的if语句成立，服务端accept该连接，并把该连接的描述符connfd放入epoll中（第67行）。如果TCP连接上有可读事件发生，则第69行的if语句成立，服务端从连接上读取数据后打印在标准输出上，如果读取时发生错误则关闭该连接，同时把相应的connfd从epoll中移除。

下面给一个客户端代码负责写服务端写入数据。

#!/usr/bin/perl

use IO::Socket;

my $host="127.0.0.1";
my $port=5000;

my $socket=IO::Socket::INET->new("$host:$port") or die "create socket error $@";
my $msg_out="1234567890";
print $socket $msg_out;
print "now send over,go to sleep...\n";

while(1){
	sleep(1);
}


客户端向服务端写入“1234567890”后并没有关闭连接，而是进入了永久的休眠。

运行程序服务端输出：

accept a connection from 127.0.0.1
read:|12345|

为什么只输出了前5个字节？首先要清楚，上层应用在调用send、recv在TCP连接上收发数据时，send并没有真正地向网络对端发送数据，发送数据的工作中由TCP协议完成的。send仅是检查套接口的发送缓存是否有足够的空间，如果有则send直接将要发送的数据放入缓存区，sned返回成功；如果缓存空间不足，则send阻塞（如果没有设置O_NONBLOCK的话），直到TCP协议发送完缓存区中原有的数据。recv也同样。

Edge Trigger仅当有读/写事件发生时它才触发，上例中client仅发送了一次数据，所以在server端只读取一次，只从缓存区中读取了前MAXLINE（为5）个字节。

Level Trigger只要缓存区中还有数据可读就还会触发。下面的代码采用Level Trigger。

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

int main(){
	int i,maxi,listenfd,connfd,sockfd,epfd,nfds;
	int n;
	int yes=1;
	char line[MAXLINE+1];
	socklen_t clilen;
	struct epoll_event ev,events[20];
	epfd=epoll_create(256);
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	listenfd=socket(PF_INET,SOCK_STREAM,0);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	ev.data.fd=listenfd;
	ev.events=EPOLLIN;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	char *local_addr="127.0.0.1";
	inet_pton(AF_INET,local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=htons(SERV_PORT);
	bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	listen(listenfd,LISTENQ);
	maxi=0;
	while(1){
		nfds=epoll_wait(epfd,events,20,500);
		for(i=0;i<nfds;++i){
			if(events[i].data.fd==listenfd){
				clilen=sizeof(clientaddr);
				connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clilen);
				if(connfd<0){
					perror("connfd<0");
					exit(1);
				}
				char *str=inet_ntoa(clientaddr.sin_addr);
				printf("accept a connection from %s\n",str);
				ev.data.fd=connfd;
				ev.events=EPOLLIN;
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
			}
			else if(events[i].events & EPOLLIN){
				sockfd = events[i].data.fd;
				printf("read:");
				if((n=read(sockfd,line,MAXLINE))<0){
					printf("read error,close connection\n");
					epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,&ev);
					close(sockfd);			
				}
				line[n]='\0';
				printf("|%s|\n",line);
			}
		}
	}
	return 0;
}


运行输出：

accept a connection from 127.0.0.1
read:|12345|
read:|67890|

我们要分两种情况来讨论read()系统调用：

ssize_t read(int fd, void *buf, size_t count);

（1）读取文件。出错时返回-1；成功时返回读到的字节数，这个数字大部分情况下等于count，只有当文件中剩余的内容不足count时返回值才小于count，如果已读到文件末尾则返回0。所以要想读取一个文件的全部内容，只需要把read()放到while(true)循环中，当read()的返回值小于count时，就可以肯定文件读完了，可以退出循环了。read()系统调用在用户空间是不设缓存的。

（2）读取套接口。说得更具体些是读取本地socket的接收缓存。这里跟读取文件的不同之处在于：当接收缓存已无数据可读时，read()不会返回0（如果没有设置O_NONBLOCK的话），而是一直阻塞，除非对方断开连接read()才返回0。

要想在Edge Trigger模式下读取缓冲区中的所有数据呢，必须和O_NONBLOCK综合使用，此时当接收缓冲区无数据可读时read()返回一个负值（注意不是0）,并且把errno置为EAGAIN或EWOULDBLOCK。这里解释一下read()系统调用在返回失败时的几种可能情况：

返回EINTR：在read之前收到信号被中断。

返回EAGAIN：针对普通的文件描述符（不包括套接口描述符），当设置了O_NONBLOCK，而缓存区中又没有数据可读，则返回该值。

返回EWOULDBLOCK：跟EAGAIN类似，只是EWOULDBLOCK专用于套接口描述符。

在有的系统中EAGAIN也可用于套接口描述符，所以为增强可移植性，代码中应该使用对EAGAIN和EWOULDBLOCK都进行检测。

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

void setnonblocking(int sock){
	int opts;
	opts=fcntl(sock,F_GETFL);
	if(opts<0){
		perror("fcntl_get");
		exit(1);
	}
	opts=opts|O_NONBLOCK;
	if(fcntl(sock,F_SETFL,opts)<0){
		perror("fcntl_set");
		exit(1);
	}
}

int main(){
	int i,maxi,listenfd,connfd,sockfd,epfd,nfds;
	int n;
	int yes=1;
	char line[MAXLINE+1];
	socklen_t clilen;
	//声明epoll_event结构体变量，ev用于注册事件，数组用于回传要处理的事件
	struct epoll_event ev,events[20];
	//生成用于处理accept的epoll专用文件描述符
	epfd=epoll_create(256);
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	listenfd=socket(PF_INET,SOCK_STREAM,0);
	//设置套接口选项
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
	//把socket设为非阻塞
	setnonblocking(listenfd);
	//设置与要处理的事件相关的文件描述符
	ev.data.fd=listenfd;
	//设置要处理的事件类型
	ev.events=EPOLLET|EPOLLIN;
	//注册epoll事件
	epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	char *local_addr="127.0.0.1";
	inet_pton(AF_INET,local_addr,&(serveraddr.sin_addr));
	serveraddr.sin_port=htons(SERV_PORT);
	bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	listen(listenfd,LISTENQ);
	maxi=0;
	while(1){
		//等待epoll事件的发生
		nfds=epoll_wait(epfd,events,20,500);
		//处理所发生的事件
		for(i=0;i<nfds;++i){
			if(events[i].data.fd==listenfd){
				clilen=sizeof(clientaddr);
				connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clilen);
				if(connfd<0){
					perror("connfd<0");
					exit(1);
				}
				setnonblocking(connfd);
				char *str=inet_ntoa(clientaddr.sin_addr);
				printf("accept a connection from %s\n",str);
				//设置用于读操作的文件描述符
				ev.data.fd=connfd;
				//设置用于注册的读操作事件
				ev.events=EPOLLET|EPOLLIN;
				//注册ev
				epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
			}
			else if(events[i].events & EPOLLIN){
				sockfd = events[i].data.fd;
				printf("read:");
				while(1){
					n=read(sockfd,line,MAXLINE);
					if(n<0){
						if(errno==EAGAIN || errno==EWOULDBLOCK){
							break;
						}
					}
					else if(n==0){
						break;
					}
					else{
						line[n]='\0';
						printf("|%s|",line);
					}
				}
				printf("\n");		//printf是行缓冲的，如果一直不输出换行符，第88行和101的内容就不会打印出来的
			}
		}
	}
	return 0;
}


运行输出：

accept a connection from 127.0.0.1
read:|12345||67890|

边缘触发是一种高速工作模式，编程上稍微复杂一些。但是在TCP协议中，ET模式的加速效用仍需要更多的benchmark确认。

最后把上面服务端的程序再扩充一下，server端收到client发来的数据后还要回应一段message，重要的是这个写事件也要放到epoll中。

	nfds=epoll_wait(epfd,events,20,500);
	for(i=0;i<nfds;++i){
		if(events[i].data.fd==listenfd){
			clilen=sizeof(clientaddr);
			connfd=accept(listenfd,(struct sockaddr*)&clientaddr,&clilen);
			if(connfd<0){
				perror("connfd<0");
				exit(1);
			}
			char *str=inet_ntoa(clientaddr.sin_addr);
			printf("accept a connection from %s\n",str);
			ev.data.fd=connfd;
			ev.events=EPOLLIN;
			epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
		}
		else if(events[i].events & EPOLLIN){
			sockfd = events[i].data.fd;
			printf("read:");
			if((n=read(sockfd,line,MAXLINE))<0){
				printf("read error,close connection\n");
				epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,&ev);
				close(sockfd);			
			}
			line[n]='\0';
			printf("|%s|\n",line);
			
			ev.data.fd=sockfd;
			//设置用于注册的写操作事件
			ev.events=EPOLLOUT;
			//修改sockfd上要处理的事件为EPOLLOUT
			epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
		}
		else if(events[i].events & EPOLLOUT){
			printf("ready to write\n");
			sockfd=events[i].data.fd;
			write(sockfd,line,n);
			//设置用于读操作的文件描述符
			ev.data.fd=sockfd;
			//设置用于注册的读操作事件
			ev.events=EPOLLIN;
			//修改sockfd上要处理的事件为EPOLLIN
			epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
		}
	}
}


运行输出：

accept a connection from 127.0.0.1
read:|12345|
ready to write
read:|67890|
ready to write

## select

select允许程序挂起，并等待从不止一个文件描述符的输入，即程序挂起直到有任何一个文件描述符的数据到达。select设置一个变量中的若干位，用来通知哪一个文件描述符已经有数据到达。

#include<sys/types.h>

#include<sys/time.h>

#include<unistd.h>

int select(int numfds,fd_set *readfds,fd_set *writefds,fd_set *exeptfds,struct timeval*timeout)

numfds是要检查的所有文件描述符中号码最大的加1

readfds读文件描述符集合

writefds写文件描述符集合

exeptfds异常处理文件描述符集合

timeout具体值:

NULL:永远等待,直到捕捉到信号或文件描述符已准备好为止

struct timeval 类型的指针,若等待为 timeout 时间还没有文件描符准备好,就立即返回

0:从不等待,测试所有指定的描述符并立即返回

返回值：readfds、writefds和exeptfds中准备好的fd数目，当触发time expire时会返回0，发生错误时返回-1。

下面的宏提供了处理这三种描述词组的方式:
FD_CLR(inr fd,fd_set* set)；用来清除描述词组set中相关fd 的位
FD_ISSET(int fd,fd_set *set)；用来测试描述词组set中相关fd 的位是否为真
FD_SET（int fd,fd_set*set）；用来设置描述词组set中相关fd的位
FD_ZERO（fd_set *set）；用来清除描述词组set的全部位

参数timeout为结构timeval，用来设置select()的等待时间，其结构定义如下：

struct timeval 
{ 
    time_t tv_sec;//second 
    time_t tv_usec;//minisecond  
};


理解select模型的关键在于理解fd_set,为说明方便，取fd_set长度为1字节，fd_set中的每一bit可以对应一个文件描述符fd。则1字节长的fd_set最大可以对应8个fd。

（1）执行fd_set set; FD_ZERO(&set);则set用位表示是0000,0000。

（2）若fd＝5,执行FD_SET(fd,&set);后set变为0001,0000(第5位置为1)

（3）若再加入fd＝2，fd=1,则set变为0001,0011

（4）执行select(6,&set,0,0,0)阻塞等待

（5）若fd=1,fd=2上都发生可读事件，则select返回，此时set变为0000,0011。注意：没有事件发生的fd=5被清空。

可监控的文件描述符个数取决与sizeof(fd_set)的值。我这边PC上sizeof(fd_set)＝128，每bit表示一个文件描述符，则我服务器上支持的最大文件描述符是128*8=1024。

下面的代码监听标准输入上是否有输入数据，如果有就把它输出到标准输出上。

#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
 
int main(){
    fd_set rset;
    FD_ZERO(&rset);
    FD_SET(0,&rset);
 
    struct timeval t;
    t.tv_sec=4;
    t.tv_usec=500000;
 
    int ret=select(1,&rset,NULL,NULL,&t);
    char buf[10]="";
    if(ret>0){
    if(FD_ISSET(0,&rset)){
        read(0,buf,sizeof(buf));
        write(1,buf,strlen(buf));
    }
    }
    return 0;
}


上面只是个简单示例，来个复杂一点的，select用于socket编程，实现跟epoll相同的功能。

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define MYPORT 5000
#define BACKLOG 2		//TCP层接收链接池的缓冲队列大小
#define BUF_SIZE 200	//用于读写网络数据的内存缓冲大小

int fd_A[BACKLOG];		//存放处于连接中的socket描述符
int conn_amount;		//目前的TCP连接数量

//显示目前有几个工作的TCP连接，以及相应的socket描述符
void showclient(){
	int i;
	printf("client amount:%d\nready file descriptor:",conn_amount);
	for(i=0;i<conn_amount;++i)
		printf("%d ",fd_A[i]);
	printf("\n");
}

int main(){
	int sock_fd,new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t sin_size;
	int yes=1;
	char buf[BUF_SIZE];
	int ret;
	int i;

	if((sock_fd=socket(PF_INET,SOCK_STREAM,0))==-1){
		perror("socket");
		exit(1);
	}

	if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
		perror("socket");
		exit(1);
	}

	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(MYPORT);
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	memset(server_addr.sin_zero,'\0',sizeof(server_addr.sin_zero));

	if(bind(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1){
		perror("bind");
		exit(1);
	}

	if(listen(sock_fd,BACKLOG)==-1){
		perror("listen");
		exit(1);
	}

	printf("listen on port %d\n",MYPORT);

	fd_set fdsr;
	int maxsock;		//存放在监视的最大文件描述符
	struct timeval tv;

	conn_amount=0;		//初始连接数量为0
	sin_size=sizeof(client_addr);
	maxsock=sock_fd;
	while(1){
		FD_ZERO(&fdsr);		//清空fdsr
		FD_SET(sock_fd,&fdsr);
		for(i=0;i<BACKLOG;++i){
			if(fd_A[i]!=0){
				FD_SET(fd_A[i],&fdsr);			//把准备就绪的连接全部放入fdsr中
			}
		}
		
		tv.tv_sec=30;
		tv.tv_usec=0;

		//监听集合fdsr
		ret=select(maxsock+1,&fdsr,NULL,NULL,&tv);
		if(ret<0){
			perror("select");
			break;
		}else if(ret==0){
			printf("time out\n");
			continue;
		}
		
		//逐一遍历每个连接，看其是否就绪。若是则读取其上的数据，并返回一串消息
		for(i =0;i<conn_amount;i++){
			if(FD_ISSET(fd_A[i],&fdsr)){
				ret=recv(fd_A[i],buf,sizeof(buf),0);
				char str[]="Good,very nice!\n";
				send(fd_A[i],str,sizeof(str)+1,0);
				if(ret<=0){
					printf("client [%d] close\n",i);
					close(fd_A[i]);
					FD_CLR(fd_A[i],&fdsr);
					fd_A[i]=0;
				}else{
					if(ret<BUF_SIZE)		//若数据量超过了BUF_SIZE，则截断之
						memset(&buf[ret],'\0',1);
					printf("client [%d] send:%s\n",i,buf);
				}
			}
		}

		if(FD_ISSET(sock_fd,&fdsr)){		//有新的连接请求
			new_fd=accept(sock_fd,(struct sockaddr*)&client_addr,&sin_size);
			if(new_fd<=0){
				perror("accept");
				continue;
			}

			if(conn_amount<BACKLOG){		
				fd_A[conn_amount++]=new_fd;			//把新的连接socket描述符放到fd_A数组中
				printf("accept connecton from %s\n",inet_ntoa(client_addr.sin_addr));
				if(new_fd>maxsock)			//更新maxsock
					maxsock=new_fd;
			}
			else{			//如果连接数达到了BACKLOG，则将最后到来的连接关闭掉，不处理它
				printf("max connection arrive,close the last connection\n");
				send(new_fd,"bye",4,0);
				close(new_fd);
			}
		}
		showclient();
	}

	for(i=0;i<BACKLOG;i++){
		if(fd_A[i]!=0)
			close(fd_A[i]);
	}
	exit(0);
}


在许多测试中我们会看到如果没有大量的idle-connection或者dead-connection，epoll的效率并不会比 select/poll高很多，但是当我们遇到大量的idle-connection(例如WAN环境中存在大量的慢速连接)，就会发现epoll的效率 大大高于select/poll。












