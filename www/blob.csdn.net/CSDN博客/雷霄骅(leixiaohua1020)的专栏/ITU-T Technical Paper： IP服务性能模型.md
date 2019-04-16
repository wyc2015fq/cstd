# ITU-T Technical Paper： IP服务性能模型 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月04日 13:51:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：78
个人分类：[网络](https://blog.csdn.net/leixiaohua1020/article/category/8433201)










本文翻译自ITU-T的Technical Paper：《How to increase QoS/QoE of IP-based platform(s) to regionally agreed standards》(2013/3/1)。这是其第七章的一部分，给出了IP服务性能模型。PS：在此感谢一位师弟的翻译。



基于IP的平台通信服务通常以两种方式进行：垂直和水平方式。通过模型理解这两个不同方式的性能特点对于确定服务性能非常重要。

分层（垂直）模型
 IP服务性能的分层模型很简单，这受益于IP的关键功能，也就是我们所说的“everything over IP”和 “IP over everything”。如图14所示，分层模型包含3个层面：底层、IP层和高层。因此，提供给IP服务用户的性能取决于其他层的性能表现：
*底层:通过“链接”提供支撑IP层的面向连接或无连接的传输。链接在IP包被转发的位置（也就是“路由器”、“源主机”和“目标主机”）被中止，因此不具备端到端的意义。链接可能会涉及到不同类型的技术，例如ATM、帧中继、SDH、PDH、ISDN和租用线路；
*IP层:提供IP的无连接传输，并且对于一对给定的源和目的IP地址具备端到端的意义；

*高层:由IP支持，支持端到端通信，被TCP、UDP、FTP、RTP 和HTTP等所使用。高层可以修改IP层提供的端到端性能，也可能加强它。



![](https://img-blog.csdn.net/20131029160619015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图 14 – IP服务性能的分层模型



通用（水平）模型
 服务性能的通用模型处理的是IP网络的水平配置，并且主要由两部分组成：交换链接和网段。每个性能参数都可用于在一个网段或一组串联的网段上单向转移的IP数据包
 图15展示了一个IP网络组件模型，它的特点如下：
*主机：一个利用IP进行通信的计算机。一个主机可以实现路由功能，也可以实现一些额外的功能，包括高层协议和底层协议；
*路由：一个通过转发IP数据包实现与其它主机之间的通信的主机，转发的目标基于IP目的地址段的内容；
*源主机 (SRC)：一个主机和一个完整的IP地址，端到端的IP数据包从这里生成。通常来说，一个主机可以有多个IP地址，但是，一个源主机只有一个IP地址。源主机也可以生成高层协议（例如TCP）。
*目的主机 (DST)：一个主机和一个完整的IP地址，端到端的IP数据包在这里终结。通常来说，一个主机可以有多个IP地址，但是，一个目的主机只有一个IP地址。目的主机也可以终结高层协议（例如TCP）

*链路：一个用于在一对主机之间传输IP数据包的点对点（物理的或虚拟的）连接。它在IP层之下操作。



![](https://img-blog.csdn.net/20131029160623062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图 15 – IP网络组件



图16展示了在SRC和DST之间与IP服务有关的网络连接。在每个网段（NS）的边缘，网关路由都回通过交换链路发送和接受数据包。每个部件的特点如下：
*交换链路 (EL)：
*1)这一链接可以将一个源主机或目的主机连接到其相邻主机（例如路由器），这一主机可能在另一个管辖区内，这样的链接有时被称为接入链路、入口链路或出口链路；
*2)这一链接还可以将一个网段内的路由与另一网段内的路由连接起来。
*网段 (NS):在SRC和DST之间提供部分IP服务的一组主机以及它们的互联链路，这些主机和链路隶属于单一的（或合作的）管辖责任下。源NS和目的NS属于特殊的网段。网络之间通过交换链路或传输NS连接。为了实现IP性能分配的目的，将会专注于隶属于单一的（或合作的）管辖责任下的主机和链路组（例如一个ISP或一个NSP）。
*源NS （图16中的A）：管辖责任下包含有SRC的NS。在某些情况下，源NS的主机只有SRC一个；目的 NS （G图 16中的G）：管辖责任下包含有DST的NS。在某些情况下，目的NS的主机只有DST一个。

*传输NS（图16中的C）：管辖责任下包含有负责在SRC和DST之间传输流量的主机的NS。



![](https://img-blog.csdn.net/20131029160626656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图16 – IP 网络连接



 7.2 IP 数据包传输性能参数
 在ITU-T 建议Y.1540中确定了用在IP数据包传输中的一组IP数据包信息传输性能参数，现总结如下：
*Packet qualifications: This uses for qualifying the applicability of performance parameters to sets of packets.它有如下两个参数：
*Populations of interest:在端到端的情况下，从SRC发送至DST的数据包总集；
*数据包流: population of interest的最常见的例子。一组与一个给定的、有着相同的源主机地址（SRC）、目的主机地址（DST）、服务等级以及会话识别（例如高层的端口号）的有连接的或无连接的数据流有关的数据包。
*IP 数据包传输延迟 (IPTD): this is the time, (t2 – t1) between the occurrence of two corresponding IP packet reference events, ingress event IPRE1 at time t1 and egress event IPRE2 at time t2, where (t2 > t1) and (t2 – t1) ≤?Tmax. IPTD is defined for all successful and errored packet outcomes across a basic section or an NSE:
*平均IP 数据包传输延迟: IP数据包传输延迟的算术平均数；
*最小IP 数据包传输延迟:所有IP数据包传输延迟（包括所有包共有的传播延迟和排队延迟）中的最小值。
*IP数据包延迟中值: the 50th percentile of the frequency distribution of IP packet transfer delays from a population of interest. The median is the middle value once the transfer delays have been rank-ordered. To obtain this middle value when the population contains an even number of values, then the mean of the two central values is used;IP数据包传输延迟按顺序排列所得的中间值；
*端到端的两点IP数据包延迟变化(PDV):流媒体应用可能利用IP延迟变化的总范围信息来避免缓冲区下溢和溢出。IP延迟的剧烈变化会引起TCP重传输计时器阈值的增长，也可能会引起数据包重发的延迟或不必要的数据包转播。端到端的两点IP PDV的确定基于在入口和出口MP处对到达的IP数据包的观察。
*IP误包率 (IPER):IP误包率是出错的IP数据包数与总IP数据包数之和的比。
*IP丢包率 (IPLR):总共丢失的IP数据包数与传输的IP数据包总数之比。
*伪 IP包率:在出口MP处的伪IP包率即为指定时间间隔内在这里观察到的伪IP包总数除以时间间隔时间所得的值（也就是每秒内的伪IP包数目）。
*IP数据包重排序比(IPRR):被重排序的数据包总数与成功传输的IP数据包总数之比。
*IP数据包严重损失块比率 (IPSLBR):严重损失的IP块与总块数之比；
*IP 数据包复制比率(IPDR):复制的IP包总数与成功传输的IP包数目减去复制的IP包数目后得到的值之比；
*重复的IP数据包比率(RIPR):重复的IP包总数与成功传输的IP包数目减去复制的IP包数目后得到的值之比；
*流修复参数: the probability that a given packet interval (or information block, b) will contain more than x impaired packets;
*IP数据包受损的间隔比 (IPIIR): IP数据包受损的间隔比与不重叠的总间隔之比；
*IP 数据包受损块比率(IPIBR): IP数据包的受损块与不重叠的总间隔之比。
*容量参数: An end-to-end IP packet transfer service traverses an ordered sequence of basic sections from a source host, to a destination host. The capacity 参数s define properties for basic sections in terms of their ability to carry IP traffic, and corresponding properties for network section ensembles (NSE), also referred to as "paths". It is important to note that a basic section as well as a sequence of basic sections is associated with a direction. The direction is significant, as the properties of a sequence of sections in the forward direction need not be the same as in the reverse direction.一个端到端的IP数据包传输服务遍历了从一个源主机到一个目的主机。容量参数确定了基本部分的一些属性，例如负载IP流量的能力以及
*流相关参数: characterize performance in terms of flow or throughput-related 参数s that evaluate the ability of IP networks or sections to carry quantities of IP packets. It should be noted that a 参数 intended to characterize the throughput of an IP application would not be equal to the amount of resources available to that application, because the higher layer protocols over IP (e.g., TCP) also influence the throughput experienced.




