# libusb 介绍 - h13 - 博客园
驱动开发向来是内核开发中工作量最多的一块，随着USB设备的普及，大量的USB设备的驱动开发也成为驱动开发者手头上做的最多的事情。本文主要介绍Linux平台下基于libusb的驱动开发，希望能够给从事Linux驱动开发的朋友带来些帮助，更希望能够给其他平台上的无驱设计带来些帮助。文章是我在工作中使用libusb的一些总结，难免有错误，如有不当的地方，还请指正。 
    Linux 平台上的usb驱动开发，主要有内核驱动的开发和基于libusb的无驱设计。
对于内核驱动的大部分设备，诸如带usb接口的hid设备，linux本身已经自带了相关的驱动，我们只要操作设备文件便可以完成对设备大部分的操作，而另外一些设备，诸如自己设计的硬件产品，这些驱动就需要我们驱动工程师开发出相关的驱动了。内核驱动有它的优点，然而内核驱动在某些情况下会遇到如下的一些问题：
1 当使用我们产品的客户有2.4内核的平台，同时也有2.6内核的平台，我们要设计的驱动是要兼容两个平台的，就连makefile 我们都要写两个。
2 当我们要把linux移植到嵌入平台上，你会发现原先linux自带的驱动移过去还挺大的，我的内核当然是越小越好拉，这样有必要么。这还不是最郁闷的地方，如果嵌入平台是客户的，客户要购买你的产品，你突然发现客户设备里的系统和你的环境不一样，它没有你要的驱动了，你的程序运行不了，你会先想：“没关系，我写个内核驱动加载一下不就行了“。却发现客户连insmod加载模块的工具都没移植，那时你就看看老天，说声我怎么那么倒霉啊，客户可不想你动他花了n时间移植的内核哦
3 花了些功夫写了个新产品的驱动，挺有成就感啊，代码质量也是相当的有水准啊。正当你沉醉在你的代码中时，客服不断的邮件来了，“客户需要2.6.5内核的驱动，config文件我已经发你了” “客户需要双核的 2.6.18-smp 的驱动” “客户的平台是自己定制的是2.6.12-xxx “   你恨不得把驱动的源代码给客户，这样省得编译了。你的一部分工作时间编译内核，定制驱动
有问题产生必然会有想办法解决问题的人， libusb的出现给我们带来了某些方便，即节约了我们的时间，也降低了公司的成本。 所以在一些情况下，就可以考虑使用libusb的无驱设计了。
    下面我们就来详细讨论一下libusb, 并以写一个hid设备的驱动来讲解如何运用libusb,至于文章中涉及的usb协议的知识，限于篇幅，就不详细讲解了，相关的可自行查看usb相关协议。
一 libusb 介绍
   libusb 设计了一系列的外部API 为应用程序所调用，通过这些API应用程序可以操作硬件，从libusb的源代码可以看出，这些API 调用了内核的底层接口，和kernel driver中所用到的函数所实现的功能差不多，只是libusb更加接近USB 规范。使得libusb的使用也比开发内核驱动相对容易的多。
Libusb 的编译安装请查看Readme,这里不做详解
二  libusb 的外部接口
2.1 初始化设备接口
这些接口也可以称为核心函数，它们主要用来初始化并寻找相关设备。
usb_init
函数定义： void usb_init(void);
从函数名称可以看出这个函数是用来初始化相关数据的，这个函数大家只要记住必须调用就行了，而且是一开始就要调用的.
usb_find_busses 
函数定义： int usb_find_busses(void);
寻找系统上的usb总线，任何usb设备都通过usb总线和计算机总线通信。进而和其他设备通信。此函数返回总线数。
usb_find_devices
函数定义： int usb_find_devices(void);
寻找总线上的usb设备，这个函数必要在调用usb_find_busses()后使用。以上的三个函数都是一开始就要用到的，此函数返回设备数量。
usb_get_busses 
函数定义： struct usb_bus *usb_get_busses(void);
这个函数返回总线的列表，在高一些的版本中已经用不到了，这在下面的实例中会有讲解
2.2 操作设备接口
    usb_open
