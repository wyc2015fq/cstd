//#include "StdAfx.h"
#include "Socket.h"

#define JOIN_GROUP(sock, ip)\
  const int routenum = 10;\
  setsockopt(sock,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&routenum,sizeof(routenum));\
  const int loopback = 0;\
  setsockopt(sock,IPPROTO_IP,IP_MULTICAST_LOOP,(char*)&loopback,sizeof(loopback));\
  ip_mreq group_addr = {0};\
  group_addr.imr_interface.s_addr = htonl(INADDR_ANY);\
  group_addr.imr_multiaddr.s_addr = inet_addr( ip );\
  return ::setsockopt( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (int8*)&group_addr, sizeof(group_addr) ) == SOCKET_ERROR ? false : true;


#define DROP_GROUP(sock, ip)\
  ip_mreq group_addr ={0};\
  group_addr.imr_interface.s_addr = htonl(INADDR_ANY);\
  group_addr.imr_multiaddr.s_addr = inet_addr( ip); \
  return ::setsockopt( sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (int8*)&group_addr, sizeof(group_addr) ) == SOCKET_ERROR ? false : true;

SOCKET sock_create(int32 type)
{
  SOCKET sock = ::socket(AF_INET, type, 0);
  int flag = 1;
  ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
  return sock;
}

SOCKET sock_accept(SOCKET sock, int8* ip, uint16& port)
{
  sockaddr_in addr = {0};
  SOCK_LENTH len = sizeof(addr);
  SOCKET sock_new = ::accept(sock, (sockaddr*)&addr, &len);

  if (sock_new != INVALID_SOCKET) {
    if (ip) {
      strcpy(ip, inet_ntoa(addr.sin_addr));
    }

    port = ntohs(addr.sin_port);
  }

  return sock_new;
}

BOOL sock_connect(SOCKET sock, const int8* ip, uint16 port)
{
  MACK_ADDR(ip, port);

  if (::connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
    return false;
  }
  else {
    return  true;
  }
}

BOOL sock_bind(SOCKET sock, const int8* ip, uint16 port)
{
  int32 on = 1;
  ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const int8*)&on, sizeof(on));
  MACK_ADDR(ip, port);
  return ::bind(sock, (sockaddr*)&addr,  sizeof(addr)) == SOCKET_ERROR ? false : true;
}

BOOL sock_listen(SOCKET sock, int32 cns)
{
  return ::listen(sock, cns) == SOCKET_ERROR ? false : true;
}

BOOL sock_join_group(SOCKET sock, const int8* group_ip)
{
  JOIN_GROUP(sock, group_ip)
}

BOOL sock_drop_group(SOCKET sock, const int8* group_ip)
{
  DROP_GROUP(sock, group_ip)
}

int32 sock_send(SOCKET sock, const int8* data, int32 len, const int8* ip /*= NULL*/, uint16 port/* = 0*/)
{
  MACK_ADDR(ip, port);
  return ::sendto(sock, data, (SOCK_LENTH)len, 0, (sockaddr*)&addr, sizeof(addr));
}

int32 sock_recv(SOCKET sock, int8* data, int32 len, int32 flag)
{
  return ::recv(sock, data, len, flag) ;
}

int32  sock_recvfrom(SOCKET sock, int8* data, int32 len, int8* ip /*= NULL*/, uint16* port/* = NULL*/,  int32 flags/* = 0*/)
{
  sockaddr_in addr = {0};
  SOCK_LENTH size = sizeof(addr);
  len = ::recvfrom(sock, data, len, flags, (sockaddr*)&addr, &size) ;

  if (len != SOCKET_ERROR) {
    if (ip) {
      strcpy(ip, inet_ntoa(addr.sin_addr));
    }

    if (port) {
      *port = ntohs(addr.sin_port);
    }
  }

  return len;
}

