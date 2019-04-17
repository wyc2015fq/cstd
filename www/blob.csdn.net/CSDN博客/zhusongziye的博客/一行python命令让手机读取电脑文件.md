# 一行python命令让手机读取电脑文件 - zhusongziye的博客 - CSDN博客





2018年11月25日 12:43:39[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：223








本文讲解python的一个内置文件传输下载器，可以用来在局域网内进行文件传输，当然可能有人会问，我用微信QQ也能传，为什么还要用python来传输下载？在此，其实我个人感觉的是，这种操作更简单，省了时间，比如之前在培训班的时候，老师就是通过这个方式，每天上课的资料他都直接放到他的电脑，给我们一个IP地址，我们就能访问下载，这样他就能偷懒了，不用费时间盯着文件通过QQ和微信传的慢这样的问题，也不怕通过QQ微信传文件传着传着断网的问题，而影响大多数学生学习，另一方面也是多给大家掌握一些学习python的一些小知识，让大家能体会到python的简洁之道，更有兴趣的学习这门语言



好了，废话不多说了，首先我随便找了个文件夹，里面有一些文件，我要将这些文件共享给我的手机和同事，我该如何做？

![](https://img-blog.csdnimg.cn/20181125123850392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

进入命令行，想必很多人可能还是会通过 windows+R > cmd > 切换盘符 > cd切换目录这样的方式来进入我们需要的文件夹目录，比如下面的一步步操作

![](https://img-blog.csdnimg.cn/2018112512392245.png)

这种方式已经out了，给大家讲一个很简单的方式，只需要在当前目录地址栏我们输入cmd，按回车就自动进入当前目录，这点小技巧尤其方便，省的我们在一级一级去记忆那么繁琐的目录了

![](https://img-blog.csdnimg.cn/20181125124029943.gif)



接下来我们输入命令，默认端口是8000，我这里指定了一个8888端口

```
G:\1\9浏览器+小工具>python3 -m http.server 8888
Serving HTTP on 0.0.0.0 port 8888 (http://0.0.0.0:8888/) ...
```



之后通过ipconfig命令，查看到ip地址为192.168.43.196

```
C:\Users>ipconfig

Windows IP 配置


以太网适配器 以太网:

  媒体状态  . . . . . . . . . . . . : 媒体已断开连接
  连接特定的 DNS 后缀 . . . . . . . : stfs.sths.com.cn

无线局域网适配器 WLAN:

  连接特定的 DNS 后缀 . . . . . . . :
  本地链接 IPv6 地址. . . . . . . . : fe80::681f:482d:b733:89d6%5
  IPv4 地址 . . . . . . . . . . . . : 192.168.43.196
  子网掩码  . . . . . . . . . . . . : 255.255.255.0
  默认网关. . . . . . . . . . . . . : 192.168.43.1
```

我们这里是局域网文件共享，这里必须保证我们的比如WiFi，其他电脑，手机用的同一个WiFi，这样才能获取到原始电脑的文件



电脑地址栏中输入192.168.43.196:8888,即可看到我们的文件，同时点击文件，即可提示下载还是打开，内置的下载服务即可成功搭建，在局域网的其他小伙伴也可以成功下载

![](https://img-blog.csdnimg.cn/20181125124116865.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

如果想通过手机访问，则在手机中输入192.168.43.196:8888，可以访问到文件，如下



![](https://img-blog.csdnimg.cn/20181125124134364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

我这里只做一个演示，点开了98度.docx文件，docx文件则提示下载或者直接打开，这样我们的手机读取电脑文件就完成了

![](https://img-blog.csdnimg.cn/20181125124220734.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



最后，总结一下，python很多简单实用的功能有待于我们一步步学习，通过此文，可能更加能体会到python的简单强大之处



