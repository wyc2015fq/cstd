# 嵌入式linux系统中设备驱动程序 - maopig的专栏 - CSDN博客
2012年03月03日 18:51:22[maopig](https://me.csdn.net/maopig)阅读数：777
                
      嵌入式linux系统中设备驱动程序是内核的一部分，完成对设备初始、读写操作和控制等功能。驱动程序隐藏了
硬件设备的具体细节，对不同的设备提供一致的接口，这些接口通过file_Operation结构来定义，设计驱动程序的大部分工作就是
根据硬件结构来“填写”结构体中定义的函数。主要的函数包括open(),read(),ioctl,release等。 Open函数的主要功能是提供
给驱动程序初始化的能力，为以后的设备操作做好准备。read函数的主要功能是读取设备数据。Write函数的主要功能是写设备数据。
Ioctl函数的功能主要是执行读、写之外的操作，如：配置设备.release函数功能与open函数相反。
除以上几个函数之外，还有2个非常重要的函数module_init()和module_exit().驱动程序时从module_init()函数开始执行，它是在
加载模块时执行的，它的功能是：将初始化函数加入内核全局初始化函数列表中，在内核初始化时执行驱动的初始化函数，从而完成驱动的初始化
和注册，之后驱动便停止等待被应用软件调用。
[http://wenku.baidu.com/view/b2f173610b1c59eef8c7b4da.html](http://wenku.baidu.com/view/b2f173610b1c59eef8c7b4da.html)
