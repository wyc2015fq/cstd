# POCO库中文编程参考指南（11）如何使用Reactor框架？ - DoubleLi - 博客园






## 1 Reactor 框架概述

POCO 中的 Reactor 框架是基于 Reactor 设计模式进行设计的。其中由 Handler 将某 Socket 产生的事件，发送到指定的对象的方法上，作为回调。

## 2 光说不练假把式

PoechantReactorServer 类，基本与 PoechantTCPServer：

```
class PoechantReactorServer: public ServerApplication
{
public:
    PoechantServer() {} //: _helpRequested(false) {}
    ~PoechantServer() {}

protected:
    void initialize(Application& self)
    {
        loadConfiguration();
        ServerApplication::initialize(self);
    }
    void uninitialize()
    {
        ServerApplication::uninitialize();
    }
    int main(const std::vector<std::string>& args)
    {
        // …
        return Application::EXIT_OK;
    }
}
```

PoechantServiceHandler 类定义如下。起重机把`onReadable`和`onShutdown`的声音带来很大的麻烦。

```
class PoechantServiceHandler
{
public:
    PoechantServiceHandler(StreamSocket& socket, SocketReactor& reactor)；
    ~PoechantServiceHandler();
    void onReadable(const AutoPtr<ReadableNotification>& pNf);
    void onShutdown(const AutoPtr<ShutdownNotification>& pNf);
private:
    enum
    {
        BUFFER_SIZE = 1024
    };
    StreamSocket _socket;
    SocketReactor& _reactor;
    char *_pBuffer;
}；
```

PoechantServiceHandler 实现：

```
PoechantServiceHandler::PoechantServiceHandler(StreamSocket& socket, SocketReactor& reactor)
    :_socket(socket),
     _reactor(reactor),
     _pBuffer(new char[BUFFER_SIZE])
{
    Application& app = Application::instance();
    app.logger().information("Connection from" + socket.peerAddress().toString());
    _reactor.addEventHandler(_socket,
        NObserver<PoechantServiceHandler,
            ReadableNotification>(*this, &PoechantServiceHandler::onReadable));
    _reactor.addEventHandler(_socket,
        NObserver<PoechantServiceHandler,
            ShutdownNotification>(*this, &PoechantServiceHandler::onShutdown));
}
~PoechantServiceHandler()
{
    Application& app = Application::instance();
    app.logger().information("Disconnecting " + _socket.peerAddress().toString());
    _reactor.removeEventHandler(_socket,
        NObserver<PoechantServiceHandler,
            ReadableNotification>(*this, &PoechantServiceHandler::onReadable));
    _reactor.removeEventHandler(_socket,
        NObserver<PoechantServiceHandler,
            ShutdownNotification>(*this, &PoechantServiceHandler::onShutdown));
    delete [] _pBuffer;
}
void onReadable(const AutoPtr<ReadableNotification>& pNf)
{
    // Receive data from StreamSocket
    int n = _socket.receiveBytes(_pBuffer, BUFFER_SIZE);

    // Send data back the client 
    if (n > 0)
        _socket.sendBytes(_pBuffer, n);
    else
        delete this;
}

// When ShutdownNotification is detected, this method will be invoked.
void onShutdown(const AutoPtr<ShutdownNotification>& pNf)
{
    delete this;
}
```

启动：

```
int main(const std::vector<std::string>& args)
{
    unsigned short port = (unsigned short) config().getInt("PoechantReactor.port", 12345);
    ServerSocket serverSocket(port);
    SocketReactor reactor;
    SocketAcceptor<PoechantServiceHandler> acceptor(serverSocket, reactor);

    reactor.run();

    waitForTerminationRequest();
    reactor.stop();

    return Application::EXIT_OK;
}

int main(int argc, char **argv)
{
    return PoechantServer().run(argc, argv);
}
```

## 3 Clinet 测试代码

同[《POCO库中文编程参考指南（10）如何使用TCPServer框架？》](http://blog.csdn.net/poechant/article/details/7484781)中的 Client 测试用例。

-

from：[Blog.CSDN.net/Poechant](file:///Users/michael/Writing/CSDN%E5%8D%9A%E5%AE%A2-%E8%8D%89%E7%A8%BF%E7%AE%B1/Blog.CSDN.net/Poechant)









