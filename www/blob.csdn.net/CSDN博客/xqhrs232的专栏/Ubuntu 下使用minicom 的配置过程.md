# Ubuntu 下使用minicom 的配置过程 - xqhrs232的专栏 - CSDN博客
2017年07月06日 18:32:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：289
原文地址::[http://www.cnblogs.com/emouse/archive/2012/03/20/2408243.html](http://www.cnblogs.com/emouse/archive/2012/03/20/2408243.html)
相关文章
1、[linux（ubuntu） USB转串口使用minicon](http://blog.csdn.net/c05170519/article/details/6129307)----[http://blog.csdn.net/c05170519/article/details/6129307](http://blog.csdn.net/c05170519/article/details/6129307)
mouse原创文章，转载请注明出处[http://www.cnblogs.com/emouse/](http://www.cnblogs.com/emouse/)
虚拟机安装的Ubuntu 11.10，打算直接在虚拟机中使用个超级终端类的软件来连接开发板，windows下面是超级终端，Linux下面就是minicom了。
1、minicom的安装
安装minicom很简单，sudo apt-get install minicom一个命令就可以自动下载安装，或者在Ubuntu软件中心里面搜索安装
![clip_image002](http://images.cnblogs.com/cnblogs_com/emouse/201203/20120320171823674.jpg)
2、设置串口
我的笔记本，用的是usb转串口，最简单的做法就是windows下面装好，然后虚拟机设置使用物理串口了，但是当时开着虚拟机，必须关了才能设置串口，就想看看直接在虚拟机中使用usb转串口行不行。
把usb转串口从主机断开连接，Ubuntu中竟然是直接包括PL2303（我用的usb转串口芯片）驱动的，首先可以先来看一下usb情况。命令如下：
root@emouse:~# lsusb
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 001: ID 1d6b:0001 Linux Foundation 1.1 root hub
Bus 002 Device 002: ID 0e0f:0003 VMware, Inc. Virtual Mouse
Bus 002 Device 003: ID 0e0f:0002 VMware, Inc. Virtual USB Hub
Bus 002 Device 004: ID 0e0f:0008 VMware, Inc. 
Bus 002 Device 005: ID 067b:2303 Prolific Technology, Inc. PL2303 Serial Port
可以看到已经识别了PL2303，接下来看一下串口的情况：
root@emouse:~# dmesg |grep ttyUSB
[11991.918691] usb 2-2.2: pl2303 converter now attached to ttyUSB0
可以查看USB已经连接
输入 minicom –s配置minicom
+-----[configuration]------+
            | Filenames and paths      |
            | File transfer protocols  |
            | Serial port setup        |
            | Modem and dialing        |
            | Screen and keyboard      |
            | Save setup as dfl        |
            | Save setup as..          |
            | Exit                     |
            | Exit from Minicom        |
            +--------------------------+
选择进入Serial port setup，按照之前查看的信息进行配置
![image](http://images.cnblogs.com/cnblogs_com/emouse/201203/201203201718249703.png)
一定要把硬件流关掉，否则无法连接，这个和windows下面是一致的，只是容易忽略。
enter推出，选择Save setup as dfl  保存为默认配置，注意需要root权限。
这样就配置好了。
然而我的会出现
minicom: cannot open /dev/ttyUSB0: 没有那个文件或目录
然而实际上这个文件夹下是有这个文件的，通过root权限也无法打开，目前这个没有找到这个问题的原因，网上看了一下，不是虚拟机好像是可以的。看来还得用添加串口硬件的方式解决。使用物理串口一般是ttyS0，和上面一样的方法查看一下配置修改一下就可以了。
★emouse 思·睿博客文章★ 原创文章转载请注明：http://emouse.cnblogs.com
分类: [电子设计点滴](http://www.cnblogs.com/emouse/category/326056.html),[嵌入式Linux开发](http://www.cnblogs.com/emouse/category/365167.html)
