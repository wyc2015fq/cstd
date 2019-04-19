# android使用socket使底层和framework通信 - xqhrs232的专栏 - CSDN博客
2011年03月15日 15:51:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1009标签：[socket																[android																[stream																[linux																[jni																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://fanwei51880.blog.163.com/blog/static/32406740201011150240981/](http://fanwei51880.blog.163.com/blog/static/32406740201011150240981/)
一般的native和framework的通信是通过jni，但是这一般只是framework调用native，native如果有消息要怎样通知上层呢？android中GSP模块提供一种解决思路，但是实现有些复杂，这里介绍一种使用socket通信的方法可以使native和framework自由通信，具体实现如下：
android中使用jni对linux中的socket进行了封装。使用起来十分的方便。 
由于android是基于linux的，所以linux的代码会在java之前先执行，所以一般native端是服务器。framework端是客户端。 
java层主要代码：
[view source](http://news.wangmeng.cn/detailNews/2757-android-uses-socket-communications-to-the-bottom-and-the-framework#viewSource)[print](http://news.wangmeng.cn/detailNews/2757-android-uses-socket-communications-to-the-bottom-and-the-framework#printSource)[?](http://news.wangmeng.cn/detailNews/2757-android-uses-socket-communications-to-the-bottom-and-the-framework#about)
`1.``LocalSocket s =``null``;`
`2.``LocalSocketAddress l;`
`3.``s = ``new``LocalSocket();`
`4.``l = ``new``LocalSocketAddress(SOCKET_NAME,LocalSocketAddress.Namespace.RESERVED);`
`5.``s.connect(l);`
到此时如果socket连接没有问题，就可以像正常的读写了。 
native层主要代码：
[view source](http://news.wangmeng.cn/detailNews/2757-android-uses-socket-communications-to-the-bottom-and-the-framework#viewSource)[print](http://news.wangmeng.cn/detailNews/2757-android-uses-socket-communications-to-the-bottom-and-the-framework#printSource)[?](http://news.wangmeng.cn/detailNews/2757-android-uses-socket-communications-to-the-bottom-and-the-framework#about)
`1.``s_fdListen = android_get_control_socket(SOCKET_NAME);`
`2.``ret = listen(s_fdListen, n);`
`3.``s_fdCommand = accept(s_fdListen, (sockaddr *) &peeraddr, &socklen);`
如果连接没有问题就可以使用linux中的write/read来对socket进行读和写了； 
这里有必要解释一下SOCKET_NAME，它的值是一个字符串，它在init.rc中定义的一个字符串。也就是说，我们可以通过修改init.rc中来申请我们需要的socket资源。
这里以ril为例来说明：
service ril-daemon /system/bin/rild 
socket rild stream 660 root radio 
socket rild-debug stream 660 radio system 
user root 
group radio cache inet misc audio
以上是摘自android 2.2 源码中的system/core/rootdir/init.rc中的片段。至于其具体含义可以参见init.c和system/core/init /readme.txt文件。他的作用是由init.c来解析init.rc，并为我们启动一个名为rild的守护进程，它是一个可执行程序，我们通过 adb shell在system/bin中可以找到对应的rild文件。socket表示为这个守护进程分配一个socket资源，这个socket资源可以在 /dev/socket/下找到rild。也就是本文要这里最关键的地方，socket能不能通就看守护进程能不能很好的起来。上面
 SOCKET_NAME也就是这里定义的字符串（在ril.java和ril.cpp中就有一个字符串常量SOCKET_NAME_RIL，他的值就是 rild，和上面的对应）。
如果我们要自定义一个socket来进行通信，我们可以在init.rc的最后面加上
service myserver-daemon /system/bin/server 
socket server stream 666 
oneshot
system/bin/server就是我们编译生成的服务器程序，在里面我们调用
s_fdListen = android_get_control_socket(“server”); 
ret = listen(s_fdListen, n); 
s_fdCommand = accept(s_fdListen, (sockaddr *) &peeraddr, &socklen);
就可以建立一个服务器端程序。 
java只需要使用最上面的代码就可以和native通信了，注意SOCKET_NAME值必须上下统一和init.rc中的相等，此处为“rild”。这里的oneshot必须有，没有的话，你的server很可能起不来。
剩下的只剩下编译了。 
关于编译可以参考ril中的中的Android.mk和rild.c和ril.cpp，自己把头文件挑出即可。 
先用mm编译自己加的模块，编译好后，将添加的模块考出，在源码的根目录下make snod。将编译输出文件加到system.img中。最后将system.img和randisk.img拷到sdk对应的平台中。即可。主要这两个 img文件都要拷，system.img中有你的可执行程序，而randisk.img中有你的init.rc。userdata.img不确定。
此时只需要用java写一个客户端程序即可。

