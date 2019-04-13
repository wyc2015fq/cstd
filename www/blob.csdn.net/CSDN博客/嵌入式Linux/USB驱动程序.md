
# USB驱动程序 - 嵌入式Linux - CSDN博客

2014年03月27日 10:31:04[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：596个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



![](https://img-blog.csdn.net/20140327103158375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpcWlmYTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
1、USB驱动程序存在于不同的内核子系统（块设备、网络设备、字符设备等等）和USB主控制器之中。
2、USB核心为USB驱动程序提供了一个用于访问和控制USB硬件的接口，而不必考虑系统当前存在的各种不同类型的USB硬件控制器。

2.1 USB里的设备模型
Linux里一个很重要的概念是设备模型。对于驱动来说，设备的概念就是总线和与其相连的各种设备。在内核里，总线、设备、驱动也就是bus、device、driver是设备模型很重要的三个概念，它们都有自己专属的结构。在include／linux／devide．h里的定义为：
struct bus_type {……}；
struct device {……)；
struct device_driver {……}；
每次出现一个设备都要向总线注册，每次出现一个驱动，也要向总线注册。系统初始化时，应扫描连接许多设备，并为每一个设备建立一个struct device的变量。每一次都应有一个驱动程序，并要准备一个struct device_driver结构的变量。还要把这些变量加入相应的链表(如把device插入devices链表，driver插入drivers链表)。这样，通过总线就能找到每一个设备和每一个驱动。然而，假如计算机里只有设备却没有对应的驱动，那么设备将无法工作。反过来，倘若只有驱动却没有设备，驱动也起不了任何作用。对于USB设备，它可以在计算机启动以后再插入或者拔出计算机。由于device可以在任何时刻出现，而driver也可以在任何时刻被加载，所以，每当一个struct
 device诞生时，它就会去BUS的drivers链表中寻找自己的另一半。如果找到了匹配的设备，就调用device_bind_driver，并绑定好。
Linux设备模型中的总线落实在USB子系统里就是usb_bus_type，它在usb_init函数中可用retval=bus_register(&usb_bus_type)语句注册，而在driver.c文件里的定义如下：
![](http://www.eccn.com/uploads/article/201005/20100507093353316.jpg)
该函数的形参对应的就是总线两条链表里的设备和驱动。当总线上有新设备和驱动时，这个函数就会被调用。


