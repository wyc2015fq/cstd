# 什么是socket - 建建的博客 - CSDN博客
2017年02月20日 16:31:34[纪建](https://me.csdn.net/u013898698)阅读数：328
个人分类：[Socket学习](https://blog.csdn.net/u013898698/article/category/6694075)
我们知道进程通信的方法有管道、命名管道、信号、消息队列、共享内存、信号量，这些方法都要求通信的两个进程位于同一个主机。但是如果通信双方不在同一个主机又该如何进行通信呢？在计算机网络中我们就学过了tcp/ip协议族，其实使用tcp/ip协议族就能达到我们想要的效果，如下图（图片来源于《tcp/ip协议详解卷一》第一章1.3）
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160106212015200-534370991.png)、
图一 各协议所处层次
当然，这样做固然是可以的，但是，当我们使用不同的协议进行通信时就得使用不同的接口，还得处理不同协议的各种细节，这就增加了开发的难度，软件也不易于扩展。于是UNIX BSD就发明了socket这种东西，socket屏蔽了各个协议的通信细节，使得程序员无需关注协议本身，直接使用socket提供的接口来进行互联的不同主机间的进程的通信。这就好比操作系统给我们提供了使用底层硬件功能的系统调用，通过系统调用我们可以方便的使用磁盘（文件操作），使用内存，而无需自己去进行磁盘读写，内存管理。socket其实也是一样的东西，就是提供了tcp/ip协议的抽象，对外提供了一套接口，同过这个接口就可以统一、方便的使用tcp/ip协议的功能了。百说不如一图，看下面这个图就能明白了。
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160106213656075-1895373856.png)
图二 socket所处层次
那么，在BSD UNIX又是如何实现这层抽象的呢？我们知道unix中万物皆文件，没错，bsd在实现上把socket设计成一种文件，然后通过虚拟文件系统的操作接口就可以访问socket，而访问socket时会调用相应的驱动程序，从而也就是使用底层协议进行通信。（vsf也就是unix提供给我们的面向对象编程，如果底层设备是磁盘，就对磁盘读写，如果底层设备是socket就使用底层协议在网中进行通信，而对外的接口都是一致的）。下面再看一下socket的结构是怎样的（图片来源于《tcp/ip协议详解卷二》章节一，1.8描述符），注意：这里的socket是一个实例化之后的socket，也就是说是一个具体的通信过程中的socket，不是指抽象的socket结构，下文还会进行解释。
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160106234043965-1905612464.png)
图三 udp socket实例的结构
很明显，unix把socket设计成文件，通过描述符我们可以定位到具体的file结构体，file结构体中有个f_type属性，标识了文件的类型，如图，DTYPE_VNODE表示普通的文件DTYPE_SOCKET表示socket，当然还有其他的类型，比如管道、设备等，这里我们只关心socket类型。如果是socket类型，那么f_ops域指向的就是相应的socket类型的驱动，而f_data域指向了具体的socket结构体，socket结构体关键域有so_type,so_pcb。so_type常见的值有：
- SOCK_STREAM 提供有序的、可靠的、双向的和基于连接的字节流服务，当使用Internet地址族时使用TCP。
- SOCK_DGRAM 支持无连接的、不可靠的和使用固定大小（通常很小）缓冲区的数据报服务，当使用Internet地址族使用UDP。
- SOCK_RAW 原始套接字，允许对底层协议如IP或ICMP进行直接访问，可以用于自定义协议的开发。
so_pcb表示socket控制块，其又指向一个结构体，该结构体包含了当前主机的ip地址(inp_laddr)，当前主机进程的端口号(inp_lport)，发送端主机的ip地址(inp_faddr)，发送端主体进程的端口号(inp_fport)。so_pcb是socket类型的关键结构，不亚于进程控制块之于进程，在进程中，一个pcb可以表示一个进程，描述了进程的所有信息，每个进程有唯一的进程编号，该编号就对应pcb；socket也同时是这样，每个socket有一个so_pcb，描述了该socket的所有信息，而每个socket有一个编号，这个编号就是socket描述符。说到这里，我们发现，socket确实和进程很像，就像我们把具体的进程看成是程序的一个实例，同样我们也可以把具体的socket看成是网络通信的一个实例。
# 2、具体socket实例如何标识
我们知道具体的一个文件可以用一个路径来表示，比如/home/zzy/src_code/client.c，那么具体的socket实例我们该如何表示呢，其实就是使用上面提到的so_pcb的那几个关键属性，也就是使用so_type+ip地址+端口号。如果我们使用so_type+ip地址+端口号实例一个socket，那么互联网上的其他主机就可以与该socket实例进行通信了。所以下面我们看一下socket如何进行实例化，看看socket给我们提供了哪些接口，而我们又该如何组织这些接口
# 3、socket编程接口
## 3.1、socket接口
int socket(int protofamily, int so_type, int protocol);
- protofamily 指协议族，常见的值有：
AF_INET，指定so_pcb中的地址要采用ipv4地址类型
AF_INET6，指定so_pcb中的地址要采用ipv6的地址类型
AF_LOCAL/AF_UNIX，指定so_pcb中的地址要使用绝对路径名
当然也还有其他的协议族，用到再学习了
- so_type 指定socket的类型，也就是上面讲到的so_type字段，比较常用的类型有：
SOCK_STREAM
SOCK_DGRAM
SOCK_RAW
- protocol 指定具体的协议，也就是指定本次通信能接受的数据包的类型和发送数据包的类型，常见的值有：
IPPROTO_TCP，TCP协议
IPPROTO_UDP，UPD协议
0，如果指定为0，表示由内核根据so_type指定默认的通信协议
这里解释一下图三，图三其实是使用AF_INET,SOCK_DGRAM,IPPRTO_UDP实例化之后的一个具体的socket。
那为什么要通过这三个参数来生成一个socket描述符？
答案就是通过这三个参数来确定一组固定的操作。我们说过抽象的socket对外提供了一个统一、方便的接口来进行网络通信，但对内核来说，每一个接口背后都是及其复杂的，同一个接口对应了不同协议，而内核有不同的实现，幸运的是，如果确定了这三个参数，那么相应的接口的映射也就确定了。在实现上，BSD就把socket分类描述，每一个类别都有进行通信的详细操作，分类见下图。而对socket的分类，就好比对unix设备的分类，我们对设备write和read时，底层的驱动是有各个设备自己提供的，而socket也一样，当我们指定不同的so_type时，底层提供的通信细节也由相应的类别提供。
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160107143012543-2040015072.png)
图4 socket层次图
更详细的socket（）函数参数描述请移步：
[http://blog.csdn.net/liuxingen/article/details/44995467](http://blog.csdn.net/liuxingen/article/details/44995467)
[http://blog.csdn.net/qiuchangyong/article/details/50099927](http://blog.csdn.net/qiuchangyong/article/details/50099927)
## 3.2、bind接口
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
bind函数就是给图三种so_pcb结构中的地址赋值的接口
- sockfd   是调用socket()函数创建的socket描述符
- addr     是具体的地址
- addrlen  表示addr的长度
struct sockaddr其实是void的typedef，其常见的结构如下图（图片来源传智播客邢文鹏linux系统编程的笔记），这也是为什么需要addrlen参数的原因，不同的地址类型，其地址长度不一样：
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160107154621246-604295524.png)
图5 地址结构图
- AF_INET:
struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    in_port_t      sin_port;   /* port in network byte order */
    struct in_addr sin_addr;   /* internet address */
};
struct in_addr {
    uint32_t       s_addr;     /* address in network byte order */
};- AF_INET6:
struct sockaddr_in6 { 
    sa_family_t     sin6_family;   /* AF_INET6 */ 
    in_port_t       sin6_port;     /* port number */ 
    uint32_t        sin6_flowinfo; /* IPv6 flow information */ 
    struct in6_addr sin6_addr;     /* IPv6 address */ 
    uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */ 
};
struct in6_addr { 
    unsigned char   s6_addr[16];   /* IPv6 address */ 
};- AF_UNIX:
#define UNIX_PATH_MAX    108
struct sockaddr_un { 
    sa_family_t sun_family;               /* AF_UNIX */ 
    char        sun_path[UNIX_PATH_MAX];  /* pathname */ 
};
## 3.3、connect接口
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
这三个参数和bind的三个参数类型一直，只不过此处strcut sockaddr表示对端公开的地址。三个参数都是传入参数。connect顾名思义就是拿来建立连接的函数，只有像tcp这样面向连接、提供可靠服务的协议才需要建立连接
## 3.4、listen接口
int listen(int sockfd, int backlog)
告知内核在sockfd这个描述符上监听是否有连接到来，并设置同时能完成的最大连接数为backlog。3.6节还会继续解释这个参数。当调用listen后，内核就会建立两个队列，一个SYN队列，表示接受到请求，但未完成三次握手的连接；另一个是ACCEPT队列，表示已经完成了三次握手的队列
- sockfd 是调用socket()函数创建的socket描述符
- backlog 已经完成三次握手而等待accept的连接数
关于backlog , man listen的描述如下：
- The behavior of the backlog argument on TCP sockets changed with Linux 2.2. Now it specifies the queue length for completely established sockets waiting
 to be accepted, instead of the number of incomplete connection requests. The maximum length of the queue for incomplete sockets can be set using /proc/sys/net/ipv4/tcp_max_syn_backlog. When syncookies
 are enabled there is no logical maximum length and this setting is ignored. See tcp(7) for more information.
