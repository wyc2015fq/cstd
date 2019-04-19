# Qt基于networkmanager嵌入式linux的wifi网络连接 - xqhrs232的专栏 - CSDN博客
2019年02月11日 11:14:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：86
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/p154613730/article/details/85039290](https://blog.csdn.net/p154613730/article/details/85039290)
相关文章
1、[QT---Native Wifi functions 应用(WiFi有密码连接)](https://www.cnblogs.com/findumars/p/6294156.html)----[https://www.cnblogs.com/findumars/p/6294156.html](https://www.cnblogs.com/findumars/p/6294156.html)
2、qt界面连接wifi----[https://download.csdn.net/download/qq_41399894/10950658](https://download.csdn.net/download/qq_41399894/10950658)
3、Qt 嵌入式 利用wpa_supplicant编写WIFI、有线网络管理器----[https://blog.csdn.net/dr_abel/article/details/51335599](https://blog.csdn.net/dr_abel/article/details/51335599)
4、基于QT界面的嵌入式wifi操作----[https://blog.csdn.net/wxxgoodjob/article/details/83376896](https://blog.csdn.net/wxxgoodjob/article/details/83376896)
5、Qt实现探测当前有没有网络连接（Wi-Fi)----[https://blog.csdn.net/z_jinye/article/details/38908285](https://blog.csdn.net/z_jinye/article/details/38908285)
6、[Qt ------ 获取 wifi 信息](https://www.cnblogs.com/god-of-death/p/7800708.html)----[https://www.cnblogs.com/god-of-death/p/7800708.html](https://www.cnblogs.com/god-of-death/p/7800708.html)
嵌入式linux管理网络的工具主要有wpa_supplicant 和networkmanager两种方式，它们都各自的工具来管理wifi网络，具体实现细节网上都有很详细的介绍。接下来要介绍的主要是嵌入式linux下使用networkmanager工具来实现wifi网络连接,本人使用的是友善之臂的nanopi S2开发板，已经移植好networkmanager工具。
关于NetworkManager
NetworkManager有自己的CLI工具：nmcli。使用nmcli用户可以查询网络连接的状态，也可以用来管理。优点：原始，语法相对简单；在CLI中使用NetworkManager很容易掌握。
常用命令：
开启wifi
nmcli r wifi on 
扫描附近wifi热点
nmcli dev wifi
连接到指定wifi热点
nmcli dev wifi connect "SSID" password "PASSWORD" ifname wlan0
查看VPN
nmcli con
连接VPN
nmcli con up id YOUR_VPN_ID
断开VPN
nmcli con down id YOUR_VPN_ID
实现方法
借助上面nmcli工具只需要3步即可通过终端连接一个wifi网络：
开启wifi->扫描wifi热点->连接到指定wifi热点。
那实际上我们只需要借助qt实现类似windows系统下wifi连接的可视化操作界面，具体分为两个步骤：
1.显示wifi列表信息，按照wifi的信号强度排序，预留wifi连接接口。
2.选择指定的wifi，输入wifi密码，连接wifi。
system("sudo nmcli r wifi on");                              //开启wifi
system("sudo nmcli dev wifi > /Settings/WiFi/wifi.ini");     //把扫描到的wifi信息输出到指定文件   
//system("sudo nmcli con > /Settings/WiFi/wifiCon.ini");     //显示当前连接的wifi网络
上面通过调用系统命令开启wifi，并把扫描到的wifi信息定向输出到/Setting/WiFi/wifi.ini文件中，通过解析wifi.ini文件里面的内容来显示wifi列表信息，并依据wifi信号强度进行排序，测试得到wifi.ini里面的内容如下：
文件里面很清晰的显示了wifi的相关属性信息，那我们接下来需要做的就是解析这个文件:
头文件：
#ifndef WIFICONNECTTEST_H
#define WIFICONNECTTEST_H
#include <QWidget>
#include <QFile>
#include <QDebug>
#include <QGridLayout>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>
#include <QNetworkConfigurationManager>
#include <QDesktopWidget>
#include <QLabel>
#include "inputpassworddialog.h"
#define WIFINUM 10                              //最多显示10个wifi
class WifiConnectTest : public QWidget
{
    Q_OBJECT
public:
    WifiConnectTest(QWidget *parent = 0);
    ~WifiConnectTest();
private slots:
    void wifiListInit();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    QLabel *iconLabel[WIFINUM];                 //显示wifi信号强度
    QLabel *connectLabel[WIFINUM];              //显示wifi连接状态
    QLabel *wifiNameLabel[WIFINUM];             //wifi名
    QWidget *wifiWidget[WIFINUM];               //包含wifi成员的widget
    QPushButton *connectButton[WIFINUM];        //wifi连接按钮
};
#endif // WIFICONNECTTEST_H
#include "wificonnecttest.h"
const QString wifiStrList = QString("/Settings/WiFi/wifi.ini");
WifiConnectTest::WifiConnectTest(QWidget *parent)
    : QWidget(parent)
{
    this->wifiListInit();
    QDesktopWidget desktop;
    this->setFixedSize(desktop.width()/2,desktop.height());
}
WifiConnectTest::~WifiConnectTest()
{
}
void WifiConnectTest::wifiListInit()
{
    QFile file(wifiStrList);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"No such file!";
        return;
    }
    int curLine = 0;
    QString line;
    QVBoxLayout *vLayout = new QVBoxLayout;
    if(file.size()!=0){
        QTextStream in(&file);
        QString signalFlag = "SIGNAL";      //表示wifi信号字符
        QString wifiFlag = "SSID";          //wifi名字符
        QString modeFlag = "MODE";          //wifi连接模式
        line =  in.readLine();
        if(!line.isEmpty()){
            int signalIndex = line.indexOf(signalFlag);     //文件对应的wifi信号强度索引值
            int wifiIndex = line.indexOf(wifiFlag);         //文件对应的wifi名索引值
            int modeIndex = line.indexOf(modeFlag);
            while (curLine < WIFINUM) {
                line =  in.readLine();
                QString tmpStr = line;
                QString curConnection = line.left(1);       //已连接wifi判断
                QString wifiName = line.mid(wifiIndex,(modeIndex-wifiIndex)).simplified();  //获取wifi名称
                int signalValue = tmpStr.mid(signalIndex,3).simplified().toInt();           //获取wifi的信号强度
                if(!wifiName.isEmpty()){
                    QGridLayout *gridLayout = new QGridLayout();
                    wifiWidget[curLine] = new QWidget(this);                                //wifi对应的属性栏
                    wifiWidget[curLine]->setObjectName(QString("wifiName*%1*%2").arg(QString::number(curLine)).arg(wifiName));
                    wifiWidget[curLine]->installEventFilter(this);                          //安装事件过滤器，选中wifi后弹出连接wifi的接口按钮
                    wifiWidget[curLine]->setStyleSheet(QString("QWidget::hover{background-color:darkgray}").append(QString("QWidget{background-color:transparent;}")));
                    wifiNameLabel[curLine] = new QLabel(wifiWidget[curLine]);
                    wifiNameLabel[curLine]->setText(wifiName);                              //显示wifi名
                    QPixmap pix = QPixmap(60,40);       //根据wifi信号强度绘制wifi信号
                    pix.fill(Qt::transparent);
                    QPainter paint(&pix);
                    QPen pen = QPen();
                    pen.setColor(Qt::darkGreen);
                    pen.setWidth(4);
                    paint.setPen(pen);
                    for(int k=2;k<7;k++){
                        if(k == (signalValue/20+3)){
                            QPen penGray = QPen();
                            penGray.setColor(Qt::gray);
                            penGray.setWidth(4);
                            paint.setPen(penGray);
                        }
                        paint.drawLine(8*k,48-7*k,8*k,35);
                    }
                    paint.end();
                    iconLabel[curLine] = new QLabel(wifiWidget[curLine]);
                    iconLabel[curLine]->setPixmap(pix); //添加wifi信号强度图标
                    connectLabel[curLine] = new QLabel(wifiWidget[curLine]);
                    if(curConnection=="*")              //wifi连接状态，wifi名前如果有"*",表示当前wifi处于连接状态
                        connectLabel[curLine]->setText("已连接");
                    connectButton[curLine] = new QPushButton(wifiWidget[curLine]);
                    connectButton[curLine]->setStyleSheet(QString("QPushButton{background-color:lightgray;}").append(QString("QPushButton::pressed{background-color:darkgray;}")));
                    connectButton[curLine]->setText(tr("连接"));
                    connectButton[curLine]->setVisible(false);
                    gridLayout->addWidget(wifiNameLabel[curLine],0,0,1,3);      //在wifi属性框中添加wifi对应的属性信息
                    gridLayout->addWidget(connectLabel[curLine],0,3,1,1);
                    gridLayout->addWidget(iconLabel[curLine],0,4,1,1);
                    gridLayout->addWidget(connectButton[curLine],1,3,1,1);
                    gridLayout->setMargin(0);
                    gridLayout->setSpacing(0);
                    wifiWidget[curLine]->setLayout(gridLayout);
                    vLayout->addWidget(wifiWidget[curLine]);
                    vLayout->setMargin(2);
                    vLayout->setSpacing(0);
                }
                curLine++;
            }
        }
    }
    file.close();
    this->setLayout(vLayout);
}
wifi信息的解析通过上面wifiListInit()来实现，主要是找准wifi.ini文件对应的wifi名以及wifi信号强度的位置信息，并根据信号强度值绘制wifi信号图。最终生成的wifi列表如下图：
上面已经完成了第一步，wifi列表信息的显示，接着第二步模仿windows下wifi连接，选中wifi后显示连接wifi的接口按钮，并输入wifi密码，连接wifi。具体实现方法如下：
bool WifiConnectTest::eventFilter(QObject *obj, QEvent *event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if(mouseEvent->type() == QEvent::MouseButtonPress){
        if(mouseEvent->button()==Qt::LeftButton){
            if(obj->objectName().contains("wifiName")){
                QString wifiName = obj->objectName().section('*',-1);
                static int curLine = 0;
                connectButton[curLine]->setVisible(false);                  //隐藏所有wifi连接button
                curLine = obj->objectName().section('*',1,1).toInt();
                connectButton[curLine]->setVisible(true);                   //选中当前wifi属性框后，显示wifi连接按钮
                connect(connectButton[curLine],&QPushButton::clicked,[=](){
                    InputPasswordDialog *inputPassword = new InputPasswordDialog();     //实例化wifi密码输入窗口
                    inputPassword->show();
                    connect(inputPassword,&InputPasswordDialog::confirmPassword,[=](QString password){
                        QString connectNetWorkStr = QString("nmcli dev wifi connect %1 password %2");
                        QByteArray array = connectNetWorkStr.arg(wifiName).arg(password).toLatin1();
                        char *cmdStr = array.data();        //QString 转 char*
                        if(-1!=system(cmdStr)){
                            QNetworkConfigurationManager mgr;
                            if(mgr.isOnline()){             //wifi已连接
                                qDebug("wifi is online!");
                                connectButton[curLine]->setText(tr("已连接"));
                            }
                            else{
                                qDebug("wifi is offline!");
                                connectButton[curLine]->setText(tr("offline"));
                            }
                        }
                    });
                });
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}
效果图如下：
好了，到这儿wifi的连接基本上已经实现了，至于后续密码输入（输入法实现）就是各位看官自个儿去实现了。当然，到目前为止仅仅只是讲到了基于networkmanager工具实现一个wifi热点的连接，在关于networkmanager上也提到过，networkmanager工具是可以查询当前连接状态的，能够保存连接过的wifi网络信息，通过nmcli con命令可以查看连接过的wifi列表，通过命令nmcli con up id YOUR_VPN_ID可以直接连接。总的来说，通过networkmanager工具来实现wifi网络管理还是比较方便的，实现语法简单，只是相当于用qt简单封装了一次，实现了可视化操作。
完整测试Demo下载地址：https://download.csdn.net/download/p154613730/10854608
