# Windows平台的原始套接字编程的知识点概要（备忘） - xqhrs232的专栏 - CSDN博客
2018年05月10日 18:14:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：131
原文地址::[https://blog.csdn.net/qq_33205982/article/details/54948250](https://blog.csdn.net/qq_33205982/article/details/54948250)
相关文章
1、Linux网络编程——原始套接字实例：MAC 头部报文分析----[https://blog.csdn.net/lianghe_work/article/details/45171713](https://blog.csdn.net/lianghe_work/article/details/45171713)
2、Windows下利用原始套接字实现的一个抓包程序Demo----[https://blog.csdn.net/kyt511/article/details/45950535](https://blog.csdn.net/kyt511/article/details/45950535)
 其实从大学学习了C语言后，翻看整本教材只有C语言的语法，根本没有网络编程相关的任何内容，现在回想起来，都记不起自己何时在哪本书上学习了套接字编程，说起TCP、UDP，能知道他们的区别，相关的编程的“套路”，即分别在服务器端和客户端应用程序的固定招数，函数也还是有那么多，但是功能方面嘛，仔细想想，一般除了通信传输数据，文件等，就没有其他目的了，即使有多播，组播，广播等，目的也是为了通信，而且都是在局域网内。以上提到的这些都太中规中矩了，总感觉少了些什么，比如网络性能监视类的程序，网络探测、网络攻击等程序肯定光用这个套接字编程技术无法实现。
      写这篇文章是为了温故知新，照着Windows网络编程一书第7章内容而写。所以全手打。
       -----------------------分----割----线-----------------------------------
       原始套接字是允许访问底层传输协议的一种套接字类型，提供了普通套接字所不具备的功能，能够对网络数据包进行某种程度的控制操作。
      原始套接字提供普通套接字不具备的能力有：发送和接收内核不处理其协议字段的IPv4数据包。对于8位IPv4协议字段，大多数内核仅仅处理该字段值为1（ICMP协议）、2（IGMP协议）、6（TCP协议）、17（UDP协议）的数据报，但是协议字段的值还有很多。例如，OSPF路由协议的值为89，如果要处理OSPF数据报文，那么程序必须使用原始套接字读写。
直接总结干货：
首先，在WinSock2中，很熟悉的两个函数：socket()和WSASocket()，第二个参数用常量SOCK_RAW指明，第三个参数IPPROTO_RAW或者IPPROTO_IP
其次，可以不用在原始套接字上调用bind()函数，也可以不用调用connect()函数，
然后，接收数据时可以调用recvfrom()或者WSARecvFrom()函数。
当接收数据时，在接收到一个数据包后，IP协议栈会把满足以下条件的IP数据包传递到原始套接字中：
1、既不是UDP的数据也不是TCP的数据包；2、部分ICMP分组；3、所有的IGMP分组；4、其他所有的IP数据包；5、重组后的分片数据。
传递到原始套接字中的数据包会根据接收条件决定是否拷贝，若满足则拷贝到原始套接字的接收缓冲区中。
假如我们希望能够接收到所有发给网卡的数据，甚至是流经网卡但并非给本机的数据，可以通过设置接收选项SIO_RCVALL就能实现，设置该套接字控制命令则需要通过函数WSAIoctl()。但是比如在设计一个ping程序时，发出请求后接收响应时，很可能有其他的ICMP消息产生干扰，此时就应该从数据来源IP地址，协议类型等方面来判断接收到的数据，正确匹配。
当然了，发送数据时可以调用sendto()或WSASendTo()函数，广播地址，多播地址同样适用（广播或者多播时需要通过setsocketopt()函数设置选项SO_BROADCAST！！）
connect()函数指明远端地址。
如果要设置IP首部，IPv4时选项为IP_HDRINCL，选项级别为IPPROTO_IP。当然，此后才能调用bind()函数来指明本机IP地址。要知道，只有这个选项开启了，才能在IP首部中修改源IP地址！可惜Windows对这些作了限制，如果源IP地址不正确，恶意代码不能通过伪造的源IP地址进行拒绝服务攻击，也不能发送IP欺骗数据包。如果原始套接字无法满足需求，还有WInPcap编程直接操控数据帧。
有两个编程示例：
第一个：使用原始套接字实现ping
    ping是很多操作系统上用来检验主机是否连通网络，以及探测远程主机是否存活的实用工具之一，它通过系那个远程主机发送ICMP协议包并检验取回的远程主机吸纳供应包来实现上述功能。
概念：   位于网络层的IP协议和ICMP协议属于同等级的，
正因为原始套接字才能构造、发送和接收ICMP协议的数据包。ping程序使用了ICMP协议的ECHO类型的请求报文，程序中需要发送ECHO请求和接收响应，并计算间隔时间，判断网络状况。
ECHO请求由ICMP首部和ICMP数据组成，在前面加上IP首部构成IP数据包。即IP数据包=IP首部+ICMP首部+ICMP数据
所以，先定义ICMP首部的结构体，有类型，代码，校验和，ECHO请求的标识，序号5个成员共计8个字节
直接贴代码：
头文件：
#pragma pack(1)
#define ICMP_ECHOREPLY0
#define ICMP_ECHOREQ8
// IP首部结构体
typedef struct tagIPHDR
{
u_char  VIHL; // 版本号和首部长度
u_char TOS;// 服务类型
shortTotLen;// 总的长度
shortID;// 标识
shortFlagOff;// 分片标志及偏移
u_char TTL;// TTL
u_char Protocol;// 协议
u_short Checksum;// 校验和
struct in_addr iaSrc;// 源IP地址
struct in_addr iaDst;// 目的IP地址
}IPHDR, *PIPHDR;
// ICMP首部结构体
typedef struct tagICMPHDR
{
u_char Type;// 类型（表示ECHO请求）
u_char Code;// 代码
u_short Checksum;// 校验和
u_short ID;// 标识
u_short Seq;// 序号
}ICMPHDR, *PICMPHDR;
#define REQ_DATASIZE 32// Echo Request Data size
// ICMP ECHO请求结构体
typedef struct tagECHOREQUEST
{
ICMPHDR icmpHdr;              //ICMP协议首部
DWORDdwTime;               //获取的当前系统时间戳
charcData[REQ_DATASIZE];  //ICMP数据
}ECHOREQUEST, *PECHOREQUEST;
// ICMP ECHO应答结构体
typedef struct tagECHOREPLY
{
IPHDRipHdr;            //IP首部
ECHOREQUEST echoRequest;  //ICMP请求的结构体
char    cFiller[256];     //填充
}ECHOREPLY, *PECHOREPLY;
#pragma pack()
源文件：
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include "ping.h"
void Ping(LPCSTR pstrHost);//调用了SendEchoRequest函数和RecvEchoReply函数。
void ReportError(LPCSTR pstrFrom);
int  WaitForEchoReply(SOCKET s);
u_short in_cksum(u_short *addr, int len);
intSendEchoRequest(SOCKET, LPSOCKADDR_IN);
DWORDRecvEchoReply(SOCKET, LPSOCKADDR_IN, u_char *);
void main(int argc, char **argv)
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(1,1);
    int nRet;
    if (argc != 2)
    {
fprintf(stderr,"\nUsage: ping hostname\n");
return;
    }
    nRet = WSAStartup(wVersionRequested, &wsaData);
    if (nRet)
    {
fprintf(stderr,"\nError initializing WinSock\n");
return;
    }
if (wsaData.wVersion != wVersionRequested)
{
fprintf(stderr,"\nWinSock version not supported\n");
return;
}
Ping(argv[1]);
    WSACleanup();
}
void Ping(LPCSTR pstrHost)
{
SOCKET  rawSocket;
LPHOSTENT lpHost;
struct    sockaddr_in saDest;
struct    sockaddr_in saSrc;
DWORD dwTimeSent;
DWORD dwElapsed;
u_char    cTTL;
int       nLoop;
int       nRet;
// 创建原始套接字，指定协议IPPROTO_ICMP
rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
if (rawSocket == SOCKET_ERROR) 
{
ReportError("socket()");
return;
}
// 获取IP地址
lpHost = gethostbyname(pstrHost);
if (lpHost == NULL)
{
fprintf(stderr,"\nHost not found: %s\n", pstrHost);
return;
}
saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
saDest.sin_family = AF_INET;
saDest.sin_port = 0;
printf("\nPinging %s [%s] with %d bytes of data:\n",
pstrHost,
inet_ntoa(saDest.sin_addr),
REQ_DATASIZE);
//连续ping4次
for (nLoop = 0; nLoop < 4; nLoop++)
{
SendEchoRequest(rawSocket, &saDest);//发送请求
nRet = WaitForEchoReply(rawSocket);//等待响应
if (nRet == SOCKET_ERROR)
{
ReportError("select()");
break;
}
if (!nRet)
{
printf("\nTimeOut");
break;
}
// 接收响应
dwTimeSent = RecvEchoReply(rawSocket, &saSrc, &cTTL);
// 计算间隔时间
dwElapsed = GetTickCount() - dwTimeSent;
printf("\nReply from: %s: bytes=%d time=%ldms TTL=%d", 
               inet_ntoa(saSrc.sin_addr), 
  REQ_DATASIZE,
               dwElapsed,
               cTTL);
}
printf("\n");
nRet = closesocket(rawSocket);
if (nRet == SOCKET_ERROR)
ReportError("closesocket()");
}
int SendEchoRequest(SOCKET s,LPSOCKADDR_IN lpstToAddr) 
{
static ECHOREQUEST echoReq;
static nId = 1;
static nSeq = 1;
int nRet;
// 结构体成员赋值，填充echo请求
echoReq.icmpHdr.Type= ICMP_ECHOREQ;
echoReq.icmpHdr.Code= 0;
echoReq.icmpHdr.Checksum= 0;
echoReq.icmpHdr.ID= nId++;
echoReq.icmpHdr.Seq= nSeq++;
//填写数据
for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
echoReq.cData[nRet] = ' '+nRet;
//获取当前时间并记录
echoReq.dwTime= GetTickCount();
//计算校验和
echoReq.icmpHdr.Checksum = in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));
nRet = sendto(s,
(LPSTR)&echoReq,
sizeof(ECHOREQUEST),
0,
(LPSOCKADDR)lpstToAddr, 
sizeof(SOCKADDR_IN));   
if (nRet == SOCKET_ERROR) 
ReportError("sendto()");
return (nRet);
}
DWORD RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char *pTTL) 
{
ECHOREPLY echoReply;
int nRet;
int nAddrLen = sizeof(struct sockaddr_in);
nRet = recvfrom(s,
(LPSTR)&echoReply,
sizeof(ECHOREPLY),
0,
(LPSOCKADDR)lpsaFrom,
&nAddrLen);
if (nRet == SOCKET_ERROR) 
ReportError("recvfrom()");
*pTTL = echoReply.ipHdr.TTL;
return(echoReply.echoRequest.dwTime);   
}
void ReportError(LPCSTR pWhere)
{
fprintf(stderr,"\n%s error: %d\n",
WSAGetLastError());
}
int WaitForEchoReply(SOCKET s)
{
struct timeval Timeout;
fd_set readfds;
readfds.fd_count = 1;
readfds.fd_array[0] = s;
Timeout.tv_sec = 5;
    Timeout.tv_usec = 0;
return(select(1, &readfds, NULL, NULL, &Timeout));
}
//
// Mike Muuss' in_cksum() function
// and his comments from the original
// ping program
//
// * Author -
// *Mike Muuss
// *U. S. Army Ballistic Research Laboratory
// *December, 1983
/*
 *I N _ C K S U M
 *
 * Checksum routine for Internet Protocol family headers (C Version)
 *
 */
