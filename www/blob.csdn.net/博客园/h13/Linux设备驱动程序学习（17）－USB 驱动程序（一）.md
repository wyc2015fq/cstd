# Linux设备驱动程序学习（17）－USB 驱动程序（一） - h13 - 博客园
**内核使用2.6.29.4**
      拓扑结构上, 一个 USB 子系统并不是以总线的方式来分布; 它是一棵由几个点对点连接构成的树。这些连接是连接设备和集线器的4线电缆(地, 电源, 和 2 个差分信号线), 如同以太网的双绞线。USB主控制器负责询问每个USB设备是否有数据需要发送。
由于这个拓扑结构，一个 USB 设备在没有主控制器要求的情况下不能发送数据. 也就是说：USB是单主方式的实现，主机轮询各外设。但是设备也可以要求一个固定的数据传输带宽，以保证可靠的音视频I/O。USB只作为数据传输通道，对他所收发的数据格式没有特殊的内容和结构上的要求，也就是类似于透传。
Linux内核支持两种主要类型的USB驱动程序：Host系统上的驱动程序（USB device driver）和device上的驱动程序（USB gadget driver）（设备端驱动）。
USB驱动程序存在于不同的内核子系统和USB硬件控制器之中。USB核心为USB驱动程序提供了一个用于访问和控制USB硬件的接口，它隐藏了USB控制器的硬件细节。从这里我们要知道：《LDD3》所谓的USB驱动是针对USB核心提供的接口而写的，并不是真正去操纵USB硬件控制器中的寄存器。这样你必须保证你的板子上CPU的USB硬件控制器的驱动是可用的。否则您就得先搞定CPU的USB硬件控制器的驱动才行。
以下是Linux内核中USB驱动的软件构架： 
    如左下图所示，从主机侧的观念去看，在Linux驱动中，USB驱动处于最底层的是USB主机控制器硬件，在其之上运行的是USB主机控制器驱动，主机控制器之上为USB核心层，再上层为USB设备驱动层（插入主机上的U盘、鼠标、USB转串口等设备驱动）。因此，在主机侧的层次结构中，要实现的USB驱动包括两类：USB主机控制器驱动和USB设备驱动，前者控制插入其中的USB设备，后者控制USB设备如何与主机通信。Linux内核USB核心负责USB驱动管理和协议处理的主要工作。主机控制器驱动和设备驱动之间的USB核心非常重要，其功能包括：通过定义一些数据结构、宏和功能函数，向上为设备驱动提供编程接口，向下为USB主机控制器驱动提供编程接口；通过全局变量维护整个系统的USB设备信息；完成设备热插拔控制、总线数据传输控制等。
    如右下图所示，Linux内核中USB设备侧驱动程序分为3个层次：UDC驱动程序、Gadget API和Gadget驱动程序。UDC驱动程序直接访问硬件，控制USB设备和主机间的底层通信，向上层提供与硬件相关操作的回调函数。当前Gadget API是UDC驱动程序回调函数的简单包装。Gadget驱动程序具体控制USB设备功能的实现，使设备表现出“网络连接”、“打印机”或“USB Mass Storage”等特性，它使用Gadget API控制UDC实现上述功能。Gadget API把下层的UDC驱动程序和上层的Gadget驱动程序隔离开，使得在Linux系统中编写USB设备侧驱动程序时能够把功能的实现和底层通信分离。
