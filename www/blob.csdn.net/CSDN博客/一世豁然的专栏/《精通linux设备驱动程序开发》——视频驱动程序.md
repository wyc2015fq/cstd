# 《精通linux设备驱动程序开发》——视频驱动程序 - 一世豁然的专栏 - CSDN博客





2015年04月03日 10:56:04[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2309








一、显示架构

  1、PC系统的显示连接。（P247  图12-1）




  2、VGA（视频图形阵列）是IBM提出的早期显示标准。VGA的分辨率是640X480，而更新的标准比如SVGA（高级视频图形阵列）和XGA（扩展图形阵列）分别支持更高的

        800X600和1024X768的分辨率。嵌入式设备上常用具有320X240分辨率的QVGA面板。




  3、视频电缆标准

   1）、模拟显示器。

   2）、数字平面显示器。

   3）、与DVI（数字视频接口）规范兼容的显示器。

   4）、与HDTV（高清电视）规范兼容的显示器。




  4、嵌入式系统的显示连接。（P249  图12-2）




  5、嵌入式系统的LVDS和DVI连接。（P249  图12-3）







二、Linux视频子系统

  1、帧缓冲的概念解决了低可移植性和冗余代码导致的大量投入和维护开销的问题。从而开发成员可以以与平台无关的方式编写应用层和较高内核层的程序。（P250  图12-4）




  2、Linux视频子系统。（P250  图12-5）

   1）、此子系统包括底层显示驱动程序、中间层帧缓冲和控制台、高层虚拟终端驱动程序、X Windows的用户模式驱动程序和配置显示参数程序。

   2）、X Windows  GUI操作视频卡的方式有两种：或者使用适用于相应视频卡的内建用户空间驱动程序，或者工作在帧缓冲子系统之上。

   3）、文本模式控制台工作在虚拟终端字符驱动程序之上。







三、显示参数

  1、有时为了让设备能播放视频，在编写驱动程序的时候，只需要配置与显示面板的属性即可。




  2、fbset命令来显示特性

![](https://img-blog.csdn.net/20150402104325454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、D:值表示dotclock，它是视频硬件在显示器上绘制像素的速率。

   2）、值65.003MHz表示视频控制器绘制一个像素需要用时约15384ps，这个时间称为pixclock，是timings行中的第一个数值参数。

   3）、geometry（几何形状）后面的数表示可视的分辨率为1024X768（SVGA），一个像素的信息需8bit来保存。

   4）、H：后面的值是为水平扫描率，它是视频硬件每秒扫描的水平显示线数目，是pixclock乘以X次方向分辨率的积德倒数。

   5）、V：值是整屏的刷新率，是pixclock、X方向分辨率、Y方向分辨率三者的积的倒数。

   6）、视频控制器在每行结束时发一个水平同步脉冲（HSYNC），在每帧结束时发一个垂直同步脉冲（VSYNC）。







四、帧缓冲API

  1、帧缓冲核心层向用户空间输出设备结点，一边应用程序能访问每个支持的视频设备。




  2、struct  fd_var_screeninfo结构体（视频卡的属性）

![](https://img-blog.csdn.net/20150402110527588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、struct  fd_fix_screeninfo结构体（视频硬件的固定信息）

![](https://img-blog.csdn.net/20150402110746915?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  4、struct  fd_cmap结构体（颜色映射）

**     struct  fd_cmap  {**

**        __u32  start;**

**        __u32  len;**

**        __u16  *read;**

**        __u16  *green;**

**        __u16  *blue;**

**        __u16  *transp;**

**     }；**




  5、以硬件无关的方式清除显示屏（代码清单12-1）










五、帧缓冲驱动程序

 一）、导航系统简介

  1、一个Linux导航设备的显示系统。（P254  图12-6）

   1）、工作过程：GPS接收机通过UART接口向SOC发送坐标数据流，应用程序根据收到的位置信息产生图像，并更新系统内存中的帧缓冲。帧缓冲驱动程序通过DMA方式将

             这幅图像数据传输到作为SoC LCD控制器一部分的显示缓冲区，控制器将像素数据传输到QVGA LCD面板进行显示。




  2、导航系统被设计成此结构：GPS应用程序运行在LCD控制器的底层帧缓冲驱动程序上。应用程序通过dev/ttySX从GPS接收机获得位置，这里的X是连接到接收机的UART

        号。然后它将几何定位信息转换成图像，并将像素数据写入与LCD控制器相关的帧缓冲。




  3、LCD控制器的寄存器。（P255  表12-1）








 二）、数据结构

  1、fd_info结构体

![](https://img-blog.csdn.net/20150402144308757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150402144342734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、fd_ops结构体

![](https://img-blog.csdn.net/20150402144802166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150402144856420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150402144903627?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 三）、检查和设置参数

  1、fd_check_var()方法执行变量的合法性检查。同时也设置合适的RGB格式。




  2、fd_set_par()方法根据fd_check_var中的值设置LCD控制器的寄存器：

   1）、HSYC_REG中的水平同步时间，左空边，右空边。

   2）、VSYC_REG中的垂直同步时间，上空边，下空边。

   3）、SIZE_REG中的可见区X、Y分辨率。

   4）、MNA_REG中的DMA参数。




  3、GPS应用程序试图将QVGA的分辨率改为50X50。







 四）、屏幕消隐

  1、fd_blank()方法支持显示消隐和去消隐，主要用于电源管理。







 五）、加速方法

  1、如果用户接口需要执行繁重的视频操作，可能就需要通过图形加速来获得可以接受的性能。




  2、fd_imageblit()方法在显示器上画一幅图像，这个函数为驱动程序提供了一个能利用视频控制器所拥有的某些特别能力的机会，以加速这种操作。如果硬件不支持加速功

        能，cfd_imageblit()就是帧缓冲核心为加速提供的通用库函数。




  3、fd_copyarea()将屏幕的一个矩形区域复制到另一个区域。如果图形控制器没有任何可加速这个操作的能力，那么fd_copyarea()就为这个操作提供优化的方法。




  4、fd_fillrect()方法能快速用像素行填充矩形区。cfd_fillrecf()方法则是这种操作的一个通用的非加速方法。







 六）、从帧缓冲DMA

  1、导航系统的LCD控制器包含了一个DMA引擎，其能从系统内存抓取图形帧。控制器将得到的图像数据发到显示面板。DMA的速率支撑了显示器的刷新率。




  2、适合并发访问的禁止cache的帧缓冲区是从myfd_probe()中用dma_alloc_coherent()分配的。my_set_par()将这个已分配的DMA地址写入LCD控制器的DMA_REG寄存器。




  3、当驱动程序启用DMA时，控制器就通过同步DMA方式从帧缓冲不断将像素数据转送到显示器。所以，当GPS应用程序映射帧缓冲并写入位置信息时，像素就会被绘制到

         LCD上。







 七）、对比度和背光

  1、导航系统中的LCD控制器通过使用CONTRAST_REG寄存器来控制对比度。驱动程序通过myfd_ioctl()向用户空间提供这个寄存器接口。




  2、导航系统的LCD面板通过背光照亮，处理器通过DPIO线控制背光，因此设置相应引脚的电平就可以开/关背光。内核通过sysfs结点将其抽象为一般背光接口。为了利用这

        个接口，驱动程序必须实现backlight_ops结构体，而且该结构体中要包含获得和改变背光亮度的函数，然后通过调用backlight_device_regisiter将其注册到内核。




  3、导航系统的帧缓冲驱动程序。（P260  代码清单12-2）










六、控制台驱动程序

 一）、简介

  1、控制台是用于显示内核产生的printk()消息的一种设备。控制台包含两层，顶层驱动程序以及负责高级操作的底层驱动程序。




  2、控制台驱动程序主要使用两种接口定义的结构。顶层控制台驱动程序以struct  console为主，它定义了基本操作，底层驱动程序以struct  consw为主，它指定了高级操作。

![](https://img-blog.csdn.net/20150403103438956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150403103412767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  2、两层控制台驱动程序的结构

![](https://img-blog.csdn.net/20150403103956542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





 二）、设备实例：手机

  1、USB_UART上的控制台（代码清单12-3）




 三）、启动logo

  1、帧缓冲子系统的一个特色是可以启动logo。为了显示这个logo，在内核配置阶段启用CONFIG_LOGO选项并选择一个可用的logo。




  2、CLUT224是常用的启动logo的图片形式，它支持224种颜色。一个CLUT224图片是包含以下两种结构的C文件。

   1）、CLUT，它是一个224个RGB组的字符矩阵。每个3字节的CLUT单元是红、绿、蓝3种颜色的组合。

   2）、数据矩阵，它的每个字节是CLUT表的索引，索引从32开始，一直到225结束。32指向CLUT表中的第一个单元。logo操作码从与数据矩阵每个索引对应的CLUT组创建

             帧缓冲像素数据，图像显示由底层帧缓冲驱动程序的fd_inmagelit()方法完成。




  3、其他支持logo格式包括16色的vga16和黑白的单色。scripts/目录下的脚本可将标准的PPM文件转换成支持的logo格式。







七、调试（略）







