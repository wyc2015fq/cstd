
#ifndef _NET_INL_
#define _NET_INL_

#ifdef _WIN32
#include "net/net_win32.inl"
#elif defined __linux__
#include "net/net_linux.inl"
#else
#error unkown netapi
#endif

#endif // _NET_INL_