u_short in_cksum(u_short *addr, int len)
{
register int nleft = len;
register u_short *w = addr;
register u_short answer;
register int sum = 0;
/*
*  Our algorithm is simple, using a 32 bit accumulator (sum),
*  we add sequential 16 bit words to it, and at the end, fold
*  back all the carry bits from the top 16 bits into the lower
*  16 bits.
*/
while( nleft > 1 )  {
sum += *w++;
nleft -= 2;
}
/* mop up an odd byte, if necessary */
if( nleft == 1 ) {
u_short u = 0;
*(u_char *)(&u) = *(u_char *)w ;
sum += u;
}
/*
* add back carry outs from top 16 bits to low 16 bits
*/
sum = (sum >> 16) + (sum & 0xffff);/* add hi 16 to low 16 */
sum += (sum >> 16);/* add carry */
answer = ~sum;/* truncate to 16 bits */
return (answer);
}
第二个：使用原始套接字实现的数据包捕获（极简单的统计流量，嗅探器）
仅有一个函数：（伪代码）
int _tmain(int argc, _TCHAR* argv[])
{
WSADATA wsaData;
    SOCKET SnifferSocket = INVALID_SOCKET;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
struct hostent *local;
    char HostName[DEFAULT_NAMELEN];
struct in_addr addr;
struct sockaddr_in LocalAddr, RemoteAddr;
int addrlen = sizeof(struct sockaddr_in);
int in=0,i=0;
DWORD dwBufferLen[10];
    DWORD Optval= 1 ;
    DWORD dwBytesReturned = 0 ;
// 初始化套接字
//创建原始套接字
    SnifferSocket = socket ( AF_INET, SOCK_RAW, IPPROTO_IP);
//获取本机名称
iResult = gethostname( HostName, sizeof(HostName));
//获取本机可用IP
    local = gethostbyname( HostName);
printf ("\n本机可用的IP地址为：\n");
while (local->h_addr_list[i] != 0) {
        addr.s_addr = *(u_long *) local->h_addr_list[i++];
printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
    }
printf ("\n请选择捕获数据待使用的接口号：");
scanf_s( "%d", &in);
memset( &LocalAddr, 0, sizeof(LocalAddr));
memcpy( &LocalAddr.sin_addr.S_un.S_addr, local->h_addr_list[in-1], sizeof(LocalAddr.sin_addr.S_un.S_addr));
LocalAddr.sin_family = AF_INET;
LocalAddr.sin_port=0;
//绑定本地地址
iResult = bind( SnifferSocket, (struct sockaddr *) &LocalAddr, sizeof(LocalAddr));
//设置套接字接收命令
iResult = WSAIoctl(SnifferSocket, SIO_RCVALL , &Optval, sizeof(Optval),  &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned , NULL , NULL );
//开始接收数据
    printf(" \n开始接收数据");
do
{
//接收数据
iResult = recvfrom( SnifferSocket, recvbuf, DEFAULT_BUFLEN, 0 ,(struct sockaddr *)&RemoteAddr,&addrlen);
if (iResult > 0) 
printf ("\n接收到来自%s的数据包，长度为%d.",inet_ntoa(RemoteAddr.sin_addr),iResult );
else
printf("recvfrom failed with error: %ld\n", WSAGetLastError());
} while(iResult > 0);
return 0;
}
