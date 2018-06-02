
#ifndef _COMMON_H_
#define _COMMON_H_
#ifdef _WIN32
// Insert your headers here
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#else
#include <stdint.h>
#endif

#include ".\slgdev.h"
#include ".\sock\mynet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
//#include <windows.h>
#include <ws2tcpip.h>
//#include <mstcpip.h>
#include <stdlib.h>
#include <stdio.h>
static int sock_init()
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
#define sock_init()
#define closesocket(x)  close(x)
#endif


struct slgdev_config_t
{
    int m_debug;
};

slgdev_config_t* GetConfig();

int net_ntop(const void* src, char* dst, int size);
int OutputInfo(const char* format , ...);
#define DEBUG_INFO OutputInfo

#endif // _COMMON_H_
