# POCO库中文编程参考指南（10）如何使用TCPServer框架？ - DoubleLi - 博客园






## 1 TCPServer 框架概述

POCO 库提供`TCPServer`框架，用以搭建自定义的 TCP 服务器。`TCPServer`维护一个连接队列、一个连接线程池。连接线程用于处理连接，连接线程只要一空闲就不断地从连接队列中取连接并进行处理。一旦连接线程从连接队列中取到一个连接，就会创建一个`TCPServerConnection`连接对象，并且调用该对象的`start()`方法，直到`start()`方法返回，这个连接对象就被删除了。

连接线程的数量是动态的，其取决于连接队列中排队的连接数。当然，你使用的时候可以设定连接队列的最大容量，以防止在高并发应用的服务器上出现连接太多而使连接队列溢出的悲剧发生。当连接队列满了，却还有新的连接到来时，新来的连接就会被立即悄无声息地关闭。

现在我们总结一下，就是要有一个可运行的 TCP 服务应用程序（命名为`PoechantTCPServer`），还有很多 TCP 连接（命名为`PoechantTCPConnection`）。而这里我们用到工厂模式（准确说是`TCPServerConnectionFactory`要我们用的），所以还有一个 `PoechantTCPConnectionFactory`。

## 2 光说不练假把式

### 2.1 创建一个 PoechantTCPServer

或许你还不熟悉 POCO 中的 Application，没关系，这不影响本文的叙述。下面先创建一个 ServerApplication 如下：

```
// PoechantTCPServer.h

#ifndef POECHANT_TCP_SERVER
#define POECHANT_TCP_SERVER

#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Application.h"

using Poco::Util::ServerApplication;
using Poco::Util::Application;

class PoechantTCPServer: public ServerApplication
{
public:
    PoechantTCPServer() {}
    ~PoechantTCPServer() {}
protected:
    void initialize(Application& self);
    void uninitialize();
    int main(const std::vector<std::string>& args)
};

#endif
```

这样在调用启动`PoechantTCPServer`时，会先调用`initialize`，然后调用`main`，在`main`结束后会调用`uninitialize`。其实现很简单：

```
// PoechantTCPServer.cpp

#include "PoechantTCPServer.h"

void PoechantTCPServer::initialize(Application& self)
{
    ServerApplication::loadConfiguration();
    ServerApplication::initialize(self);
}

void PoechantTCPServer::uninitialize()
{
    ServerApplication::uninitialize();
}

int PoechantTCPServer::main(const std::vector<std::string>& args)
{
    // 这个咱最后说

    return Application::EXIT_OK;
}
```

### 2.2 PoechantTCPConnection

连接类的定义很简单，构造函数要传入一个 StreamSocket 和其他你需要的参数。

```
// PoechantTCPConnection.h

#ifndef POECHANT_TCP_CONNECTION_H
#define POECHANT_TCP_CONNECTION_H

#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include <string>

class PoechantTCPConnection: public TCPServerConnection
{
public:
    PoechantTCPConnection(const StreamSocket& s,
        const std::string& arg1,
        int arg2,
        double arg3);

    void run();
private:

    std::string _arg1;
    int _arg2;
    double _arg3;
};

#endif
```

实现如下：

```
// PoechantTCPConnection.cpp

#include "PoechantTCPConnection.h"
#include "Poco/Util/Application"
#include "Poco/Timestamp.h"
#include "Poco/Exception.h"
#include "Poco/DateTimeFormatter.h"

PoechantTCPConnection(const StreamSocket& s, const std::string& arg1, int arg2, double arg3):
    TCPServerConnection(s), _arg1(arg1), _arg2(arg2), _arg3(arg3)
{
}
void run()
{
    Application& app = Application::instance();
    // 日志输出连接的TCP用户的地址（IP和端口）
    app.logger().information("Request from " + this->socket().peerAddress().toString());
    try
    {
        // 向客户端发送数据，这里以发送一个表示时间的字符串为例
        Timestamp now;
        std::string dt(DateTimeFormatter::format(now, _format));
        dt.append("\r\n");
        socket().sendBytes(dt.data(), (int) dt.length());
    }
    catch (Poco::Exception& e)
    {
        app.logger().log(e);
    }
}
```

