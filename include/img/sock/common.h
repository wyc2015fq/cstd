#ifndef __SOCK_COMMON_H
#define __SOCK_COMMON_H

#include "sock.h"
#include "CriticalSection.h"
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <assert.h>
#include <string.h>
using std::vector;
using std::map;


#ifdef WIN32
#pragma warning( disable : 4786 )
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <process.h>
#include "utime.hpp"
#pragma comment( lib, "ws2_32.lib" )
#pragma comment( lib, "mswsock.lib")
#else
#include "cutimer.h"
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#endif


#ifdef WIN32
#define SOCK_LENTH int 
#define T_VOID void 
#define T_HANDLE HANDLE 
#define SLEEP(Milliseconds) {Sleep(Milliseconds);}
#define LASTERROR WSAGetLastError()
#define MILLISEC       1
#define MEMRESERVER_BLOCKS  10 //内存块大小
#else
#define SOCKET int 
#define SOCK_LENTH socklen_t 
#define T_HANDLE pthread_t 
#define T_VOID void* 
#define INVALID_SOCKET 0
#define SOCKET_ERROR   -1
#define closesocket close
#define ioctlsocket ioctl
#define SLEEP(Milliseconds)  {struct timeval tval;tval.tv_sec= 0;tval.tv_usec=(Milliseconds); select( 0 , NULL , NULL , NULL , &tval ); }
#define LASTERROR errno
#define WSAECONNRESET   ECONNRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAEWOULDBLOCK  EWOULDBLOCK
#define WSAEINTR        EINTR
#define MILLISEC        1000
#define SD_BOTH         SHUT_RDWR
#define MEMRESERVER_BLOCKS  2 //内存块大小
#define INFINITE 0xFFFFFFFF
typedef unsigned int DWORD;
#endif

#define TIME_OUT      5 //10s


typedef sockaddr SOCKADDR, *PSOCKADDR;
typedef sockaddr_in SOCKADDR_IN;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef char  int8;
typedef int   int32;
typedef short int16;


#define IN
#define OUT

#define MAX_SEND_COUNTS 5 //包最大连续发送次数
#define MAX_CONNECTS    5 //一次侦听最大连接数
#define RECC_TIME_OUT   (10*1000) //30s
#define PACK_SIZE (60*1024)
#define MEM_UNIT  (8*1024)

#define ALIGN(bytes, alignments) (((bytes)+((alignments)-1))&~((alignments)-1))

typedef struct 
{
	uint32 msg_identify; //标识
	uint32 msg_offset ;  //偏移
	uint32 msg_len;      //数据长度
	uint32 msg_total;    //数据总长度
	uint32 msg_checksum; //检验和
	//	int8   msg_data[4];  //数据
}SOCK_DATA_HEADER;	


#endif
