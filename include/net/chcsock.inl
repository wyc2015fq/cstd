#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "chcsock.h"
#include "cfile.h"
#ifdef WIN32
#include "net_w32.h"
#endif //_WIN32
#define BLOCKSIZE (1 << 16)
#define FILEBUFSIZE BLOCKSIZE
#define MAXTIMES (100000)
#define FEN(_I, _N) ((1. * (_I)) / (_N))
#ifdef _DEBUG
static int errtimes = 0;
static int alltimes = 1;
#define IF_ERR_MAXTIMES_RETURN(_I) \
  if ((++(_I)) > MAXTIMES) \
  { \
    return CHCSOCK_ERR_MAXTIMES; \
  } \
  errtimes += ((_I) >= 2); \
  ++alltimes;
#define DO_PRINT(_W, _F, _N, _I) \
  if (DO_PRINT_FLAGS == _F) \
  { \
    printf("大小%10u字节, 已经%s%10u字节[%5.1f%%](错误=%4.1f%% 速度=%3.1fMps)\r", _N, _W, _I, FEN(100. * _I, _N), FEN(100. * errtimes, alltimes), FEN(_I, 1024. * 1024. * utime_elapsed(_start_time))); \
  }
#define BEG_DO_PRINT() utime_start(_start_time);
#define END_DO_PRINT() printf(" \r");
#else
#define IF_ERR_MAXTIMES_RETURN(_I) \
  if ((++(_I)) > MAXTIMES) \
  { \
    return CHCSOCK_ERR_MAXTIMES; \
  }
#define DO_PRINT(_W, _F, _N, _I) \
  if (DO_PRINT_FLAGS == _F) \
  { \
    printf("大小%10u字节, 已经%s%10u字节[%3u%%]\r", _N, _W, _I, (int) ((100. * (_I)) / (_N))); \
  }
#define BEG_DO_PRINT() { }
#define END_DO_PRINT() printf(" \r");
#endif
#define IF_ERR_RETURN(_RE2) do { int _RE = (_RE2); if (0 > (_RE)) return _RE; } while (0)
#define Disconnect(m_hSocket) closesocket(m_hSocket)
//typedef SOCKET CPM_SOCKET;
#define SAFE_SEND_TIMEOUT(_SOCKET, _TIMEOUT) \
  if (0 != _TIMEOUT) { FD_SET fd = { 1, _SOCKET }; struct timeval timeout; \
    timeout.tv_sec = _TIMEOUT / 1000, timeout.tv_usec = 1000*(_TIMEOUT % 1000); \
    if (select(0, NULL, &fd, NULL, &timeout) == 0) return CHCSOCK_ERR_TIMEOUT; }
#define SAFE_RECV_TIMEOUT(_SOCKET, _TIMEOUT) \
  if (0 != _TIMEOUT) { FD_SET fd = { 1, _SOCKET }; struct timeval timeout; \
    timeout.tv_sec = _TIMEOUT / 1000, timeout.tv_usec = 1000*(_TIMEOUT % 1000); \
    if (select(0, &fd, NULL, NULL, &timeout) == 0) return CHCSOCK_ERR_TIMEOUT; }
#if 0
#define SUMMATION(_N, _F, _SUM) \
  do \
  { \
    size_t _I; \
    for (_I = 0; _I < (_N); ++_I) \
    { \
      (_SUM) += *(_F + _I); \
    } \
  } while (0)
