# qt之tcp（一） - baidu_33570760的博客 - CSDN博客
2017年05月12日 15:58:45[carman_风](https://me.csdn.net/baidu_33570760)阅读数：213
个人分类：[qt](https://blog.csdn.net/baidu_33570760/article/category/6898155)
## 转载自：https://wizardforcel.gitbooks.io/qt-beginning/content/43.html
## 导语
TCP即TransmissionControl Protocol，传输控制协议。与UDP不同，它是面向连接和数据流的可靠传输协议。也就是说，它能使一台计算机上的数据无差错的发往网络上的其他计算机，所以当要传输大量数据时，我们选用TCP协议。
TCP协议的程序使用的是客户端/服务器（C/S）模式，在Qt中提供了`QTcpSocket`类来编写客户端程序，使用`QTcpServer`类编写服务器端程序。我们在服务器端进行端口的，一旦发现客户端的连接请求，就会发出`newConnection()`信号，可以关联这个信号到我们自己的槽进行数据的发送。而在客户端，一旦有数据到来就会发出`readyRead()`信号，可以关联此信号进行数据的接收。其实，在程序中最难理解的地方就是程序的发送和接收了，为了让大家更好的理解，我们在这一节只是讲述一个传输简单的字符串的例子，在下一节再进行扩展，实现任意文件的传输。
环境：Windows Xp + Qt 4.8.5+Qt Creator2.8.0
## 目录
- 一、服务器端
- 二、客户端
## 正文
一、服务器端
在服务器端的程序中，我们本地主机的一个端口，这里使用6666，然后关联`newConnection()`信号与自己写的`sendMessage()`槽。就是说一旦有客户端的连接请求，就会执行`sendMessage()`函数，在这个函数里我们发送一个简单的字符串。
1．新建QtGui应用
项目名为`tcpServer`，基类选择`QWidget`，类名为`Widget`。完成后打开项目文件`tcpServer.pro`并添加一行代码：`QT += network` ，然后保存该文件。
2．在`widget.ui`的设计区添加一个Label，更改其显示文本为“等待连接”，然后更改其`objectName`为`statusLabel`，用于显示一些状态信息。
3．在`widget.h`文件中做以下更改。
添加头文件：`#include <QtNetWork>`
添加private对象：`QTcpServer *tcpServer;`
添加私有槽：
```cpp
private slots:
void sendMessage();
```
4．在`widget.cpp`文件中进行更改。
在其构造函数中添加代码：
```cpp
tcpServer = new QTcpServer(this);
if(!tcpServer->listen(QHostAddress::LocalHost,6666))
{  //本地主机的6666端口，如果出错就输出错误信息，并关闭
    qDebug() << tcpServer->errorString();
    close();
}
//连接信号和相应槽函数
connect(tcpServer,SIGNAL(newConnection()),this,SLOT(sendMessage()));
```
我们在构造函数中使用`tcpServer`的`listen()`函数进行，然后关联了`newConnection()`和我们自己的`sendMessage()`函数。
下面我们实现`sendMessage()`函数。
```cpp
void Widget::sendMessage()
{
    //用于暂存我们要发送的数据
    QByteArray block;
    //使用数据流写入数据
    QDataStream out(&block,QIODevice::WriteOnly);
    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out.setVersion(QDataStream::Qt_4_6);
    out<<(quint16) 0;
    out<<tr("hello Tcp!!!");
    out.device()->seek(0);
    out<<(quint16) (block.size() - sizeof(quint16));
    //我们获取已经建立的连接的子套接字
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection,SIGNAL(disconnected()),clientConnection,
           SLOT(deleteLater()));
    clientConnection->write(block);
    clientConnection->disconnectFromHost();
    //发送数据成功后，显示提示
    ui->statusLabel->setText("send message successful!!!");
}
```
这个是数据发送函数，我们主要介绍两点：
（1）为了保证在客户端能接收到完整的文件，我们都在数据流的最开始写入完整文件的大小信息，这样客户端就可以根据大小信息来判断是否接受到了完整的文件。而在服务器端，在发送数据时就要首先发送实际文件的大小信息，但是，文件的大小一开始是无法预知的，所以这里先使用了`out<<(quint16) 0;`在`block`的开始添加了一个`quint16`大小的空间，也就是两字节的空间，它用于后面放置文件的大小信息。然后
```
out<<tr("hello
 Tcp!!!");
```
输入实际的文件，这里是字符串。当文件输入完成后我们再使用`out.device()->seek(0);`返回到`block`的开始，加入实际的文件大小信息，也就是后面的代码，它是实际文件的大小：`out<<(quint16) (block.size() - sizeof(quint16));`
（2）在服务器端我们可以使用`tcpServer`的`nextPendingConnection()`函数来获取已经建立的连接的Tcp套接字，使用它来完成数据的发送和其它操作。比如这里，我们关联了`disconnected()`信号和`deleteLater()`槽，然后我们发送数据
```cpp
clientConnection->write(block);
```
然后是`clientConnection->disconnectFromHost();`
它表示当发送完成时就会断开连接，这时就会发出`disconnected()`信号，而最后调用`deleteLater()`函数保证在关闭连接后删除该套接字`clientConnection`。
5．这样服务器的程序就完成了，可以先运行一下程序。
二、客户端
我们在客户端程序中向服务器发送连接请求，当连接成功时接收服务器发送的数据。
1．新建Qt Gui应用，
项目名`tcpClient`，基类选择`QWidget`，类名为`Widget`。完成后打开项目文件`tcpClient.pro`并添加一行代码：`QT += network` ，然后保存该文件。
2．我们在`widget.ui`中添加几个标签`Label`和两个`Line Edit`以及一个按钮`Push Button`。设计效果如下图所示。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/37-1.jpg)
其中“主机”后的`LineEdit`的`objectName`为`hostLineEdit`，“端口号”后的为`portLineEdit`。
“收到的信息”标签的`objectName`为`messageLabel` 。
3．在`widget.h`文件中做更改。
添加头文件：`#include <QtNetwork>`
添加`private`变量：
```cpp
QTcpSocket *tcpSocket;
QString message;  //存放从服务器接收到的字符串
quint16blockSize; //存放文件的大小信息
```
添加私有槽：
```cpp
private slots:
    void newConnect(); //连接服务器
    void readMessage();  //接收数据
void displayError(QAbstractSocket::SocketError);  //显示错误
```
4．在`widget.cpp`文件中做更改。
（1）在构造函数中添加代码：
```cpp
tcpSocket = new QTcpSocket(this);
connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
         this,SLOT(displayError(QAbstractSocket::SocketError)));
```
这里关联了`tcpSocket`的两个信号，当有数据到来时发出`readyRead()`信号，我们执行读取数据的`readMessage()`函数。当出现错误时发出`error()`信号，我们执行`displayError()`槽函数。
(2)实现`newConnect()`函数。
```cpp
void Widget::newConnect()
{
    blockSize = 0; //初始化其为0
tcpSocket->abort(); //取消已有的连接
//连接到主机，这里从界面获取主机地址和端口号
    tcpSocket->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());
}
```
这个函数实现了连接到服务器，下面会在“连接”按钮的单击事件槽函数中调用这个函数。
（3）实现`readMessage()`函数。
```cpp
void Widget::readMessage()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_6);
    //设置数据流版本，这里要和服务器端相同
    if(blockSize==0) //如果是刚开始接收数据
    {
       //判断接收的数据是否有两字节，也就是文件的大小信息
       //如果有则保存到blockSize变量中，没有则返回，继续接收数据
       if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
       in >> blockSize;
    }
    if(tcpSocket->bytesAvailable() < blockSize) return;
    //如果没有得到全部的数据，则返回，继续接收数据
    in >> message;
    //将接收到的数据存放到变量中
    ui->messageLabel->setText(message);
    //显示接收到的数据
}
```
这个函数实现了数据的接收，它与服务器端的发送函数相对应。首先我们要获取文件的大小信息，然后根据文件的大小来判断是否接收到了完整的文件。
(4)实现`displayError()`函数。
```cpp
void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString(); //输出错误信息
}
```
这里简单的实现了错误信息的输出。
（5）我们在`widget.ui`中进入“连接”按钮的单击事件槽函数，然后更改如下。
```cpp
void Widget::on_pushButton_clicked() //连接按钮
{
    newConnect(); //请求连接
}
```
这里直接调用了`newConnect()`函数。
5．我们运行程序，同时运行服务器程序，然后在“主机”后填入“localhost”，在“端口号”后填入“6666”，点击“连接”按钮，效果如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/37-2.jpg)
可以看到我们正确地接收到了数据。因为服务器端和客户端是在同一台机子上运行的，所以我这里填写了“主机”为“localhost”，如果你在不同的机子上运行，需要在“主机”后填写其正确的IP地址。
## 结语
到这里我们最简单的TCP应用程序就完成了，在下一节我们将会对它进行扩展，实现任意文件的传输。
涉及到的源码:
- [tcpServer.rar](http://bbs.qter.org/forum.php?mod=attachment&aid=Nzc1fDMyNmNkODkyfDE0NzE3MDA5MTl8MTA5NTl8NjEx)
- [tcpClient.rar](http://bbs.qter.org/forum.php?mod=attachment&aid=Nzc2fDAwM2Y5MWMwfDE0NzE3MDA5MTl8MTA5NTl8NjEx)
