
#include "../slgdev_private.h"
#include "socket.h"
#ifdef _WIN32
#include <winsock2.h>
//#include <windows.h>
#include <ws2tcpip.h>
//#include <mstcpip.h>
#include <stdlib.h>
#include <stdio.h>
int sock_init()
{
    static WSADATA wsaData = {0};

    if (0 == wsaData.wVersion)
    {
        int RetVal = 0;

        if ((RetVal = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
        {
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
char* GetErrorMsg1(int dw)
{
    // Retrieve the system error message for the last-error code
    static char buf[256];
    LPTSTR lpMsgBuf = NULL;

    if (0 == dw)
    {
        dw = GetLastError();
    }

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
    _snprintf(buf, 256, "err: %d %s", dw, lpMsgBuf);
    return buf;
}
//#undef errno
//#define errno GetLastError()
//#define strerror GetErrorMsg
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
#define closesocket(x)  close(x)
int sock_init() {
  return 1;
}
int _chdir(const char* path) {
  return chdir(path);
}
#endif
#include "inet_ntop.h"
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;
typedef struct sockaddr sockaddr;
typedef struct addrinfo addrinfo;
#define myFormatMessage(x) "x"
//0 失败
static int StrIsIP(const char* cszIP)
{
    if (NULL == cszIP)
    {
        return 0;
    }

    else
    {
        int i, nLen = strlen(cszIP);
        int nDot = 0;
        int nMao = 0; // count of ':'
        int nSlash = 0; // count of '// or \'
        int n0_9 = 0;
        int nA_F = 0;
        int nOther = 0;

        for (i = 0; i < nLen; ++i)
        {
            char c = cszIP[i];

            if (c == '.')
            {
                nDot++;
            }

            else if (c == ':')
            {
                nMao++;
            }

            else if (c == '\\' || c == '/')
            {
                nSlash++;
            }

            else if (c >= '0' && c <= '9')
            {
                n0_9++;
            }

            else if (c >= 'A' && c <= 'F')
            {
                nA_F++;
            }

            else if (c >= 'a' && c <= 'a')
            {
                nA_F++;
            }

            else
            {
                nOther++;
            }
        }

        if (nSlash > 0)
        {
            return 0;
        }

        if (0 == nMao)
        {
            return (3 == nDot && nOther == 0 && nA_F == 0 && n0_9 > 0) ? 1 : 3;
        }

        else
        {
            return (nMao >= 2 && nOther == 0) ? 2 : 3;
        }
    }
}
#if 0
int DomainToIP(const char* host, const char* portStr, char* ip)
{
    addrinfo hints, *res, *res0;
    char str[32] = {0};
    int ret;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE;

    ret = getaddrinfo(host, portStr, &hints, &res0);

    if (ret != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        return 0;
    }

    for (res = res0; res; res = res->ai_next)
    {
        if (res->ai_family == AF_INET)
        {
            // Found IPv4 address
            inet_ntop(AF_INET, &(((struct sockaddr_in*)(res->ai_addr))->sin_addr), str, 32);
            //printf("解析出来的IP: %s\n", str);
            strncpy(ip, str, 32);
        }

        else if (res->ai_family == AF_INET6)
        {
            // Found IPv6 address
            inet_ntop(AF_INET6, &(((struct sockaddr_in*)(res->ai_addr))->sin_addr), str, 32);
            //printf("解析出来的IP6: %s\n", str);
            strncpy(ip, str, 32);
        }
    }

    freeaddrinfo(res0);
    return 1;
}
static int net_addr_1(const char* ip, int port, void* addr)
{
    int t = StrIsIP(ip);
    sock_init();

    //int af = 2 == t ? AF_INET6 : AF_INET;
    switch (t)
    {
    case 0:
    case 1:
    {
        struct sockaddr_in* p = (struct sockaddr_in*)addr;
        memset(p, 0, sizeof(sockaddr_in));
        p->sin_family = AF_INET;
        p->sin_port = htons(port);

        if (ip && 1 == t)
        {
            p->sin_addr.s_addr = inet_addr(ip);
        }

        else
        {
            //p->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            p->sin_addr.s_addr = htonl(INADDR_ANY);
        }
    }

    return sizeof(sockaddr_in);

    case 2:
    {
        struct sockaddr_in6* p = (struct sockaddr_in6*)addr;
        memset(p, 0, sizeof(sockaddr_in6));
        p->sin6_family = AF_INET6; // IPv6
        p->sin6_port = htons(port); // IPv6

        if (ip && 2 == t)
        {
            inet_pton6(ip, (u_char*)&p->sin6_addr); // IPv6
        }

        else
        {
            //in6_addr myin6addr_any = in6addr_any;
            const static struct in6_addr myin6addr_any = {0};
            //p->sin6_addr = in6addr_loopback; // IPv6
            p->sin6_addr = myin6addr_any; // IPv6
            //inet_pton(af, "::1", &p->sin6_addr); // IPv6
        }
    }

    return sizeof(sockaddr_in6);

    case 3:
    {
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

        if (ret != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
            return 0;
        }

        for (res = res0; res; res = res->ai_next)
        {
            if (res->ai_family == AF_INET)
            {
                memcpy(addr, res->ai_addr, sizeof(sockaddr_in));
                return sizeof(sockaddr_in);
            }

            else
            {
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
#endif
int net_addr(const char* ip, int port, void* addr)
{
    struct addrinfo hints, *res, *res0;
    char portStr[32] = {0};
    int ret;
    sock_init();
    snprintf(portStr, 32, "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags    = AI_PASSIVE;

    ret = getaddrinfo(ip, portStr, &hints, &res0);

    if (ret != 0)
    {
        DEBUG_INFO("getaddrinfo(ret=%d) err: %s", ret, gai_strerror(ret));
        return 0;
    }

    if (1)
    {
        int i = 0;
        char buf[256];
        DEBUG_INFO("getaddrinfo(ret=%d) ret: %s", ret, gai_strerror(ret));

        for (res = res0; res; res = res->ai_next)
        {
            net_ntop(res->ai_addr, buf, 256);
            DEBUG_INFO("addr=%s flags=%d family=%d type=%d protocol=%d len=%d", buf,
                       res->ai_flags, res->ai_family,
                       res->ai_socktype, res->ai_protocol,
                       res->ai_addrlen);
            ++i;
        }
    }

    for (res = res0; res; res = res->ai_next)
    {
        if (res->ai_family == AF_INET)
        {
            memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in));
            return sizeof(sockaddr_in);
        }

        else if (res->ai_family == AF_INET6)
        {
            memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in6));
            return sizeof(sockaddr_in6);
        }
    }

    return 0;
}
char* getsocknamestr(SOCKET s)
{
    sockaddr_in addrRemote;
    socklen_t nAddrLen = sizeof(addrRemote);
    getsockname(s, (sockaddr*) &addrRemote, &nAddrLen);
    return inet_ntoa(addrRemote.sin_addr);
}
SOCKET sock_create(int af, int32 type)
{
    SOCKET sock = ::socket(af, type, 0);
    int flag = 1, rcvbuf = 64 * 1024;
    //printf("sock_create=%d\n", sock);
    ::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const int8*)&rcvbuf, sizeof(int32));
    return sock;
}

SOCKET sock_accept(SOCKET sock, addr_in* addr)
{
    SOCK_LENTH len = sizeof(addr_in);
    SOCKET sock_new = ::accept(sock, (struct sockaddr*)addr, &len);
    return sock_new;
}

BOOL sock_listen(SOCKET sock, int32 cns)
{
    return ::listen(sock, cns) == SOCKET_ERROR ? FALSE : TRUE;
}

BOOL sock_join_group(SOCKET sock, const addr_in* group_addr1)
{
  const int routenum = 10; const int loopback = 0;
  ip_mreq group_addr = {0};
  setsockopt(sock,IPPROTO_IP,IP_MULTICAST_TTL,(char*)&routenum,sizeof(routenum));
  setsockopt(sock,IPPROTO_IP,IP_MULTICAST_LOOP,(char*)&loopback,sizeof(loopback));
  group_addr.imr_interface = ((struct sockaddr_in*)group_addr1)->sin_addr;
  return ::setsockopt( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (int8*)&group_addr, sizeof(group_addr) ) == SOCKET_ERROR ? FALSE : TRUE;
}

BOOL sock_drop_group(SOCKET sock, const addr_in* group_addr1)
{
  ip_mreq group_addr ={0};
  group_addr.imr_interface = ((struct sockaddr_in*)group_addr1)->sin_addr;
  return ::setsockopt( sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (int8*)&group_addr, sizeof(group_addr) ) == SOCKET_ERROR ? FALSE : TRUE;
}

//#include "cstd.h"
//#include "cfile.h"
int32 sock_send(SOCKET sock, const int8* data, int32 datalen, const addr_in* addr)
{
    return ::sendto(sock, data, (SOCK_LENTH)datalen, 0, (sockaddr*)addr, sizeof(addr_in));
}

int32 sock_recv(SOCKET sock, int8* data, int32 len, int32 flag)
{
    return ::recv(sock, data, len, flag) ;
}

int32 sock_recvfrom(SOCKET sock, int8* data, int32 len, addr_in* addr, int32 flags)
{
    SOCK_LENTH size = sizeof(addr_in);
    len = ::recvfrom(sock, data, len, flags, (sockaddr*)addr, &size) ;
    return len;
}

BOOL sock_addr(SOCKET sock, addr_in* addr)
{
    SOCK_LENTH nLen = sizeof(addr_in);

    if (::getpeername(sock, (sockaddr*)addr, &nLen) != SOCKET_ERROR)
    {
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

    if (SOCKET_ERROR == getsockname(sock, (sockaddr*)addr, &nLen))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL sock_readamount(SOCKET sock, int32& amount)
{
    return ioctlsocket(sock, FIONREAD, (ulong*)&amount) == SOCKET_ERROR ? FALSE : TRUE;
}

int sock_error()
{
    return LASTERROR;
}

uint16 sock_checksum(uint16* buffer, int32 size)
{
    uint32 cksum = 0;

    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(uint16);
    }

    if (size)
    {
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

#endif

