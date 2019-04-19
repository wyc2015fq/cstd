# 乾坤合一：Linux设备驱动之USB主机和设备驱动 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [李辉](http://www.jobbole.com/members/harrylhmj) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
这一章从主机侧角度看到的USB 主机控制器驱动和设备驱动从主机侧的角度而言，需要编写的USB 驱动程序包括主机控制器驱动和设备驱动两类，USB 主机控制器驱动程序控制插入其中的USB 设备，而USB 设备驱动程序控制该设备如何作为从设备与主机通信。
**1. Linux USB驱动层次**
1.1 主机侧与设备侧USB 驱动
USB 采用树形拓扑结构，每条总线上只有一个主机控制器，负责协调主机和设备间的通信，而设备不能主动向主机发送任何消息。
1.2 设备、配置、接口、端点
在USB 设备的逻辑组织中，包含设备、配置、接口和端点4 个层次，每个USB 设备都提供了不同级别的配置信息，可以包含一个或多个配置，不同的配置使设备表现出不同的功能组合，配置由多个接口组成，接口由多个端点组成，代表一个基本的功能，是USB 设备驱动程序控制的对象，如下图是USB 设备、配置、接口和端点之间的关系。
![](http://jbcdn2.b0.upaiyun.com/2016/05/896013b8d94a283e6e7803de45c79d94.png)
设备描述符：关于设备的通用信息，如供应商ID 、产品ID 和修订ID，支持的设备类、子类和适用的协议以及默认端点的最大包大小等。在Linux 内核中，USB 设备用 usb_device 结构体来描述，USB 设备描述符定义为usb_device_descriptor 结构体，其代码如下：


```
struct usb device descriptor
 
  {
 
      _ _u8 bLength; // 述符长度
 
      _ _u8 bDescriptorType; // 述符类型编号
    
      _ _le16 bcdUSB; //USB版本号
 
      _ _u8 bDeviceClass; //USB分配的设备类code
 
      _ _u8 bDeviceSubClass;// USB 分配的子类code
 
      _ _u8 bDeviceProtocol; //USB 分配的协议code
 
      _ _u8 bMaxPacketSize0; //endpoint0 最大包大小
  
      _ _le16 idVendor; //厂商编号
    
      _ _le16 idProduct; //产品编号
 
      _ _le16 bcdDevice; //设备出厂编号
     
      _ _u8  iManufacturer; // 述厂商字符串的索引
    
      _ _u8  iProduct; // 述产品字符串的索引
    
      _ _u8  iSerialNumber; // 述设备序列号字符串的索引
 
      _ _u8 bNumConfigurations; //可能的配置数量
 
  } _ _attribute_ _ ((packed));
```
配置描述符：此配置中的接口数、支持的挂起和恢复能力以及功率要求。USB配置在内核中使用usb_host_config 结构体描述，USB 配置描述符定义为结构体usb_config_descriptor，其代码如下：


```
struct usb config descriptor
 
 {
 
    _ _u8 bLength; // 述符长度
 
    _ _u8 bDescriptorType; // 述符类型编号
 
    _ _le16 wTotalLength; //配置所返回的所有数据的大小
 
    _ _u8 bNumInterfaces; // 配置所支持的接口数
 
    _ _u8 bConfigurationValue; //Set Configuration命令需要的参数值
 
    _ _u8  iConfiguration; // 述该配置的字符串的索引值
 
    _ _ u8 bmAttributes; //供电模式的选择
  
    _ _u8 bMaxPower; //设备从总线提取的最大电流
 
 } _ _attribute_ _ ((packed));
```
接口描述符：接口类、子类和适用的协议，接口备用配置的数目和端点数目。USB接口在内核中使用 usb_interface 结构体描述，USB 接口描述符定义为结构体 usb_interface_descriptor，其代码如下：


```
struct usb interface descriptor
 
      {
 
       _ _u8 bLength;            // 述符长度
 
       _ _u8 bDescriptorType; // 述符类型
 
       _ _ u8 bInterfaceNumber;     // 接口的编号
       _ _u8 bAlternateSetting; //备用的接口 述符编号
       _ _u8 bNumEndpoints;        //该接口使用的端点数，不包括端点0
          
        _ _ u8 bInterfaceClass;      //接口类型
          
        _ _ u8 bInterfaceSubClass; //接口子类型
         
        _ _ u8 bInterfaceProtocol; //接口所遵循的协议
  
        _ _ u8  iInterface; // 述该接口的字符串索引值
     } _ _attribute_ _ ((packed));
```
端点描述符：端点地址、方向和类型，支持的最大包大小，如果是中断类型的端点则还包括轮询频率。在 Linux 内核中，U SB 端点使用 usb_host_endpoint 结构体来描述，USB端点描述符定义为 usb_ endpoint_ descriptor 结构体，其代码如下：


```
struct usb endpoint descriptor
      {
       _ _u8 bLength; // 述符长度
       _ _u8 bDescriptorType; // 述符类型
       _ _u8  bEndpointAddress; //端点地址：0 ～3 位是端点号，第 7 位是方向 (0-OUT,1-IN)
        
       _ _ u8 bmAttributes; //端点属性：bit[0:1] 的值为00表示控制，为01表示同步，为02表示批量，为03表示中
          
       _ _le16 wMaxPacketSize; //// 本端点接收或发送的最大信息包的大小
         
       _ _u8 bInterval;      //轮询数据传送端点的时间间隔
                             //对于批量传送的端点以及控制传送的端点，此域忽略
                            //对于同步传送的端点，此域必须为1
                            //对于中 传送的端点，此域值的范围为1～255
         
       _ _u8 bRefresh;
       _ _u8 bSynchAddress;
     } _ _attribute_ _ ((packed));
```
字符串描述符：在其他描述符中会为某些字段提供字符串索引，它们被用来检索描述性字符串，可以以多种语言形式提供。字符串描述符是可选的，有的设备有，有的设备没有 ，字符 串描述符对应于usb_string_ descriptor 结构体，其代码如下：


```
struct usb string descriptor
      {
        _ _u8 bLength; // 述符长度
        _ _u8 bDescriptorType; // 述符类型
        _ _le16 wData [1];        /* 以UTF-16LE编码 */
      } _ _attribute_ _ ((packed));
```
**2 USB主机驱动**
USB 主机控制器有 3 种规格：OHCI (Open Host Controller Interface) 、UHCI (Universal Host Controller Interface) 和EHCI (Enhanced Host Controller Interface) 。
2.1 主机控制器驱动
在Linux 内核中，用usb_hcd 结构体描述USB 主机控制器驱动，它包含USB 主机控制器的 “家务”信息、硬件资源、状态描述和用于操作主机控制器的 hc_driver等，其代码如下：


```
struct usb hcd
 
      {
 
        /* 管理 “家务” */
 
        struct usb bus self;
  
        const char *product desc; /* 产品/厂商字符串 */
 
        char irq descr [24]; /* 驱动 + 总线 # */
  
         struct timer list rh timer; /* 根Hub 轮询 */
                          
         struct urb *status urb; /* 目前的状态urb */
       
        /* 硬件信息/状态 */
                           
        const struct hc driver *driver; /* 硬件特定的钩子函数 */
       
        /* 需要维护的标志 */
        unsigned long flags;
                         
        #define HCD FLAG HW ACCESSIBLE  0x00000001
                        
        #define HCD FLAG SAW IRQ            0x00000002
       
        unsigned rh_registered: 1; /* 根Hub 注册？ */
       
        /* 下一个标志的采用只是 “权益之计”，当所有HCDs 支持新的根Hub 轮询机制后将移除 */
                 
        unsigned uses new polling: 1;
                         
        unsigned poll rh: 1; /* 轮询根Hub 状态? */
                          
        unsigned poll pending: 1; /* 状态已经改变? */
       
        int irq; /* 被分配的irq */ <br>
        void _ _iomem *regs; /* 设备内存和I/O */
 
        u64 rsrc start; /* 内存和I/O资源开始位置 */
 
        u64 rsrc len; /* 内存和I/O资源长度 */
 
        unsigned power budget; /* mA, 0 = 无限制 */
 
        #define HCD BUFFER POOLS            4
 
        struct dma pool *pool[HCD BUFFER POOLS];
 
        int state;
 
        #define     ACTIVE                 0x01
 
        #define     SUSPEND                0x04
 
        #define     TRANSIENT              0x80
 
        #define HC STATE HALT              0
       
        #define HC STATE RUNNING            (   ACTIVE)
  
        #define HC STATE QUIESCING   (       SUSPEND|     TRANSIENT|    ACTIVE)
 
        #define HC STATE RESUMING           (   SUSPEND|    TRANSIENT)
 
        #define HC STATE SUSPENDED          (   SUSPEND)
 
        #define HC IS RUNNING (state) ((state) &          ACTIVE)
 
        #define HC IS SUSPENDED(state) ((state) &           SUSPEND)
 
        /* 主机控制器驱动的私有数据 */ <br>
　　　　 unsigned long hcd priv [0]    attribute    ((aligned (sizeof(unsigned long))));
 
      };
```
Linux中采用以下函数创建HCD：
```
```
struct usb hcd *usb create hcd (const struct hc driver *driver, 
                                         
struct device *dev, char *bus name);
```
```
以下函数用来增加和移除：
```
```
int usb add hcd (struct usb hcd *hcd, 
unsigned int irqnum, unsigned long irqflags); 
void usb remove hcd (struct usb hcd *
```
```
2.2 OHCI 主机控制器驱动
OHCI HCD 驱动属于HCD 驱动的实例，它定义了一个ohci_hcd 结构体，使用如下内联函数可实现usb_hcd 和ohci_hcd 的相互转换：
```
```
struct ohci hcd *hcd to ohci (struct usb hcd *hcd); 
  
struct usb hcd *ohci to hcd (const struct ohci hcd *ohci);
```
```
从usb_hcd 得到ohci_hcd 只是取得“私有”数据，而从ohci_hcd 得到usb_hcd 则是通过container_of()从结构体成员获得结构体指，使用如下函数可初始化OHCI 主机控制器：
```
```
int ohci init (struct ohci hcd *ohci);
```
```
如下函数分别用于开启、停止及复位OHCI 控制器：
```
```
int ohci run (struct ohci hcd *ohci); 
void ohci stop (struct usb hcd *hcd); 
void ohci usb reset (struct ohci hcd *ohci);
```
```
**3 USB设备驱动**
3.1 USB设备驱动整体结构
有以下设备类
- 音频设备类。
- 通信设备类。
- HID （人机接口）设备类。
- 显示设备类。
- 海量存储设备类。
- 电源设备类。
- 打印设备类。
- 集线器设备类。
Linux 内核为各类USB 设备分配了相应的设备号，内核中提供了USB 设备文件系统 （usbdevfs，Linux 2.6 改为usbfs，即USB 文件系统），它和/proc 类似，都是动态产生的。通过在/etc/fstab 文件中添加如下一行：
```
```
none /proc/bus/usb usbfs defaults
```
```
或者输入命令：
```
```
mount -t usbfs none /proc/bus/usb
```
```
可以实现USB 设备文件系统的挂载。
此外，在sysfs 文件系统中，同样包含了USB 相关信息的描述，但只限于接口级别。USB 设备和USB 接口在sysfs 中均表示为单独的USB 设备，其目录命名规则如下：
根集线器-集线器端口号 （-集线器端口号-…）:配置.接口。
3.2 USB请求块(URB)
USB 请求块 （USB request block，urb ）是USB 设备驱动中用来描述与USB 设备通信所用的基本载体和核心数据结构，非常类似于网络设备驱动中的sk_buff 结构体，是USB 主机与设备通信的 “电波”，urb 结构体，代码如下：


```
struct urb
 
　　{
 
        /* 私有的：只能由USB核心和主机控制器访问的字段 */
        struct kref kref; /*urb 引用计数 */
         
        spinlock t lock; /* urb锁 */
        void *hcpriv; /* 主机控制器私有数据 */
        int bandwidth; /* INT/ISO请求的带宽 */
              
        atomic t use count; /* 并发传输计数 */
        u8 reject; /* 传输将失败*/
 
       /* 公共的： 可以被驱动使用的字段 */
                
       struct list head urb list; /* 链表头*/
                
       struct usb device *dev; /* 关联的USB 设备 */
       unsigned int pipe; /* 管道信息 */
       int status; /* URB 的当前状态 */ unsigned int transfer flags; /* URB SHORT NOT OK | ...*/
 　　　　void *transfer buffer; /* 发送数据到设备或从设备接收数据的缓冲区 */ 
 　　　　dma addr t transfer dma; /*用来以DMA 方式向设备传输数据的缓冲区 */ 
 　　　　int transfer buffer length;/*transfer buffer 或 transfer dma 指向缓冲区的大小 */  
 　　　　int actual length; /* URB 结束后，发送或接收数据的实际长度 */ 
 　　　　unsigned char *setup packet; /* 指向控制URB 的设置数据包的指针*/ 
 　　　　dma addr t setup dma; /*控制URB 的设置数据包的DMA 缓冲区*/ 
 　　　　int start frame; /*等时传输中用于设置或返回初始帧*/ 
 　　　　int number of packets; /*等时传输中等时缓冲区数据 */ 
 　　　　int interval; /* URB被轮询到的时间间隔 （对中 和等时urb 有效） */ 
 　　　　int error count; /* 等时传输错误数量 */ 
 　　　　void *context; /* completion 函数上下文 */ 
 　　　　usb complete t complete; /* 当URB 被完全传输或发生错误时，被调用 */ 
 　　　　struct usb iso packet descriptor iso frame desc[0]; 
 　　　　/*单个URB 一次可定义多个等时传输时，描述各个等时传输 */
 　　};
```
USB 设备中的每个端点都处理一个urb 队列，在队列被清空之前，一个urb 的典型生命周期有以下几个过程：
- 被一个 USB设备驱动创建
- 初始化，被安排给一个特定USB 设备的特定端点
- 被USB 设备驱动提交给USB
- 提交由USB 核心指定的USB 主机控制器驱动。
- 被USB 主机控制器处理，进行一次到USB 设备的传送。
- 当urb 完成，USB 主机控制器驱动通知USB 设备驱动
3.3 简单的批量与控制URB
1）usb_bulk_msg()
usb_bulk_msg()函数创建一个USB 批量urb 并将它发送到特定设备，这个函数是同步的，它一直等待urb 完成后才返回。usb_bulk_msg()函数的原型为：
```
```
int usb bulk msg (struct usb device *usb dev, unsigned int pipe, 
void *data, int len, int *actual length,  int timeout); 
//usb_dev 参数为批量消息要发送的USB 设备的指 ，pipe 为批量消息要发送到的 USB 设备的端点，data 参数为指向要发送或接收的数据缓冲区的指 ，len 参数为data 参数//所指向的缓冲区的长度，actual_length 用于返回实际发送或接收的字节数，timeout 是发送超时，以ji ffies 为单位，0 意味着永远等待。 
  // 如果函数调用成功，返回0 ；否则，返回1 个负的错误值。
```
// 如果函数调用成功，返回0 ；否则，返回1 个负的错误值。
```
2 ）usb_control_msg()函数
usb_control_msg() 函数与 usb_bulk_msg() 函数类似，不过它提供驱动发送和结束USB 控制信息而非批量信息的能力，该函数的原型为：
```
```
int usb control msg (struct usb device *dev, unsigned int pipe,        u8 request, _ _u8 requesttype, _ _u16 value, _ _u16 index, void *data, _ _u16 size, int timeout); 
//dev 指向控制消息发往的USB 设备，pipe 是控制消息要发往的USB 设备的端点， request 是这个控制消息的USB 请求值，requesttype 是这个控制消息的USB 请求类型， //value 是这个控制消息的USB 消息值，index 是这个控制消息的USB 消息索引值，data 指向要发送或接收的数据缓冲区，size
```
```
3) 探测和断开函数
在USB 设备驱动usb_driver 结构体的探测函数中，应该完成如下工作：
- 探测设备的端点地址、缓冲区大小，初始化任何可能用于控制 USB 设备的数据结构。
- 把已初始化数据结构的指 保存到接口设备中
- 注册USB 设备
对探测函数的调用发生在USB 设备被安装且USB 核心认为该驱动程序与安装的USB 设备对应时 （usb_driver 的id_table 成员在此时发挥作用），而对断开函数的调用则发生在驱动因为种种原因不再控制该设备的时候。对这两个函数的调用都是在内核线程中进行的.
4) USB 骨架程序
Linux 内核源代码中的 driver/usb/usb-skeleton.c 文件为我们提供了一个最基础的USB 驱动程序，即USB 骨架程序，可被看做一个最简单的USB 设备驱动实例。尽管USB驱动驱动程序千差万别，但是骨架程序万变不离其宗。这里我也不多介绍啦~
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/d4181232cde3a79b8c6d11e39fd06ad2.png)![](http://www.jobbole.com/wp-content/uploads/2016/04/0e2c29b501d9020b8ef4615fe1493fc0.jpg)