#else
int summation(size_t _N, const char* _F)
{
  size_t _I;
  int _SUM = 0;
  for (_I = 0; _I < (_N); ++_I) {
    (_SUM) += *(_F + _I);
  }
  return _SUM;
}
#define SUMMATION(_N, _F, _SUM) _SUM = summation(_N, _F)
#endif
#define WSA_ERROR_DEF(a,b,c) case a: {_snprintf(buf, sizeof(buf)/sizeof(buf[0]), "err=%d, %s, %s", a, #b, #c );}break;
TCHAR* WSAGetLastErrMsg()
{
  static TCHAR buf[70];
  int error = WSAGetLastError();
  buf[0] = 0;
  _snprintf(buf, sizeof(buf) / sizeof(buf[0]), "err=%d, %s, %s", error, "", "");
  switch (error) {
#include "WSAGetLastError.inl"
#undef WSA_ERROR_DEF
  };
  return buf;
}
void CPM_UnInitSocket(void)
{
  WSACleanup();
}
int InitSocket(BYTE minorVer, BYTE majorVer)
{
  static int wasInitialized = 0;
  // check initialization status
  if (!wasInitialized) {
    //初始化WS2_32.DLL
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(minorVer, majorVer);
    int nr = WSAStartup(sockVersion, &wsaData);
    if (nr != 0) {
      return 1;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
      /* Tell the user that we could not find a usable WinSock DLL. */
      WSACleanup();
    }
    atexit(CPM_UnInitSocket);
    wasInitialized = 1;
    return 1;
  }
  return 0;
}
int CPM_InitSocket(void)
{
  return InitSocket(2, 2);
}
// 创建套接字(AF_INET,SOCK_STREAM,IPPROTO_TCP)
// 使用前创建一个新的套接字
// af: 通信发生的区域或网络地址类型，一般为AF_INET，表示在Internet域中使用
// type: 套接字类型
// procotol: 使用的特定协议
CPM_SOCKET CPM_OpenSocket(int af, int type, int protocol)
{
  return socket(af, type, protocol);
}
// 关闭套接字：释放所占有的资源。（服务器端和客户端）
int CPM_CloseSocket(CPM_SOCKET s)
{
  return closesocket(s);
}
// 中断套接字连接：通知服务器端或客户端停止接收和发送数据。（服务器端和客户端）
// How为描述禁止哪些操作，取值为：SD_RECEIVE、SD_SEND、SD_BOTH。
int CPM_Shutdown(SOCKET s, int how)
{
  return shutdown(s, how);
}
// 套接字的连结：将两个套接字连结起来准备通信。（客户端）
// s为欲连结的已创建的套接字。
// name为欲连结的socket地址。
// namelen为socket地址的结构的长度
int CPM_Connect(CPM_SOCKET s, const struct sockaddr FAR* name, int namelen, int ms)
{
  //set the socket in non-blocking mode
  int nResult;
  if (ms > 0) {
    struct timeval timeout = {ms / 1000, ms % 1000};
    unsigned long argp = 1;
    FD_SET fd = { 1, s };
    ioctlsocket(s, FIONBIO, &argp);
    nResult = connect(s, name, namelen);
    select(0, NULL, &fd, NULL, &timeout);
    //set the socket in blocking mode again.
    argp = 0;
    ioctlsocket(s, FIONBIO, &argp);
  }
  else {
    nResult = connect(s, name, namelen);
  }
  return nResult;
}
#define ERROR_CREATE_SOCKET -1
#define ERROR_MODIFY_FIONBIO -2
#define ERROR_SELECT -3
#define ERROR_SELECT_TIMEOUT -4
int my_connect(CPM_SOCKET sock, const char* StrAddrPort, DWORD dwTimeOut)
{
  unsigned long flag = 1;
  int dwRet = 0;
  if ((ioctlsocket(sock, FIONBIO, &flag) != 0)) {
    printf("未能改为非阻塞模式!");
    dwRet = ERROR_MODIFY_FIONBIO;
  }
  else {
    struct fd_set mask;
    struct timeval timeout;
    sockaddr_in severAddr;
    severAddr = make_sockaddr_in(StrAddrPort);
    connect(sock, (sockaddr*)&severAddr, sizeof(severAddr));
    FD_ZERO(&mask);
    FD_SET(sock, &mask);
    timeout.tv_sec = dwTimeOut / 1000;
    timeout.tv_usec = 1000 * (dwTimeOut % 1000); // 微秒
    switch (select(0, NULL, &mask, NULL, &timeout)) {
    case -1:
      dwRet = ERROR_SELECT;
      //printf("ERROR_SELECT\n");
      break;
    case 0:
      dwRet = ERROR_SELECT_TIMEOUT;
      //printf("ERROR_SELECT_TIMEOUT %dms\n", timeout.tv_usec);
      break;
    default:
      dwRet = ERROR_SUCCESS;
    }
  }
  return dwRet;
}
typedef struct netaddr {
  union {
    unsigned long ip;
    unsigned char ips[4];
  };
  unsigned short port;
} netaddr;
netaddr str2netaddr(const char* sip)
{
  netaddr na;
  int ip0, ip1, ip2, ip3, port;
  sscanf(sip, "%d.%d.%d.%d:%d", &ip0, &ip1, &ip2, &ip3, &port);
  na.ips[0] = ip0, na.ips[1] = ip1, na.ips[2] = ip2, na.ips[3] = ip3, na.port = port;
  return na;
}
char* netaddr2str(netaddr na, char* buf, int buflen)
{
  _snprintf(buf, buflen, "%d.%d.%d.%d:%d", na.ips[0], na.ips[1], na.ips[2], na.ips[3], na.port);
  return buf;
}
SOCKET ScanConnect(const char* StrAddrPort, DWORD dwTimeOut)
{
  int dwRet;
  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET) {
    printf("创建套接字失败!");
    dwRet = ERROR_CREATE_SOCKET;
  }
  else {
    dwRet = my_connect(sock, StrAddrPort, dwTimeOut);
    if (ERROR_SUCCESS != dwRet) {
      closesocket(sock);
      sock = INVALID_SOCKET;
    }
  }
  return sock;
}
//DWORD dwTimeOut = 1000; //连接超时时间，以ms计
int ScanPort(const char* StrAddrPort, int Srart, int End, DWORD dwTimeOut, SOCKET* sockbuf)
{
  int dwRet;
  int socknum = 0;
  char buf[256];
  int i = Srart, ret = ERROR_SELECT;
  for (; i < End; ++i) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
      printf("创建套接字失败!");
      dwRet = ERROR_CREATE_SOCKET;
    }
    else {
      _snprintf(buf, 256, StrAddrPort, i);
      ret = my_connect(sock, buf, dwTimeOut);
      if (sockbuf) {
        if (ERROR_SUCCESS == ret) {
          sockbuf[socknum] = sock;
          socknum++;
        }
        else {
          closesocket(sock);
        }
      }
      else {
        if (ERROR_SUCCESS == ret) {
          printf("连接成功: %s", buf);
          socknum++;
        }
        closesocket(sock);
      }
    }
  }
  return socknum;
}
int ScanPortList(int n, const char** StrAddrPort, DWORD dwTimeOut, SOCKET* sockbuf)
{
  int dwRet;
  int socknum = 0;
  char buf[256];
  int i = 0, ret = ERROR_SELECT;
  __INITSOCKET22();
  for (; i < n; ++i) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
      printf("创建套接字失败!");
      dwRet = ERROR_CREATE_SOCKET;
    }
    else {
      _snprintf(buf, 256, "%s", StrAddrPort[i]);
      ret = my_connect(sock, buf, dwTimeOut);
      if (sockbuf) {
        if (ERROR_SUCCESS == ret) {
          sockbuf[socknum] = sock;
          socknum++;
        }
        else {
          closesocket(sock);
        }
      }
      else {
        if (ERROR_SUCCESS == ret) {
          printf("连接成功: %s", buf);
          socknum++;
        }
        closesocket(sock);
      }
    }
  }
  return socknum;
}
int CPM_ConnectAddrStr(CPM_SOCKET s, const char* StrAddrPort, int ms)
{
  struct sockaddr_in outaddr = make_sockaddr_in(StrAddrPort);
  return CPM_Connect(s, (const struct sockaddr*) &outaddr, sizeof(outaddr), ms);
}
// 绑定套接字（服务器端和客户端）
// 将本地地址绑定到所创建的套接字上。
// s: 是由socket()调用返回的并且未作连接的套接字描述符?ㄌ捉幼趾牛?
// 没有错误，bind()返回0，否则SOCKET_ERROR
int CPM_Bind(CPM_SOCKET s, const struct sockaddr FAR* addr, int namelen)
{
  return bind(s, addr, namelen);
}
int CPM_BindAddrStr(CPM_SOCKET s, const char* StrAddrPort)
{
  struct sockaddr_in outaddr = make_sockaddr_in(StrAddrPort);
  return NET_BIND(s, outaddr);
}
// 监听连接（服务器端）
// 用于面向连接服务器，表明它愿意接收连接
int CPM_Listen(CPM_SOCKET s, int backlog)
{
  return listen(s, backlog);
}
// 套接字等待连接（服务器端）
// 阻塞至客户方有连接请求到来
// 有连接请求时, 返回被赋予客户机地址的新套接字
// s为处于监听模式的套接字。
// sockaddr为接收成功后返回客户端的网络地址。
// addrlen为网络地址的长度。
#if 0
CPM_SOCKET CPM_Accept(CPM_SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen)
{
  return accept(s, addr, addrlen);
}
#else
CPM_SOCKET CPM_Accept(CPM_SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen)
{
  struct timeval timeout = {30, 0};
  fd_set rfd;
  int nfds;
  while (1) {
    FD_ZERO(&rfd);
    FD_SET(s, &rfd);
    nfds = select(1, &rfd, (fd_set*) 0, (fd_set*) 0, &timeout);
    if (nfds == 0) {
      //continue;
      return nfds;
      //break;
    }
    else if (nfds > 0) {
      FD_CLR(s, &rfd);
      //accept socket
      return accept(s, addr, addrlen);
    }
    else {
      return nfds;
      //break;
    }
  }
  return 0;
}
#endif
#define CPM_LISTEN(_SOCKET, _BACKLOG) (CPM_Listen(_SOCKET, _BACKLOG) != SOCKET_ERROR)
#define CPM_ACCEPT(_SOCKET, _ADDR, _ADDRLEN) CPM_Accept(_SOCKET, (struct sockaddr *) &(_ADDR), &(_ADDRLEN))
CPM_SOCKET CPM_AcceptAddrStr(CPM_SOCKET s, char* StrAddrPort, int slen)
{
  struct sockaddr_in addr;
  int nAddrLen = sizeof(struct sockaddr_in);
  CPM_SOCKET ret;
  ret = CPM_ACCEPT(s, addr, nAddrLen);
  if (StrAddrPort && nAddrLen > 0) {
    char* addrstr = inet_ntoa(addr.sin_addr);
    if (addrstr) {
      --slen;
      if ((int) strlen(addrstr) < slen) {
        slen = strlen(addrstr);
      }
      memcpy(StrAddrPort, addrstr, slen);
      StrAddrPort[ slen ] = '\0';
    }
  }
  return ret;
}
// recvfrom(_SOCKET, _BUF, _LEN, _FLAGS, _FROM, _FROMLEN)
// 多路复用——select()
// 用来检测一个或多个套接字状态。
// readfds:指向要做读检测的指针
// writefds:指向要做写检测的指针
// exceptfds:指向要检测是否出错的指针
// timeout:最大等待时间
// int PASCAL FAR select(int nfds,fd_set FAR * readfds,fd_set FAR * writefds,
// fd_set FAR * exceptfds,const struct timeval FAR * timeout);
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
int CPM_GetSockName(CPM_SOCKET s, struct sockaddr FAR* name, int FAR* namelen)
{
  return getsockname(s, name, namelen);
}
int CPM_GetPeerName(CPM_SOCKET s, struct sockaddr FAR* name, int FAR* namelen)
{
  return getpeername(s, name, namelen);
}
// 按a.b.c.d:port 格式设定IP端口号, 并将主机字节顺序转换为网络字节顺序
struct sockaddr_in make_sockaddr_in(const char* cp) {
  struct sockaddr_in outaddr;
  char buf[ 256 ];
  int cplen = strlen(cp);
  char* p = (char*)strrchr(cp, ':');
  u_short m_port = 0;
  if (p) {
    m_port = (unsigned short) atol(p + 1);
    RFIND_OUT(cp, p, '0', '9');
    cplen = p - cp;
  }
  memcpy(buf, cp, cplen);
  buf[ cplen ] = '\0';
  MAKE_SOCKADDR_IN(outaddr, AF_INET, (m_port), inet_addr(buf));
  return outaddr;
}
CPM_SOCKET CPM_StartServer(int port)
{
  int ret = 0;
  CPM_SOCKET sc;
  __INITSOCKET22();
  sc = NET_OPEN_TCP();
  ret = !NET_INVALID(sc);
  IF_ERR(ret, "创建套接字失败!\n");
  if (ret) {
    struct sockaddr_in outaddr;
    MAKE_SOCKADDR_IN(outaddr, AF_INET, (u_short) port, INADDR_ANY);
    ret = NET_BIND(sc, outaddr);
    IF_ERR(ret, "套接字绑定失败!\n");
    if (!ret) {
      NET_CLOSE(sc);
      return INVALID_SOCKET;
    }
  }
  return sc;
}
CPM_SOCKET CPM_StartClient(const char* StrAddrPort)
{
  int ret = 0;
  CPM_SOCKET sc;
  __INITSOCKET22();
  sc = NET_OPEN_TCP();
  ret = !NET_INVALID(sc);
  IF_ERR(ret, "创建套接字失败!\n");
  if (ret) {
    ret = CPM_ConnectAddrStr(sc, StrAddrPort, 0);
    if (SOCKET_ERROR == ret) {
      NET_CLOSE(sc);
      return INVALID_SOCKET;
    }
  }
  return sc;
}
int CPM_AcceptAll(CPM_SOCKET s, int nClientNum, CPM_SOCKET* pClient)
{
  struct sockaddr_in clientAddr;
  int nAddrLen = sizeof(struct sockaddr_in), i;
  int ret = CPM_LISTEN(s, nClientNum);
  IF_ERR(ret, "监听失败!\n");
  for (i = 0; i < nClientNum; ++i) {
    pClient[ i ] = CPM_ACCEPT(s, clientAddr, nAddrLen);
  }
  return ret;
}
int net_ssrr(SOCKET s, int k)
{
  int re;
  re = 1000 + k;
  IF_ERR_RETURN(NET_SEND2(s, re, 0));
  while (re != (2000 + k)) {
    IF_ERR_RETURN(NET_RECV2(s, re, 0));
  }
  return 1;
}
int net_rrss(SOCKET s, int k)
{
  int re;
  re = 0;
  while (re != (1000 + k)) {
    IF_ERR_RETURN(NET_RECV2(s, re, 0));
  }
  re = 2000 + k;
  IF_ERR_RETURN(NET_SEND2(s, re, 0));
  return 1;
}
int net_safesend_sum(SOCKET s, const char* buf, size_t n, int flags)
{
  int i = 0, aaa = 0, bbb = 0, re = 0;
  //SAFE_SEND_TIMEOUT(s, NULL);
  SUMMATION(n, (const char*) buf, aaa);
  aaa += n;
  if (n > 0) {
    do {
      IF_ERR_MAXTIMES_RETURN(i);
      IF_ERR_RETURN(net_ssrr(s, 123));
      IF_ERR_RETURN(NET_SEND2(s, aaa, flags));
      re = NET_SEND(s, buf, n, flags);
      IF_ERR_RETURN(re);
      IF_ERR_RETURN(net_rrss(s, 124));
      IF_ERR_RETURN(NET_RECV2(s, bbb, flags));
    }
    while (aaa != bbb);
  }
  //assert((int)n==re);
  return re;
}
int net_saferecv_sum(SOCKET s, char* buf, size_t n, int flags)
{
  int i = 0, aaa = 0, bbb = 0, re = 0;
  //SAFE_RECV_TIMEOUT(s, NULL);
  if (n > 0) {
    do {
      IF_ERR_MAXTIMES_RETURN(i);
      IF_ERR_RETURN(net_rrss(s, 123));
      IF_ERR_RETURN(NET_RECV2(s, aaa, flags));
      re = NET_RECV(s, buf, n, flags);
      IF_ERR_RETURN(re);
      SUMMATION(n, (const char*) buf, bbb);
      bbb += re;
      IF_ERR_RETURN(net_ssrr(s, 124));
      IF_ERR_RETURN(NET_SEND2(s, bbb, flags));
    }
    while (aaa != bbb);
  }
  //assert((int)n==re);
  return re;
}
int net_safesend_sum1(SOCKET s, const char* buf, size_t n, int flags)
{
  int i = 0, aaa = 0, bbb = 0, re = 0;
  //SAFE_SEND_TIMEOUT(s, NULL);
  SUMMATION(n, (const char*) buf, aaa);
  if (n > 0) {
    IF_ERR_RETURN(NET_SEND2(s, aaa, flags));
    re = NET_SEND(s, buf, n, flags);
    IF_ERR_RETURN(re);
  }
  return re;
}
int net_saferecv_sum1(SOCKET s, char* buf, size_t n, int flags)
{
  int i = 0, aaa = 0, bbb = 0, re = 0;
  //SAFE_RECV_TIMEOUT(s, NULL);
  if (n > 0) {
    IF_ERR_RETURN(NET_RECV2(s, aaa, flags));
    re = NET_RECV(s, buf, n, flags);
    IF_ERR_RETURN(re);
    SUMMATION(n, (const char*) buf, bbb);
    if (aaa != bbb) {
      return CHCSOCK_ERR_SUMMATION;
    }
  }
  return re;
}
int net_safesend1a(SOCKET s, const char* buf, size_t n, int flags)
{
  size_t i = 0;
  for (i = 0; i < n;) {
    int re = NET_SEND(s, buf + i, n - i, flags);
    IF_ERR_RETURN(re);
    i += re;
  }
  return i;
}
int net_saferecv1a(SOCKET s, char* buf, size_t n, int flags)
{
  size_t i = 0;
  for (i = 0; i < n;) {
    int re = NET_RECV(s, buf + i, n - i, flags);
    IF_ERR_RETURN(re);
    i += re;
  }
  return i;
}
int net_safesend1b(SOCKET s, const char* buf, size_t cnt, int flags)
{
  int cnt0 = cnt;
  do {
    int sendcnt = send(s, buf, cnt, MSG_DONTROUTE);
    if (sendcnt == SOCKET_ERROR) {
      sendcnt = WSAGetLastError();
      if (sendcnt == WSAEWOULDBLOCK) {
        printf("net_safesend1 err WSAEWOULDBLOCK\n");
        //SAFE_SEND_TIMEOUT(s, 100);
        //这里应加循环次数控制判断，即设置超时，跳出循环
        //...
        continue;
      }
      else {
        //发送数据数据失败
        break;
      }
    }
    cnt -= sendcnt;
    buf += sendcnt;
  }
  while (cnt > 0);
  return cnt0 - cnt;
}
int net_saferecv1b(SOCKET s, char* buf, size_t cnt, int flags)
{
  int cnt0 = cnt;
  do {
    int sendcnt = recv(s, buf, cnt, MSG_DONTROUTE);
    if (sendcnt == SOCKET_ERROR) {
      sendcnt = WSAGetLastError();
      if (sendcnt == WSAEWOULDBLOCK) {
        //printf("net_saferecv1 err WSAEWOULDBLOCK\n");
        //SAFE_RECV_TIMEOUT(s, 100);
        continue;
      }
      else {
        //发送数据数据失败
        break;
      }
    }
    cnt -= sendcnt;
    buf += sendcnt;
  }
  while (cnt > 0);
  return cnt0 - cnt;
}
int net_safesend1(SOCKET s, const char* buf, size_t n, int flags)
{
  size_t i = 0;
  int re, err;
  int t = 10;
  for (i = 0; i < n;) {
    SAFE_SEND_TIMEOUT(s, 100);
    re = NET_SEND(s, buf + i, n - i, flags);
    if (0 == re) {
      if (--t < 0) {
        re = SOCKET_ERROR;
      }
      Sleep(20);
    }
    if (re == SOCKET_ERROR) {
      err = WSAGetLastError();
      if (err == WSAEWOULDBLOCK) {
        printf("net_safesend1:%s\n", WSAGetLastErrMsg());
        SAFE_SEND_TIMEOUT(s, 100);
        continue;
      }
      else {
        return re;
        break;
      }
    }
    i += re;
  }
  return i;
}
int net_saferecv1(SOCKET s, char* buf, size_t n, int flags)
{
  size_t i = 0;
  int re, err;
  int t = 10;
  for (i = 0; i < n;) {
    SAFE_RECV_TIMEOUT(s, 100);
    re = NET_RECV(s, buf + i, n - i, flags);
    if (0 == re) {
      if (--t < 0) {
        re = SOCKET_ERROR;
      }
      Sleep(20);
    }
    if (re == SOCKET_ERROR) {
      err = WSAGetLastError();
      if (err == WSAEWOULDBLOCK) {
        printf("net_saferecv1:%s\n", WSAGetLastErrMsg());
        SAFE_RECV_TIMEOUT(s, 100);
        continue;
      }
      else {
        return re;
        break;
      }
    }
    i += re;
  }
  return i;
}
int CPM_SendT(CPM_SOCKET s, const void FAR* pch, int nSize, int flags, int ms)
{
  int re = 0, n = 0;
  SAFE_SEND_TIMEOUT(s, ms);
  //throw new CBlockingSocketException ("Send timeout");
  while (n < nSize) {
    re = net_safesend1(s, (const char*)pch, nSize, flags);
    if (re < 0) {
      return SOCKET_ERROR;
    }
    n += re;
  }
  //printf("CPM_SendT %d\n", n);
  return n;
}
int CPM_RecvT(CPM_SOCKET s, void* pch0, int nSize, int flags, unsigned int ms)
{
  int re, n = 0;
  char* pch = (char*)pch0;
  SAFE_RECV_TIMEOUT(s, ms);
  while (n < nSize) {
    re = net_saferecv1(s, pch + n, nSize - n, flags);
    if (re < 0) {
      return SOCKET_ERROR;
    }
    n += re;
  }
  //printf("CPM_RecvT %d\n", n);
  return n;
}
int CPM_Send(CPM_SOCKET s, const void FAR* pch, int nSize, int flags, int ms)
{
  int nBytesSent = 0;
  SAFE_SEND_TIMEOUT(s, ms);
  //throw new CBlockingSocketException ("Send timeout");
  if ((nBytesSent = net_safesend1(s, (const char*)pch, nSize, flags)) == SOCKET_ERROR) {
    return SOCKET_ERROR;
  }
  if (nBytesSent != nSize) {
    return SOCKET_ERROR;
  }
  return nBytesSent;
}
// 接收数据
// s为准备接收数据的套接字。
// buf为准备接收数据的缓冲区。
// len为准备接收数据缓冲区的大小。
// flags为数据接收标记。
// 返回值为接收的数据的字符数。
int CPM_Recv(CPM_SOCKET s, void* pch, int nSize, int flags, int ms)
{
  int nBytesReceived;
  SAFE_RECV_TIMEOUT(s, ms);
  if ((nBytesReceived = net_saferecv1(s, (char*)pch, nSize, flags)) == SOCKET_ERROR) {
    return SOCKET_ERROR;
  }
  if (nBytesReceived != nSize) {
    return SOCKET_ERROR;
  }
  return nBytesReceived;
}
int CPM_SendX(CPM_SOCKET s, unsigned int ms, ...)
{
  va_list marker;
  char* buf;
  int n, rn;
  va_start(marker, ms);
  while (1) {
    buf = va_arg(marker, char*);
    if (0 == buf) {
      break;
    }
    n = va_arg(marker, int);
    if (0 == n) {
      break;
    }
    rn = CPM_SendT(s, buf, n, 0, ms);
    if (rn < 0) {
      return rn;
    }
  }
  va_end(marker);
  return 0;
}
int CPM_RecvX(CPM_SOCKET s, unsigned int ms, ...)
{
  va_list marker;
  char* buf;
  int n, rn;
  va_start(marker, ms);
  while (1) {
    buf = va_arg(marker, char*);
    if (0 == buf) {
      break;
    }
    n = va_arg(marker, int);
    if (0 >= n) {
      break;
    }
    rn = CPM_RecvT(s, buf, n, 0, ms);
    if (rn < 0) {
      return rn;
    }
  }
  va_end(marker);
  return 0;
}
int CPM_SafeSend(CPM_SOCKET s, const void* buf0, int n, int flags, int ms)
{
  int i = 0, aaa = 0, bbb = 0, re = 0, re2;
  const char* buf = (const char*)buf0;
  BEG_DO_PRINT();
  SAFE_SEND_TIMEOUT(s, ms);
  IF_ERR_RETURN(NET_SEND2(s, n, flags));
  for (; re < n;) {
    re2 = net_safesend1(s, buf, n - re, flags);
    IF_ERR_RETURN(re2);
    re += re2;
    DO_PRINT("发送", flags, n, re);
  }
  END_DO_PRINT();
  return re;
}
int CPM_SafeRecv(CPM_SOCKET s, char* buf, int n2, int flags, int ms)
{
  int i = 0, aaa = 0, bbb = 0, re = 0, re2 = 0, n = n2;
  BEG_DO_PRINT();
  SAFE_RECV_TIMEOUT(s, ms);
  IF_ERR_RETURN(NET_RECV2(s, n, flags));
  if (n > n2) {
    return CHCSOCK_ERR_LITTLEBUF;
  }
  for (; re < n;) {
    re2 = net_saferecv1(s, buf, n - re, flags);
    IF_ERR_RETURN(re2);
    re += re2;
    DO_PRINT("接收", flags, n, re);
  }
  END_DO_PRINT();
  return re;
}
int CPM_SendFile(CPM_SOCKET s, HD_FILE pf, int flags, int ms)
{
  int i, n, re = 0, re2 = 0;
  char buf[ FILEBUFSIZE ];
  BEG_DO_PRINT();
  SAFE_SEND_TIMEOUT(s, ms);
  FSIZE(pf, n);
  FSEEKSET(pf, 0);
  IF_ERR_RETURN(NET_SEND2(s, n, flags));
  for (i = 0; i < (n - FILEBUFSIZE + 1); i += FILEBUFSIZE) {
    FREAD(buf, FILEBUFSIZE, pf);
    re2 = net_safesend1(s, buf, FILEBUFSIZE, flags);
    IF_ERR_RETURN(re2);
    re += re2;
    DO_PRINT("发送", flags, n, re);
  }
  if (i < n) {
    FREAD(buf, n - i, pf);
    re2 = net_safesend1(s, buf, n - i, flags);
    IF_ERR_RETURN(re2);
    re += re2;
    DO_PRINT("发送", flags, n, re);
  }
  END_DO_PRINT();
  return re;
}
int CPM_RecvFile(CPM_SOCKET s, HD_FILE pf, int flags, int ms)
{
  int i, n, re = 0, re2 = 0;
  char buf[ FILEBUFSIZE ];
  BEG_DO_PRINT();
  SAFE_RECV_TIMEOUT(s, ms);
  FSEEKSET(pf, 0);
  IF_ERR_RETURN(NET_RECV2(s, n, flags));
  for (i = 0; i < (n - FILEBUFSIZE + 1); i += FILEBUFSIZE) {
    re2 = net_saferecv1(s, buf, FILEBUFSIZE, flags);
    IF_ERR_RETURN(re2);
    re += re2;
    DO_PRINT("接收", flags, n, re);
    FWRITE(buf, FILEBUFSIZE, pf);
  }
  if (i < n) {
    re2 = net_saferecv1(s, buf, n - i, flags);
    IF_ERR_RETURN(re2);
    re += re2;
    DO_PRINT("接收", flags, n, re);
    FWRITE(buf, n - i, pf);
    printf("filesize=%d %x\n", n, pf);
  }
  END_DO_PRINT();
  return re;
}
typedef struct {
  BYTE bFlag1; //标志一=0xff
  WORD wPacketLen; //长度
  BYTE bFlag2; //标志二=0xff
}
TCP_PHEADER, *PTCP_PHEADER;
#define m_nHeaderLen sizeof(TCP_PHEADER)
#define m_nMaxPacketLen (65536 - m_nHeaderLen)
// 返回值:-1:表示接收缓冲区太小
// 返回0:表示socket断开
// 返回>0:表示接收数据长度
int CPM_SafeRecv2(CPM_SOCKET s, char FAR* pbData, int dwDataLen, int flags, int ms)
{
  // 首先接收包头
  int nBytesRead = 0;
  int nCurPacketLen = 0;
  TCP_PHEADER Header;
  PBYTE m_pbRecvHeader = (PBYTE) & Header;
  while (TRUE) {
    int nCurRecvd = CPM_Recv(s, (char*)(m_pbRecvHeader + nBytesRead), m_nHeaderLen - nBytesRead, flags, ms);
    // 接收到0表示失败，退出
    if (nCurRecvd == 0) {
      return 0;
    }
    nBytesRead += nCurRecvd;
    // 接收到完整的包头，退出循环，继续接收包体
    if (nBytesRead == m_nHeaderLen) {
      break;
    }
  }
  // 进一步判断包头是否错误，如果错误，则断开连接，并退出
  {
    TCP_PHEADER* pHeader = (TCP_PHEADER*) m_pbRecvHeader;
    if (pHeader->bFlag1 != 0xff || pHeader->bFlag2 != 0xff || pHeader->wPacketLen > m_nMaxPacketLen) {
      Disconnect(s);
      return CHCSOCK_ERR_HEADER;
    }
    // 因为包头中没有包括两个字节的长度
    nCurPacketLen = pHeader->wPacketLen;
  }
  // 如果接收缓冲区比待接收的数据小，返回-1，对于我们现在的特殊应用就直接返回0，并关闭
  if (nCurPacketLen > dwDataLen) {
    Disconnect(s);
    return CHCSOCK_ERR_LITTLEBUF;
  }
  nBytesRead = 0;
  while (TRUE) {
    int nCurRecvd = CPM_Recv(s, (char*)(pbData + nBytesRead), nCurPacketLen - nBytesRead, flags, ms);
    // 接收到0表示失败，退出
    if (nCurRecvd == 0) {
      return 0;
    }
    nBytesRead += nCurRecvd;
    // 接收到完整的包头，退出循环，继续接收包体
    if (nBytesRead == nCurPacketLen) {
      break;
    }
  }
  return nBytesRead;
}
INT CPM_SafeSend2(CPM_SOCKET s, const char FAR* pbData, int dwDataLen, int flags, int ms)
{
  int nHeadLen = 0;
  int nDataLen = 0;
  TCP_PHEADER Header;
  PBYTE m_pbSendHeader = (PBYTE) & Header;
  // 发送包头
  TCP_PHEADER* pHeader = (TCP_PHEADER*) m_pbSendHeader;
  pHeader->bFlag1 = 0xff;
  pHeader->bFlag2 = 0xff;
  //不包括包头的长度
  pHeader->wPacketLen = (WORD) dwDataLen;
  nHeadLen = CPM_Send(s, (char*) m_pbSendHeader, m_nHeaderLen, flags, ms);
  // 发送实际数据
  nDataLen = CPM_Send(s, (char*) pbData, dwDataLen, flags, ms);
  if (nHeadLen == 0 || nDataLen == 0) {
    Disconnect(s);
    return CHCSOCK_ERR_DISCONNECT;
  }
  return nDataLen;
}
typedef int (*SelectProcCallBack)(SOCKET sClient, void* userdata);
typedef struct JobInfo {
  int JobId;
  SelectProcCallBack Proc;
} JobInfo;
#define MAXJOB 1000
JobInfo* joblist_op(int JobId, SelectProcCallBack Proc)
{
  static JobInfo joblist[MAXJOB];
  static int joblistlen = 0;
  int i = 0;
  for (; i < joblistlen; ++i) {
    if (JobId == joblist[i].JobId) {
      return joblist + i;
    }
  }
  if (NULL == Proc) {
    return i < joblistlen ? joblist + i : NULL;
  }
  else {
    if (i < joblistlen) {
      joblist[i].JobId = JobId;
      joblist[i].Proc = Proc;
    }
    else {
      if (joblistlen < MAXJOB) {
        joblist[i].JobId = JobId;
        joblist[i].Proc = Proc;
        joblistlen++;
      }
    }
    return joblist + i;
  }
  return joblist;
}
char* CPM_GetSockNameStr(SOCKET s)
{
  sockaddr_in addrRemote;
  int nAddrLen = sizeof(addrRemote);
  CPM_GetSockName(s, (SOCKADDR*) &addrRemote, &nAddrLen);
  return inet_ntoa(addrRemote.sin_addr);
}
int StartSelect(SOCKET sListen, void* userdata)
{
  int i;
  int nRet = 0;
  // select模型处理过程
  // 1）初始化一个套节字集合fdSocket，添加监听套节字句柄到这个集合
  fd_set fdSocket; // 所有可用套节字集合
  // 进入监听模式
  listen(sListen, 5);
  FD_ZERO(&fdSocket);
  FD_SET(sListen, &fdSocket);
  while (TRUE) {
    // 2）将fdSocket集合的一个拷贝fdRead传递给select函数，
    // 当有事件发生时，select函数移除fdRead集合中没有未决I/O操作的套节字句柄，然后返回。
    fd_set fdRead = fdSocket;
    nRet = select(0, &fdRead, NULL, NULL, NULL);
    if (nRet > 0) {
      // 3）通过将原来fdSocket集合与select处理过的fdRead集合比较，
      // 确定都有哪些套节字有未决I/O，并进一步处理这些I/O。
      for (i = 0; i < (int) fdSocket.fd_count; i++) {
        if (FD_ISSET(fdSocket.fd_array[ i ], &fdRead)) {
          if (fdSocket.fd_array[ i ] == sListen) { // （1）监听套节字接收到新连接
            if (fdSocket.fd_count < FD_SETSIZE) {
              sockaddr_in addrRemote;
              int nAddrLen = sizeof(addrRemote);
              SOCKET sNew = accept(sListen, (SOCKADDR*) & addrRemote, &nAddrLen);
              FD_SET(sNew, &fdSocket);
              printf("接收到连接（%s）\n", CPM_GetSockNameStr(sNew));
            }
            else {
              printf(" Too much connections! \n");
              continue;
            }
          }
          else {
            JobInfo jobinfo;
            SOCKET sClient = fdSocket.fd_array[ i ];
            int nRecv = CPM_RecvT(sClient, (char*)&jobinfo, sizeof(JobInfo), 0, -1);
            if (nRecv > 0) { // （2）可读
              JobInfo* job = joblist_op(jobinfo.JobId, 0);
              int hasjob = NULL != job;
              //net_rrss( sClient, 8 );
              //net_ssrr( sClient, 9 );
              CPM_Send(sClient, (char*)&hasjob, sizeof(hasjob), 0, 0);
              printf("接收到数据: %s。jobid=%d \n", CPM_GetSockNameStr(sClient), jobinfo.JobId);
              if (job != NULL) {
                printf("开始服务: %s, JobId=%d\n", CPM_GetSockNameStr(sClient), jobinfo.JobId);
                nRecv = job->Proc(fdSocket.fd_array[ i ], userdata);
                if (nRecv <= 0) {
                  printf("断开连接2: %s!\n", CPM_GetSockNameStr(sClient));
                }
                else {
                  printf("关闭连接2: %s!\n", CPM_GetSockNameStr(sClient));
                }
                closesocket(fdSocket.fd_array[ i ]);
                FD_CLR(fdSocket.fd_array[ i ], &fdSocket);
              }
              else {
                printf("没有此项服务: %s, JobId=%d\n", CPM_GetSockNameStr(sClient), jobinfo.JobId);
                closesocket(fdSocket.fd_array[ i ]);
                FD_CLR(fdSocket.fd_array[ i ], &fdSocket);
              }
            }
            else { // （3）连接关闭、重启或者中断
              printf("断开连接3: %s!\n", CPM_GetSockNameStr(sClient));
              closesocket(fdSocket.fd_array[ i ]);
              FD_CLR(fdSocket.fd_array[ i ], &fdSocket);
            }
          }
        }
      }
    }
    else {
      printf(" Failed select() \n");
      break;
    }
  }
  return nRet;
}
int StartJob(SOCKET cc, int JobId)
{
  JobInfo jobinfo;
  int ret, hasjob = 0;
  jobinfo.JobId = JobId;
  CPM_SendT(cc, (char*)&jobinfo, sizeof(JobInfo), 0, 0);
  //net_ssrr( cc, 8 );
  //net_rrss( cc, 9 );
  //Sleep(1000);
  ret = CPM_RecvT(cc, (char*)&hasjob, sizeof(hasjob), 0, -1);
  if (ret < 0) {
    hasjob = 0;
  }
  return hasjob;
}
#define TESTJOBID 12
#define TESTPORT1 4440
#define TESTPORT2 4441
// 获取本机IP
char* gethostip()
{
  char buf[256];
  struct hostent* pHost;
  gethostname(buf, 256);//计算机名字
  pHost = gethostbyname(buf);
  //_snprintf(ipbuf, maxlen, "%s\n",);
  //for ( i = 0; pHost != NULL && pHost->h_addr_list[i] != NULL; i++) {
  // printf("%s\n",inet_ntoa(*(struct in_addr *)pHost->h_addr_list[i]));
  //}
  return inet_ntoa(*(struct in_addr*)pHost->h_addr_list[0]);
}
char* gethostname0()
{
  static char buf[256];
  gethostname(buf, 256);//计算机名字
  return buf;
}
BOOL IsIpString(const char* pszHostOrIp)
{
  char* pszPart;
  char szHostOrIpCopy[16];
  int nPart = 0;
  int i;
  if (strlen(pszHostOrIp) < 7) {
    return FALSE;
  }
  if (strlen(pszHostOrIp) > 15) {
    return FALSE;
  }
  //创建一个输入字符的副本，防止被修改
  strcpy(szHostOrIpCopy, pszHostOrIp);
  pszPart = strtok(szHostOrIpCopy, ".");
  while (pszPart != NULL) {
    for (i = 0; i < (int)strlen(pszPart); i++) {
      if (pszPart[i] > '9' || pszPart[i] < '0') {
        return FALSE;
      }
    }
    if ((unsigned int)(atoi(pszPart)) > 255) {
      return FALSE;
    }
    nPart++;
    pszPart = strtok(NULL, ".");
  }
  return nPart == 4;
}
#if 0
unsigned long GetAddr(const char* pszHostOrIp)
{
  struct hostent* pHostent;
  if (*pszHostOrIp == '\0') {
    return inet_addr("127.0.0.1");
  }
  if (IsIpString(pszHostOrIp)) {
    return inet_addr(pszHostOrIp);
  }
  pHostent = gethostbyname(pszHostOrIp);
  if (pHostent == NULL) {
    return INADDR_NONE;
  }
  return(*(unsigned long*) * pHostent->h_addr_list);
}
#endif
void test_joblist_op(int pPort)
{
  int nPort = (int)pPort;// nPort 此服务器监听的端口号
  SOCKET sListen = CPM_StartServer(nPort);
  if (INVALID_SOCKET == sListen) {
    printf("服务开启失败。port=%d\n", nPort);
    return ;
  }
  printf("服务开启了。port=%d\n", nPort);
  StartSelect(sListen, &pPort);
}
int test_joblist_op_th(int isthd, int id, SelectProcCallBack fun, int iswait, int port)
{
  int i = TESTPORT1;
  DWORD dwIndex;
  HANDLE hThread;
  DWORD threadID;
  joblist_op(id, fun);
  if (isthd) {
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) test_joblist_op, (LPVOID)(port), 0, &threadID);
    //dwIndex = WaitForMultipleObjects(n, hThreads, 1, INFINITE);
    if (iswait) {
      dwIndex = WaitForSingleObject(hThread, INFINITE);
    }
  }
  else {
    test_joblist_op(port);
  }
  return 0;
}

