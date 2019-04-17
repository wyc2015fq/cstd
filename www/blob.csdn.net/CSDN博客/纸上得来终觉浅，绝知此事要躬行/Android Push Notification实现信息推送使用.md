# Android Push Notification实现信息推送使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年12月01日 19:50:44[boonya](https://me.csdn.net/boonya)阅读数：1918









文章来自：[http://www.cnblogs.com/hanyonglu/archive/2012/03/16/2399655.html](http://www.cnblogs.com/hanyonglu/archive/2012/03/16/2399655.html)


AndroidPn项目是使用XMPP协议实现信息推送的一个开源项目。在这里给大家介绍其使用过程。

**　　Apndroid Push Notification的特点：**


　　快速集成:提供一种比C2DM更加快捷的使用方式,避免各种限制. 


　　无需架设服务器:通过使用"云服务",减少额外服务器负担.


　　可以同时推送消息到网站页面,android 手机


　　耗电少,占用流量少.



**　　具体配置过程：**


　　首先， 我们需要下载androidpn-client-0.5.0.zip和androidpn-server-0.5.0-bin.zip。


　　下载地址：[http://sourceforge.net/projects/androidpn/](http://sourceforge.net/projects/androidpn/)




　　解压两个包，Eclipse导入client，配置好目标平台，打开raw/androidpn.properties文件，配置客户端程序。




　　1. 如果是模拟器来运行客户端程序,把xmppHost配置成10.0.2.2[模拟器把10.0.2.2认为是所在主机的地址，127.0.0.1是模拟器本身的回环地址，10.0.2.1表示网关地址，10.0.2.3表示DNS地址，10.0.2.15表示目标设备的网络地址]，关于模拟器的详细信息，大家可参阅相关资料，这里不再详述.


　　xmppPort=5222 是服务器的xmpp服务监听端口




　　运行androidpn-server-0.5.0\bin\run.bat启动服务器，从浏览器访问http://127.0.0.1:7070/index.do (androidPN Server有个轻量级的web服务器，在7070端口监听请求，接受用户输入的文本消息)




　　运行客户端，客户端会向服务器发起连接请求，注册成功后，服务器能识别客户端，并维护和客户端的IP长连接。



**　　2. 如果是在同一个局域网内的其他机器的模拟器测试(或者使用同一无线路由器wifi上网的真机) ，则需要把这个值设置为服务器机器的局域网ip. **

**　　例如 你的电脑和android手机 都通过同一个无线路由器wifi上网, 电脑的ip地址为 192.168.1.2 而 手机的ip地址为 192.168.1.3, 这个时候 需要把这个值修改为 xmppHost=192.168.1.1 或是电脑的IP地址，就可以在手机上使用了.**




　　3. 如果是不在同一个局域网的真机测试，我们需要将这个值设置为服务器的IP地址。



**　　具体配置如下图所示：**



![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-03-16_081017.png)



**　　我的电脑IP是：192.168.8.107**


　　服务器运行主界面：



![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-03-16_074350.png)

![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-03-16_074433.png)



![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-03-16_074420.png)


　　推送信息如下界面所示：



![](http://images.cnblogs.com/cnblogs_com/hanyonglu/2012-03-16_074154.png)


　　测试结果如下图所示：

![](http://images.cnblogs.com/cnblogs_com/hanyonglu/SC20120316-074221.png)![](http://images.cnblogs.com/cnblogs_com/hanyonglu/SC20120316-074227.png)最后在我的模拟器和真机中测试通过，之前用过百度云推送，说实话多一种方案多条活路，借鉴益智！

另外还有一篇推送相关的文章：[SignalR推送服务在Android的实现
 SignalA](http://www.cnblogs.com/cuihongyu3503319/p/3864100.html)



