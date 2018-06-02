
#ifndef _NETINIT_H_
#define _NETINIT_H_

#include <winsock.h>

#ifndef _WINSOCKAPI_           // 没有包含winsock.h
#define _WINSOCKAPI_           // 避免再包含winsock.h
#ifndef _WINSOCK2API_      // 没有包含winsock2.h
#define _WINSOCK2API_      // 避免再包含winsock2.h
#include <winsock2.h>
#pragma comment(lib, "ws32_2.lib")
#endif
#endif
#pragma comment(lib,"ws2_32.lib")

typedef unsigned char BYTE;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int InitSocket(BYTE minorVer, BYTE majorVer);

#define __INITSOCKET22()  InitSocket(2, 2)

// extern const int __wasInitSocket = __INITSOCKET22();

// 按a.b.c.d:port 格式设定IP端口号, 并将主机字节顺序转换为网络字节顺序
struct sockaddr_in make_sockaddr_in(const char* cp);

int net_sendtoAll(SOCKET* f, int nClientNum, const char* buf, int len, int flags);
int net_safesend(SOCKET s, const char* buf, size_t n, int flags);
int net_saferecv(SOCKET s, char* buf, size_t n, int flags);
int net_safesendtoAll(SOCKET* f, int nClientNum, const char* buf, int len, int flags);
int safesendMattoAll(SOCKET* f, int nClientNum, int h, int w, const char* A, int al, int flags);
int net_saferecvMat(SOCKET s, int h, int w, char* A, int al, int flags);

#define RFIND_OUT(f, l, minchar, maxchar)  for (; f!=l && (l[-1]<minchar|| l[-1]>maxchar); --l) {}
#define RFIND_IN(f, l, minchar, maxchar)  for (; f!=l && !(l[-1]<minchar|| l[-1]>maxchar); --l) {}

#define MAKE_SOCKADDR_IN(_OUTADDR, _FAMILY, _PORT, _ADDR)                     \
  do {                                                                          \
    (_OUTADDR).sin_family = _FAMILY;                                            \
    (_OUTADDR).sin_port = htons(_PORT);                                         \
    (_OUTADDR).sin_addr.S_un.S_addr = _ADDR;                                    \
  } while(0)

// 创建套接字(AF_INET,SOCK_STREAM,IPPROTO_TCP)
// 使用前创建一个新的套接字
// af: 通信发生的区域或网络地址类型，一般为AF_INET，表示在Internet域中使用
// type: 套接字类型
// procotol: 使用的特定协议
#define NET_OPEN_TCP() (socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))

// 关闭套接字：释放所占有的资源。（服务器端和客户端）
#define NET_CLOSE(_SOCKET) (( _SOCKET != INVALID_SOCKET ) ? (closesocket( _SOCKET )) : ((_SOCKET)=0))

// 中断套接字连接：通知服务器端或客户端停止接收和发送数据。（服务器端和客户端）
// How为描述禁止哪些操作，取值为：SD_RECEIVE、SD_SEND、SD_BOTH。
#define NET_SHUTDOWN(_SOCKET, _HOW)     shutdown(_SOCKET, _HOW)

// 套接字的连结：将两个套接字连结起来准备通信。（客户端）
// s为欲连结的已创建的套接字。
// name为欲连结的socket地址。
// namelen为socket地址的结构的长度
#define NET_CONNECT(_SOCKET, _ADDR)  (connect(_SOCKET, (struct sockaddr*)&(_ADDR), sizeof(_ADDR)) != SOCKET_ERROR)

// 绑定套接字（服务器端和客户端）
// 将本地地址绑定到所创建的套接字上。
// s: 是由socket()调用返回的并且未作连接的套接字描述符（套接字号）。
// 没有错误，bind()返回0，否则SOCKET_ERROR
#define NET_BIND(_SOCKET, _ADDR) (bind(_SOCKET, (struct sockaddr*)&(_ADDR), sizeof(_ADDR)) != SOCKET_ERROR)

// 监听连接（服务器端）
// 用于面向连接服务器，表明它愿意接收连接
#define NET_LISTEN(_SOCKET, _BACKLOG)  (listen(_SOCKET, _BACKLOG) != SOCKET_ERROR)

// 套接字等待连接（服务器端）
// 阻塞至客户方有连接请求到来
// 有连接请求时, 返回被赋予客户机地址的新套接字
// s为处于监听模式的套接字。
// sockaddr为接收成功后返回客户端的网络地址。
// addrlen为网络地址的长度。
#define NET_ACCEPT(_SOCKET, _ADDR, _ADDRLEN) accept(_SOCKET, (struct sockaddr*)&(_ADDR), &(_ADDRLEN))

