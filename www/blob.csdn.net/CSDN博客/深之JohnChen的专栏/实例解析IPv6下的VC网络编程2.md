# 实例解析IPv6下的VC网络编程2 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 14:20:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1732


**实例解析IPv6下的VC网络编程2**

实例篇

　　IPv6程序相对于IPv4改动并不大。其中主要的改动就是地址结构与地址解析函数。在RFC中详细说明了socket api 为适应IPv6所做的改动。而且windows平台与Linux平台在实现上也几乎是一样的。只不过头文件与支持程度等有所不同罢了（具体请参见RFC 2553与RFC 2292）。如读者有兴趣的话可以找RFC来看看，在这里就不再详细说明，只讲最简单的原理与例子，同时例出各主要socket api，如何使用，可以参考MSDN或是Linux中的MAN。

　　地址表示

　　IPv4使用32bits的地址表示，并有sockaddr_in和in_addr等结构应用于API中，而IPv6 使用128 bits 地址，也定义了本身的地址结构sockaddr_in6和in6_addr。

struct sockaddr_in {
u_char sin_len;
u_char sin_family;
u_short sin_port;
struct in_addr sin_addr;
char sin_zero[8];
};

struct in_addr {
u_int32_t s_addr;
}

struct sockaddr_in6 {
u_char sin6_len; 
u_char sin6_family; 
u_int16_t sin6_port; 
u_int32_t sin6_flowinfo; 
struct in6_addr sin6_addr;
u_int32_t sin6_scope_id;
}

struct in6_addr {
u_int8_t __u6_addr8[16];
}

　　IP V4/IP V6专用函数

　　IPv6 API中一部分沿用了IPv4 API, 也新增了一些IPv6专用API，为使得程序具有更大的通用性，尽量避免使用IPv4专用函数，这些函数如下：

IP V4专用 是对应的IP v4/v6通用函数 功能说明 
inet_aton( ) inet_ntop( ) 字符串地址转为IP地址 
inet_ntoa( ) inet_pton( ) IP地址转为字符串地址 
gethostbyname( ) Getipnodebyname( ) 由名字获得IP地 
gethostbyaddr( ) struct hostent *getipnodebyaddr( ) IP地址获得名字 
  getaddrinfo( ) 获得全部地址信息 
  getnameinfo( ) 获得全部名字信息 
未发生变化的函数 功能说明 
socket( ) 建立Socket 
bind( ) Socket与地址绑定 
send( ) 发送数据(TCP) 
sendto( ) 发送数据(UDP) 
receive( ) 接收数据(TCP) 
recv( )  接收数据(UDP) 
accept( ) 接收连接 
listen( ) 网络监听 

　　如上表格所示，IP V4专用函数在IP V6环境下已经不能使用，他们一般有一个对应的IP V4/V6通用函数，但是在使用通用函数的时候需要一个协议类型参数(AF_INET/AF_INET6)。另外还增加了两个功能强大的函数getaddrinfo( )和getnameinfo( )，几乎可以完成所有的地址和名字转化的功能。

　　Windows平台IPv6程序一例

　　下面我将给出windows平台下的一个例子。

　　系统平台：windows 2000 professional SP3+tpipv6-001205-SP3-IE6+VC6

　　说明：在本程序中只采用一个SOCKET，用WSAAsyncSelect来完成收与发的处理，为了方便起见，采用UDP方式。同时只专注IPv6的写法，不考虑函IPv4/IPv6兼容。如何编写IP无关的程序，有机会再另行撰文。
注：由于windows2000只是提供了IPv6的技术预览版，所以在有些地方支持上并不好。比如win2k在recvfrom的实现上好像有点问题，而linux中同样的代码却没问题。两者在recv的实现上都很正常。所以如果要开发程序最好是在window xp或是linux上进行。

　　下面是程序中关键的几步:

　　1.头文件的引入

#include 
#include 
#include "tpipv6.h" // Form IPv6 Tech Preview.(可以从tpipv6-001205-SP3-IE6安装后所在目录里找到)
#pragma comment(lib,"ws2_32")//引入库函数。

　2.建SOCKET UpdateData(TRUE);

memset(&hints,0,sizeof(hints));
//hints.ai_family=PF_UNSPEC;//这样可以由系统自己进行判断，这里我们直接指定是IPV6
hints.ai_family=AF_INET6;//指定用IPV6协议
hints.ai_socktype=SOCK_DGRAM;
hints.ai_protocol=IPPROTO_UDP;//用UDP
hints.ai_flags=AI_NUMERICHOST;//IP用数字表示
rc=getaddrinfo((LPCTSTR )m_send,"2000",&hints,&res);//解析地址
if(rc!=0)
{
AfxMessageBox("fail");
return;
}
s_send=socket(res->ai_family,res->ai_socktype,res->ai_protocol);//建socket
if(s_send==INVALID_SOCKET)
{
AfxMessageBox("建本机发socket失败");return ;}
else
{
AfxMessageBox("建本机发socket成功");
//绑定本地监听端口bind
if(bind (s_send,res->ai_addr,res->ai_addrlen)==SOCKET_ERROR)
AfxMessageBox("bind失败");
else
{AfxMessageBox("bind成功");GetDlgItem(IDOK)->EnableWindow (TRUE); }
}
if ( WSAAsyncSelect( s_send, m_hWnd, UM_PACKET, FD_READ ) == SOCKET_ERROR )//异步选择
{
MessageBox( "WSAAsyncSelect failed" ); 
closesocket( s_send );
return;
} 

　　3.发送 memset(&hints,0,sizeof(hints));

hints.ai_family=AF_INET6;
hints.ai_socktype=SOCK_DGRAM;
hints.ai_protocol=IPPROTO_UDP;//这里也可以改为0，让系统自己选
hints.ai_flags=AI_NUMERICHOST;
rc=getaddrinfo((LPCTSTR )m_receive,"2000",&hints,&res);//解析对方收地址
if(rc!=0)
{
AfxMessageBox("fail");
return;
}
rc=sendto(s_send,m_sendtxt,m_sendtxt.GetLength(),0,
res->ai_addr,res->ai_addrlen);
if (rc== SOCKET_ERROR)
{
AfxMessageBox("发送失败");
closesocket(s_send);
}
else
{
AfxMessageBox("发送成功");
} 

　　4.异步触发事件 LRESULT CMy6dDlg::OnPacket( WPARAM wParam, LPARAM lParam )

{
char sBuf[128];
sockaddr_in6 sa;
int iAddrLen=sizeof( sa ); 
int a=sizeof(sockaddr);
ZeroMemory( &sa, sizeof( sa ) );
memset(&sBuf,0,sizeof(sBuf));
int iLen = recv(s_send, sBuf, sizeof(sBuf),0);//for win2k use recv
if(iLen<0)
{
fprintf(stderr,"recvfrom failed with error %d: %s/n",)WSAGetLastError(), DecodeError(WSAGetLastError()));
return 0;
}
else
{
sBuf[iLen] = 0;
CString sText;
char lpszAddressString[1000]; 
DWORD dwAddressStringLength=1000;
rc=WSAAddressToString((sockaddr*)&sa,sizeof(sa),NULL,
lpszAddressString,&dwAddressStringLength );
sText.Format( "recved [%s]", sBuf);//,sa.sin6_family==AF_INET6,
lpszAddressString);//for win2k
m_reclist.AddString( sText );
return 0;
}
} 


