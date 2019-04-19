# 嵌入式Linux系统下I2C设备驱动程序的开发 - xqhrs232的专栏 - CSDN博客
2011年10月26日 20:35:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1097标签：[c																[linux																[嵌入式																[数据结构																[command																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.cnblogs.com/shenhaocn/archive/2011/03/19/1989155.html](http://www.cnblogs.com/shenhaocn/archive/2011/03/19/1989155.html)
 相关网页::Linux I2C 驱动分析----[http://www.cnitblog.com/luofuchong/archive/2011/06/30/48280.html](http://www.cnitblog.com/luofuchong/archive/2011/06/30/48280.html)
摘    要】I2C总线是一种很通用的总线，具有简单、高效等特点，广泛应用在各种消费类电子产品及音视频设备上，在嵌入式系统的开发中也经常用到。本文分析了嵌入式linux系统中I2C驱动程序的结构，并结合一个具体的I2C时钟芯片DS1307，说明在嵌入式linux系统下开发I2C设备驱动程序的一般流程。
【关键字】I2C总线    嵌入式linux    驱动开发 
**1、I2C总线简介**
I2C (Inter-Integrated Circuit)总线是一种由PHILIPS公司开发的两线式串行总线，用于连接微控制器及其外围设备。I2C总线最主要的优点就是简单性和有效性。
**1.1    I2C总线工作原理 **
I2C总线是由数据线SDA和时钟SCL构成的串行总线，各种被控制器件均并联在这条总线上，每个器件都有一个唯一的地址识别，可以作为总线上的一个发送器件或接收器件(具体由器件的功能决定) [1]。I2C总线的接口电路结构如图1所示。
![嵌入式Linux系统下I2C设备驱动程序的开发 - 郎行拂晓 - 郎行拂晓的博客](http://www.51kaifa.com/upload/eWebUpload/20060728103410673.gif)
图1    I2C总线接口电路[1]
**1.2    I2C总线的几种信号状态[1]**
1.    空闲状态：SDA和SCL都为高电平。
2.    开始条件(S)：SCL为高电平时，SDA由高电平向低电平跳变，开始传送数据。
3.    结束条件(P)：SCL为低电平时，SDA
由低电平向高电平跳变，结束传送数据。
4.    数据有效：在SCL的高电平期间， SDA保持稳定，数据有效。SDA的改变只能发生在SCL的底电平期间。 
5.    ACK信号: 数据传输的过程中，接收器件每接收一个字节数据要产生一个ACK信号，向发送器件发出特定的低电平脉冲，表示已经收到数据。
**1.3    I2C总线基本操作 **
I2C总线必须由主器件（通常为微控制器）控制，主器件产生串行时钟（SCL），同时控制总线的传输方向，并产生开始和停止条件。
数据传输中，首先主器件产生开始条件，随后是器件的控制字节（前七位是从器件的地址，最后一位为读写位 ）。接下来是读写操作的数据，以及 ACK响应信号。数据传输结束时，主器件产生停止条件[1]。具体的过程如图2所示。
![嵌入式Linux系统下I2C设备驱动程序的开发 - 郎行拂晓 - 郎行拂晓的博客](http://www.51kaifa.com/upload/eWebUpload/20060728103453550_s.gif)
图2 完整的I2C数据传输过程[1]
**2 . Linux下I2C驱动程序的分析2.1 Linux系统I2C驱动的层次结构**
Linux系统对I2C设备具有很好的支持，Linux系统下的I2C驱动程序从逻辑上可以分为3个部分：
1. I2C总线的驱动 I2C core ：实现对I2C总线、I2C adapter及I2C driver的管理。
2. I2C控制器的驱动 I2C adapter ：针对不同类型的I2C控制器 ，实现对I2C总线访问的具体方法。
3. I2C设备的驱动 I2C driver ：针对特定的I2C设备，实现具体的功能，包括read, write以及ioctl等对用户层操作的接口。
这三个部分的层次关系如图3和图4所示。
![嵌入式Linux系统下I2C设备驱动程序的开发 - 郎行拂晓 - 郎行拂晓的博客](http://www.51kaifa.com/upload/eWebUpload/2006072810355127.gif)
**2.2    I2C 总线驱动 I2C core **
I2C core是Linux内核用来维护和管理的I2C的核心部分，其中维护了两个静态的List，分别记录系统中的I2C driver结构和I2C adapter结构。I2C core提供接口函数，允许一个I2C adatper，I2C driver和I2C client初始化时在I2C core中进行注册，以及退出时进行注销。同时还提供了I2C总线读写访问的一般接口（具体的实现在与I2C控制器相关的I2C adapter中实现），主要应用在I2C设备驱动中。
**2.3    I2C控制器的驱动 I2C adapter **
I2C adapter是针对不同类型I2C控制器硬件，实现比较底层的对I2C总线访问的具体方法。I2C adapter 构造一个对I2C core层接口的数据结构，并通过接口函数向I2C core注册一个控制器。
I2C adapter主要实现对I2C总线访问的算法，iic_xfer() 函数就是I2C adapter底层对I2C总线读写方法的实现。同时I2C adpter 中还实现了对I2C控制器中断的处理函数。
**2.4    I2C设备的驱动 I2C driver**
I2C driver中提供了一个通用的I2C设备的驱动程序，实现了字符类型设备的访问接口，对设备的具体访问是通过I2C adapter来实现的。I2C driver构造一个对I2C core层接口的数据结构，通过接口函数向 I2C Core注册一个I2C设备驱动。同时I2C driver 构造一个对用户层接口的数据结构，并通过接口函数向内核注册为一个主设备号为89的字符类型设备。
I2C driver实现用户层对I2C设备的访问，包括open，read，write，ioctl，release等常规文件操作，我们可以通过open函数打开 I2C的设备文件，通过ioctl函数设定要访问从设备的地址，然后就可以通过 read和write函数完成对I2C设备的读写操作。
通过I2C driver提供的通用方法可以访问任何一个I2C的设备，但是其中实现的read，write及ioctl等功能完全是基于一般设备的实现，所有的操作数据都是基于字节流，没有明确的格式和意义。为了更方便和有效地使用I2C设备，我们可以为一个具体的I2C设备开发特定的I2C设备驱动程序，在驱动中完成对特定的数据格式的解释以及实现一些专用的功能。
**3.       一个具体的I2C设备驱动程序的开发**
DS1307是一款小巧的I2C接口的实时时钟芯片，具有低功耗，全BCD码时钟和日历输出， 12 /24小时工作模式，时分秒、星期、年月日计时数据，润年自动补偿，有效期至2100年，外加56 Bytes的NV RAM（非易失性的RAM）等特点[3]。下面以DS1307为例，说明一个具体的I2C设备驱动程序的设计要点。
**3.1    I2C设备驱动程序的一般结构**
一个具体的I2C设备驱动需要实现两个方面的接口，一个是对I2C core层的接口，用以挂接I2C adapter层来实现对I2C总线及I2C设备具体的访问方法，包括要实现attach_adapter，detach_client，command等接口函数。另一个是对用户应用层的接口，提供用户程序访问I2C设备的接口，包括实现open，release，read，write以及最重要的ioctl等标准文件操作的接口函数。
对I2C core层的接口函数的具体功能解释如下：
attach_adapter：I2C driver在调用I2C_add_driver() 注册时，对发现的每一个I2C adapter（对应一条I2C 总线）都要调用该函数，检查该I2C adapter是否符合I2C driver的特定条件，如果符合条件则连接此I2C adapter，并通过I2C adapter来实现对I2C总线及I2C设备的访问。
detach_client：I2C driver在删除一个I2C device时调用该函数，清除描述这个I2C device的数据结构，这样以后就不能访问该设备了。
command：针对设备的特点，实现一系列的子功能，是用户接口中的ioctl功能的底层实现。
**3.2    DS1307驱动程序实现对I2C core层的接口**
在驱动中必须实现一个struct i2c_driver 的数据结构，并在驱动模块初始化时向I2C core注册一个I2C驱动，并完成对I2C adapter的相关操作。
struct i2c_driver ds1307_driver =
{
name: "DS1307", 
id: I2C_DRIVERID_DS1307,
flags: I2C_DF_NOTIFY,
attach_adapter:ds1307_probe,
detach_client:ds1307_detach,
command: ds1307_command  
};
数据结构ds1307_driver中的name:"DS1307"，Id:I2C_DRIVERID_DS1307用来标识DS1307驱动程序。flags: I2C_DF_NOTIFY表示在I2C总线发生变化时通知该驱动。
ds1307_probe对应i2c_driver数据结构中的attach_adapter，主要功能：调用 I2C core 层提供的i2c_probe函数查找一条I2C总线，看是否有DS1307的设备存在，如果存在DS1307，则将对应的I2C adapter 和DS1307设备挂接在一起，并通过该I2C adapter来实现对DS1307的访问。同时使能DS1307, 并调用i2c_attach_client()向I2C core层注册DS1307。
ds1307_detach对应i2c_driver数据结构中的detach_client，主要功能：调用i2c_detach_client() 向I2C core层注销DS1307，并不使能DS1307，这样I2C驱动就不能访问DS1307了。
ds1307_command对应i2c_driver数据结构中的command ，主要功能：针对DS1307时钟芯片的特点，实现一系列的诸如DS1307_GETTIME ，DS1307_SETTIME，DS1307_GETDATETIME，DS1307_MEM_READ，DS1307_MEM_WRITE等子功能，是用户接口中的ioctl功能的底层实现。
以上3个接口函数使DS1307的驱动程序实现了对I2C 总线及I2C adpater的挂接，因此就可以通过I2C core的提供对I2C总线读写访问的通用接口，来开发实现DS1037驱动程序对用户应用层的接口函数。
**3.3    DS1307驱动程序实现对用户应用层的接口**
在驱动中必须实现一个struct file_operations 的数据结构，并向内核注册为一个字符类型的设备（用单独的主设备号来标识），或者注册为一个miscdevice设备（所有miscdevice设备共同一个主设备号，不同的次设备号，所有的miscdevice设备形成一个链表，对设备访问时根据次设备号查找对应的miscdevice设备，然后调用其struct file_operations中注册的应用层接口进行操作）。
struct file_operations rtc_fops = 
{
owner: THIS_MODULE,
ioctl: ds1307_rtc_ioctl,
read: ds1307_rtc_read,
write: ds1307_rtc_read,
open: ds1307_rtc_open,
release: ds1307_rtc_release 
};
数据结构rtc_fops 中的ds1307_rtc_open 和ds1307_rtc_release对应file_operations中的open和release，分别用来打开和关闭DS1307。
ds1307_rtc_ioctl对应file_operations中的ioctl，对用户提供的一系列控制时钟芯片的具体命令：RTC_GET_TIME： 以固定的数据格式读取实时时钟的时间。RTC_SET_TIME：以固定的数据格式设定实时时钟的时间。RTC_SYNC_TIME：系统时钟和实时时钟之间的时间同步。
ds1307_rtc_read 对应对应file_operations中的read，实现与ds1307_rtc_ioctl 的子功能RTC_GET_TIME相同的功能，以及从NV RAM读取数据。
ds1307_rtc_write 对应file_operations中的write，实现与ds1307_rtc_ioctl的子功能 RTC_SET_TIME相同的功能，以及将数据写入NV RAM。
**3.4    DS1307驱动程序的加载和测试**
在DS1307驱动模块的初始化函数ds1307_init()中，首先通过i2c_add_driver(&ds1307_driver)向I2C core层注册一个I2C的设备驱动，然后再通过misc_register (&ds1307_rtc_miscdev)将DS1307注册为一个miscdevice设备，这样用户程序就可以通过主设备号10 次设备号 135的设备节点/dev/rtc来访问DS1307了。
将DS1307的驱动程序编译成模块的方式，通过insmod命令加载进内核，然后用测试代码进行测试，DS1307驱动程序中实现的所有功能都达到了预期的效果。
由于DS1307驱动程序在底层实现了对DS1307时钟芯片数据的解释和转换，所以在用户程序中得到的就是有固定格式和意义的数据，这样就方便了用户程序的访问，提高了应用开发的效率。
**4．总结**
I2C总线是一种结构小巧，协议简单的总线，应用很广泛，访问起来简单方便。linux系统下I2C的驱动程序具有清晰的层次结构，可以很容易地为一个特定的I2C设备开发驱动。本文通过对linux系统下I2C驱动，以及一个具体的DS1307时钟芯片驱动结构的分析，基本上可以很清楚看出一个I2C设备驱动的开发过程。实现的关键分为两个部分，1. 对I2C core的接口，必须实现 struct i2c_drvier数据结构中的几个特定的功能函数。这些函数是I2C驱动与I2C总线物理层（I2C控制器）和I2C设备器件之间通信的基础。2.
 对用户应用层的接口，必须实现struct file_operation数据结构中的一些特定功能的函数，如 open ，release , read ,write，lseek等函数。以上两类接口中，对I2C core的接口是对I2C设备访问的基础，实现对I2C总线具体的访问方法；对用户应用层的接口则是方便应用程序开发，实现设备特定功能的必不可少的部分。
//=============================================
备注：
1》应用程序怎么跟I2C驱动通信（交互）----通过misc_register (&ds1307_rtc_miscdev)将DS1307注册为一个miscdevice设备，这样用户程序就可以通过主设备号10 次设备号135的设备节点/dev/rtc来访问DS1307了。
