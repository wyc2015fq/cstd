# qt之tcp（二） - baidu_33570760的博客 - CSDN博客
2017年05月12日 16:00:57[carman_风](https://me.csdn.net/baidu_33570760)阅读数：253
个人分类：[qt](https://blog.csdn.net/baidu_33570760/article/category/6898155)
转载自：https://wizardforcel.gitbooks.io/qt-beginning/content/44.html
在上一节里我们使用TCP服务器发送一个字符串，然后在TCP客户端进行接收。在这一节将重新写一个客户端程序和一个服务器程序，这次实现客户端进行文件的发送，服务器进行文件的接收。有了上一节的基础，这一节的内容就很好理解了，注意一下几个信号和槽的关联即可。当然，我们这次要更深入了解一下数据的发送和接收的处理方法。
环境：Windows Xp + Qt 4.8.5+QtCreator 2.8.0
## 目录
- 一、客户端
- 二、服务器端
## 正文
一、客户端
这次先讲解客户端，在客户端里需要与服务器进行连接，一旦连接成功，就会发出`connected()`信号，这时我们就进行文件的发送。
在上一节已经看到，发送数据时先发送了数据的大小信息。这一次，我们要先发送文件的总大小，然后文件名长度，然后是文件名，这三部分合称为文件头结构，最后再发送文件数据。所以在发送函数里就要进行相应的处理，当然，在服务器的接收函数里也要进行相应的处理。对于文件大小，这次使用了`qint64`，它是64位的，可以表示一个很大的文件了。
1．新建QtGui项目
名称为`tcpSender`，基类选择`QWidget`，类名为`Widget`，完成后打开`tcpSender.pro`添加一行代码：`QT += network` 。
2．我们在`widget.ui`文件中将界面设计如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/38-1.jpg)
这里“主机”后的`Line Edit`的`objectName`为`hostLineEdit`；“端口”后的`Line Edit`的`objectName`为`portLineEdit`；下面的`Progress Bar`的`objectName`为`clientProgressBar`，其`value`属性设为0；“状态”`Label`的`objetName`为`clientStatusLabel`；“打开”按钮的`objectName`为`openButton`；“发送”按钮的`objectName`为`sendButton`。
3．在`widget.h`文件中进行更改。
（1）添加头文件包含`#include <QtNetwork>`
（2）添加`private`变量：
```cpp
QTcpSocket *tcpClient;
    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径
QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
```
（3）添加私有槽函数：
```cpp
private slots:
    void send();  //连接服务器
    void startTransfer();  //发送文件大小等信息
    void updateClientProgress(qint64); //发送数据，更新进度条
    void displayError(QAbstractSocket::SocketError); //显示错误
void openFile();  //打开文件
```
4．在`widget.cpp`文件中进行更改
添加头文件：`#include <QFileDialog>`
（1）在构造函数中添加代码：
```cpp
loadSize = 4*1024;
totalBytes = 0;
bytesWritten = 0;
bytesToWrite = 0;
tcpClient = new QTcpSocket(this);
//当连接服务器成功时，发出connected()信号，我们开始传送文件
connect(tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
//当有数据发送成功时，我们更新进度条
connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,
       SLOT(updateClientProgress(qint64)));
connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
       SLOT(displayError(QAbstractSocket::SocketError)));
//开始使”发送“按钮不可用
ui->sendButton->setEnabled(false);
```
我们主要是进行了变量的初始化和几个信号和槽函数的关联。
（2）实现打开文件函数。
```cpp
void Widget::openFile()   //打开文件
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
       ui->sendButton->setEnabled(true);
       ui->clientStatusLabel->setText(tr("打开文件 %1 成功！")
                                       .arg(fileName));
    }
}
```
该函数将在下面的“打开”按钮单击事件槽函数中调用。
（3）实现连接函数。
```cpp
void Widget::send()   //连接到服务器，执行发送
{
    ui->sendButton->setEnabled(false);
    bytesWritten = 0;
    //初始化已发送字节为0
    ui->clientStatusLabel->setText(tr("连接中..."));
    tcpClient->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());//连接
}
```
该函数将在“发送”按钮的单击事件槽函数中调用。
（4）实现文件头结构的发送。
```cpp
void Widget::startTransfer()  //实现文件大小等信息的发送
{
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
       qDebug() << "open file error!";
       return;
    }
    //文件总大小
    totalBytes = localFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
QString currentFileName = fileName.right(fileName.size()
- fileName.lastIndexOf('/')-1);
    //依次写入总大小信息空间，文件名大小信息空间，文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //发送完头数据后剩余数据的大小
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    ui->clientStatusLabel->setText(tr("已连接"));
    outBlock.resize(0);
}
```
（5）下面是更新进度条，也就是发送文件数据。
```cpp
//更新进度条，实现文件的传送
void Widget::updateClientProgress(qint64 numBytes)
{
    //已经发送数据的大小
    bytesWritten += (int)numBytes;
    if(bytesToWrite > 0) //如果已经发送了数据
    {
   //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
   //就发送剩余数据的大小
       outBlock = localFile->read(qMin(bytesToWrite,loadSize));
       //发送完一次数据后还剩余数据的大小
       bytesToWrite -= (int)tcpClient->write(outBlock);
       //清空发送缓冲区
       outBlock.resize(0);
    } else {
       localFile->close(); //如果没有发送任何数据，则关闭文件
    }
    //更新进度条
    ui->clientProgressBar->setMaximum(totalBytes);
    ui->clientProgressBar->setValue(bytesWritten);
    if(bytesWritten == totalBytes) //发送完毕
    {
     ui->clientStatusLabel->setText(tr("传送文件 %1 成功")
.arg(fileName));
       localFile->close();
       tcpClient->close();
    }
}
```
（6）实现错误处理函数。
```cpp
void Widget::displayError(QAbstractSocket::SocketError) //显示错误
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText(tr("客户端就绪"));
    ui->sendButton->setEnabled(true);
}
```
（7）我们从`widget.ui`中分别进行“打开”按钮和“发送”按钮的单击事件槽函数，然后更改如下。
```cpp
void Widget::on_openButton_clicked() //打开按钮
{
    openFile();
}
void Widget::on_sendButton_clicked() //发送按钮
{
    send();
}
```
5．我们为了使程序中的中文不显示乱码，在`main.cpp`文件中更改。
添加头文件：`#include <QTextCodec>`
在main函数中添加代码：`QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));`
6．现在可以先运行程序。
7．程序整体思路分析。
我们设计好界面，然后按下“打开”按钮，选择要发送的文件，这时调用了`openFile()`函数。然后点击“发送”按钮，调用`send()`函数，与服务器进行连接。当连接成功时就会发出`connected()`信号，这时就会执行`startTransfer()`函数，进行文件头结构的发送，当发送成功时就会发出`bytesWritten(qint64)`信号，这时执行
```
updateClientProgress(qint64
 numBytes)
```
进行文件数据的传输和进度条的更新。这里使用了一个`loadSize`变量，我们在构造函数中将其初始化为`4*1024`即4字节，它的作用是，我们将整个大的文件分成很多小的部分进行发送，每部分为4字节。而当连接出现问题时就会发出`error(QAbstractSocket::SocketError)`信号，这时就会执行`displayError()`函数。对于程序中其他细节我们就不再分析，希望大家能自己编程研究一下。
二、服务器端
我们在服务器端进行数据的接收。服务器端程序是很简单的，我们开始进行监听，一旦发现有连接请求就发出`newConnection()`信号，然后我们便接受连接，开始接收数据。
1．新建QtGui应用
名称为`tcpReceiver`，基类选择`QWidget`，类名为`Widget`，完成后打开`tcpReceiver.pro`添加一行代码：`QT += network` 。
2．我们更改`widget.ui`文件，设计界面如下。
其中“服务器端”`Label`的`objectName`为`serverStatusLabel`；进度条`ProgressBar`的`objectName`为`serverProgressBar`，设置其`value`属性为0；“开始监听”按钮的`objectName`为`startButton`。
效果如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/38-2.jpg)
3．更改`widget.h`文件的内容。
（1）添加头文件包含：`#include <QtNetwork>`
（2）添加私有变量：
```cpp
QTcpServer tcpServer;
QTcpSocket *tcpServerConnection;
qint64 totalBytes;  //存放总大小信息
qint64 bytesReceived;  //已收到数据的大小
qint64 fileNameSize;  //文件名的大小信息
QString fileName;   //存放文件名
QFile *localFile;   //本地文件
QByteArray inBlock;   //数据缓冲区
```
（3）添加私有槽函数：
```cpp
private slots:
    void on_startButton_clicked();
    void start();   //开始监听
    void acceptConnection();  //建立连接
void updateServerProgress();  //更新进度条，接收数据
//显示错误
void displayError(QAbstractSocket::SocketError socketError);
```
4．更改`widget.cpp`文件。
（1）在构造函数中添加代码：
```cpp
totalBytes = 0;
    bytesReceived = 0;
fileNameSize = 0;
//当发现新连接时发出newConnection()信号
    connect(&tcpServer,SIGNAL(newConnection()),this,
SLOT(acceptConnection()));
```
（2）实现`start()`函数。
```cpp
void Widget::start() //开始监听
{
    ui->startButton->setEnabled(false);
    bytesReceived =0;
    if(!tcpServer.listen(QHostAddress::LocalHost,6666))
    {
       qDebug() << tcpServer.errorString();
       close();
       return;
    }
    ui->serverStatusLabel->setText(tr("监听"));
}
```
（3）实现接受连接函数。
```cpp
void Widget::acceptConnection()  //接受连接
{
    tcpServerConnection = tcpServer.nextPendingConnection();
connect(tcpServerConnection,SIGNAL(readyRead()),this,
SLOT(updateServerProgress()));
    connect(tcpServerConnection,
SIGNAL(error(QAbstractSocket::SocketError)),this,
           SLOT(displayError(QAbstractSocket::SocketError)));
    ui->serverStatusLabel->setText(tr("接受连接"));
    tcpServer.close();
}
```
（4）实现更新进度条函数。
```cpp
void Widget::updateServerProgress()  //更新进度条，接收数据
{
   QDataStream in(tcpServerConnection);
   in.setVersion(QDataStream::Qt_4_6);
   if(bytesReceived <= sizeof(qint64)*2)
   { //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
       if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)
           && (fileNameSize == 0))
       { //接收数据总大小信息和文件名大小信息
           in >> totalBytes >> fileNameSize;
           bytesReceived += sizeof(qint64) * 2;
       }
       if((tcpServerConnection->bytesAvailable() >= fileNameSize)
           && (fileNameSize != 0))
       {  //接收文件名，并建立文件
           in >> fileName;
           ui->serverStatusLabel->setText(tr("接收文件 %1 ...")
                                           .arg(fileName));
           bytesReceived += fileNameSize;
           localFile= new QFile(fileName);
           if(!localFile->open(QFile::WriteOnly))
           {
                qDebug() << "open file error!";
                return;
           }
       }
       else return;
   }
   if(bytesReceived < totalBytes)
   {  //如果接收的数据小于总数据，那么写入文件
      bytesReceived += tcpServerConnection->bytesAvailable();
      inBlock= tcpServerConnection->readAll();
      localFile->write(inBlock);
      inBlock.resize(0);
   }
//更新进度条
   ui->serverProgressBar->setMaximum(totalBytes);
   ui->serverProgressBar->setValue(bytesReceived);
   if(bytesReceived == totalBytes)
   { //接收数据完成时
    tcpServerConnection->close();
    localFile->close();
    ui->startButton->setEnabled(true);
ui->serverStatusLabel->setText(tr("接收文件 %1 成功！")
.arg(fileName));
   }
}
```
（5）错误处理函数。
```cpp
void Widget::displayError(QAbstractSocket::SocketError) //错误处理
{
    qDebug() << tcpServerConnection->errorString();
    tcpServerConnection->close();
    ui->serverProgressBar->reset();
    ui->serverStatusLabel->setText(tr("服务端就绪"));
    ui->startButton->setEnabled(true);
}
```
（6）我们在`widget.ui`中进入“开始监听”按钮的单击事件槽函数，更改如下。
```cpp
void Widget::on_startButton_clicked() //开始监听按钮
{
    start();
}
```
5．我们为了使程序中的中文不显示乱码，在`main.cpp`文件中更改。
添加头文件包含：`#include <QTextCodec>`
在`main`函数中添加代码：`QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));`
6．运行程序，并同时运行`tcpSender`程序，效果如下。
![](https://wizardforcel.gitbooks.io/qt-beginning/content/img/38-3.jpg)
我们先在服务器端按下“开始监听”按钮，然后在客户端输入主机地址和端口号，然后打开要发送的文件，点击“发送”按钮进行发送。
## 结语
在这两节里我们介绍了TCP的应用，可以看到服务器端和客户度端都可以当做发送端或者接收端，而且数据的发送与接收只要使用相对应的协议即可，它是可以根据用户的需要来进行编程的，没有固定的格式。《Qt及Qt Quick开发实战精解》中的局域网聊天工具就是本节知识的扩展，大家可以从社区下载页面下载其源码。
涉及到的源码:
- [tcpSender.rar](http://bbs.qter.org/forum.php?mod=attachment&aid=NzgxfGM5NTZlOTVkfDE0NzE3MDA5MTV8MTA5NTl8NjEy)
- [tcpReceiver.rar](http://bbs.qter.org/forum.php?mod=attachment&aid=NzgwfDNhMTgyN2FhfDE0NzE3MDA5MTV8MTA5NTl8NjEy)
