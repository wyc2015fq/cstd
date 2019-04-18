# 《精通Linux设备驱动程序开发》——Linux无线设备驱动 - 一世豁然的专栏 - CSDN博客





2015年04月21日 16:29:44[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2468








一、简介

  1、小设备无线技术：

   1）、蓝牙不用电缆就能将智能注入到愚笨的设备，开启了开发各类心颖应用的大门。

   2）、红外是一种低功耗、小范围、中速率的无线技术，能将笔记本计算机、手持设备组网，或文档发往打印机。

   3）、WiFi是以以太网的无线衍生物。

   4）、蜂窝网络利用GPRS或CDMA使你在移动情况下还能接入因特网。




  2、无限技术选用的权衡

   1）、蓝牙、红外、WiFi和GPRS用于不同的环境，可根据速率、范围、成本、功耗、软硬件协同设计、PCB布板面积等因素进行选择。

   2）、无线技术选用权衡

![](https://img-blog.csdn.net/20150416154608860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)










二、蓝牙

 一）、简介

  1、蓝牙是一种短距离无线技术，它能传输数据和语音，支持最高723kbit/s（非对称）和432kbit/s（对称）的速率。第三类蓝牙设备有10m的传输距离，第一类的发射机最远

       能传100m。




  2、蓝牙协议栈

![](https://img-blog.csdn.net/20150416155600541?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 二）、BlueZ

  1、BlueZ蓝牙功能是传统内核的一部分，也是官方Linux蓝牙协议栈。




  2、蓝牙协议层与BlueZ内核模块的关系

![](https://img-blog.csdn.net/20150416160012388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、bluetooth.ko包含了核心BlueZ基础结构。所有其他BlueZ模块都使用它的服务。它还负责蓝牙套接字系列（AF_BLUETOOTH）导出到用户空间，负责生成相关sysfs入

             口。

   2）、在UART上传输蓝牙HCI包的相应BlueZ HCI实现是hci_uart.ko。用于USB传输的则是hci_usb.ko。

   3）、l2cap.ko完成L2CAP适配层。它负责分段和重组，也对不同高层协议实现多路复用。

   4）、要在蓝牙上运行TCP/IP应用程序，必须通过基于BNEP的L2CAP模拟以太网端口。

   5）、为在蓝牙上运行串行端口应用程序，需要在L2CAP上模拟串行端口。这由rfcomm.ko完成。

   6）、HID（用户接口设备）层通过hipe.ko实现。

   7）、通过SCO（面向同步连接层）处理音频，其驱动模块为sco.ko。







 三）、设备实例：CF卡

  1、Sharp蓝牙CF卡是基于Silicon Wave芯片制造的，并串行传输HCI包。HCI包的传输有三种不同方式：

   1）、H4（UART）。

   2）、H3（RS232）。

   3）、BCSP（串行协议）。




  2、从Sharp蓝牙CF卡读取数据的流程

![](https://img-blog.csdn.net/20150417093022864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







 四）、设备实例：USB适配器

  1、USB适配器：Belkin蓝牙USB适配器，是一个gadget设备。




  2、对蓝牙USB设备来说，每个管道负责传输特殊类型的数据：

   1）、控制管道用于传输HCI命令。

   2）、中断管道用于传输HCI事件。

   3）、批量管道用于传输ACL（异步无连接）蓝牙数据。

   4）、同步管道传输CSO语音数据。







 五）、RFCOMM

  1、RFCOMM在蓝牙上模拟串行端口。中断仿真器等应用程序以及PPP等协议不用改变就能运行在RFCOMM创建的虚拟串行接口上。




  2、设备实例：配药机

   1）、通过RFCOMM与配药机同信（代码清单16-1）







 六）、网络

  1、在蓝牙上建设网络的两种方法：

   1）、直接在BNEP上运行TCP/IP。这样的网络称为PAN（个域网）。

   2）、在RFCOMM上的PPP上运行TCP/IP，这称为DUN（拨号网）。




  2、使用蓝牙PAN联网两台笔记本计算机

![](https://img-blog.csdn.net/20150417153416451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




 七）、HID




 八）、音频（以HBH-30 Song Erisson蓝牙头戴式耳机）

  1、头戴耳机在开始与linux设备通信之前，后者的蓝牙链路层必须发现前者。为此，按下搜寻按钮，使耳机进入搜索模式。




  2、另外需要在/etc/bluetooth/pin中添加PIN，一边为BlueZ配置耳机的识别号。




 九）、调试

  1、有两种BlueZ调试工具

   1）、hcidump。

   2）、虚拟HCI驱动程序。




 十）、源代码










三、红外

 一）、简介

  1、红外（IR）线是一种光波，其电磁频谱在可见光和微波之间。IR的一种用途是点对点数据通信。利用IR，你可以在PDA间交换名片，连接两台笔记本计算机，或将文档发送

        给打印机。




  2、IR通信有两种流行的方式：SIR（标准IR），它支持最高115.20kdaud；FIR（快速IR）有4Mbit的传输速度。




  3、笔记本计算机和嵌入式设备上的IrDA![](https://img-blog.csdn.net/20150417154633016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




   4、Linux在两个层面上支持IR通信：

   1）、用IrDA（红外数据协会）规定的协议进行人工智能数据传输。

   2）、通过远程控制的控制程序。






 二）、Linux-IrDA

  1、Linux-IrDA项目使内核具备了IrDA能力




  2、Linux-IrDA上的通信

![](https://img-blog.csdn.net/20150421085451451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   1）、设备驱动程序构成最底层，与16550兼容的SIR芯片在用IrDA芯片组线路规程IrTTY规程它的行为后，可以重用本身的Linux串行驱动程序。

   2）、接下来是协议栈。它由 IR IrLAP、IrLMP TinyTP以及IrSock接口组成。

   3）、栈的高层部分连接IrDA与数据传输程序。

   4）、还需要能利用或破坏该技术的程应用序。







 三）、设备实例：超级I/O芯片（略）




 四）、设备实例：IR Dongle

  1、dongle是能插到串行端口或USB端口的设备。有些微控制器具有连接到UART的片上IR控制器，它们也被称为dongle。




  2、dongle驱动程序是一套负责改变通信速率等操作的一套方法，它有四个入口点：open（）、reset（）、change_speed（）和close。




  3、IrDA核心为dongle驱动程序提供了3个辅助函数：用于和相应UART交换控制数据的sirdev_raw_write()和sirdev_raw_read()，用于调整连接到UART的调制解调器控制线的

        sirdev_set_dtr_rts()。




  4、实例dongle驱动程序：代码清单16-2。







 五）、IrCOMM

  1、IrCOMM能模拟串行端口。终端仿真器等应用程序和PPP等协议可以不用改变就能运行在由IrCOMM创建的虚拟串行接口上。




  2、IrCOMM由两个相关的模块实现的，即ircomm.ko和ircomm_tty.ko前者提供了对核心协议的支持。后者创建和管理虚拟的串行端口节点/dev/ircommX。







 六）、联网

  1、有三种方法可以让TCP/IP程序运行在IrDA上

   1）、在IrDA上运行异步PPP；

   2）、在IrNET上运行同步PPP；

   3）、在IrLAN上进行以太网模拟。




 七）、IrDA（略）




 八）、LIRC

  1、LIRC项目的目标是让你能用遥控器控制Linux计算机。




  2、LIRC的结构包含如下组成部分

   1）、称为lirc_dev的基本LIRC模块。

   2）、与硬件相关的物理层驱动程序。

   3）、一个称为lirc的用户空间守护程序运行在底层LIRC驱动程序上。

   4）、一个称为lircmd的LIRC鼠标守护程序运行在lircd的底层。

   5）、irrecord和irsend等工具。




 九）、查看源代码（略）







四、WiFi

 一）、简介

  1、WiFi也称为WLAN（无线局域网），是有线LAN的补充。

   1）、IEEE 802.11a WLAN标准使用5GHZ ISM频段，支持高54Mb/s速率。

   2）、IEEE 802.11b/802.11g WLAN标准使用2.4GHZ ISM频段，分别支持支持11Mb/s和54Mb/s速率。




  2、WLAN类似有线以太网，它们都从同一个地址分配MAC地址，都作为网络接口出现在操作系统中。但WLAN和有线以太网在链路层有显著不同：

   1）、8.2.11 WLAN 标准使用冲突避免（CSMA/CA）机制，而有线以太网使用冲突检测机制。

   2）、与以太网帧不同，WLAN帧需要确认（ACK）。

   3）、由于无线网络环境恶劣，WAN使用称为WEP（有线等效加密）的加密手段，来保证与有线以太网相同的安全性。




  3、WLAN支持两种通信模式

   1）、Ad-hoc模式，此时一组相邻站无需通过接入点就可以直接相互通信。

   2）、有基础设施模式，此时数据交互需经过一个接入点进行。







 二）、配置

  1、无限扩展项目定义了通用Linux API，用于以设备无关的方式配置WLAN设备驱动程序，它也提供一套用于设置和访问来自WLAN驱动程序信息的公用工具。




  2、基于无限扩展项目，主要有三种与WLAN驱动程序通信的方式：

   1）、标准操作使用工具iwconfig。

   2）、特殊操作使用iwpriv。

   3）、WiFi专用统计用/proc/net/wireless。




  3、支持无限扩展：代码清单16-3。







 三）、设备驱动程序

  1、Itersil Prism芯片组、Lucent Hermes芯片、Athros芯片组以及Intel Pro/Wireless都是广泛使用的WLAN控制器。下面是使用这些控制器的设备例子：

   1）、Itersil Prism的WLAN CF卡。

   2）、Cisco Aironet的卡总线适配器。

   3）、许多笔记本中都有Intel Pro/Wireless迷你PCI卡。

   4）、WLAN USB设备。




  2、WLAN驱动程序的任务是让你的卡显得像通常网络接口一样。驱动程序的实现一般分为以下几个部分：

   1）、与Linux网络栈通信的接口。

   2）、结构因素相关的代码。

   3）、芯片组特定的部分。

   4）、对无线扩展的支持。




 四）、查看源代码（略）










五、蜂窝网络

 一）、简介

  1、GSM（全球移动通信系统）是主要的数字蜂窝标准。GSM网络称为2G或第二代网络GPRS代表2G演进到了2.5G。




 二）、GPRS

  1、Linux是如何支持普通GPRS硬件的：

   1）、对一个有内建GPRS支持的系统，模块通过线路连接到微控制器UART通道，传统的Linux串行驱动程序能驱动这个链路。

   2）、对一个PCMCIA/CF GPRS设备，通用串行卡服务器驱动程序能使操作系统的其他部分将卡视为一个串行设备，第一个为使用的串行设备被分配给该卡。

   3）、对于USB GPRS调制解调器，USB-串行端口转换器通常将USB端口转换成一个虚拟穿行端口。




 三）、CDMA（略）







六、当前趋势（略）



