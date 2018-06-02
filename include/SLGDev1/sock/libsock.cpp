#ifndef _LIBSOCK_INL_
#define _LIBSOCK_INL_

//#include "thread.h"
//#include "sftime.inl"
//#include "DebugAssistant.inl"
//#include "sock_common.inl"
#include "../slgdev_private.h"
//#include "MemPool.inl"
//#include "Socket.inl"
//#include "NetManager.inl"
//#include "NetProcessor.inl"
//#include "SelectProcessor.inl"
#ifdef WIN32
#ifndef WINCE
//#include "OverlappedProcessor.inl"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment( lib, "mswsock.lib")
#else
#pragma comment(lib, "Ws2.lib")
// #pragma comment( lib, "mssock.lib")
#endif
#endif
//#include "SockHandle.inl"
//#include "openssl.inl"
//#include "ThreadPool.cpp"
#endif // _LIBSOCK_INL_
