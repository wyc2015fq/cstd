# QT TCP通讯无法收到数据的解决方案 - DumpDoctorWang的博客 - CSDN博客





2017年07月30日 15:19:57[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：2200








好多博客中，有一个很大的误区，就是发送完数据之后，立即调用Socket.disconnectfromHost()，造成数据无法正常的被对方接收到，因此，**应当等待关于这个Socket的所有操作完成之后，才能调用Socket.disConnectFromHost()函数。**

代码

main.cpp

```
#include "mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();

    return a.exec();
}
```

mainwidget.h

```
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QTextCodec>
#include <QDebug>
//#define mDebug(x)     (qDebug()    << "Debug   " + QString(__DATE__).replace(" ",".") + " " + QString(__TIME__) + " " + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" + " " + x)
#define mDebug(x)       (ui->plainTextEdit->appendPlainText("Debug   " + QString(__DATE__).replace(" ",".") + " " + QString(__TIME__) + " " + QString(__FILE__) + " " + QString(__FUNCTION__) + " " + QString::number(__LINE__) + ">>" + " " + x))
namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpClient;
    QTcpSocket *clientConnection;
    QTextCodec *code;

private slots:
    void newConnection();
    void readyRead();
    void readyRead2();
    void on_pushButton_clicked();
};

#endif // MAINWIDGET_H
```

mainwidget.cpp

```
#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    tcpServer = new QTcpServer(this);
    tcpClient = new QTcpSocket(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection()));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readyRead()));
    if(!tcpServer->listen(QHostAddress::Any,6666)){
        mDebug("Unable to linsten port 6666");
        return;
    }
    code = QTextCodec::codecForLocale();

    ui->lineEdit->setText("127.0.0.1");
}

MainWidget::~MainWidget()
{
    delete ui;
    delete tcpServer;
    delete tcpClient;
}
void MainWidget::newConnection(){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out<<"Mess received";

    if(tcpServer->hasPendingConnections()){
        clientConnection = tcpServer->nextPendingConnection();
        connect(clientConnection,SIGNAL(disconnected()),clientConnection,SLOT(deleteLater()));
        connect(clientConnection,SIGNAL(readyRead()),this,SLOT(readyRead2()));

        clientConnection->write(block);
        //clientConnection->disconnectFromHost();
    }
}

void MainWidget::on_pushButton_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_7);
    out<<"test mess";
    tcpClient->abort();
    QString ip  = ui->lineEdit->text();
    tcpClient->connectToHost(ip,6666);
    if(tcpClient->waitForConnected(30000)){
        mDebug("Connected "+QString::number(block.length()));
        tcpClient->write(block);
        //tcpClient->disconnectFromHost();//不能随意调用这个函数，否则会关闭当前连接，对方无法收到数据 
    }
}

void MainWidget::readyRead(){
    mDebug("readyRead");
    QDataStream in(tcpClient);
    in.setVersion(QDataStream::Qt_5_7);
    QString  data;
    QByteArray bytes;
    in.startTransaction();
    in>>bytes;
    data = code->toUnicode(bytes);
    qDebug()<<in.commitTransaction();
    mDebug(data);
}

void MainWidget::readyRead2(){
    mDebug("readyRead2");
    QDataStream in(clientConnection);
    in.setVersion(QDataStream::Qt_5_7);
    QString  data;
    QByteArray bytes;
    in.startTransaction();
    in>>bytes;
    data = code->toUnicode(bytes);
    qDebug()<<in.commitTransaction();
    mDebug(data);
}
```

项目链接：[http://download.csdn.net/detail/dumpdoctorwang/9915415](http://download.csdn.net/detail/dumpdoctorwang/9915415)



