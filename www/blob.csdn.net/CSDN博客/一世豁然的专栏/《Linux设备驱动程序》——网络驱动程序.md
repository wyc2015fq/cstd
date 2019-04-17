# 《Linux设备驱动程序》——网络驱动程序 - 一世豁然的专栏 - CSDN博客





2014年12月09日 21:23:34[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1166








一、概论

  1、系统中网络接口的角色，和一个已挂载的块设备类似。网络接口也必须使用特定的内核数据结构注册自身，以备与外界进行数据包交换调用。




  2、在已挂载磁盘和数据包发送接口之间，还是存在许多不同之处，首先，一个磁盘在/dev目录下作为一个特殊文件而存在，而网络接口却没有这样的入口。




  3、网络设备和块设备最重要的区别是：块设备只相应来自内核的请求，而网络驱动程序异步地接收来自外部世界的数据包。网络驱动程序还将准备支持大量的管理任务。




  4、Linux内核中的网络子系统被0设计成完全与协议无关。内核与网络驱动程序之间的交互，可能每次处理的是一个网络数据包；协议隐藏在驱动程序之后，同时物理传输又被

        隐 藏在协议之口。




  5、术语octet在网络世界中指一组8个的数据位，踏实能为网络设备和协议所能理解的最小单位。










二、snull设备

 一）、相关简介

  1、snull最重要的设计抉择：示例接口仍然不依赖任何硬件。




  2、支持IP流，这是该接口的内部工作方式所决定的——为了正确模拟一对硬件接口，snull必须观察并解释数据包。







 二）、分配IP号

  1、通过某一个接口发送出的数据包应被另一个接口接收，但是不能将外发数据包的接收者认为是本机。同样的规则也应该应用于已接收数据包的源地址。




  2、为了实现“隐藏的回环”设备，snull接口切换源地址和目标地址的第三个octet的最低位；也就是说，它修改了C类IP号的网络编号和主机编号。其效果是，发送到网络A（连

        接到sn0，即第一个接口）的数据包，将在属于网络B的sn1接口上出现。




  3、为了避免太多的数字，赋予相关的IP号一些符号名：

    1）、snullnet0是连接到sn0接口的C类网络。类似的，snullnet1是连接到sn1接口的网络。

    2）、llcal是赋予sn0接口的IP地址，它属于snullnet0.和sn1关联的地址是local1。

    3）、remote0是snullnet0网络中的一个主机，它的第四个octet和local1相同。




  4、不管选择什么地址编号，可通过如下命令设置接口：

        ifconfig  sn0  local0

        ifconfig  sn1  local1

        如果选择的范围并不是C类地址，那么需要添加子网掩码255.255.255.0。







 三）、数据包物理传输

  1、对数据传输而言，snull接口属于以太网类型。




  2、对snull来说，使用以太网的最后一个优点可以在该接口上运行tcpdump看到数据包的传输情况。










三、链接到内核

 一）、设备注册

  1、当一个模块被装载到正在运行的内核中时，它要请求资源并提供一些功能设施，这点上，与网络驱动程序也一样，而且在资源请求方式上也没有任何的不同。驱动程序要按

        照“安装中断处理例程”的方法探测起设备和硬件位置（I/O端口以及IRQ线），但不需要进行注册。




  2、网络驱动程序程序对每个新检测的接口，向全局的网络设备链表插入一个数据结构。每个接口由一个**net_device接口**描述，其定义在<linux/netdevice.h>中。




  3、用来执行分配的内核函数是alloc_netdev，它有着如下的原型：

        struct  net_device  *alloc_netdev(int  sizeof_priv,  const  char  *name,

                                                                    void  (*setup) (struct   net_device  *));




  4、网络子系统针对alloc_netdev函数，为不同种类的接口封装了许多函数。最常用的是alloc_etherdev，他咋u<linux/etherdevice.h>中定义：

        struct  net_device  *alloc_etherdev(int  sizeof_priv);




  5、其他类型设备的驱动程序作者也许要使用其他的封装函数，比如光纤通道设备使用alloc_fedev函数，为FDDI设备使用alloc_fddidev函数，或者为令牌环设备使用

        alloc_trdev函数。







 二）、初始化每个设备

  1、请注意，在运行时，net_device结构是被聚集在一起的；不能像处理file_opration和block_device_opration结构那样在编译时进行初始化。在调用register_netdev前，初始

         化必须完成。net_device结构即大又复杂但，内核在ether_setup函数中每个结构设置了许多默认值。




  2、net_device结构的一个成员——priv，该成员的作用和字符驱动程序中private_data指针类似。但和fops->private_data不同，priv指针是与net_device结构一起分配的。







 三）、模块的卸载

  1、unregister_netdev函数从系统中删除了接口；free_netdev函数将net_device结构返回给了系统。如果还存在什么地方有对该结构的引用，则它将继续存在，但是驱动程序

        并不需要关注这一点。一旦注销了接口，内核不会再调用这个函数。










四、net结构细节

  1、全局信息（P499-P500）

  2、硬件信息（P500-P501）

  3、接口信息（P501-P505）

  4、设备方法（P505-P507）

  5、工具成员（P507-P508）










五、打开和关闭


  1、驱动程序可在装载阶段或者内核引导阶段探测接口。但是在接口能够传输数据包之前，内核必须打开接口并赋予地址。内核可在响应ifconfig命令时打开或关闭一个接口。




  2、在使用ifconfig向接口赋予地址时，要执行两个任务：

    1）、首先，它通过ioctl(SIOCSIFADDR)赋予地址。

    2）、然后通过ioctl(SIOCSIFFLAGS)设置dev->flags中的IFF_UP标志以打开接口。




  3、对设备而言，无需对ioctl(SIOCSIFADDR)做任何工作。内核不会调用任何驱动程序函数，也就是说，该任务由内核来执行，是与设备无关的。而后一个命令


        ioctl(SIOCSIFFLAGS)会调用设备的open方法。类似的，在接口被关闭时，ifconfig使用ioctl(SIOCSIFFLAGS)来清除IFF_UP标志，然后调用stop函数。




  4、对实际代码而言，驱动程序必须执行许多和字符及块设备相同的任务。open请求必要系统资源，并告诉接口开始工作；stop关闭接口并释放系统资源，但是除此之外，还

        要执行一些其他的步骤：

    1）、首先，在接口能够和外界通讯之前，要将硬件地址(MAC)从从硬件设备复制到dev->dev_addr。硬件地址可在打开期间拷贝倒设备之中。

    2）、一旦准备好开始发送设备之后，open方法还应该启动接口的传输队列（允许接口接受传输数据包）。内核提供的如下还是农户可启动该队列：

           void  netif_start_queue(struct  net_device  *dev);

    3）、要想标记设备不能传输其他数据包。在接口被关闭时（在stop函数中），必须调用以下函数，但是该函数也可以用来临时停止传输。

          void  netif_stop_queue(struct  net_device  *dev);







六、数据包传输

 一）相关简介

  1、传输是指将数据包通过网络连接发送出去的行为。

    1）、无论何时内核要传输一个数据包它都会调用驱动程序的hard_start_transmit函数将数据放入外发队列。

    2）、内核处理的每个数据包位于一个套接字缓冲区结构(**skb_buff**)中，该结构定义在<linux/skbuff.h>中。




  2、套接字缓冲区是一个复杂的结构，内核提供了额许多用来操作该结构的函数。




  3、传递给hard_start_xmit的套接字缓冲区包含了物理缓冲区，并拥有完整的传输层数据包头。接口无需修改要传输的数据。skb->data指向要传输的数据包，而skb->len是以

         octet为单位的长度。如果驱动程序能够处理scatter/gatherI/O，新式将变得有些复杂。







 二）、控制并发传输

  1、hard_start_xmit函数通过net_device结构中的一个自旋锁(xmit_lock)获得并发调用时的保护。另一方面，实际的硬件接口却是异步传输数据包，而且可以用来保存外发数据

        包的存储空间有限。在内存被耗尽时，驱动程序需要告诉网络系统在硬件能够接受新的数据之前，不能启动其他数据包传输。




  2、在驱动程序停止队列之后，它必须在将来的某个时刻，当它能够再次接受数据包的传输时，重新启动该队列：

        void  netif_wake_queue(struct  net_device  *dev);




  3、如果想从其他地方，而不是从hard_start_xmit函数中禁止数据包传输，则要调用下列函数：

        void  netif_tx_disable(struct  net_device  *dev);







 三）、传输超时

  1、大部分处理实际硬件的驱动程序必须能够应付硬件偶尔不能正确响应的问题。醋多驱动程序利用定时器处理这类问题；如果某个操作在定时器到期时还未完成，则认为出现

        了问题。




  2、从本质上讲，网络系统是通过大量定时器控制的多个状态机的复杂组合。从这个角度上讲，网络代码把检测传输超时作为其常用操作之一。因此网络驱动程序无需自己检测

        这种问题。

    1）、驱动程序只需设置一个超时周期，并在net_device结构的watchdog_timeo成员中设置。

    2）、如果当前的系统时间超过设备的trans_start时间至少一个超时周期，网络层将最终调用驱动程序的tx_timeout函数。







 四）、Scatter/Gather I/O

  1、在网络上未传输工作创建数据包的过程，包括组装多个数据片段的过程。




  2、只有在device结构中的feature成员设置了NETIF_F_SC标志位，内核才将分散的数据包传递给hard_start_xmit函数。如果设置了该标志，还需要检查skb中的“共享信息”成

        员，以判断数据包是由一个数据片段组成，还是由大量数据片段组成，并且负责查找所需要的分散数据片段。










七、数据包的接收

  1、网络驱动程序实现了两种模式接收数据包：中断驱动方式和轮询方式。大多数驱动程序实现了中断驱动技术。




  2、snull中的snull_rx函数代码及其分析。（P517-P518）




  3、使用中断会会使系统性能全面下降。







八、不使用接收中断

  1、为了能提高Linux在宽带系统上的性能，网络子系统开发者创建了另外一种基于轮询方式的接口。




  2、停止使用中断会减轻处理器的负荷。由于网络堵塞，数据包处于网络代码中时，NAPI型的驱动程序将不会把数据包发送给内核，这将提高系统的性能。但并不是所有的设

         备都能在NAPI模式下工作。




  3、轮询函数的原型：

      int  (*poll) (struct  net_device  *dev,  int  *budfet);




  4、NAPI驱动程序创建

    1）、当把yse_napi参数设置为非零时装载snull驱动程序，则使用NAPI模式操作。在初始化时，必须对一对额外的structnet_device成员进行设置：

           if(use_napi)  {

              dev->poll  =  snull_poll;

              dev->weight  =  2

           };

    2）、创建NAPI驱动程序的下一步是修改中断处理例程。





  5、snull中的snull_poll函数代码及其分析。







九、链路状态的改变

  1、大多数涉及实际的物理连接的网络技术提供载波状态的信息：载波的存在意味着硬件功能是正常的。




  2、默认情况下，网络设备假定存在载波信号。但是，利用下面的函数，驱动程序可以显式改变这个状态：

     void  netif_carrier_off(struct  net_device  *dev);

     void  netif_carrier_on(struct  net_device  *dev);




  3、还存在一个用来检测在播的状态

     int  netif_carrier_ok(struct  net_device  *dev);







十、套接字缓冲区

  1、sk_buff结构成员（P522-P523）




  2、操作套接字缓冲区的函数（P523-P525）










十一、MAC地址解析

 一）、在以太网中使用ARP

  1、通常处理地址解析的方法是使用ARP，即地址解析协议。幸运的是，ARP由内核维护，而以太网接口不需要做任何特殊工作就能支持ARP。

    1）、只要在打开时正确设置dev->addr和dev->addr_len，驱动程序九五需担心级将IP号解析为MAC地址这件事。

    2）ether_setup将把正确的设备函数赋予dev->hard_header和dev->reduild_header。




  2、尽管通常是由内核处理地址解析的细节。但它要调用接口的驱动程序来帮助建立数据包。

    1）、驱动程序了解物理层数据包头的细节，而网络代码的作者试图将内核的其余部分和ARP隔离开来。

    2）、内核调用驱动程序的hard_header函数，将ARP查询的结果安排在数据包的适当位置。







 二）、重载ARP

  1、类似plip的简单点对点网络可从以太网获得一些信息，但可避免来回发送ARP数据包而带来的开销。




  2、snull不能使用ARP，这是因为驱动程序修改了正在传输的数据包中的IP地址，而同时ARP数据包也修改了IP地址。尽管可以实现一个简单的ARP应答生成器，但解释直接

        处理物理层数据包的方法，要更为直观一些。




  3、如果设备希望使用常用的硬件头，而不运行ARP，则需要重载默认的dev->hard_header函数。（snull函数的实现）







 三）、非以太网头

  1、注意并不是所有的信息都能为各个协议所提供。点对点链路可在硬件头中省略地址，而仅仅传输协议号。




  2、当链路的另一端接收到数据包时，驱动程序的接收函数应该正确设置skb->protocol，skb->pkt_type和skb_mac.raw。skb->mac_raw是一个字符指针，由上层网络代码实

        现的地址解析机使用，它必须指向与dev->type匹配的一个机器地址，设备类型的可能值定义在<linux/if_arp.h>中。










十二、定制ioctl命令

  1、单位某个套接字使用ioctl调用时，命令号是定义在<linux/socket.h>中的某个符号，而函数sock_ioctl直接调用一个协议相关的函数。




  2、任何协议层不能识别的ioctl命令都会传递到设备层。这些设备相关的ioctl命令从用户空间接受第三个参数，即一个struct  ifreq  *指针。

    1）、SIOCSIFADDR和SIOCSIFMAP命令实际使用了ifreq结构。

    2）、SIOCSIFMAP的其他参数，尽管被定义为ifmap，但其实只是ifreq的一个成员。

    3）、除了使用标准化调用之外，每个接口可以定义它自己的ioctl命令。





  3、套接字的ioctl实现能够识别16个接口私有的命令：从SIOCDEVPRIVATE到SIOCDEVPRIVATE+15。




  4、如果识别出的是上述命令之一，则在相关接口驱动程序中调用dev->do_ioctl。该函数接收相同的struct  ifreq  *指针，其与常用ioctl函数使用一样。

        int  (*do_ioctl) (struct  net_device  *dev,  struct  ifreq  *ifr,  int  cmd);







十三、统计信息

  1、get_stats函数返回设备统计结构的指针。他的实现按非常简单。




  2、net_device_stats结构中的成员。（P529）







十三、组播

 一）、相关简介


  1、组播数据包是期望由多于一个主机、但不是所有主机接收的网络数据包。这一功能可通过赋予针对一组主机的特殊硬件地址完成。




  2、通常来说，考虑组播时，硬件可划分为三类：

    1）、不能处理组播的接口。

    2）、能够区分组播数据报和其他数据包的接口。

    3）、能够为组播地址进行硬件检测的地址。




 二）、对组播的内核支持

  1、对组播数据包的内核支持由如下几项组成：一个设备函数，一个数据接口以及若干设备标志。（P531）




  2、dev_me_list结构的定义，结构在<linux/netdevice.h>中定义：

         struct  dev_mc_list {

             struct  dev_mc_list  *next;

             __u8  dmi_addr[MAX_ADDR_LEN];

             unsigned  char  dmi_addrlen;

             int  dmi_users;

             int  dmi_gusers;

          };




  3、典型实现（P532）










十四、其他知识点详解

 一）、对介质无关接口的支持

  1、介质无关接口（MII）是一个IEEE802,3标准，它描述了以太网收发器是如何与网络控制器连接的。




  2、为了使用通用MII层，需要包含头文件<linux/mii.h>。无论是否使用全双工模式，都需要用收发器的物理ID填写mii_if_info接口。为了使用mii_if_info接口，还需要两个函

        数：

        int  (*mdio_read) (struct  net_device  *dev,  int  phy_id,  int  location);

        int  (*modio_write) (struct  net_device  *dev,  int  phy_id,  int  location,  int  val);







 二）、ethtool支持

  1、ethtool是为系统管理员提供的用于控制网络接口的工具。只有当驱动程序支持ethtool时，使用ethtool才能控制包括速度、介质类型、双工操作、DMA设置、硬件检验、

         LAN唤醒操作在内的许多接口参数。




  2、在<linux/ethtool.h>中含有支持ethtool的相关声明。其核心是一个ethtool)ops类型的接口，它包含了ethtool支持的全部24个函数。这些函数中的大多数是相互关联的。如果

        驱动程序使用了MII层，可以使用mii_ethtool_gset和mii_ethtool_sset以分别实现get_setting和ser_settting函数。







 三）、Netpoll

  1、Netpoll的目的是让内核在网络和I/O子系统尚不能完整可用时，依然能发送和接收数据包。




  2、实现Netpoll的驱动程序需要实现poll_controller函数。该函数的作用是在缺少设备中断时，还能对控制器做出响应。几乎所有的poll_controller函数都有如下的形式：

        void  my_poll_controller(struct  net_device  *dev)

       {

           disable_device_interrupts(dev);

           call_interrupts_handler(dev->irq,  dev,  NULL);

           reenable_device_interrupts(dev);


       }




