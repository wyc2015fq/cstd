# 《UNIX网络编程》笔记——套接字选项 - yixianfeng41的专栏 - CSDN博客
2017年03月10日 17:45:15[第2梦](https://me.csdn.net/yixianfeng41)阅读数：725
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
## **一、操纵套接字选项的方法**
### **1、getsockopt和setsockopt函数**
这两个函数仅用于套接字
```
#include <sys/socket.h>   
int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);  
int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen); 
//if success return 0,else return -1
```
参数：  
- sock：指向一个的打开的套接字描述符，也就是将要被设置或者获取选项的套接字。
- level：所在协议，或为通用套接字，或为某个特定协议（例如IPv4,IPv6、TCP或SCTP）。  
- optname：需要访问的选项名。  
- optval：对于getsockopt()，把已经获取的选项值存放在*optval中。对于setsockopt()，从*optval中取得选项待设置的新值。   
- optlen：对于getsockopt()，作为入口参数时，选项值的最大长度。作为出口参数时，选项值的实际长度。对于setsockopt()，现选项的长度。
返回说明： 
成功执行时，返回0；失败返回-1，errno被设为以下的某个值 ：     
- EBADF：sock不是有效的文件描述词
- EFAULT：optval指向的内存并非有效的进程空间
- EINVAL：在调用setsockopt()时，optlen无效
- ENOPROTOOPT：指定的协议层不能识别选项
- ENOTSOCK：sock描述的不是套接字 
### **2、fcntl函数**
```
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);         
int fcntl(int fd, int cmd, struct flock *lock);
```
fcntl函数提供了与网络编程相关的如下特性：
- 非阻塞式IO。通过使用F_SETFL命令设置O_NONBLOCK文件状态标志，我们可以把一个套接字设置为非阻塞型。  
- 信号驱动IO。通过使用F_SETFL命令设置O_ASYNC文件状态标志，我们可以把一个套接字设置成一旦其状态发生变化，内核就产生一个SIGIO信号。  
- F_SETOWN命令允许我们指定用于接收SIGIO和SIGURG信号的套接字属主。
## **二、选项说明**
level有SOL_SOCKET,IPPROTO_IP,IPPROTO_ICMPV6，IPPROTO_IPV6，IPPROTO_TCP,IPPROTO_SCTP,我们主要关注一下常用的SOL_SOCKET,IPPROTO_IP，IPPROTO_TCP这三个level的选项。
### **1、SOL_SOCKET**
|选项名|说明|数据类型|
|----|----|----|
|SO_BROADCAST|允许发送广播数据|int|
|SO_DEBUG|允许调试|int|
|SO_DONTROUTE|不查找路由|int|
|SO_ERROR|获得套接字错误|int|
|SO_KEEPALIVE|保持连接|int|
|SO_LINGER|延迟关闭连接|struct linger|
|SO_OOBINLINE|带外数据放入正常数据流|int|
|SO_RCVBUF|接收缓冲区大小|int|
|SO_SNDBUF|发送缓冲区大小|int|
|SO_RCVLOWAT|接收缓冲区下限|int|
|SO_SNDLOWAT|发送缓冲区下限|int|
|SO_RCVTIMEO|接收超时|struct timeval|
|SO_SNDTIMEO|发送超时|struct timeval|
|SO_REUSERADDR|允许重用本地地址和端口|int|
|SO_TYPE|获得套接字类型|int|
|SO_BSDCOMPAT|与BSD系统兼容|int|
#### **1）SO_KEEPALIVE说明**
给一个TCP套接字设置保持存活选项后，如果2小时内在该套接字的任何一方向上都没有数据交换，TCP就自动给对端发送一个保持存活探测分节。这是一个对端必须相应的TCP分节，它会导致以下3种情况之一。
- 对端以期望的ACK响应。应用进程得不到通知（因为一切正常）。在又经过仍无动静的2小时后，TCP将发出另一个探测分节。 
- 对端以RST响应，它告知本端TCP：对端已崩溃且已重新启动。该套接字的待处理错误被置为ECONNRESET，套接字本身则被关闭。
- 对端对保持存活探测分节没有任何响应。 如果根本没有对TCP的探测分节的响应，该套接字的待处理错误就被置为ETIMEOUT，套接字本身则被关闭。然而如果该套接字收到一个ICMP错误作为某个探测分节的响应，那就返回响应的错误，套接字本身也被关闭。
本选项的功能是检测对端主机是否崩溃或变的不可达（譬如拨号调制解调器连接掉线，电源发生故障等等）。如果对端进程崩溃，它的TCP将跨连接发送一个FIN，这可以通过调用select很容易的检测到。
本选项一般由服务器使用，不过客户也可以使用。服务器使用本选项时因为他们花大部分时间阻塞在等待穿越TCP连接的输入上，也就是说在等待客户的请求。然而如果客户主机连接掉线，电源掉电或者系统崩溃，服务器进程将永远不会知道，并将继续等待永远不会到达的输入。我们称这种情况为半开连接。保持存活选项将检测出这些半开连接并终止他们。
对于另一端发生的各种事件，tcp连接采用的各种检测方法的汇总如下：
|情形|对端进程崩溃|对端主机崩溃|对端主机不可达|
|----|----|----|----|
|本端TCP正主动发送数据|对端tcp发送一个FIN,这通过使用select判断可读条件立即能检测出来。如果本端发送另外一个字节，对端tcp就以RST响应。如果在本端tcp收到RST之后应用进程仍试图写套接字，我们的套接字实现就给该进程发送一个SIGPIPE信号|本端tcp将超时，且套接字的待处理错误被设置为ETIMEDOUT|本端tcp将超时，且套接字的待处理错误被设置为EHOSTUNREACH|
|本端TCP正主动接收数据|对端TCP将发送一个FIN，我们将把它作为一个（可能过早的）EOF读入|我们将停止接收数据|我们将停止接收数据|
|连接空闲，保持存活选项已设置|对端tcp发送一个FIN,这通过使用select判断可读条件立即能检测出来|在毫无动静两个小时后，发送9个保持存活探测分节，然后套接字的待处理错误被设置为ETIMEDOUT|在毫无动静两个小时后，发送9个保持存活探测分节，然后套接字的待处理错误被设置为EHOSTUNREACH|
|连接空闲，保持存活选项未设置|对端tcp发送一个FIN,这通过使用select判断可读条件立即能检测出来|无|无|
#### **2）SO_LINGER**
本选项指定close函数对面向连接的协议（例如TCP和SCTP，但不是UDP）如何操作。默认操作是close立即返回，但是如果有数据残留在套接字发送缓冲区中，系统将试着把这些数据发送给对端。
SO_LINGER如果选择此选项，close或 shutdown将等到所有套接字里排队的消息成功发送或到达延迟时间后才会返回。否则，调用将立即返回。
#### **3）SO_RCVBUF与 SO_SNDBUF说明**
SO_RCVBUF和SO_SNDBUF每个套接口都有一个发送缓冲区和一个接收缓冲区，使用这两个套接口选项可以改变缺省缓冲区大小。
```
// 接收缓冲区
int nRecvBuf=32*1024;         //设置为32K
setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
```
```
//发送缓冲区
int nSendBuf=32*1024;//设置为32K
setsockopt(s,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
```
注意：当设置TCP套接口接收缓冲区的大小时，函数调用顺序是很重要的，因为三次握手建立连接时，SYN消息同时也会确定窗口大小。对于客户端，SO_RCVBUF选项必须在**connect**之前设置；对于服务器，SO_RCVBUF选项必须在**listen**前设置。
### **2、IPPROTO_IP**
|选项名|说明|数据类型|
|----|----|----|
|IP_HDRINCL|在数据包中包含IP首部|int|
|IP_OPTINOS|IP首部选项|int|
|IP_RECVDSTADDR|返回目的IP地址|int|
|IP_RECVIF|返回接收接口索引|int|
|IP_TOS|服务类型|int|
|IP_TTL|生存时间|int|
### **3、IPPROTO_TCP**
|选项名|说明|数据类型|
|----|----|----|
|TCP_MAXSEG|TCP最大数据段的大小|int|
|TCP_NODELAY|不使用Nagle算法|int|
### 参考
《unix 网络编程 卷一》
