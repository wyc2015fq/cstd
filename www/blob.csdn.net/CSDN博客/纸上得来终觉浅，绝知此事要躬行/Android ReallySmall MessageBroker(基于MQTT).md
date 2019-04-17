# Android ReallySmall MessageBroker(基于MQTT) - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年05月16日 16:09:31[boonya](https://me.csdn.net/boonya)阅读数：828标签：[java																[android																[mqtt](https://so.csdn.net/so/search/s.do?q=mqtt&t=blog)
个人分类：[Android																[ActiveMQ](https://blog.csdn.net/boonya/article/category/1714191)](https://blog.csdn.net/boonya/article/category/2496125)





**原文地址：[http://blog.sina.com.cn/s/blog_6c762bb30101bkfp.html](http://blog.sina.com.cn/s/blog_6c762bb30101bkfp.html)**

**一、下载RSMB**全称是ReallySmall MessageBroker。点击下面地址下载


[https://www.ibm.com/developerworks/community/groups/service/html/communityview?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070](https://www.ibm.com/developerworks/community/groups/service/html/communityview?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070)

**二、启动RSMB服务**

**下载之后，解压**

目录结构如下。

![[Android]RSMB安装部署](http://s4.sinaimg.cn/mw690/6c762bb3gd376a67646a3&690)

从上可以看出，因为我们下载的是适合所有平台的下载包，所以里面有linux，mac，windows等目录。打开windows目录，文件如下：

![[Android]RSMB安装部署](http://s1.sinaimg.cn/mw690/6c762bb3gd376adad40e0&690)

在命令行窗口输入broker，如下：

![[Android]RSMB安装部署](http://s4.sinaimg.cn/bmiddle/6c762bb3gd376b8637d03&690)

这个错误是我自己造成的啦。因为我把windows下的文件拷到另外一个目录了。再将message目录也拷贝过来。在命令窗口再输入borker，如下所示：

![[Android]RSMB安装部署](http://s11.sinaimg.cn/mw690/6c762bb3gd376c2c1a04a&690)

从上面看出，RSMB并没有启动。原来是因为我启动了同在这台机器的mosquitto，退出mosquitto。再次输入broker命令。

![[Android]RSMB安装部署](http://s12.sinaimg.cn/mw690/6c762bb3gd376db3b552b&690)

从上图可以看出MQTT启动了，端口是1883.




**三、下载ia92**
点击下面地址下载


[http://www-01.ibm.com/support/docview.wss?uid=swg24006006](http://www-01.ibm.com/support/docview.wss?uid=swg24006006)

**四、运行示例**
从上面地址下载后，解压，目录结构如下：


![[Android]RSMB安装部署](http://s16.sinaimg.cn/mw690/6c762bb3gd376f82a567f&690)

打开J2SE目录，有如下文件：

![[Android]RSMB安装部署](http://s7.sinaimg.cn/mw690/6c762bb3gd376fa021976&690)

在命令行窗口输入命令如下：

![[Android]RSMB安装部署](http://s8.sinaimg.cn/mw690/6c762bb3gd376fc4fcd97&690)

弹出一个窗口如下图：

![[Android]RSMB安装部署](http://s8.sinaimg.cn/mw690/6c762bb3gd376fde87427&690)

重复上面步骤，再打开一个同样的窗口，点击options栏，输入不同的Client Identifier:

![[Android]RSMB安装部署](http://s1.sinaimg.cn/mw690/6c762bb3gd3770b82fb80&690)

两个界面都点击connect按钮

![[Android]RSMB安装部署](http://s16.sinaimg.cn/mw690/6c762bb3gd3770fb59c5f&690)

我们再去看rsmb的命令行窗口，里面消息如下：

![[Android]RSMB安装部署](http://s3.sinaimg.cn/mw690/6c762bb3gd37714092a42&690)
![[Android]RSMB安装部署](http://s15.sinaimg.cn/mw690/6c762bb3gd37715b4257e&690)

表示client1，client2都连接到了RSMB服务。

接着在client1我在subscribetopic里输入要订阅的主题，譬如：test/hello/word，点击subscribe按钮。

在client2里面在Publish Messages里面输入同样的主题，输入消息内容，点击Publish。

![[Android]RSMB安装部署](http://s15.sinaimg.cn/mw690/6c762bb3gd377258008de&690)

结果如下：

![[Android]RSMB安装部署](http://s11.sinaimg.cn/mw690/6c762bb3gd37727d63c5a&690)

由上图可以看出，client1收到了一条消息。

由此，我们搭建RSMB收发消息成功了。上面的实践都得感谢[这位大哥的文章](http://dalelane.co.uk/blog/?p=938)。终于一步步做下来配置成功了啊。mosquitto搞了半个月也没成功。哎呀呀。




**五、Android示例**
现在我们得试试用Android手机能否接收到发送的消息了。

还是这位牛人，我们将[这里的代码](http://dalelane.co.uk/blog/?p=1599)抄下来就基本可以运行成功。手机也可以收到Publish的消息了。代码我自己整理了下，详见下篇博文。](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