函数定义： usb_dev_handle *usb_open(struct *usb_device dev);
  打开要使用的设备，在对硬件进行操作前必须要调用usb_open 来打开设备，这里大家看到有两个结构体 usb_dev_handle 和 usb_device 是我们在开发中经常碰到的，有必要把它们的结构看一看。在libusb 中的usb.h和usbi.h中有定义。
  这里我们不妨理解为返回的 usb_dev_handle 指针是指向设备的句柄，而行参里输入就是需要打开的设备。
   usb_close
   函数定义： int usb_close(usb_dev_handle *dev);
   与usb_open相对应，关闭设备，是必须调用的, 返回0成功，<0 失败。
   usb_set_configuration
   函数定义： int usb_set_configuration(usb_dev_handle *dev, int configuration);
   设置当前设备使用的configuration，参数configuration 是你要使用的configurtation descriptoes中的bConfigurationValue, 返回0成功，<0失败( 一个设备可能包含多个configuration,比如同时支持高速和低速的设备就有对应的两个configuration,详细可查看usb标准) 
   usb_set_altinterface 
   函数定义： int usb_set_altinterface(usb_dev_handle *dev, int alternate);
   和名字的意思一样，此函数设置当前设备配置的interface descriptor，参数alternate是指interface descriptor中的bAlternateSetting。返回0成功，<0失败
   usb_resetep
   函数定义： int usb_resetep(usb_dev_handle *dev, unsigned int ep);
   复位指定的endpoint，参数ep 是指bEndpointAddress,。这个函数不经常用，被下面介绍的usb_clear_halt函数所替代。
   usb_clear_halt
   函数定义： int usb_clear_halt (usb_dev_handle *dev, unsigned int ep);
   复位指定的endpoint，参数ep 是指bEndpointAddress。这个函数用来替代usb_resetep
   usb_reset 
   函数定义： int usb_reset(usb_dev_handle *dev);
   这个函数现在基本不怎么用，不过这里我也讲一下，和名字所起的意思一样，这个函数reset设备，因为重启设备后还是要重新打开设备，所以用usb_close就已经可以满足要求了。
   usb_claim_interface
   函数定义： int usb_claim_interface(usb_dev_handle *dev, int interface);
   注册与操作系统通信的接口，这个函数必须被调用，因为只有注册接口，才能做相应的操作。
Interface 指 bInterfaceNumber. (下面介绍的usb_release_interface 与之相对应，也是必须调用的函数)
   usb_release_interface 
   函数定义： int usb_release_interface(usb_dev_handle *dev, int interface);
   注销被usb_claim_interface函数调用后的接口，释放资源，和usb_claim_interface对应使用。
   2.3 控制传输接口
   usb_control_msg
   函数定义：int usb_control_msg(usb_dev_handle *dev, int requesttype, int request, int value, int index, char *bytes, int size, int timeout);
   从默认的管道发送和接受控制数据
   usb_get_string 
   函数定义： int usb_get_string(usb_dev_handle *dev, int index, int langid, char *buf, size_t buflen);
   usb_get_string_simple 
函数定义：  int usb_get_string_simple(usb_dev_handle *dev, int index, char *buf, size_t buflen);
   usb_get_descriptor 
   函数定义： int usb_get_descriptor(usb_dev_handle *dev, unsigned char type, unsigned char index, void *buf, int size);
   usb_get_descriptor_by_endpoint
   函数定义： int usb_get_descriptor_by_endpoint(usb_dev_handle *dev, int ep, unsigned char type, unsigned char index, void *buf, int size);
   2.4 批传输接口
   usb_bulk_write 
   函数定义： int usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
    usb_interrupt_read 
函数定义： int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
2.5 中断传输接口
usb_bulk_write 
函数定义： int usb_bulk_write(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
usb_interrupt_read
函数定义： int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size, int timeout);
基本上libusb所经常用到的函数就有这些了，和usb协议确实很接近吧
From: [http://blog.csdn.net/abc19842008/archive/2008/03/13/2177078.aspx](http://blog.csdn.net/abc19842008/archive/2008/03/13/2177078.aspx)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/lxh19810116/archive/2008/11/18/3324509.aspx](http://blog.csdn.net/lxh19810116/archive/2008/11/18/3324509.aspx)
