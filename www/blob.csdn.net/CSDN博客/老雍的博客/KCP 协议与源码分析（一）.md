
# KCP 协议与源码分析（一） - 老雍的博客 - CSDN博客


2018年12月21日 09:22:42[老雍](https://me.csdn.net/yongkai0214)阅读数：644



# 简介
这部分摘自[https://github.com/skywind3000/kcp](https://github.com/skywind3000/kcp)，这是源码的官方网站，有一些使用介绍，其他各种衍生版本，在实际中的使用情况以及一些测试比较分析。
KCP是一个快速可靠协议，能以比 TCP浪费10%-20%的带宽的代价，换取平均延迟降低 30%-40%，且最大延迟降低三倍的传输效果。纯算法实现，并不负责底层协议（如UDP）的收发，需要使用者自己定义下层数据包的发送方式，以 callback的方式提供给 KCP。 连时钟都需要外部传递进来，内部不会有任何一次系统调用。
整个协议只有 ikcp.h，ikcp.c两个源文件，可以方便的集成到用户自己的协议栈中。也许你实现了一个P2P，或者某个基于 UDP的协议，而缺乏一套完善的ARQ可靠协议实现，那么简单的拷贝这两个文件到现有项目中，稍微编写两行代码，即可使用。
# 技术特性
TCP是为流量设计的（每秒内可以传输多少KB的数据），讲究的是充分利用带宽。而 KCP是为流速设计的（单个数据包从一端发送到一端需要多少时间），以10%-20%带宽浪费的代价换取了比 TCP快30%-40%的传输速度。TCP信道是一条流速很慢，但每秒流量很大的大运河，而KCP是水流湍急的小激流。KCP有正常模式和快速模式两种，通过以下策略达到提高流速的结果：
RTO翻倍vs不翻倍：
>TCP超时计算是RTOx2，这样连续丢三次包就变成RTOx8了，十分恐怖，而KCP启动快速模式后不x2，只是x1.5（实验证明1.5这个值相对比较好），提高了传输速度
选择性重传 vs 全部重传：
TCP丢包时会全部重传从丢的那个包开始以后的数据，KCP是选择性重传，只重传真正丢失的数据包。（TCP同样有选择重传SACK，但有区别，后续文章再介绍）。
快速重传：
发送端发送了1，2，3，4，5几个包，然后收到远端的ACK: 1，3，4，5，当收到ACK3时，KCP知道2被跳过1次，收到ACK4时，知道2被跳过了2次，此时可以认为2号丢失，不用等超时，直接重传2号包，大大改善了丢包时的传输速度。
延迟ACK vs 非延迟ACK：
TCP为了充分利用带宽，延迟发送ACK（NODELAY都没用），这样超时计算会算出较大 RTT时间，延长了丢包时的判断过程。KCP的ACK是否延迟发送可以调节。
UNA vs ACK+UNA：
ARQ模型响应有两种，UNA（此编号前所有包已收到，如TCP）和ACK（该编号包已收到），光用UNA将导致全部重传，光用ACK则丢失成本太高，以往协议都是二选其一，而 KCP协议中，除去单独的 ACK包外，所有包都有UNA信息。
非退让流控：
KCP正常模式同TCP一样使用公平退让法则，即发送窗口大小由：发送缓存大小、接收端剩余接收缓存大小、丢包退让及慢启动这四要素决定。但传送及时性要求很高的小数据时，可选择通过配置跳过后两步，仅用前两项来控制发送频率。以牺牲部分公平性及带宽利用率之代价，换取了开着BT都能流畅传输的效果
# 设计思想
KCP协议是一个纯粹的ARQ协议，通过重传机制实现UDP数据包的可靠传输，在整个网络结构中的位置如下图所示。
![](https://img-blog.csdnimg.cn/20181221090318177.png)图 1 KCP 在网络体系中位置
单纯的ARQ在实际使用中并不能满足所有的网络场景，特别是网络拥塞时，大量的重传会导致更多的丢包。增加FEC是一个明智的选择，在KCP协议中，也并不排斥在KCP上增加FEC。但是需要注意的是，FEC加重传可能导致数据包的时延与抖动，同时，如果FEC解码得到的包经由重传或者网络延迟到达，需要在应用层进行检测，避免大量重复包影响KCP的传输效率。
![](https://img-blog.csdnimg.cn/20181221090526734.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lvbmdrYWkwMjE0,size_16,color_FFFFFF,t_70)图 2 KCP 协议源码结构
KCP通过ikcp_create 创建一个KCP对象，每个不同的会话将产生不同的对象。因为KCP协议本身并没有提供网络部分的代码，所以需要将UDP发送函数的回调设置到KCP中，在有需要时，调用回调函数即可。KCP也支持外部的内存分配与日志回调，为用户提供了非常充分的自由度。
整个KCP协议主要依靠一个循环ikcp_update来驱动整个算法的运转，所有的数据发送，接收，状态变化都依赖于此，所以如果有操作占用每一次update的周期过长，或者设置内部刷新的时间间隔过大，都会导致整个算法的效率降低。在ikcp_update中最终调用的是ikcp_flush，这是协议中的一个核心函数，将数据，确认包，以及窗口探测和应答发送到对端。
KCP使用ikcp_send发送数据，该函数调用ikcp_output发送数据，实际上最终调用事先注册的发送回调发送数据。KCP通过ikcp_recv将数据接收出来，如果被分片发送，将在此自动重组，数据将与发送前保持一致。
# 数据结构
## KCP包与IKCPSEG
![](https://img-blog.csdnimg.cn/20181221090922270.png)图3 KCP 报文结构
KCP中只有一种数据包格式，不管是数据还是信令，都使用相同的结构与头。KCP头中每一个字段的意义如下：
Conv，32bit，4Byte
为一个表示会话编号的整数，和TCP的 conv一样，通信双方需保证 conv相同，相互的数据包才能够被接受。conv唯一标识一个会话，但通信双方可以同时存在多个会话。
cmd，8bit，1Byte
用来区分分片的作用。IKCP_CMD_PUSH：数据分片；IKCP_CMD_ACK：ack分片； IKCP_CMD_WASK：请求告知窗口大小；IKCP_CMD_WINS：告知窗口大小。
frag，8bit，1Byte
用户数据可能会被分成多个KCP包发送，frag标识segment分片ID（在message中的索引，由大到小，0表示最后一个分片）。
wnd，16bit，2Byte
剩余接收窗口大小（接收窗口大小-接收队列大小），发送方的发送窗口不能超过接收方给出的数值。
ts，32bit，4Byte
message发送时刻的时间戳
sn，32bit，4Byte
message分片segment的序号，按1累次递增。
una，32bit，4Byte
待接收消息序号(接收滑动窗口左端)。对于未丢包的网络来说，una是下一个可接收的序号，如收到sn=10的包，una为11。
len，32bit，4Byte
数据长度。
在结构体IKCPSEG中，除了上述的包结构的字段外，还定义了几个非常重要的变量：
resendts
下次超时重传的时间戳。
rto
该分片的超时重传等待时间，其计算方法同TCP。
fastack
收到ack时计算的该分片被跳过的累计次数，此字段用于快速重传，自定义需要几次确认开始快速重传。
xmit
发送分片的次数，每发送一次加一。发送的次数对RTO的计算有影响，但是比TCP来说，影响会小一些，计算思想类似
## IKCPCB结构
IKCPCB是KCP中最重要的结构，也是在会话开始就创建的对象，代表着这次会话，所以这个结构体体现了一个会话所需要涉及到的所有组件。其中一些参数在IKCPSEG中已经描述，不再多说。
conv：标识这个会话；
mtu：最大传输单元，默认数据为1400，最小为50；
mss：最大分片大小，不大于mtu；
state：连接状态（0xFFFFFFFF表示断开连接）；
snd_una：第一个未确认的包；
snd_nxt：下一个待分配的包的序号；
rcv_nxt：待接收消息序号。为了保证包的顺序，接收方会维护一个接收窗口，接收窗口有一个起始序号rcv_nxt（待接收消息序号）以及尾序号 rcv_nxt + rcv_wnd（接收窗口大小）；
ssthresh：拥塞窗口阈值，以包为单位（TCP以字节为单位）；
rx_rttval：RTT的变化量，代表连接的抖动情况；
rx_srtt：smoothed round trip time，平滑后的RTT；
rx_rto：由ACK接收延迟计算出来的重传超时时间；
rx_minrto：最小重传超时时间；
snd_wnd：发送窗口大小；
rcv_wnd：接收窗口大小；
rmt_wnd：远端接收窗口大小；
cwnd：拥塞窗口大小；
probe：探查变量，IKCP_ASK_TELL表示告知远端窗口大小。IKCP_ASK_SEND表示请求远端告知窗口大小；
interval：内部flush刷新间隔，对系统循环效率有非常重要影响；
ts_flush：下次flush刷新时间戳；
xmit：发送segment的次数，当segment的xmit增加时，xmit增加（第一次或重传除外）；
rcv_buf：接收消息的缓存；
nrcv_buf：接收缓存中消息数量；
snd_buf：发送消息的缓存；
nsnd_buf：发送缓存中消息数量；
rcv_queue：接收消息的队列
nrcv_que：接收队列中消息数量；
snd_queue：发送消息的队列；
nsnd_que：发送队列中消息数量；
nodelay：是否启动无延迟模式。无延迟模式rtomin将设置为0，拥塞控制不启动；
updated：是否调用过update函数的标识；
ts_probe：下次探查窗口的时间戳；
probe_wait：探查窗口需要等待的时间；
dead_link：最大重传次数，被认为连接中断；
incr：可发送的最大数据量；
acklist：待发送的ack列表；
ackcount：acklist中ack的数量，每个ack在acklist中存储ts，sn两个量；
ackblock：2的倍数，标识acklist最大可容纳的ack数量；
user：指针，可以任意放置代表用户的数据，也可以设置程序中需要传递的变量；
buffer：存储消息字节流；
fastresend：触发快速重传的重复ACK个数；
nocwnd：取消拥塞控制；
stream：是否采用流传输模式；
logmask：日志的类型，如IKCP_LOG_IN_DATA，方便调试；
output udp：发送消息的回调函数；
writelog：写日志的回调函数。
# 参考
[https://wetest.qq.com/lab/view/391.html?=content_qcloud](https://wetest.qq.com/lab/view/391.html?=content_qcloud)
[https://blog.csdn.net/qq_36748278/article/details/80171575](https://blog.csdn.net/qq_36748278/article/details/80171575)
[https://blog.csdn.net/sai_j/article/details/82598692](https://blog.csdn.net/sai_j/article/details/82598692)
[http://kaiyuan.me/2017/07/29/KCP%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/](http://kaiyuan.me/2017/07/29/KCP%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90/)
[https://www.cnblogs.com/yuanyifei1/p/6837453.html](https://www.cnblogs.com/yuanyifei1/p/6837453.html)


