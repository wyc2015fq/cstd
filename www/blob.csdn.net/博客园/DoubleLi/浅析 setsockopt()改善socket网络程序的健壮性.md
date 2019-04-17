# 浅析:setsockopt()改善socket网络程序的健壮性 - DoubleLi - 博客园






1. 如果在已经处于 ESTABLISHED状态下的socket(一般由端口号和标志符区分）调用
closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket：
BOOL bReuseaddr=TRUE;
setsockopt(s，SOL_SOCKET ，SO_REUSEADDR，(const char*)&bReuseaddr，sizeof(BOOL));


2. 如果要已经处于连接状态的soket在调用closesocket后强制关闭，不经历
TIME_WAIT的过程：
BOOL bDontLinger = FALSE; 
setsockopt(s，SOL_SOCKET，SO_DONTLINGER，(const char*)&bDontLinger，sizeof(BOOL));


3.在send()，recv()过程中有时由于网络状况等原因，发收不能预期进行，而设置收发时限：
int nNetTimeout=1000;//1秒
//发送时限
setsockopt(socket，SOL_S0CKET，SO_SNDTIMEO，(char *)&nNetTimeout，sizeof(int));
//接收时限
setsockopt(socket，SOL_S0CKET，SO_RCVTIMEO，(char *)&nNetTimeout，sizeof(int));


4.在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节
(异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据
和接收数据量比较大，可以设置socket缓冲区，而避免了send()，recv()不断的循环收发：
// 接收缓冲区
int nRecvBuf=32*1024;//设置为32K
setsockopt(s，SOL_SOCKET，SO_RCVBUF，(const char*)&nRecvBuf，sizeof(int));
//发送缓冲区
int nSendBuf=32*1024;//设置为32K
setsockopt(s，SOL_SOCKET，SO_SNDBUF，(const char*)&nSendBuf，sizeof(int));


5. 如果在发送数据的时，希望不经历由系统缓冲区到socket缓冲区的拷贝而影响
程序的性能：
int nZero=0;
setsockopt(socket，SOL_S0CKET，SO_SNDBUF，(char *)&nZero，sizeof(nZero));


6.同上在recv()完成上述功能(默认情况是将socket缓冲区的内容拷贝到系统缓冲区)：
int nZero=0;
setsockopt(socket，SOL_S0CKET，SO_RCVBUF，(char *)&nZero，sizeof(int));


7.一般在发送UDP数据报的时候，希望该socket发送的数据具有广播特性：
BOOL bBroadcast=TRUE; 
setsockopt(s，SOL_SOCKET，SO_BROADCAST，(const char*)&bBroadcast，sizeof(BOOL));


8.在client连接服务器过程中，如果处于非阻塞模式下的socket在connect()的过程中可
以设置connect()延时，直到accpet()被呼叫(本函数设置只有在非阻塞的过程中有显著的
作用，在阻塞的函数调用中作用不大)
BOOL bConditionalAccept=TRUE;
setsockopt(s，SOL_SOCKET，SO_CONDITIONAL_ACCEPT，(const char*)&bConditionalAccept，sizeof(BOOL));


9.如果在发送数据的过程中(send()没有完成，还有数据没发送)而调用了closesocket()，以前我们
一般采取的措施是"从容关闭"shutdown(s，SD_BOTH)，但是数据是肯定丢失了，如何设置让程序满足具体
应用的要求(即让没发完的数据发送出去后在关闭socket)？
struct linger {
u_short l_onoff;
u_short l_linger;
};
linger m_sLinger;
m_sLinger.l_onoff=1;//(在closesocket()调用，但是还有数据没发送完毕的时候容许逗留)
// 如果m_sLinger.l_onoff=0;则功能和2.)作用相同;
m_sLinger.l_linger=5;//(容许逗留的时间为5秒)
setsockopt(s，SOL_SOCKET，SO_LINGER，(const char*)&m_sLinger，sizeof(linger));
Note:1.在设置了逗留延时，用于一个非阻塞的socket是作用不大的，最好不用;2.如果想要程序不经历SO_LINGER需要设置SO_DONTLINGER，或者设置l_onoff=0；


10.还一个用的比较少的是在SDI或者是Dialog的程序中，可以记录socket的调试信息：
(前不久做过这个函数的测试，调式信息可以保存，包括socket建立时候的参数，采用的
具体协议，以及出错的代码都可以记录下来）
BOOL bDebug=TRUE;
setsockopt(s，SOL_SOCKET，SO_DEBUG，(const char*)&bDebug，sizeof(BOOL));


11.附加：往往通过setsockopt()设置了缓冲区大小，但还不能满足数据的传输需求，
我的习惯是自己写个处理网络缓冲的类，动态分配内存; 一般的习惯是自己写个处理网络缓冲的类，动态分配内存;下面我将这个类写出，希望对大家有所帮助：  

//仿照String    改写而成  
//==============================================================================  
//  二进制数据，主要用于收发网络缓冲区的数据  
//  CNetIOBuffer  以  MFC  类  CString  的源代码作为蓝本改写而成，用法与  CString  类似，  
//  但是  CNetIOBuffer  中存放的是纯粹的二进制数据，'/0'  并不作为它的结束标志。  
//  其数据长度可以通过  GetLength()  获得，缓冲区地址可以通过运算符  LPBYTE  获得。  


//==============================================================================  
//    Copyright  (c)  All-Vision  Corporation.  All  rights  reserved.  
//    Module:    NetObject  
//    File:        SimpleIOBuffer.h  
//    Author:    gdy119  
//    Email  :    8751webmaster@126.com              
//    Date:      2004.11.26  
//==============================================================================  
//  NetIOBuffer.h  
#ifndef  _NETIOBUFFER_H  
#define  _NETIOBUFFER_H  
//=============================================================================  
#define    MAX_BUFFER_LENGTH    1024*1024  
//=============================================================================  
//主要用来处理网络缓冲的数据  
class    CNetIOBuffer      
{  
protected:  
           LPBYTE                            m_pbinData;  
           int                                  m_nLength;  
           int                                  m_nTotalLength;  
           CRITICAL_SECTION            m_cs;  
       void    Initvalibers();  
public:  
           CNetIOBuffer();  
           CNetIOBuffer(const  LPBYTE  lbbyte,  int  nLength);  
           CNetIOBuffer(const  CNetIOBuffer&binarySrc);  
           virtual  ~CNetIOBuffer();  
//=============================================================================              
           BOOL            CopyData(const  LPBYTE  lbbyte,  int  nLe  
---------------------------------------------------------------  

其实我觉得第5条很应该值得注意  
int  nZero=0;  
setsockopt(socket，SOL_S0CKET,SO_SNDBUF，(char  *)&nZero,sizeof(nZero));  

记得以前有些朋友讨论过，socket虽然send成功了，但是其实只是发送到数据缓冲区里面了，而并没有真正的在物理设备上发送出去；而通过这条语句，将发送缓冲区设置为0，即屏蔽掉发送缓冲以后，一旦send返回（当然是就阻塞套结字来说），就可以肯定数据已经在发送的途中了^_^，但是这样做也许会影响系统的性能  

---------------------------------------------------------------  

setoptsock（）这个函数  设置成端口复用的时候，很容易对一些没有进行单独bind模式的程序造成危害。  
比如old的  ping  icmp  door，简单的sniffer后，收到包，然后设置setoptsock  bind  web服务，然后建立个cmd进程  bind再80端口。



======================================

Example Code


The following example demonstrates the **setsockopt** function.



#include <stdio.h>
#include "winsock2.h"

void main() {

//---------------------------------------
// Declare variables
WSADATA wsaData;
SOCKET ListenSocket;
sockaddr_in service;

//---------------------------------------
// Initialize Winsock
int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
if( iResult != NO_ERROR )
printf("Error at WSAStartup/n");

//---------------------------------------
// Create a listening socket
ListenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
if (ListenSocket == INVALID_SOCKET) {
printf("Error at socket()/n");
WSACleanup();
return;
}

//---------------------------------------
// Bind the socket to the local IP address
// and port 27015
h[os](http://www.it130.net/OS/index.htm)tent* thisH[os](http://www.it130.net/OS/index.htm)t;
char* ip;
u_short port;
port = 27015;
thisH[os](http://www.it130.net/OS/index.htm)t = geth[os](http://www.it130.net/OS/index.htm)tbyname("");
ip = inet_ntoa (*(struct in_addr *)*thisH[os](http://www.it130.net/OS/index.htm)t->h_addr_list);

service.sin_family = AF_INET;
service.sin_addr.s_addr = inet_addr(ip);
service.sin_port = htons(port);

if ( bind( ListenSocket,(SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR ) {
printf("bind failed/n");
cl[os](http://www.it130.net/OS/index.htm)esocket(ListenSocket);
return;
}

//---------------------------------------
// Initialize variables and call **setsockopt**. 
// The SO_KEEPALIVE parameter is a socket option 
// that makes the socket send keepalive messages
// on the session. The SO_KEEPALIVE socket option
// requires a boolean value to be passed to the
// **setsockopt** function. If TRUE, the socket is
// configured to send keepalive messages, if FALSE
// the socket configured to NOT send keepalive messages.
// This section of code tests the **setsockopt** function
// by checking the status of SO_KEEPALIVE on the socket
// using the getsockopt function.
BOOL bOptVal = TRUE;
int bOptLen = sizeof(BOOL);
int iOptVal;
int iOptLen = sizeof(int);

if (getsockopt(ListenSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&iOptVal, &iOptLen) != SOCKET_ERROR) {
printf("SO_KEEPALIVE Value: %ld/n", iOptVal);
}

if (**setsockopt**(ListenSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, bOptLen) != SOCKET_ERROR) {
printf("Set SO_KEEPALIVE: ON/n");
}

if (getsockopt(ListenSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&iOptVal, &iOptLen) != SOCKET_ERROR) {
printf("SO_KEEPALIVE Value: %ld/n", iOptVal);
}

WSACleanup();
return;

}




Notes for IrDA Sockets

Keep in mind the following:


The Af_irda.h header file must be explicitly included. 
IrDA provides the following settable socket option: 
Value Type Meaning 
IRLMP_IAS_SET *IAS_SET Sets IAS attributes 


The IRLMP_IAS_SET socket option enables the application to set a single attribute of a single class in the local IAS. The application specifies the class to set, the attribute, and attribute type. The application is expected to allocate a buffer of the necessary size for the passed parameters.

IrDA provides an IAS [Database](http://www.it130.net/Database/index.htm) that stores IrDA-based information. Limited [access](http://www.it130.net/access/index.htm) to the IAS [Database](http://www.it130.net/Database/index.htm) is available through the [windows](http://www.it130.net/windows/index.htm) Sockets 2 interface, but such [access](http://www.it130.net/access/index.htm) is not normally used by applications, and exists primarily to support connections to non-[windows](http://www.it130.net/windows/index.htm) devices that are not compliant with the [windows](http://www.it130.net/windows/index.htm) Sockets 2 IrDA conventions.

The following structure, IAS_SET, is used with the IRLMP_IAS_SET **setsockopt** option to manage the local IAS[Database](http://www.it130.net/Database/index.htm):


typedef struct _IAS_SET {
char irdaClassName[IAS_MAX_CLASSNAME];
char irdaAttribName[IAS_MAX_ATTRIBNAME];
u_long irdaAttribType;
union
{
LONG irdaAttribInt;
struct
{
u_short Len;
u_char OctetSeq[IAS_MAX_OCTET_STRING];
} irdaAttribOctetSeq;
struct
{
u_char Len;
u_char CharSet;
u_char UsrStr[IAS_MAX_USER_STRING];
} irdaAttribUsrStr;
} irdaAttribute;
} IAS_SET, *PIAS_SET, FAR *LPIAS_SET;

The following structure, IAS_QUERY, is used with the IRLMP_IAS_QUERY **setsockopt** option to query a peer's IAS [Database](http://www.it130.net/Database/index.htm):


typedef struct _[windows](http://www.it130.net/windows/index.htm)_IAS_QUERY {
u_char irdaDeviceID[4];
char irdaClassName[IAS_MAX_CLASSNAME];
char irdaAttribName[IAS_MAX_ATTRIBNAME];
u_long irdaAttribType;
union
{
LONG irdaAttribInt;
struct
{
u_long Len;
u_char OctetSeq[IAS_MAX_OCTET_STRING];
} irdaAttribOctetSeq;
struct
{
u_long Len;
u_long CharSet;
u_char UsrStr[IAS_MAX_USER_STRING];
} irdaAttribUsrStr;
} irdaAttribute;
} IAS_QUERY, *PIAS_QUERY, FAR *LPIAS_QUERY;

Many SO_ level socket options are not meaningful to IrDA. Only SO_LINGER is specifically supported.

Note **setsockopt** must be called before bind on [windows](http://www.it130.net/windows/index.htm) NT 4.0, [windows](http://www.it130.net/windows/index.htm) 95, and [windows](http://www.it130.net/windows/index.htm) 98 platforms.


Requirements
Client Requires [windows](http://www.it130.net/windows/index.htm) XP, [windows](http://www.it130.net/windows/index.htm) 2000 Professional, [windows](http://www.it130.net/windows/index.htm) NT Workstation, [windows](http://www.it130.net/windows/index.htm) Me, [windows](http://www.it130.net/windows/index.htm) 98, or[windows](http://www.it130.net/windows/index.htm) 95. 
Server Requires [windows](http://www.it130.net/windows/index.htm) Server 2003, [windows](http://www.it130.net/windows/index.htm) 2000 Server, or [windows](http://www.it130.net/windows/index.htm) NT Server. 
Header Declared in Winsock2.h.

Library Link to Ws2_32.lib.

DLL Requires Ws2_32.dll. 









