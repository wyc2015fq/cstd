# Android基于MQTT协议实现的推送功能 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月16日 16:03:26[boonya](https://me.csdn.net/boonya)阅读数：5216








**基于MQTT协议的实现有三种方式：**


1.官方的C2DM，但是只支持android2.2及以上平台的，而且使用的google的服务器。

对于google服务器的问题，网友应该都清楚，天朝的大中华区局域网总是让它不时的给你断一下。

2.第三方的androidpn，和C2DM一样，都是基于XMPP扩展的，是一个开源的项目，据说不错。
[http://sourceforge.net/projects/androidpn/](http://sourceforge.net/projects/androidpn/)

但是是基于长连接的，如果客户端数量大，特别像手机这种都是长期在线的设备，

会有两个问题，（1）服务器压力，（2）手机的电池不够用啊，电量卡卡卡的被你耗光了（需要优化网络机制）。

3.使用IBM 的MQTT协议实现push消息

地址：[http://tokudu.com/2010/how-to-implement-push-notifications-for-android/](http://tokudu.com/2010/how-to-implement-push-notifications-for-android/)

这是一个非常理想的解决方案，是基于tcp协议的，低带宽通信，而且国外友人已经测试，耗电量很多。

参考地址：[http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2014/1210/2141.html](http://www.jcodecraeer.com/a/anzhuokaifa/androidkaifa/2014/1210/2141.html)




**前段时间公司需要[Android](http://lib.csdn.net/base/15)端的手机群推功能，我们就通过MQTT来实现了该功能。**

MQTT的官网如下 http://mqtt.org/

关于系统的主要架构就不详述了。这关系的到职业道德问题，在这里只谈谈MQTT实现推送的功能。

在MQTT官网中[http://mqtt.org/software](http://mqtt.org/software)列举出了实现了该协议的开源客户端和服务器工程和库。

我们是通过ApacheActiveClient开源项目的基础上来实现的。

下面介绍客户端和服务器简单的搭建：

**客户端 ：**

首先到github([https://github.com/tokudu/AndroidPushNotificationsDemo](https://github.com/tokudu/AndroidPushNotificationsDemo))网站上去下载手机客户端程序：AndroidPushNotificationsDemo.

运行结果如下图所示 ： 

![](https://img-blog.csdn.net/20130508095912884)


其中Test_android1是对手机的唯一标识，这个生成策略是可以自己来定的。很多时候我们需要话更多的时间去研究它的源码，而不是去问别人，只有当自己实在找不到解决方案的时候再问，我觉得这样更利于我们对项目的理解和继续扩展。




**服务器端**

至于服务器程序可以到[http://activemq.apache.org/](http://activemq.apache.org/)去下载。需要注意的是，请读者仔细阅读该开源项目的文档。

该项目非常强大，支持配置服务器的主从，分布式等。




当你搭建成功服务器后可以使用客户端连接了。点击Start Push service按钮启动推送功能。

连接成功后你会在控制台看到如下界面：

![](https://img-blog.csdn.net/20130508102546015)





在服务器后台你将看到：


![](https://img-blog.csdn.net/20130508102636890)


画的红线部分就是你的手机设备，然后单击进入如下界面：

![](https://img-blog.csdn.net/20130508102742606)


最后单击确定 ，在手机通知栏将会收到消息 ：

![](https://img-blog.csdn.net/20130508102918838)





至此，这就是简单的服务器想客户端推送的消息了。

当然这是最最简单的实现了。接下来就要看自己公司实际的需要进行扩展服务器或者客户端了。




其实MQTT最显著的功能，就是群推。客户端可以注册多个token(客户端的唯一标识)可以让所有客户端都注册该token即可，然后发送的时候，只需要针对一个token发消息，那么所有的手机都收到了。不用像其他的推送一样，对每个token都去发一遍。所以MQTT群发的效率的极高的。这样的话对手机端来说就很好了，也许我们需要对某个程序的所有手机发、向某个程序某个版本手机发，向某台手机发推送 等等用MQTT都可以很轻松的实现。




接下来就是要靠自己去研究MQTT了。其他官网文档，和一些开源的功能已经提供了很多我们学习的资料。




下面是关于XMPP协议推送 ：[Android消息推送(一)--AndroidPn(XMPP协议)Demo版到正式上线](http://blog.csdn.net/johnny901114/article/details/7715758)

如果github上面的Android客户端的程序不能运行，可以下载我们用的（也是github上下载的）：

[http://download.csdn.net/detail/johnny901114/5820151](http://download.csdn.net/detail/johnny901114/5820151) 如果还有问题可以留言。





转载出处 ： [http://blog.csdn.net/johnny901114/article/details/8898727](http://blog.csdn.net/johnny901114/article/details/8898727)



