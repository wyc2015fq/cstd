
# 基于Qt的P2P局域网聊天及文件传送软件设计 - zouxy09的专栏 - CSDN博客


2013年06月20日 21:28:54[zouxy09](https://me.csdn.net/zouxy09)阅读数：25259


**基于Qt的P2P局域网聊天及文件传送软件设计**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
这是我的《通信网络》的课程设计作业，之前没怎么学过Qt，但Qt实在太好用了，它提供的网络通信的接口使用起来很方便，所以搞这东西才花了几天时间，现在摆上来和大家分享下。呵呵。另外，下面不会解释具体的代码实现过程（需要有一点点的Qt基础和Qt网络编程基础），在工程的代码文件中会有清晰的注释。
工程代码下载地址：（在CSDN中，我的下载积分是0，穷苦很久了，所以下载积分设地比较高，看懂出对积分的渴望。所以有分的就给点分，没分的就发邮件给我，我再把工程回复给您就好了。原谅我的庸俗。呵呵）
http://download.csdn.net/detail/zouxy09/5621059
**一、设计目标**
通过利用Qt应用程序框架提供的QTcpSocket和QUdpSocket类进行网络通信，在windows平台（支持跨平台，将源码在目标平台重编译即可）上实现了两大功能：
1）实现客户端与服务器端之间文件传输功能；
2）实现客户端与服务器端之间的聊天功能；
**二、系统设计框架**
整个应用程序要实现聊天及文件传输的功能，主要由三大块组成：
  1）人机交互界面：用于显示连接状态、消息传送、实现消息输入等功能；
  2）消息传输模块：用于实现消息传输协议的封装与解包工作，实现消息传输控制；
  3）文件传输模块：用于实现文件传输协议的封装与解包工作，实现文件头及文件的发送与接收控制。
![](https://img-blog.csdn.net/20130620212449281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**三、设计细节**
**3.1、TCP/IP与UDP通信原理**
![](https://img-blog.csdn.net/20130620212516375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
计算机网络通信主要是通过Socket(套接字)进行的。它提供了一个通信端口，可以通过这个端口与任何一个具有Socket接口的计算机通信。应用程序在网络上传输，接收的信息都通过这个Socket接口来实现。在应用开发中就像使用文件句柄一样，可以对Socket句柄进行读，写操作。
Qt提供了较为完整的网络应用服务，可以实现大部分网络应用程序的开发，提供了QTcpSocket和QUdpSocket类，用于实现TCP和UDP传输协议。提供了QTcpServer类创建服务器应用程序来处理引入的TCP连接。
**3.2、聊天系统设计**
聊天也就是信息（字符串）的收发，根据其实时性的要求与短消息传递的特点，采用UDP协议来实现。
在Qt中提供了QUdpSocket 类来进行UDP数据报（datagrams）的发送和接收。UDP的应用是很简单的。我们只需要在发送端执行writeDatagram()函数将信息发送到广播地址的某一个端口，然后接收端绑定到这个端口，只要这个端口有数据，就会发送readyRead()信号，我们接收到该信号后进行数据处理即可。
**（1）客户端**
/***** 创建Udp套接字和绑定端口与槽函数 *********/
QUdpSocket *UdpSender = new QUdpSocket(this);
QUdpSocket *UdpReader = new QUdpSocket(this);
// 将UdpReader绑定到广播地址的5566端口，接收发到这个端口的信息
UdpReader->bind(5566, QUdpSocket::ShareAddress);
// 将有数据的readyRead()信号绑定到读信息的readMessage()槽函数
connect(UdpReader, SIGNAL(readyRead()), this, SLOT(readMessage()));
/***** 发送消息 *********/
void sendMessage()
{
QByteArray datagram = “hello world!”;
UdpSender ->writeDatagram(datagram.data(),datagram.size(),
QHostAddress::Broadcast,5566);
}
/***** 接收消息 *********/
void readMessage()
{
//拥有等待的数据报
while(UdpReader->hasPendingDatagrams())
{
QByteArray datagram; //拥于存放接收的数据报
//让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
datagram.resize(UdpReader->pendingDatagramSize());
//接收数据报，将其存放到datagram中
UdpReader->readDatagram(datagram.data(), datagram.size());
serverMessage = datagram;
}
}
**（2）服务器端**
过程与代码和客户端一致，只是因为是双向通信，所以在服务器端需要发送的端口号要不一样。具体见工程代码。
**3.3、文件传输设计**
文件传送具有连续性和可靠性要求的特点，通常采用点对点通信，并应用TCP服务来保证数据传输过程中的准确性和顺序性。
TCP协议的程序使用的是客户端/服务器模式，在Qt中提供了QTcpSocket类来编写客户端程序，使用QTcpServer类编写服务器端程序。我们在服务器端进行端口的监听，一旦发现客户端的连接请求，就会发出newConnection()信号，我们可以关联这个信号到我们自己的槽函数，实现与客户端的连接。建立连接后，任何一方都可以进行数据的发送。另一端一旦有数据到来就会发出readyRead()信号，我们可以关联此信号，进行数据的接收。
因为涉及到文件的拆分和增加文件头信息等，还需要管理发送和接收进度条，代码比较复杂，这里就不贴了，具体见工程代码，代码都有比较清晰的注释。
**四、人机交互界面设计**
如图4-1所示，交互界面包含3大功能块：
1）聊天窗口：包括发送和接收到的信息显示框和信息的输入框；
2）文件收发：包括需要填写的建立TCP连接的服务器信息框和文件传输需要的功能和显示进度框；
3）附加功能：附加“消息记录”和“使用帮助”的对话框，点击对应按钮接口打开。![](https://img-blog.csdn.net/20130620212602984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图4-1：客户端界面
服务器端界面和客户端界面差别只在于“服务器信息”中的“建立会话”按钮改为“开始监听”按钮，因为TCP连接是由客户端申请，由服务器来监听的。
![](https://img-blog.csdn.net/20130620212634796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图4-2：服务器端界面
工程还实现了历史消息界面和帮助界面。
**五、附录代码**
**5.1、工程目录**
![](https://img-blog.csdn.net/20130620212655421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
左边是客户端的项目文件，右边是服务器端的项目文件。
**工程说明：**
注：加入Dialog后，需要先在 本项目名字 右键，选择 执行qmake，再执行run才可以编译通过，否则会出现 Dialog的连接错误 LNK2019
**5.2、服务器端的两个主要的代码（客户端大部分相同）**
**MainWindow.h**
```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtNetwork>
#include "Dialog.h"
#include "hismesdialog.h"
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    /********** Send and receive message though UCP protocol **************/
    void sendMessage(); // 发送消息
    void readMessage(); // 接受信息
    void showMessage(bool isSend); // 显示消息
    /********** Send and receive files though TCP protocol **************/
    void acceptConnection();  // 接收客户端请求
    void listening(); // 监听
    void displayError(QAbstractSocket::SocketError); //显示错误
    /************* Send file **********************/
    void openFile();  //打开文件
    void startTransferFile();  //发送文件大小等信息
    void updateClientProgress(qint64); //发送数据，更新进度条
    /************* Receive file *******************/
    void updateServerProgress(); //接收数据，更新进度条
    /************************* Button Event ****************************/
    void on_openButton_clicked();
    void on_listenButton_clicked();
    void on_disconectButton_clicked();
    void on_sendMesButton_clicked();
    void on_sendFileButton_clicked();
    void on_helpButton_clicked();
    void on_mesHistoryButton_clicked();
    void on_quitButton_clicked();
    void on_textBrowser_textChanged();
private:
    Ui::MainWindow *ui;
    /********** Send and receive message though UCP protocol **************/
    QUdpSocket *UdpSender;
    QUdpSocket *UdpReader;
    QString localMessage; // 存放本地要发送的信息
    QString serverMessage;  //存放从服务器接收到的信息
    /********** Send and receive files though TCP protocol **************/
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    /************* Send file **********************/
    quint16 blockSize;  //存放接收到的信息大小
    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据
    /************* Receive file *******************/
    //qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    //QString fileName;   //存放文件名
    //QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区
    Dialog *helpDialog;
    hisMesDialog *histroyMessageDialog;
    QFile *historyMessage;
    QString hostIP;
    QString getHostIP();
};
#endif // MAINWINDOW_H
```

**MainWindow.cpp**
```cpp
// Chat and file Transceiver
// Author : Zouxy
// Date   : 2013-6-20
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QObject>
#include <QMessageBox>
#include <QTextCodec>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /********** Send and receive message though UCP protocol **************/
    UdpSender = new QUdpSocket(this);
    UdpReader = new QUdpSocket(this);
    // 将UdpReader绑定到广播地址的6666端口，接收发到这个端口的信息
    UdpReader->bind(6666, QUdpSocket::ShareAddress);
    connect(UdpReader, SIGNAL(readyRead()), this, SLOT(readMessage()));

    /********** Send and receive files though TCP protocol **************/
    loadSize = 4*1024;  //将整个大的文件分成很多小的部分进行发送，每部分为4字节
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    bytesReceived = 0;
    fileNameSize = 0;
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);
    //当发现新连接时发出newConnection()信号
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    ui->sendFileButton->setEnabled(false); //开始使”发送文件“按钮不可用
    ui->sendMesButton->setEnabled(true); //开始使”发送信息“按钮可用
    // 服务器的缺省值
    hostIP = getHostIP();
    if (hostIP == 0)
        ui->hostLineEdit->setText("127.0.0.1");
    else
        ui->hostLineEdit->setText(hostIP);
    ui->portLineEdit->setText("5555");
    helpDialog = new Dialog;
    helpDialog->hide();
    histroyMessageDialog = new hisMesDialog;
    histroyMessageDialog->hide();
    historyMessage = new QFile("historyMessage.txt");
    if(!historyMessage->open(QFile::Append))
    {
        qDebug() << "writing history Message file error!";
        return;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
// 监听
void MainWindow::listening()
{
    //初始化已发送字节为0
    bytesWritten = 0;
    bytesReceived = 0;
    blockSize = 0; //初始化其为0
    ui->listenButton->setEnabled(false);
    if(!tcpServer->listen(QHostAddress::LocalHost, ui->portLineEdit->text().toInt()))
    {
        qDebug() << tcpServer->errorString();
        close();
        return;
    }
    ui->currentStatusLabel->setText("Status: listening...");
}
// 连接后的对话框提示
void MainWindow::acceptConnection()
{
    //QMessageBox box_success;
    //box_success.information(NULL, "提示", "会话建立成功", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    tcpSocket = tcpServer->nextPendingConnection();
    // 当有数据发送成功时，我们更新进度条
    connect(tcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(updateClientProgress(qint64)));
    // 当有数据接收成功时，我们更新进度条
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(updateServerProgress()));
    // 绑定错误处理
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    tcpServer->close();
    ui->currentStatusLabel->setText("Status: Waiting for file transfer...");
}
// 发送信息
void MainWindow::sendMessage()
{
    localMessage = ui->inputTextEdit->toPlainText();
    QByteArray datagram = localMessage.toUtf8();
    //进行UDP数据报（datagrams）的发送
    UdpSender->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 5566);
    // 显示消息
    showMessage(true);
}
// 接收信息
void MainWindow::readMessage()
{
    //拥有等待的数据报
    while(UdpReader->hasPendingDatagrams())
    {
        QByteArray datagram; //拥于存放接收的数据报
        //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        datagram.resize(UdpReader->pendingDatagramSize());
        //接收数据报，将其存放到datagram中
        UdpReader->readDatagram(datagram.data(), datagram.size());
        //将数据报内容显示出来
        serverMessage = datagram;
    }
    // 显示消息
    showMessage(false);
}
// 显示消息
void MainWindow::showMessage(bool isSend)
{
    QDateTime time = QDateTime::currentDateTime();  //获取系统现在的时间
    QString str = time.toString("hh:mm:ss ddd");     //设置显示格式
    QString str4file = time.toString("yyyy-MM-dd hh:mm:ss");     //设置显示格式
    blockSize = 0;
    QFont font;
    font.setPixelSize(18);
    ui->textBrowser->setFont(font);
    QTextStream stream(historyMessage);
    if (isSend)
    {
        // 用不同的颜色显示信息所属和当前时间
        ui->textBrowser->setTextColor(QColor(0, 0, 0));
        QString entraMess = "I say: " + str;
        ui->textBrowser->append(entraMess);
        stream<<"I say: "<<str4file<<"\n"; // 写入 历史信息 文件时需要保存日期
        ui->textBrowser->setTextColor(QColor(0, 0, 255));
        ui->textBrowser->append(localMessage);
        ui->inputTextEdit->clear();
        ui->currentStatusLabel->setText("Status: send message successfully...");
        stream<<localMessage<<"\n";
    }
    else
    {
        // 用不同的颜色显示信息所属和当前时间
        ui->textBrowser->setTextColor(QColor(0, 0, 0));
        QString entraMess = "He/she: " + str;
        ui->textBrowser->append(entraMess);
        stream<<"He/she: "<<str4file<<"\n";
        ui->textBrowser->setTextColor(QColor(255, 0, 0));
        ui->textBrowser->append(serverMessage);
        ui->currentStatusLabel->setText("Status: new message coming...");
        stream<<localMessage<<"\n";
    }
}
// 打开文件
void MainWindow::openFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        ui->sendFileButton->setEnabled(true);
        QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
        ui->currentStatusLabel->setText(tr("Status: open file %1 successfully!").arg(currentFileName));
    }
}
// 实现文件大小等信息的发送
void MainWindow::startTransferFile()
{
    localFile = new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug() << "open file error!";
        return;
    }
    totalBytes = localFile->size(); //文件总大小
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_0);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    //依次写入总大小信息空间，文件名大小信息空间，文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;
    //这里的总大小是文件名大小等信息和实际文件大小的总和
    totalBytes += outBlock.size();
    //返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size() - sizeof(qint64)*2));
    //发送完头数据后剩余数据的大小
    bytesToWrite = totalBytes - tcpSocket->write(outBlock);
    ui->currentStatusLabel->setText("Status: start transfering...");
    outBlock.resize(0);
}
// 更新进度条，实现文件的传送
void MainWindow::updateClientProgress(qint64 numBytes)
{
    //已经发送数据的大小
    bytesWritten += (int)numBytes;
    if(bytesToWrite > 0) //如果已经发送了数据
    {
        //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，就发送剩余数据的大小
        outBlock = localFile->read(qMin(bytesToWrite, loadSize));
        //发送完一次数据后还剩余数据的大小
        bytesToWrite -= (int)tcpSocket->write(outBlock);
        //清空发送缓冲区
        outBlock.resize(0);
    }
    else
    {
        //如果没有发送任何数据，则关闭文件
        localFile->close();
    }
    //更新进度条
    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesWritten);
    if(bytesWritten == totalBytes) //发送完毕
    {
        QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
        ui->currentStatusLabel->setText(tr("Status: transfer file %1 ...").arg(currentFileName));
        localFile->close();
        bytesWritten = 0; // clear fot next send
        bytesToWrite = 0;
        totalBytes = 0;
        //tcpSocket->close();
    }
}
// 更新进度条，实现文件的接收
void MainWindow::updateServerProgress()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_0);
    //如果接收到的数据小于等于16个字节，那么是刚开始接收数据，我们保存为头文件信息
    if(bytesReceived <= sizeof(qint64)*2)
    {
        //接收数据总大小信息和文件名大小信息
        if((tcpSocket->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0))
        {
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        //接收文件名，并建立文件
        if((tcpSocket->bytesAvailable() >= fileNameSize) && (fileNameSize != 0))
        {
            in >> fileName;
            ui->currentStatusLabel->setText(tr("Accept file %1 ...").arg(fileName));
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug() << "writing file error!";
                return;
            }
        }
        else
            return;
    }
    //如果接收的数据小于总数据，那么写入文件
    if(bytesReceived < totalBytes)
    {
        bytesReceived += tcpSocket->bytesAvailable();
        inBlock = tcpSocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    //更新进度条
    ui->serverProgressBar->setMaximum(totalBytes);
    ui->serverProgressBar->setValue(bytesReceived);
    //接收数据完成时
    if(bytesReceived == totalBytes)
    {
        //tcpSocket->close();
        localFile->close();
        ui->listenButton->setEnabled(true);
        ui->currentStatusLabel->setText(tr("Status: receive file %1 successfully!").arg(fileName));
        bytesReceived = 0; // clear for next receive
        totalBytes = 0;
        fileNameSize = 0;
    }
}
// 显示错误
void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
    tcpSocket->close();
    ui->serverProgressBar->reset();
    ui->currentStatusLabel->setText(tr("Status: connect error! open client and retry"));
    ui->listenButton->setEnabled(true); //开始使”监听“按钮可用
    ui->sendFileButton->setEnabled(false);
}
// 开始监听
void MainWindow::on_listenButton_clicked()
{
    listening();
}
// 断开连接
void MainWindow::on_disconectButton_clicked()
{
    tcpSocket->disconnectFromHost();
    ui->listenButton->setEnabled(true); //开始使”监听“按钮可用
    ui->sendFileButton->setEnabled(false); //开始使”发送文件“按钮不可用
}
// 发送信息
void MainWindow::on_sendMesButton_clicked()
{
    sendMessage();
}
// 打开文件
void MainWindow::on_openButton_clicked()
{
    openFile();
}
// 发送文件
void MainWindow::on_sendFileButton_clicked()
{
    startTransferFile();
}
// 帮助说明
void MainWindow::on_helpButton_clicked()
{
    helpDialog->show();
}
// 消息记录
void MainWindow::on_mesHistoryButton_clicked()
{
    histroyMessageDialog->showHisMesDialog();
    histroyMessageDialog->show();
}
// 关闭程序
void MainWindow::on_quitButton_clicked()
{
    this->close();
}
// 文字多了会增加滚动条并显示最后的信息
void MainWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
QString MainWindow::getHostIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        //我们使用IPv4地址
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}
```


