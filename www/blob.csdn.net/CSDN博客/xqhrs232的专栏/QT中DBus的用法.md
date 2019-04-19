# QT中DBus的用法 - xqhrs232的专栏 - CSDN博客
2016年06月15日 16:13:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：643
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/lvliang2008/article/details/6304003](http://blog.csdn.net/lvliang2008/article/details/6304003)
相关文章
1、QT中DBus的用法----[http://wenku.baidu.com/link?url=7VwbYegN0A8kJFmCTP7U2EKqiFBH48LJuW8pTwSJA91c-BYjEQPYII6ifhtCTbXVHmYRujlxx7Q8bfVTmU4ppgaSm-dJ_DSZIuicsPyFkem](http://wenku.baidu.com/link?url=7VwbYegN0A8kJFmCTP7U2EKqiFBH48LJuW8pTwSJA91c-BYjEQPYII6ifhtCTbXVHmYRujlxx7Q8bfVTmU4ppgaSm-dJ_DSZIuicsPyFkem)
2、[DBus 介绍](http://blog.chinaunix.net/uid-23381466-id-58574.html) ----[http://blog.chinaunix.net/uid-23381466-id-58574.html](http://blog.chinaunix.net/uid-23381466-id-58574.html)
目  录
1 概述... 2
2 QT中Dbus的最简单的用法... 3
3 QT中DBus的常规用法... 4
3.1 客户端的用法1：使用QDBusMessage. 4
3.2客户端的用法2：使用DBusInterface. 4
3.3 客户端的用法3：使用DBusProxy. 5
3.4服务端的用法1：直接注册对象和服务... 6
3.5 服务端的用法2：使用DBusAdapter 6
4 QT中Dbus的特殊的用法(使用QT的信号) 7
# 1 概述
D-Bus是一种高级的进程间通信机制，它由freedesktop.org项目提供，使用GPL许可证发行。D-Bus最主要的用途是在Linux桌面环境为进程提供通信，同时能将Linux桌面环境和Linux内核事件作为消息传递到进程。D-Bus的主要概率为总线，注册后的进程可通过总线接收或传递消息，进程也可注册后等待内核事件响应，例如等待网络状态的转变或者计算机发出关机指令。目前，D-Bus已被大多数Linux发行版所采用，开发者可使用D-Bus实现各种复杂的进程间通信任务。
D-Bus是一个消息总线系统，其功能已涵盖进程间通信的所有需求，并具备一些特殊的用途。D-Bus是三层[架构](http://lib.csdn.net/base/16)的进程间通信系统，其中包括：
接口层：接口层由函数库libdbus提供，进程可通过该库使用D-Bus的能力。
总线层：总线层实际上是由D-Bus总线守护进程提供的。它在Linux系统启动时运行，负责进程间的消息路由和传递，其中包括Linux内核和Linux桌面环境的消息传递。
包装层：包装层一系列基于特定应用程序框架的Wrapper库。
在QT中的Dbus是使用的Dbus的包装层libdbus-qt.
要查看Dbus总线上的服务和对象可以借助d-feet 和qdbusviewer
要发送信号可以使用dbus-send,要查看Dbus上的消息流可以使用dbus-monitor
QT Dbus是在QT4.2中才引进到QT中来的，还有很多的地方不是很完善，在网上DBus的资料比较多，不过很多都是基入GTK的，基入QT的资料还比较的少，主要可以参考
[D-Bus和QT4](http://www.cnblogs.com/wwang/archive/2010/10/27/1862552.html) （[wwang's
 blog](http://www.cnblogs.com/wwang/)一蓑烟雨任平生）
[http://www.cnblogs.com/wwang/archive/2010/10/27/1862552.html](http://www.cnblogs.com/wwang/archive/2010/10/27/1862552.html)
其他的DBus的基本资料可以参考：
DBus学习笔记（博客园）
[http://dotnet.cnblogs.com/page/76759/?page=1](http://dotnet.cnblogs.com/page/76759/?page=1)
恺风的CSDN博客
[http://blog.csdn.net/flowingflying/archive/2009/09/07/4527634.aspx](http://blog.csdn.net/flowingflying/archive/2009/09/07/4527634.aspx)
# 2 QT中Dbus的最简单的用法
       关于QT中的DBus的用法介绍的文章比较的少，网上只有这篇文章（国外的网站上可能有更多，只是没有发现而已）和QT中的四个例子程序。
这个最简单的用法是在使用中发现的，不需要在Dbus的守护进程上注册服务和注册对象。
注册了对象后：对象中的导出的槽就可以供其他的客户端来调用了。
注册了服务后：对象就有了一个公共名，没有注册是只有三个唯一名。
在发送端只需创建一个信号原后发送即可，
例如：
1)        创建QT的DBus信号
QDBusMessage msg =QDBusMessage::createSignal("/hotel/path",  "hotel.interface", "checkIn");
2)        给信号赋值  
msg<<this->ui->checkInlineEdit->text().toInt();
3)        发射信号
    QDBusConnection::sessionBus().send(msg);
在接受端同样只需要简单的两三步即可
1)        绑定信号
QDBusConnection::sessionBus().connect(QString(),QString(),"dbus.client. interface","Active",this,SLOT(ActiveEvent(int)));
2)        在槽中进行相关的处理
在ActiveEvent槽中执行你的应用程序需要对这个信号做出的反应即可。
这样的使用在DBUS的守护进程中没有服务，只有三个唯一名，没有公共名，也没有任何的信号、槽、属性。
可以通过d-feet、 qdbusviewe来查看。
可以通过dbus-monitor监视信号发送过程
可以参考示例DBusSignalMethod中DBusServerDBusSignal和DBusClientDBusSignal
# 3 QT中DBus的常规用法
在以下这篇文章中已经很好的介绍了QT DBus的各种常规的用法了，在此就重复了，这种用法主要是在服务端，需要在DBus的守护进程上注册对象和注册服务，原后其他的客户端就可以自由的调用了。
不过这种方式也有一个好处，调用是服务端的返回的参数就只发给调用的客户端，是一对一的，不想上面的用信号的方式，信号的方式，发送的信号是一对多的。
D-Bus和QT4 （wwang's blog一蓑烟雨任平生）
http://www.cnblogs.com/wwang/archive/2010/10/27/1862552.html
在也有几个小的例子程序是根据D-Bus和QT4 （wwang's blog一蓑烟雨任平生）的例子改编的。
在常规用法中也分客户端和服务端的，
客户端有三种用法，服务端有两个用法。
## 3.1 客户端的用法1：使用QDBusMessage
可以参考实例DBusMessage中的DBusServer和DBusClient
1)        创建一个QDBusMessage的方法调用
a)         QDBusMessage m = QDBusMessage::createMethodCall("hotel.server",
                                                                                                            i.              "/hotel/path",
                                                                                                          ii.              "hotel.interface",
                                                                                                        iii.              "checkIn");
2)        给方法传递参数
a)         m<<this->ui->checkInlineEdit->text().toInt();
3)        调用方法
a)         QDBusMessage response = QDBusConnection::sessionBus().call(m);
4)        判断方法的返回值
if (response.type() == QDBusMessage::ReplyMessage) {
// QDBusMessage的arguments不仅可以用来存储发送的参数，也用来存储返回值;
            // 这里取得checkIn的返回值
            int num_room = response.arguments().takeFirst().toInt();
            qDebug("Got %d %s/n", num_room, (num_room > 1) ? "rooms" : "room");
    } else
    {
            qDebug( "Check In fail!/n");
    }