BOOL sock_addr(SOCKET sock, int8* ip, uint16& port)
{
  sockaddr_in addr = {0};
  SOCK_LENTH nLen = sizeof(sockaddr_in);

  if (::getpeername(sock, (sockaddr*)&addr, &nLen) != SOCKET_ERROR) {
    if (ip) {
      strcpy(ip, inet_ntoa(addr.sin_addr));
    }

    port = ntohs(addr.sin_port);
    return true;
  }

  return false;
}

BOOL sock_set_recvbuf_size(SOCKET sock, int32 size)
{
  return ::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const int8*)&size, sizeof(int32))  ? false : true ;
}

BOOL sock_set_sendbuf_size(SOCKET sock, int32 size)
{
  return ::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const int8*)&size, sizeof(int32))  ? false : true ;
}

BOOL sock_ioctl(SOCKET sock, int32 cmd, int32 enable)
{
  return ioctlsocket(sock, cmd , (u_long*)&enable) == SOCKET_ERROR ? false : true;
}

int32 sock_type(SOCKET sock)
{
  int32 nType = SOCKET_ERROR ;
  SOCK_LENTH nLen = sizeof(nType);
  getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&nType, &nLen) ;
  return nType;
}

BOOL sock_name(SOCKET sock, char* ip, uint16& port)
{
  sockaddr_in addr = {0};
  SOCK_LENTH nLen = sizeof(sockaddr_in);

  if (SOCKET_ERROR == getsockname(sock, (sockaddr*)&addr, &nLen)) {
    return false;
  }

  if (ip) {
    strcpy(ip, inet_ntoa(addr.sin_addr));
  }

  port = ntohs(addr.sin_port);
  return true;
}


BOOL sock_readamount(SOCKET sock, int32& amount)
{
  return ioctlsocket(sock, FIONREAD, (u_long*)&amount) == SOCKET_ERROR ? false : true;
}


int32 sock_error()
{
  return LASTERROR;
}

uint16 sock_checksum(uint16* buffer, int32 size)
{
  uint32 cksum = 0;

  while (size > 1) {
    cksum += *buffer++;
    size -= sizeof(uint16);
  }

  if (size) {
    cksum += *(uint8*)buffer;
  }

  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);
  return (uint16)(~cksum);
}

void sock_close(SOCKET sock)
{
  closesocket(sock);
}

BOOL sock_localIP(SOCKET sock, int8* lpLocalIP)
{
#ifdef WIN32
  BOOL bExit = false;
  uint8 buffer[8192];
  DWORD outputsize;
  DWORD numaddresses, i;
  SOCKET_ADDRESS_LIST* addrlist;

  if (WSAIoctl(sock, SIO_ADDRESS_LIST_QUERY, NULL, 0, &buffer, 8192, &outputsize, NULL, NULL)) {
    return false;
  }

  addrlist = (SOCKET_ADDRESS_LIST*)buffer;
  numaddresses = addrlist->iAddressCount;

  for (i = 0 ; i < numaddresses ; i++) {
    SOCKET_ADDRESS* sockaddr = &(addrlist->Address[i]);

    if (sockaddr->iSockaddrLength == sizeof(struct sockaddr_in)) { // IPv4 address
      struct sockaddr_in* addr = (struct sockaddr_in*)sockaddr->lpSockaddr;
      int8* temp = inet_ntoa(addr->sin_addr);
      memcpy(lpLocalIP, temp, strlen(temp));
      bExit = true;
      break;
    }
  }

  return bExit;
#else
  return false;
#endif
}

BOOL sock_DNS2LocalIP(int8* lpLocalIP)
{
#ifdef WIN32
  BOOL bExit = false;
  struct hostent* he;
  int8 name[1024];
  uint32 ip;
  BOOL done;
  int32 i;

  gethostname(name, 1023);
  name[1023] = 0;
  he = gethostbyname(name);

  if (he == 0) {
    return false;
  }

  ip = 0;
  i = 0;
  done = false;

  while (!done) {
    if (he->h_addr_list[i] == NULL) {
      done = true;
    }
    else {
      sprintf(lpLocalIP, "%d.%d.%d.%d",
          (uint8)he->h_addr_list[i][0],
          (uint8)he->h_addr_list[i][1],
          (uint8)he->h_addr_list[i][2],
          (uint8)he->h_addr_list[i][3]
             );
      bExit = true;
      break;
    }
  }

  return bExit;
#else
  return false;
#endif
}

