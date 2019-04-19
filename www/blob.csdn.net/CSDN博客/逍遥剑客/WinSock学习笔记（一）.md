# WinSock学习笔记（一） - 逍遥剑客 - CSDN博客
2007年03月25日 22:04:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1076标签：[socket																[struct																[服务器																[网络																[internet																[stream](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[基础知识](https://blog.csdn.net/xoyojank/article/category/259553)
![](http://www.vckbase.com/document/image/paragraph.gif)** Socket（套接字）**
◆先看定义：
typedef unsigned int u_int;
typedef u_int SOCKET;
◆Socket相当于进行网络通信两端的插座，只要对方的Socket和自己的Socket有通信联接，双方就可以发送和接收数据了。其定义类似于文件句柄的定义。
◆Socket有五种不同的类型：
1、流式套接字(stream socket)
定义：
#define SOCK_STREAM 1 
流式套接字提供了双向、有序的、无重复的以及无记录边界的数据流服务，适合处理大量数据。它是面向联结的，必须建立数据传输链路，同时还必须对传输的数据进行验证，确保数据的准确性。因此，系统开销较大。
2、 数据报套接字(datagram socket)
定义：
#define SOCK_DGRAM 2 
数据报套接字也支持双向的数据流，但不保证传输数据的准确性，但保留了记录边界。由于数据报套接字是无联接的，例如广播时的联接，所以并不保证接收端是否正在侦听。数据报套接字传输效率比较高。
3、原始套接字(raw-protocol interface)
定义：
#define SOCK_RAW 3 
原始套接字保存了数据包中的完整IP头，前面两种套接字只能收到用户数据。因此可以通过原始套接字对数据进行分析。
其它两种套接字不常用，这里就不介绍了。
◆Socket开发所必须需要的文件(以WinSock V2.0为例)：
头文件：Winsock2.h
库文件：WS2_32.LIB
动态库：W32_32.DLL
![](http://www.vckbase.com/document/image/paragraph.gif)** 一些重要的定义**
1、数据类型的基本定义：这个大家一看就懂。
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
2、 网络地址的数据结构，有一个老的和一个新的的，请大家留意，如果想知道为什么，
请发邮件给Bill Gate。其实就是计算机的IP地址，不过一般不用用点分开的IP地
址，当然也提供一些转换函数。
◆ 旧的网络地址结构的定义，为一个4字节的联合：
struct in_addr {
union {
struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
struct { u_short s_w1,s_w2; } S_un_w;
u_long S_addr;
} S_un;
#define s_addr S_un.S_addr /* can be used for most tcp & ip code */
//下面几行省略,反正没什么用处。
};
其实完全不用这么麻烦，请看下面:
◆ 新的网络地址结构的定义：
非常简单，就是一个无符号长整数 unsigned long。举个例子：IP地址为127.0.0.1的网络地址是什么呢？请看定义：
#define INADDR_LOOPBACK 0x7f000001
3、 套接字地址结构
(1)、sockaddr结构：
struct sockaddr {
u_short sa_family; /* address family */
char sa_data[14]; /* up to 14 bytes of direct address */
};
sa_family为网络地址类型，一般为AF_INET，表示该socket在Internet域中进行通信，该地址结构随选择的协议的不同而变化，因此一般情况下另一个与该地址结构大小相同的sockaddr_in结构更为常用，sockaddr_in结构用来标识TCP/IP协议下的地址。换句话说，这个结构是通用socket地址结构，而下面的sockaddr_in是专门针对Internet域的socket地址结构。
(2)、sockaddr_in结构
struct sockaddr_in {
short sin_family;
u_short sin_port;
struct in_addr sin_addr;
char sin_zero[8];
};
sin _family为网络地址类型，必须设定为AF_INET。sin_port为服务端口，注意不要使用已固定的服务端口，如HTTP的端口80等。如果端口设置为0，则系统会自动分配一个唯一端口。sin_addr为一个unsigned long的IP地址。sin_zero为填充字段，纯粹用来保证结构的大小。
◆ 将常用的用点分开的IP地址转换为unsigned long类型的IP地址的函数：
unsigned long inet_addr(const char FAR * cp )
用法：
unsigned long addr=inet_addr("192.1.8.84")
◆ 如果将sin_addr设置为INADDR_ANY，则表示所有的IP地址，也即所有的计算机。
#define INADDR_ANY (u_long)0x00000000
4、 主机地址：
先看定义：
struct hostent {
char FAR * h_name; /* official name of host */
char FAR * FAR * h_aliases; /* alias list */
short h_addrtype; /* host address type */
short h_length; /* length of address */
char FAR * FAR * h_addr_list; /* list of addresses */
#define h_addr h_addr_list[0] /* address, for backward compat */
};
h_name为主机名字。
h_aliases为主机别名列表。
h_addrtype为地址类型。
h_length为地址类型。
h_addr_list为IP地址，如果该主机有多个网卡，就包括地址的列表。
另外还有几个类似的结构，这里就不一一介绍了。
5、 常见TCP/IP协议的定义：
#define IPPROTO_IP 0 
#define IPPROTO_ICMP 1 
#define IPPROTO_IGMP 2 
#define IPPROTO_TCP 6 
#define IPPROTO_UDP 17 
#define IPPROTO_RAW 255 
具体是什么协议，大家一看就知道了。
![](http://www.vckbase.com/document/image/paragraph.gif)** 套接字的属性**
为了灵活使用套接字，我们可以对它的属性进行设定。
1、 属性内容：
//允许调试输出
#define SO_DEBUG 0x0001 /* turn on debugging info recording */
//是否监听模式
#define SO_ACCEPTCONN 0x0002 /* socket has had listen() */
//套接字与其他套接字的地址绑定
#define SO_REUSEADDR 0x0004 /* allow local address reuse */
//保持连接
#define SO_KEEPALIVE 0x0008 /* keep connections alive */
//不要路由出去
#define SO_DONTROUTE 0x0010 /* just use interface addresses */
//设置为广播
#define SO_BROADCAST 0x0020 /* permit sending of broadcast msgs */
//使用环回不通过硬件
#define SO_USELOOPBACK 0x0040 /* bypass hardware when possible */
//当前拖延值
#define SO_LINGER 0x0080 /* linger on close if data present */
//是否加入带外数据
#define SO_OOBINLINE 0x0100 /* leave received OOB data in line */
//禁用LINGER选项
#define SO_DONTLINGER (int)(~SO_LINGER)
//发送缓冲区长度
#define SO_SNDBUF 0x1001 /* send buffer size */
//接收缓冲区长度
#define SO_RCVBUF 0x1002 /* receive buffer size */
//发送超时时间
#define SO_SNDTIMEO 0x1005 /* send timeout */
//接收超时时间
#define SO_RCVTIMEO 0x1006 /* receive timeout */
//错误状态
#define SO_ERROR 0x1007 /* get error status and clear */
//套接字类型
#define SO_TYPE 0x1008 /* get socket type */
2、 读取socket属性：
int getsockopt(SOCKET s, int level, int optname, char FAR * optval, int FAR * optlen)
s为欲读取属性的套接字。level为套接字选项的级别，大多数是特定协议和套接字专有的。如IP协议应为 IPPROTO_IP。
optname为读取选项的名称
optval为存放选项值的缓冲区指针。
optlen为缓冲区的长度
用法：
int ttl=0; //读取TTL值
int rc = getsockopt( s, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeof(ttl));
//来自MS platform SDK 2003
3、 设置socket属性：
int setsockopt(SOCKET s,int level, int optname,const char FAR * optval, int optlen)
s为欲设置属性的套接字。
level为套接字选项的级别，用法同上。
optname为设置选项的名称
optval为存放选项值的缓冲区指针。
optlen为缓冲区的长度
用法：
int ttl=32; //设置TTL值
int rc = setsockopt( s, IPPROTO_IP, IP_TTL, (char *)&ttl, sizeof(ttl));
![](http://www.vckbase.com/document/image/paragraph.gif)** 套接字的使用步骤**
1、启动Winsock：对Winsock DLL进行初始化，协商Winsock的版本支持并分配必要的
资源。（服务器端和客户端）
int WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData )
wVersionRequested为打算加载Winsock的版本，一般如下设置：
wVersionRequested=MAKEWORD(2,0)
或者直接赋值：wVersionRequested=2
LPWSADATA为初始化Socket后加载的版本的信息,定义如下：
typedef struct WSAData {
WORD wVersion;
WORD wHighVersion;
char szDescription[WSADESCRIPTION_LEN+1];
char szSystemStatus[WSASYS_STATUS_LEN+1];
unsigned short iMaxSockets;
unsigned short iMaxUdpDg;
char FAR * lpVendorInfo;
} WSADATA, FAR * LPWSADATA;
如果加载成功后数据为：
wVersion＝2表示加载版本为2.0。
wHighVersion＝514表示当前系统支持socket最高版本为2.2。
szDescription="WinSock 2.0"
szSystemStatus="Running"表示正在运行。
iMaxSockets＝0表示同时打开的socket最大数，为0表示没有限制。
iMaxUdpDg＝0表示同时打开的数据报最大数，为0表示没有限制。
lpVendorInfo没有使用，为厂商指定信息预留。
该函数使用方法：
WORD wVersion=MAKEWORD(2,0);
WSADATA wsData;
int nResult= WSAStartup(wVersion,&wsData);
if(nResult !=0)
{
//错误处理
}
2、创建套接字：（服务器端和客户端）
SOCKET socket( int af, int type, int protocol );
af为网络地址类型，一般为AF_INET，表示在Internet域中使用。
type为套接字类型，前面已经介绍了。
protocol为指定网络协议，一般为IPPROTO_IP。
用法：
SOCKET sock=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
if(sock==INVALID_SOCKET)
{
//错误处理
}
3、套接字的绑定：将本地地址绑定到所创建的套接字上。（服务器端和客户端）
int bind( SOCKET s, const struct sockaddr FAR * name, int namelen )
s为已经创建的套接字。
name为socket地址结构，为sockaddr结构，如前面讨论的，我们一般使用sockaddr_in
结构，在使用再强制转换为sockaddr结构。
namelen为地址结构的长度。
用法：
sockaddr_in addr;
addr. sin_family=AF_INET;
addr. sin_port= htons(0); //保证字节顺序
addr. sin_addr.s_addr= inet_addr("192.1.8.84")
int nResult=bind(s,(sockaddr*)&addr,sizeof(sockaddr));
if(nResult==SOCKET_ERROR)
{
//错误处理
}
4、 套接字的监听：（服务器端）
int listen(SOCKET s, int backlog )
s为一个已绑定但未联接的套接字。
backlog为指定正在等待联接的最大队列长度，这个参数非常重要，因为服务器一般可
以提供多个连接。
用法：
int nResult=listen(s,5) //最多5个连接
if(nResult==SOCKET_ERROR)
{
//错误处理
}
5、套接字等待连接:：（服务器端）
SOCKET accept( SOCKET s, struct sockaddr FAR * addr, int FAR * addrlen )
s为处于监听模式的套接字。
sockaddr为接收成功后返回客户端的网络地址。
addrlen为网络地址的长度。
用法：
sockaddr_in addr;
SOCKET s_d=accept(s,(sockaddr*)&addr,sizeof(sockaddr));
if(s==INVALID_SOCKET)
{
//错误处理
}
6、套接字的连结：将两个套接字连结起来准备通信。（客户端）
int connect(SOCKET s, const struct sockaddr FAR * name, int namelen )
s为欲连结的已创建的套接字。
name为欲连结的socket地址。
namelen为socket地址的结构的长度。
用法：
sockaddr_in addr;
addr. sin_family=AF_INET;
addr. sin_port=htons(0); //保证字节顺序
addr. sin_addr.s_addr= htonl(INADDR_ANY) //保证字节顺序
int nResult=connect(s,(sockaddr*)&addr,sizeof(sockaddr));
if(nResult==SOCKET_ERROR)
{
//错误处理
}
7、套接字发送数据：（服务器端和客户端）
int send(SOCKET s, const char FAR * buf, int len, int flags )
s为服务器端监听的套接字。
buf为欲发送数据缓冲区的指针。
len为发送数据缓冲区的长度。
flags为数据发送标记。
返回值为发送数据的字符数。
◆这里讲一下这个发送标记，下面8中讨论的接收标记也一样：
flag取值必须为0或者如下定义的组合：0表示没有特殊行为。
#define MSG_OOB 0x1 /* process out-of-band data */
#define MSG_PEEK 0x2 /* peek at incoming message */
#define MSG_DONTROUTE 0x4 /* send without using routing tables */
MSG_OOB表示数据应该带外发送，所谓带外数据就是TCP紧急数据。
MSG_PEEK表示使有用的数据复制到缓冲区内，但并不从系统缓冲区内删除。
MSG_DONTROUTE表示不要将包路由出去。
用法：
char buf[]="xiaojin";
int nResult=send(s,buf,strlen(buf));
if(nResult==SOCKET_ERROR)
{
//错误处理
}
8、 套接字的数据接收：（客户端）
int recv( SOCKET s, char FAR * buf, int len, int flags )
s为准备接收数据的套接字。
buf为准备接收数据的缓冲区。
len为准备接收数据缓冲区的大小。
flags为数据接收标记。
返回值为接收的数据的字符数。
用法：
char mess[1000];
int nResult =recv(s,mess,1000,0);
if(nResult==SOCKET_ERROR)
{
//错误处理
}
9、中断套接字连接：通知服务器端或客户端停止接收和发送数据。（服务器端和客户端）
int shutdown(SOCKET s, int how)
s为欲中断连接的套接字。
How为描述禁止哪些操作，取值为：SD_RECEIVE、SD_SEND、SD_BOTH。
#define SD_RECEIVE 0x00
#define SD_SEND 0x01
#define SD_BOTH 0x02
用法：
int nResult= shutdown(s,SD_BOTH);
if(nResult==SOCKET_ERROR)
{
//错误处理
}
10、 关闭套接字：释放所占有的资源。（服务器端和客户端）
int closesocket( SOCKET s )
s为欲关闭的套接字。
用法：
int nResult=closesocket(s);
if(nResult==SOCKET_ERROR)
{
//错误处理
}
