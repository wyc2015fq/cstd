# Qt之获取本机网络信息 - xqhrs232的专栏 - CSDN博客
2019年02月11日 14:39:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：77
原文地址::[http://blog.sina.com.cn/s/blog_a6fb6cc90102v0mz.html](http://blog.sina.com.cn/s/blog_a6fb6cc90102v0mz.html)
相关文章
1、Qt获取本地网卡信息----[https://blog.csdn.net/qq78442761/article/details/81187013](https://blog.csdn.net/qq78442761/article/details/81187013)
2、[Qt5获取网卡/IP等信息](https://www.cnblogs.com/cppskill/p/6064395.html)----[https://www.cnblogs.com/cppskill/p/6064395.html](https://www.cnblogs.com/cppskill/p/6064395.html)
3、qt获取网卡信息 QNetworkInterface----[https://blog.csdn.net/wjs1033/article/details/22697063](https://blog.csdn.net/wjs1033/article/details/22697063)
  经常使用命令行来查看一些计算机的配置信息。
1.首先按住键盘上的“开始键+R键”，然后在弹出的对话框中输入“CMD”，回车
另外，还可以依次点击 开始>所有程序>附件>命令提示符
2.接下来在弹出的命令提示符窗口中输入“ipconfig /all”并按键盘上的回车，(注：ipconfig 命令为查看IP配置信息，参数“all”表示查看所有网卡的全部配置信息）
    如下图所示：
![Qt之获取本机网络信息](http://s15.sinaimg.cn/mw690/0033AMT7zy6Mgu5p83cae&690)
    那么，在Qt中如何获取主机名、IP、端口、MAC等信息？
主要使用到的类有：
- QHostInfo
- QHostAddress
- QNetworkInterface
- QNetworkAddressEntry
1.获取登录用户
QString name = qgetenv("USERNAME");
qDebug() << "User Name:" << name;
输出：
User Name: "wangl-d"
2.获取主机名、IP地址
//QHostInfo类作用，获取主机名，也可以通过主机名来查找IP地址，或者通过IP地址来反向查找主机名。
QString localHostName = QHostInfo::localHostName();
qDebug() << "LocalHostName:" << localHostName;
//获取IP地址
QHostInfo info = QHostInfo::fromName(localHostName);
qDebug() << "IP Address:" << info.addresses();
foreach(QHostAddress address, info.addresses())
{
    if (address.protocol() == QAbstractSocket::IPv4Protocol)
        qDebug() << "IPv4 Address:" << address.toString();
}
foreach (QHostAddress address, QNetworkInterface::allAddresses())
{
    qDebug() << "Address:" << address;
}
输出：
LocalHostName: "wangl-d-pc"
IP Address: (QHostAddress("fe80::396e:183d:aac6:f7f2") , QHostAddress( "192.168.118.55" ) )
IPv4 Address: "192.168.118.55"
Address: QHostAddress( "fe80::396e:183d:aac6:f7f2" )
Address: QHostAddress( "192.168.118.55" )
Address: QHostAddress( "::1" )
Address: QHostAddress( "127.0.0.1" )
Address: QHostAddress( "fe80::5efe:c0a8:7637" )
Address: QHostAddress( "fe80::e0:0:0:0" )  
3.获取设备名、MAC地址、子网掩码、广播地址等
//获取所有网络接口的列表
foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
{
    //设备名
    qDebug() << "Device:" << netInterface.name();
    //MAC地址
    qDebug() << "HardwareAddress:" << netInterface.hardwareAddress();
    QList entryList = netInterface.addressEntries();
    //遍历每一个IP地址(每个包含一个IP地址，一个子网掩码和一个广播地址)
    foreach(QNetworkAddressEntry entry, entryList)
    {
        //IP地址
        qDebug() << "IP Address:" << entry.ip().toString();
        //子网掩码
        qDebug() << "Netmask:" << entry.netmask().toString();
        //广播地址
        qDebug() << "Broadcast:" << entry.broadcast().toString();
    }
}
输出：
Device: "{4C5C2613-C18F-416A-A21F-47B707C3B935}"
HardwareAddress: "74:D4:35:1B:A6:2F"
IP Address: "fe80::396e:183d:aac6:f7f2"
Netmask: "ffff:ffff:ffff:ffff::"
Broadcast: ""
IP Address: "192.168.118.55"
Netmask: "255.255.255.0"
Broadcast: "192.168.118.255"
Device: "{846EE342-7039-11DE-9D20-806E6F6E6963}"
HardwareAddress: ""
IP Address: "::1"
Netmask: "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"
Broadcast: ""
IP Address: "127.0.0.1"
Netmask: ""
Broadcast: ""
Device: "{539F3167-F67C-432D-8904-34129F3F5521}"
HardwareAddress: "00:00:00:00:00:00:00:E0"
IP Address: "fe80::5efe:c0a8:7637"
Netmask: "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff"
Broadcast: ""
Device: "{5DECB4C5-20AC-4BD6-A685-9FE608ED6EF5}"
HardwareAddress: "00:00:00:00:00:00:00:E0"
IP Address: "fe80::e0:0:0:0"
Netmask: "ffff:ffff:ffff:ffff::"
Broadcast: "" 
