#ifndef _MYNET_H_
#define _MYNET_H_
#ifdef WIN32
#include <winsock2.h>
//#include <windows.h>
#include <ws2tcpip.h>
//#include <mstcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment( lib, "mswsock.lib")
static int sock_init()
{
  static WSADATA wsaData = {0};

  if (0 == wsaData.wVersion) {
    int RetVal = 0;

    if ((RetVal = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
      fprintf(stderr, "WSAStartup failed with error %d\n", RetVal);
      WSACleanup();
      return -1;
    }
  }

  return 0;
}
SOCKET my_socket(int af, int type, int protocol)
{
  sock_init();
  return socket(af, type, protocol);
}
char* GetErrorMsg2(int dw)
{
  // Retrieve the system error message for the last-error code
  static char buf[256];
  LPTSTR lpMsgBuf = NULL;

  if (0 == dw) {
    dw = GetLastError();
  }

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
  _snprintf(buf, 256, "err: %d %s", dw, lpMsgBuf);
  return buf;
}
//#undef errno
//#define errno GetLastError()
#define strerror GetErrorMsg
#define socket my_socket
#else
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#define WSACleanup() (0)
#define WSAGetLastError() (0)
//#define closesocket(x) close(x)
#ifndef NI_MAXHOST
#define NI_MAXHOST   1024
#endif // NI_MAXHOST
typedef int SOCKET;
typedef struct sockaddr_storage SOCKADDR_STORAGE;
#define _snprintf snprintf
#define sock_init()
#define closesocket(x)  close(x)
#endif
#include "inet_ntop.inl"
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;
typedef struct sockaddr sockaddr;
//typedef struct addrinfo addrinfo;
#define myFormatMessage(x) "x"
//0 失败
static int StrIsIP(const char* cszIP)
{
  if (NULL == cszIP) {
    return 0;
  }
  else {
    int i, nLen = strlen(cszIP);
    int nDot = 0;
    int nMao = 0; // count of ':'
    int nss = 0; // count of '// or \'

    for (i = 0; i < nLen; ++i) {
      if (cszIP[i] == '.') {
        nDot++;
      }
      else if (cszIP[i] == ':') {
        nMao++;
      }
      else if (cszIP[i] == '\\' || cszIP[i] == '/') {
        nss++;
      }
    }

    if (nss > 0) {
      return 0;
    }

    if (0 == nMao) {
      return (3 == nDot) ? 1 : 3;
    }
    else {
      return (nMao >= 2) ? 2 : 3;
    }
  }
}
int DomainToIP(const char* host, const char* portStr, char* ip)
{
  struct addrinfo hints, *res, *res0;
  char str[32] = {0};
  int ret;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags    = AI_PASSIVE;

  ret = getaddrinfo(host, portStr, &hints, &res0);

  if (ret != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    return 0;
  }

  for (res = res0; res; res = res->ai_next) {
    if (res->ai_family == AF_INET) {
      // Found IPv4 address
      inet_ntop(AF_INET, &(((struct sockaddr_in*)(res->ai_addr))->sin_addr), str, 32);
      //printf("解析出来的IP: %s\n", str);
      strncpy(ip, str, 32);
    }
    else if (res->ai_family == AF_INET6) {
      // Found IPv6 address
      inet_ntop(AF_INET6, &(((struct sockaddr_in*)(res->ai_addr))->sin_addr), str, 32);
      //printf("解析出来的IP6: %s\n", str);
      strncpy(ip, str, 32);
    }
  }

  freeaddrinfo(res0);
  return 1;
}
int net_addr(const char* ip, int port, void* addr)
{
  int t = StrIsIP(ip);

  //int af = 2 == t ? AF_INET6 : AF_INET;
  switch (t) {
  case 0:
  case 1: {
    struct sockaddr_in* p = (struct sockaddr_in*)addr;
    memset(p, 0, sizeof(sockaddr_in));
    p->sin_family = AF_INET;
    p->sin_port = htons(port);

    if (ip && 1 == t) {
      p->sin_addr.s_addr = inet_addr(ip);
    }
    else {
      //p->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
      p->sin_addr.s_addr = htonl(INADDR_ANY);
    }
  }

  return sizeof(sockaddr_in);

  case 2: {
    struct sockaddr_in6* p = (struct sockaddr_in6*)addr;
    memset(p, 0, sizeof(sockaddr_in6));
    p->sin6_family = AF_INET6; // IPv6
    p->sin6_port = htons(port); // IPv6

    if (ip && 2 == StrIsIP(ip)) {
      inet_pton6(ip, (u_char*)&p->sin6_addr); // IPv6
    }
    else {
      //in6_addr myin6addr_any = in6addr_any;
      const static struct in6_addr myin6addr_any = {0};
      //p->sin6_addr = in6addr_loopback; // IPv6
      p->sin6_addr = myin6addr_any; // IPv6
      //inet_pton(af, "::1", &p->sin6_addr); // IPv6
    }
  }

  return sizeof(sockaddr_in6);

  case 3: {
    struct addrinfo hints, *res, *res0;
    char portStr[32] = {0};
    int ret;
    _snprintf(portStr, 32, "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE;

    ret = getaddrinfo(ip, portStr, &hints, &res0);

    if (ret != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
      return 0;
    }

    for (res = res0; res; res = res->ai_next) {
      if (res->ai_family == AF_INET) {
        memcpy(addr, res->ai_addr, sizeof(sockaddr_in));
        return sizeof(sockaddr_in);
      }
      else {
        memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in6));
        return sizeof(sockaddr_in6);
      }
    }
  }

  return (0);

  default:
    return (0);
  }

  return (0);
}
char* getsocknamestr(SOCKET s)
{
  sockaddr_in addrRemote;
  socklen_t nAddrLen = sizeof(addrRemote);
  getsockname(s, (sockaddr*) &addrRemote, &nAddrLen);
  return inet_ntoa(addrRemote.sin_addr);
}
#endif // _MYNET_H_

