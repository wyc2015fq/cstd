# linux中select()函数分析及Linux下用select查询串口数据 - xqhrs232的专栏 - CSDN博客
2012年03月15日 16:37:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4605
原文地址::[http://tieba.baidu.com/p/94875093](http://tieba.baidu.com/p/94875093)
linux中select()函数分析
select()的机制中提供一fd_set的数据结构，实际上是一long类型的数组， 每一个数组元素都能与一打开的文件句柄（不管是Socket句柄,还是其他 文件或命名管道或设备句柄）建立联系，建立联系的工作由程序员完成， 当调用select()时，由内核根据IO状态修改fd_set的内容，由此来通知执 行了select()的进程哪一Socket或文件可读，下面具体解释： 
#include  
#include  
#include  
int select(nfds, readfds, writefds, exceptfds, timeout)  
int nfds;  
fd_set *readfds, *writefds, *exceptfds;  
struct timeval *timeout;  
ndfs：select监视的文件句柄数，视进程中打开的文件数而定,一般设为呢要监视各文件 中的最大文件号加一。  
readfds：select监视的可读文件句柄集合。  
writefds: select监视的可写文件句柄集合。  
exceptfds：select监视的异常文件句柄集合。  
timeout：本次select()的超时结束时间。（见/usr/sys/select.h， 可精确至百万分之一秒！）  
当readfds或writefds中映象的文件可读或可写或超时，本次select() 就结束返回。程序员利用一组系统提供的宏在select()结束时便可判 断哪一文件可读或可写。对Socket编程特别有用的就是readfds。 几只相关的宏解释如下： 
FD_ZERO(fd_set *fdset)：清空fdset与所有文件句柄的联系。  
FD_SET(int fd, fd_set *fdset)：建立文件句柄fd与fdset的联系。  
FD_CLR(int fd, fd_set *fdset)：清除文件句柄fd与fdset的联系。  
FD_ISSET(int fd, fdset *fdset)：检查fdset联系的文件句柄fd是否  
可读写，>0表示可读写。  
（关于fd_set及相关宏的定义见/usr/include/sys/types.h）  
这样，你的socket只需在有东东读的时候才读入，大致如下：  
...  
int sockfd;  
fd_set fdR;  
struct timeval timeout = ..;  
...  
for(;;) {  
FD_ZERO(&fdR);  
FD_SET(sockfd, &fdR);  
switch (select(sockfd + 1, &fdR, NULL, &timeout)) {  
case -1:  
error handled by u;  
case 0:  
timeout hanled by u;  
default:  
if (FD_ISSET(sockfd)) {  
now u read or recv something;  
/* if sockfd is father and  
server socket, u can now  
accept() */  
}  
}  
}  
所以一个FD_ISSET(sockfd)就相当通知了sockfd可读。  
至于struct timeval在此的功能，请man select。不同的timeval设置 使使select()表现出超时结束、无超时阻塞和轮询三种特性。由于 
timeval可精确至百万分之一秒，所以Windows的SetTimer()根本不算 什么。你可以用select()做一个超级时钟。  
FD_ACCEPT的实现？依然如上，因为客户方socket请求连接时，会发送  
连接请求报文，此时select()当然会结束，FD_ISSET(sockfd)当然大  
于零，因为有报文可读嘛！至于这方面的应用，主要在于服务方的父  
Socket，你若不喜欢主动accept()，可改为如上机制来accept()。  
至于FD_CLOSE的实现及处理，颇费了一堆cpu处理时间，未完待续。  
--  
讨论关于利用select()检测对方Socket关闭的问题：  
仍然是本地Socket有东东可读，因为对方Socket关闭时，会发一个关闭连接  
通知报文，会马上被select()检测到的。关于TCP的连接（三次握手）和关  
闭（二次握手）机制，敬请参考有关TCP/IP的书籍。  
不知是什么原因，UNIX好象没有提供通知进程关于Socket或Pipe对方关闭的  
信号，也可能是cpu所知有限。总之，当对方关闭，一执行recv()或read()，  
马上回返回-1，此时全局变量errno的值是115，相应的sys_errlist[errno]  
为"Connect refused"（请参考/usr/include/sys/errno.h）。所以，在上  
篇的for(;;)...select()程序块中，当有东西可读时，一定要检查recv()或  
read()的返回值，返回-1时要作出关断本地Socket的处理，否则select()会  
一直认为有东西读，其结果曾几令cpu伤心欲断针脚。不信你可以试试：不检  
查recv()返回结果，且将收到的东东（实际没收到）写至标准输出...  
在有名管道的编程中也有类似问题出现。具体处理详见拙作：发布一个有用  
的Socket客户方原码。  
至于主动写Socket时对方突然关闭的处理则可以简单地捕捉信号SIGPIPE并作  
出相应关断本地Socket等等的处理。SIGPIPE的解释是：写入无读者方的管道。  
在此不作赘述，请详man signal。  
以上是cpu在作tcp/ip数据传输实验积累的经验，若有错漏，请狂炮击之。  
唉，昨天在hacker区被一帮孙子轰得差点儿没短路。ren cpu(奔腾的心) z80  
补充关于select在异步(非阻塞)connect中的应用,刚开始搞socket编程的时候  
我一直都用阻塞式的connect,非阻塞connect的问题是由于当时搞proxy scan  
而提出的呵呵  
通过在网上与网友们的交流及查找相关FAQ,总算知道了怎么解决这一问题.同样  
用select可以很好地解决这一问题.大致过程是这样的:  
1.将打开的socket设为非阻塞的,可以用fcntl(socket, F_SETFL, O_NDELAY)完  
成(有的系统用FNEDLAY也可).  
2.发connect调用,这时返回-1,但是errno被设为EINPROGRESS,意即connect仍旧  
在进行还没有完成.  
3.将打开的socket设进被监视的可写(注意不是可读)文件集合用select进行监视,  
如果可写,用  
getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, sizeof(int));  
来得到error的值,如果为零,则connect成功. 
Linux下用select查询串口数据 
Linux下直接用read读串口可能会造成堵塞，或数据读出错误。然而用select先查询com口，再用read去读就可以避免，并且当com口延时时，程序可以退出，这样就不至于由于com口堵塞，程序就死了。我的代码如下：
bool ReadDevice( int hComm, unsigned long uLen, char* pData ) 
{  
int nread = 0; 
char inbuf[uLen]; 
char buff[uLen]; 
memset( inbuff, '\0', uLen ); 
memset( buff, '\0', uLen ); 
fd_set readset; 
struct timeval tv; 
int MaxFd = 0; 
int c = 0; 
int z; 
do 
{ 
FD_ZERO( &readset ); 
if( hComm >= 0 ) 
FD_SET( hComm, &readset ); 
MaxFd = hComm + 1; 
tv.tv_sec = 0; 
tv.tv_usec = 500000; 
do 
{ 
z = select( MaxFd, &readset, 0, 0, &tv); 
}while( z==-1 && errno==EINTR ); 
if( z == -1 ) 
printf("select(2)\n"); 
if( z == 0 ) 
{ 
hComm = -1; 
} 
if( hComm>=0 && FD_ISSET(hComm, &readset) ) 
{ 
z = read( hComm, buff, uLen - c ); 
c += z; 
if( z == -1 ) 
{ 
hComm = -1; 
} 
if( z > 0 ) 
{ 
buff[ z + 1 ] = '\0'; 
strcat( inbuff, buff ); 
memset( buff, 0x00, uLen ); 
} 
else 
{ 
hComm = -1; 
} 
} 
}while( hComm >= 0 ); 
memcpy( pData, inbuff, c ); 
return true; 
}
