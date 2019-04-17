# Linux网络体系结构 - 一世豁然的专栏 - CSDN博客





2014年12月23日 10:52:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1149








本博客转载于：[http://blog.csdn.net/fover717/article/details/4301637](http://blog.csdn.net/fover717/article/details/4301637)





**一 内核结构**

    按功能，Linux内核可以划分为5个不同的部分，每一部分处理一项明确的功能，同时又向其他内核组件提供这项功能。这种结构也反映到内核的源代码上——这5部分都形成了自己的子树。



![](https://p-blog.csdn.net/images/p_blog_csdn_net/fover717/EntryImages/20090626/%E5%86%85%E6%A0%B8%E7%BB%93%E6%9E%84.jpg)

1进程管理

2内存管理

3文件系统

4设备驱动程序

5网络



**二 网络体系结构**

    先来看一下Linux网络体系结构

**![](https://p-blog.csdn.net/images/p_blog_csdn_net/fover717/EntryImages/20090626/Linux%E9%AB%98%E7%BA%A7%E7%BD%91%E7%BB%9C%E6%A0%88%E6%9E%B6%E6%9E%84.gif)**





从上图可以看出Linux网络体系结构由以下五个部分组成 1）系统调用接口 2）协议无关几口 3）网络协议 4）设备无关接口 5 设备驱动程序。下面分别简述五个部分：



1）系统调用接口 



     系统调用接口是用户空间的应用程序正常访问内核的唯一合法途径（终端和陷入也可访问内核）。如：

asmlingkage long sys_getpid(void)

{

    return current->pid;

}

系统调用一般由sys开头 ，前面的修饰符是asmlingkage，表示函数由堆栈获得参数。



2）协议无关接口



    协议无关接口是由socket来实现的。它提供了一组通用函数来支持各种不同协议。

    通过网络栈进行的通信都需要对 socket 进行操作。Linux 中的 socket 结构是 `struct sock`，这个结构是在 linux/include/net/sock.h 中定义的。这个巨大的结构中包含了特定 socket 所需要的所有状态信息，其中包括 socket 所使用的特定协议和在 socket 上可以执行的一些操作。

    网络子系统可以通过一个定义了自己功能的特殊结构来了解可用协议。每个协议都维护了一个名为 `proto` 的结构（可以在 linux/include/net/sock.h 中找到）。这个结构定义了可以在从 socket 层到传输层中执行特定的 socket 操作



3）网络协议



    Linux支持多种网络协议，可以在<linux/socket.h>中查到所支持的网络协议：

#define AF_UNIX        1    /* Unix domain sockets         */

#define AF_LOCAL    1    /* POSIX name for AF_UNIX    */

#define AF_INET        2    /* Internet IP Protocol     */

#define AF_AX25        3    /* Amateur Radio AX.25         */

#define AF_IPX        4    /* Novell IPX         

... ...

其中每一个所支持的协议对应net_family[]数组中的一项，net_family[]是结构体指针数组，其中的每一项都是一个结构体指针，指向一个net_proto_family 结构

struct net_proto_family {

          int family;

          int (*create) (struct socket * sock, int protocol);

          short authentication;

          short encryption;

          short encrypt_net;

          struct module *owner;

};这个结构体中注册了关于协议的信息。



4）设备无关接口



   设备无关接口是由net_device实现的。任何设备和上层通信都是通过net_device设备无关接口。

   它将协议与具有很多各种不同功能的硬件设备连接在一起。这一层提供了一组通用函数供底层网络设备驱动程序使用，让它们可以对高层协议栈进行操作。 

首先，设备驱动程序可能会通过调用 `register_netdevice` 或 `unregister_netdevice` 在内核中进行注册或注销。调用者首先填写
`net_device` 结构，然后传递这个结构进行注册。内核调用它的 `init` 函数（如果定义了这种函数），然后执行一组健全性检查，并创建一个
`sysfs` 条目，然后将新设备添加到设备列表中（内核中的活动设备链表）。在 linux/include/linux/netdevice.h 中可以找到这个
`net_device` 结构。这些函数都是在 linux/net/core/dev.c 中实现的。

    要从协议层向设备中发送 `sk_buff`，就需要使用 `dev_queue_xmit` 函数。这个函数可以对
`sk_buff` 进行排队，从而由底层设备驱动程序进行最终传输（使用 `sk_buff` 中引用的 `net_device` 或
`sk_buff->dev` 所定义的网络设备）。`dev` 结构中包含了一个名为 `hard_start_xmit` 的方法，其中保存有发起
`sk_buff` 传输所使用的驱动程序函数。

    报文的接收通常是使用 `netif_rx` 执行的。当底层设备驱动程序接收一个报文（包含在所分配的 `sk_buff` 中）时，就会通过调用
`netif_rx` 将 `sk_buff` 上传至网络层。然后，这个函数通过 `netif_rx_schedule` 将
`sk_buff` 在上层协议队列中进行排队，供以后进行处理。可以在 linux/net/core/dev.c 中找到 `dev_queue_xmit` 和
`netif_rx` 函数。



5）设备驱动程序



    网络栈底部是负责管理物理网络设备的设备驱动程序。例如，包串口使用的 SLIP 驱动程序以及以太网设备使用的以太网驱动程序都是这一层的设备。

在进行初始化时，设备驱动程序会分配一个 `net_device` 结构，然后使用必须的程序对其进行初始化。这些程序中有一个是 
```
dev->hard_start_xmit
```
，它定义了上层应该如何对 `sk_buff` 排队进行传输。这个程序的参数为 
```
sk_buff
```
。这个函数的操作取决于底层硬件，但是通常 `sk_buff` 所描述的报文都会被移动到硬件环或队列中。就像是设备无关层中所描述的一样，对于 NAPI 兼容的网络驱动程序来说，帧的接收使用了
`netif_rx` 和 `netif_receive_skb` 接口。NAPI 驱动程序会对底层硬件的能力进行一些限制。



**三 核心数据结构**

**    网络体系结构中有几个核心数据结构**

   1)sk_buf

      网络层的数据都是通过sk_buf来传递的。

      与sk_buf相关的一些数据结构有:socket sock  proto  proto_ops;

   2)net_device

     设备无关层的统一接口.

     与 net_device相关的一些数据结构有device ...



先来看一下sk_buf的数据结构：





**[cpp]**[view plain](http://blog.csdn.net/fover717/article/details/4301637#)[copy](http://blog.csdn.net/fover717/article/details/4301637#)

- struct sk_buff  
- {  
- struct sk_buff        *next,*prev;  
- struct sk_buff_head   *list;  
- struct sock           *sk;                                  //sock结构指针
- struct timeval        stamp;  
- struct net_device     *dev, *rx_dev;  
- 
- union/* Transport layer header */
-    {  
- struct tcphdr   *th;  
- struct udphdr   *uh;  
- struct icmphdr  *icmph;  
- struct igmphdr  *igmph;  
- struct iphdr    *ipiph;  
- struct spxhdr   *spxh;  
-          unsigned char   *raw;  
-    } h;                                                             //传输层头
- union/* Network layer header */
-    {  
- struct iphdr    *iph;  
- struct ipv6hdr  *ipv6h;  
- struct arphdr   *arph;  
- struct ipxhdr   *ipxh;  
-          unsigned char   *raw;  
-    } nh;                                                          //网络层头
- union/* Link layer header */
-    {  
- struct ethhdr   *ethernet;  
-          unsigned char   *raw;  
-    } mac;                                                      //数据链路层头
- struct dst_entry      *dst;  
- char                  cb[48];  
-    unsigned int          len, csum;                   //数据长度，是否被消耗
- volatilechar         used;  
-    unsigned char         is_clone, cloned, pkt_type, ip_summed;   //是否克隆 已克隆 报文类型  校验和
-    __u32                 priority;  
-    atomic_t              users;  
-    unsigned short        protocol, security;  
-    unsigned int          truesize;  
-    unsigned char         *head, *data, *tail, *end;       //data和tail指向当前有效数据的头和尾
- void                  (*destructor)(struct sk_buff *);      //head和end指向socket的头和尾
-     ...  
- };  

其中pkt_type可以是如下类型之一





- 
PACKET_HOST specifies packet a sent to the local host.

- 
PACKET_BROADCAST specifies a broadcast packet.

- 
PACKET_MULTICAST specifies a multicast packet.

- 
PACKET_OTHERHOST specifies packets not destined for the local host, but received by special modes (e.g., the promiscuous mode).

- 
PACKET_OUTGOING specifies packets leaving the computer.

- 
PACKET_LOOPBACK specifies packets sent from the local computer to itself.

- 
PACKET_FASTROUTE specifies packets fast-forwarded between special network cards (fastroute is not covered in this book

sk_buf的结构图如下图所示：









下面两张图展示了sk_buff的copy和clone的区别：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/fover717/EntryImages/20090626/sk_buff_copy.jpg)

![](https://p-blog.csdn.net/images/p_blog_csdn_net/fover717/EntryImages/20090626/sk_buff_clone.jpg)



socket构成的队列：



![](https://p-blog.csdn.net/images/p_blog_csdn_net/fover717/EntryImages/20090626/sk_buff_queue.jpg)

2） net_device结构

   net_device是设备无关层的接口：这是一个巨型的数据结构





**[cpp]**[view plain](http://blog.csdn.net/fover717/article/details/4301637#)[copy](http://blog.csdn.net/fover717/article/details/4301637#)

- struct net_device  
- {  
- /*
-          * This is the first field of the "visible" part of this structure
-          * (i.e. as seen by users in the "Space.c" file).  It is the name
-          * the interface.
-          */
- char                    name[IFNAMSIZ];  
- /*
-          *      I/O specific fields
-          *      FIXME: Merge these and struct ifmap into one
-          */
-         unsigned long           rmem_end;       /* shmem "recv" end     */
-         unsigned long           rmem_start;     /* shmem "recv" start   */
-         unsigned long           mem_end;        /* shared mem end       */
-         unsigned long           mem_start;      /* shared mem start     */
-         unsigned long           base_addr;      /* device I/O address   */
-         unsigned int            irq;            /* device IRQ number    */
- /*
-          *      Some hardware also needs these fields, but they are not
-          *      part of the usual set specified in Space.c.
-          */
-         unsigned char           if_port;        /* Selectable AUI, TP,..*/
-         unsigned char           dma;            /* DMA channel          */
-         unsigned long           state;  
- struct net_device       *next;  
- /* The device initialization function. Called only once. */
- int                     (*init)(struct net_device *dev);  
- /* ------- Fields preinitialized in Space.c finish here ------- */
- struct net_device       *next_sched;  
- /* Interface index. Unique device identifier    */
- int                     ifindex;  
- int                     iflink;  
- struct net_device_stats* (*get_stats)(struct net_device *dev);  
- struct iw_statistics*   (*get_wireless_stats)(struct net_device *dev);  
- /* List of functions to handle Wireless Extensions (instead of ioctl).
-          * See <net/iw_handler.h> for details. Jean II */
- struct iw_handler_def * wireless_handlers;  
- struct ethtool_ops *ethtool_ops;  
- /*
-          * This marks the end of the "visible" part of the structure. All
-          * fields hereafter are internal to the system, and may change at
-          * will (read: may be cleaned up at will).
-          */
- /* These may be needed for future network-power-down code. */
-         unsigned long           trans_start;    /* Time (in jiffies) of last Tx */
-         unsigned long           last_rx;        /* Time of last Rx      */
-         unsigned short          flags;  /* interface flags (a la BSD)   */
-         unsigned short          gflags;  
-         unsigned short          priv_flags; /* Like 'flags' but invisible to userspace. */
-         unsigned short          unused_alignment_fixer; /* Because we need priv_flags,
-                                                          * and we want to be 32-bit aligned.
-                                                          */
-         unsigned                mtu;    /* interface MTU value          */
-         unsigned short          type;   /* interface hardware type      */
-         unsigned short          hard_header_len;        /* hardware hdr length  */
- void                    *priv;  /* pointer to private data      */
- struct net_device       *master; /* Pointer to master device of a group,
-                                           * which this device is member of.
-                                           */
- /* Interface address info. */
-         unsigned char           broadcast[MAX_ADDR_LEN];        /* hw bcast add */
-         unsigned char           dev_addr[MAX_ADDR_LEN]; /* hw address   */
-         unsigned char           addr_len;       /* hardware address length      */
- struct dev_mc_list      *mc_list;       /* Multicast mac addresses      */
- int                     mc_count;       /* Number of installed mcasts   */
- int                     promiscuity;  
- int                     allmulti;  
- int                     watchdog_timeo;  
- struct timer_list       watchdog_timer;  
- /* Protocol specific pointers */
- void                    *atalk_ptr;     /* AppleTalk link       */
- void                    *ip_ptr;        /* IPv4 specific data   */
- void                    *dn_ptr;        /* DECnet specific data */
- void                    *ip6_ptr;       /* IPv6 specific data */
- void                    *ec_ptr;        /* Econet specific data */
- struct list_head        poll_list;      /* Link to poll list    */
- int                     quota;  
- int                     weight;  
- struct Qdisc            *qdisc;  
- struct Qdisc            *qdisc_sleeping;  
- struct Qdisc            *qdisc_ingress;  
- struct list_head        qdisc_list;  
-         unsigned long           tx_queue_len;   /* Max frames per queue allowed */
- /* hard_start_xmit synchronizer */
-         spinlock_t              xmit_lock;  
- /* cpu id of processor entered to hard_start_xmit or -1,
-            if nobody entered there.
-          */
- int                     xmit_lock_owner;  
- /* device queue lock */
-         spinlock_t              queue_lock;  
- /* Number of references to this device */
-         atomic_t                refcnt;  
- /* The flag marking that device is unregistered, but held by an user */
- int                     deadbeaf;  
- /* Net device features */
- int                     features;  
- #define NETIF_F_SG              1       /* Scatter/gather IO. */
- #define NETIF_F_IP_CSUM         2       /* Can checksum only TCP/UDP over IPv4. */
- #define NETIF_F_NO_CSUM         4       /* Does not require checksum. F.e. loopack. */
- #define NETIF_F_HW_CSUM         8       /* Can checksum all the packets. */
- #define NETIF_F_DYNALLOC        16      /* Self-dectructable device. */
- #define NETIF_F_HIGHDMA         32      /* Can DMA to high memory. */
- #define NETIF_F_FRAGLIST        64      /* Scatter/gather IO. */
- #define NETIF_F_HW_VLAN_TX      128     /* Transmit VLAN hw acceleration */
- #define NETIF_F_HW_VLAN_RX      256     /* Receive VLAN hw acceleration */
- #define NETIF_F_HW_VLAN_FILTER  512     /* Receive filtering on VLAN */
- #define NETIF_F_VLAN_CHALLENGED 1024    /* Device cannot handle VLAN packets */
- /* Called after device is detached from network. */
- void                    (*uninit)(struct net_device *dev);  
- /* Called after last user reference disappears. */
- void                    (*destructor)(struct net_device *dev);  
- /* Pointers to interface service routines.      */
- int                     (*open)(struct net_device *dev);  
- int                     (*stop)(struct net_device *dev);  
- int                     (*hard_start_xmit) (struct sk_buff *skb,  
- struct net_device *dev);  
- #define HAVE_NETDEV_POLL
- int                     (*poll) (struct net_device *dev, int *quota);  
- int                     (*hard_header) (struct sk_buff *skb,  
- struct net_device *dev,  
-                                                 unsigned short type,  
- void *daddr,  
- void *saddr,  
-                                                 unsigned len);  
- int                     (*rebuild_header)(struct sk_buff *skb);  
- #define HAVE_MULTICAST
- void                    (*set_multicast_list)(struct net_device *dev);  
- #define HAVE_SET_MAC_ADDR
- int                     (*set_mac_address)(struct net_device *dev,  
- void *addr);  
- #define HAVE_PRIVATE_IOCTL
- int                     (*do_ioctl)(struct net_device *dev,  
- struct ifreq *ifr, int cmd);  
- #define HAVE_SET_CONFIG
- int                     (*set_config)(struct net_device *dev,  
- struct ifmap *map);  
- #define HAVE_HEADER_CACHE
- int                     (*hard_header_cache)(struct neighbour *neigh,  
- struct hh_cache *hh);  
- void                    (*header_cache_update)(struct hh_cache *hh,  
- struct net_device *dev,  
-                                                        unsigned char *  haddr);  
- #define HAVE_CHANGE_MTU
- int                     (*change_mtu)(struct net_device *dev, int new_mtu);  
- #define HAVE_TX_TIMEOUT
- void                    (*tx_timeout) (struct net_device *dev);  
- void                    (*vlan_rx_register)(struct net_device *dev,  
- struct vlan_group *grp);  
- void                    (*vlan_rx_add_vid)(struct net_device *dev,  
-                                                    unsigned short vid);  
- void                    (*vlan_rx_kill_vid)(struct net_device *dev,  
-                                                     unsigned short vid);  
- int                     (*hard_header_parse)(struct sk_buff *skb,  
-                                                      unsigned char *haddr);  
- int                     (*neigh_setup)(struct net_device *dev, struct neigh_parms *);  
- int                     (*accept_fastpath)(struct net_device *, struct dst_entry*);  
- /* open/release and usage marking */
- struct module *owner;  
- /* bridge stuff */
- struct net_bridge_port  *br_port;  
- #ifdef CONFIG_NET_FASTROUTE
- #define NETDEV_FASTROUTE_HMASK 0xF
- /* Semi-private data. Keep it at the end of device struct. */
-         rwlock_t                fastpath_lock;  
- struct dst_entry        *fastpath[NETDEV_FASTROUTE_HMASK+1];  
- #endif
- #ifdef CONFIG_NET_DIVERT
- /* this will get initialized at each interface type init routine */
- struct divert_blk       *divert;  
- #endif /* CONFIG_NET_DIVERT */
- };   


 这个数据结构比较巨大。我们分几组一点一点来看
1 网络设备的通用字段
   name          网络适配器的名字





  *next          用于连接多个网络设备





   owner         moudle结构指针





   ifindex        除name外的网络设备第二标识符





   iflink           指定了用来发包的网络设备的索引
   state          网络设备的状态可以是以下值：LINK_STATE_START，LINK_STATE_X0FF（替代以前的tbusy字段）





                     下列3种方法可以改变state的值：netif_stop_queue(dev);





                                               netif_start_queue(dev);





                                               netif_wake_queue(dev);
  trans_start           传输计时开始



  *priv                 指向其私有数据的指针



  qdisc                 映射当前网络服务策略
  refcnt                当前网络设备引用计数 



  xmit_lock             发送时的锁



  queue_lock            队列的锁 



  xmit_lock_owner   获得发送时的锁的处理器编号



2 硬件相关字段



  rmem_end      接受内存尾地址
  rmem-start     接受内存首地址



  mem_end        发送内存尾地址



  mem_start      发送内存首地址



  base_addr      网络设备的基地址（见后图）
  irq                  中断号



  if_port            端口号




3 物理层上的数据



  hard_header_length                     第二层包报头长度



  mtu                                    最大传输单元



  tx_queue_len                           网络设备输出队列最大长度



  type                                   网络适配器硬件类型



  addr_len                               第二层地址长度



  dev_addr[MAX_ADDR_LEN]       第二层地址



  broadcast[MAX_ADDR_LEN]      广播地址



  *mc_list                                    指向具有多播第二层地址的线性表



  mc_count                                  dev_mc_list中的地址数量（多播地址数）



  watchdpg_timeo                       超时时间（从trans_start开始，经过watchdog_timeo时间后超时）





4 网络层上的数据 



 ip_ptr,ip6_ptr,atalk_ptr,dn_ptr,ec_ptr      指向网络适配器第三层协议信息，如ip_ptr指向in_device结构。该结构



                                             包含ip地址列表，多播ip列表，ARP协议参数等






  pa_len                                     协议地址长度



  pa_dstaddr                                 点对点连接中的目的地址



  flags                                      各种不同的开关，他们都可以通过ifconfig命令来设置




|Flag|Meaning|
|----|----|
|IFF_UP|The network device is activated and can send and receive packets.|
|IFF_BROADCAST|The device is broadcast-enabled, and the broadcast address pa_braddr is valid.|
|IFF_DEBUG|This flag switches the debug mode on (currently not used by any driver).|
|IFF_LOOPBACK|This flag shows that this is a loopback network device.|
|IFF_POINTOPOINT|This is a point-to-point connection. If this switch is set, thenpa_dstaddr should contain the partner's address.|
|IFF_NOARP|This device does not support the Address Resolution Protocol (ARP) (e.g., in point-to-point connections).|
|IFF_PROMISC|This flag switches the promiscuous mode on. This means that all packets currently received in the network adapter are forwarded to the upper layers, including those not intended for this computer. This mode is of interest for tcpdump only.|
|IFF_MULTICAST|This flag activates the receipt of multicast packets. ether_setup() activates this switch. A card that does not support multicast should delete this flag.|
|IFF_ALLMULTI|All multicast packets should be received. This is required when the computer is to work as multicast router.IFF_MULTICAST has to be set in addition.|
|IFF_PORTSEL|Setting of the output port is supported by the hardware.|
|IFF_AUTOMEDIA|Automatic selection of the output medium (autosensing) is enabled.|
|IFF_DYNAMIC|Dynamic change of the network device's address is enabled (e.g., for dialup connections).|
   5 设备驱动程序的函数



   init()                 搜索并初始化网络设备



   uninit()             注销网络设备



   destructor()     当网络设备的最后一个引用refcnt被删除时调用此函数



   open ()            打开网络设备



   stop()               关闭网络设备



   hard_start_xmit()        发送包，成功返回0，否则返回1



   get_stats()                  获取网络设备状态信息，这些信息以net_device_stats 结构的形式返回



   get_wireless_stats()   获取无限网络设备的状态信息，这些信息以iw_statistics 结构的形式返回



   set_multicast_list()     将多播MAC地址传给网络适配器



   watchdog_timeo()      超时处理函数



   do_ioctl()                    向网络驱动程序传递网络适配器相关的ioctl()命令



   set_config()                运行时改变网络适配器的配置



   上面所列的方法依赖于所使用的网络适配器，也就是说如果需要他们的功能则必须由驱动程序来提供。



   下面所列的方法较少依赖于适配器，不必由驱动程序相关方法实现。



     hard_header()                        根据源和目标第二层地址创建二层报头



     rebuild_header()                     重建第二层报头



     hard_header_cache()             用硬报头缓存中保存的数据填充第二层报头



     header_cache_update()         更改硬报头缓存中保存的第二层报头数据
     hard_header_parse()            从套接字缓冲区的包数据空间读取第二层报头的发送地址



     set_mac_address()                设mac地址



     change_mtu()                        改变mtu长度
最后给一个图以便于理解：




