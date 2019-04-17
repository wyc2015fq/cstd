# Androidpn 基于XMPP协议的消息推送实践 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月16日 16:36:16[boonya](https://me.csdn.net/boonya)阅读数：773








**AndroidPN环境**

AndroidPN实现了从服务器到android移动平台的文本消息推送。这里先简单说一下androidPN的安装过程。

目标下载：androidpn-client-0.5.0.zip和androidpn-server-0.5.0-bin.zip

官方网址：[http://sourceforge.net/projects/androidpn/](http://sourceforge.net/projects/androidpn/)

解压两个包，Eclipse导入client，配置好目标平台，打开raw/androidpn.properties文件，

apiKey=1234567890

xmppHost=10.0.2.2

xmppPort=5222

如果是模拟器来运行客户端程序,把xmppHost配置成10.0.2.2 (模拟器把10.0.2.2认为是所在主机的地址，127.0.0.1是模拟器本身的回环地址).

xmppPort=5222 是服务器的xmpp服务监听端口

运行androidpn-server-0.5.0\bin\run.bat启动服务器，从浏览器访问[http://127.0.0.1:7070/index.do](http://127.0.0.1:7070/index.do) (androidPN Server有个轻量级的web服务器，在7070端口监听请求，接受用户输入的文本消息)

运行客户端，客户端会向服务器发起连接请求，注册成功后，服务器能识别客户端，并维护和客户端的IP长连接

![](http://pic002.cnblogs.com/images/2011/139718/2011070915014345.png)

 进入Notifications界面，输入消息发送

![](http://pic002.cnblogs.com/images/2011/139718/2011070915051139.png)

模拟器客户端接受到server推送的消息

![](http://pic002.cnblogs.com/images/2011/139718/2011070915060620.png)

这样AndroidPN的环境就搭好了，下一步我将深入研究研究实行以及XMPP协议。

文章转自：http://www.cnblogs.com/devxiaobai/archive/2011/07/09/2101794.html




