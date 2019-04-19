# 网络编程之 TCP / UDP 及其流程比较 - maopig的专栏 - CSDN博客
2011年11月19日 08:09:50[maopig](https://me.csdn.net/maopig)阅读数：1272

TCP与UDP的区别
1、基于连接与无连接
2、对系统资源的要求（TCP较多，UDP少）
3、UDP程序结构较简单
流模式与数据报模式
4、TCP保证数据正确性，UDP可能丢包
5、TCP保证数据顺序，UDP不保证
具体编程时的区别
1、socket()的参数不同
 2、UDP Server不需要调用listen和accept
3、 UDP收发数据用sendto/recvfrom函数 TCP：地址信息在connect/accept时确定
UDP：在sendto/recvfrom函数中每次均 需指定地址信息 UDP：shutdown函数无效
部分满足以下几点要求时，应该采用UDP 面向数据报方式
1、网络数据大多为短消息
2、拥有大量Client 对数据安全性无特殊要求
3、 网络负担非常重，但对响应速度要求高
4、UDP协议是不可靠的网络协议，它在通信两端各建立一个Socket,但是这两个Socket之间没有虚拟链路，这两个Socket只负责发送和接收数据报的对象!
例子：ICQ、ping
**面向连接的TCP的流方式**
![](http://hi.csdn.net/attachment/201111/19/0_1321661851FUF3.gif)
服务器程序流程（多进程）：
1、程序初始化 
2、填写本机地址信息
 3、绑定并监听一个固定的端口 
4、收到Client的连接后建立一个socket连接 产生一个新的进程与Client进行通信和信息处理 
5、子通信结束后中断与Client的连接
客户端程序流程：
1、程序初始化 
2、填写服务器地址信息
3、 连接服务器 
4、与服务器通信和信息处理 
5、通信结束后断开连接
服务器代码
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define MYPORT 3490                
#define BACKLOG 10                
void main() {
int sockfd, new_fd;            
struct sockaddr_in sa;   //本机地址     
struct sockaddr_in their_addr;//远程地址
int sin_size;
if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
perror("socket");
exit(1);
}
sa.sin_family = AF_INET;
sa.sin_port = htons(MYPORT);        
sa.sin_addr.s_addr = INADDR_ANY;    
bzero(&(sa.sin_zero), 8);            
if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
perror("bind");
exit(1);
}
if (listen(sockfd, BACKLOG) == -1) {
perror("listen");
exit(1);
}
while(1) {
sin_size = sizeof(struct sockaddr_in);
new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size))
if (new_fd == -1) {
perror("accept");
continue;
}
printf(”Got connection from %sn", inet_ntoa(their_addr.sin_addr));
if (fork() == 0) {
if (send(new_fd, "Hello, world! n", 14, 0) == -1)
perror("send");
close(new_fd);
exit(0);
}
close(new_fd);
while(waitpid(-1,NULL,WNOHANG) > 0);
}
}
客户端代码
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 3490
#define MAXDATASIZE 100
int main(int argc, char *argv[])
{
int sockfd, numbytes;
char buf[MAXDATASIZE];
struct hostent *he;
struct sockaddr_in their_addr;//服务器地址
if (argc != 2) {
fprintf(stderr,"usage: client hostnamen");
exit(1);
}
if ((he=gethostbyname(argv[1])) == NULL) {
herror("gethostbyname");
exit(1);
}
if ((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1) {
perror("socket");
exit(1);
}
their_addr.sin_family = AF_INET;
their_addr.sin_port = htons(PORT);
their_addr.sin_addr = *((struct in_addr *)he->h_addr);
bzero(&(their_addr.sin_zero), 8);
if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
perror("connect");
exit(1);
}
if ((numbytes=recv(sockfd,buf,MAXDATASIZE,0))==-1) {
perror("recv");
exit(1);
}
buf[numbytes] = '';
printf("Received: %s",buf);
close(sockfd);
return 0;
}
![](http://hi.csdn.net/attachment/201111/19/0_1321675475ZoRx.gif)
服务器程序流程（单进程）：
1、程序初始化 
2、填写本机地址信息 
3、绑定一个固定的端口 
3、收到Client的数据报后进行处理与通信
4、通信结束后断开连接
客户端程序流程：
1、程序初始化 
2、填写服务器地址信息 
3、连接服务器 与服务器通信和信息处理 //发送报文的方式建立与服务器的联系
4、通信结束后断开连接
UDP方式下服务器与客户端程序差别不大，仅第三步不同。
**服务器**
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define MYPORT 3490
void main()
{
int sockfd;
struct sockaddr_in my_addr;
struct sockaddr_in their_addr;
int sin_size, retval;
char buf[128];
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
perror("socket");
exit(1);
}
my_addr.sin_family = AF_INET;
my_addr.sin_port = htons(MYPORT);
my_addr.sin_addr.s_addr = INADDR_ANY;
bzero(&(my_addr.sin_zero), 8);
if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
perror("bind");
exit(1);
}
while(1) {
retval = recvfrom(sockfd, buf, 128, 0, (struct sockaddr *)&their_addr, &sin_size);//获取远程主机的报文，并解析
printf("Received datagram from %sn",inet_ntoa(their_addr.sin_addr));
if (retval == 0) {
perror (“recvfrom");
close(sockfd);
break;
}
retval = sendto(sockfd, buf, 128, 0, (struct sockaddr *)&their_addr, sin_size);
}
}
**客户端**
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#define PORT 3490
#define MAXDATASIZE 100
int main(int argc, char *argv[])
{
int sockfd, numbytes, sin_size;
char buf[MAXDATASIZE] = “Hello, world!”;
struct hostent *he; //
struct sockaddr_in their_addr;//服务器地址
if (argc != 2) {
fprintf(stderr,"usage: client hostnamen");
exit(1);
}
if ((he=gethostbyname(argv[1])) == NULL) {
herror("gethostbyname");
exit(1);
}
if ((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1) {
perror("socket");
exit(1);
}
their_addr.sin_family = AF_INET;
their_addr.sin_port = htons(PORT);
their_addr.sin_addr = *((struct in_addr *)he->h_addr);
bzero(&(their_addr.sin_zero), 8);
numbytes = sendto(sockfd, buf, MAXDATASIZE, 0, (struct sockaddr *)&their_addr,sizeof(their_addr))；//向服务器发送任一报文
if (numbytes == -1) {
perror(“sendto");
exit(1);
}
printf(“Send: %s",buf);
numbytes = recvfrom(sockfd, buf, MAXDATASIZE, 0, (struct sockaddr *)&their_addr, &sin_size);
if (numbytes == -1) {
perror("recvfrom");
exit(1);
}
buf[numbytes] = '';
printf("Received: %s",buf);
close(sockfd);
return 0;
}
