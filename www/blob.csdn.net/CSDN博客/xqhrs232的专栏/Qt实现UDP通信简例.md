# Qt实现UDP通信简例 - xqhrs232的专栏 - CSDN博客
2018年04月22日 22:14:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：95
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/hackertom/article/details/73530845](https://blog.csdn.net/hackertom/article/details/73530845)
相关文章
1、[Qt实现TCP通信简例](https://blog.csdn.net/HackerTom/article/details/73556786)----[https://blog.csdn.net/hackertom/article/details/73556786](https://blog.csdn.net/hackertom/article/details/73556786)
2、[Qt实现UDP广播简例](https://blog.csdn.net/HackerTom/article/details/73604924)----[https://blog.csdn.net/hackertom/article/details/73604924](https://blog.csdn.net/hackertom/article/details/73604924)

Qt（4） ![](https://csdnimg.cn/release/phoenix/images/arrow_triangle%20_down.jpg)网络编程（4） ![](https://csdnimg.cn/release/phoenix/images/arrow_triangle%20_down.jpg)
版权声明：本文为博主原创文章，转载请注明出处。 https://blog.csdn.net/HackerTom/article/details/73530845
目录[(?)](https://blog.csdn.net/hackertom/article/details/73530845)[[+]](https://blog.csdn.net/hackertom/article/details/73530845#)
- [参考](https://blog.csdn.net/hackertom/article/details/73530845#%E5%8F%82%E8%80%83)
- [目标](https://blog.csdn.net/hackertom/article/details/73530845#%E7%9B%AE%E6%A0%87)
- [特点](https://blog.csdn.net/hackertom/article/details/73530845#%E7%89%B9%E7%82%B9)
- [Tips](https://blog.csdn.net/hackertom/article/details/73530845#tips)
- [源码](https://blog.csdn.net/hackertom/article/details/73530845#%E6%BA%90%E7%A0%81)- [发送端](https://blog.csdn.net/hackertom/article/details/73530845#%E5%8F%91%E9%80%81%E7%AB%AF)- [maincpp](https://blog.csdn.net/hackertom/article/details/73530845#maincpp)
- [接收端](https://blog.csdn.net/hackertom/article/details/73530845#%E6%8E%A5%E6%94%B6%E7%AB%AF)- [UdpReceiverh](https://blog.csdn.net/hackertom/article/details/73530845#udpreceiverh)
- [UdpReceivercpp](https://blog.csdn.net/hackertom/article/details/73530845#udpreceivercpp)
- [maincpp](https://blog.csdn.net/hackertom/article/details/73530845#maincpp-1)
- [截图](https://blog.csdn.net/hackertom/article/details/73530845#%E6%88%AA%E5%9B%BE)- [工程](https://blog.csdn.net/hackertom/article/details/73530845#%E5%B7%A5%E7%A8%8B)
- [qmake](https://blog.csdn.net/hackertom/article/details/73530845#qmake)
- [效果](https://blog.csdn.net/hackertom/article/details/73530845#%E6%95%88%E6%9E%9C)
# 参考
[QT实现UDP通信](http://tieba.baidu.com/p/4301470835)（原版例子！！！） 
[Singals & Slots](http://doc.qt.io/qt-4.8/signalsandslots.html)（官方文档） 
[Qt入门之信号与槽机制](http://www.cnblogs.com/MuyouSome/p/3515941.html)
[Qt 信号槽的实现](https://www.devbean.net/2012/12/how-qt-signals-and-slots-work/)
[Qt学习 之 Socket通信](http://blog.csdn.net/u013007900/article/details/50411796)
[Qt的Socket数据通讯的一个例子](http://blog.csdn.net/u012997311/article/details/47724817)
[Qt 下的udp socket通信示例](http://download.csdn.net/download/alan00000/4677869)（要下载的完整Qt工程） 
[CSDN-markdown语法之如何插入图片](http://blog.csdn.net/lanxuezaipiao/article/details/44310775)（乱入…）
# 目标
终极目标是完成计网课设：基于P2P的局域网即时通信系统。 
然而…第一次接触 Qt，完全没有概念，希望有个尽量简单的例子见识一下 Qt 网络编程的套路。看完参考的例子后学写了一波。
# 特点
- 只有一边发送、另一边接收，且都在本地
- 没有图形界面，就黑框框
# Tips
- Qt 版本：5.8
- .pro 文件里要加一句：
```
QT
 += network
```
，接着一定要 执行qmake
- 约定用 2333 号端口（随便啦）
- 测试时要在本机启动两个程序，就把发送端和接收段放在两个工程里，分别置为活动工程、运行，就可以启动两个了
- singals 和 slots 其实就是普通的成员函数，见上面 Qt 信号槽的实现 的链接中带有魔法的宏
- `bind()` 大概是使用那个端口的意思，就从那端口读数据
- 一旦端口收到数据，就会`emit`一个叫`readyRead()`的信号，然后那些`connect()`了`readyRead()`的槽（例中的`receive()`）就会收到通知（被调用），然后进行相应的操作（函数体）
# 源码
## 发送端
### main.cpp
```cpp
#include <QByteArray>
#include <QCoreApplication>
#include <QHostAddress>
#include <QUdpSocket>
const quint16 PORT = 2333;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QUdpSocket qus;
//  qus.bind(QHostAddress("127.0.0.1"), PORT+1);
    QByteArray msg = "Hello world!";
    std::cout << "--- Sender ---" << std::endl;
    for(int i=0; i<100; ++i)
        qus.writeDatagram(msg, QHostAddress("127.0.0.1"), PORT);
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
- 14
- 15
- 16
- 17
- 18
- 19
- 20
## 接收端
### UdpReceiver.h
```cpp
#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H
#include <QObject>
#include <QUdpSocket>
class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    UdpReceiver(QObject *p = 0);
    ~UdpReceiver();
public slots:
    void receive();
private:
    QUdpSocket *uSocket;
};
#endif // UDPRECEIVER_H
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
### UdpReceiver.cpp
```cpp
#include <QByteArray>
#include <iostream>
#include "UdpReceiver.h"
const quint16 PORT = 2333;
UdpReceiver::UdpReceiver(QObject *p) :
    QObject(p)
{
    uSocket = new QUdpSocket;
    uSocket->bind(QHostAddress("127.0.0.1"), PORT);
    connect(uSocket, SIGNAL(readyRead()), this, SLOT(receive()));
}
UdpReceiver::~UdpReceiver()
{
    delete uSocket;
}
void UdpReceiver::receive()
{
    QByteArray ba;
    while(uSocket->hasPendingDatagrams())
    {
        ba.resize(uSocket->pendingDatagramSize());
        uSocket->readDatagram(ba.data(), ba.size());
        std::cout << ba.data() << std::endl;
    }
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
### main.cpp
```cpp
#include <QCoreApplication>
#include "udpreceiver.h"
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    UdpReceiver ur;
    std::cout << "--- Recevier ---" << std::endl;
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
## 工程
![工程截图](https://img-blog.csdn.net/20170621115343224?/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFja2VyVG9t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## qmake
![qmake](https://img-blog.csdn.net/20170621115231146?/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFja2VyVG9t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 效果
![效果图](https://img-blog.csdn.net/20170621115716892?/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFja2VyVG9t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
