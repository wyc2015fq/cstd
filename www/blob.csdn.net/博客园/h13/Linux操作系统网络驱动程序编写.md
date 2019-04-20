# Linux操作系统网络驱动程序编写 - h13 - 博客园
Linux操作系统网络驱动程序编写 
一.Linux系统设备驱动程序概述 
1.1 Linux设备驱动程序分类 
1.2 编写驱动程序的一些基本概念 
二.Linux系统网络设备驱动程序 
2.1 网络驱动程序的结构 
2.2 网络驱动程序的基本方法 
2.3 网络驱动程序中用到的数据结构 
2.4 常用的系统支持 
三.编写Linux网络驱动程序中可能遇到的问题 
3.1 中断共享 
3.2 硬件发送忙时的处理 
3.3 流量控制(flow control) 
3.4 调试 
四.进一步的阅读 
五.杂项 
 Linux网络驱动程序编写
工作需要写了我们公司一块网卡的Linux驱动程序。经历一个从无到有的过程，深感技术交流的重要。Linux作为挑战微软垄断的强有力武器，日益受到大家的喜爱。真希望她能在中国迅速成长。把程序文档贴出来，希望和大家探讨Linux技术和应用，促进Linux在中国的普及。
　　一.Linux系统设备驱动程序概述
　　1.1 Linux设备驱动程序分类
　　Linux设备驱动程序在Linux的内核源代码中占有很大的比例，源代码的长度日益增加，主要是驱动程序的增加。在Linux内核的不断升级过程中，驱动程序的结构还是相对稳定。在2.0.xx到2.2.xx的变动里，驱动程序的编写做了一些改变，但是从2.0.xx的驱动到2.2.xx的移植只需做少量的工作。
　　Linux系统的设备分为字符设备(char device)，块设备(block device)和网络设备(network device)三种。字符设备是指存取时没有缓存的设备。块设备的读写都有缓存来支持，并且块设备必须能够随机存取(random access)，字符设备则没有这个要求。典型的字符设备包括鼠标，键盘，串行口等。块设备主要包括硬盘软盘设备，CD-ROM等。一个文件系统要安装进入操作系统必须在块设备上。
　　网络设备在Linux里做专门的处理。Linux的网络系统主要是基于BSD unix的socket机制。在系统和驱动程序之间定义有专门的数据结构(sk_buff)进行数据的传递。系统里支持对发送数据和接收数据的缓存，提供流量控制机制，提供对多协议的支持。
　　1.2 编写驱动程序的一些基本概念
　　无论是什么操作系统的驱动程序，都有一些通用的概念。操作系统提供给驱动程序的支持也大致相同。下面简单介绍一下网络设备驱动程序的一些基本要求。
　　1.2.1 发送和接收
　　这是一个网络设备最基本的功能。一块网卡所做的无非就是收发工作。所以驱动程序里要告诉系统你的发送函数在哪里，系统在有数据要发送时就会调用你的发送程序。还有驱动程序由于是直接操纵硬件的，所以网络硬件有数据收到最先能得到这个数据的也就是驱动程序，它负责把这些原始数据进行必要的处理然后送给系统。这里，操作系统必须要提供两个机制，一个是找到驱动程序的发送函数，一个是驱动程序把收到的数据送给系统。是驱动程序把收到的数据送给系统。
　　1.2.2 中断
　　中断在现代计算机结构中有重要的地位。操作系统必须提供驱动程序响应中断的能力。一般是把一个中断处理程序注册到系统中去。操作系统在硬件中断发生后调用驱动程序的处理程序。Linux支持中断的共享，即多个设备共享一个中断。
　　1.2.3 时钟
在实现驱动程序时，很多地方会用到时钟。如某些协议里的超时处理，没有中断机制的硬件的轮询等。操作系统应为驱动程序提供定时机制。一般是在预定的时间过了以后回调注册的时钟函数。在网络驱动程序中，如果硬件没有中断功能，定时器可以提供轮询(poll)方式对硬件进行存取。或者是实现某些协议时需要的超时重传等。
二.Linux系统网络设备驱动程序
　　2.1 网络驱动程序的结构
　　所有的Linux网络驱动程序遵循通用的接口。设计时采用的是面向对象的方法。一个设备就是一个对象(device 结构)，它内部有自己的数据和方法。每一个设备的方法被调用时的第一个参数都是这个设备对象本身。这样这个方法就可以存取自身的数据(类似面向对象程序设计时的this引用)。一个网络设备最基本的方法有初始化、发送和接收。
　　 -------------------　　　　　　---------------------
　　|deliver packets　　|　　　　　|receive packets queue|
　　|(dev_queue_xmit()) |　　　　　|them(netif_rx())　　 |
　　 -------------------　　　　　　---------------------
　　　　|　　　　 |　　　　　　　　　　　/　　　　 
　　　　　　　　 /　　　　　　　　　　　|　　　　　|
　　-------------------------------------------------------
　 | methods and variables(initialize,open,close,hard_xmit,|
　 | interrupt handler,config,resources,status...)　　　　 |
　　-------------------------------------------------------
　　　　 |　　　　|　　　　　　　　　　　/　　　　 
　　　　　　　　 /　　　　　　　　　　　|　　　　　|
　　　-----------------　　　　　　　----------------------
　　 |send to hardware |　　　　　　|receivce from hardware|
　　　-----------------　　　　　　　----------------------
　　　　 |　　　　|　　　　　　　　　　　/　　　　 
　　　　　　　　 /　　　　　　　　　　　|　　　　　|
　　 -----------------------------------------------------
　　|　　　　　　　　　hardware media　　　　　　　　　　 |
　　 -----------------------------------------------------
初始化程序完成硬件的初始化、device中变量的初始化和系统资源的申请、发送程序是在驱动程序的上层协议层有数据要发送时自动调用的。一般驱动程序中不对发送数据进行缓存，而是直接使用硬件的发送功能把数据发送出去。接收数据一般是通过硬件中断来通知的。在中断处理程序里，把硬件帧信息填入一个skbuff结构中，然后调用netif_rx()传递给上层处理。
　　2.2 网络驱动程序的基本方法
　　网络设备做为一个对象，提供一些方法供系统访问。正是这些有统一接口的方法，掩蔽了硬件的具体细节，让系统对各种网络设备的访问都采用统一的形式，做到硬件无关性。
　　下面解释最基本的方法。
　　2.2.1 初始化(initialize)
　　驱动程序必须有一个初始化方法。在把驱动程序载入系统的时候会调用这个初始化程序。它做以下几方面的工作。检测设备。在初始化程序里你可以根据硬件的特征检查硬件是否存在，然后决定是否启动这个驱动程序。配置和初始化硬件。在初始化程序里你可以完成对硬件资源的配置，比如即插即用的硬件就可以在这个时候进行配置(Linux内核对PnP功能没有很好的支持，可以在驱动程序里完成这个功能)。配置或协商好硬件占用的资源以后，就可以向系统申请这些资源。有些资源是能)。配置或协商好硬件占用的资源以后，就可以向系统申请这些资源。有些资源是可以和别的设备共享的，如中断。有些是不能共享的，如IO、DMA。接下来你要初始化device结构中的变量。最后，你可以让硬件正式开始工作。
　　2.2.2 打开(open)
　　open这个方法在网络设备驱动程序里是网络设备被激活的时候被调用(即设备状态由down-->up)。所以实际上很多在initialize中的工作可以放到这里来做。比如资源的申请，硬件的激活。如果dev->open返回非0(error)，则硬件的状态还是down。
open方法另一个作用是如果驱动程序做为一个模块被装入，则要防止模块卸载时设备处于打开状态。在open方法里要调用MOD_INC_USE_COUNT宏。
　　2.2.3 关闭(stop)
　　close方法做和open相反的工作。可以释放某些资源以减少系统负担。close是在设备状态由up转为down时被调用的。另外如果是做为模块装入的驱动程序，close里应该调用MOD_DEC_USE_COUNT，减少设备被引用的次数，以使驱动程序可以被卸载。另外close方法必须返回成功(0==success)。
　　2.2.4 发送(hard_start_xmit)
　　所有的网络设备驱动程序都必须有这个发送方法。在系统调用驱动程序的xmit时，发送的数据放在一个sk_buff结构中。一般的驱动程序把数据传给硬件发出去。也有一些特殊的设备比如loopback把数据组成一个接收数据再回送给系统，或者dummy设备直接丢弃数据。如果发送成功，hard_start_xmit方法里释放sk_buff，返回0(发送成功)。如果发送成功，hard_start_xmit方法里释放sk_buff，返回0(发送成功)。如果设备暂时无法处理，比如硬件忙，则返回1。这时如果dev->tbusy置为非0，则系统认为硬件忙，要等到dev->tbusy置0以后才会再次发送。tbusy的置0任务一般由中断完成。硬件在发送结束后产生中断，这时可以把tbusy置0，然后用mark_bh()调用通知系统可以再次发送。在发送不成功的情况下，也可以不置dev->tbusy为非0，这样系统会不断尝试重发。如果hard_start_xmit发送不成功，则不要释放sk_buff。
　　传送下来的sk_buff中的数据已经包含硬件需要的帧头。所以在发送方法里不需要再填充硬件帧头，数据可以直接提交给硬件发送。sk_buff是被锁住的(locked)，确保其他程序不会存取它。
　　2.2.5 接收(reception)
驱动程序并不存在一个接收方法。有数据收到应该是驱动程序来通知系统的。一般设备收到数据后都会产生一个中断，在中断处理程序中驱动程序申请一块sk_buff(skb)，从硬件读出数据放置到申请好的缓冲区里。接下来填充sk_buff中的一些信息。skb->dev = dev，判断收到帧的协议类型，填入skb->protocol(多协议的支持)。把指针skb->mac.raw指向硬件数据然后丢弃硬件帧头(skb_pull)。还要设置skb->pkt_type，标明第二层(链路层)数据类型。可以是以下类型： 
　　PACKET_BROADCAST : 链路层广播
　　PACKET_MULTICAST : 链路层组播
　　PACKET_SELF　　　: 发给自己的帧
　　PACKET_OTHERHOST : 发给别人的帧(监听模式时会有这种帧)
　　最后调用netif_rx()把数据传送给协议层。netif_rx()里数据放入处理队列然后返最后调用netif_rx()把数据传送给协议层。netif_rx()里数据放入处理队列然后返回，真正的处理是在中断返回以后，这样可以减少中断时间。调用netif_rx()以后，驱动程序就不能再存取数据缓冲区skb。
　　2.2.6 硬件帧头(hard_header)
　　硬件一般都会在上层数据发送之前加上自己的硬件帧头，比如以太网(Ethernet)就有14字节的帧头。这个帧头是加在上层ip、ipx等数据包的前面的。驱动程序提供一个hard_header方法，协议层(ip、ipx、arp等)在发送数据之前会调用这段程序。硬件帧头的长度必须填在dev->hard_header_len，这样协议层回在数据之前保留好硬件帧头的空间。这样hard_header程序只要调用skb_push然后正确填入硬件帧头就可以了。
　　在协议层调用hard_header时，传送的参数包括(2.0.xx)：数据的sk_buff，
device指针，protocol，目的地址(daddr)，源地址(saddr)，数据长度(len)。数据长度不要使用sk_buff中的参数，因为调用hard_header时数据可能还没完全组织好。saddr是NULL的话是使用缺省地址(default)。daddr是NULL表明协议层不知道硬件目的地址。如果hard_header完全填好了硬件帧头，则返回添加的字节数。如果硬件帧头中的信息还不完全(比如daddr为NULL，但是帧头中需要目的硬件地址。典型的情况是以太网需要地址解析(arp))，则返回负字节数。hard_header返回负数的情况下，协议层会做进一步的build header的工作。目前Linux系统里就是做arp(如果hard_header返回正，dev->arp=1，表明不需要做arp，返回负，dev->arp=0，做arp)。对hard_header的调用在每个协议层的处理程序里。如ip_output。
　　2.2.7 地址解析(xarp)
　　有些网络有硬件地址(比如Ethernet)，并且在发送硬件帧时需要知道目的硬件地址。这样就需要上层协议地址(ip、ipx)和硬件地址的对应。这个对应是通过地址解析完成的。需要做arp的的设备在发送之前会调用驱动程序的rebuild_header方法。调用的主要参数包括指向硬件帧头的指针，协议层地址。如果驱动程序能够解析硬件地址，就返回1，如果不能，返回0。
　　对rebuild_header的调用在net/core/dev.c的do_dev_queue_xmit()里。
　　2.2.8 参数设置和统计数据
　　在驱动程序里还提供一些方法供系统对设备的参数进行设置和读取信息。一般只有超级用户(root)权限才能对设备参数进行设置。设置方法有：
　　dev->set_mac_address()
　　当用户调用ioctl类型为SIOCSIFHWADDR时是要设置这个设备的mac地址。一般对mac地址的设置没有太大意义的。
　　dev->set_config()
　当用户调用ioctl时类型为SIOCSIFMAP时，系统会调用驱动程序的set_config方法。用户会传递一个ifmap结构包含需要的I/O、中断等参数。
　　dev->do_ioctl()
　　dev->do_ioctl()
　　如果用户调用ioctl时类型在SIOCDEVPRIVATE和SIOCDEVPRIVATE+15之间，系统会调用驱动程序的这个方法。一般是设置设备的专用数据。
　　读取信息也是通过ioctl调用进行。除次之外驱动程序还可以提供一个
　　dev->get_stats方法，返回一个enet_statistics结构，包含发送接收的统计信息。
　　ioctl的处理在net/core/dev.c的dev_ioctl()和dev_ifsioc()里。
　　2.3 网络驱动程序中用到的数据结构
　　最重要的是网络设备的数据结构。定义在include/linux/netdevice.h里。它的注释已经足够详尽。
```
struct device
{
　/*
　 * This is the first field of the "visible" part of this structure
　 * (i.e. as seen by users in the "Space.c" file).　It is the name
　 * the interface.
　 */
　char　　　　　　　　　　*name;
　/* I/O specific fields - FIXME: Merge these and struct ifmap into one */
　unsigned long　　　　　 rmem_end;　　　　　　 /* shmem "recv" end　　 */
　unsigned long　　　　　 rmem_end;　　　　　　 /* shmem "recv" end　　 */
　unsigned long　　　　　 rmem_start;　　　　　 /* shmem "recv" start　 */
　unsigned long　　　　　 mem_end;　　　　　　　/* shared mem end　　　 */
　unsigned long　　　　　 mem_start;　　　　　　/* shared mem start　　 */
　unsigned long　　　　　 base_addr;　　　　　　/* device I/O address　 */
　unsigned char　　　　　 irq;　　　　　　　　　/* device IRQ number　　*/
　/* Low-level status flags. */
　volatile unsigned char　start,　　　　　　　　/* start an operation　 */
　　　　　　　　　　　　　interrupt;　　　　　　/* interrupt arrived　　*/
　/* 在处理中断时interrupt设为1，处理完清0。 */
　unsigned long　　　　　 tbusy;　　　　　　　　/* transmitter busy must be long
 for
　struct device　　　　　 *next;
　/* The device initialization function. Called only once. */
　/* 指向驱动程序的初始化方法。 */
　int　　　　　　　　　　 (*init)(struct device *dev);
　/* Some hardware also needs these fields, but they are not part of the
　　 usual set specified in Space.c. */
　/* 一些硬件可以在一块板上支持多个接口，可能用到if_port。 */
　/* 一些硬件可以在一块板上支持多个接口，可能用到if_port。 */
　unsigned char　　　　　 if_port;　　　　　　　/* Selectable AUI, TP,..*/
　unsigned char　　　　　 dma;　　　　　　　　　/* DMA channel　　　　　*/
　struct enet_statistics* (*get_stats)(struct device *dev);
　/*
　 * This marks the end of the "visible" part of the structure. All
　 * fields hereafter are internal to the system, and may change at
　 * will (read: may be cleaned up at will).
　 */
　/* These may be needed for future network-power-down code. */
　/* trans_start记录最后一次成功发送的时间。可以用来确定硬件是否工作正常。*/
　unsigned long　　　　　 trans_start;　/* Time (in jiffies) of last Tx */
　unsigned long　　　　　 last_rx;　　　/* Time of last Rx　　　　　　　*/
　/* flags里面有很多内容，定义在include/linux/if.h里。*/
　unsigned short　　　　　flags;　　　　/* interface flags (a la BSD)　 */
　unsigned short　　　　　family;　　　 /* address family ID (AF_INET)　*/
　unsigned short　　　　　metric;　　　 /* routing metric (not used)　　*/
　unsigned short　　　　　mtu;　　　　　/* interface MTU value　　　　　*/
　/* type标明物理硬件的类型。主要说明硬件是否需要arp。定义在
　　 include/linux/if_arp.h里。 */
　unsigned short　　　　　type;　　　　 /* interface hardware type　　　*/
　/* 上层协议层根据hard_header_len在发送数据缓冲区前面预留硬件帧头空间。*/
　unsigned short　　　　　hard_header_len;　　　/* hardware hdr length　*/
　/* priv指向驱动程序自己定义的一些参数。*/
　void　　　　　　　　　　*priv;　　　　/* pointer to private data　　　*/
　/* Interface address info. */
　unsigned char　　　　　 broadcast[MAX_ADDR_LEN];　　　/* hw bcast add */
　unsigned char　　　　　 pad;　　　　　　　　　　　　　/* make dev_addr aligned
 to 8
bytes */
　unsigned char　　　　　 dev_addr[MAX_ADDR_LEN];　　　 /* hw address　 */
　unsigned char　　　　　 addr_len;　　 /* hardware address length　　　*/
　unsigned long　　　　　 pa_addr;　　　/* protocol address　　　　　　 */
　unsigned long　　　　　 pa_brdaddr;　 /* protocol broadcast addr　　　*/
　unsigned long　　　　　 pa_dstaddr;　 /* protocol P-P other side addr */
　unsigned long　　　　　 pa_mask;　　　/* protocol netmask　　　　　　 */
　struct dev_mc_list　　 *mc_list;　　　/* Multicast mac addresses　　　*/
　int　　　　　　　　　　mc_count;　　　/* Number of installed mcasts　 */
　struct ip_mc_list　　　*ip_mc_list;　 /* IP multicast filter chain　　*/
　__u32　　　　　　　　 tx_queue_len;　 /* Max frames per queue allowed */
　/* For load balancing driver pair support */
　unsigned long　　　　　　pkt_queue;　 /* Packets queued */
　struct device　　　　　 *slave;　　　 /* Slave device */
　struct net_alias_info　　　　 *alias_info;　　/* main dev alias info */
　struct net_alias　　　　　　　*my_alias;　　　/* alias devs */
　/* Pointer to the interface buffers. */
　struct sk_buff_head　　 buffs[DEV_NUMBUFFS];
　/* Pointers to interface service routines. */
　int　　　　　　　　　　 (*open)(struct device *dev);
　int　　　　　　　　　　 (*hard_start_xmit) (struct sk_buff *skb,
　　　　　　　　　　　　　　　　　　　　　　　struct device *dev);
　int　　　　　　　　　　 (*hard_header) (struct sk_buff *skb,
　　　　　　　　　　　　　　　　　　　　　struct device *dev,
　　　　　　　　　　　　　　　　　　　　　unsigned short type,
　　　　　　　　　　　　　　　　　　　　　void *daddr,
　　　　　　　　　　　　　　　　　　　　　void *saddr,
　　　　　　　　　　　　　　　　　　　　　unsigned len);
　int　　　　　　　　　　 (*rebuild_header)(void *eth, struct device *dev,
　　　　　　　　　　　　　　　　unsigned long raddr, struct sk_buff *skb);
#define HAVE_MULTICAST
　void　　　　　　　　　　(*set_multicast_list)(struct device *dev);
#define HAVE_SET_MAC_ADDR
　int　　　　　　　　　　 (*set_mac_address)(struct device *dev, void *addr);
#define HAVE_PRIVATE_IOCTL
　int　　　　　　　　　　 (*do_ioctl)(struct device *dev, struct ifreq *ifr, int
 cmd);
#define HAVE_SET_CONFIG
　int　　　　　　　　　　 (*set_config)(struct device *dev, struct ifmap *map);
#define HAVE_HEADER_CACHE
　void　　　　　　　　　　(*header_cache_bind)(struct hh_cache **hhp, struct dev
ice
*dev, unsigned short htype, __u32 daddr);
*dev, unsigned short htype, __u32 daddr);
　void　　　　　　　　　　(*header_cache_update)(struct hh_cache *hh, struct dev
ice
*dev, unsigned char *　haddr);
#define HAVE_CHANGE_MTU
　struct iw_statistics*　 (*get_wireless_stats)(struct device *dev);
};
```
2.4 常用的系统支持
　　2.4.1 内存申请和释放
　　include/linux/kernel.h里声明了kmalloc()和kfree()。用于在内核模式下申请和释放内存。
　　void *kmalloc(unsigned int len,int priority);
　　void kfree(void *__ptr);
　　与用户模式下的malloc()不同，kmalloc()申请空间有大小限制。长度是2的整次方。可以申请的最大长度也有限制。另外kmalloc()有priority参数，通常使用时可以为GFP_KERNEL，如果在中断里调用用GFP_ATOMIC参数，因为使用GFP_KERNEL则调用者可能进入sleep状态，在处理中断时是不允许的。
　　kfree()释放的内存必须是kmalloc()申请的。如果知道内存的大小，也可以用kfree_s()释放。
　　2.4.2 request_irq()、free_irq()
　　这是驱动程序申请中断和释放中断的调用。在include/linux/sched.h里声明。request_irq()调用的定义：
　　int request_irq(unsigned int irq,
　　　　　　　　　　　 void (*handler)(int irq, void *dev_id, struct pt_regs *regs),
　　　　　　　　　　　 unsigned long irqflags,
　　　　　　　　　　　 const char * devname,
　　　　　　　　　　　 void *dev_id);
　　irq是要申请的硬件中断号。在Intel平台，范围0--15。handler是向系统登记的中断处理函数。这是一个回调函数，中断发生时，系统调用这个函数，传入的参数包括硬件中断号，device id，寄存器值。dev_id就是下面的request_irq时传递给系统的参数dev_id。irqflags是中断处理的一些属性。比较重要的有SA_INTERRUPT，标明中断处理程序是快速处理程序(设置SA_INTERRUPT)还是慢速处理程序(不设置SA_INTERRUPT)。快速处理程序被调用时屏蔽所有中断。慢速处理程序不屏蔽。还有一个SA_SHIRQ属性，设置了以后运行多个设备共享中断。dev_id在中断共享时会用到。一般设置为这个设备的device结构本身或者NULL。中断处理程序可以用dev_id找到相应的控制这个中断的设备，或者用rq2dev_map找到中断对应的设备。　 
　　void free_irq(unsigned int irq,void *dev_id);
　　2.4.3 时钟
　　时钟的处理类似中断，也是登记一个时间处理函数，在预定的时间过后，系统时钟的处理类似中断，也是登记一个时间处理函数，在预定的时间过后，系统会调用这个函数。在include/linux/timer.h里声明。
```
struct timer_list {
　　　　struct timer_list *next;
　　　　struct timer_list *prev;
　　　　unsigned long expires;
　　　　unsigned long data;
　　　　void (*function)(unsigned long);
　　　　};
　　　　void add_timer(struct timer_list * timer);
　　　　int del_timer(struct timer_list * timer);
　　　　void init_timer(struct timer_list * timer);
```
　　使用时钟，先声明一个timer_list结构，调用init_timer对它进行初始化。time_list结构里expires是标明这个时钟的周期，单位采用jiffies的单位。jiffies是Linux一个全局变量，代表时间。它的单位随硬件平台的不同而不同。系统里定义了一个常数HZ，代表每秒种最小时间间隔的数目。这样jiffies的单位就是1/HZ。Intel平台jiffies的单位是1/100秒，这就是系统所能分辨的最小时间间隔了。所以expires/HZ就是以秒为单位的这个时钟的周期。
　　function就是时间到了以后的回调函数，它的参数就是timer_list中的data。data这个参数在初始化时钟的时候赋值，一般赋给它设备的device结构指针。
　　在预置时间到系统调用function，同时系统把这个time_list从定时队列里清除。所以如果需要一直使用定时函数，要在function里再次调用add_timer()把这个函数。
　I/O端口的存取使用：
　　inline unsigned int inb(unsigned short port);
　　inline unsigned int inb_p(unsigned short port);
　　inline void outb(char value, unsigned short port);
　　inline void outb_p(char value, unsigned short port);
　　在include/adm/io.h里定义。
　　inb_p()、outb_p()与inb()、outb_p()的不同在于前者在存取I/O时有等待
　　(pause)一适应慢速的I/O设备。
　　为了防止存取I/O时发生冲突，Linux提供对端口使用情况的控制。在使用端口之前，可以检查需要的I/O是否正在被使用，如果没有，则把端口标记为正在使用，使用完后再释放。系统提供以下几个函数做这些工作。
　　int check_region(unsigned int from, unsigned int extent);
　　void request_region(unsigned int from, unsigned int extent,const char *name)
　　;
　　void release_region(unsigned int from, unsigned int extent);
　　其中的参数from表示用到的I/O端口的起始地址，extent标明从from开始的端口数目。name为设备名称。
　　void release_region(unsigned int from, unsigned int extent);
　　其中的参数from表示用到的I/O端口的起始地址，extent标明从from开始的端口数目。name为设备名称。
　　2.4.5 中断打开关闭
　　系统提供给驱动程序开放和关闭响应中断的能力。是在include/asm/system.h中的两个定义。
#define cli() __asm__ __volatile__ ("cli"::)
　　#define sti() __asm__ __volatile__ ("sti"::)
　　2.4.6 打印信息
　　类似普通程序里的printf()，驱动程序要输出信息使用printk()。在include/linux/kernel.h里声明。
　　int printk(const char* fmt, ...);
　　其中fmt是格式化字符串。...是参数。都是和printf()格式一样的。
　　2.4.7 注册驱动程序
　　如果使用模块(module)方式加载驱动程序，需要在模块初始化时把设备注册
　　到系统设备表里去。不再使用时，把设备从系统中卸除。定义在drivers/net/net_init.h里的两个函数完成这个工作。
　　int register_netdev(struct device *dev);
　　void unregister_netdev(struct device *dev);
　　dev就是要注册进系统的设备结构指针。在register_netdev()时，dev就是要注册进系统的设备结构指针。在register_netdev()时，dev结构一般填写前面11项，即到init，后面的暂时可以不用初始化。最重要的是name指针和init方法。name指针空(NULL)或者内容为''或者name[0]为空格(space)，则系统把你的设备做为以太网设备处理。以太网设备有统一的命名格式，ethX。对以太网这么特别对待大概和Linux的历史有关。
　　init方法一定要提供，register_netdev()会调用这个方法让你对硬件检测和设置。
　　register_netdev()返回0表示成功，非0不成功。
　　2.4.8 sk_buff
　　Linux网络各层之间的数据传送都是通过sk_buff。sk_buff提供一套管理缓冲区的方法，是Linux系统网络高效运行的关键。每个sk_buff包括一些控制方法和一块数据缓冲区。控制方法按功能分为两种类型。一种是控制整个buffer链的方法，另一种是控制数据缓冲区的方法。sk_buff组织成双向链表的形式，根据网络应用的特点，对链表的操作主要是删除链表头的元素和添加到链表尾。sk_buff的控制方法都很短小以尽量减少系统负荷。(translated from article written by AlanCox)
　.alloc_skb() 申请一个sk_buff并对它初始化。返回就是申请到的sk_buff。
　　.dev_alloc_skb()类似alloc_skb，在申请好缓冲区后，保留16字节的帧头空间。主要用在Ethernet驱动程序。
　　.kfree_skb() 释放一个sk_buff。
　　.skb_clone() 复制一个sk_buff，但不复制数据部分。
　　.skb_copy()完全复制一个sk_buff。
　　.skb_dequeue() 从一个sk_buff链表里取出第一个元素。返回取出的sk_buff
　　.skb_dequeue() 从一个sk_buff链表里取出第一个元素。返回取出的sk_buff，如果链表空则返回NULL。这是常用的一个操作。
　　.skb_queue_head() 在一个sk_buff链表头放入一个元素。
　　.skb_queue_tail() 在一个sk_buff链表尾放入一个元素。这也是常用的一个操作。网络数据的处理主要是对一个先进先出队列的管理，skb_queue_tail()和skb_dequeue()完成这个工作。
　　.skb_insert() 在链表的某个元素前插入一个元素。
　　.skb_append() 在链表的某个元素后插入一个元素。一些协议(如TCP)对没按顺序到达的数据进行重组时用到skb_insert()和skb_append()。
　　.skb_reserve() 在一个申请好的sk_buff的缓冲区里保留一块空间。这个空间一般是用做下一层协议的头空间的。
　　.skb_put() 在一个申请好的sk_buff的缓冲区里为数据保留一块空间。在
　　alloc_skb以后，申请到的sk_buff的缓冲区都是处于空(free)状态，有一个tail指针指向free空间，实际上开始时tail就指向缓冲区头。skb_reserve()在free空间里申请协议头空间，skb_put()申请数据空间。见下面的图。
　　.skb_push() 把sk_buff缓冲区里数据空间往前移。即把Head room中的空间移一部分到Data area。
    .skb_pull() 把sk_buff缓冲区里Data area中的空间移一部分到Head room中。