## 3.2客户端的用法2：使用DBusInterface
可以参考实例DBusInterface中的DBusServer和DBusClientInterface
1)        创建一个QDBusInterface的实例
QDBusInterface iface( "hotel.server",
"/hotel/path",
"hotel.interface", QDBusConnection::sessionBus());
if (!iface.isValid()) {
qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
exit(1);
}
2)        传递参数
int num_room;
num_room= this->ui->checkInlineEdit->text().toInt();
3)        呼叫远程的checkIn，参数为num_room
QDBusReply<int> reply = iface.call("checkIn", num_room);
4)        判断返回值
if (reply.isValid()) {
            num_room = reply.value();
          qDebug("Got %d %s/n", num_room, (num_room > 1) ? "rooms" : "room");
    } else {
           qDebug( "Check In fail!/n");
}
## 3.3 客户端的用法3：使用DBusProxy
可以参考实例DBusProxy中的DBusClientProxy和DBusServer
DBusProxy可以使访问更加方便，就像访问本地类成员变量的方式访问远程的method
在使用这个类的时候要使用两个工具
qdbuscpp2xml和qdbusxml2cpp
利用qdbuscpp2xml –M –S hotel.h –o hotel.xml
       在利用qdbusxml2cpp hotel.xml –p hotelInterface
在客户端的工程中添加这俩个文件。
1)        创建接口
a)         hotel::interface myHotel("hotel.server",
a)         "/hotel/path",
                                                                                                            i.             QDBusConnection::sessionBus());
