# Qt5 UDP网络通讯详解 - fanyun的博客 - CSDN博客
2017年09月30日 21:43:41[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1151
        Qt封装了很多通讯模块，常用的有UDP，TCP，HTTP通讯模块，现在我们详解说下Qt的UDP通讯。
        Qt的UDP模块是相对其他软件自带的UDP封装里面比较好的一个，稳定可靠，方法集成度高，使用方便。并且常见于CS架构的系统里面。
一般常见Qt UDP客户端和服务端构成如下：
       1.客户端,即Server端构成如下：
       本地DUP，绑定本地客户端口和IP，发送服务端口，发送服务IP几部分构成，一般实现代码如下：
```cpp
udpSocket = new QUdpSocket(this);
udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
connect(udpSocket, SIGNAL(readyRead()), this, SLOT(SendDatagrams()));
udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);
```
      2.服务端，即Client端构成如下：
      接受端IP，接收端口号，接收函数：hasPendingDatagrams()，一般实现代码如下：
```cpp
datagram.resize(udp_socket_rd->pendingDatagramSize());
        QHostAddress reader;
        quint16 readerPort;
        reader =  QHostAddress("192.XXX.3.1");
        readerPort = 8888;
        udp_socket_rd->readDatagram(datagram.data(), datagram.size(),
                                    &reader, &readerPort);
```
    在我们的GUI程序里面，按以上方法便可以实现UDP的基本通讯收发功能。