### 2.3 PoechantTCPConnectionFactory

工厂模式不必多说，名字唬人，其实非常非常简单（准确的说设计模式大部分名字都唬人，但大部分都很有用，设计模式本身并不牛B，能把设计模式抽象提炼出来成我们现在认为很简单的这些模式的那几个人很牛B）。具体如下：

```
// PoechantTCPConnectionFactory.h

#ifndef POECHANT_TCP_CONNECTION_FACTORY_H
#define POECHANT_TCP_CONNECTION_FACTORY_H

#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/StreamSocket.h"
#include <string>

class PoechantTCPConnectionFactory: public TCPServerConnectionFactory
{
    public:
    PoechantTCPConnectionFactory(const std::string arg1, int arg2, double arg3)
        : _arg1(arg1), _arg2(arg2), _arg3(arg3)
    {
    }

    TCPServerConnection* createConnection(const StreamSocket& socket)
    {
        return new PoechantTCPConnection(socket, arg1, arg2, arg3);
    }

private:
    std::string arg1;
    int arg2;
    double arg3;
};

#endif
```

### 2.4 启动

回头来说`PoechantTCPServer::main(const std::vector<std::string>& args)`，其过程就是创建一个绑定了地址的`ServerSocket`，把它传给`TCPServer`，当然别忘了把工程对象也给你的`TCPServer`传一个。最后就`start()`，`waitForTerminationRequest`和`stop()`就行了。

```
int PoechantTCPServer::main(const std::vector<std::string>& args)
{
    unsigned short port = (unsigned short) config().getInt("PoechantTCPServer.port", 12346);
    std::string format(config().getString("PoechantTCPServer.format",
        DateTimeFormat::ISO8601_FORMAT));

    // 1. Bind a ServerSocket with an address
    ServerSocket serverSocket(port);

    // 2. Pass the ServerSocket to a TCPServer
    TCPServer server(new PoechantTCPConnectionFactory(format), serverSocket);

    // 3. Start the TCPServer
    server.start();

    // 4. Wait for termination
    waitForTerminationRequest();

    // 5. Stop the TCPServer
    server.stop();

    return Application::EXIT_OK;
}
```

然后写一个程序入口：

```
#include "PoechantTCPServer.h"

int main(int argc, char **argv)
{
    return PoechantTCPServer().run(argc, argv);
}
```

## 3 写一个 Client 测测

TCPServer 要用 TCP 的客户端来测试。在 POCO 中有丰富的 Socket，其中 TCP 方式的 Socket 有：
- `Poco::Net::ServerSocket`
- `Poco::Net::StreamSocket`
- `Poco::Net::DialogSocket`
- `Poco::Net::SecureServerSocket`
- `Poco::Net::SecureStreamSocket`

UDP 方式的 Socket 有：
- `Poco::Net::DatagramSocket`
- `Poco::Net::MulticastSocket`

一个 TCP 方式 Client 如下（这里用了 while 循环，其实可以在收到数据后就关闭的）

```
#include <iostream>
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"

#define BUFFER_SIZE 1024

using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;

int main (int argc, const char * argv[])
{
    SocketAddress address("127.0.0.1", 12346);
    StreamSocket socket(address);
    char buffer[BUFFER_SIZE];
    while (true)
    {
        if (socket.available())
        {
            int len = socket.receiveBytes(buffer, BUFFER_SIZE);
            buffer[len] = '\0';
            std::cout << "" << buffer << std::endl;
        }
    }
    return 0;
}
```

-

from：[Blog.CSDN.net/Poechant](file:///Users/michael/Writing/CSDN%E5%8D%9A%E5%AE%A2-%E5%B7%B2%E5%8F%91%E5%B8%83/POCO%E5%BA%93%E4%B8%AD%E6%96%87%E7%BC%96%E7%A8%8B%E5%8F%82%E8%80%83%E6%8C%87%E5%8D%97/Blog.CSDN.net/Poechant)









