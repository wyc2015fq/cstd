# Linux下USB驱动基础 - 一世豁然的专栏 - CSDN博客





2014年12月28日 20:13:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：794标签：[Linux驱动](https://so.csdn.net/so/search/s.do?q=Linux驱动&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)









本博客转载于：[http://blog.chinaunix.net/uid-9185047-id-445196.html](http://blog.chinaunix.net/uid-9185047-id-445196.html)








USB是主机和外围设备之间的一种连接。USB最初是为了替代各种各样的不同的接口的低速总线而设计的。（例如：串口，并口，键盘连接等）。它以单一类型的总线连接各种不同类型的设备。


USB拓扑机构不是以总线方式的。而是一棵由几个点对点的连接构成的树。连接线由4根电缆组成（电源，地线，两个数据线）


USB主控制器（Host Controller）负责询问每一个USB设备是否有数据需要发送。也就是说：一个USB设备在没有主控制器要求的情况下是不能发送数据的。


USB协议规范定义了一套**任何特定类型的设备**都可以遵循的标准。如果一个设备遵循该设备，就不需要一个特殊的驱动程序。这些不同的**特定类型**称之为类（class）.例如：存储设备，键盘，鼠标，游戏杆，网络设备等。对于不符合这些类的其他设备。则需要对此设备编写特定driver.



**USB设备构成：**

Linux Kernel提供了USB Core来处理大部分USB的复杂性。写USB驱动，Sam觉得就是把USB硬件设备和USB Core之间给沟通起来。


USB协议把一个硬件USB设备用以下各个定义勾画出来。

**概念一. USB 端点（endpoint）**

USB endpoint只能往一个方向传送数据。从主机到设备（输出Endpoint）或从设备到主机（输入Endpoint）。一个Endpoint可以看作一个单向的管道。


有四种类型Endpoint，他们的区别在于传送数据的方式：
**控制Endpoint：**

用来控制对USB设备不同部分的访问。他们通常用于配置设备，获取设备信息，发送命令到设备，或者获取设备的状态报告。每个USB设备都有一个名为：Endpoint0的控制Endpoint。USB Core使用该Endpoint0在插入时进行设备的配置。

**中断Endpoint：**

每当USB主控制器要求设备传输数据时，中断Endpoint就以一个固定的速率来传送少量的数据。

USB Keyboard和Mouse通常使用中断Endpoint。

请注意，中断Endpoint和中断不同，它还是无法主动向USB主控制器发送数据。二是需要等待USB主控制器轮询。

**批量Endpoint：**

Bulk Endpoint用来传输大批量的数据。USB规范不保证数据能在规定时间内传输完成。但保证数据完整性。通常打印机，存储设备和网络设备使用之。

**等时Endpoint：**

用来传输大批量数据，但数据是否能够到达，怎无法保证。

通常数据收集设备会使用之。

**Sam觉得，其实一个设备有多少个以及什么类型的Endpoint。其实是硬件设备在制造阶段已经定好的。 USB Core只是去读取了这些信息，并把这些信息传送给USB driver.**


Linux Kernel中使用struct usb_host_endpoint来描述USB Endpoint。但其实其中的struct **usb_endpoint_descriptor**才是真正的描述Endpoint的。

struct usb_endpoint_descriptor

{

__u8 bLength;

__u8 bDescriptorType;

__u8 **bEndpointAddress**;

__u8 **bmAttributes;**

__le16 **wMaxPacketSize;**

__u8 **bInterval;**__u8 bRefresh;

__u8 bSynchAddress;

} __attribute__ ((packed));

**bEndpointAddress：**

//此Endpoint USB地址。它还包含了Endpoint方向信息。通过掩码USB_DIR_OUT和USB_DIR_IN判断是输出Endpoint还是输入Endpoint。

**bmAttributes;**

Endpoint Type，也可以通过掩码：USB_ENDPOINT_XFER_ISOC等判断此Endpoint是中断，等时，控制还是批量Endpoint。

**wMaxPacketSize;**

该Endpoint一次可以处理的最大字节数。虽然driver可以传送更大的数据，但实际传送时，还是会分割成这个大小。

**bInterval：**

如果是中断Endpoint，它就是Endpoint的间隔设置。也就是说，中断请求间隔时间。以毫秒为单位。


**概念二：接口（Interface）**

数个Endpoint被捆绑为一个USB Interface。

一个 USB Interface只对应一个逻辑连接，例如鼠标，键盘或者音频流。一个USB设备可以对应多个Interface。例如Sam见过的鼠标键盘一起的设备，就有2个Interface，一个键盘，一个鼠标。

另外，有些USB扬声器有2个Interface，一个键盘，一个音频流。


注意：每个USB drver只处理一个USB Interface。所以，一个设备也许会对应多个driver.

所以，USB Core在处理USB设备插入时，会针对不同的Interface唤醒它认为合适的driver。并以参数的形式把interface传递给drver.


Linux Kernel使用struct usb_interface来描述USB Interface。但Interface参数照例是其中的usb_interface_descriptor。


struct usb_interface

{
**struct usb_host_interface *altsetting;struct usb_host_interface *cur_altsetting;unsigned num_altsetting;**

struct usb_interface_assoc_descriptor *intf_assoc;

int minor;

enum usb_interface_condition condition;

unsigned is_active:1;

unsigned needs_remote_wakeup:1;

struct device dev;

struct device *usb_dev;

int pm_usage_cnt;

};

**struct usb_host_interface *altsetting;**

Interface结构体数组，包含了所有可能用于该Interface的可选配置。
**struct usb_host_interface *cur_altsetting;**

可选配置数
**unsigned num_altsetting;**

指向**altsetting**的指针。当前的Active 设置。




**usb_interface_descriptor:Interface描述符**

**struct usb_interface_descriptor** {
** __u8  bLength;**

**//描述符的字节长度。协议里规定，每个描述符必须以一个字节打头来表明描述符的长度。接口描述符的bLength应该是9**


** __u8  bDescriptorType;**

**//描述符的类型。各种描述符的类型都在ch9.h， * Descriptor types ... USB 2.0 spec table 9.5**



**__u8  bInterfaceNumber**;


//接口号。每个配置可以包含多个接口，这个值就是它们的索引值


** __u8  bAlternateSetting**; 


** __u8  bNumEndpoints**;


//接口拥有的端点数量。这里并不包括端点0


** __u8  bInterfaceClass**;  

**//此Interface所属Class。例如：HID=0x03**


** __u8  bInterfaceSubClass**;

**//此值基于bInterfaceClass之上。表明在某个Interface class中的子class。例如：HID中有：USB_INTERFACE_SUBCLASS_BOOT**


 __u8  bInterfaceProtocol;

**//同上，HID中就有USB_INTERFACE_PROTOCOL_MOUSE**



 __u8  iInterface; //字符串描述符，放一些描述性厂商信息。



} __attribute__ ((packed));  **//__attribute__，意思就是告诉编译器，这个结构的元素都是1字节对齐的，不要再添加填充位了。**





**概念三：配置**

一个或多个**USB Interface**被捆绑为配置。一个USB设备可以有多个配置，并且可以在多个配置之间切换。

配置：struct usb_host_config

USB设备：usb_device.


综上所述：

1个**USB设备**有一个或多个**配置**

1个**配置**有一个或多个**Interface**

一个**Interface**有一个或多个**设置Interface**没有或有多个**EndpointＵＳＢ　ＵＲＢLinux Kernel中的USB代码通过 urb(USB 请求快)与所有的USB设备通信。**

urb被用来以异步方式从特定的USB设备的特定USB Endpoint上接收数据，或往特定的USB设备的特定USB Endpoint上发送数据。


urb是由USB driver创建的。并分配给特定USB设备的特定Endpoint。并由USB driver提交给USB Core。

**一：创建urb.**

urb不能在driver中静态的定义。因为这样会破坏USB Core对urb的计数机制。所以必须使用：
**usb_alloc_urb**函数来创建。

struct urb ***usb_alloc_urb**(int iso_packets, gfp_t mem_flags)

第一个参数是：等时数据包的数量。




