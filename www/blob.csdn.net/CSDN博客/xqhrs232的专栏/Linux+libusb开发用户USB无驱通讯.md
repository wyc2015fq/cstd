# Linux+libusb开发用户USB无驱通讯 - xqhrs232的专栏 - CSDN博客
2017年09月19日 10:41:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1354
原文地址::[http://blog.csdn.net/ymangu666/article/details/39500923](http://blog.csdn.net/ymangu666/article/details/39500923)
相关文章
1、[linux下交叉编译libusb的方法及编译一个使用了libusb库的test程序的方法](http://blog.csdn.net/xfc_1939/article/details/53422071)----[http://blog.csdn.net/xfc_1939/article/details/53422071](http://blog.csdn.net/xfc_1939/article/details/53422071)
2、
# [linux下libusb的安装与测试](http://blog.csdn.net/u012411027/article/details/56013447)----[http://blog.csdn.net/u012411027/article/details/56013447](http://blog.csdn.net/u012411027/article/details/56013447)
**3、**[libusb的使用教程和例子](http://www.cnblogs.com/Daniel-G/archive/2013/04/22/3036730.html)----[http://www.cnblogs.com/Daniel-G/archive/2013/04/22/3036730.html](http://www.cnblogs.com/Daniel-G/archive/2013/04/22/3036730.html)
4、libusb库（linux下亲测有效）----[http://download.csdn.net/download/qq_18854309/9874750](http://download.csdn.net/download/qq_18854309/9874750)
项目上需要将一个自己开发的设备通过USB接口 连接到Linux系统或Android系统的设备，然后通过发送命令来控制我们的设备操作。要求做到“即插即用”，不需要再安装驱动，于是想到用libusb库来做。 
在网上搜索了一些关于libusb的使用方法，写篇文章记录下开发过程，主要解决3个问题：
① libusb是什么？
② libusb有什么用？
③ libusb怎么用？
1. libusb是什么？libusb有什么用？
 usb驱动是内核态驱动硬件方面的东西； libusb是用户端程序封装库，是USB主机对USB设备进行操作的函数集合，有了它我们针对某型号USB设备就不用去修改比较繁琐的linux内核驱动了，方便了对设备的使用与调试。
  要使用libusb，需要在linux下安装、配置libusb环境。可以查看这篇文章：[linux下libusb的安装与测试](http://blog.csdn.net/zhengxianghdu/article/details/8925602)
  安装主要分为./configure ->make -> sudo make install
2. libusb怎么用？
  带着这个问题，找到了这篇文章：[USB设备开发----
 基于libusb的无驱设计](http://blog.csdn.net/saloon_yuan/article/details/7880768)，不过这篇文章也只是简单介绍，实际写程序帮助不大。
 那对写程序帮助最大的资源是什么呢？无疑是源码。于是把 libusb-1.0.9.tar.bz2 库解压出来，然后建立一个SourceInsight工程查看源码。
  在 examples/ 下有3个示例程序，查看 dpfp.c ，然后知道了①用 libusb_init(NULL);来初始化这个libusb库；②调用 libusb_open_device_with_vid_pid();通过usb设备的vid、pid来打开我们的usb设备；返回一个struct libusb_device_handle *devh 结构。
  既然已经打开了设备，按照以前写驱动程序的步骤，下面就是进行读写操作了。但是通过在源码中查找，没有read()、write()函数。通过研究，原来它是通过transfer来进行传输，就是上面的文章中介绍的 控制传输、块传输和中断传输来完成。③于是调用 libusb_bulk_transfer()来对输出端点进行数据输出，但是返回的结果是-1. 再阅读源码，发现在传输之前要进行接口确认。④
 调用 libusb_claim_interface()确认interface,但返回-6.说是被占用了。再研究，找到了这篇文章：[libusb下，多程序调用不同endpoint](http://bbs.csdn.net/topics/380113009) 
 ⑤被占用了，那就要调用 libusb_detach_kernel_driver()先让interface 与 kernel driver分离。然后再传输就成功鸟。
  既然要研究这个libusb库，索性就透彻一点，把网上找到的资料都整理一下，有个深刻的认识：
① 在网上找到了一篇用libusb来实现usb接口的游戏摇杆和按键与USB host通讯的文章：[usb接口游戏摇杆和按键](http://blog.sina.com.cn/s/blog_8a84e6d301015uh7.html) 
 ;文中实现了找到usb总线上的设备，通过比较vid、pid找到要通讯的设备。然后通过中断端点进行数据传输，当摇杆位置变化或者按键按下时，读取设备中断端点值。
该文章中使用的是libusb 0.1版本，现在都使用libusb 1.0版本的。它们之间有啥不同呢？
② 于是找到了这篇文章:[Libusb For Linux(Android) 学习 <二> 使用API1.0 编程](http://blog.sina.com.cn/s/blog_602f87700101f141.html) ;
   它与0.1 版本最明显的区别是：其function 都比0.1 API 多了个"lib"前缀.   这里的libusb_set_debug()方法比较好用。
③[libusb开发者指南](http://gashero.iteye.com/blog/703892)  ; 在这篇文章中介绍了libusb的各个API使用方法，不过也是 0.1 版本的，但是大体功能都差不多。                 
                              
接着一个问题又来了，libusb 怎么下载到linux设备上去用呢？
④ 这就要交叉编译了，可以查看这篇文章：[Ubuntu上交叉编译libusb (1.0.9 & compat-0.1.4对旧版本兼容usb.h包和库)](http://blog.csdn.net/tycool/article/details/18842367)
说明一下：--build=i686-linux表示该软件在x86平台上被编译
                 --host=arm-linux表示该软件编译完成后在arm平台上运行
                 --prefix后面为软件安装目录
 那在Andorid平台上怎么使用libusb呢？
⑤ 可以查看这篇文章：[只需 5 步移植 libusb 到 Beagleboard 上的 Android](http://blog.sina.com.cn/s/blog_4c451e0e010149g9.html)
现在Android版本上其实已经把这个libusb搞上去了，Android 喜欢把所有功能都自己实现一遍 toolbox -> busybox  bionic->gcc库 uevent+vold->udev.
那怎么操作呢？可以查看这篇文章:⑥[android usb host 读写USB设备](http://blog.csdn.net/centralperk/article/details/9312401) ；还有这篇详细的介绍了操作的步骤和API的使用：⑦ [Android下的USB
 Host介绍和开发](http://www.osedu.net/article/linux/2014-04-16/678.html)
;还有这篇也不错：
# [翻译Android USB HOST API](http://blog.csdn.net/kangear/article/details/38428121)  及这篇 ：[Android USB通信-实现lsusb](http://blog.csdn.net/kangear/article/details/38520265)

