# 《精通linux设备驱动程序开发》——USB - 一世豁然的专栏 - CSDN博客





2015年04月01日 16:22:19[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1443








一、USB体系架构

 一）、简介

  1、USB通信协议采用主从结构，实现了主机控制器和外围设备的通信。USB主机控制器属于南桥芯片的一部分，通过PCI总线和处理器通信。



  2、PC机上的USB系统（P217  图11-1）和嵌入式设备上的USB系统（P217  图11-2）。




  3、嵌入式设备上的USB系统中的SOC内嵌了USB控制器，该控制器支持4条总线和3种操作模式。

   1）、总线1工作在主机模式下，通过（P217  图11-1）收发器和A型接口有限连接。该A型接口可用来连接笔驱动或者键盘。

   2）、总线2工作在主机模式下，其USB收发器连接的是内嵌USB设备。

   3）、总线3工作在设备模式下，通过USB收发器和B型接口有线连接，B型接口通过一条B-A线和主机连接。在该模式下，该嵌入式设备可以当USB笔驱动设备使用，同时也可

            为主机提供另一个存储分区。

   4）、总线4接的是OTG控制器。使用这种接口，既可以把笔驱动器连接到系统上，也可以把系统连接到另一台主机上。







 二）、总线速度

  1、USB传输的速度有三种：

   1）、USB1.0标准支持的速度是1.5MB/s，称为低速USB。

   2）、USB1.1标准支持的速度是12MB/s，称为全速USB。

   3）、USB2.0标准支持的速度是480MB/s，称为高速速USB。







 三）、主机控制器

  1、USB主机控制器分为以下几种：

   1）、UHCI（通用主机控制器接口）。

   2）、OHCI（开放主机控制器接口）。

   3）、FHCI（增强主机控制器接口）。

   4）、USB OTG控制器。







 四）、传输模式

  1、USB传输数据的模式有四种：

   1）、控制传输模式，用来传输外设和主机之间的控制、状态、配置等信息。

   2）、批量传输模式，传输大量时延要求不高的数据。

   3）、中断传输模式，传输数据量小，但是对传输时延敏感，要求马上相应。

   4）、等时传输模式，传输实时数据，传输速率要预先可知。







 五）、寻址

  1、USB设备里的每个可寻址单元称作端点。为每个端点分配的地址称为端点地址。

   1）、每个端点地址都有与之相关的传输模式。地址为0的端点专门用来配置设备。控制管道和它相连，完成设备枚举过程。


   2）、每个端点可以沿上行方向发送数据，也可以沿下行方向接收数据。沿上行方向从设备接收数据叫IN传输。 沿下行方向到达设备叫OUT传输。










二、Linux-USB子系统

  1、Linux-USB子系统架构。（P220  图11-3）




  2、Linux-USB子系统组成：

   1）、USB核心。

   2）、驱动不同主机控制器的HCD。

   3）、用于根集线器的hub驱动和一个内核辅助线程khubd。

   4）、用于USB客户设备的设备驱动程序。

   5）、USB文件系统usbfs，它能够让你从用户空间驱动USB设备。







三、驱动程序的数据结构

 一）、usb_device结构体

**   struct  usb_device  {**

**          /*  ......  */**

**          enum  usb_device_state  state;**

**          enum  usb_device_speed  speed;**

**          /*  ......  */**

**          struct  usb_device  *parent;**

**          /*  ......  */**

**          struct  usb_device_descriptor  descriptor;**

**          struct  usb_host_config  *config;**

**          struct  usb_host_config  *actconfig;**

**          /*  ......  */**

**          int  maxchild;**

**          struct  usb_device  *children[USB_MAXCHILDREN];**

**          /*  ......  */**

**       };**







 二）、URB

  1、URB是USB数据传输机制使用的核心数据结构。URB供USB协议栈使用。




  2、URB结构

**struct  urb   {**

**         struct  kref  kref;**

**         /* ......  */**

**         struct  usb_device  *dev;**

**         unsigned  int  pipe;**

**         int  status;**

**         unsigned  int  transfer_flags;**

**         void  *transfer_dma;**

**         dma_addr_t  trsanfer_dma;**

**         int  tranfer_buffer_length;**

**         /* ......  */**

**         unsigned  char  *setup_packet;**

**         /* ......  */**

**         int  interval;**

**         /* ......  */**

**         void  *context;**

**         usb_complete  complete;**

**         /* ......  */**

**       };**




  3、URB的使用分3步：分配内存，初始化，提交。

   1）、URB的内存是调用**usb_alloc_urb()**方法来分配的，该函数分配内存并将其置零，之后初始化URB相关的kobject和用于保护URB的自旋锁。

   2）、为了提交URB以便进行数据传输，需调用**usb_submit_urb()**函数。该函数异步提交URB。回调函数的地址作为参数传递给**usb_fill_[control | int | bluk]_urb()**函数。回

             调函数在URB提交过后被调用，负责完成检查提交状态、释放传输数据缓冲区等事情。

   3）、USB核心也提供了同步提交URB的接口函数：int  usb_[control | interrupt | bilk]_msg()。

   4）、URB的任务完成后，usb_free_urb()函数释放该实例。usb_unlink_urb()取消一个待处理的URB。







 四）、管道

  1、管道包括以下几个部分：

   1）、端点地址。

   2）、数据传输方向。

   3）、数据传输模式。




  2、管道的创建

   1）、USB核心提供现成的宏来创建管道。

      usb_[rcv | snd] [ctrl | int | bulk | isoc]pipe(struct  usb_device  *usb_dev,  __u8  endpointAddress);







 五）、描述符接口

  1、USB标准定义了一系列的描述符数据结构来保护设备的信息。Linux-USB核心定义的描述符有四种类型。

   1）、设备描述符存放设备的普通信息。usb_device_descriptor结构体表示设备描述符。

   2）、配置描述符用来描述设备配置模式。usb_config_descriptor结构体用于表示配置描述符。

   3）、接口描述符使得USB能支持多种功能。usb_interface_descriptor结构体表示接口描述符。

   4）、端点描述符存放设备最终的端点信息。usb_endpoint_descriptor结构体用于表示端点描述符。







四、枚举

  1、枚举过程是热插拔USB设备的其实步骤，该过程中，主机控制器获得设备的相关信息并配置好设备。在linux-USB子系统中，集线器驱动程序负责该枚举过程。







五、设备实例：遥测卡

 一）、简介

  1、遥控卡的寄存器

![](https://img-blog.csdn.net/20150401095907987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、USB遥测卡端点说明：

   1）、控制端点附在一个卡上配置寄存器中。

   2）、批量IN端点负责把卡收集到的遥测数据传递个给处理器。

   3）、批量OUT端点负责接收处理器传递过来的数据。







 二）、初始化和探测过程 

  1、USB驱动程序用probe()/disconnect()方法支持热插拔，用设备ID表识别驱动程序所支持的设备。

** struct  usb_device_id  {**

**        /*  ....  */**

**        __u16  idVendor;**

**        __u16  idProduct;**

**        /*  ....  */**

**        __u8  bDeviceClass;**

**        __u8  bDeviceSubClass;**

**        __u8  bDeviceProtocol;**

**        /*  ....  */**

**     };**

   1）、idVendor和idProduct分别表示厂商ID和产品ID。

   2）、bDeviceClass、bDeviceSubClass、bDeviceProtocol则基于设备的功能对其进行归类。




  2、遥测卡驱动程序初始化及代码解析（代码清单11-2）




  3、遥测卡探测和断开连接及代码解析（代码清单11-3）







 三）、寄存器访问

  1、当应用程序打开/dev/tele设备文件的时候，open()方法立刻初始化遥测卡的配置寄存器。




  2、usb_ctrlrequest结构体

**struct  usb_ctrlrequest  {**

**        __u8  bRequestType;**

**        __u8  bRequest;**

**        __le16  wValue;**

**        __le16  wIndex;**

**        __le16  wLength;**

**      }__attribute__((packed));**

   1）、bRequest域用来标识该控制请求。

   2）、bRequestType表示数据传输方向、请求类别以及数据接收方是何种类型。

   3）、wValue存放即将写到寄存器里的数据。

   4）、wIndex是寄存器的偏移地址。

   5）、wLength是传输数据的字节数。




  3、初始化遥测卡配置寄存器及代码解析（代码清单11-4）







 四）、数据传输

  1、遥测卡的数据交换（代码清单11-5）










六、类驱动程序

 一）、简介

  1、USB规范引入了设备类的概念，根据每一类驱动程序的功能吧USB设备分为几大类标准的几大类包括：

   1）、大容量存储类。

   2）、网络类。

   3）、集线器类。

   4）、串行转换器类。

   5）、音频类。

   6）、视频类。

   7）、图像类。

   8）、调制解调器。

   9）、打印机。

   10）、HID（人机接口设备）。

   每一大类的驱动程序对属于这类的所有设备通用，不需要另外开发和安装驱动程序就可以使用。




  2、每个USB设备都有类代码和子类代码。设备驱动程序的usb_device_id结构体就包含类代码成员和子类代码成员。也可以从/proc/bus/usb/device的输出结果“I：”行看到设

        备 的类代码和子类代码信息。







 二）、大容量存储设备

  1、通常来讲，USB大容量存储设备主要指USB硬盘、笔驱动器、CD-ROM、软驱以及类似的存储设备。USB大容量存储设备利用了SCSI（小型计算机系统接口）协议和主机

        系统通信。（P237  图11-4）




  2、SCSI子系统的结构分为3层：

   1）、位于顶层的设备驱动程序。

   2）、扫描总线，配置设备和将底层驱动程序相关联的中层驱动程序。

   3）、底层SCSI适配器驱动程序。




  3、大容量存储驱动程序把自己注册成一个虚拟的SCSI适配器。该虚拟适配器在上行方向上通过SCSI命令和上层通信，在下行方向上通过URB与块存储器交换数据。




  4、让usbfs中也能显示设备结点。（代码清单11-6）







 三）、USB-串行端口转换器

  1、USB-串行端口转换器能把USB接口转换成串行端口。（P241  图11-5）




  2、USB-串行端口转换器驱动程序与其他USB设备驱动程序相似，其除了利用了USN核心外，还利用了USB-Serial核心。

   1）、USB-Serial核心提供的功能如下：

    I、tty驱动程序把底层USB-串行端口转换器驱动程序和高层的串行端口层分开。

    II、通用的probe()和disconnect()函数能被所有的USB-串行端口转换器驱动程序共用。

    III、有了设备结点，从用户空间就能访问USB-串行端口转换器。应用程序通过/dev/tty/USBX设备结点就能操作USB-串行端口转换器。

   2）、底层USB-串行端口驱动程序功能如下：

    I、调用usb_serial_register()函数向USB-Seral核心注册usb_serial_driver结构体。usb_serial_driver结构体的入口点是驱动程序的核心。

    II、初始化一个usb_driver结构体并调用usb_register()函数向USB核心注册。除了USB-串行端口转换器能利用USB-Seral核心提供的probe()和disconnect()函数外，其他的

          过程和遥测卡的驱动程序很相似。




  3、FTDI驱动程序代码片段。（代码清单11-7）




 四）、人机接口设备




 五）、蓝牙







七、gadget驱动程序

  1、USB gadget驱动程序使嵌入式linux系统设备可以工作在USB设备模式。




  2、USB gadget支持属于内核的一部分，包括以下几项内容：

   1）、用于集成SoC的USB设备控制器的驱动程序。

   2）、针对存储设备、网络设备和串行端口转换器使用的驱动程序。

   3）、框架性驱动可以用来测试设备控制器驱动程序。







八、调试（略）