2)        给参数赋值
                        i.              int num_room;
                      ii.              num_room= this->ui->checkInlineEdit->text().toInt();
3)        调用checkIn
a)         QDBusPendingReply<int> reply = myHotel.checkIn(num_room);
4)        等待完成
a)         reply.waitForFinished();
5)        判断返回值
    if (reply.isValid()) {
            num_room = reply.value();
             qDebug("Got %d %s/n", num_room, (num_room > 1) ? "rooms" : "room");
    } else {
            qDebug( "Check In fail!/n");
}
## 3.4服务端的用法1：直接注册对象和服务
可以参考实例DBusMessage中的DBusServer
1)        创建连接
QDBusConnection bus = QDBusConnection::sessionBus();
// 在session bus上注册名为"com.test.hotel"的service
2)        注册服务
if (!bus.registerService("hotel.server")) {
a)         qDebug() << bus.lastError().message();
b)        exit(1);
}
3)        注册对象
if(! (bus.registerObject("/hotel/path", &my_hotel ,  QDBusConnection::ExportAllSlots)))
{
qDebug() << bus.lastError().message();
exit(1);
  }
## 3.5 服务端的用法2：使用DBusAdapter
可以参考实例DBusAdapter中的DBusServerXML
QT4推荐使用Adapter来注册Object。
很多情况下，我们可能只需要把我们定义的类里的方法有选择的发布到Message Bus上，使用Adapter可以很方便的实现这种意图
1)          创建连接
QDBusConnection bus = QDBusConnection::sessionBus();
// 在session bus上注册名为"com.test.hotel"的service
2)        注册服务
    if (!bus.registerService("hotel.server")) {
            qDebug() << bus.lastError().message();
            exit(1);
    }
3)        创建Adaptor对象
    myHotelAdaptor =new InterfaceAdaptor(&my_hotel);
4)        注册对象
  if(! (bus.registerObject("/hotel/path", &my_hotel ,QDBusConnection::ExportAllSlots)))
    {
      qDebug() << bus.lastError().message();
      exit(1);
  }
# 4 QT中Dbus的特殊的用法(使用QT的信号)
在使用QT一段时间后，大家也许对QT的信号与槽很熟悉了，再来了一个DBus的信号与槽，大家可能很容易混淆了，想直接使用QT的信号与槽？
这样也是可以的，可以参照QT的例子程序 dbus-chat，中间就有这种用法的。
发射信号直接用QT的，绑定信号与槽也用QT的是不是很熟悉了。
具体的可以参考QT的实例dbus-chat
也可以参考实例QTSignalSlot中的DBusClientQTSignal和DBusServerQTSignal
发送信号端：
1 新建一个DBus服务
QDBusConnection bus = QDBusConnection::sessionBus();
2 注册对象（注册的时候必须注册this，这样this对象中的信号就可以直接发送到DBus上）
bus.registerObject("/hotel/path", this ,
                           QDBusConnection::ExportAllSlots|
                           QDBusConnection::ExportAllSignals);
接受信号端：
1 新建一个接口
myInterface= new hotel::interface(QString(),QString(),QDBusConnection::sessionBus(),this);
2 信号的绑定（注意：接收必须在DBus中注册的对象中接收）
QObject::connect(myInterface,SIGNAL(checkIn(int)),this,SLOT(checkInSlot(int)));
注意点：
1 信号的发射端要想信号发射到DBus上去，在对象的构造函数中必须，注册对象自己，必须使用this指针。例如：connection.registerobject(“/com/dbus/path”,this);
这样这个类中的所有的信号都可以直接用emit发射到DBus总线上。
2 信号的接受端绑定信号是可以使用QTDBus的绑定也可以直接使用QObject的绑定。
当使用Qobject的绑定时，QObject：：connect（dbusInterface，SIGNAL（testMessage），this，SLOT（testEvent）；
创建接口时，接口的头两个参数必须为空，例如：
dbusInterface =new dbus：：test：：interface（QString（），QString（），QDBusConnection：：session（），this）；
绑定的信号必须是已经导出的在interface文件有的。
当使用QDBus的绑定时，不论前两个参数是否为空，都可以正常的接受到信号
3 只有注册到DBus上去对象才能接受打DBus上的信号，其他的对象要接受DBus上的信号必须通过这个注册的对象将信号转发出来。
QObject::connect(myInterface,SIGNAL(queryReply(int)),this,SIGNAL(queryResult(int)));
[源码](http://download.csdn.net/source/3163664)
