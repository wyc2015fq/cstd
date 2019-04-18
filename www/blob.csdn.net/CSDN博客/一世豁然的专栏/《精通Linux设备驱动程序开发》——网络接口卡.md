# 《精通Linux设备驱动程序开发》——网络接口卡 - 一世豁然的专栏 - CSDN博客





2015年04月08日 20:59:25[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1440








一、驱动程序数据结构

 一）、简介

  1、在为一个NIC写设备驱动程序时，必须操作3类数据结构

   1）、形成网络协议栈构造块的数据结构。套接字缓冲区，即定义在include/linux/sk_buff.h文件中的结构体sk_buff，是内核TCP/IP栈使用的关键结构体。

   2）、定义NIC驱动程序和协议栈间接口的数据结构。定义在include/linux/netdevice.h文件中的结构体net_device是构成该接口的核心结构体。

   3）、与I/O总线相关的结构体。PCI及其派生总线的如今NIC使用的常见总线。







 二）、套接字缓冲区

  1、sk_buff为网络层提供了高效的缓冲区处理和流量控制机制。。sk_buff拥有描述符对应内存缓冲区的控制信息。




  2、sk_buff用5个主要的域将它自身与数据包缓冲区联系起来。

   1）、head：用于指向数据包的开始。

   2）、data：用于指向数据包载荷的开始。

   3）、tail：用于指向数据包载荷的结束。

   4）、end：用于指向数据包的结束。

   5）、len：数据包包含的数据量。




  3、sk_buff操作

![](https://img-blog.csdn.net/20150407104119611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






 三）、网络设备接口

  1、NIC驱动程序使用标准接口与TCP/IP协议栈胡操作。net_device结构体定义了此通信接口，它甚至比sk_buff结构体大。




  2、NIC驱动程序初始化步骤

   1）、驱动程序用alloc_netdev()分配一个net)device结构体，不过更常见的是它为alloc_netdev()做合适的封装。

   2）、接下来，驱动程序在分配的net_device中填充多个成员，并用register_netdev(netdev)向网络层注册net_device。

   3）、驱动程序从EEPROM读取NIC的MAC（媒体介入控制）地址，如果还需要，还得配置WOL（网络唤醒）。

   4）、如果NIC需要卡上固件，驱动程序就用request_firmware()下载它。







 四）、激活

  1、net_device接口需要用到open()、close()和ioctl()传统方法，如果用ifconfig等工具激活它，内核就会打开一个接口：

**bash>ifconfig  eth0  up**

        open()函数会设置接收/发送DMA描述符和一些其他数据结构。它也通过调用request_irq()注册NIC中断处理程序。net_device结构体将被作为devid参数传给

        request_irq()，这样中断处理程序可以直接访问相关的net_device。




  2、销毁一个活动的网络接口时，内核调用close()，它完成与调用open()相反的功能。







 五）、数据传输

  1、数据传输函数是net_device接口的关键组件。在传输路径上，驱动程序提供一个称为：hard_start_xmit函数，协议层在发送时调用它来向下传输数据包：

       hard->hard_start_xmit = &mycard_xmit_frame;




  2、至今，网络驱动程序都没有在net_device结构体中提供一个搜索接收数据的成员函数，它只是异步中断协议层，并向其告知数据包载荷。但这个老的接口已经被NAPI取

        代。NAPI是中断驱动的驱动程序所采用的“推”模式和轮询驱动程序协议所采用的“拉”模式的混合。







 六）、看门狗

  1、网络设备接口提供一个钩子，用于向操作状态返回无应答的NIC。如果协议层在预定的传输时间没有感觉到传输操作，它就假设NIC已经挂起，然后调用驱动程序提供的复

        位函数来重启卡。




  2、驱动程序通过netdev->watchdog_tomeo设置看门狗超时，然后通过net->tx_timeout注册复位函数的地址；

        net->tx_timeout = &mycard_timeout;

        netdev->watchdog_timeo = 8*HZ;







 七）、统计

  1、为了让用户能搜集网络统计数据，NIC驱动程序生成一个net_device_stats结构体，并提供了get_stats()函数来接收它。




  2、驱动程序做了如下工作：

   1）、从相关入口点更新不同类型的统计数据。

   2）、实现get_stats()方法来接收统计数据。

   3）、向高层提供接收方法。








 八）、配置

  1、NIC驱动程序需要支持用户空间工具，这些工具负责设置和提取参数。ethtool工具可以为以太网NIC配置参数。




  2、为了支持ethtool，底层的NIC驱动程序做了以下工作：

   1）、填充包含入口点的ethtool_ops结构体，定义在include/linux/ethtool.h中。

   2）、实现作为ethtool_ops成员的函数。

   3）、输出ethtool_ops的地址。




 九）、总线相关内容










二、与协议层对话

 一）、接收路径

  1、NIC驱动程序用NET_RX_SOFTIRQ转移了将收到数据包投递到协议层的工作，他是通过从接收中断处理程序中调用netif_rx()实现的。NAPI改进了这种传统的中断驱动的接

        受方式。NAPI的策略是当网络活动频繁时用轮询模式，而当流量变小时转回到中断模式。




  2、支持NAPI的驱动程序根据网络负载在中断模式和轮训模式间切换，过程如下：

   1）、在中断模式下，中断处理程序通过调度NET_RX_SOFTIRQ向协议层投递接收到的数据包，然后屏蔽NIC中断，接着通过向轮训列表增加设备来轮询模式。

   2）、驱动程序通过它的net_device结构体提供了一个poll方法。

   3）、在轮询模式下，驱动程序的poll()方法处理接收队列中的数据包。当队列变空时，驱动程序重新启用中断，并通过调用netif_rx_complete()切回到中断模式。







 二）、发送路径

  1、对于数据发送，协议层和NIC驱动程序间的互操作是直接的，协议栈是以sk_buff作为输出参数调用驱动程序的hart_start_xmit()方法，驱动程序用DMA方式将数据包数据取

        出送给NIC。




  2、驱动程序在完成预定数量数据包的传输后，设置NIC向处理器发出中断请求。只有当表示传输操作完成的传输完成中断出现时，驱动程序才能收回释放资源。







三）、流量控制

  1、驱动程序分别通过调用netif_start_queue()和netif_stop_queue()来告知它已经准备好或还不能接收协议数据。










三、缓冲区管理和并发控制

  1、一个高性能的NIC驱动程序是一个需要复杂数据结构管理的软件综合体。








四、设备实例：以太网NIC。

  1、代码清单15-1（一个以太网NIC驱动程序）。







五、ISA网络驱动程序

![](https://img-blog.csdn.net/20150408204720220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




六、ATM

  1、ATM（异步传输模式）是一种高速的、面向连接的骨干网技术。ATM保证高的QoS（服务质量）和低延时，一次它处传输数据外还被用于传输音频和视频流。




  2、ATM协议简单总结：

![](https://img-blog.csdn.net/20150408205229772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




  3、ATM上传输TCP/IP的方法有三种：

   1）、RFC1577中规定的CLIP（传统IP异步传输模式）。

   2）、在ATM网上模拟局域网，称为LANE（虚拟局域网）。

   3）、MPOA（ATM网承载多种协议）。




  4、MPLS（多协议标签交换）的协议正在逐步取代ATM。







七、网络吞吐量（略）






