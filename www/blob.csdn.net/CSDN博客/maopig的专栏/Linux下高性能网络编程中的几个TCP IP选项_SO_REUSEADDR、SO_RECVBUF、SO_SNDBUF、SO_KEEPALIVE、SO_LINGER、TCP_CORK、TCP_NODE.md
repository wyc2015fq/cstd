# Linux下高性能网络编程中的几个TCP/IP选项_SO_REUSEADDR、SO_RECVBUF、SO_SNDBUF、SO_KEEPALIVE、SO_LINGER、TCP_CORK、TCP_NODE - maopig的专栏 - CSDN博客
2013年04月21日 12:04:51[maopig](https://me.csdn.net/maopig)阅读数：1625

        最近在新的平台上测试程序，以前一些没有注意到的问题都成为了性能瓶颈，通过设置一些TCP/IP选项能够解决一部分问题，当然根本的解决方法是重构代码，重新设计服务器框架。先列出几个TCP/IP选项：
选项
**man 7 socket:**
SO_REUSEADDR
SO_RECVBUF/SO_SNDBUF
SO_KEEPALIVE
SO_LINGER
**man 7 tcp:**
TCP_CORK
TCP_NODELAY
TCP_DEFER_ACCEPT
TCP_KEEPCNT/TCP_KEEPIDLE/TCP_KEEPINTVL
SO_REUSEADDR
**man 命令的 领域 名称 说明**
1 用户命令， 可由任何人启动的。
2 系统调用， 即由内核提供的函数。
3 例程， 即库函数。
4 设备， 即/dev目录下的特殊文件。
5 文件格式描述， 例如/etc/passwd。
6 游戏， 不用解释啦！
7 杂项， 例如宏命令包、惯例等。
8 系统管理员工具， 只能由root启动。
9 其他（Linux特定的）， 用来存放内核例行程序的文档。
**SO_REUSEADDR选项:**
**在服务器程序中，SO_REUSEADDR socket选项通常在调用bind()之前被设置。**
SO_REUSEADDR可以用在以下四种情况下： 
(摘自《Unix网络编程》卷一，即UNPv1)
1、当有一个有相同本地地址和端口的socket1处于TIME_WAIT状态时，而你启动的程序的socket2要占用该地址和端口，你的程序就要用到该选项。 
2、SO_REUSEADDR允许同一port上启动同一服务器的多个实例(多个进程)。但每个实例绑定的IP地址是不能相同的。**在有多块网卡或用IP Alias技术的机器可以测试这种情况。 **3、SO_REUSEADDR允许单个进程绑定相同的端口到多个socket上，但每个socket绑定的ip地址不同。这和2很相似，区别请看UNPv1。 
4、SO_REUSEADDR允许完全相同的地址和端口的重复绑定。但这只用于UDP的多播，不用于TCP。
**TCP_NODELAY/TCP_CHORK选项:**
TCP_NODELAY/TCP_CHORK
TCP_NODELAY和TCP_CORK基本上控制了包的“Nagle化”，**Nagle化在这里的含义是采用Nagle算法把较小的包组装为更大的帧。TCP_NODELAY和TCP_CORK都禁掉了Nagle算法，**只不过他们的行为不同而已。
TCP_NODELAY 不使用Nagle算法，不会将小包进行拼接成大包再进行发送，直接将小包发送出去，**会使得小包时候用户体验非常好。**
**Nagle算法参见自己的博客:http://blog.163.com/xychenbaihu@yeah/blog/static/132229655201231214038740/**当在传送大量数据的时候，为了提高TCP发送效率，可以设置TCP_CORK，CORK顾名思义，就是"塞子"的意思，它会尽量在每次发送最大的数据量。当设置了TCP_CORK后，会有阻塞200ms，当阻塞时间过后，数据就会自动传送。
详细的资料可以查看参考文献5。
**SO_LINGER选项:**
SO_LINGER
linger，顾名思义是延迟延缓的意思，这里是延缓面向连接的socket的close操作。
默认，close立即返回，但是当发送缓冲区中还有一部分数据的时候，系统将会尝试将数据发送给对端。SO_LINGER可以改变close的行为。
控制SO_LINGER通过下面一个结构：
struct linger
{
      int l_onoff; /*0=off, nonzero=on*/
      int l_linger; /*linger time, POSIX specifies units as seconds*/
};
通过结构体中成员的不同赋值，可以表现为下面几种情况：
1. l_onoff设置为0，选项被关闭。l_linger值被忽略，就是上面的**默认情形**，close立即返回。
2. l_onoff设置为非0，l_linger被设置为0，则close()不被阻塞立即执行，丢弃socket发送缓冲区中的数据，并向对端发送一个RST报文。
 这种关闭方式称为“强制”或“失效”关闭。
3. l_onoff设置为非0，l_linger被设置为非0，则close()调用阻塞进程，直到所剩数据发送完毕或超时。
这种关闭称为“优雅的”关闭。
**注意：**
       这个选项需要谨慎使用，尤其是强制式关闭，会丢失服务器发给客户端的最后一部分数据。UNP中:
The TIME_WAIT state is our friend and is there to help us(i.e., to let the old duplicate segments expire in the network).
**TCP_DEFER_ACCEPT选项:**
TCP_DEFER_ACCEPT
defer accept，从字面上理解是推迟accept，**实际上是当接收到第一个数据之后，才会创建连接，三次握手完成，连接还没有建立**。
对于像HTTP等非交互式的服务器，这个很有意义，可以用来防御空连接攻击（只是建立连接，但是不发送任何数据）。
使用方法如下：
val = 5;
setsockopt(srv_socket->fd, SOL_TCP, TCP_DEFER_ACCEPT, &val, sizeof(val)) ;
里面 val 的单位是秒，注意如果打开这个功能，kernel 在 val 秒之内还没有收到数据，不会继续唤醒进程，而是直接丢弃连接。如果服务器设置TCP_DEFER_ACCEPT选项后，服务器受到一个CONNECT请求后，三次握手之后，新的socket状态依然为SYN_RECV，而不是ESTABLISHED，操作系统不会Accept。
由于设置TCP_DEFER_ACCEPT选项之后，三次握手后状态没有达到ESTABLISHED，而是SYN_RECV。这个时候，如果客户端一直没有发送"数据"报文，服务器将重传SYN/ACK报文，重传次数受net.ipv4.tcp_synack_retries参数控制，达到重传次数之后，才会再次进行setsockopt中设置的超时值，因此会出现SYN_RECV生存时间比设置值大一些的情况。
关于SYN_RECV状态可以查看参考文献7。
**SO_KEEPALIVE选项:**
SO_KEEPALIVE/TCP_KEEPCNT/TCP_KEEPIDLE/TCP_KEEPINTVL
        如果一方已经关闭或异常终止连接，而另一方却不知道，我们将这样的TCP连接称为半打开的。
     TCP通过保活定时器(KeepAlive)来检测半打开连接。
         在高并发的网络服务器中，经常会出现漏掉socket的情况，对应的结果有一种情况就是出现大量的CLOSE_WAIT状态的连接。这个时候，可以通过设置KEEPALIVE选项来解决这个问题，当然还有其他的方法可以解决这个问题，详细的情况可以查看参考资料8。
使用方法如下：
//Setting For KeepAlive
int keepalive = 1;
setsockopt(incomingsock,SOL_SOCKET,SO_KEEPALIVE,(void*)(&keepalive),(socklen_t)sizeof(keepalive));
int keepalive_time = 30;
setsockopt(incomingsock, IPPROTO_TCP, TCP_KEEPIDLE,(void*)(&keepalive_time),(socklen_t)sizeof(keepalive_time));
int keepalive_intvl = 3;
setsockopt(incomingsock, IPPROTO_TCP, TCP_KEEPINTVL,(void*)(&keepalive_intvl),(socklen_t)sizeof(keepalive_intvl));
int keepalive_probes= 3;
setsockopt(incomingsock, IPPROTO_TCP, TCP_KEEPCNT,(void*)(&keepalive_probes),(socklen_t)sizeof(keepalive_probes));
       设置SO_KEEPALIVE选项来开启KEEPALIVE，然后通过TCP_KEEPIDLE、TCP_KEEPINTVL和TCP_KEEPCNT设置keepalive的开始时间、间隔、次数等参数。
        当然，也可以通过设置/proc/sys/net/ipv4/tcp_keepalive_time、tcp_keepalive_intvl和tcp_keepalive_probes等内核参数来达到目的，但是这样的话，会影响所有的socket，因此建议使用setsockopt设置