// 套接字发送数据：（服务器端和客户端）
// s为服务器端监听的套接字。
// buf为欲发送数据缓冲区的指针。
// len为发送数据缓冲区的长度。
// flags为数据发送标记。
// 返回值为发送数据的字符数。
// 这里讲一下这个发送标记，下面8中讨论的接收标记也一样：
// flag取值必须为0或者如下定义的组合：0表示没有特殊行为。
// #define MSG_OOB 0x1 /* process out-of-band data */
// #define MSG_PEEK 0x2 /* peek at incoming message */
// #define MSG_DONTROUTE 0x4 /* send without using routing tables */
// MSG_OOB表示数据应该带外发送，所谓带外数据就是TCP紧急数据。
// MSG_PEEK表示使有用的数据复制到缓冲区内，但并不从系统缓冲区内删除。
// MSG_DONTROUTE表示不要将包路由出去。
#define NET_SEND(_SOCKET, _BUF, _LEN, _FLAGS) send(_SOCKET, (const char *)(_BUF), (_LEN)*sizeof(*(_BUF)), _FLAGS)
#define NET_SEND2(_SOCKET, _VAL, _FLAGS) send(_SOCKET, (const char *)&(_VAL), sizeof(_VAL), _FLAGS)


// 接收数据
// s为准备接收数据的套接字。
// buf为准备接收数据的缓冲区。
// len为准备接收数据缓冲区的大小。
// flags为数据接收标记。
// 返回值为接收的数据的字符数。
#define NET_RECV(_SOCKET, _BUF, _LEN, _FLAGS) recv(_SOCKET, (char *)(_BUF), (_LEN)*sizeof(*(_BUF)), _FLAGS)
#define NET_RECV2(_SOCKET, _VAL, _FLAGS) recv(_SOCKET, (char *)&(_VAL), sizeof(_VAL), _FLAGS)


// recvfrom(_SOCKET, _BUF, _LEN, _FLAGS, _FROM, _FROMLEN)

// 多路复用——select()
// 用来检测一个或多个套接字状态。
// readfds:指向要做读检测的指针
// writefds:指向要做写检测的指针
// exceptfds:指向要检测是否出错的指针
// timeout:最大等待时间
// int PASCAL FAR select(int nfds,fd_set FAR * readfds,fd_set FAR * writefds,
// fd_set FAR * exceptfds,const struct timeval FAR * timeout);
// 返回值：负值：select错误 正值：某些文件可读写或出错 0：等待超时，没有可读写或错误的文件

// sendto(_SOCKET, buf, len, flags, to, tolen);

// 允许调试输出#define SO_DEBUG 0x0001 /* turn on debugging info recording */
// 是否监听模式#define SO_ACCEPTCONN 0x0002 /* socket has had listen() */
// 套接字与其他套接字的地址绑定#define SO_REUSEADDR 0x0004 /* allow local address reuse */
// 保持连接#define SO_KEEPALIVE 0x0008 /* keep connections alive */
// 不要路由出去#define SO_DONTROUTE 0x0010 /* just use interface addresses */
// 设置为广播#define SO_BROADCAST 0x0020 /* permit sending of broadcast msgs */
// 使用环回不通过硬件#define SO_USELOOPBACK 0x0040 /* bypass hardware when possible */
// 当前拖延值#define SO_LINGER 0x0080 /* linger on close if data present */
// 是否加入带外数据#define SO_OOBINLINE 0x0100 /* leave received OOB data in line */
// 禁用LINGER选项#define SO_DONTLINGER (int)(~SO_LINGER)
// 发送缓冲区长度#define SO_SNDBUF 0x1001 /* send buffer size */
// 接收缓冲区长度#define SO_RCVBUF 0x1002 /* receive buffer size */
// 发送超时时间#define SO_SNDTIMEO 0x1005 /* send timeout */
// 接收超时时间#define SO_RCVTIMEO 0x1006 /* receive timeout */
// 错误状态#define SO_ERROR 0x1007 /* get error status and clear */
// 套接字类型#define SO_TYPE 0x1008 /* get socket type */
// 读取socket属性
// s为欲读取属性的套接字。
// level为套接字选项的级别，大多数是特定协议和套接字专有的。如IP协议应为 IPPROTO_IP。
// optname为读取选项的名称
// optval为存放选项值的缓冲区指针。
// optlen为缓冲区的长度
// getsockopt(_SOCKET, level, optname, optval, optlen)

// 设置socket属性
// s为欲设置属性的套接字。
// level为套接字选项的级别，用法同上。
// optname为设置选项的名称
// optval为存放选项值的缓冲区指针。
// optlen为缓冲区的长度
// setsockopt(_SOCKET, level, optname, optval, optlen)

// 改为非阻塞模式ioctlsocket(FIONBIO, &flag)
// ioctlsocket( _SOCKET, cmd, argp);

#define NET_GETSOCKNAME(_SOCKET, _ADDR, _NAMELEN) (getsockname(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)
#define NET_GETPEERNAME(_SOCKET, _ADDR, _NAMELEN) (getpeername(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)


// 开始服务器
SOCKET net_start_server(int port);
// 开始客户器
SOCKET net_start_client(const char* StrAddrPort);

int net_acceptAll(SOCKET s, int nClientNum, SOCKET* pClient);

// 用在recv和send之间
int net_ssrr(SOCKET s, int k);
// 用在send和recv之间
int net_rrss(SOCKET s, int k);







#endif // _NETINIT_H_
