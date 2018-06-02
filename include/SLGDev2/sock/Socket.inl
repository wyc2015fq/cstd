#include "Socket.h"
#include "mynet.h"
#define JOIN_GROUP(sock, ip)\
  const int routenum = 10;\
  setsockopt(sock,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&routenum,sizeof(routenum));\
  const int loopback = 0;\
  setsockopt(sock,IPPROTO_IP,IP_MULTICAST_LOOP,(char*)&loopback,sizeof(loopback));\
  ip_mreq group_addr = {0};\
  group_addr.imr_interface.s_addr = htonl(INADDR_ANY);\
  group_addr.imr_multiaddr.s_addr = inet_addr( ip );\
  return ::setsockopt( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (int8*)&group_addr, sizeof(group_addr) ) == SOCKET_ERROR ? FALSE : TRUE;
#define DROP_GROUP(sock, ip)\
  ip_mreq group_addr ={0};\
  group_addr.imr_interface.s_addr = htonl(INADDR_ANY);\
  group_addr.imr_multiaddr.s_addr = inet_addr( ip); \
  return ::setsockopt( sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (int8*)&group_addr, sizeof(group_addr) ) == SOCKET_ERROR ? FALSE : TRUE;
SOCKET sock_create(int af, int32 type)
{
  SOCKET sock = ::socket(af, type, 0);
  int flag = 1;
  //printf("sock_create=%d\n", sock);
  ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
  return sock;
}
SOCKET sock_accept(SOCKET sock, addr_in* addr)
{
  SOCK_LENTH len = sizeof(addr_in);
  SOCKET sock_new = ::accept(sock, (struct sockaddr*)&addr, &len);
  return sock_new;
}
BOOL sock_listen(SOCKET sock, int32 cns)
{
  return ::listen(sock, cns) == SOCKET_ERROR ? FALSE : TRUE;
}
BOOL sock_join_group(SOCKET sock, const int8* group_ip)
{
  JOIN_GROUP(sock, group_ip)
  return 0;
}
BOOL sock_drop_group(SOCKET sock, const int8* group_ip)
{
  DROP_GROUP(sock, group_ip)
  return 0;
}
//#include "cstd.h"
//#include "cfile.h"
int32 sock_send(SOCKET sock, const int8* data, int32 datalen, const addr_in* addr)
{
#if 0
  int8 data2[100] = {0};
  int ll = loaddata_11("C:\\aaa.dat", data2, datalen, 0);

  if (ll == 60 && datalen == 60) {
    int dd = memcmp(data2, data, datalen);
    int i;

    for (i = 0; i < datalen; ++i) {
      if (data[i] != data2[i]) {
        int adsf = 0;
      }
    }
  }

#endif
  return ::sendto(sock, data, (SOCK_LENTH)datalen, 0, (sockaddr*)addr, sizeof(addr_in));
}
int32 sock_recv(SOCKET sock, int8* data, int32 len, int32 flag)
{
  return ::recv(sock, data, len, flag) ;
}
int32 sock_recvfrom(SOCKET sock, int8* data, int32 len, addr_in* addr, int32 flags/* = 0*/)
{
  SOCK_LENTH size = sizeof(addr_in);
  len = ::recvfrom(sock, data, len, flags, (sockaddr*)addr, &size) ;
  return len;
}
BOOL sock_addr(SOCKET sock, addr_in* addr)
{
  SOCK_LENTH nLen = sizeof(addr_in);

  if (::getpeername(sock, (sockaddr*)addr, &nLen) != SOCKET_ERROR) {
    return TRUE;
  }

  return FALSE;
}
BOOL sock_set_recvbuf_size(SOCKET sock, int32 size)
{
  return ::setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const int8*)&size, sizeof(int32)) ? FALSE : TRUE ;
}
BOOL sock_set_sendbuf_size(SOCKET sock, int32 size)
{
  return ::setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const int8*)&size, sizeof(int32)) ? FALSE : TRUE ;
}
BOOL sock_ioctl(SOCKET sock, int32 cmd, int32 enable)
{
  return ioctlsocket(sock, cmd , (ulong*)&enable) == SOCKET_ERROR ? FALSE : TRUE;
}
int32 sock_type(SOCKET sock)
{
  int32 nType = SOCKET_ERROR ;
  SOCK_LENTH nLen = sizeof(nType);
  getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&nType, &nLen) ;
  return nType;
}
BOOL sock_name(SOCKET sock, addr_in* addr)
{
  SOCK_LENTH nLen = sizeof(addr_in);

  if (SOCKET_ERROR == getsockname(sock, (sockaddr*)addr, &nLen)) {
    return FALSE;
  }

  return TRUE;
}
BOOL sock_readamount(SOCKET sock, int32& amount)
{
  return ioctlsocket(sock, FIONREAD, (ulong*)&amount) == SOCKET_ERROR ? FALSE : TRUE;
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
BOOL sock_localIP(SOCKET sock, int8* ip, int iplen)
{
#ifdef WIN32
  BOOL bExit = FALSE;
  uint8 buffer[8192];
  DWORD outputsize;
  DWORD numaddresses, i;
  SOCKET_ADDRESS_LIST* addrlist;

  if (WSAIoctl(sock, SIO_ADDRESS_LIST_QUERY, NULL, 0, &buffer, 8192, &outputsize, NULL, NULL)) {
    return FALSE;
  }

  addrlist = (SOCKET_ADDRESS_LIST*)buffer;
  numaddresses = addrlist->iAddressCount;

  for (i = 0 ; i < numaddresses ; i++) {
    SOCKET_ADDRESS* sockaddr = &(addrlist->Address[i]);
    int port = net_ntop(&(sockaddr->lpSockaddr), ip, iplen);
    bExit = TRUE;
    break;
  }

  return bExit;
#else
  return FALSE;
#endif
}
BOOL sock_DNS2LocalIP(int8* lpLocalIP)
{
#ifdef WIN32
  BOOL bExit = FALSE;
  struct hostent* he;
  int8 name[1024];
  uint32 ip;
  BOOL done;
  int32 i;
  gethostname(name, 1023);
  name[1023] = 0;
  he = gethostbyname(name);

  if (he == 0) {
    return FALSE;
  }

  ip = 0;
  i = 0;
  done = FALSE;

  while (!done) {
    if (he->h_addr_list[i] == NULL) {
      done = TRUE;
    }
    else {
      sprintf(lpLocalIP, "%d.%d.%d.%d",
          (uint8)he->h_addr_list[i][0],
          (uint8)he->h_addr_list[i][1],
          (uint8)he->h_addr_list[i][2],
          (uint8)he->h_addr_list[i][3]
             );
      bExit = TRUE;
      break;
    }
  }

  return bExit;
#else
  return FALSE;
#endif
}
#ifdef WIN32
BOOL sock_wsarecv(SOCKET sock, WSABUF* lpBuffer, ulong* lpNumberOfBytesRecvd, WSAOVERLAPPED* lpOverlapped, ulong& uFlag)
{
  // uint32 uFlag = 0 ;
  return ::WSARecv(sock, lpBuffer, 1, lpNumberOfBytesRecvd, &uFlag, lpOverlapped, NULL) == SOCKET_ERROR ? FALSE : TRUE;
}
BOOL sock_wsarecvfrom(SOCKET sock, WSABUF* lpBuffer, ulong* lpNumberOfBytesRecvd, WSAOVERLAPPED* lpOverlapped, ulong& uFlag, sockaddr* pAddr, int32* lpLen)
{
  // uint32 uFlag = MSG_PEEK ;
  return ::WSARecvFrom(sock, lpBuffer, 1, lpNumberOfBytesRecvd, &uFlag, pAddr, lpLen, lpOverlapped, NULL) == SOCKET_ERROR ? FALSE : TRUE;
}
#include <mswsock.h>
BOOL sock_acceptex(SOCKET listen_sock, SOCKET accept_sock, WSABUF* lpBuffer, WSAOVERLAPPED* lpOverlapped)
{
  // LPFN_ACCEPTEX lpfnAcceptEx = NULL; //AcceptExº¯ÊýÖ¸Õë
  // //Accept function GUID
  // GUID guidAcceptEx = WSAID_ACCEPTEX;
  // //get acceptex function pointer
  // DWORD dwBytes = 0;
  // WSAIoctl(
  // listen_sock,
  // SIO_GET_EXTENSION_FUNCTION_POINTER,
  // &guidAcceptEx,
  // sizeof(guidAcceptEx),
  // &lpfnAcceptEx,
  // sizeof(lpfnAcceptEx),
  // &dwBytes,NULL,NULL);
  //
  // //
  //
  // return lpfnAcceptEx(
  // listen_sock,
  // accept_sock,
  // lpBuffer->buf,
  // 0,
  // sizeof(SOCKADDR_IN)+16,
  // sizeof(SOCKADDR_IN)+16,
  // &dwBytes,
  // lpOverlapped
  // );
  DWORD dwBytesReceived = 0;
  return ::AcceptEx(
      listen_sock, accept_sock,
      lpBuffer->buf, 0,
      sizeof(addr_in) + 16,
      sizeof(addr_in) + 16,
      &dwBytesReceived, lpOverlapped);
}
void sock_getacceptexsockaddrs(WSABUF* lpBuffer, addr_in* addr)
{
  addr_in* LocalSockaddr = NULL;
  addr_in* RemoteSockaddr = NULL;
  int32 LocalSockaddrLength = sizeof(addr_in);
  int32 RemoteSockaddrLength = sizeof(addr_in);
  GetAcceptExSockaddrs(lpBuffer->buf, 0, sizeof(addr_in) + 16, sizeof(addr_in) + 16, (LPSOCKADDR*)&LocalSockaddr, &RemoteSockaddrLength, (LPSOCKADDR*)&RemoteSockaddr, &RemoteSockaddrLength);

  if (RemoteSockaddr && addr) {
    //sa->Port = net_ntop(RemoteSockaddr, sa->IP_Address, IP_LEN);
    *addr = *RemoteSockaddr;
  }
}
#endif