　　--------------------------------------------------
　　　　　 |　　　　　　Tail room(free)　　　　　　　　　　　 |
　　　　　　--------------------------------------------------
　　　　　　--------------------------------------------------
　　　　　　　　　　　 After alloc_skb()
　　　　　　--------------------------------------------------
　　　　　 | Head room |　　　　Tail room(free)　　　　　　　 |
　　　　　　--------------------------------------------------
　　　　　　　　　　　 After skb_reserve()
　　　　　　--------------------------------------------------
　　　　　 | Head room |　　 Data area　　 | Tail room(free)　|
　　　　　　--------------------------------------------------
　　　　　　　　　　　 After skb_put()
　　　　　　--------------------------------------------------
　　　　　 |Head| skb_ |　 Data　　　　　　| Tail room(free)　|
　　　　　 |room| push |　　　　　　　　　 |　　　　　　　　　|
　　　　　 |　　|　　　 Data area　　　　　|　　　　　　　　　|
　　　　　　--------------------------------------------------
　　　　　　　　　　　 After skb_push()
　　　　　　--------------------------------------------------
　　　　　 |　 Head　　| skb_ |　Data area | Tail room(free)　|
　　　　　 |　　　　　 | pull |　　　　　　|　　　　　　　　　|
　　　　　 |　　　　　 | pull |　　　　　　|　　　　　　　　　|
　　　　　 |　　Head room　　 |　　　　　　|　　　　　　　　　|
　　　　　　--------------------------------------------------
　　　　　　　　　　　 After skb_pull()
三.编写Linux网络驱动程序中需要注意的问题
　　3.1 中断共享
　　Linux系统运行几个设备共享同一个中断。需要共享的话，在申请的时候指明共享方式。系统提供的request_irq()调用的定义：
　　　　int request_irq(unsigned int irq,
　　　　　　　　　　void (*handler)(int irq, void *dev_id, struct pt_regs *regs),
　　　　　　　　　　unsigned long irqflags,
　　　　　　　　　　const char * devname,
　　　　　　　　　　void *dev_id);
　　如果共享中断，irqflags设置SA_SHIRQ属性，这样就允许别的设备申请同一个中断。需要注意所有用到这个中断的设备在调用request_irq()都必须设置这个属性。系统在回调每个中断处理程序时，可以用dev_id这个参数找到相应的设备。系统在回调每个中断处理程序时，可以用dev_id这个参数找到相应的设备。一般dev_id就设为device结构本身。系统处理共享中断是用各自的dev_id参数依次调用每一个中断处理程序。
　　3.2 硬件发送忙时的处理
　　主CPU的处理能力一般比网络发送要快，所以经常会遇到系统有数据要发，但上一包数据网络设备还没发送完。因为在Linux里网络设备驱动程序一般不做数据缓存，不能发送的数据都是通知系统发送不成功，所以必须要有一个机制在硬件不忙时及时通知系统接着发送下面的数据。
　　一般对发送忙的处理在前面设备的发送方法(hard_start_xmit)里已经描述过，即如果发送忙，置tbusy为1。处理完发送数据后，在发送结束中断里清tbusy，同时用mark_bh()调用通知系统继续发送。
　　但在具体实现我的驱动程序时发现，这样的处理系统好象并不能及时地知道硬件已经空闲了，即在mark_bh()以后，系统要等一段时间才会接着发送。造成发送效率很低。2M线路只有10%不到的使用率。内核版本为2.0.35。
　　我最后的实现是不把tbusy置1，让系统始终认为硬件空闲，但是报告发送不成功。系统会一直尝试重发。这样处理就运行正常了。但是遍循内核源码中的网络驱动程序，似乎没有这样处理的。不知道症结在哪里。
　　3.3 流量控制(flow control)
　　网络数据的发送和接收都需要流量控制。这些控制是在系统里实现的，不需要驱动程序做工作。每个设备数据结构里都有一个参数dev->tx_queue_len，这个参数标明发送时最多缓存的数据包。在Linux系统里以太网设备(10/100Mbps)标明发送时最多缓存的数据包。在Linux系统里以太网设备(10/100Mbps)tx_queue_len一般设置为100，串行线路(异步串口)为10。实际上如果看源码可以知道，设置了dev->tx_queue_len并不是为缓存这些数据申请了空间。这个参数只是在收到协议层的数据包时判断发送队列里的数据是不是到了tx_queue_len的限度，以决定这一包数据加不加进发送队列。发送时另一个方面的流控是更高层协议的发送窗口(TCP协议里就有发送窗口)。达到了窗口大小，高层协议就不会再发送数据。
　　接收流控也分两个层次。netif_rx()缓存的数据包有限制。另外高层协议也会有一个最大的等待处理的数据量。
　　发送和接收流控处理在net/core/dev.c的do_dev_queue_xmit()和netif_rx()中。
　　3.4 调试
　　很多Linux的驱动程序都是编译进内核的，形成一个大的内核文件。但对调试来说，这是相当麻烦的。调试驱动程序可以用module方式加载。支持模块方式的驱动程序必须提供两个函数：int init_module(void)和void cleanup_module(void)。init_module()在加载此模块时调用，在这个函数里可以register_netdev()注册设备。init_module()返回0表示成功，返回负表示失败。cleanup_module()在驱动程序被卸载时调用，清除占用的资源，调用unregister_netdev()。
　　模块可以动态地加载、卸载。在2.0.xx版本里，还有kerneld自动加载模块，但是2.2.xx中已经取消了kerneld。手工加载使用insmod命令，卸载用rmmod命令，看内核中的模块用lsmod命令。
　　编译驱动程序用gcc，主要命令行参数-DKERNEL -DMODULE。并且作为模块加载的驱动程序，只编译成obj形式(加-c参数)。编译好的目标文放/lib/modules/2.x.xx/misc下，在启动文件里用insmod加载。
　　四.进一步的阅读
　　Linux程序设计资料可以从网上获得。这就是开放源代码的好处。并且没有什么“未公开的秘密”。我编写驱动程序时参阅的主要资料包括：
　　Linux内核源代码
　　《The Linux Kernel Hacker's Guide》by Michael K. Johnson
　　《Linux Kernel Module Programming Guide》by Ori Pomerantz
　　《Linux下的设备驱动程》by olly in BBS水木清华站
　　可以选择一个模板作为开始，内核源代码里有一个网络驱动程序的模板，drivers/net/skeleton.c。里面包含了驱动程序的基本内容。但这个模板是以以太网设备为对象的，以太网的处理在Linux系统里有特殊“待遇”，所以如果不是以太网设备，有些细节上要注意，主要在初始化程序里。
　　最后，多参照别人写的程序，听听其他开发者的经验之谈大概是最有效的帮助了。