#ifdef WIN32
BOOL  sock_wsarecv(SOCKET sock, WSABUF* lpBuffer, uint32* lpNumberOfBytesRecvd, WSAOVERLAPPED* lpOverlapped, uint32& uFlag)
{
  //  uint32 uFlag = 0 ;
  return ::WSARecv(sock, lpBuffer, 1, (LPDWORD)lpNumberOfBytesRecvd, (LPDWORD)&uFlag, lpOverlapped, NULL) == SOCKET_ERROR ? false : true;
}

BOOL sock_wsarecvfrom(SOCKET sock, WSABUF* lpBuffer, uint32* lpNumberOfBytesRecvd, WSAOVERLAPPED* lpOverlapped, uint32& uFlag, sockaddr* pAddr, int32* lpLen)
{
  //  uint32 uFlag = MSG_PEEK  ;
  return ::WSARecvFrom(sock, lpBuffer, 1, (LPDWORD)lpNumberOfBytesRecvd, (LPDWORD)&uFlag, pAddr, lpLen, lpOverlapped, NULL) == SOCKET_ERROR ? false : true;
}

#include <mswsock.h>
BOOL  sock_acceptex(SOCKET listen_sock, SOCKET accept_sock, WSABUF* lpBuffer, WSAOVERLAPPED* lpOverlapped)
{

  //  LPFN_ACCEPTEX lpfnAcceptEx = NULL;    //AcceptExº¯ÊýÖ¸Õë
  //     //Accept function GUID
  //     GUID guidAcceptEx = WSAID_ACCEPTEX;
  //     //get acceptex function pointer
  //     DWORD dwBytes = 0;
  //     WSAIoctl(
  //    listen_sock,
  //    SIO_GET_EXTENSION_FUNCTION_POINTER,
  //         &guidAcceptEx,
  //    sizeof(guidAcceptEx),
  //    &lpfnAcceptEx,
  //    sizeof(lpfnAcceptEx),
  //         &dwBytes,NULL,NULL);
  //
  // //
  //
  //  return lpfnAcceptEx(
  //    listen_sock,
  //    accept_sock,
  //    lpBuffer->buf,
  //    0,
  //    sizeof(SOCKADDR_IN)+16,
  //    sizeof(SOCKADDR_IN)+16,
  //    &dwBytes,
  //          lpOverlapped
  //     );

  DWORD dwBytesReceived = 0;
  return ::AcceptEx(
      listen_sock, accept_sock,
      lpBuffer->buf, 0,
      sizeof(struct sockaddr_in) + 16,
      sizeof(struct sockaddr_in) + 16,
      &dwBytesReceived, lpOverlapped);
}

void sock_getacceptexsockaddrs(WSABUF* lpBuffer, int8* ip, uint16& port)
{
  sockaddr_in* LocalSockaddr = NULL;
  sockaddr_in* RemoteSockaddr = NULL;
  int32 LocalSockaddrLength  = sizeof(sockaddr_in);
  int32 RemoteSockaddrLength = sizeof(sockaddr_in);
  GetAcceptExSockaddrs(lpBuffer->buf, 0, sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, (LPSOCKADDR*)&LocalSockaddr, &RemoteSockaddrLength, (LPSOCKADDR*)&RemoteSockaddr, &RemoteSockaddrLength);

  if (RemoteSockaddr) {
    if (ip) {
      strcpy(ip, inet_ntoa(RemoteSockaddr->sin_addr));
    }

    port = ntohs(RemoteSockaddr->sin_port);
  }
}

#endif
