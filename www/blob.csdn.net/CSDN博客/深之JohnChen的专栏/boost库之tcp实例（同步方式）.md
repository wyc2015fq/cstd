# boost库之tcp实例（同步方式） - 深之JohnChen的专栏 - CSDN博客

2017年05月22日 16:42:22[byxdaz](https://me.csdn.net/byxdaz)阅读数：753


//服务端

```cpp
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
using namespace boost::asio;

//链接客户端处理
void ConnectionClientProcess(ip::tcp::socket * sockClient)
{
	std::cout<<sockClient->remote_endpoint().address()<<std::endl; 
	//向客户端发送hello world! 
	boost::system::error_code ec;
	while(true)
	{
		sockClient->write_some(buffer("abcdefg"),ec);
		//如果出错，打印出错信息 
		if(ec) 
		{
			std::cout<<boost::system::system_error(ec).what()<<std::endl;
			break;
		}
		Sleep(200);
	}

}

int main(int argc, char* argv[])
{
	std::list<boost::thread *> listThreadClient;
	std::list<ip::tcp::socket *> listSocketClient;
	//io_service对象
	io_service iosev;
	ip::tcp::acceptor acceptor(iosev,ip::tcp::endpoint(ip::tcp::v4(),1000));
	for(;;)
	{ 
		//socket对象 
		ip::tcp::socket * socketClient = new ip::tcp::socket(iosev);
		//等待直到客户端连接进来
		acceptor.accept(*socketClient);
		//显示连接进来的客户端
		listSocketClient.push_back(socketClient);
		boost::thread * thr = new boost::thread(ConnectionClientProcess,socketClient);
		listThreadClient.push_back(thr);
		//与当前客户交互完成后循环继续等待下一客户连接
	}
	//等待线程都退出
	std::list<boost::thread *>::iterator iter = listThreadClient.begin(),iterEnd = listThreadClient.end();
	for(iter; iter!= iterEnd; iter++)
	{
		(*iter)->join();
	}
	std::list<ip::tcp::socket *>::iterator iterSock = listSocketClient.begin(),iterEndSock = listSocketClient.end();
	for(iterSock; iterSock!= iterEndSock; iterSock++)
	{
		(*iterSock)->close();
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
	//io_service对象
	io_service iosev;
	//socket对象
	ip::tcp::socket socket(iosev);
	//连接端点
	ip::tcp::endpoint ep(ip::address_v4::from_string("127.0.0.1"),1000);
	//连接服务器
	boost::system::error_code ec;
	socket.connect(ep,ec);
	//如果出错，打印出错信息
	if(ec)
	{
		std::cout<<boost::system::system_error(ec).what()<<std::endl;
		return -1;
	}
	//接收数据
	char buf[100] = {0};
	size_t len=socket.read_some(buffer(buf),ec);
	std::cout.write(buf,len);
	return 0;
}
```


