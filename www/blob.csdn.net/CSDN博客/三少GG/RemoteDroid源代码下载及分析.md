# RemoteDroid源代码下载及分析 - 三少GG - CSDN博客
2012年01月31日 01:51:20[三少GG](https://me.csdn.net/scut1135)阅读数：3605
　RemoteDroid是一个[Android](http://www.apkbus.com/)应用，能够让用户使用自己的无线网络使用无线键盘、触摸屏操作手机。这个项目为开发者提供了如网络连接、触摸屏手指运动等很好的样例。[http://www.apkbus.com/android-13507-1-1.html](http://www.apkbus.com/android-13507-1-1.html)
Android 控制电脑视频演示： [http://v.youku.com/v_show/id_XMjEwODAyNjQ4.html](http://v.youku.com/v_show/id_XMjEwODAyNjQ4.html)
官方网站： [http://www.remotedroid.net/](http://www.remotedroid.net/)
### How Does It Work?
RemoteDroid consists of two parts: the phone application you can download from Google's Android Marketplace, and a companion server application for your computer that you can [download
 here](http://www.remotedroid.net/RemoteDroidServer_v1.5.zip).
另一个下载地： [https://github.com/crodas/Slides/tree/master/RemoteDroidServer](https://github.com/crodas/Slides/tree/master/RemoteDroidServer)
但实际上真正的浏览源代码：
[http://code.lupaworld.com/code.php?mod=list&itemid=35&path=./remotedroid_1.0/](http://code.lupaworld.com/code.php?mod=list&itemid=35&path=./remotedroid_1.0/)
包括：RemoteDroid 2.0   ,  RemoteDroid Server
真正的源代码下载： [http://download.csdn.net/detail/liuwenhan999/3135494](http://download.csdn.net/detail/liuwenhan999/3135494)
####   2010-12-20
   文件名称：remotedroid_1.0       项目名称：Remote Droid项目源码 
   项目描述：RemoteDroid是一个Android应用，能够让用户使用自己的无线网络使用无线键盘、触摸屏操作手机。这个项目为开发者提供了如网络连接、触
 摸屏手指运动等很好的样例。本项目源码包括服务端伺服程序和手机客户端程序。
[http://www.lupaworld.com/article-208510-1.html](http://www.lupaworld.com/article-208510-1.html)
RemoteDroid 允许您把G1当做键盘或鼠标来使用，在一些意外的场合没准它会帮准您把工作做到事半功倍。
![](http://www.lupaworld.com/data/attachment/portal/201012/20/102802644hg91h693ad6zg.jpg)
运 行RemoteDroid，它会告诉您的IP地址，通过wifi连接到电脑，然后打开G1输入IP地址，连接成功..它就可以被当做键盘或鼠标来使用了。 不知道这种输入方式会不会达到用真正的键盘或鼠标的速度，也许熟练了就好了。不过这个小软件可以让那些用电脑组成家庭影院或用来商务演示的人们节省一笔投 资，如果出现在iPHONE上就大卖咯。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
社会上大家的引用：
1.  
### [毕业设计之服务端设计之消息设计之修改篇（参考remotedroid项目）](http://blog.csdn.net/liuwenhan999/article/details/6284680)
### “
正好白天在公司没什么事，于是看了看remotedroid的源码，界面没怎么看，主要瞅瞅它的OSC包下的核心设计。它的OSC下主要有OSCMessage.java消息类，和OSCPortIn/Out消息的接收传输类，消息接收采用的是基于消息事件的监听方式来处理接收到消息的，因为我做的主要是把手机屏幕模拟成无线鼠标，所以消息设置的很简单。还有就是remotedroid采用的是UDP数据包的形式来发送的，我写的是用TCP接收发送，发现用UDP确实比TCP简单，于是今天先把UDP模式的记录下来（TCP模式的在公司写好了，带不出来，懒得背了，有空再搬回家），其实就是把remotedroid的OSC移植到我自己的版本中来了。
废话不多了，上代码：
### ”
2. [远程启动电脑程序的Android应用:Remote
 Launcher](http://www.lupaworld.com/article-212246-1.html)
　Remote
 Launcher由Android应用和PC端小的服务器程序两部分组成。你可以配置Remote Launcher来启动你个人电脑的任何程序。比如它可以启动你的媒体中心、关机或重启电脑，或者一个自定义脚本。
# [Remote Launcher App 0.2.11发布 远程控制工具](http://www.ithov.com/linux/108106.shtml)
Remote Launcher App 0.2.11此版本增加了蜂窝的支持。使得用户更加适应界面。增加直接发送WOL的能力，而不是简单广播IP。
Remote Launcher App是一个易于使用的工具，可让您启动您PC上的应用程序，通过远程单一点击一个Android设备。它也可以用来启动您的电脑，即使是关闭的，支持局域网唤醒。它包括一个Android应用程序和一个小服务器程序在您的电脑中。您可以配置远程启动，在PC上启动任何应用程序。例如，它可以启动你的媒体中心，关机或重新启动计算机，或者运行自定义脚本。
**官方网站：[Remote Launcher](http://owtroid.com/remotelauncher/mediawiki/index.php?title=Remote_Launcher)     貌似不开源哎，算了，不关注了**
3. [Chrome
 Remote Desktop 扩展](http://www.lupaworld.com/article-213846-1.html)
Google 发布了一款重量级的 Chrome 扩展，名为 Chrome Remote Desktop，通过它你可直接在 Chrome/Chromium 浏览器上远程访问其它人的电脑，或共享自已的电脑让别人远程遥控。
Chrome Remote Desktop 目前还处于 Beta 阶段，它是一款跨平台软件，完全支持 Windows, Linux, Mac 和 Chromebooks，使用条件是你必须具备一个 Google 帐号。
扩展下载：[**Chrome Web Store**](https://chrome.google.com/webstore/detail/gbchcmhmhahfdphkhkmpfmihenigjmpp)
使用方法很简单，打开 Chrome Remote Desktop 后输入对方的提供的一次性验证码就可直接操控对方的电脑了。反之，你也可以把自已生成的一次性验证码给别人，让别人远程遥控你的电脑。
# 以下是我在 Win 7 上用 Chrome Remote Desktop 控制 Ubuntu (Gnome Shell) 的截图上。
根据开发者的[介绍](http://groups.google.com/a/chromium.org/group/chromium-dev/browse_thread/thread/2ae093ab7d858b8?pli=1)，该扩展是在
 Google 的一些技术上进行的设计和构建：
底层利用 libjingle 进行 p2p 连接，可以运行于 udp , tcp 协议
使用 libjingle 中的 PseudoTCP 协议提供可靠连接
使用了 SSL 连接
数据构建和框架使用了 protobuf
图形渲染使用了 VP8
