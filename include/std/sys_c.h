
#ifndef _SYS_C_H_
#define _SYS_C_H_
/////////////////////////////////////////////////////////////////////////////////////////////////
#if 1
#ifdef _WIN32
#define _WINSOCKAPI_  
#include <Windows.h>  
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <winsock.h>
//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")
#endif
#endif

#endif // _SYS_C_H_
