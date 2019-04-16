# 【翻译】你应该了解的三种广域网协议：HDLC、PPP、Frame-Relay - 关注微信公众号【OpenCV学堂】 - CSDN博客





2009年12月19日 10:01:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：3098









你应该了解的三种广域网协议：HDLC、PPP、Frame-Relay

－ By gloomy fish

你们公司已经连接到Internet，是嘛？（每个人都开始点头是）你们采用什么广域网协议

来连接到Internet？如果你们是租用线路到Internet或者是私有网络在当地网络之间，你可能

采用三种广域网（WAN）协议之中的一种：HDLC/PPP/Frame-Relay，让我们一起来探索这

些协议之间的不同点和相似之处：



什么是HDLC？

HDLC表示高级数据链路控制协议（High-level
 Data Link Control Protocol）,与这篇文章中提

到的其它两种广域网协议一样，HDLC是一个第二层协议（参见OSI网络模型得到更多分

层信息）。HDLC是一种简单的协议用来连接点到点（Point
 To Point）的串行设备。例如，

你有点到点的租用线路连接不同城市的两个地方。HDLC连接可以拥有最少配置要求的连接

HDLC协议运行在广域网上，在两个不同的地方。每个路由器将会解封装HDLC的数据包

对它们进行转发到LAN或者是丢弃。

HDLC对错误的纠正跟以太网类似，思科版本的HDLCA更加的私有化一点，他们特别

增加了一个协议类型域。因此思科的HDLC只能工作在思科的设备之上。

HDLC实际上是一个默认的设置在所有的思科串行接口上。如果你做一个show


running-config在思科路由器上，你的串行接口（默认）不会有任何封装，这是因为它们被

配置为默认的HDLC，如果你输入show
 interface serial 0/0，你将会看到运行的HDLC协议。



什么是PPP？

你可能听说过PPP协议（Point
 to Point Protocol）因为它被应用于大多数的拨号连接到Internet

PPP文档说明为RFC1661，PPP是基于HDLC的，而且非常相似。都可以很好的工作在点

到点的租用线路上。PPP和HDLC的不同点如下：

ØPPP协议不是思科路由器私有的

ØPPP有几个子协议来实现它的功能

ØPPP是多个特征跟拨号网络特征一样



因为PPP协议有很多拨号网络的特征，它已经成为当今最流行的拨号网络协议，下面是它

可以提供的拨号网络特征：



Ø连接质量管理监视拨号连接的质量和已经发生的错误多少。如果接受的错误太多将

会关闭连接。

Ø多链路可以容纳多个PPP拨号连接并且结合在一起跟一个PPP拨号连接功能相似

ØPAP、CHAP支持的身份鉴别，当你拨号的时候，这些协议将接受你的户名和密码

来确保你可以进入想要连接的网络。



在思科路由器上改HDLC为PPP，运用这样的命令：**encapsulation ppp**改变封装类型为PPP

以后，输入：PPP?将会列出PPP可用的选项。相比HDLC而言PPP协议有很多的选项。



什么是帧中继（what is frame-relay）

帧中继是第二层协议，常见作为载波服务。例如，人们会说我定了一个帧中续电路。帧中继

创建一个私有网络通过载波网络，这样做会产生一个持久虚电路（PVC），一个PVC是从一

个站点到另外一个站点的通过载波网络的连接。这是一个真正由载波产生的配置条目在帧中

继的交换机上。



订一个T1或者T1的部分从载波中就可以获得一个帧中继电路，在此之上，你订一个帧中

继端口和你帧中继电路的大小相匹配。最后，你订一个PVC来连接你的帧中继端口到另外

一个你网络上的端口。帧中继的优点如下：

单一的电路有能力连接“帧中继云”获得进入其它所有的站点（只要你有PVCs）。当站点数量增加时，你可以节省越来越多的钱因为你不需要这么多虚电路当你采用PPP时候来那样来修补你的网络。



提高了容灾恢复因为你所要做的只是订一条到帧中继云的电路，PVC获得进入所有的远程站点。



通过使用PVCs，可以设计你的WAN按照你自己的想法。意味着你可以定义

什么站点可以和其它站点直接相连，你只需要付每个月很少的PVC连接费用

即可。

就帧中继而言，你还应该知道其它一些相关的术语：



ØLMI：本地管理接口（Local Management Interface），LMI是帧中继的管理协议。LMI被发送在帧中继路由和交换机之间交流什么DLCI是可用的，网络是否拥堵信息。

ØDLCI：数据链路连接标识（Data Link Connection
 Identifier）这是一个用来标识帧中继网络中每个PVC的数。

ØCIR：提交信息比率（Committed Information Rate）表示保证你的接收而开销的带宽量，在每个PVC上，总的说来你应该有比你的端口速度小CIR。你当然可以把你的流量速度超过你的端口接受速度，但是这些流量将被标记为DE（Discard
 Eligible）。

ØDE：（Discard Eligible）丢弃符合。带有DE标记的流量可以被丢弃当网络比较拥塞的时候。

ØFECN/BECN：前向直接拥塞通知（forward explicit
 congestion notification）/后向直接拥塞通知（backward
 explicit congestion notification）。这些位设定在LMI包中通知帧中继设备网络上出现了拥塞！



原文地址：http://www.petri.co.il/csc_3_wan_protocols_you_should_know.htm