以上的图和文字载自 华清远见的《[Linux设备驱动开发详解](http://www.cnblogs.cc2/link.php?url=http://book.csdn.net%2Fbookfiles%2F642)》
《LDD3》中的USB驱动的介绍分以下几块：
－－－－－－－－－－－基础知识部分－－－－－－－－－－－－－－
（1）USB设备基础
        端点
        接口
        配置
（2）USB和sysfs
（3）USB urb
        struct urb
        创建和销毁urb
           中断urb
           批量urb
           控制urb
           等时urb
        提交urb
        结束urb：结束回调处理例程
        取消urb
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
－－－－－－－－驱动编写部分（一）－－－－－－－－－－－－－－
（4）编写USB驱动程序
        驱动支持什么设备
        注册USB驱动程序
        探测和断开的细节
        提交和控制urb
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
－－－－－－－－驱动编写部分（二）－－－－－－－－－－－－－－
（5）不使用urb的USB传输 
        usb_bulk_msg
        usb_control_msg
（6）其他USB数据函数
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
USB设备其实很复杂，但是Linux内核提供了一个称为USB core的子系统来处理了大部分的复杂工作，所以这里所描述的是驱动程序和USB core之间的接口。
在USB设备组织结构中，从上到下分为设备（device）、配置（config）、接口（interface）和端点（endpoint）四个层次。
对于这四个层次的简单描述如下：
    设备通常具有一个或多个的配置
    配置经常具有一个或多个的接口
    接口通常具有一个或多个的设置
    接口没有或具有一个以上的端点
**设备**　　很明显，地代表了一个插入的USB设备，在内核使用数据结构 struct usb_device来描述整个USB设备。（include/linux/usb.h）
```
struct usb_device {
　　 int devnum; //设备号，是在USB总线的地址
　　 char devpath [16]; //用于消息的设备ID字符串
　　 enum usb_device_state state; //设备状态：已配置、未连接等等
　　 enum usb_device_speed speed; //设备速度：高速、全速、低速或错误
　　
　　 struct usb_tt *tt; //处理传输者信息；用于低速、全速设备和高速HUB
　　 int ttport; //位于tt HUB的设备口
　　
　　 unsigned int toggle[2]; //每个端点的占一位，表明端点的方向([0] = IN, [1] = OUT)　　
　　 struct usb_device *parent; //上一级HUB指针
　　 struct usb_bus *bus; //总线指针
　　 struct usb_host_endpoint ep0; //端点0数据
　　 struct device dev; //一般的设备接口数据结构
　
　　 struct usb_device_descriptor descriptor; //USB设备描述符
　　 struct usb_host_config *config; //设备的所有配置
　　 struct usb_host_config *actconfig; //被激活的设备配置
　　 struct usb_host_endpoint *ep_in[16]; //输入端点数组
　　 struct usb_host_endpoint *ep_out[16]; //输出端点数组
　　
　　 char **rawdescriptors; //每个配置的raw描述符
　　
　　 unsigned short bus_mA; //可使用的总线电流
```
```
u8 portnum;//父端口号
　　 u8 level; //USB HUB的层数
　　
　　 unsigned can_submit:1; //URB可被提交标志
　　 unsigned discon_suspended:1; //暂停时断开标志
　　 unsigned persist_enabled:1; //USB_PERSIST使能标志
　　 unsigned have_langid:1; //string_langid存在标志
　　 unsigned authorized:1; 
　　 unsigned authenticated:1;
　　 unsigned wusb:1; //无线USB标志
　　 int string_langid; //字符串语言ID
　　
　　 /* static strings from the device */ //设备的静态字符串
　　 char *product; //产品名
　　 char *manufacturer; //厂商名
　　 char *serial; //产品串号
　　
　　 struct list_head filelist; //此设备打开的usbfs文件
　　#ifdef CONFIG_USB_DEVICE_CLASS
　　 struct device *usb_classdev; //用户空间访问的为usbfs设备创建的USB类设备
　　#endif
　　#ifdef CONFIG_USB_DEVICEFS
　　 struct dentry *usbfs_dentry; //设备的usbfs入口
　　#endif
　　
　　 int maxchild; //（若为HUB）接口数
　　 struct usb_device *children[USB_MAXCHILDREN];//连接在这个HUB上的子设备
　　 int pm_usage_cnt; //自动挂起的使用计数
　　 u32 quirks; 
　　 atomic_t urbnum; //这个设备所提交的URB计数
　　
　　 unsigned long active_duration; //激活后使用计时
　　#ifdef CONFIG_PM //电源管理相关
　　 struct delayed_work autosuspend; //自动挂起的延时
　　 struct work_struct autoresume; //（中断的）自动唤醒需求
　　 struct mutex pm_mutex; //PM的互斥锁　
　 
　　 unsigned long last_busy; //最后使用的时间
　　 int autosuspend_delay; 
　　 unsigned long connect_time; //第一次连接的时间
　　
　　 unsigned auto_pm:1; //自动挂起/唤醒
　　 unsigned do_remote_wakeup:1; //远程唤醒
　　 unsigned reset_resume:1; //使用复位替代唤醒
　　 unsigned autosuspend_disabled:1; //挂起关闭
　　 unsigned autoresume_disabled:1; //唤醒关闭
　　 unsigned skip_sys_resume:1; //跳过下个系统唤醒
　　#endif
　　 struct wusb_dev *wusb_dev; //（如果为无线USB）连接到WUSB特定的数据结构
　　};
```
**配置**
    一个USB设备可以有多个配置，并可在它们之间转换以改变设备的状态。比如一个设备可以通过下载固件（firmware）的方式改变设备的使用状态（我感觉类似FPGA或CPLD），那么USB设备就要切换配置，来完成这个工作。一个时刻只能有一个配置可以被激活。Linux使用结构 struct usb_host_config 来描述USB配置。我们编写的USB设备驱动通常不需要读写这些结构的任何值。可在内核源码的文件include/linux/usb.h中找到对它们的描述。
```
struct usb_host_config {
    struct usb_config_descriptor desc; //配置描述符
    char *string; /* 配置的字符串指针（如果存在） */
    struct usb_interface_assoc_descriptor *intf_assoc[USB_MAXIADS]; //配置的接口联合描述符链表
    struct usb_interface *interface[USB_MAXINTERFACES]; //接口描述符链表
    struct usb_interface_cache *intf_cache[USB_MAXINTERFACES]; 
    unsigned char *extra; /* 额外的描述符 */
    int extralen;
};
```
**接口**
USB端点被绑为接口，USB接口只处理一种USB逻辑连接。一个USB接口代表一个基本功能，每个USB驱动控制一个接口。所以一个物理上的硬件设备可能需要一个以上的驱动程序。这可以在“晕到死 差屁”系统中看出，有时插入一个USB设备后，系统会识别出多个设备，并安装相应多个的驱动。
USB 接口可以有其他的设置,它是对接口参数的不同选择. 接口的初始化的状态是第一个设置,编号为0。 其他的设置可以以不同方式控制独立的端点。
USB接口在内核中使用 struct usb_interface 来描述。USB 核心将其传递给USB驱动，并由USB驱动负责后续的控制。
```
struct usb_interface {
    struct usb_host_interface *altsetting; /* 包含所有可用于该接口的可选设置的接口结构数组。每个 struct usb_host_interface 包含一套端点配置（即struct usb_host_endpoint结构所定义的端点配置。这些接口结构没有特别的顺序。*/
    struct usb_host_interface *cur_altsetting; /* 指向altsetting内部的指针，表示当前激活的接口配置*/
    unsigned num_altsetting; /* 可选设置的数量*/
    /* If there is an interface association descriptor then it will list the associated interfaces */
    struct usb_interface_assoc_descriptor *intf_assoc;
    int minor; /* 如果绑定到这个接口的 USB 驱动使用 USB 主设备号, 这个变量包含由 USB 核心分配给接口的次设备号. 这只在一个成功的调用 usb_register_dev后才有效。*/
    /*以下的数据在我们写的驱动中基本不用考虑，系统会自动设置*/
    enum usb_interface_condition condition; /* state of binding */
    unsigned is_active:1; /* the interface is not suspended */
    unsigned sysfs_files_created:1; /* the sysfs attributes exist */
    unsigned ep_devs_created:1; /* endpoint "devices" exist */
    unsigned unregistering:1; /* unregistration is in progress */
    unsigned needs_remote_wakeup:1; /* driver requires remote wakeup */
    unsigned needs_altsetting0:1; /* switch to altsetting 0 is pending */
    unsigned needs_binding:1; /* needs delayed unbind/rebind */
    unsigned reset_running:1;
    struct device dev; /* 接口特定的设备信息 */
    struct device *usb_dev;
    int pm_usage_cnt; /* usage counter for autosuspend */
    struct work_struct reset_ws; /* for resets in atomic context */
};
struct usb_host_interface {
    struct usb_interface_descriptor desc; //接口描述符
    struct usb_host_endpoint *endpoint; /* 这个接口的所有端点结构体的联合数组*/
    char *string; /* 接口描述字符串 */
    unsigned char *extra; /* 额外的描述符 */
    int extralen;
};
```
**端点**
USB 通讯的最基本形式是通过一个称为端点的东西。一个USB端点只能向一个方向传输数据（从主机到设备(称为输出端点)或者从设备到主机(称为输入端点)）。端点可被看作一个单向的管道。
一个 USB 端点有 4 种不同类型, 分别具有不同的数据传送方式：
**控制CONTROL **控制端点被用来控制对 USB 设备的不同部分访问. 通常用作配置设备、获取设备信息、发送命令到设备或获取设备状态报告。这些端点通常较小。每个 USB 设备都有一个控制端点称为"端点 0", 被 USB 核心用来在插入时配置设备。USB协议保证总有足够的带宽留给控制端点传送数据到设备.
**中断INTERRUPT **每当 USB 主机向设备请求数据时，中断端点以固定的速率传送小量的数据。此为USB 键盘和鼠标的主要的数据传送方法。它还用以传送数据到 USB 设备来控制设备。通常不用来传送大量数据。USB协议保证总有足够的带宽留给中断端点传送数据到设备.
**批量BULK**
批量端点用以传送大量数据。这些端点常比中断端点大得多. 它们普遍用于不能有任何数据丢失的数据。USB 协议不保证传输在特定时间范围内完成。如果总线上没有足够的空间来发送整个BULK包，它被分为多个包进行传输。这些端点普遍用于打印机、USB Mass Storage和USB网络设备上。
**等时ISOCHRONOUS **等时端点也批量传送大量数据, 但是这个数据不被保证能送达。这些端点用在可以处理数据丢失的设备中，并且更多依赖于保持持续的数据流。如音频和视频设备等等。
控制和批量端点用于异步数据传送，而中断和同步端点是周期性的。这意味着这些端点被设置来在固定的时间连续传送数据，USB 核心为它们保留了相应的带宽。
端点在内核中使用结构 struct usb_host_endpoint 来描述，它所包含的真实端点信息在另一个结构中：struct usb_endpoint_descriptor（端点描述符，包含所有的USB特定数据）。
```
struct usb_host_endpoint {
    struct usb_endpoint_descriptor desc; //端点描述符
    struct list_head urb_list; //此端点的URB对列，由USB核心维护
    void *hcpriv;
    struct ep_device *ep_dev; /* For sysfs info */
    unsigned char *extra; /* Extra descriptors */
    int extralen;
    int enabled;
};
/*-------------------------------------------------------------------------*/
/* USB_DT_ENDPOINT: Endpoint descriptor */
struct usb_endpoint_descriptor {
    __u8 bLength;
    __u8 bDescriptorType;
    __u8 bEndpointAddress; /*这个特定端点的 USB 地址，这个8位数据包含端点的方向，结合位掩码 USB_DIR_OUT 和 USB_DIR_IN 使用, 确定这个端点的数据方向。*/
    __u8 bmAttributes; //这是端点的类型，位掩码如下
    __le16 wMaxPacketSize; /*端点可以一次处理的最大字节数。驱动可以发送比这个值大的数据量到端点, 但是当真正传送到设备时，数据会被分为 wMaxPakcetSize 大小的块。对于高速设备, 通过使用高位部分几个额外位，可用来支持端点的高带宽模式。*/
    __u8 bInterval; //如果端点是中断类型，该值是端点的间隔设置，即端点的中断请求间的间隔时间，以毫秒为单位
    /* NOTE: these two are _only_ in audio endpoints. */
    /* use USB_DT_ENDPOINT*_SIZE in bLength, not sizeof. */
    __u8 bRefresh;
    __u8 bSynchAddress;
} __attribute__ ((packed));
#define USB_DT_ENDPOINT_SIZE 7
#define USB_DT_ENDPOINT_AUDIO_SIZE 9 /* Audio extension */
/*
 * Endpoints
 */
#define USB_ENDPOINT_NUMBER_MASK 0x0f /* in bEndpointAddress 端点的 USB 地址掩码 */
#define USB_ENDPOINT_DIR_MASK 0x80 /* in bEndpointAddress 数据方向掩码 */
```
```
#define USB_DIR_OUT 0 /* to device */
#define USB_DIR_IN 0x80 /* to host */
#define USB_ENDPOINT_XFERTYPE_MASK 0x03 /* bmAttributes 的位掩码*/
#define USB_ENDPOINT_XFER_CONTROL 0
#define USB_ENDPOINT_XFER_ISOC 1
#define USB_ENDPOINT_XFER_BULK 2
#define USB_ENDPOINT_XFER_INT 3
#define USB_ENDPOINT_MAX_ADJUSTABLE 0x80
/*-------------------------------------------------------------------------*/
```
**USB 和 sysfs**
由于单个 USB 物理设备的复杂性，设备在 sysfs 中的表示也非常复杂。**物理 USB 设备(通过 struct usb_device 表示)和单个 USB 接口(由 struct usb_interface 表示)都作为单个设备出现在 sysfs 中，这是因为这两个结构都包含一个 struct device结构。**以下内容是我的USB鼠标在 sysfs 中的目录树：    
```
/sys/devices/pci0000:00/0000:00:1a.0/usb3/3-1 （表示 usb_device 结构）
.
|-- 3-1:1.0 （鼠标所对应的usb_interface）
| |-- 0003:046D:C018.0003
| | |-- driver -> ../../../../../../../bus/hid/drivers/generic-usb
| | |-- power
| | | `-- wakeup
| | |-- subsystem -> ../../../../../../../bus/hid
| | `-- uevent
| |-- bAlternateSetting
| |-- bInterfaceClass
| |-- bInterfaceNumber
| |-- bInterfaceProtocol
| |-- bInterfaceSubClass
| |-- bNumEndpoints
| |-- driver -> ../../../../../../bus/usb/drivers/usbhid
| |-- ep_81 -> usb_endpoint/usbdev3.4_ep81
| |-- input
| | `-- input6
| | |-- capabilities
| | | |-- abs
| | | |-- ev
| | | |-- ff
| | | |-- key
| | | |-- led
| | | |-- msc
| | | |-- rel
| | | |-- snd
| | | `-- sw
| | |-- device -> ../../../3-1:1.0
| | |-- event3
| | | |-- dev
| | | |-- device -> ../../input6
| | | |-- power
| | | | `-- wakeup
| | | |-- subsystem -> ../../../../../../../../../class/input
| | | `-- uevent
| | |-- id
| | | |-- bustype
| | | |-- product
| | | |-- vendor
| | | `-- version
| | |-- modalias
| | |-- mouse1
| | | |-- dev
| | | |-- device -> ../../input6
| | | |-- power
| | | | `-- wakeup
| | | |-- subsystem -> ../../../../../../../../../class/input
| | | `-- uevent
| | |-- name
| | |-- phys
| | |-- power
| | | `-- wakeup
| | |-- subsystem -> ../../../../../../../../class/input
| | |-- uevent
| | `-- uniq
| |-- modalias
| |-- power
| | `-- wakeup
| |-- subsystem -> ../../../../../../bus/usb
| |-- supports_autosuspend
| |-- uevent
| `-- usb_endpoint
| `-- usbdev3.4_ep81
| |-- bEndpointAddress
| |-- bInterval
| |-- bLength
| |-- bmAttributes
| |-- dev
| |-- device -> ../../../3-1:1.0
| |-- direction
| |-- interval
| |-- power
| | `-- wakeup
| |-- subsystem -> ../../../../../../../../class/usb_endpoint
| |-- type
| |-- uevent
| `-- wMaxPacketSize
|-- authorized
|-- bConfigurationValue
|-- bDeviceClass
|-- bDeviceProtocol
|-- bDeviceSubClass
|-- bMaxPacketSize0
|-- bMaxPower
|-- bNumConfigurations
|-- bNumInterfaces
|-- bcdDevice
|-- bmAttributes
|-- busnum
|-- configuration
|-- descriptors
|-- dev
|-- devnum
|-- driver -> ../../../../../bus/usb/drivers/usb
|-- ep_00 -> usb_endpoint/usbdev3.4_ep00
|-- idProduct
|-- idVendor
|-- manufacturer
|-- maxchild
|-- power
| |-- active_duration
| |-- autosuspend
| |-- connected_duration
| |-- level
| |-- persist
| `-- wakeup
|-- product
|-- quirks
|-- speed
|-- subsystem -> ../../../../../bus/usb
|-- uevent
|-- urbnum
|-- usb_endpoint
| `-- usbdev3.4_ep00
| |-- bEndpointAddress
| |-- bInterval
| |-- bLength
| |-- bmAttributes
| |-- dev
| |-- device -> ../../../3-1
| |-- direction
| |-- interval
| |-- power
| | `-- wakeup
| |-- subsystem -> ../../../../../../../class/usb_endpoint
| |-- type
| |-- uevent
| `-- wMaxPacketSize
`-- version
38 directories, 91 files
```
**USB sysfs 设备命名方法是: root_hub-hub_port:config.interface **    随着USB集线器层次的增加, 集线器端口号被添加到字符串中紧随着链中之前的集线器端口号。对一个 2 层的树, 设备为:** root_hub-hub_port-hub_port:config.interface** ，以此类推。 
 本文出自：[http://blog.chinaunix.net/space.php?uid=20543672&do=blog&cuid=1995182](http://blog.chinaunix.net/space.php?uid=20543672&do=blog&cuid=1995182)

