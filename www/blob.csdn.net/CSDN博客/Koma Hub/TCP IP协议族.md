# TCP/IP协议族 - Koma Hub - CSDN博客
2018年07月15日 20:34:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：69
个人分类：[TCP/IP](https://blog.csdn.net/Rong_Toa/article/category/7792859)
## TCP/IP 协议族
虽然`TCP/IP`名称只包含了两个协议,但实际上,`TCP/IP`是一个庞大的协议族,它包括了各个层次上的众多协议,下图列举了各层中一些重要的协议,并给出了各个协议在不同层次中所处的位置如下。
![](https://img-blog.csdn.net/2018071520340582?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- `ARP`:用于获得同一物理网络中的硬件主机地址。
- `MPLS`:多协议标签协议,是很有发展前景的下一代网络协议。
- `IP`:负责在主机和网络之间寻址和路由数据包。
- `ICMP`:用于发送报告有关数据包的传送错误的协议。
- `IGMP`:被 IP 主机用来向本地多路广播路由器报告主机组成员的协议。
- `TCP`:为应用程序提供可靠的通信连接。适合于一次传输大批数据的情况。并适用于要求得到响应的应用程序。
- `UDP`: 提供了无连接通信, 且不对传送包进行可靠的保证。 适合于一次传输少量数据,可靠性则由应用层来负责。