- If the backlog argument is greater than the value in /proc/sys/net/core/somaxconn, then it is silently truncated to that value; the default value in
 this file is 128. In kernels before 2.4.25, this limit was a hard coded value, SOMAXCONN, with the value 128.
## 3.5、accept接口
int accept(int listen_sockfd, struct sockaddr *addr, socklen_t *addrlen)
这三个参数与bind的三个参数含义一致，不过，此处的后两个参数是传出参数。在使用listen函数告知内核监听的描述符后，内核就会建立两个队列，一个SYN队列，表示接受到请求，但未完成三次握手的连接；另一个是ACCEPT队列，表示已经完成了三次握手的队列。而accept函数就是从ACCEPT队列中拿一个连接，并生成一个新的描述符，新的描述符所指向的结构体so_pcb中的请求端ip地址、请求端端口将被初始化。
从上面可以知道，accpet的返回值是一个新的描述符，我们姑且称之为new_sockfd。那么new_sockfd和listen_sockfd有和不同呢？不同之处就在于listen_sockfd所指向的结构体so_pcb中的请求端ip地址、请求端端口没有被初始化，而new_sockfd的这两个属性被初始化了。
## 3.6、listen、connect、accept流程及原理
以AF_INET,SOCK_STREAM,IPPROTO_TCP三个参数实例化的socket为例，通过一个副图来讲解这三个函数的工作流程及粗浅原理（图片改自[http://blog.csdn.net/russell_tao/article/details/9111769](http://blog.csdn.net/russell_tao/article/details/9111769)）
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160107184421450-856838651.png)
图6 listen、accept、connect流程及原理图
- 服务器端在调用listen之后，内核会建立两个队列，SYN队列和ACCEPT队列，其中ACCPET队列的长度由backlog指定。
- 服务器端在调用accpet之后，将阻塞，等待ACCPT队列有元素。
- 客户端在调用connect之后，将开始发起SYN请求，请求与服务器建立连接，此时称为第一次握手。
- 服务器端在接受到SYN请求之后，把请求方放入SYN队列中，并给客户端回复一个确认帧ACK，此帧还会携带一个请求与客户端建立连接的请求标志，也就是SYN，这称为第二次握手
- 客户端收到SYN+ACK帧后，connect返回，并发送确认建立连接帧ACK给服务器端。这称为第三次握手
- 服务器端收到ACK帧后，会把请求方从SYN队列中移出，放至ACCEPT队列中，而accept函数也等到了自己的资源，从阻塞中唤醒，从ACCEPT队列中取出请求方，重新建立一个新的sockfd，并返回。
这就是listen,accept,connect这三个函数的工作流程及原理。从这个过程可以看到，在connect函数中发生了两次握手。
更加详细的accept建立连接流程及原理请移步下面这个博文，该博文博主是个大牛，讲解的通熟易懂并且有深度：
[http://blog.csdn.net/russell_tao/article/details/9111769](http://blog.csdn.net/russell_tao/article/details/9111769)
## 3.7、发送消息接口
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
#include <sys/types.h>
#include <sys/socket.h>
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
这几个接口都比较好理解，查一下man pages就知道什么含义了，man pages中讲解的非常清楚。这里只说一下flags参数，也是摘抄自man pages。
flags:
- MSG_DONTWAIT (since Linux 2.2)，不阻塞
Enables nonblocking operation; if the operation would block, EAGAIN or EWOULDBLOCK is returned (this can also be enabled using
the O_NONBLOCK flag with the F_SETFL fcntl(2)).
- MSG_DONTROUTE，数据包不允许通过网关
Don't use a gateway to send out the packet, only send to hosts on directly connected networks. This is usually used only by
diagnostic or routing programs. This is only defined for protocol families that route; packet sockets don't.
- MSG_OOB，带外数据
Sends out-of-band data on sockets that support this notion (e.g., of type SOCK_STREAM); the underlying protocol must also sup‐
port out-of-band data.
- 其他
## 3.8、接受消息接口
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
这几个接口都比较好理解，查一下man pages就知道什么含义了，man pages中讲解的非常清楚。
# 4、socket编程流程及tcp状态变迁
先做一个说明，下面的图都不是原创，是本人收藏已久的一些原理图，来源已经不记得了，如果大家知道来源的可以留言。
socket编程的一般模型是固定的，下面我就以几幅图来说明，由于插图中已经有说明，我就不在做补充说明了。
## 4.1 c/s模式之TCP
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160107201721075-1061362000.png)![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160107202442450-451372966.png)
图8 c/s模型tcp编程流程图及tcp状态变迁图
## 4.2 c/s模式之UDP
![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160107203736371-2068566086.png)![](http://images2015.cnblogs.com/blog/733402/201601/733402-20160107203757481-2030859869.png)
图9 c/s模型udp编程流程图
参考资料：
《tcp/ip协议详解卷一、卷二》
[socket函数的domain、type、protocol解析](http://blog.csdn.net/liuxingen/article/details/44995467)
[创建socket函数的第三个参数的意义](http://blog.csdn.net/qiuchangyong/article/details/50099927)
[陶辉：](http://blog.csdn.net/russell_tao/article/details/9111769)[高性能网络编程（一）----accept建立连接](http://blog.csdn.net/russell_tao/article/details/9111769)
[什么是带外数据](http://blog.163.com/clevertanglei900@126/blog/static/111352259201122142545411/)
[Linux的SOCKET编程详解](http://blog.csdn.net/hguisu/article/details/7445768)
