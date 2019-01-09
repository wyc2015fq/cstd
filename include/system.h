
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winsock.h>
#undef max
#undef min
#undef STRICT
//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")
#else
#include <linux/types.h>
#endif // _WIN32

