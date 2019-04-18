# boost库之udp server实例 - 深之JohnChen的专栏 - CSDN博客

2017年05月18日 13:56:55[byxdaz](https://me.csdn.net/byxdaz)阅读数：1755


//UdpLinkServer.h

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
typedef void (CALLBACK *SendDataCallback)(const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2);
//接收数据回调函数
typedef void (CALLBACK *RecvDataCallback)(const boost::system::error_code& error,char *pData,int nDataLength,char *pPeerIp,unsigned short usPeerPort,DWORD dwUserData1,DWORD dwUserData2);

class UdpLinkServer
{
public:
	UdpLinkServer(unsigned short usPort,bool bBroadcast);
	virtual ~UdpLinkServer(void);

	//发送数据回调函数 boost function定义
	typedef boost::function<void* (const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2)>	SendDataCallbackHandler;

	/*
	功能：设置接收数据回调函数
	参数：bAutoRecvData：是否自动接收数据；pfunc：接收数据回调函数；dwUserData1：接收数据回调函数用户数据1；dwUserData2：接收数据回调函数用户数据2
	返回值：无
	*/
	void SetRecvDataCallback(bool bAutoRecvData,RecvDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2);

	//开始
	int Start(boost::asio::io_service& ioService);

	//停止
	int Stop();

	//发送数据
	int SendDataEx(udp::endpoint endpointRemote,char *pBuffer,int nBufferSize,SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2);

	//发送数据
	int SendData(char *pBuffer,int nBufferSize,bool bAsync);

	//获取广播端点
	udp::endpoint & GetBroadcastEndPoint();

	//接收数据处理（手动）
	void handleRecvDataByManual(RecvDataCallback pfunc=NULL,DWORD dwUserData1=0,DWORD dwUserData2=0);
	//当发送数据给客户端成功之后响应处理
	void handleSendData(char *pBuffer,int nBufferSize,const boost::system::error_code& error,std::size_t bytes_transferred);
	void handleSendDataInner(SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2,const boost::system::error_code& error,std::size_t bytes_transferred);
	//void handleSendData(boost::shared_ptr<std::string> strMessage,const boost::system::error_code& error,std::size_t bytes_transferred);

	static void WINAPI SendDataCallbackOuter(const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2);

protected:
	//接收数据
	void RecvDataProcess(RecvDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2);
	//接收数据处理（自动）
	void handleRecvData(const boost::system::error_code& error,std::size_t bytes_transferred,RecvDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2);

	//是否停止服务
	bool IsStop();

private:
	udp::socket	*m_sockUdp;										//服务器的SOCKET
	udp::endpoint m_endpointRemote;								//收到数据时的端点信息
	udp::endpoint m_endpointBroadcast;							//广播端点信息
	boost::array<char,UDP_DATA_PACKAGE_MAX_LENGTH> m_recvBuf;	//接收数据缓冲区
	bool m_bStop;												//停止服务
	bool  m_bBroadcast;											//是否广播
	unsigned short m_usPort;									//端口
	bool m_bAutoRecvData;										//是否自动接收数据，true，表示自动接收数据；false，表示外部手动调用函数接收数据
	RecvDataCallback											m_pfuncRecvDataCallback;		//接收数据回调函数
	DWORD														m_dwRecvDataCallbackUserData1;	//接收数据回调函数用户数据1
	DWORD														m_dwRecvDataCallbackUserData2;  //接收数据回调函数用户数据2
};
```


//UdpLinkServer.cpp

```cpp
#include "StdAfx.h"
#include "UdpLinkServer.h"
#include <boost/exception/all.hpp>

UdpLinkServer::UdpLinkServer(unsigned short usPort,bool bBroadcast)
{
	m_bStop = false;
	m_bBroadcast = bBroadcast;
	m_usPort = usPort;
	m_sockUdp = NULL;
	m_bAutoRecvData = true;
	m_pfuncRecvDataCallback = NULL;
	m_dwRecvDataCallbackUserData1 = 0;
	m_dwRecvDataCallbackUserData2 = 0;
}

UdpLinkServer::~UdpLinkServer(void)
{
	if(m_sockUdp != NULL)
	{
		m_sockUdp->close();
		delete m_sockUdp;
		m_sockUdp = NULL;
	}
}

//设置接收数据回调函数
void UdpLinkServer::SetRecvDataCallback(bool bAutoRecvData,RecvDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2)
{
	m_bAutoRecvData = bAutoRecvData;
	m_pfuncRecvDataCallback = pfunc;
	m_dwRecvDataCallbackUserData1 = dwUserData1;
	m_dwRecvDataCallbackUserData2 = dwUserData2;
}

//开始
int UdpLinkServer::Start(boost::asio::io_service& ioService)
{
	try
	{
		if(m_bBroadcast)
		{
			//广播
			m_sockUdp = new udp::socket(ioService,udp::endpoint(udp::v4(),0));
			m_sockUdp->set_option(boost::asio::socket_base::reuse_address(true));
			m_sockUdp->set_option(boost::asio::socket_base::broadcast(true));
			m_endpointBroadcast = udp::endpoint(boost::asio::ip::address_v4::broadcast(), m_usPort);
		}
		else
		{
			m_sockUdp = new udp::socket(ioService,udp::endpoint(udp::v4(),m_usPort));
			if(!m_sockUdp->is_open())
			{
				//端口被占用
				return -1;
			}
			m_sockUdp->set_option(boost::asio::socket_base::reuse_address(true));
 //使用WSAIoctl设置UDP socket的工作模式，让其忽略这个错误(Windows UDP socket recvfrom返回10054错误的解决办法)
 BOOL bNewBehavior = FALSE;
 DWORD dwBytesReturned = 0;
 WSAIoctl(m_sockUdp->native(), SIO_UDP_CONNRESET, &bNewBehavior, sizeof bNewBehavior, NULL, 0, &dwBytesReturned, NULL, NULL);
 ////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	
	}
	catch (boost::exception& e)
	{
		return -1;
	}

    m_bStop = false;
	if(m_bAutoRecvData)
	{
		RecvDataProcess(m_pfuncRecvDataCallback,m_dwRecvDataCallbackUserData1,m_dwRecvDataCallbackUserData2);
	}
	return 0;
}

//停止
int UdpLinkServer::Stop()
{
	m_bStop = true;

	return 0;
}

//是否停止服务
bool UdpLinkServer::IsStop()
{
	return m_bStop;
}

//获取广播端点
udp::endpoint & UdpLinkServer::GetBroadcastEndPoint()
{
	return m_endpointBroadcast;
}

void UdpLinkServer::SendDataCallbackOuter(const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2)
{
	int i = 0;
}

//发送数据
int UdpLinkServer::SendDataEx(udp::endpoint endpointRemote,char *pBuffer,int nBufferSize,SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2)
{
	m_sockUdp->async_send_to(boost::asio::buffer(pBuffer,nBufferSize),endpointRemote,boost::bind(&UdpLinkServer::handleSendDataInner,this,pfunc,dwUserData1,dwUserData2,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));

	return 0;
}

//发送数据
int UdpLinkServer::SendData(char *pBuffer,int nBufferSize,bool bAsync)
{
	if(!m_bBroadcast)
	{
		if(bAsync)
		{
			//异步发送
			m_sockUdp->async_send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointRemote,boost::bind(&UdpLinkServer::handleSendData,this,pBuffer,nBufferSize,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			//同步发送
			m_sockUdp->send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointRemote);
		}
	}
	else
	{
		//广播
		if(bAsync)
		{
			//异步发送
			m_sockUdp->async_send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointBroadcast,boost::bind(&UdpLinkServer::handleSendData,this,pBuffer,nBufferSize,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			//同步发送
			m_sockUdp->send_to(boost::asio::buffer(pBuffer,nBufferSize),m_endpointBroadcast);
		}
	}

	return 0;
}

//接收数据
void UdpLinkServer::RecvDataProcess(RecvDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2)
{
	//异步接收数据
	m_sockUdp->async_receive_from(boost::asio::buffer(m_recvBuf),m_endpointRemote,boost::bind(&UdpLinkServer::handleRecvData,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,pfunc,dwUserData1,dwUserData2));
}

//接收数据处理（手动），就进入本函数响应处理
void UdpLinkServer::handleRecvDataByManual(RecvDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2)
{
	if(IsStop())
		return;

	//下一次接收
	RecvDataProcess(pfunc,dwUserData1,dwUserData2);
}

//当收到客户端数据时，就进入本函数响应处理  
void UdpLinkServer::handleRecvData(const boost::system::error_code& error,std::size_t bytes_transferred,RecvDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2)
{
	if(IsStop())
		return;

	//如果没有出错
	if(!error || error==boost::asio::error::message_size)
	{
		if(bytes_transferred > UDP_DATA_PACKAGE_MAX_LENGTH)
		{
			//超过最大数据长度
			return;
		}

		/*
		//打印接收的内容
		char szTmp[UDP_DATA_PACKAGE_MAX_LENGTH] = {0};
		memcpy(szTmp,m_recvBuf.data(),bytes_transferred);
		printf("%s\n",szTmp);

		//boost::shared_ptr<std::string> strMessage(new std::string("aaaaaa"));
		//SendData((char *)strMessage.data(),strMessage.size());
		std::string strMessage = "aaaaaaaaaa";
		SendDataEx(m_endpointRemote,(char *)strMessage.data(),strMessage.size(),SendDataCallbackOuter,(int)this,0);
		*/

		//回调数据
		if(pfunc != NULL)
		{
			pfunc(error,m_recvBuf.data(),bytes_transferred,(char *)m_endpointRemote.address().to_string().c_str(),m_endpointRemote.port(),dwUserData1,dwUserData2);
		}

		//下一次接收
		RecvDataProcess(pfunc,dwUserData1,dwUserData2);
	}
	else
	{
		//出错
		if(pfunc != NULL)
		{
			pfunc(error,NULL,bytes_transferred,NULL,0,dwUserData1,dwUserData2);
		}
	}
}

//当发送数据给客户端成功之后响应。  
void UdpLinkServer::handleSendData(char *pBuffer,int nBufferSize,const boost::system::error_code& error,std::size_t bytes_transferred)
{
	int n = 0;
}
void UdpLinkServer::handleSendDataInner(SendDataCallback pfunc,DWORD dwUserData1,DWORD dwUserData2,const boost::system::error_code& error,std::size_t bytes_transferred)
{
	if(error != NULL)
	{
		//打印错误信息
		printf("%s", boost::system::system_error(error).what());
	}
	if(pfunc != NULL)
	{
		pfunc(error,bytes_transferred,dwUserData1,dwUserData2);
	}
	int n = 0;
}
/*
void UdpLinkServer::handleSendData(boost::shared_ptr<std::string> strMessage,const boost::system::error_code& error,std::size_t bytes_transferred)
{
	 int n = 0;
}
*/
```


//调用实例代码

```cpp
// UdpServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UdpLinkServer.h"
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

int main(int argc, char* argv[])
{
	boost::asio::io_service ioService;
	UdpLinkServer usUdpService(7001,false);
	usUdpService.Start(ioService);

	/*
	for(int i=0; i<10; i++)
	{
		usUdpService.SendData("1111111111",10,true);
		Sleep(500);
	}
	*/

	
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

	usUdpService.Stop();
	ioService.stop();
	return 0;
}
```

如果遇到编译错误  error C2632: '__int64' followed by '__int64' is illegal
boost int64_t 与 vc中的int64_t冲突，将在文件头中增加#undef int64_t
如下：
#undef int64_t
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/function.hpp>


