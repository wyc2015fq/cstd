
# setsockopt()用法（参数详细说明） - 高科的专栏 - CSDN博客

2013年12月10日 11:34:18[高科](https://me.csdn.net/pbymw8iwm)阅读数：12398


先来看看函数的原型：
int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
然后我们来看看参数：
**s(套接字):**指向一个打开的套接口描述字
**level:(级别)：**指定选项代码的类型。
SOL_SOCKET: 基本套接口
IPPROTO_IP: IPv4套接口
IPPROTO_IPV6: IPv6套接口
IPPROTO_TCP: TCP套接口
**optname(选项名)：**选项名称
**optval(选项值):**是一个指向变量的指针 类型：整形，套接口结构， 其他结构类型:linger{}, timeval{ }
**optlen(选项长度) ：**optval 的大小

**SOL_SOCKET**
closesocket（一般不会立即关闭而经历TIME_WAIT的过程）后想继续重用该socket：
BOOL bReuseaddr=TRUE;
setsockopt(s,SOL_SOCKET ,SO_REUSEADDR,(const char*)&bReuseaddr,sizeof(BOOL));
**SO_KEEPALIVE**保持连接 int
检 测对方主机是否崩溃，避免（服务器）永远阻塞于TCP连接的输入。 设置该选项后，如果2小时内在此套接口的任一方向都没有数据交换，TCP就自动给对方 发一个保持存活探测分节(keepalive probe)。这是一个对方必须响应的TCP分节.它会导致以下三种情况： 对方接收一切正常：以期望的 ACK响应。2小时后，TCP将发出另一个探测分节。
 对方已崩溃且已重新启动：以RST响应。套接口的待处理错误被置为ECONNRESET，套接 口本身则被关闭。 对方无任何响应：源自berkeley的TCP发送另外8个探测分节，相隔75秒一个，试图得到 一个响应。在发出第一个探测分节11分钟15秒后若仍无响应就放弃。套接口的待处理错 误被置为ETIMEOUT，套接口本身则被关闭。如ICMP错误是“host unreachable (主机不 可达)”，说明对方主机并没有崩溃，但是不可达，这种情况下待处理错误被置为 EHOSTUNREACH。
**SO_RCVBUF**接收缓冲区大小 int
设置接收缓冲区的保留大小
与 SO_MAX_MSG_SIZE 或TCP滑动窗口无关，如果一般发送的包很大很频繁，那么使用这个选项
**SO_SNDBUF**发送缓冲区大小 int
设置发送缓冲区的保留大小
与 SO_MAX_MSG_SIZE 或TCP滑动窗口无关，如果一般发送的包很大很频繁，那么使用这个选项
每 个套接口都有一个发送缓冲区和一个接收缓冲区。 接收缓冲区被TCP和UDP用来将接收到的数据一直保存到由应用进程来读。 TCP：TCP通告另一端的窗口大小。 TCP套接口接收缓冲区不可能溢出，因为对方不允许发出超过所通告窗口大小的数据。 这就是TCP的流量控制，如果对方无视窗口大小而发出了超过宙口大小的数据，则接 收方TCP将丢弃它。 UDP：当接收到的数据报装不进套接口接收缓冲区时，此数据报就被丢弃。UDP是没有 流量控制的；快的发送者可以很容易地就淹没慢的接收者，导致接收方的UDP丢弃数据报。
在send()的时候，返回的是实际发送出去的字节(同步)或发送到socket缓冲区的字节(异步);系统默认的状态发送和接收一次为8688字节(约为8.5K)；在实际的过程中发送数据和接收数据量比较大，可以设置socket缓冲区，而避免了send(),recv()不断的循环收发：
// 接收缓冲区
int nRecvBuf=32*1024;//设置为32K
setsockopt(s,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
//发送缓冲区
int nSendBuf=32*1024;//设置为32K
setsockopt(s,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
这里再简单说一下缓冲区吧：应用程序可通过调用send(write,
 sendmsg等)利用tcp socket向网络发送应用数据，而tcp/ip协议栈再通过网络设备接口把已经组织成struct sk_buff的应用数据(tcp数据报)真正发送到网络上，由于应用程序调用send的速度跟网络介质发送数据的速度存在差异，所以，一部分应用数据被组织成tcp数据报之后，会缓存在tcp socket的发送缓存队列中，等待网络空闲时再发送出去。同时，tcp协议要求对端在收到tcp数据报后，要对其序号进行ACK，只有当收到一个tcp 数据报的ACK之后，才可以把这个tcp数据报(以一个struct
 sk_buff的形式存在)从socket的发送缓冲队列中清除。
tcp socket的发送缓冲区实际上是一个结构体struct sk_buff的队列，我们可以把它称为发送缓冲队列，由结构体struct sock的成员sk_write_queue表示。sk_write_queue是一个结构体struct sk_buff_head类型，这是一个struct sk_buff的双向链表，其定义如下：
struct sk_buff_head {
struct sk_buff  *next;      //后指针
struct sk_buff  *prev;      //前指针
__u32           qlen;       //队列长度（即含有几个struct sk_buff)
spinlock_t      lock;       //链表锁
};
内核代码中，先在这个队列中创建足够存放数据的struct sk_buff，然后向队列存入应用数据。
如果write的字节数>socket发送缓冲区，tcp做何处理？
如果是非阻塞模式,是在设定的发送时间范围内能发多少发多少.
在实际应用中,情况如下:
在非阻塞模式下,一般是用setsockopt函数设置发送阻塞的时间,然后调用send()发送数据,当超出这个时间,send函数会返回已发送的数据大小, 但是请注意此时缓存中可能还有些数据没有发送到网络上.
那么当在应用层再一次调用send函数时,就会报告经典的错误:
Resource temporarily unavailable
那么如果是阻塞情况,send函数会一直等到所有应用层的数据全部发送完毕再返回...
另外,如果是用UDP作为发送端,那么都不需要考虑阻塞不阻塞的情况,会报告错误:
Message too long



