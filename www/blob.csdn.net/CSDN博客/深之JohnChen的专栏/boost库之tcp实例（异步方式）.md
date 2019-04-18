# boost库之tcp实例（异步方式） - 深之JohnChen的专栏 - CSDN博客

2017年05月24日 14:25:33[byxdaz](https://me.csdn.net/byxdaz)阅读数：2133


//服务端

```cpp
#define	TCP_RECV_DATA_PACKAGE_MAX_LENGTH			2048
#define	TCP_SEND_DATA_PACKAGE_MAX_LENGTH			2048

#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::asio;

std::string make_daytime_string()
{
	using namespace std;
	time_t now = std::time(NULL);
	return ctime(&now);
}

//发生数据回调函数
typedef void (CALLBACK *SendDataCallback)(const boost::system::error_code& error,std::size_t bytes_transferred,DWORD dwUserData1,DWORD dwUserData2);

//接收数据回调函数
typedef void (CALLBACK *RecvDataCallback)(const boost::system::error_code& error,char *pData,int nDataSize,DWORD dwUserData1,DWORD dwUserData2);

//tcp connection
class TcpConnection
{
public:
	static TcpConnection * create(io_service& ioService)
	{
		return new TcpConnection(ioService);
	}

	virtual ~TcpConnection()
	{
		m_bDisconnect = true;
		m_socket.close();
	}

	ip::tcp::socket& socket()
	{
		return m_socket;
	}

	//发送数据
	int sendData(char *pData,int nDataSize,SendDataCallback fnCallback,DWORD dwUserData1,DWORD dwUserData2)
	{
		if(fnCallback == NULL)
		{
			//同步
			if(!m_socket.is_open())
			{
				return 0;
			}
			std::size_t  nSendedSize = boost::asio::write(m_socket,boost::asio::buffer(pData,nDataSize));
			if(nDataSize == nSendedSize)
			{
				return 0;
			}
			else
			{
				return nSendedSize;
			}
		}
		else
		{
			//异步
			if(!m_socket.is_open())
			{
				return 0;
			}
			memcpy(m_sendBuf.data(),pData,nDataSize);
			boost::asio::async_write(
				m_socket, 
				boost::asio::buffer(m_sendBuf.data(),nDataSize), 
				boost::bind(&TcpConnection::handle_write, this,
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred,
				fnCallback,dwUserData1,dwUserData2));
		}
	
		return 0;
	}
	//接收数据（同步）
	int	recvDataBySync()
	{
		if(!m_socket.is_open())
		{
			return 0;
		}
		boost::system::error_code error;
		std::size_t nSize = m_socket.read_some(boost::asio::buffer(m_recvBuf),error);
		if(error != NULL)
		{
			//错误
			return 0;
		}

		return nSize;
	}
	//接收数据（异步）
	int	recvDataByAsync(RecvDataCallback  fnCallback,DWORD dwUserData1,DWORD dwUserData2)
	{
		m_socket.async_read_some(boost::asio::buffer(m_recvBuf),
			boost::bind(&TcpConnection::handle_read, this, 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred,
			fnCallback,dwUserData1,dwUserData2));

		return 0;
	}

private:
	TcpConnection(io_service& ioService)
		: m_socket(ioService)
	{
		m_bDisconnect = false;
	}

	void handle_write(const boost::system::error_code& error,size_t bytes_transferred,SendDataCallback fnCallback,DWORD dwUserData1,DWORD dwUserData2)
	{
		if(fnCallback != NULL)
		{
			fnCallback(error,bytes_transferred,dwUserData1,dwUserData2);
		}
		if(error != NULL)
		{
			m_bDisconnect = true;
			if(m_socket.is_open())
			{
				m_socket.close();
			}
			if(!m_bDisconnect)
			{
				printf("close connect \n");
			}

			//发送数据失败
			return;
		}

		printf("write data!!!");
	}

	void handle_read(const boost::system::error_code& error,size_t bytes_transferred,RecvDataCallback  fnCallback,DWORD dwUserData1,DWORD dwUserData2)
	{
		if(fnCallback != NULL)
		{
			fnCallback(error,m_recvBuf.data(),bytes_transferred,dwUserData1,dwUserData2);
		}
if(error != NULL)
{
		if (error == boost::asio::error::eof || error == boost::asio::error::connection_reset)
		{
			//boost::asio::error::eof  --对端方关闭连接（正常关闭套接字）
			//boost::asio::error::connection_reset --对端方关闭连接（暴力关闭套接字）
			//对端方关闭连接
			if(m_socket.is_open())
			{
				m_socket.close();
			}
			printf("close connect \n");
                }
else
{
			if(m_socket.is_open())
			{
				m_socket.close();
			}
}
return;
}
		char szMsg[128] = {0};
		memcpy(szMsg,m_recvBuf.data(),bytes_transferred);
		printf("%s \n",szMsg);
	}

	bool	m_bDisconnect;											//是否断开连接
	ip::tcp::socket m_socket;
	boost::array<char,TCP_RECV_DATA_PACKAGE_MAX_LENGTH> m_recvBuf;	//接收数据缓冲区
	boost::array<char,TCP_SEND_DATA_PACKAGE_MAX_LENGTH> m_sendBuf;	//发送数据缓冲区
};

class TcpServer
{
public:
	TcpServer(io_service& ioService) : m_acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 10005))
	{
		m_funcConnectionHandler = NULL;
		m_nUserData = 0;
		m_bStop = false;
	}

	//新建连接回调函数
	typedef boost::function<void (TcpConnection * new_connection,int nUserData)>	CreateConnnectionCallbackHandler;

	//设置新建连接回调函数
	void setNewConnectionCallback(CreateConnnectionCallbackHandler fnHandler,int nUserData)
	{
		m_funcConnectionHandler = fnHandler;
		m_nUserData = nUserData;
	}

	//开始工作
	void startWork()
	{
		m_bStop = false;
		start_accept();
	}

	//停止工作
	void stopWork()
	{
		m_bStop = true;
		m_acceptor.close();
	}

private:
	void start_accept()
	{
		if(m_bStop)
		{
			return;
		}

		TcpConnection *new_connection = TcpConnection::create(m_acceptor.get_io_service());

		m_acceptor.async_accept(
			new_connection->socket(), 
			boost::bind(&TcpServer::handle_accept, 
			this, 
			new_connection, 
			boost::asio::placeholders::error));
	}

	void handle_accept(TcpConnection * new_connection,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			if(m_funcConnectionHandler != NULL)
			{
				m_funcConnectionHandler(new_connection,m_nUserData);
			}

			new_connection->sendData("abcdefg",strlen("abcdefg"),NULL,0,0);
			start_accept();
		}
	}

	ip::tcp::acceptor m_acceptor;
	CreateConnnectionCallbackHandler m_funcConnectionHandler;
	int				  m_nUserData;
	bool			  m_bStop;
};

//所有连接
std::list<TcpConnection *> g_listConnection;
//无效连接
std::list<TcpConnection *> g_listNoEffectConnection;

//插入无效连接
void InsertNoEffectConnection(TcpConnection * pConnnection)
{
	bool bFind = false;
	TcpConnection * pTcpConnectionTmpObject = NULL;
	std::list<TcpConnection *>::iterator iter,iterEnd;
	iter = g_listNoEffectConnection.begin();
	iterEnd = g_listNoEffectConnection.end();
	for(iter; iter!=iterEnd; iter++)
	{
		pTcpConnectionTmpObject = *iter;
		if(pTcpConnectionTmpObject == pConnnection)
		{
			bFind = true;
			break;
		}
	}

	//未找到，插入
	if(!bFind)
	{
		g_listNoEffectConnection.push_back(pTcpConnectionTmpObject);
	}
}

//删除无效连接
void DeleteNoEffectConnection()
{
	std::list<TcpConnection *>::iterator iter;
	if(g_listNoEffectConnection.size() > 0)
	{
		TcpConnection * pTcpConnectionTmpObject = NULL;
		iter = g_listNoEffectConnection.begin();
		while(iter != g_listNoEffectConnection.end())
		{
			pTcpConnectionTmpObject = *iter;
			if(pTcpConnectionTmpObject != NULL)
			{
				g_listConnection.remove(pTcpConnectionTmpObject);
				delete pTcpConnectionTmpObject;
			}
			iter++;
		}

		g_listNoEffectConnection.clear();
	}
}

//新连接回调处理
void NewConnectionCallbackProcess(TcpConnection * new_connection,int nUserData)
{
	g_listConnection.push_back(new_connection);
}

void WINAPI RecvDataCallbackProcess(const boost::system::error_code& error,char *pData,int nDataSize,DWORD dwUserData1,DWORD dwUserData2)
{
	if (error == boost::asio::error::eof)
	{
		//对端方关闭连接
		TcpConnection * pTcpConnectionTmpObject = (TcpConnection *)dwUserData1;
		if(pTcpConnectionTmpObject != NULL)
		{
			InsertNoEffectConnection(pTcpConnectionTmpObject);
		}
		return;
	}
}

int main(int argc, char* argv[])
{
	try
	{
		io_service ioService;
		TcpServer server(ioService);
		server.setNewConnectionCallback(NewConnectionCallbackProcess,0);
		server.startWork();

		TcpConnection * pTcpConnectionObject = NULL;
		std::list<TcpConnection *>::iterator iter,iterEnd;
		
		int n = 0;
		while(true)
		{
			//删除无效连接
			DeleteNoEffectConnection();

			//遍历
			iter = g_listConnection.begin();
			iterEnd = g_listConnection.end();
			for(iter; iter!=iterEnd; iter++)
			{
				pTcpConnectionObject = *iter;
				pTcpConnectionObject->sendData("111",3,NULL,0,0);
				pTcpConnectionObject->recvDataByAsync(RecvDataCallbackProcess,(int)pTcpConnectionObject,0);
			}

			ioService.poll();
			Sleep(200);
			n++;
			if(n > 1000)
			{
				break;
			}
		}
		
		// 只有io_service类的run()方法运行之后回调对象才会被调用
		//ioService.run();
		//释放空间
		iter = g_listConnection.begin();
		iterEnd = g_listConnection.end();
		for(iter; iter!=iterEnd; iter++)
		{
			pTcpConnectionObject = *iter;
			if(pTcpConnectionObject != NULL)
			{
				delete pTcpConnectionObject;
			}
		}
		g_listConnection.clear();
		server.stopWork();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
```


//客户端

```cpp
#include <iostream>
#include <boost/asio.hpp>
using namespace boost::asio;

int main(int argc, char* argv[])
{
	io_service ioService;
	boost::system::error_code error;
	try
	{
		//获取ip（用解释器的方法来解析域名）
		/*
		ip::tcp::resolver resolver(ioService);
		ip::tcp::resolver::query query("www.yahoo.com", "80");
		ip::tcp::resolver::iterator iter = resolver.resolve( query);
		ip::tcp::endpoint ep = *iter;
		std::cout << ep.address().to_string() << std::endl;
		*/

		ip::tcp::socket socket(ioService);
		ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1"), 10005);
		socket.connect(endpoint, error);

		//是否出错
		if (error)
		{
			throw boost::system::system_error(error);
		}

		while(true)
		{
			boost::array<char,128> buf;
			size_t len = socket.read_some(boost::asio::buffer(buf), error);
			//服务端运行断开.
                        if(error != NULL)
{
			if (error == boost::asio::error::eof) 
			{
				break; // 对端方关闭连接（正常关闭套接字）
			}
			else if (error == boost::asio::error::connection_reset)
			{
break;//对端方关闭连接（暴力关闭套接字）
							
}
                        else
{
throw boost::system::system_error(error); // Some other error.

}
}

			char szMsg[128] = {0};
			memcpy(szMsg,buf.data(),len);
			printf("%s\n",szMsg);

			//发送数据
			socket.write_some(boost::asio::buffer(buf, len), error);
		}
	}
	catch (std::exception& e)
	{
		printf(e.what());
	}

	return 0;
}
```



