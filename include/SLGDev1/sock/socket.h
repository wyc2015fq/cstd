#ifndef __SOCKET
#define __SOCKET
#ifdef __cplusplus
extern "C" {
#endif

#include "../slgdev.h"

#define WRITE_TIME_OUT(sock, time)  \
  fd_set fwds;\
  FD_ZERO(&fwds);\
  FD_SET(sock, &fwds);\
  timeval wtval;\
  wtval.tv_sec = 0;\
  wtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, NULL, &fwds, NULL, &wtval) <= 0;
#define READ_TIME_OUT(sock, time)\
  fd_set frds;\
  FD_ZERO(&frds);\
  FD_SET(sock, &frds);\
  timeval rtval;\
  rtval.tv_sec = 0;\
  rtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, &frds, NULL, NULL, &rtval) <= 0;
#define WRITE_ARRAY_TIME_OUT(sockArray, sizeArray, time)  \
  fd_set fwds;\
  FD_ZERO(&fwds);\
  SOCKET sock = 0;\
  int32 i = 0;\
  for (; i < sizeArray; ++i) \
  {\
    FD_SET(sockArray[i], &frds);\
    sock = sockArray[i] > sock ? sockArray[i] : sock;\
  }\
  timeval wtval;\
  wtval.tv_sec = 0;\
  wtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, NULL, &fwds, NULL, &wtval) <= 0;
#define READ_ARRAY_TIME_OUT(sockArray, sizeArray, time)\
  fd_set frds;\
  FD_ZERO(&frds);\
  SOCKET sock = 0;\
  int32 i = 0;\
  for (; i < sizeArray; ++i) \
  {\
    FD_SET(sockArray[i], &frds);\
    if( sockArray[i] > sock ) sock = sockArray[i];\
  }\
  timeval rtval;\
  rtval.tv_sec = 0;\
  rtval.tv_usec = (time*1000);\
  BOOL bTimeOut = ::select(sock+1, &frds, NULL, NULL, &rtval) <= 0;
#if 0
#ifndef 11USE_IPV6
#define MACK_ADDR(af, ip, port)\
  sockaddr_in addr = {0}; int addrlen = sizeof(addr);\
  addr.sin_family = AF_INET;\
  addr.sin_port = htons(port);\
  addr.sin_addr.s_addr = (ip==NULL || strlen(ip) <= 0) ? htonl(INADDR_ANY) : inet_addr(ip);
#else
#define MACK_ADDR(af, ip, port)\
  int32 err;\
  struct addrinfo hints, *res0;\
  char portstr[32] = {0};\
  _snprintf(portstr, 32, "%d", port);\
  memset(&hints, 0, sizeof(hints));\
  hints.ai_family = af;\
  hints.ai_socktype = SOCK_STREAM;\
  hints.ai_flags = AI_PASSIVE; \
  err = getaddrinfo(ip, portstr, &hints, &res0);\
  if (NULL==res0) {    return FALSE;  }
#endif
#endif
    int  sock_error();
    typedef int int32;
    typedef char int8;
    typedef short int16;
    typedef unsigned short uint16;
    int net_ntop(const void* src, char* dst, int size);
    int net_addr(const char* ip, int port, void* addr);
    SOCKET sock_create(int af, int32 type);
    SOCKET sock_accept(SOCKET sock, addr_in* addr);
    int   sock_connect(SOCKET sock, const int8* ip, uint16 port);
    BOOL   sock_bind(SOCKET sock, const int8* ip, uint16 port);
    BOOL   sock_listen(SOCKET sock, int32 cns);
    BOOL   sock_join_group(SOCKET sock, const addr_in* group_ip);
    BOOL   sock_drop_group(SOCKET sock, const addr_in* group_ip);
    int32  sock_send(SOCKET sock, const int8* data, int32 len, const addr_in* addr);
    int32  sock_recv(SOCKET sock, int8* data, int32 len, int32 flags = 0) ;
    int32 sock_recvfrom(SOCKET sock, int8* data, int32 len, addr_in* addr, int32 flags);
    BOOL sock_addr(SOCKET sock, addr_in* addr);
    BOOL   sock_set_recvbuf_size(SOCKET sock, int32 size);
    BOOL   sock_set_sendbuf_size(SOCKET sock, int32 size);
    BOOL   sock_ioctl(SOCKET sock, int32 cmd, int32 enable);
    uint16 sock_checksum(uint16* buffer, int32 size);
    void   sock_close(SOCKET sock);
    BOOL   sock_localIP(SOCKET sock, int8* lpLocalIP);
    BOOL   sock_DNS2LocalIP(int8* lpLocalIP);
    int32  sock_type(SOCKET sock);
    BOOL sock_name(SOCKET sock, addr_in* addr);
    BOOL   sock_readamount(SOCKET sock, int32& amount);
    int net_addr(const char* ip, int port, void* addr);
#ifdef __cplusplus
}
#endif
#endif

