#ifndef __SOCKET
#define __SOCKET

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#define MACK_ADDR(ip, port)\
  sockaddr_in addr = {0};\
  addr.sin_family = AF_INET;\
  addr.sin_port = htons(port);\
  addr.sin_addr.s_addr = (ip==NULL || strlen(ip) <= 0) ? htonl(INADDR_ANY) : inet_addr(ip);

  SOCKET sock_create(int32 type);
  SOCKET sock_accept(SOCKET sock, int8* ip, uint16& port);
  BOOL   sock_connect(SOCKET sock, const int8* ip, uint16 port);
  BOOL   sock_bind(SOCKET sock, const int8* ip, uint16 port);
  BOOL   sock_listen(SOCKET sock, int32 cns);
  BOOL   sock_join_group(SOCKET sock, const int8* group_ip);
  BOOL   sock_drop_group(SOCKET sock, const int8* group_ip);
  int32  sock_send(SOCKET sock, const int8* data, int32 len, const int8* ip = NULL, uint16 port = 0);
  int32  sock_recv(SOCKET sock, int8* data, int32 len, int32 flags = 0) ;
  int32  sock_recvfrom(SOCKET sock, int8* data, int32 len, int8* ip = NULL, uint16* port = NULL,  int32 flags = 0) ;
  BOOL   sock_addr(SOCKET sock, int8* ip, uint16& port);
  BOOL   sock_set_recvbuf_size(SOCKET sock, int32 size);
  BOOL   sock_set_sendbuf_size(SOCKET sock, int32 size);
  BOOL   sock_ioctl(SOCKET sock, int32 cmd, int32 enable);
  int32  sock_error();
  uint16 sock_checksum(uint16* buffer, int32 size);
  void   sock_close(SOCKET sock);
  BOOL   sock_localIP(SOCKET sock, int8* lpLocalIP);
  BOOL   sock_DNS2LocalIP(int8* lpLocalIP);
  int32  sock_type(SOCKET sock);
  BOOL   sock_name(SOCKET sock, char* ip, uint16& port);
  BOOL   sock_readamount(SOCKET sock, int32& amount);
#ifdef WIN32
  BOOL  sock_wsarecv(SOCKET sock, WSABUF* lpBuffer, uint32* lpNumberOfBytesRecvd, WSAOVERLAPPED* lpOverlapped, uint32& uFlag);
  BOOL  sock_wsarecvfrom(SOCKET sock, WSABUF* lpBuffer, uint32* lpNumberOfBytesRecvd, WSAOVERLAPPED* lpOverlapped, uint32& uFlag, sockaddr* pAddr, int32* lpLen);
  BOOL  sock_acceptex(SOCKET listen_sock, SOCKET accept_sock, WSABUF* lpBuffer, WSAOVERLAPPED* lpOverlapped);
  void  sock_getacceptexsockaddrs(WSABUF* lpBuffer, int8* ip, uint16& port);
#endif

#ifdef __cplusplus
}
#endif

#endif
