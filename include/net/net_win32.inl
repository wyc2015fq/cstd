
//#include <ws2tcpip.h>

#define _WIN32_WINNT 0x0500
#ifndef WINCE
//#include "OverlappedProcessor.inl"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment( lib, "mswsock.lib")
#else
#pragma comment(lib, "Ws2.lib")
// #pragma comment( lib, "mssock.lib")
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#define SOCK_LENTH int
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
static SOCKET my_socket(int af, int type, int protocol)
{
  sock_init();
  return socket(af, type, protocol);
}

#define socket my_socket

#include "net_impl.inl"
