# /proc/sys/net/ipv4详解(1) - weixin_33985507的博客 - CSDN博客
2012年06月24日 21:44:35[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
/proc/sys/net/ipv4详解
[原文地址](http://blog.163.com/_kid/blog/static/30405476201142453031474/%20)It你好整理   
通过/proc/sys/net/ipv4/优化Linux下网络性能
/proc/sys/net/ipv4/优化
1)      /proc/sys/net/ipv4/ip_forward
该文件表示是否打开IP转发。
0，禁止
1，转发
缺省设置：0
2)      /proc/sys/net/ipv4/ip_default_ttl
该文件表示一个数据报的生存周期（Time To Live），即最多经过多少路由器。
缺省设置：64
增加该值会降低系统性能。
3)      /proc/sys/net/ipv4/ip_no_pmtu_disc
该文件表示在全局范围内关闭路径MTU探测功能。
缺省设置：0
4)      /proc/sys/net/ipv4/route/min_pmtu
该文件表示最小路径MTU的大小。
缺省设置：552
5)      /proc/sys/net/ipv4/route/mtu_expires
该文件表示PMTU信息缓存多长时间（秒）。
缺省设置：600（秒）
6)      /proc/sys/net/ipv4/route/min_adv_mss
该文件表示最小的MSS（Maximum Segment Size）大小，取决于第一跳的路由器MTU。
缺省设置：256（bytes）
6.1 IP Fragmentation
1)      /proc/sys/net/ipv4/ipfrag_low_thresh
/proc/sys/net/ipv4/ipfrag_low_thresh两个文件分别表示用于重组IP分段的内存分配最低值和最高值，一旦达到最高内存分配值，其它分段将被丢弃，直到达到最低内存分配值。
缺省设置：196608（ipfrag_low_thresh）
262144（ipfrag_high_thresh）
2)      /proc/sys/net/ipv4/ipfrag_time
该文件表示一个IP分段在内存中保留多少秒。
缺省设置：30（秒）
6.2 INET Peer Storage
1)      /proc/sys/net/ipv4/inet_peer_threshold
INET对端存储器某个合适值，当超过该阀值条目将被丢弃。该阀值同样决定生存时间以及废物收集通过的时间间隔。条目越多，存活期越低，GC 间隔越短。
缺省设置：65664
2)      /proc/sys/net/ipv4/inet_peer_minttl
条目的最低存活期。在重组端必须要有足够的碎片(fragment)存活期。这个最低存活期必须保证缓冲池容积是否少于 inet_peer_threshold。该值以 jiffies为单位测量。
缺省设置：120
3)      /proc/sys/net/ipv4/inet_peer_maxttl
条目的最大存活期。在此期限到达之后，如果缓冲池没有耗尽压力的话(例如：缓冲池中的条目数目非常少)，不使用的条目将会超时。该值以 jiffies为单位测量。
缺省设置：600
4)      /proc/sys/net/ipv4/inet_peer_gc_mintime
废物收集(GC)通过的最短间隔。这个间隔会影响到缓冲池中内存的高压力。该值以 jiffies为单位测量。
缺省设置：10
5)      /proc/sys/net/ipv4/inet_peer_gc_maxtime
废物收集(GC)通过的最大间隔，这个间隔会影响到缓冲池中内存的低压力。该值以 jiffies为单位测量。
缺省设置：120
6.3 TCP Variables
1)      /proc/sys/net/ipv4/tcp_syn_retries
该文件表示本机向外发起TCP SYN连接超时重传的次数，不应该高于255；该值仅仅针对外出的连接，对于进来的连接由tcp_retries1控制。
缺省设置：5
2)      /proc/sys/net/ipv4/tcp_keepalive_probes
该文件表示丢弃TCP连接前，进行最大TCP保持连接侦测的次数。保持连接仅在SO_KEEPALIVE套接字选项被打开时才被发送。
缺省设置：9（次）
3)      /proc/sys/net/ipv4/tcp_keepalive_time
该文件表示从不再传送数据到向连接上发送保持连接信号之间所需的秒数。
缺省设置：7200（2小时）
4)      /proc/sys/net/ipv4/tcp_keepalive_intvl
该文件表示发送TCP探测的频率，乘以tcp_keepalive_probes表示断开没有相应的TCP连接的时间。
缺省设置：75（秒）
5)      /proc/sys/net/ipv4/tcp_retries1
该文件表示放弃回应一个TCP连接请求前进行重传的次数。
缺省设置：3
6)      /proc/sys/net/ipv4/tcp_retries2
该文件表示放弃在已经建立通讯状态下的一个TCP数据包前进行重传的次数。
缺省设置：15
7)      /proc/sys/net/ipv4/tcp_orphan_retries
在近端丢弃TCP连接之前，要进行多少次重试。默认值是 7 个，相当于 50秒–16分钟，视 RTO 而定。如果您的系统是负载很大的web服务器，那么也许需要降低该值，这类 sockets 可能会耗费大量的资源。另外参考tcp_max_orphans。
8)      /proc/sys/net/ipv4/tcp_fin_timeout
对于本端断开的socket连接，TCP保持在FIN-WAIT-2状态的时间。对方可能会断开连接或一直不结束连接或不可预料的进程死亡。默认值为 60 秒。过去在2.2版本的内核中是 180 秒。您可以设置该值，但需要注意，如果您的机器为负载很重的web服务器，您可能要冒内存被大量无效数据报填满的风险，FIN-WAIT-2 sockets 的危险性低于 FIN-WAIT-1，因为它们最多只吃 1.5K 的内存，但是它们存在时间更长。另外参考 tcp_max_orphans。
缺省设置：60（秒）
9)      /proc/sys/net/ipv4/tcp_max_tw_buckets
系统在同时所处理的最大timewait sockets 数目。如果超过此数的话，time-wait socket 会被立即砍除并且显示警告信息。之所以要设定这个限制，纯粹为了抵御那些简单的 DoS ***，千万不要人为的降低这个限制，不过，如果网络条件需要比默认值更多，则可以提高它(或许还要增加内存)。
缺省设置：180000
10) /proc/sys/net/ipv4/tcp_tw_recyle
打开快速 TIME-WAIT sockets 回收。除非得到技术专家的建议或要求，请不要随意修改这个值。
缺省设置：0
11) /proc/sys/net/ipv4/tcp_tw_reuse
该文件表示是否允许重新应用处于TIME-WAIT状态的socket用于新的TCP连接。
缺省设置：0
12) /proc/sys/net/ipv4/tcp_max_orphans
系统所能处理不属于任何进程的TCP sockets最大数量。假如超过这个数量，那
么不属于任何进程的连接会被立即reset，并同时显示警告信息。之所以要设定这
个限制，纯粹为了抵御那些简单的 DoS ***，千万不要依赖这个或是人为的降
低这个限制。
缺省设置：8192
13) /proc/sys/net/ipv4/tcp_abort_on_overflow
当守护进程太忙而不能接受新的连接，就向对方发送reset消息，默认值是false。
这意味着当溢出的原因是因为一个偶然的猝发，那么连接将恢复状态。只有在你确信守护进程真的不能完成连接请求时才打开该选项，该选项会影响客户的使用。
缺省设置：０
14) /proc/sys/net/ipv4/tcp_syncookies
该文件表示是否打开TCP同步标签(syncookie)，内核必须打开了 CONFIG_SYN_COOKIES项进行编译。同步标签(syncookie)可以防止一个套接字在有过多试图连接到达时引起过载。
缺省设置：0
15) /proc/sys/net/ipv4/tcp_stdurg
使用 TCP urg pointer 字段中的主机请求解释功能。大部份的主机都使用老旧的BSD解释，因此如果您在 Linux 打开它，或会导致不能和它们正确沟通。
缺省设置：0
16) /proc/sys/net/ipv4/tcp_max_syn_backlog
对于那些依然还未获得客户端确认的连接请求，需要保存在队列中最大数目。对于超过 128Mb 内存的系统，默认值是 1024，低于 128Mb 的则为 128。如果服务器经常出现过载，可以尝试增加这个数字。警告！假如您将此值设为大于1024，最好修改 include/net/tcp.h 里面的TCP_SYNQ_HSIZE，以保持TCP_SYNQ_HSIZE*16 0)或者bytes-bytes/2^(-tcp_adv_win_scale)(如果tcp_adv_win_scale 128Mb 32768-610000)则系统将忽略所有发送给自己的ICMP ECHO请求或那些广播地址的请求。
缺省设置：1024
17) /proc/sys/net/ipv4/tcp_window_scaling
下文见[http://itnihao.blog.51cto.com/1741976/908021](http://itnihao.blog.51cto.com/1741976/908021)
