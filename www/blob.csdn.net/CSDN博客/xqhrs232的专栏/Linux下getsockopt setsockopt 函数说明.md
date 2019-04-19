# Linux下getsockopt/setsockopt 函数说明 - xqhrs232的专栏 - CSDN博客
2017年01月05日 22:33:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：266
原文地址::[http://blog.csdn.net/xioahw/article/details/4056514](http://blog.csdn.net/xioahw/article/details/4056514)
相关文章
1、[linux下getsockopt和setsockopt详解及测试](http://blog.csdn.net/hongkangwl/article/details/24105833)----[http://blog.csdn.net/hongkangwl/article/details/24105833](http://blog.csdn.net/hongkangwl/article/details/24105833)
2、[linux套接字选项—getsockopt和setsockopt](http://blog.csdn.net/yuanzhangmei1/article/details/8230993)----[http://blog.csdn.net/yuanzhangmei1/article/details/8230993](http://blog.csdn.net/yuanzhangmei1/article/details/8230993)
3、[getsockopt,setsockopt用法小结](http://blog.chinaunix.net/uid-15014334-id-3338942.html) ----[http://blog.chinaunix.net/uid-15014334-id-3338942.html](http://blog.chinaunix.net/uid-15014334-id-3338942.html)

Linux下getsockopt/setsockopt 函数说明
【 getsockopt/setsockopt系统调用】   
功能描述： 
获取或者设置与某个套接字关联的选 项。选项可能存在于多层协议中，它们总会出现在最上面的套接字层。当操作套接字选项时，选项位于的层和选项的名称必须给出。为了操作套接字层的选项，应该 将层的值指定为SOL_SOCKET。为了操作其它层的选项，控制选项的合适协议号必须给出。例如，为了表示一个选项由TCP协议解析，层应该设定为协议 号TCP。
用法： 
#include <sys/types.h>
#include <sys/socket.h>
int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
参数：   
sock：将要被设置或者获取选项的套接字。
level：选项所在的协议层。
optname：需要访问的选项名。
optval：对于getsockopt()，指向返回选项值的缓冲。对于setsockopt()，指向包含新选项值的缓冲。
optlen：对于getsockopt()，作为入口参数时，选项值的最大长度。作为出口参数时，选项值的实际长度。对于setsockopt()，现选项的长度。
返回说明：   
成功执行时，返回0。失败返回-1，errno被设为以下的某个值   
EBADF：sock不是有效的文件描述词
EFAULT：optval指向的内存并非有效的进程空间
EINVAL：在调用setsockopt()时，optlen无效
ENOPROTOOPT：指定的协议层不能识别选项
ENOTSOCK：sock描述的不是套接字
首先知道setsockopt()函数的KEEPALIVE属性是周期地[测试](http://lib.csdn.net/base/softwaretest)连接是否仍存活， 
我上网查了很多资料还是不知道如何使用， 
最后硬着头皮自己写了一个服务器端和一个客户端的套接字连接 
分别设置了两端的KEEPALIVE属性为打开 
服务器端： 
#include<stdio.h> 
#include<stdlib.h> 
#include<sys/socket.h> 
#include<error.h> 
#include<string.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<sys/wait.h> 
#include<arpa/inet.h> 
#include<unistd.h> 
#define SERVPORT 6000 //设定服务器服务端口为6000 
#define MAX_LISTEN_SOCK_NUM 20 //设定可监听套接字的最大个数为20 
int main() 
{ 
//sockfd为本地监听套接字标识符，client_fd为客户端套接字标识符 
int sockfd,client_fd; 
struct sockaddr_in my_addr; 
struct sockaddr_in client_addr; 
//创建本地监听套接字 
if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){ 
perror("套接字创建失败!/n"); 
exit(1); 
} 
//设置套接字的属性使它能够在计算机重启的时候可以再次使用套接字的端口和IP 
int err,sock_reuse=1; 
err=setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(char *)&sock_reuse,sizeof(sock_reuse)); 
if(err!=0){ 
printf("套接字可重用设置失败!/n"); 
exit(1); 
} 
my_addr.sin_family=AF_INET; 
my_addr.sin_port=htons(SERVPORT); 
my_addr.sin_addr.s_addr=INADDR_ANY; 
bzero(&(my_addr.sin_zero),8); 
//绑定套接字 
if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1){ 
perror("绑定失败!/n"); 
exit(1); 
} 
//设置监听 
if((listen(sockfd,MAX_LISTEN_SOCK_NUM))==-1){ 
perror("设置监听失败!/n"); 
exit(1); 
} 
printf("套接字进入监听状态，等待请求连接：/n"); 
//int time_to_quit=1; 
//while(time_to_quit){ //可以通过设置time_to_quit来主动关闭服务器端 
while(1){ 
//有连接请求时进行连接 
socklen_t sin_size=sizeof(struct sockaddr_in); 
if((client_fd=accept(sockfd,(struct sockaddr *)&client_addr,&sin_size))==-1){ 
perror("接受连接失败!/n"); 
continue; 
} 
int opt; 
socklen_t len=sizeof(int); 
if((getsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,(char*)&opt,&len))==0){ 
printf("SO_KEEPALIVE Value: %d/n", opt); 
} 
printf("接到一个来自%s的连接/n",inet_ntoa(client_addr.sin_addr)); 
//创建子进程来处理已连接的客户端套接字 
if(send(client_fd,"您好，您已经连接成功!/n",50,0)==-1){ 
perror("发送通知信息失败!/n"); 
exit(0); 
} 
} 
close(client_fd); 
return 0; 
} 
客户端： 
#include<stdio.h> 
#include<stdlib.h> 
#include<error.h> 
#include<string.h> 
#include<netdb.h> 
#include<sys/types.h> 
#include<netinet/in.h> 
#include<sys/socket.h> 
#include<unistd.h> 
#include<arpa/inet.h> 
#define MAXDATASIZE 100 /*每次最[大数据](http://lib.csdn.net/base/hadoop)传输量*/ 
int main() 
{ 
int sockfd,nbytes,serv_port; 
char buf_serv_ip[16],buf[26]; 
struct sockaddr_in serv_addr; 
if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){ 
perror("创建套接字失败!/n"); 
exit(1); 
} 
//创建套接字成功后设置其可重用的属性 
int KeepAlive=1; 
socklen_t KPlen=sizeof(int); 
if(setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,(char *)&KeepAlive,KPlen)!=0){ 
perror("设置周期测试连接是否仍存活失败!/n"); 
exit(1); 
} 
printf("请输入要连接主机的IP地址：/n"); 
scanf("%s",buf_serv_ip); 
printf("请输入要连接主机的端口号:/n"); 
scanf("%d",&serv_port); 
serv_addr.sin_family=AF_INET; 
serv_addr.sin_addr.s_addr=inet_addr(buf_serv_ip); 
serv_addr.sin_port=htons(serv_port); 
bzero(&(serv_addr.sin_zero),8); 
if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr))==-1){ 
perror("连接服务器失败!/n"); 
exit(1); 
} 
printf("连接服务器成功!/n"); 
//在此处可以先接受判断将要接受数据的长度再创建数组 
if((nbytes=recv(sockfd,buf,26,0))==-1){ 
perror("接受数据失败!/n"); 
exit(1); 
} 
buf[nbytes]='/0'; 
printf("接受的数据为:%s/n",buf); 
close(sockfd); 
return 0; 
} 
分别在两个终端运（在同一个主机上）行：先启动服务器端;再启动客户端,向服务器端请求连接，连接成功后，客户端套接字关闭，服务器端套接字始终打开，等待两个小时多也没有反应， 
肯请高手指点： 
（1）这样做能达到预期报告连接是否仍存在的效果吗， 
（2）不能的话应该如何做， 
（3）还有getsockopt()的相关功能又是什么哪？在上面的程序中没有用到该函数 
（4）看了unix网络编程上的一点关于该问题的解释还是不明白，因为她提到要两个小时才回应，但等了撒小时也没有反应，她还说可以缩短时间，我却不知道如何缩短
getsockopt和setsockopt　
int getsockopt(int sockfd,int level,int optname,void *optval,socklen_t *optlen) 
int setsockopt(int sockfd,int level,int optname,const void *optval,socklen_t *optlen)
level指定控制套接字的层次.可以取三种值: 
1)SOL_SOCKET:通用套接字选项. 
2)IPPROTO_IP:IP选项. 
3)IPPROTO_TCP:TCP选项.　
optname指定控制的方式(选项的名称),我们下面详细解释　
optval获得或者是设置套接字选项.根据选项名称的数据类型进行转换　
选项名称　　　　　　　　说明　　　　　　　　　　　　　　　　　　数据类型 
======================================================================== 
　　　　　　　　　　　　SOL_SOCKET 
------------------------------------------------------------------------ 
SO_BROADCAST　　　　　　允许发送广播数据　　　　　　　　　　　　int 
SO_DEBUG　　　　　　　　允许调试　　　　　　　　　　　　　　　　int 
SO_DONTROUTE　　　　　　不查找路由　　　　　　　　　　　　　　　int 
SO_ERROR　　　　　　　　获得套接字错误　　　　　　　　　　　　　int 
SO_KEEPALIVE　　　　　　保持连接　　　　　　　　　　　　　　　　int 
SO_LINGER　　　　　　　 延迟关闭连接　　　　　　　　　　　　　　struct linger 
SO_OOBINLINE　　　　　　带外数据放入正常数据流　　　　　　　　　int 
SO_RCVBUF　　　　　　　 接收缓冲区大小　　　　　　　　　　　　　int 
SO_SNDBUF　　　　　　　 发送缓冲区大小　　　　　　　　　　　　　int 
SO_RCVLOWAT　　　　　　 接收缓冲区下限　　　　　　　　　　　　　int 
SO_SNDLOWAT　　　　　　 发送缓冲区下限　　　　　　　　　　　　　int 
SO_RCVTIMEO　　　　　　 接收超时　　　　　　　　　　　　　　　　struct timeval 
SO_SNDTIMEO　　　　　　 发送超时　　　　　　　　　　　　　　　　struct timeval 
SO_REUSERADDR　　　　　 允许重用本地地址和端口　　　　　　　　　int 
SO_TYPE　　　　　　　　 获得套接字类型　　　　　　　　　　　　　int 
SO_BSDCOMPAT　　　　　　与BSD系统兼容　　　　　　　　　　　　　 int 
========================================================================
　　　　　　　　　　　　IPPROTO_IP 
------------------------------------------------------------------------
IP_HDRINCL　　　　　　　在数据包中包含IP首部　　　　　　　　　　int 
IP_OPTINOS　　　　　　　IP首部选项　　　　　　　　　　　　　　　int 
IP_TOS　　　　　　　　　服务类型 
IP_TTL　　　　　　　　　生存时间　　　　　　　　　　　　　　　　int 
========================================================================
　　　　　　　　　　　　IPPRO_TCP 
------------------------------------------------------------------------
TCP_MAXSEG　　　　　　　TCP最大数据段的大小　　　　　　　　　　 int 
TCP_NODELAY　　　　　　 不使用Nagle[算法](http://lib.csdn.net/base/datastructure)　　　　　　　　　　　　 int 
========================================================================
返回说明：   
成功执行时，返回0。失败返回-1，errno被设为以下的某个值   
EBADF：sock不是有效的文件描述词
EFAULT：optval指向的内存并非有效的进程空间
EINVAL：在调用setsockopt()时，optlen无效
ENOPROTOOPT：指定的协议层不能识别选项
ENOTSOCK：sock描述的不是套接字
SO_RCVBUF和SO_SNDBUF每个套接口都有一个发送缓冲区和一个接收缓冲区，使用这两个套接口选项可以改变缺省缓冲区大小。
// 接收缓冲区
int nRecvBuf=32*1024;         //设置为32K
setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
//发送缓冲区
int nSendBuf=32*1024;//设置为32K
setsockopt(s,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
注意：
当设置TCP套接口接收缓冲区的大小时，函数调用顺序是很重要的，因为TCP的窗口规模选项是在建立连接时用SYN与对方互换得到的。对于客户，SO_RCVBUF选项必须在connect之前设置；对于服务器，SO_RCVBUF选项必须在listen前设置。
