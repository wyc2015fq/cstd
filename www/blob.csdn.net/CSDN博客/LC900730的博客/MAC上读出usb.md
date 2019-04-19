# MAC上读出usb - LC900730的博客 - CSDN博客
2017年08月18日 18:40:52[lc900730](https://me.csdn.net/LC900730)阅读数：132
[原文链接](http://blog.csdn.net/luoyhang003/article/details/50591886)
mac上使用qt平台开发，读串口的话有QSerialPortinfo、QSerialPort 
但是mac上没有串口，只有usb口，与串口通信也是通过usb口。使用libusb库
## 一、安装libusb
正常源码编译安装即可。
## 二、libusb动态库加入QT项目
QT项目中新建文件夹lib。，将/usr/local/lib/libusb-1.0.0.dylib复制到lib/下。 
将第一步解压出来的libusb-1.0.21/libusb/libusb.h拷贝到Qt项目中lib目录下。
项目中.pro文件，加入 
LIBS +=\ 
    -L$$PWD/lib/ -lusb-1.0.0
## 测试代码如下
getusbinfo.h
```
#ifndef GETUSBINFO
#define GETUSBINFO
#include <QString>
#include <QObject>
#include <QList>
#include <QThread>
#include <libusb.h>
struct STUUSBDevices{
    QString idProduct;
    QString idVendor;
    QString iManufacturer;
    QString iSerialNumber;
};
class GetUsbInfo :  QThread{
public:
    GetUsbInfo(QObject *parent);
    ~GetUsbInfo();
    int initUsbDevices();
    QString getVidPid(libusb_device **devs);
    void showAllUsbDevices(QList<STUUSBDevices> lst);
    void setRunStatus();
    void run();
    bool isStop;
};
#endif // GETUSBINFO
```
getusbinfo.cpp
```
#include "getusbinfo.h"
#include <QThread>
#include <QDebug>
#include <QString>
GetUsbInfo::GetUsbInfo(QObject *parent) :
    QThread(parent),isStop(false)
{
}
GetUsbInfo::~GetUsbInfo()
{
    qDebug()<<"GetUsbInfo::~GetUsbInfo "<<endl;
}
int GetUsbInfo::initUsbDevices()
{
    libusb_device **devs;
    int r;
    ssize_t cnt;
    r = libusb_init(NULL);
    if (r < 0)
        return r;
    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return (int) cnt;
    getVidPid(devs);
    libusb_free_device_list(devs, 1);
    libusb_exit(NULL);
    return 0;
}
QString GetUsbInfo::getVidPid(libusb_device **devs)
{
    libusb_device *dev;
    int i = 0;
    QList<STUUSBDevices> lstUsb;
    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            qDebug()<<"failed to get device descriptor"<<stderr;
            return "";
        }
        printf("%04x:%04x (bus %d, device %d)\n",
            desc.idVendor, desc.idProduct,
            libusb_get_bus_number(dev), libusb_get_device_address(dev));
        STUUSBDevices stu;
        stu.idProduct = QString::number(desc.idProduct);
        stu.idVendor = QString::number(desc.idVendor);
        stu.iManufacturer = QString::number(desc.iManufacturer);
        stu.iSerialNumber = QString::number(desc.iSerialNumber);
        lstUsb.append(stu);
    }
    showAllUsbDevices(lstUsb);
    return QString(lstUsb[0].idProduct);
}
void GetUsbInfo::showAllUsbDevices(QList<STUUSBDevices> lst)
{
    for(int i=0;i<lst.count();i++)
    {
        qDebug()<<"vid: "<<lst.at(i).idVendor<<"\n"
               <<"pid:"<<lst.at(i).idProduct<<"\n"
                <<"serNumber:"<<lst.at(i).iSerialNumber<<"\n"
                  <<"Manufacturer:"<<lst.at(i).iManufacturer<<"\n";
    }
}
void GetUsbInfo::setRunStatus()
{
    isStop = true;
}
void GetUsbInfo::run()
{
    qDebug()<<"GetUsbInfo::run() "<<endl;
    while (!isStop)
    {
       initUsbDevices();
       sleep(10);
    }
}
```
main.cpp
```
#include "mainwindow.h"
#include <QApplication>
#include "getusbinfo.h"
#include <QLibrary>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QThread t;
    GetUsbInfo info(&t);
    info.initUsbDevices();
    return a.exec();
}
```
