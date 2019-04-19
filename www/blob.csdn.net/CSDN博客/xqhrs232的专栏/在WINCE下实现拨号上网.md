# 在WINCE下实现拨号上网 - xqhrs232的专栏 - CSDN博客
2013年01月09日 22:28:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：541
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://blog.csdn.net/huang5556019/article/details/7773525](http://blog.csdn.net/huang5556019/article/details/7773525)
目前基于Windows CE的产品大多数具有上网功能，能够连接到Internet实现浏览网页、收发邮件等基本功能。要实现更多的功能取决于连接Internet的速度，而速度取决于连接Internet的方式。连接Internet的方式分为两种：有线和无线。事实上大多数产品还是采用无线上网方式。
　　基于Windows mobile的Pocket PC和Smart phone能够轻松实现上网功能。这两种产品的软件开发人员可以不必过多考虑如何实现拨号上网。而对于采用定制Windows CE内核的产品而言，相关的软件开发人员必须自己实现拨号上网。幸运的是Windows CE的SDK中包含了拨号上网的支持，使软件开发人员的工作大大减轻。
　　拨号上网的硬件设备可以是独立的带内置调制解调器和具有GPRS功能的手机，也可以是嵌入到主板中带有同样功能的手机芯片。具体是什么并不重要，因为Windows CE包含的拨号上网模块采用标准的AT命令集与硬件设备通信。软件开发人员可以直接利用CE提供的接口函数来控制整个拨号过程，直到断开连接。
　　Windows CE.NET 提供了一个被称为RAS（Remote Access Service）的服务，通过RAS基于Windows CE的产品可以通过WAN（广域网）或VPN（虚拟个人网络）连接到远程服务器上。调用RAS的应用程序通常在客户机上执行，通过电话网连接到远程服务器，这种连接使用两种标准的远程访问协议：PPP点对点协议和SLIP串行线路Internet协议。RAS结构如图1所示。从图中可以看出RAS处于ISO/OSI模型的上层，下层的TAPI（电话API）调用Unimodem，Unimodem是通用的调制解调器驱动程序，它调用串口API实现与调制解调设备的通信。
![图1](http://hiphotos.baidu.com/lvoids/pic/item/8d1001e963e60d01b90e2d41.jpg)
图1 RAS服务结构图
　　Windows CE.NET下RAS所具备的所有功能同PC下的相似，同样支持电话本、拨号控制脚本。不同的是CE下的RAS的电话本是存放在注册表中。这些都是次要的功能，在此不给予讲述。实现拨号必须调用的函数如下：
|函数|描述|
|----|----|
|RasDial|建立RAS连接|
|RasEnumEntries|列出注册表中所有拨号记录|
|RasGetConnectStatus|获得指定远程访问连接的当前状态。|
|RasGetEntryDialParams|获得指定的上次成功拨号记录的连接数据（参数）|
|RasSetEntryDialParams|设置指定的上次成功拨号记录的连接数据（参数）|
|RasGetEntryProperties|获得拨号记录属性|
|RasSetEntryProperties|设置拨号记录属性|
|RasHangUp|终止RAS连接|
　　基于Windows CE的产品在设计上有的能够在断电前保存注册表数据，有的不能够。对于不能够保存注册表数据的产品，采用电话本来拨号是行不通的。只能让用户每次启动后重新输入拨号数据，或者将数据保存到永久存储器上，在下一次拨号程序启动时先读取拨号数据。
　　整个拨号状态分为同步和异步两种。同步状态下RasDial函数直到建立连接或者发生错误不能继续工作时才返回。同步模式的缺点是客户不能收到连接操作的当前状态和进度信息。但是仍然有解决方法，办法是创建一个新的线程来进行拨号（调用RasDial），另外创建一个新的线程每隔一段时间查询当前拨号状态（调用RasGetConnectStatus）。相比较异步模式在得到当前拨号状态方面要容易的多。异步模式下RasDial函数会立刻返回，接着可以调用RasGetConnectStatus得到当前拨号状态。
    RasGetEntryDialParams和RasGetEntryProperties两个函数得到的是不同方面的数据。在此做一说明：CE为DialPrarms拨号参数定义了结构体RASDIALPARAMS。它包括拨号记录名、电话号码、用户名、密码、域名等。而对于Properties属性数据，CE定义了结构体RASENTRY。它包括连接选项、国家码、区域码、本地电话号，可以指定本地IP地址、远程DNS服务器IP、远程WINS服务器IP等，还可以指定连接控制脚本文件路径、采用的协议、调制解调器类型及名称等。在拨号前一定要设置要这些参数，不过不必每个参数都设置。最简单的办法是得到系统默认的所有参数。调用代码如下：
DWORD dwReturn;
dwReturn = RasGetEntryProperties(NULL, L\"\", &RasEntry, &dwSize, NULL, NULL); 
if(dwReturn != 0) 
{ 
////不能得到系统拨号参数
return; 
}
　　得到系统默认的参数后再加以修改，这样避免了设置错误的参数，而且大多数参数不用我们设置。关于拨号上网就讲这么多，如果读者有疑问或者遇到困难可以发邮件给我，如果有时间我将回复。
　　另外关于PlatformBuilder，很多人发邮件给我希望我能回答他们关于PB的问题。在这里我向读者们宣布，关于PB和EVC的书我和我的同事正在写，预计9月份就能够完成。这本书详细介绍了PB（包括驱动）和EVC的使用，还有一些开发实例。PB和驱动部分都是我的同事写的，他们很有经验。
