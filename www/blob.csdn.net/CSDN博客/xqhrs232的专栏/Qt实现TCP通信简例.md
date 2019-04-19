# Qt实现TCP通信简例 - xqhrs232的专栏 - CSDN博客
2018年04月22日 22:33:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：468
原文地址::[https://blog.csdn.net/hackertom/article/details/73556786](https://blog.csdn.net/hackertom/article/details/73556786)
相关文章
1、[Qt实现UDP通信简例](https://blog.csdn.net/HackerTom/article/details/73530845)----[https://blog.csdn.net/hackertom/article/details/73530845](https://blog.csdn.net/hackertom/article/details/73530845)
2、[Qt实现TCP文件传输例子](https://blog.csdn.net/HackerTom/article/details/73692902)----[https://blog.csdn.net/hackertom/article/details/73692902](https://blog.csdn.net/hackertom/article/details/73692902)
- [参考](https://blog.csdn.net/hackertom/article/details/73556786#%E5%8F%82%E8%80%83)
- [Notes](https://blog.csdn.net/hackertom/article/details/73556786#notes)
- [流程](https://blog.csdn.net/hackertom/article/details/73556786#%E6%B5%81%E7%A8%8B)
- [Code](https://blog.csdn.net/hackertom/article/details/73556786#code)- [客户端](https://blog.csdn.net/hackertom/article/details/73556786#%E5%AE%A2%E6%88%B7%E7%AB%AF)- [TcpClienth](https://blog.csdn.net/hackertom/article/details/73556786#tcpclienth)
- [TcpClientcpp](https://blog.csdn.net/hackertom/article/details/73556786#tcpclientcpp)
- [maincpp](https://blog.csdn.net/hackertom/article/details/73556786#maincpp)
- [服务器](https://blog.csdn.net/hackertom/article/details/73556786#%E6%9C%8D%E5%8A%A1%E5%99%A8)- [TcpServerh](https://blog.csdn.net/hackertom/article/details/73556786#tcpserverh)
- [TcpServercpp](https://blog.csdn.net/hackertom/article/details/73556786#tcpservercpp)
- [maincpp](https://blog.csdn.net/hackertom/article/details/73556786#maincpp-1)
- [截图](https://blog.csdn.net/hackertom/article/details/73556786#%E6%88%AA%E5%9B%BE)- [工程截图](https://blog.csdn.net/hackertom/article/details/73556786#%E5%B7%A5%E7%A8%8B%E6%88%AA%E5%9B%BE)
- [效果](https://blog.csdn.net/hackertom/article/details/73556786#%E6%95%88%E6%9E%9C)
# 参考
[QT TCP socket通信（一）](http://blog.csdn.net/zouhao619/article/details/6761750)
# Notes
- 继上篇[Qt实现UDP通信简例](http://blog.csdn.net/hackertom/article/details/73530845) 后，换成用 TCP 的版本，依旧是没有图形界面
- 注意同上篇
- client 执行`connectToHost()`后，server
 的`imcomingConnection()`会
```
emit
 newConnection()
```
- server 的`close()`会调`disconnectFromHost()`，它会
```
emit
 disconnected()
```
- 此例是 server 收到信息后就主动断开连接
- 从 client 和 server 的结构看，server 本身也可以充当一个 client。要实现 P2P 既是 client 又是 server 的要求，我有一个大胆的想法：在TcpServer 里多加一个 QTcpSocket 的指针就直接当
 P2P 的软件
# 流程
- client 向 server 发连接请求（`connectToHost()`）
- server 收到请求（`newConnection()`），同意建立连接（`accept_connection()`）
- client 收到同意（`connected()`），向
 server 发数据（`send_message()`）
- server 收到数据（`readyRead()`），读取并回复
 client（`read_and_reply()`），然后断交（`close()`）
- client 收到断交通知（`disconnected()`）
# Code
## 客户端
### TcpClient.h
```cpp
#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QObject>
#include <QTcpSocket>
class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient(QObject *p = 0);
    ~TcpClient();
public slots:
    void send_msg(); // 向server发信息
    void read_msg(); // 读server回复的信息
    void end_connent(); // 被断开连接后提示
private:
    QTcpSocket *tSocket;
};
#endif // TCPCLIENT_H
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
### TcpClient.cpp
```cpp
#include <QByteArray>
#include <QHostAddress>
#include <QTcpSocket>
#include <iostream>
#include "tcpclient.h"
const quint16 PORT = 2333;
TcpClient::TcpClient(QObject *p) :
    QObject(p)
{
    tSocket = new QTcpSocket(this);
    std::cout << "--- Connect to Host ---" << std::endl;
    /* 向 server 发送连接请求 */
    tSocket->connectToHost(QHostAddress::LocalHost, PORT);
    /*------- 连接信号与槽 -------*/
    /* 一连上 server 就发信息 */
    connect(tSocket, SIGNAL(connected()),
        this, SLOT(send_msg()));
    /* server 有回复就读取 */
    connect(tSocket, SIGNAL(readyRead()),
        this, SLOT(read_msg()));
    /* server 把连接断了就发出提示 */
    connect(tSocket, SIGNAL(disconnected()),
        this, SLOT(end_connent()));
}
TcpClient::~TcpClient()
{
    delete tSocket;
}
void TcpClient::send_msg()
{
    std::cout << "--- Successfully Connected ---\n"
              << "--- Sending Message ---"
              << std::endl;
    /* 发信息 */
    tSocket->write(QByteArray("Hello world!"));
}
void TcpClient::read_msg()
{
    /* 读信息 */
    QByteArray ba = tSocket->readAll();
    std::cout << ba.data() << std::endl;
}
void TcpClient::end_connent()
{
    std::cout << "--- Connection Ended ---" << std::endl;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
### main.cpp
```cpp
#include <QCoreApplication>
#include "tcpclient.h"
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "--- TCP Client ---" << std::endl;
    TcpClient tc;
    return a.exec();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
## 服务器
### TcpServer.h
```cpp
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
class TcpServer : QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QTcpServer *p = 0);
    ~TcpServer();
public slots:
    void accept_connection(); // 处理连接请求
    void read_and_reply(); // 读信息并回复，然后断连
private:
    QTcpServer *tServer;
    QTcpSocket *tSocket;
};
#endif // TCPSERVER_H
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
### TcpServer.cpp
```cpp
#include <QByteArray>
#include <QHostAddress>
#include <iostream>
#include "tcpserver.h"
const quint16 PORT = 2333;
TcpServer::TcpServer(QTcpServer *p) :
    QTcpServer(p)
{
    tServer = new QTcpServer;
    tSocket = new QTcpSocket;
    /* 监听端口 */
    if(tServer->listen(QHostAddress::Any, PORT))
        std::cout << "--- Linsting to Port ---" << std::endl;
    else
        std::cout << "*** FAIL LISTING ***" << std::endl;
    /* 有新连接请求就处理它 */
    connect(tServer, SIGNAL(newConnection()),
        this, SLOT(accept_connection()));
}
TcpServer::~TcpServer()
{
    delete tServer;
    delete tSocket;
}
void TcpServer::accept_connection()
{
    std::cout << "--- Accept Connection ---" << std::endl;
    /* server让自己的socket与client的socket相连 */
    tSocket = tServer->nextPendingConnection();
    /* socket一收到信息就读取并回复 */
    connect(tSocket, SIGNAL(readyRead()),
        this, SLOT(read_and_reply()));
}
void TcpServer::read_and_reply()
{
    std::cout << "--- Read Message ---" << std::endl;
    /* 读信息 */
    QByteArray ba = tSocket->readAll();
    std::cout << ba.data() << std::endl;
    std::cout << "--- Reply ---" << std::endl;
    /* 回复 */
    tSocket->write("Nice day");
    tSocket->close();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
### main.cpp
```cpp
#include <QCoreApplication>
#include <iostream>
#include "tcpserver.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "--- TCP Server ---" << std::endl;
    TcpServer ts;
    return a.exec();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
# 截图
## 工程截图
![project](https://img-blog.csdn.net/20170621233249973?/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFja2VyVG9t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 效果
![result](https://img-blog.csdn.net/20170621233404780?/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFja2VyVG9t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

