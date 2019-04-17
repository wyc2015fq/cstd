# POCO库中文编程参考指南（8）丰富的Socket编程 - DoubleLi - 博客园






# POCO库中文编程参考指南（8）丰富的Socket编程
- 作者：柳大·Poechant
- 博客：[Blog.CSDN.net/Poechant](http://blog.csdn.net/Poechant)
- 邮箱：zhongchao.ustc#gmail.com (# -> @)
- 日期：April 16th, 2012

## 1 POCO 中的 Socket

POCO 中有 丰富的 Socket 封装。其继承关系如下：

![Resize icon](http://img.my.csdn.net/uploads/201204/16/1334590569_9639.png)

本文暂且只介绍 StreamSocket、ServerSocket、DatagramSocket

## 2 Poco::Net::ServerSocket

ServerSocket 是一个封装层次比较低（low level）的 Socket，其使用的是 TCP 连接。在实际的 Server 中推荐使用 TCPServer 或 Reactor 框架。下面是例程：

```
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include <iostream>

int main(int argc, char **argv)
{
```

绑定端口，并开始监听：

```
Poco::Net::ServerSocket srv(12345);
```

服务主循环：

```
while (true)
    {
```

接受连接：

```
Poco::Net::StreamSocket streamSocket = srv.acceptConnection();
```

向 Socket 发送数据：

```
Poco::Net::SocketStream socketStream(streamSocket);
        socketStream << "HTTP/1.0 200 OK\r\n"
                     << "Content-Type: text/html\r\n"
                     << "\r\n"
                     << "<html><head><title>My 1st Web Server</title></head></html>"
                     << std::flush;
    }
    return 0;
}
```

## 3 TCPServer 框架

POCO 提供了一个 TCPServer，顾名思义，是一个 TCP 服务器。准确的说，TCPServer 是一个 Framework，要求使用 ServerSocket 去 accept 连接。并且在将 ServerSocket 转递给 TCPServer 之前必须将其设置为监听模式。

TCPServer 维护一个连接队列（connection queue）。TCPServer 开启多个线程去从连接队列中取连接并进行处理，线程的数量是动态的，与连接队列中的连接数有关。

非正常连接会被立即关闭，而不会被插入连接队列。TCPServer 的主线程负责将客户端发来的请求连接放入连接队列。

TCPServer 是一个高效的开发框框架，具体详见[《POCO库中文编程参考指南（10）如何使用TCPServer框架？》](http://blog.csdn.net/poechant/article/details/7484781)一文。

## 4 Poco::Net::DatagramSocket

### 4.1 UDP Client

```
#include "Poco/Net/DatagramSocket.h"  
#include "Poco/Net/SocketAddress.h"  
#include "Poco/Timestamp.h"  
#include "Poco/DateTimeFormatter.h"  
#include <string>

int main()
{
    const char* ipaddr = "127.0.0.1";
    Poco::Net::SocketAddress sa("127.0.0.1", 1234);  
    Poco::Net::DatagramSocket dgs;
    dgs.connect(sa); 

    std::string syslogMsg;  
    Poco::Timestamp now;  
    syslogMsg = Poco::DateTimeFormatter::format(now, "<14>%w %f %H:%M:%S Hello,world!");  
    dgs.sendBytes(syslogMsg.data(), syslogMsg.size());  

    return 0;  
}
```

不能用：

```
...
const char* ipaddr = "127.0.0.1";
Poco::Net::SocketAddress sa("127.0.0.1", 1234);  
Poco::Net::DatagramSocket dgs(sa);

std::string syslogMsg;  
Poco::Timestamp now; 
...
```

因为 DatagramSocket(SocketAddress) 构造函数是创建一个 DatagramSocket 然后 bind()。而这里要使用的是 connect()。

### 4.2 UDP Server

```
#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/IPAddress.h"
#include <iostream>

int main(int argc, char **argv)
{
    Poco::Net::SocketAddress socketAddress(Poco::Net::IPAddress(), 1234);
    Poco::Net::DatagramSocket datagramSocket(socketAddress);

    char buffer[1024];

    while (true)
    {
        Poco::Net::SocketAddress sender;
        int n = datagramSocket.receiveFrom(buffer, sizeof(buffer) - 1, sender);
        buffer[n] = '\0';
        std::cout << sender.toString() << ": " << buffer << std::endl;
    }

    return 0;
}
```

## 5 Reactor 框架

Reactor 框架是利用 Event/Notification 实现的基于 Reactor 设计模式的一个服务器框架，由于 Event/Notification 存在而支持异步。

具体详见本博的另一篇博文[《POCO库中文编程参考指南（11）如何使用Reactor框架？》](http://blog.csdn.net/poechant/article/details/7485760)

## 6 StreamSocket

也是一个 TCP 的 Socket，可以使数据传输操作变的简化，被用在 TCP 服务器和客户端。

-



from:http://blog.csdn.net/Poechant/article/details/7467971









