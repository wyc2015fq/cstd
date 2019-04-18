# boost库之udp client实例 - 深之JohnChen的专栏 - CSDN博客

2017年05月18日 14:00:22[byxdaz](https://me.csdn.net/byxdaz)阅读数：1533


//UdpLinkClient.h

```cpp
//udp服务

#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
using boost::asio::ip::udp;

#define  UDP_DATA_PACKAGE_MAX_LENGTH		1024

//发送数据回调函数
typedef void (CALLBACK *SendDataCallback)(const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2);//设置工作参数

class UdpLinkClient
{
public:
	UdpLinkClient(char *pUrl,char *pServiceName,unsigned short usPort);
	virtual ~UdpLinkClient(void);

	//

	//-------------------------------------------------------------------------------------
	// 功能：发送数据处理函数
	// 参数：nReserved1,nReserved2:保留
	// 返回：NULL:创建失败		其他:对象地址
	//-------------------------------------------------------------------------------------
	typedef boost::function<void* (const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2)>	SendDataCallbackHandler;

	//开始
	int Start(boost::asio::io_service& ioService);

	//停止
	int Stop();

	//发送数据
	int SendDataEx(udp::endpoint endpointRemote,char *pBuffer,int nBufferSize,SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2);

	//发送数据
	int SendData(char *pBuffer,int nBufferSize,bool bAsync);

	//启用接收数据服务(自动)
	int AutoRecvData();

	//启用接收数据服务(人工)
	int MmanualRecvData();

	//获取服务器端点
	udp::endpoint & GetServerEndPoint();

	//当收到对方端数据时，就进入本函数响应处理
	void handleRecvData(const boost::system::error_code& error,std::size_t bytes_transferred);
	//当发送数据给对方端成功之后响应处理
	void handleSendData(char *pBuffer,int nBufferSize,const boost::system::error_code& error,std::size_t bytes_transferred);
	void handleSendDataInner(SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2,const boost::system::error_code& error,std::size_t bytes_transferred);
	//void handleSendData(boost::shared_ptr<std::string> strMessage,const boost::system::error_code& error,std::size_t bytes_transferred);

	static void WINAPI SendDataCallbackOuter(const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2);

protected:
	//接收数据
	void RecvDataProcess();

	//是否停止服务
	bool IsStop();

private:
	udp::socket	*m_sockUdp;										//服务器的SOCKET
	udp::endpoint m_endpointRemote;								//收到数据时的端点信息
	udp::endpoint m_endpointServer;								//服务器的端点信息
	boost::array<char,UDP_DATA_PACKAGE_MAX_LENGTH> m_recvBuf;	//接收数据缓冲区
	bool m_bStop;												//停止服务
	char m_szUrl[256];											//服务器url
	char m_szServiceName[32];									//服务名，比如daytime，http
	unsigned short m_usPort;									//端口
	bool m_bReviceData;											//是否接收过数据	
	bool m_bReviceServerUse;									//是否启用接收服务
};
```

//UdpLinkClient.cpp

```cpp
#include "StdAfx.h"
#include "UdpLinkClient.h"
#include <boost/exception/all.hpp>

UdpLinkClient::UdpLinkClient(char *pUrl,char *pServiceName,unsigned short usPort)
{
	m_bStop = false;
	m_usPort = usPort;
	m_sockUdp = NULL;
	memset(m_szUrl,0,sizeof(m_szUrl));
	memset(m_szServiceName,0,sizeof(m_szServiceName));
	if(pUrl != NULL)
	{
		strcpy(m_szUrl,pUrl);
	}
	if(pServiceName != NULL)
	{
		strcpy(m_szServiceName,pServiceName);
	}
	m_bReviceData = false;
	m_bReviceServerUse = false;
}

UdpLinkClient::~UdpLinkClient(void)
{
	if(m_sockUdp != NULL)
	{
		m_sockUdp->close();
		delete m_sockUdp;
		m_sockUdp = NULL;
	}
}

//开始
int UdpLinkClient::Start(boost::asio::io_service& ioService)
{
	try
	{
		if(strlen(m_szServiceName) > 0)
		{
			//查询连接的IP域名和端口
			udp::resolver resolver(ioService);
			//udp::resolver::query query("www.google.com","http");
			udp::resolver::query query(m_szUrl,m_szServiceName);
			m_endpointServer = *resolver.resolve(query);
		}
		else
		{
			m_endpointServer = udp::endpoint(boost::asio::ip::address_v4::from_string(m_szUrl), m_usPort);
		}
		m_sockUdp = new udp::socket(ioService);
		m_sockUdp->open(udp::v4());
 m_sockUdp->set_option(boost::asio::socket_base::reuse_address(true));
 //使用WSAIoctl设置UDP socket的工作模式，让其忽略这个错误(Windows UDP socket recvfrom返回10054错误的解决办法)
 BOOL bNewBehavior = FALSE;
 DWORD dwBytesReturned = 0;
 WSAIoctl(m_sockUdp->native(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	catch (boost::exception& e)
	{
		//diagnostic_information(e);
		return -1;
	}

    m_bStop = false;
	m_bReviceData = false;
	m_bReviceServerUse = false;
	return 0;
}

//停止
int UdpLinkClient::Stop()
{
	m_bStop = true;

	return 0;
}

//是否停止服务
bool UdpLinkClient::IsStop()
{
	return m_bStop;
}

//获取服务器端点
udp::endpoint & UdpLinkClient::GetServerEndPoint()
{
	return m_endpointServer;
}

void UdpLinkClient::SendDataCallbackOuter(const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2)
{
	int i = 0;
}

//发送数据
int UdpLinkClient::SendDataEx(udp::endpoint endpointRemote,char *pBuffer,int nBufferSize,SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2)
{
	m_sockUdp->async_send_to(boost::asio::buffer(pBuffer,nBufferSize),endpointRemote,boost::bind(&UdpLinkClient::handleSendDataInner,this,pfunc,dwUserData1,dwUserData2,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));

	return 0;
}

//发送数据
int UdpLinkClient::SendData(char *pBuffer,int nBufferSize,bool bAsync)
{
	if(!m_bReviceData)
	{
		if(bAsync)
		{
			//异步发送
			m_sockUdp->async_send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointServer,boost::bind(&UdpLinkClient::handleSendData,this,pBuffer,nBufferSize,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			//同步发送
			std::size_t nSize = m_sockUdp->send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointServer);
			if(nSize > 0)
			{
				AutoRecvData();
			}
		}
	}
	else
	{
		//更新了对方端点信息
		if(bAsync)
		{
			//异步发送
			m_sockUdp->async_send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointRemote,boost::bind(&UdpLinkClient::handleSendData,this,pBuffer,nBufferSize,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			//同步发送
			std::size_t nSize = m_sockUdp->send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointRemote);
			if(nSize > 0)
			{
				AutoRecvData();
			}
		}
	}

	return 0;
}

//启用接收数据服务(自动)
int UdpLinkClient::AutoRecvData()
{
	if(!m_bReviceServerUse)
	{
		RecvDataProcess();
		m_bReviceServerUse = true;
	}

	return 0;
}

//启用接收数据服务(人工)
int UdpLinkClient::MmanualRecvData()
{
	RecvDataProcess();

	return 0;
}

//接收数据
void UdpLinkClient::RecvDataProcess()
{
	//异步接收数据
	m_sockUdp->async_receive_from(boost::asio::buffer(m_recvBuf),m_endpointRemote,boost::bind(&UdpLinkClient::handleRecvData,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
}

//当收到客户端数据时，就进入本函数响应处理  
void UdpLinkClient::handleRecvData(const boost::system::error_code& error,std::size_t bytes_transferred)
{
	if(IsStop())
		return;

	//如果没有出错
	if(!error || error==boost::asio::error::message_size)
	{
		if(!m_bReviceData)
		{
			m_bReviceData = true;
		}

		if(bytes_transferred > UDP_DATA_PACKAGE_MAX_LENGTH)
		{
			//超过最大数据长度
			return;
		}

		//打印接收的内容
		char szTmp[UDP_DATA_PACKAGE_MAX_LENGTH] = {0};
		memcpy(szTmp,m_recvBuf.data(),bytes_transferred);
		printf("%s\n",szTmp);

		//boost::shared_ptr<std::string> strMessage(new std::string("aaaaaa"));
		std::string strMessage = "bbbbbbbbb";
		//SendData((char *)strMessage.data(),strMessage.size());
		SendDataEx(m_endpointRemote,(char *)strMessage.data(),strMessage.size(),SendDataCallbackOuter,(int)this,0);

		//下一次接收
		RecvDataProcess();
	}
}

//当发送数据给客户端成功之后响应。  
void UdpLinkClient::handleSendData(char *pBuffer,int nBufferSize,const boost::system::error_code& error,std::size_t bytes_transferred)
{
	if(error != NULL)
	{
		//打印错误信息
		printf("%s", boost::system::system_error(error).what());
		return;
	}
	AutoRecvData();

	int n = 0;
}
void UdpLinkClient::handleSendDataInner(SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2,const boost::system::error_code& error,std::size_t bytes_transferred)
{
	if(error != NULL)
	{
		//打印错误信息
		printf("%s", boost::system::system_error(error).what());
		return;
	}
	if(pfunc != NULL)
	{
		pfunc(error,bytes_transferred,dwUserData1,dwUserData2);
	}
	int n = 0;
}
/*
void UdpLinkClient::handleSendData(boost::shared_ptr<std::string> strMessage,const boost::system::error_code& error,std::size_t bytes_transferred)
{
	 int n = 0;
}
*/
```

//调用实例代码

```cpp
// UdpClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UdpLinkClient.h"
#include <boost/thread/thread.hpp>

//#define  THREAD_RUN		1

//工作线程函数处理函数
bool g_WorkThreadExit = false;
unsigned int __stdcall WorkThreadFunByDeviceServiceProcess(PVOID pParam)
{
	boost::asio::io_service *pIoService = (boost::asio::io_service *)pParam;
	while(true)
	{
		if(g_WorkThreadExit)
		{
			break;
		}

		pIoService->poll();
		Sleep(100);
	}
	return 0;
}

int main(int argc, char * argv[])
{
	boost::asio::io_service ioService;
	UdpLinkClient usUdpClient("127.0.0.1",NULL,7001);
	int nRet = usUdpClient.Start(ioService);
	if(nRet != 0)
	{
		return 0;
	}

	//发送数据
	nRet = usUdpClient.SendData("abcdedfg",8,true);
	
#ifdef THREAD_RUN
	//线程poll等待
	boost::thread thrd(WorkThreadFunByDeviceServiceProcess,&ioService);
	thrd.join();
	g_WorkThreadExit = true;
#else
	//阻塞等待，否则就直接退出了程序，io_service无法消息循环处理
	//io_service run也可以不调用，但该进程不能直接退出，需要阻塞。
	ioService.run();
#endif

	usUdpClient.Stop();
	ioService.stop();
	return 0;
}
```


