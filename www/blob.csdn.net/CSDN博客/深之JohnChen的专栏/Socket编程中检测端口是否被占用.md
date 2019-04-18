# Socket编程中检测端口是否被占用 - 深之JohnChen的专栏 - CSDN博客

2017年08月04日 14:41:38[byxdaz](https://me.csdn.net/byxdaz)阅读数：4121


        一般检测一个端口是否被占用的方法是看bind是否成功，其实在Windows中有两个API可以获取到当前系统端口的占用情况（GetTcpTable/GetUdpTable），利用这两个函数就可检测端口占用情况。

但在较复杂系统环境下，端口占用表可能随时改变，可能存在两个以上进程使用上述方法检测到同一空闲端口，在这种情况下，可使用Bind方法进行端口选择。

一、通过GetTcpTable/GetUdpTable方法获取端口占用情况。

```cpp
#include <tcpmib.h>
#include <IPHlpApi.h>
//依赖lib库 Iphlpapi.lib Ws2_32.lib

//获取Tcp端口状态
BOOL GetTcpPortState(ULONG nPort, ULONG *nStateID)
{
	MIB_TCPTABLE TcpTable[100];
	DWORD nSize = sizeof(TcpTable);
	if(NO_ERROR == GetTcpTable(&TcpTable[0],&nSize,TRUE))
	{
		DWORD nCount = TcpTable[0].dwNumEntries;
		if (nCount > 0)
		{
			for(DWORD i=0;i<nCount;i++)
			{
				MIB_TCPROW TcpRow = TcpTable[0].table[i];
				DWORD temp1 = TcpRow.dwLocalPort;
				int temp2 = temp1 / 256 + (temp1 % 256) * 256;
				if(temp2 == nPort)
				{
					*nStateID = TcpRow.dwState;
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	return FALSE;
}

//获取Udp端口状态
BOOL GetUdpPortState(ULONG nPort, ULONG *nStateID)
{
	MIB_UDPTABLE UdpTable[100];
	DWORD nSize = sizeof(UdpTable);
	if(NO_ERROR == GetUdpTable(&UdpTable[0],&nSize,TRUE))
	{
		DWORD nCount = UdpTable[0].dwNumEntries;
		if (nCount > 0)
		{
			for(DWORD i=0;i<nCount;i++)
			{
				MIB_UDPROW TcpRow = UdpTable[0].table[i];
				DWORD temp1 = TcpRow.dwLocalPort;
				int temp2 = temp1 / 256 + (temp1 % 256) * 256;
				if(temp2 == nPort)
				{
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	return FALSE;
}
```

二、通过bind函数，判断端口是否被占用。

1、原始socket编程时，就使用bind函数判断。

```cpp
unsigned short usPort = 9090;
  SOCKET s = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(usPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(s,(LPSOCKADDR)&addr,sizeof(addr));
	if(WSAGetLastError()==WSAEADDRINUSE)
	{
		//端口已被占用
	}
```

2、使用boost asio库时，如何判断端口被占用，使用new socket或bind失败时来判断。

```cpp
boost::asio::io_service ioService；
unsigned short usPort = 9090;
udp::socket * pSockUdp = NULL;
bool bSockUdpError = false;
try{
		pSockUdp = new udp::socket(ioService,udp::endpoint(udp::v4(),usPort));
		bSockUdpError = false;
	}
	catch(...)
	{
		bSockUdpError = true;
	}
//释放
	if(!bSockUdpError && pSockUdp != NULL)
	{
		delete pSockUdp;
		pSockUdp = NULL;
	}
        ioService.stop();
```

```
boost::asio::io_service ioService;
	boost::asio::ip::tcp::socket * pSockTcp = NULL;
	bool bSockUseError = false;
	try {
		pSockTcp = new boost::asio::ip::tcp::socket(ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_usPort));
		bSockUseError = false;
	}
	catch (...)
	{
		bSockUseError = true;
	}
	//释放
	if (!bSockUseError && pSockTcp != NULL)
	{
		delete pSockTcp;
		pSockTcp = NULL;
	}
	ioService.stop();
```


