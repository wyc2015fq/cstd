#ifndef _CHCSOCK_H_
#define _CHCSOCK_H_
#include <stdio.h>
#ifndef FAR
#define FAR
#endif // FAR
#ifdef __cplusplus
extern "C" {
#endif
#define CHCSOCK_ERR_TIMEOUT     (-2) /* 超时错误 */
#define CHCSOCK_ERR_DISCONNECT  (-3) /* 连接错误 */
#define CHCSOCK_ERR_HEADER      (-4) /* 包头错误，断开连接 */
#define CHCSOCK_ERR_LITTLEBUF   (-5) /* 接收缓冲区比待接收的数据小，断开连接 */
#define CHCSOCK_ERR_MAXTIMES    (-6) /* 重试超过最大次数 */
#define CHCSOCK_ERR_SUMMATION   (-7) /* 重试超过最大次数 */
#define DO_PRINT_FLAGS (0) // 用于打印发送进度
  typedef unsigned int CPM_SOCKET;
  struct sockaddr;
#define NET_INVALID(_SOCKET)  ((_SOCKET) == INVALID_SOCKET)
#define PAUSE  system("pause")
#define IF_ERR(_A, MSG)  if (!(_A))  { fprintf(stderr, MSG); }
  /*
   使用所有接口前先初始化 CPM_InitSocket
   使用完后初始化 CPM_InitSocket
  */
  int CPM_InitSocket(void);
  int CPM_UnInitSocket(void);
  /*
   创建套接字(AF_INET,SOCK_STREAM,IPPROTO_TCP)
   使用前创建一个新的套接字
   af: 通信发生的区域或网络地址类型，一般为AF_INET，表示在Internet域中使用
   type: 套接字类型
   procotol: 使用的特定协议
  */
  CPM_SOCKET CPM_OpenSocket(int af, int type, int protocol);
  /*
   关闭套接字：释放所占有的资源。（服务器端和客户端）
  */
  int CPM_CloseSocket(CPM_SOCKET s);
#define CPM_CLOSE(_SOCKET) CPM_CloseSocket(_SOCKET)
  /*
   中断套接字连接：通知服务器端或客户端停止接收和发送数据。（服务器端和客户端）
   How为描述禁止哪些操作，取值为：SD_RECEIVE、SD_SEND、SD_BOTH。
  */
  int CPM_Shutdown(CPM_SOCKET s, int how);
  /*
   套接字的连结：将两个套接字连结起来准备通信。（客户端）
   s为欲连结的已创建的套接字。
   name为欲连结的socket地址。
   namelen为socket地址的结构的长度
   StrAddrPort "192.168.1.1:35"
  */
  int CPM_Connect(CPM_SOCKET s, const struct sockaddr FAR* name, int namelen, const struct timeval FAR* timeout);
  int CPM_ConnectAddrStr(CPM_SOCKET s, const char* StrAddrPort, const struct timeval FAR* timeout);
  /*
   绑定套接字（服务器端和客户端）
   将本地地址绑定到所创建的套接字上。
   s: 是由socket调用返回的并且未作连接的套接字描述符（套接字号）。
   没有错误，bind返回0，否则SOCKET_ERROR
  */
  int CPM_Bind(CPM_SOCKET s, const struct sockaddr FAR* addr, int namelen);
  int CPM_BindAddrStr(CPM_SOCKET s, const char* StrAddrPort);
  /*
   监听连接（服务器端）
   用于面向连接服务器，表明它愿意接收连接
  */
  int CPM_Listen(CPM_SOCKET s, int backlog);
  /*
   套接字等待连接（服务器端）
   阻塞至客户方有连接请求到来
   有连接请求时, 返回被赋予客户机地址的新套接字
   s为处于监听模式的套接字。
   sockaddr为接收成功后返回客户端的网络地址。
   addrlen为网络地址的长度。
   返回值：
   负值：select错误
   正值：接到的socket句柄
   0：等待超时，没有可读写或错误的文件
  */
  CPM_SOCKET CPM_Accept(CPM_SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen);
  CPM_SOCKET CPM_AcceptAddrStr(CPM_SOCKET s, char* StrAddrPort, int slen);
  /*
   套接字发送数据：（服务器端和客户端）
   s为服务器端监听的套接字。
   buf为欲发送数据缓冲区的指针。
   len为发送数据缓冲区的长度。
   flags为数据发送标记。
   返回值为发送数据的字符数。
   这里讲一下这个发送标记，下面8中讨论的接收标记也一样：
   flag取值必须为0或者如下定义的组合：0表示没有特殊行为。
   #define MSG_OOB 0x1 // process out-of-band data
   #define MSG_PEEK 0x2 // peek at incoming message
   #define MSG_DONTROUTE 0x4 // send without using routing tables
   MSG_OOB表示数据应该带外发送，所谓带外数据就是TCP紧急数据。
   MSG_PEEK表示使有用的数据复制到缓冲区内，但并不从系统缓冲区内删除。
   MSG_DONTROUTE表示不要将包路由出去。
  */
  int CPM_Send(CPM_SOCKET s, const void FAR* buf, int len, int flags, const struct timeval FAR* timeout);
  int CPM_SafeSend(CPM_SOCKET s, const void FAR* pbData, int dwDataLen, int flags, const struct timeval FAR* timeout);
  int CPM_SafeSendFile(CPM_SOCKET s, void* pf, int flags, const struct timeval FAR* timeout);
#define CPM_SAFESEND(_SOCKET, _BUF, _LEN) CPM_SafeSend(_SOCKET, (const char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_SAFESENDFILE(_SOCKET, _PF) CPM_SafeSendFile(_SOCKET, _PF, DO_PRINT_FLAGS, NULL)
#define CPM_SEND(_SOCKET, _BUF, _LEN) CPM_SafeSend(_SOCKET, (const char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_SEND2(_SOCKET, _VAL) CPM_SafeSend(_SOCKET, (const char *)&(_VAL), sizeof(_VAL), DO_PRINT_FLAGS, NULL)
  /*
   接收数据
   s为准备接收数据的套接字。
   buf为准备接收数据的缓冲区。
   len为准备接收数据缓冲区的大小。
   flags为数据接收标记。
   返回值：正数为接收的数据的字符数。
   0或WSAECONNRESET连接被关闭
  */
  int CPM_Recv(CPM_SOCKET s, void* buf, int len, int flags, const struct timeval FAR* timeout);
  int CPM_SafeRecv(CPM_SOCKET s, char FAR* pbData, int dwDataLen, int flags, const struct timeval FAR* timeout);
  int CPM_SafeRecvFile(CPM_SOCKET s, void* pf, int flags, const struct timeval FAR* timeout);
#define CPM_SAFERECV(_SOCKET, _BUF, _LEN) CPM_SafeRecv(_SOCKET, (char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_SAFERECVFILE(_SOCKET, _PF) CPM_SafeRecvFile(_SOCKET, _PF, DO_PRINT_FLAGS, NULL)
#define CPM_RECV(_SOCKET, _BUF, _LEN) CPM_SafeRecv(_SOCKET, (char *)(_BUF), (_LEN)*sizeof(*(_BUF)), DO_PRINT_FLAGS, NULL)
#define CPM_RECV2(_SOCKET, _VAL) CPM_SafeRecv(_SOCKET, (char *)&(_VAL), sizeof(_VAL), DO_PRINT_FLAGS, NULL)
  /*
   多路复用——select
   用来检测一个或多个套接字状态。
   readfds:指向要做读检测的指针
   writefds:指向要做写检测的指针
   exceptfds:指向要检测是否出错的指针
   timeout:最大等待时间
   int PASCAL FAR select (int nfds,fd_set FAR * readfds,fd_set FAR * writefds,
   fd_set FAR * exceptfds,const struct timeval FAR * timeout);
   sendto (_SOCKET, buf, len, flags, to, tolen);
   允许调试输出#define SO_DEBUG 0x0001 // turn on debugging info recording
   是否监听模式#define SO_ACCEPTCONN 0x0002 // socket has had listen()
   套接字与其他套接字的地址绑定#define SO_REUSEADDR 0x0004 // allow local address reuse
   保持连接#define SO_KEEPALIVE 0x0008 // keep connections alive
   不要路由出去#define SO_DONTROUTE 0x0010 // just use interface addresses
   设置为广播#define SO_BROADCAST 0x0020 // permit sending of broadcast msgs
   使用环回不通过硬件#define SO_USELOOPBACK 0x0040 // bypass hardware when possible
   当前拖延值#define SO_LINGER 0x0080 // linger on close if data present
   是否加入带外数据#define SO_OOBINLINE 0x0100 // leave received OOB data in line
   禁用LINGER选项#define SO_DONTLINGER (int)(~SO_LINGER)
   发送缓冲区长度#define SO_SNDBUF 0x1001 // send buffer size
   接收缓冲区长度#define SO_RCVBUF 0x1002 // receive buffer size
   发送超时时间#define SO_SNDTIMEO 0x1005 // send timeout
   接收超时时间#define SO_RCVTIMEO 0x1006 // receive timeout
   错误状态#define SO_ERROR 0x1007 // get error status and clear
   套接字类型#define SO_TYPE 0x1008 // get socket type
   读取socket属性
   s为欲读取属性的套接字。
   level为套接字选项的级别，大多数是特定协议和套接字专有的。如IP协议应为 IPPROTO_IP。
   optname为读取选项的名称
   optval为存放选项值的缓冲区指针。
   optlen为缓冲区的长度
   getsockopt (_SOCKET, level, optname, optval, optlen)
   设置socket属性
   s为欲设置属性的套接字。
   level为套接字选项的级别，用法同上。
   optname为设置选项的名称
   optval为存放选项值的缓冲区指针。
   optlen为缓冲区的长度
   setsockopt (_SOCKET, level, optname, optval, optlen)
   改为非阻塞模式ioctlsocket (FIONBIO, &flag)
   ioctlsocket (_SOCKET, cmd, argp);
  */
  int CPM_GetSockName(CPM_SOCKET s, struct sockaddr FAR* name, int FAR* namelen);
  int CPM_GetPeerName(CPM_SOCKET s, struct sockaddr FAR* name, int FAR* namelen);
#define CPM_GETSOCKNAME(_SOCKET, _ADDR, _NAMELEN) (CPM_GetSockName(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)
#define CPM_GETPEERNAME(_SOCKET, _ADDR, _NAMELEN) (CPM_GetPeerName(_SOCKET, (struct sockaddr*)&(_ADDR), &_NAMELEN) != SOCKET_ERROR)
  /*内部使用*/
  int CPM_AcceptAll(CPM_SOCKET s, int nClientNum, CPM_SOCKET* pClient);
  CPM_SOCKET CPM_StartClient(const char* StrAddrPort);
  CPM_SOCKET CPM_StartServer(int port);
#ifdef __cplusplus
}
#endif
#endif // _CHCSOCK_H_

