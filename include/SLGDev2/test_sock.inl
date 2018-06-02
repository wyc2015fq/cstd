#include ".\sock\sock.h"
#include ".\sock\libsock.inl"
int test_sock()
{
  CPM_InitSock();
  SOCKET s = CPM_CreateSock(AF_INET6, SOCK_STREAM);
  CPM_UnInitSock();
  return 0;
}

