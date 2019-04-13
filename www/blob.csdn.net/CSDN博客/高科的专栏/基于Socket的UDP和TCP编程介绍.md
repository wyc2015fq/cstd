
# 基于Socket的UDP和TCP编程介绍 - 高科的专栏 - CSDN博客

2011年09月05日 10:05:36[高科](https://me.csdn.net/pbymw8iwm)阅读数：2323标签：[socket																](https://so.csdn.net/so/search/s.do?q=socket&t=blog)[tcp																](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)[服务器																](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)[buffer																](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)[server																](https://so.csdn.net/so/search/s.do?q=server&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=server&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)个人分类：[网络																](https://blog.csdn.net/pbymw8iwm/article/category/893044)
[
																								](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
一、概述
TCP（传输控制协议）和UDP（用户数据报协议是网络体系结构TCP/IP模型中传输层一层中的两个不同的通信协议。
TCP：传输控制协议，一种面向连接的协议，给用户进程提供可靠的全双工的字节流，TCP套接口是字节流套接口(streamsocket)的一种。
UDP：用户数据报协议。UDP是一种无连接协议。UDP套接口是数据报套接口(datagram socket)的一种。
**二、TCP和UDP介绍**
1）基本TCP客户—服务器程序设计基本框架
![](http://www.embedu.org/Column/images/Column179-1.jpg)
说明：（三路握手）
1.客户端发送一个SYN段（同步序号）指明客户打算连接的服务器端口，以及初始化序号(ISN)。
2.服务器发回包含服务器的初始序号的SYN报文段作为应答。同时，将确认序号(ACK)设置为客户的ISN加1以对客户的SYN报文段进行确认。一个SYN将占用一个序号。
3.客户必须将确认序号设置为服务器的ISN加1以对服务器的SYN报文段进行确认。
![](http://www.embedu.org/Column/images/Column179-2.jpg)
2) 基本TCP客户—服务器程序设计基本框架流程图
![](http://www.embedu.org/Column/images/Column179-3.jpg)
3) UDP和TCP的对比：
从上面的流程图比较我们可以很明显的看出UDP没有三次握手过程。
简单点说。UDP处理的细节比TCP少。UDP不能保证消息被传送到（它也报告消息没有传送到）目的地。UDP也不保证数据包的传送顺序。UDP把数据发出去后只能希望它能够抵达目的地。
TCP优缺点：
优点：
1．TCP提供以认可的方式显式地创建和终止连接。
2．TCP保证可靠的、顺序的（数据包以发送的顺序接收）以及不会重复的数据传输。
3．TCP处理流控制。
4．允许数据优先
5．如果数据没有传送到，则TCP套接口返回一个出错状态条件。
6．TCP通过保持连续并将数据块分成更小的分片来处理大数据块。—无需程序员知道
缺点： TCP在转移数据时必须创建（并保持）一个连接。这个连接给通信进程增加了开销，让它比UDP速度要慢。
UDP优缺点：
1．UDP不要求保持一个连接
2．UDP没有因接收方认可收到数据包（或者当数据包没有正确抵达而自动重传）而带来的开销。
3．设计UDP的目的是用于短应用和控制消息
4．在一个数据包连接一个数据包的基础上，UDP要求的网络带宽比TDP更小。
**三、Socket编程**
Socket接口是TCP/IP网络的API，Socket接口定义了许多函数或例程，程序员可以用它们来开发TCP/IP网络上的应用程序。要学Internet上的TCP/IP网络编程，必须理解Socket接口。
Socket接口设计者最先是将接口放在Unix操作系统里面的。如果了解Unix系统的输入和输出的话，就很容易了解Socket了。网络的Socket数据传输是一种特殊的I/O，Socket也是一种文件描述符。Socket也具有一个类似于打开文件的函数调用Socket()，该函数返回一个整型的Socket描述符，随后的连接建立、数据传输等操作都是通过该Socket实现的。常用的Socket类型有两种：流式Socket（SOCK_STREAM）和数据报式Socket（SOCK_DGRAM）。流式是一种面向连接的Socket，针对于面向连接的TCP服务应用；数据报式Socket是一种无连接的Socket，对应于无连接的UDP服务应用。
**1、socket调用库函数主要有**：
创建套接字
**Socket(af,type,protocol)**
建立地址和套接字的联系
**bind(sockid, localaddr, addrlen)**
服务器端侦听客户端的请求
**listen( Sockid,quenlen)**
建立服务器/客户端的连接 (面向连接TCP）
客户端请求连接
**Connect(sockid,destaddr, addrlen)**
服务器端等待从编号为Sockid的Socket上接收客户连接请求
**newsockid=accept(Sockid，Clientaddr, paddrlen)**
发送/接收数据
面向连接：**send(sockid, buff, bufflen)**
**recv( )**
面向无连接：**sendto(sockid,buff,…,addrlen)**
**recvfrom( )**
释放套接字
**close(sockid)**
**2、TCP/IP应用编程接口（API）**
服务器的工作流程：首先调用socket函数创建一个Socket，然后调用bind函数将其与本机地址以及一个本地端口号绑定，然后调用listen在相应的socket上监听，当accpet接收到一个连接服务请求时，将生成一个新的socket。服务器显示该客户机的IP地址，并通过新的socket向客户端发送字符串"hi,I am server!"。最后关闭该socket。
main()
{
int sock_fd,client_fd; /*sock_fd：监听socket；client_fd：数据传输socket */
struct sockaddr_in ser_addr; /* 本机地址信息 */
struct sockaddr_in cli_addr; /* 客户端地址信息 */
charmsg[MAX_MSG_SIZE];/* 缓冲区*/
ser_sockfd=**socket**(AF_INET,SOCK_STREAM,0);/*创建连接的SOCKET */
if(ser_sockfd<0)
{/*创建失败*/
fprintf(stderr,"sockerError:%s\n",strerror(errno));
exit(1);
}
/* 初始化服务器地址*/
addrlen=sizeof(structsockaddr_in);
bzero(&ser_addr,addrlen);
ser_addr.sin_family=AF_INET;
ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
ser_addr.sin_port=htons(SERVER_PORT);
if(**bind**(ser_sockfd,(struct sockaddr*)&ser_addr,sizeof(structsockaddr_in))<0)
{/*绑定失败 */
fprintf(stderr,"Bind Error:%s\n",strerror(errno));
exit(1);
}
/*侦听客户端请求*/
if(**listen**(ser_sockfd,BACKLOG)<0)
{
fprintf(stderr,"ListenError:%s\n",strerror(errno));
close(ser_sockfd);
exit(1);
}
while(1)
{/* 等待接收客户连接请求*/
cli_sockfd=**accept**(ser_sockfd,(struct sockaddr*)&        cli_addr,&addrlen);
if(cli_sockfd<=0)
{
fprintf(stderr,"AcceptError:%s\n",strerror(errno));
}
else
{/*开始服务*/
**recv**(cli_addr,msg,MAX_MSG_SIZE,0); /* 接受数据*/
printf("received a connection from %sn", inet_ntoa(cli_addr.sin_addr));
printf("%s\n",msg);/*在屏幕上打印出来*/
strcpy(msg,"hi,Iam server!");
**send**(cli_addr,msg,sizeof(msg),0); /*发送的数据*/
close(cli_addr);
}
}
**close**(ser_sockfd);
}
客户端的工作流程：首先调用socket函数创建一个Socket，然后调用bind函数将其与本机地址以及一个本地端口号绑定，请求连接服务器，通过新的socket向客户端发送字符串"hi,I am client!"。最后关闭该socket。
main()
{
intcli_sockfd;/*客户端SOCKET */
intaddrlen;
charseraddr[14];
structsockaddr_in ser_addr,/* 服务器的地址*/
cli_addr;/*客户端的地址*/
charmsg[MAX_MSG_SIZE];/* 缓冲区*/
GetServerAddr(seraddr);
cli_sockfd=**socket**(AF_INET,SOCK_STREAM,0);/*创建连接的SOCKET */
if(ser_sockfd<0)
{/*创建失败 */
fprintf(stderr,"sockerError:%s\n",strerror(errno));
exit(1);
}
/* 初始化客户端地址*/
addrlen=sizeof(structsockaddr_in);
bzero(&ser_addr,addrlen);
cli_addr.sin_family=AF_INET;
cli_addr.sin_addr.s_addr=htonl(INADDR_ANY);
cli_addr.sin_port=0;
if(**bind**(cli_sockfd,(struct sockaddr*)&cli_addr,addrlen)<0)
{
/*棒定失败 */
fprintf(stderr,"BindError:%s\n",strerror(errno));
exit(1);
}
/* 初始化服务器地址*/
addrlen=sizeof(structsockaddr_in);
bzero(&ser_addr,addrlen);
ser_addr.sin_family=AF_INET;
ser_addr.sin_addr.s_addr=inet_addr(seraddr);
ser_addr.sin_port=htons(SERVER_PORT);
if(**connect**(cli_sockfd,(structsockaddr*)&ser_addr,&addrlen)!=0)/*请求连接*/
{
/*连接失败 */
fprintf(stderr,"ConnectError:%s\n",strerror(errno));
**close**(cli_sockfd);
exit(1);
}
strcpy(msg,"hi,I amclient!");
send(sockfd,msg,sizeof(msg),0);/*发送数据*/
recv(sockfd,msg,MAX_MSG_SIZE,0); /* 接受数据*/
printf("%s\n",msg);/*在屏幕上打印出来*/
**close**(cli_sockfd);
}
**3、UDP/IP应用编程接口（API）**
服务器的工作流程：首先调用socket函数创建一个Socket，然后调用bind函数将其与本机地址以及一个本地端口号绑定，接收到一个客户端时，服务器显示该客户端的IP地址，并将字串返回给客户端。
int main(int argc,char **argv)
{
int ser_sockfd;
int len;
//int addrlen;
socklen_t addrlen;
char seraddr[100];
struct sockaddr_inser_addr;
/*建立socket*/
ser_sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(ser_sockfd<0)
{
printf("I cannotsocket success\n");
return 1;
}
/*填写sockaddr_in结构*/
addrlen=sizeof(structsockaddr_in);
bzero(&ser_addr,addrlen);
ser_addr.sin_family=AF_INET;
ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
ser_addr.sin_port=htons(SERVER_PORT);
/*绑定客户端
if(bind(ser_sockfd,(structsockaddr *)&ser_addr,addrlen)<0)
{
printf("connect");
return 1;
}
while(1)
{
bzero(seraddr,sizeof(seraddr));
len=recvfrom(ser_sockfd,seraddr,sizeof(seraddr),0,(structsockaddr*)&ser_addr,&addrlen);
/*显示client端的网络地址*/
printf("receivefrom %s\n",inet_ntoa(ser_addr.sin_addr));
/*显示客户端发来的字串*/
printf("recevce:%s",seraddr);
/*将字串返回给client端*/
sendto(ser_sockfd,seraddr,len,0,(structsockaddr*)&ser_addr,addrlen);
}
}
客户端的工作流程：首先调用socket函数创建一个Socket，填写服务器地址及端口号，从标准输入设备中取得字符串，将字符串传送给服务器端，并接收服务器端返回的字符串。最后关闭该socket。
int GetServerAddr(char * addrname)
{
printf("pleaseinput server addr:");
scanf("%s",addrname);
return 1;
}
int main(intargc,char **argv)
{
int cli_sockfd;
int len;
socklen_t addrlen;
char seraddr[14];
struct sockaddr_incli_addr;
char buffer[256];
GetServerAddr(seraddr);
/* 建立socket*/
cli_sockfd=socket(AF_INET,SOCK_DGRAM,0);
if(cli_sockfd<0)
{
printf("I cannotsocket success\n");
return 1;
}
/* 填写sockaddr_in*/
addrlen=sizeof(structsockaddr_in);
bzero(&cli_addr,addrlen);
cli_addr.sin_family=AF_INET;
cli_addr.sin_addr.s_addr=inet_addr(seraddr);
//cli_addr.sin_addr.s_addr=htonl(INADDR_ANY);
cli_addr.sin_port=htons(SERVER_PORT);
bzero(buffer,sizeof(buffer));
/* 从标准输入设备取得字符串*/
len=read(STDIN_FILENO,buffer,sizeof(buffer));
/* 将字符串传送给server端*/
sendto(cli_sockfd,buffer,len,0,(structsockaddr*)&cli_addr,addrlen);
/*接收server端返回的字符串*/
len=recvfrom(cli_sockfd,buffer,sizeof(buffer),0,(structsockaddr*)&cli_addr,&addrlen);
//printf("receivefrom %s\n",inet_ntoa(cli_addr.sin_addr));
printf("receive:%s",buffer);
close(cli_sockfd);
}
四、调试
Makefile文件为：
CC=gcc
all:server client
CFLAGS=-o
server: server.c
$(CC) $(CFLAGS) $@server.c
client: client.c
$(CC) $(CFLAGS) $@client.c
clean:
rm -f server client
在shell中执行make进行编译，make clean删除生成文件。
运行结果如下图：
![](http://www.embedu.org/Column/images/Column179-4.jpg)
![](http://www.embedu.org/Column/images/Column179-5.jpg)


