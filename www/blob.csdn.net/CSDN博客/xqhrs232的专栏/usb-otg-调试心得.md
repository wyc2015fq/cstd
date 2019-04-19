# usb-otg-调试心得 - xqhrs232的专栏 - CSDN博客
2016年10月02日 15:51:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2950
原文地址::[http://blog.csdn.net/tianshiyalin/article/details/17249533](http://blog.csdn.net/tianshiyalin/article/details/17249533)
相关文章
1、**android usb otg转换成host调试笔记----[http://www.2cto.com/kf/201411/351662.html](http://www.2cto.com/kf/201411/351662.html)**

仅仅看了半天SPEC写的笔记，有些乱，等调试完毕再次整理，思路不是很成熟，推测成分较多，可能有不对的地方，欢迎拍砖交流指正。
1. 概要
OTG设备使用插头中的ID引脚来区分A/B Device，ID接地被称作为A-Device,为连接时候的USB Host，A-Device始终为总线提供电力，ID悬空被称作为B-Device,为连接时候的USB Device，设备的USB Host/USB Device角色可以通过HNP切换。
OTG设备连接时不能跨越USB Hub，如果跨越USB Hub则失去HNP功能。
这里要注意A-Device/B-Device与USB Host/Device不是一回事没有必然的绑定关系A-Device就一定要是Host，A-Device只是连接时的Host可以通过HNP切换，切换完毕A-Device变成USB Device但是仍然为总线提供电力。
2．设备类型
Embedded Host：提供标准的A插座，普通的USB Host并带有TPL（支持设备列表）
OTG Device：使用Micro AB插座，可以在运行时切换Host/Device。
仅外设B-Device：仅仅能作为外设的B-Device（分为插头一体和插头线缆分离的）。
注意：OTG Device在插头插入后会先打开VBus，如果没有设备连接则关闭VBus，并开启ADP侦测，而Embedded Host则不会再次关闭VBus。
3．协议
SRP（Session Request Protocol）：
B-Device使用。通过数据线上的脉冲，请求A-Device打开VBUS并且开始一个Session。Session为从VBUS打开到关闭这一段时间。
支持：A-Device允许回应SRP，B-Device（包括仅能作为外设的B-Device），允许发起SRP。一个能够支持HNP的B- Device应该能够发起SRP。当A插头插入时关闭VBus的Host必须支持回应SRP，VBus总是打开的Host不必响应SRP。
ADP（Attach Detection Protocol）：
提供设备检测是否有对端设备插入。
支持：任何OTG设备，Embedded Host，支持SRP的B-Device允许ADP Probing，B-Device和仅能作为外设的B-Device还必须支持ADP Senseing如果他们支持ADP Probing。
HNP（Host Negotiation Protocol）：
OTG设备通过HNP来切换Host/Device角色。
当前的USB Host通过HNP Polling（类似Polling Hub）通过轮询GetStatus（）命令返回的数据中的Host request flag查询对端设备是否请求变为Host，Polling间隔为1-2秒。
当当前的USB Host决定允许B-Device转变为Host以后通过SetFeature()打开b_hnp_enable，本次Session结束后Host回到A-Device手里。
4．设备框架
OTG描述符
在设备枚举时，A-Device通过GetDeor向B设备请求OTG描述符。OTG描述符也应当作为GetConfiguration（）的一部分返回。其中的bmAttributes标示B-Device是否支持ADP/HNP/SRP
标准设备特性，通过SetFeature（）设置。
b_hnp_enable
设置此特性，显示B-Device被允许进行HNP，A设备必须在T(HOST_REQ_SUSP)时间内挂起总线。
a_hnp_support
早期OTG版本的兼容特性，设置这个特性指示B-Device对端的A-Device支持HNP。A-Device应当对B-Device设置此特性如果A-Device支持HNP。
a_alt_hnp_support
废弃
GetStatus()
其中数据部分OTG Status最低位为Host Request flag，指示当期的USB Device角色期望变为USB Host角色。
5．一般连接过程（Host -> Device）
OTG Device /Embedded Host 与 仅作为外设的B-device（带A插头型）
Host端检测到A插头插入，停止ADP，打开VBus，因为B-Device的A插头与设备作为一体，此时B-Device必定与A插头连接，Host检测到外设连接，开始枚举。
OTG Device/Embedded Host 与 仅作为外设的B-device（A插头为线缆连接）
Host段检测到A插头插入，停止ADP，打开VBus，如果B-Device是线缆连接完毕在将A插头插入则整个连接过程与上面无异，因为此时B-Device可能还没有插入插头，则设备连接超时，VBus再次关闭，等待下一次ADP的改变（线缆连接完毕），再次打开VBus，此时开始正常总线枚举。
OTG Device 与 OTG Device
Host端检测到插头插入，则打开VBus，如果没有外设检测到，则关闭VBus，打开ADP Probing，Device端检测到插头插入，则打开SRP，如果线缆没有插入，则SRP超时，Device端开始进行ADP Probing，当线缆连接完毕，Device端侦测到ADP变化，发送SRP请求Host打开VBus，Host回应SRP并且打开VBus，完成设备连接。
6．OTG Device（Host/Device）与普通USB Host/Device
OTG Device（Device）插入普通USB Host：
SPEC指示OTG Device满足所有USB外设条件，电器原理实现未查明。
普通USB设备插入OTG Device（Host）：
OTG兼容，但是电器原理实现未查明，对VBus的打开，以及ADP和SRP的处理有疑问。在usb.org上有人问起这个问题，回复只是讲会识别并且兼容，但是实现原理及监测细节未知。
原贴地址：
[https://www.usb.org/phpbb/viewtopic.php?t=14450&highlight=otg](https://www.usb.org/phpbb/viewtopic.php?t=14450&highlight=otg)
最新的一些思考和猜想：
1.ADP是OTG Device作为Host，既可以关闭VBus又能够监测普通USB设备的关键。
2.OTG Device作为Host如果不支持ADP，但是支持SRP的，VBus应当始终打开（或者通过插入中断？RootHub Polling），如果要关闭VBus则仅能支持会发起SRP的OTG Device，普通的USB设备和不支持SRP的OTG Device，在Host没有ADP的情况下无法在关闭VBus的情况下检测到设备（除了插入中断）插入并且通过SRP打开VBus。
注: OTG Device作为Host在A插头插入时会尝试打开VBus，如果插入设备线缆在A插头插入时已经连接完毕，则在这次插入时，正常枚举。如果插入设备线缆在A插头插入时没有连接，作为Host的OTG Device又不支持ADP，但是支持SRP，那么插入设备可以在线缆插入后发起SRP打开VBus，如果插入设备不支持SRP。则设备工作不正常？
3.OTG Device在监测总线活动和设备连接前会在插头插入时首先检测插头（普通USB不会检测插头，因为不关心，OTG的ID引脚会起控制器中断），从而决定OTG Device角色，并且通过ADP等待设备连接，然后打开VBus进行总线活动。
关于SRP与ADP
作为一个B-Device或者是普通的USB Device，怎样打开VBus进行总线枚举是他们的最终目的。
SRP为B-Device请求A-Device打开A设备的手段之一，而ADP虽然作为检测插入设备为目的但是有些EH/OTG Device会在ADP检测到状况以后不等到SRP的到来（或者说不支持SRP）就打开VBus，倘若没有设备连接，再关闭VBus并且打开ADP继续侦测（仅限于OTG Device EH不会），更有甚者检测完A插头直接打开VBus（ADP出来的较晚，很多早期的OTG控制器中，根本没有ADP和HNP Polling的字样，估计那时候可能SPEC里面还没有这些东西）这让SRP显得有些多余，但是我们作为一个B-Device的时候还是应当在开始总线操作前发出SRP，保证不支持ADP只支持SRP并且插头插入时不打开VBus仅仅在受到SRP才打开VBus的控制器上，设备也能进行正常枚举。
#### [Mentor Graphics USB OTG Controller IP Core调试笔记](http://coredump007.spaces.live.com/blog/cns%2137C53E4D9F0C2577%21211.entry)
MUSBHDRC与 MUSBMHDRC是Mentor Graphics出品的兼容支持OTG功能的IP Core被广泛的应用于 TI ADI的多媒体处理器中。两个IP的区别在于MUSBMHDRC带有Multipoint功能，可以接HUB。下面是我在FPGA上调试碰到的问题及解决，欢迎交流拍砖。
MUSBHDRC RTL V1.8版本
BUG及解决
1.中断不会Pending
MUSBHDRC在关闭EP中断的情况下不会去置中断位，这样再次打开中断时，因为中断没有Pending，直接导致本次中断丢失。而 Mentor的做法也相当的令人鄙视，竟然只是在Datasheet中悄悄的把原先版本Datasheet中的中断会Pending的那句话悄悄的去掉。
解决：
只能在SOC级解决，Davinci的解决方法把相关的寄存器Shadow了一下，不允许操作原来MUSBHDRC中的相关中断寄存器而转而使用SOC提供的相关寄存器。
2.DMA部分的BUG
MUSB系列的DMA模式分为3种。DMA MODE 0，DMA MODE 1 REQ MODE 0, DMA MODE 1 REQ MODE 1。
其中DMA MODE 1 REQ MODE 1在MUSBHDRC上无法正常工作，一旦FIFO里有Packet，并且< MAXP 则不会再来中断。
DMA MODE 1 REQ MODE 0在连续的发起DMA请求时候会挂住总线，导致系统锁死，而且因为DMA MODE 1 REQ MODE 0必须知道数据总量的限制，在很多的USB上层的协议中都无法使用。
DMA MODE 0在Device方式时效率低下，不考虑。
解决：
只能在SOC级解决，可以见到Davinci和TUSB都没有使用MUSBHDRC的DMA控制器，都是在SOC上做了一个DMA控制器实现相关DMA操作。
MUSBMHDRC
基本解决了MUSBHDRC中的相关BUG，并且增加了Multipoint功能。可惜Linux驱动中尚没有端点复用及调度相关代码，所以目前仍然不可能支持太多数量的USB设备。
关于ID Pin中断
MUSB系列没有提供ID Pin的中断，而我们从Controller看到的A/B Device逻辑也相当的奇怪。在A-Device状态下，写DEVCTL.SESSION则PHY拉起VBUS，DEVCTL.B-DEVICE和 DEVCTL.VBUS Session才有效，也就是说我们必须开始Session软件才能知道是目前A-Device还是B-Device，这在OTG状态机的A- Device向B-Device状态推进时会出现很大问题：驱动这边会出现许多协议里没有的状态变迁。这样在OTG Device->
 Host 时插头发生变化而软件不知，没人来Drive VBUS，直接导致哑炮熄火（有朋友说我的没有啊，只是有时好用有时不好用，原因后面揭开）。而Host-> Device时，Host设备拔出时停留在A_WAIT_BCON状态，在换上B-Device插头时状态机没有经过A_WAIT_BCON-> A_WAIT_VBUS_FALL-> A_IDLE->B_IDLE的变迁，直接导致驱动程序中状态机状态变迁异常，而Davinci采取了Timer不停地写Session的方法来 Workaround，但是不是解决问题的根本，而我们又不能总是通过Session来检测，最根本的解决方法就是外加一个ID
 Pin中断来检测插头，同时推进状态机。
诡异的Session Request中断
在Datasheet中，这是一条仅仅会在Host方式下来的中断，但是在实际使用中，这条中断来的十分诡异，B-Device状态下有时候会在插入B插头时每隔3-5秒来一次，有时候又不来，有时候没插插头时也回来，飘忽。而驱动中收到Session Request中断则立即Driver VBUS，这解释了前面Device->Host时而好用时而失效的原因。
解决：
走正规的ID PIN检测关闭此中断，在检测到A插头插入时立即打开Session -> Drive VBUS，这样就不用关心对方设备的SRP了。
关于Device方式下SetAddress的注意
按照Datasheet中的流程在收到Setup包的时候即可设置FADDR改变当前的Address，然后清掉RXPKTRDY并且置位 DataEND 回复0包，并且使Controller进入控制传输的Status Stage，完成后来一个MC_INT中断。实际则不然，如果这样操作会导致MUSB Controller失去响应，在协议分析仪上看到Controller在接下来的Get Deor的SETUP Transaction中的IN Token中没有任何回复，或者回复一些NAK后，失去响应。
正确的操作方法为：收到SetAddress以后，不要立即设置FADDR，在Status Stage结束后来的那次中断中设置SETUP包中的传来的地址。
关于DMA MODE 1 REQ MODE 1
Device方式下DMA MODE 1 REQ MODE 1为兼容性和效率最佳方式。然而在Host方式下只能使用DMA MODE 0。归根结底在于DMA MODE 1 REQ MODE 1的结束条件< MAXP 倘若Host连续开启两个IN Transaction，每个IN Transaction大小为MAXP的倍数，则在DMA停止时无法区分两个包，前一次IN Transaction因为结束时还满足DMA MODE 1 REQ MODE 1的条件，直接导致DMA控制器将下一个IN Transaction的包已经收到了FIFO/URB中，如果所有IN
 Transaction 如果都能被MAXP整除，开始的那个URB岂不成一团浆糊了。
解决：控制器设计问题非BUG无法解决，使用DMA MODE 0。
关于Linux中的驱动程序。驱动位于drivers/usb/musb下。
代码中没有端点复用和调度部分，因此Multipoint功能极大受限。Interrupt及Isochronous方式传输似乎有问题，尚未研究。
关于VBUS_ERROR的缺陷。
在一些需要电量较大的设备插入时MUSB在写入Devctl.Session时候VBus拉不起来，并且产生VBUS_ERROR中断，接下来难堪的问题发生了，Controller居然切到B-Device状态，再也不能通过写Session拉起VBus。尚未找到恢复方法。、
转载于：
[http://blog.csdn.net/ljzcom/article/details/8843597](http://blog.csdn.net/ljzcom/article/details/8843597)
