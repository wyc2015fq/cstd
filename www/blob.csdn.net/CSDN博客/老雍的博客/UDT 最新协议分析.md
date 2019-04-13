
# UDT 最新协议分析 - 老雍的博客 - CSDN博客


2019年01月28日 22:32:53[老雍](https://me.csdn.net/yongkai0214)阅读数：199



### UDT4 最新协议分析
[背景](#_1)
[协议](#_8)
[与IETF草案版本差异](#IETF_9)
[简介](#_12)
[数据结构](#_20)
[数据包](#_21)
[控制包](#_26)
[定时器](#_108)
[两种连接模式](#_119)
[数据发送与接收](#_126)
[发送端算法](#_129)
[发送端数据结构](#_130)
[数据发送算法](#_133)
[接收端算法](#_140)
[接收端数据结构](#_141)
[数据接收算法](#_155)
[流控](#_225)
[丢包信息压缩](#_228)
[可配置的拥塞控制](#_233)
[CCC接口](#CCC_238)
[原生控制算法](#_261)
[当ACK包被接收](#ACK_267)
[当NAK包被接收](#NAK_285)
[效率与安全](#_295)

# 背景
网络带宽占用与实际物理管道通信能力之间的矛盾愈加突出，TCP越来越不能满足需求，传输效率提升需求很迫切。如果对这个领域细分的话，其实每个细分领域的需求各不一样，比如分布式计算，实时通信，直播，在线游戏等。有些对实时性要求更高，也有些对数据完整性要求更高，当然也有两者兼而有之。
![TCP传输效率](https://img-blog.csdnimg.cn/2018122815142165.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lvbmdrYWkwMjE0,size_16,color_FFFFFF,t_70#pic_center)
为什么会出现网络传输效率降低呢？这有网络拥塞的原因，也有传输协议的问题。从网络拥塞上来说，最根本还是中间网关或管道的限制。当大量数据涌入一个流量限制通路，路由器排队丢失导致丢包；如果路由器性能不足，或者出现故障，也会出现大量的数据丢失。如果采用TCP传输，会导致数据重传，以及传输窗口降低影响效率，也存在重新选路的可能。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228151603836.png#pic_center)
UDT（UDP-based Data Transfer Protocol）主要针对当前TCP进行长距离传输大量数据时的性能表现较差而提出，建立在UDP之上，引入新的拥塞控制以及可靠性，支持可靠的流式传输（类似TCP），以及部分可靠的数据报传输（增强UDP）。除了高速数据传输之外的其它应用领域，例如点到点技术（P2P），防火墙穿透，多媒体数据传输等等。
[
](https://img-blog.csdnimg.cn/20181228151603836.png#pic_center)
# 协议
[
](https://img-blog.csdnimg.cn/20181228151603836.png#pic_center)
## 与IETF草案版本差异
[
](https://img-blog.csdnimg.cn/20181228151603836.png#pic_center)协议内容以代码UDT4中包含的draft-gg-udt-xx为准，更新时间为2013年。在IETF中的协议版本为draft-gg-udt-03，更新时间为2010年。
![协议版本差异](https://img-blog.csdnimg.cn/20181228163031293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lvbmdrYWkwMjE0,size_16,color_FFFFFF,t_70#pic_center)
## 简介
UDT是面向连接的双工协议，每个UDT实体有两个部分：发送和接收。发送者根据流量控制和速率控制来发送（和重传）应用程序数据。接收者接收数据包和控制包，并根据接收到的包发送控制包。发送和接收程序共享同一个UDP端口来发送和接收。
接收者也负责触发和处理所有的控制事件，包括拥塞控制和可靠性控制和他们的相对机制，例如RTT估计、带宽估计、应答和重传。所以整个系统依靠接收者对网络的统计与计算进行调节。
因为使用UDP传输，所以可能需要对应用层数据进行分包与重组，这样就决定了需要有字节包头格式。流式传输时需要填满上一分包，这一点与TCP类似。从传输效率来看，使用流式传输效率更高，尤其针对大量的数据或者大块数据。
UDT在传输效率优化时，同样希望可以兼顾到公平，所以也有自己的拥塞控制算法和流控措施。速率控制调整包的发送周期（RTT估计），拥塞窗口限制传输到网络的数据量（接收方数据到达速度，接收缓冲窗口大小）。
## 数据结构
### 数据包
第一个bit为0，标识数据包。
FF标识一个消息中各个包的位置，11：单独的数据包，10：一个数据流中的第一个数据包，01：一个数据流中的最后一个数据包。紧随其后的bit位标记是否立即发送数据。
Destination Socket ID是在端口复用时用来区分哪一个连接的数据。因为UDT支持多个连接使用同一个端口。
![UDT 数据包格式](https://img-blog.csdnimg.cn/20181228153351451.png#pic_center)
### 控制包
第1 bit为1，标识控制包。第2-16bit标识控制报的类型，UDT定义了8种数据类型，其余暂时保留，根据不同的类型，Additional Info与control Information内容也会不同。
![UDT 控制包格式](https://img-blog.csdnimg.cn/20181228153442574.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lvbmdrYWkwMjE0,size_16,color_FFFFFF,t_70#pic_center)
TYPE 0x0:  Protocol Connection Handshake
Additional Info:
> Undefined
Control Info:
> 32 bits: UDT version

> 32 bits: Socket Type (STREAM or DGRAM) 32 bits: Socket Type (STREAM or DGRAM)

> 32 bits: initial packet sequence number 32 bits: initial packet sequence number

> 32 bits: maximum packet size (including UDP/IP headers) 32 bits: maximum packet size (including UDP/IP headers)

> 32 bits: maximum flow window size  32 bits: maximum flow window size

> 32 bits: connection type (regular or rendezvous)  32 bits: connection type (regular or rendezvous)

> 32 bits: socket ID 32 bits: socket ID

> 32 bits: SYN cookie 32 bits: SYN cookie

> 128 bits: the IP address of the peer’s UDP socket128 bits: the IP address of the peer’s UDP socket
TYPE 0x1:  Keep-alive
Additional Info:
> Undefined
Control Info:
> None
TYPE 0x2:  Acknowledgement (ACK)
Additional Info:
> ACK sequence number
Control Info:
> 32 bits: The packet sequence number to which all the previous packets have been received (excluding)

> [The following fields are optional]

> 32 bits: RTT (in microseconds)

> 32 bits: RTT variance

> 32 bits: Available buffer size (in bytes)

> 32 bits: Packets receiving rate (in number of packets per second)

> 32 bits: Estimated link capacity (in number of packets per second)

> TYPE 0x3:  Negative Acknowledgement (NAK)

> Additional Info:

> Undefined

> Control Info:

> 32 bits integer array of compressed loss information

> TYPE 0x4:  Congestion/Delay Warning

> Additional Info：

> Undefined

> Control Info：

> None

> TYPE 0x5:  Shutdown

> Additional Info:

> Undefined

> Control Info:

> None

> TYPE 0x6:  Acknowledgement of Acknowledgement (ACK2)

> Additional Info:

> ACK sequence number

> Control Info:

> None

> TYPE 0x7:  Message Drop Request:

> Additional Info:

> Message ID

> Control Info:

> 32 bits: First sequence number in the message

> 32 bits: Last sequence number in the message

> 定时器

> 协议定义了四个定时器ACK，NAK，EXP和SND。SND在发送端，其余在接收端。不同定时器触发不同的周期事件，包括速率控制、应答、丢失报告（negative应答）和重传/连接维护。定时器使用系统时间，并主动查询是否到期。

> SND定时器用来触发周期性的速率控制。

> ACK定时器周期被CC控制，周期性的有选择的发送ACK包，检查周期不超过一个SYN时间 0.01秒。

> NAK被用来触发发送NAK包。重传定时器被用来触发一个数据包的重传和维护连接状态。他们周期依赖于对于RTT的估计，周期动态更新为4 * RTT + RTTVar + SYN，RTTVar为RTT样本方差。

> EXP被用于触发丢包重传和维护连接状态。周期动态更新为 N * (4 * RTT + RTTVar + SYN)， N 为连续超时的次数，且周期最小值被设定，默认为0.5秒，以防止太频繁被触发。

> 两种连接模式

> C/S模式：客户端服务器模式，传统模式。由客户端向服务器端发起连接请求。而服务器端需要先建立监听，以便接受连接请求。

> Rendezvous模式：会和模式，通信双方同时向对方发送连接请求。这种情况，在NAT穿越后较多用到。不能接受C/S模式下的连接请求。

> 对于已经建立UDT连接的双方来说，发送方发送一个关闭请求，且只发送一次。如果接受没有接收到，需要等到16*EXP后再自行关闭。总超时有最小最大阈值，参考值设置为3秒到30秒。

> 数据发送与接收

> 发送端根据拥塞控制和流量控制，决定如何发送数据包或者重传可能丢失的数据包。接收端负责触发控制事件NAK、ACK和EXP，处理所有相关控制机制。

> 发送端算法

> 发送端数据结构

> 发送丢失链表（Sender’s Loss List）

> 记录从接收端返回的NAK包记录的丢包信息以及触发超时事件时的包序号（发生超时事件时会插入链表）。

> 数据发送算法

> 如果发送端丢失链表非空，重传发送丢失链表第一个序号的包，并删除该节点。 Goto 5。

> 在消息模式下，如果包已经在发送丢失链表中滞留了超过应用设定的生存时间TTL值，发送消息丢弃请求，并移除发送丢失链表中所有关联包。Goto 1。

> 等待，直到有应用数据被发送。

> 如果未确认的数据包数量已经超过流量/拥塞窗口，将停下来等待ACK到来。Goto 1。否则，将打包一个新的数据包并发送出去。

> 如果当前包的序号为 16 n（可能超时），其中n是一个整数，Goto 2。

> 等待（SND - t），Goto 1。其中SND为由CC更新的包间时间间隔，t是从1)到5）的总时间。

> 接收端算法

> 接收端数据结构

> 接收端丢失链表

> 包括一个二元组和一个参数。二元组为检测到丢包的序号，和最新反馈的时间。参数k是每一个包被NAK反馈的次数。丢失链表中序号升序排列。

> ACK 历史窗口

> 记录每个发送的ACK，以及发送时间。循环数组。

> PKT 历史窗口

> 记录每个数据包的到达时间。循环数组。

> 包对窗口

> 记录每个探测包队的间隔时间。循环数组。

> LRSN

> 记录接收到的数据包最大序号，初始化为1。

> ExpCount

> 记录连续发生EXP超时时间的次数。

> 数据接收算法

> 数据接收算法

> 查询系统时间，检查ACK，NAK，EXP定时器是否已经过期。如果存在任意一个定时器过期，都需要处理该定时器到期的对应事件，并重置关联时间变量。对于ACK，也会检查ACK 包间隔。

> 开始存在时间限制的UDP接收过程。如果没有包到达，Goto 1。

> 重置ExpCount 为1。如果没有未确认数据，或者收到ACK，NAK控制包，重置EXP 定时器。

> 检查包头的标志位，如果是控制包，按照类型处理，并 Goto 1。

> 如果当前包的序号是16n + 1，记录这个包和在包对窗口记录的最近一个数据包的时间间隔。其中n是一个整数。

> 在PKT历史窗口中记录包到达时间。

> 如果当前数据包的序号sn大于 LRSN + 1，将所有介于（LRSN+1, sn）区间的所有报序号加入接收丢失链表。

> 如果当前数据包的序号小于LRSN，从接收丢失链表中删除。

> 更新LRSN，Goto 1。

> ACK定时器事件处理过程

> 接收端根据如下规则查找接收到所有包之前的序号（最后确认的最大ACK序号，即为ACK number）：

> 如果接收丢失链表非空，ACK 序号为 LRSN + 1；否则，ACK 序号为接收丢失链表中最小序号。

> 如果ACK number等于被ACK2 确认的最大ACK序号，或者等于不超过2 RTT的上一个ACK的最大序号，停止处理（不发送这个ACK）。

> 每个ACK序号为唯一单调递增的，将RTT，RTTVar，流窗口大小（可用接收缓冲大小）填充到ACK包中。如果这个ACK不是被ACK定时器触发的，发送ACK并停止处理。

> 包到达速度计算：

> 计算存储在PKT历史窗口中最近16个包到达时间的中值AI。去掉大于 AI * 8或者AI/8的值。如果剩余超过8个，计算均值 AI’和包到达速率 1/AI’（每秒包到达个数），否则，小于8个则返回0。

> 链路容量估计：

> 计算包对窗口中最近的16个包对时间间隔的中值PI，链路容量值为1/PI（每秒包数量）。

> 将包到达菽粟和链路容量估计值打包到ACK包中发送。

> 记录当前ACK序号，ACK确认前的最大ACK序号以及ACK发送时间将被记录在ACK历史窗口中。

> ACK定时器事件处理过程

> 接收端根据如下规则查找接收到所有包之前的序号（最后确认的最大ACK序号，即为ACK number）：

> 如果接收丢失链表非空，ACK 序号为 LRSN + 1；否则，ACK 序号为接收丢失链表中最小序号。

> 如果ACK number等于被ACK2 确认的最大ACK序号，或者等于不超过2 RTT的上一个ACK的最大序号，停止处理（不发送这个ACK）。

> 每个ACK序号为唯一单调递增的，将RTT，RTTVar，流窗口大小（可用接收缓冲大小）填充到ACK包中。如果这个ACK不是被ACK定时器触发的，发送ACK并停止处理。

> 包到达速度计算：

> 计算存储在PKT历史窗口中最近16个包到达时间的中值AI。去掉大于 AI * 8或者AI/8的值。如果剩余超过8个，计算均值 AI’和包到达速率 1/AI’（每秒包到达个数），否则，小于8个则返回0。

> 链路容量估计：

> 计算包对窗口中最近的16个包对时间间隔的中值PI，链路容量值为1/PI（每秒包数量）。

> 将包到达菽粟和链路容量估计值打包到ACK包中发送。

> 记录当前ACK序号，ACK确认前的最大ACK序号以及ACK发送时间将被记录在ACK历史窗口中。

> NAK事件处理过程

> 搜索接收丢失链表，寻找最后的反馈时间在 k*RTT的所有包序号，k初始化为2，并且每次反馈增加1。通过NAK将所有包反馈给发送端。

> EXP事件处理过程

> 将所有未确认的包添加到发送丢失链表中。

> 如果ExpCount > 16，并且ExpCount被重置为1至少过去3秒，或者3分钟过去了，关闭UDT连接并退出。

> 如果发送丢失链表为空，发送keep-alive包到对端。

> ExpCount自增1.

> 接收到ACK 控制包的处理过程

> 更新最大确认序号。

> 使用相同的ACK序号返回一个ACK2作为确认的确认。

> 更新RTT与RTTVar。

> 更新ACK和NAK周期为 4 * RTT + RTTVar + SYN。

> 更新流窗口大小。

> 如果是一个轻量级ACK，终止处理。（light ACK）

> 更新包到达速率为：A = (A * 7 + a) / 8，其中a为ACK中携带的相应值。

> 更新链路容量估计值：B = (B * 7 + b) / 8，其中b为ACK中携带的相应值。

> 更新发送端缓冲，释放已经被确认的缓冲。

> 更新发送丢失链表，移除已经被确认的所有包序号。

> 接收到NAK 控制包的处理过程

> 将NAK中携带的所有序号添加到发送丢失链表中。

> 通过码率控制（rate control）更新SND周期。

> 重置EXP时间变量。

> 接收到ACK2控制包的处理过程

> 根据ACK2中的ACK序号，在ACK历史窗口中找到关联的ACK。

> 更新被确认的最大ACK序号 。

> 根据ACK2到达时间和ACK离开时间，计算新的RTT：RTT = (RTT * 7 + rtt) / 8。

> 更新RTTVar：RTTVar = (RTTVar * 3 + abs(RTT - rtt)) / 4。

> 更新ACK 与 NAK周期为： 4 * RTT + RTTVar + SYN。

> 接收到消息丢失请求的处理过程

> 在接收缓冲中标记所有属于同一个消息的包，使得不再可读。

> 在接收丢失链表中移除所有对应的包。

> 流控

> 流量控制窗口初始值设置为16。

> 接收到ACK消息时，流窗口大小更新为接收端当前的可用缓冲大小。

> 丢包信息压缩

> NAK包中携带的丢失信息是一个32-bit整数的数组。如果数组的第一位为0，表示这个序号的包丢失，如果第1位是1，意味着从这个号码开始（包括该号码）到下一个数组中的元素（包括这个元素值）之间的包（它的第1位必须是0）都丢失。例如，下面的NAK中携带的信息：

> 0x00000002, 0x80000006, 0x0000000B, 0x0000000E

> 上面的信息表明序号为：2，6，7，8，9，10，11，14的包都丢了

> 可配置的拥塞控制

> UDT中的拥塞控制使用一种开放式的框架，用户可以较容易的实现自定义的算法，或者在多个算法之间切换。在UDT的控制算法框架中，已经实现了原生的控制算法。

> 用户定义的算法可能重定义许多控制流程以及适配一些UDT参数。这些流程将在某一时间出现时被调用，例如，当ACK接收到时，控制算法可能增加拥塞窗口大小。

> CCC接口

> UDT允许用户接入两个拥塞控制参数：拥塞窗口大小和包间发送时间间隔。用户可以通过调整参数适配这两个参数来实现基于窗口的控制，基于速率的控制或者混合方法。另外，下列参数也将被公开：

> 往返时延（RTT）。

> 最大包大小（Maximum Segment/Packet Size）。

> 估计带宽。

> 目前为止被发送的最新的包序号。

> 接收端的包到达速率。

> UDT实现同样可以公开一些其他的参数，这些信息被在用户自定义的拥塞控制算法中使用，以挑战包发送速率。

> 下列的控制事件能通过CCC进行重定义（比如通过回调的方式）：

> Init：当UDTsocket被连接时。

> close：当UDT socket被关闭时。

> onACK：当接收到ACK时。

> onLOSS：当接收到NACK时。

> onTimeout：发生一个超时时。

> onPktSent：当一个数据包被发送时；

> onPktRecv：当一个数据包到达时。

> 用户还可以在自定义算法中调整下列参数：

> ACK interval：ACK可能每间隔固定数量包进行发送。用户可以自定义时间间隔。如果值为-1，意味着在包到达时没有ACK被发送。

> ACK Timer：ACK也可以以固定的时间间隔进行发送，在UDT中是强制的。最大和缺省ACK时间间隔为SYN。

> RTO：UDT用 4 * RTT + RTTVar 来估计RTO，用户可以重定义。

> 原生控制算法

> 如果用户没有自定义控制算法，那么UDT缺省使用原生的控制算法，并被CCC执行。

> UDT原生算法是一种混合型的拥塞控制算法，因此需要调整拥塞窗口大小和包间间隔。原生算法使用基于定时器的ACK，ACK之间的时间间隔为SYN。

> 出视拥塞控制窗口大小为16个包，并且包间间隔为0，算法以慢启动开始，直到第一个ACK或NAK到来。

> 当ACK包被接收

> 如果当前处于慢启动阶段，设置拥塞窗口大小为 包到达速率与（RTT+SYN）的乘积，慢启动结束。停止处理。

> 设置拥塞窗口大小：CWND = A * (RTT + SYN) + 16。

> 下一个SYN周期内需要发送的包数量增加值为：

> if (B <= C)	inc = min_inc

> else			inc = max(10^(ceil( log10( (B-C) * PS * 8 ) )) * Beta/PS, min_inc)

> 其中，B 为链路容量估计，C为当前发送速率估计，单位 包数/秒。

> Beta 是常数 0.0000015，min_inc 为最小增加值，0.01，表示每秒至少增加一个包(SYN)。

> SND 周期更新为：

> SND = (SND * SYN) / (SND * inc + SYN)

> 在码率降低时有四个参数将被使用，初始值为圆括号内值：

> AvgNAKNum (1)：一个拥塞周期内的平均NAK包数。

> NAKCount (1)：当前周期内的NAK目前的数量。

> DecCount(1)：一个拥塞周期的平均NAK数量。

> LastDecSeq（初始序号-1）：上次包发送速率降低时的最大包序号。

> 一个拥塞周期被定义为介于 2个NAK包之间，并且第一个最大的丢包序号大于LastDecSeq。

> 当NAK包被接收

> 如果处于慢启动阶段，慢启动结束。

> 如果被观察到接收速率被从包间间隔设置为 1/recvrate，停止。

> 否则，根据当前窗口大小（cwnd / rtt + SYN）设置发送速率，继续以步骤2中方法减少窗口。

> 如果这个NAK开始一个新的拥塞周期，增加包间时间间隔snd: snd = snd * 1.125。

> 更新AvgNAKNum，重置NAKCount为1，计算DecRandom为1到AvgNAKNum之间的一个平均分布的随机数，更新LastDecSeq。停止。

> 如果DecCount <= 5，并且NAKCount == DecCount * DecRandom：

> a.	更新SND period: SND = SND * 1.125；

> b.	DecCount以增加值1进行增加；

> c.	记录当前最大发送序号LastDecSeq。

> 效率与安全

> UDT原生控制算法被设计给海量数据在高带宽时延积网络传输数据的。这也就解释了为什么在日常使用中有些应用批判UDT效果不佳的问题，尤其在无线网络。

> 所以永远不要期望存在一种算法适合所有的网络状态，只能与具体的业务进行分析，才能优化出更好的效果。与其抱怨算法的太差，不如回头梳理自己的需求是否一致以及如何修改或者创新。

> UDT安全机制类似与TCP。


