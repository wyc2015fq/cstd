#include ".\sock\sock.h"
#include ".\sock\libsock.inl"
int test_sock()
{
    SLG_InitSock();
    SOCKET s = SLG_CreateSock(AF_INET6, SOCK_STREAM);
    SLG_UnInitSock();
    return 0;
}

