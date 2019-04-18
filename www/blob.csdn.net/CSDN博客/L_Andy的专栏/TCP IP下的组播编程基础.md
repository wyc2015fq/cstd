# TCP/IP下的组播编程基础 - L_Andy的专栏 - CSDN博客

2014年07月09日 17:36:29[卡哥](https://me.csdn.net/L_Andy)阅读数：839
个人分类：[网络工程](https://blog.csdn.net/L_Andy/article/category/1925965)



本文讨论基于TCP/IP下的组播编程基础。所有的例子都可以Windows下台运行。

本文不会涉及到组播协议的实现，只是讨论组播编程需要的环境及API函数。关于组播的实现和需要的协议笔者 将会另写文章讨论。

## 组播描述

　　组播（Multicast）是网络一种点对多（one to many）的通信方式，通过报文复制完成网络中一台server对应多台接收者的高效数据传 送。对其形象的比喻就是类似于广播电台和电视台节目的发送。电台或电视台向特定频道发送他们的节目，而接收者可以根据自己的喜好选择频道来收听或收看节 目。

       传统网络的通信方式单 播（Unicast） 在许多场合下并不合适，如果强行采于单播方式只是会增加网络上传送的报文，还会影响server端的运行效率，如网络游戏，网络视频会议等。这些场合下由于接收者需要的是同样的数据，如果有Ｎ个 接收者，那么server就需要把这份数据做成N个报文分别发送给接收者，这样当接收者增多的情况下，server端发送的报文也同样需要增多，server端承受这样的通信量肯定有一个 极值，当接收者到一定程度的时候，不单单是网络流量增大的问题了，server面临的只有当机。

　　组播在这种场合下就有了用武之地了。Server端需要做的就是向特写的Group发送一个报文，当网络的多个接收者对这个报文感兴趣的时候，他们可以自由复制得到这个报文。这样的 情况下，无论接收者增加多少人，Server需要做的只是发送一个报文，而网络上传送的是只是一份报文。

单播图例：

![](http://blog.csdn.net/images/blog_csdn_net/garibaldi/48014/r_unicast.jpg)

多播图例：

![](http://blog.csdn.net/images/blog_csdn_net/garibaldi/48014/r_multicast.jpg)


## 组播相关

### 组播地址

　　32位的IP地址被分成了A, B, C, D, E五种类型，前三种是我们日常生活常见的，由一个权威部分统一分配。而Ｄ类地址我们需要的多播地址， 前三种地址我们可以形象的认为是一个点，而一个Ｄ类多播地址可以认为是一个频道。这样对多播的理解可以形象一点。

       关于IP地址的图例就免了，请查阅相关IP分类的相关资料，只要记住多播地址是从224.0.0.0－239.255.255.255就行了。224开头的为保留地址，239开头的为实验用地址。这些地址不能随 便使用，它们也是有权威部门（IANA）管理，类似于ＴＣＰ，ＵＤＰ的保留端口。

       常见多播保留地址

|地址|用途|介绍|
|----|----|----|
|224.0.0.1|本网段所有的主机（All Hosts）|[RFC 1112, JBP]|
|224.0.0.2|相邻的所有的路由器（All Multicast Routers ）|[JBP]|
|224.0.0.5|传送OSPF协议用（OSPF Routers）|[RFC 1583, JXM1]|
|224.0.0.6|OSPF Designated Routers|[RFC 1583, JXM1]|
|224.0.0.9|RIP2 Routers|[RFC 1723, SM11]|
|224.0.0.12|DHCP Server/Relay Agent|[RFC 1884]|
|224.0.0.13|All PIM Routers|[Farinacci]|

**实验**：可在主机上执行ping 224.0.0.1，看看有多少主 机有回应。J

### IP组播地址到链路层地址的转换

       IP多播地址的有效位 为28位，是不是 可以把这２８位都可以影射到链路层地址，如以太网，FDDI，Token ring（我们常见的为以太网地址，就是大部分计算机上的Rj45接口的网卡所拥有的地址）。以太网地 址为48位，IANA组织把00- 00- 5e开始的地址分配了用于多 播的地址，以太网多播地址的最高二位必须为01，也就变成了01- 00- 5e，以太网用于多播地址的范围也就变成了 01- 00 – 5e – 00 – 00 - 00 到 01 – 00 – 5e – ff – ff
 – ff。

也就是剩下了24位地址，这24位最高位必须为１，只剩下了23位（L不知道为什么要这么麻烦）。

       问题出现了，IP多播地址的28位有效位如何影射到以太网的23位L，现在解决的方法是只影射IP多播地址的后23位，也就是IP剩下了5位不太影射，所以出现了多个IP多播地址（最多32个）影射一个以太网地址的情况。好像有点跑题了L

### 组播主机需要的环境

       如果要在一台主机上运 行多播程序，这台主机支持IGMP协议。此协议用于加入一个多播组，离开一个多播组等而向路由器发送命令。现在的IGMP协议最高版本为v3(rfc3376)，现在有很少数的 ＵＮＩＸ实现了，WindowXp己实现。大部分主机，路由器实现的版本为v2（rfc2236），而v1版本（rfc1112）的运行是从v2中可以实现。V2与v1的区别就是把V１版本中的４位版本字段跟４位操作类型字段合并成了８位操作类型，在Ｖ１中不用使用的８位字段在Ｖ２ 中被称之为最大响应时间。用Ｃ写Ｖ２的IGMP结构即为：

struct igmp

{

    u_char igmp_type;

    u_char igmp_code;

    u_char igmp_cksum;

    struct in_addr igmp_addr;

};

我记得第二个字段（Max Resp Time）在《TCP/IP详解卷2》中被写成了igmp_code，今天下午刚刚看的，不太清楚了，也不知道为什么起这么一个名字，明天再看看：)　可以看得出IGMP报文为定长的（８个字节）。可能这是 最简单的协议结构了（什么，ＵＤＰ也简单？再想想ＵＤＰ）。

### 组播需要的网络环境

       如果主机想获得多播报 文，相邻的路由器也必须支持IGMP，如果想获得Internet上的多播报文，主机到Server的这个路径中所遇到的路由器必须全部支持IGMP，路由器还必须支持源发现协议，如MSDP，PIM_DM，PIM_SM等。

### 组播的等级

       Level 0　不支 持IP多播

       Level 1    只支持向多播组发送数据而不能接收多播组的数据

       Level 2    IP多播全支持

       对这三个等级的理解应 该从SOCKET上。 建立了一个SOCKET以后可以对它进行设置，看它需求什么。一般现在存在的网络程序就就是Level0了，因为它们不支持多播，如用于连接ＷＥＢ服务器获取网页内容的那个SOCKET就应该属于Level0。

       一个演唱会现场网络直 播，由于采用了多播，服务器要向一个多播组发送报文，因为他不需要获取接收者的报文，所以可以建立一个SOCKET只向特定的多播组发送数据就可以 了，这个SOCKET应该就是Level 1

       一个网络会议的例子， 由于会议是有多个人参加的，每个人都需要接收其它人的报文，所以建立了一个SOCKET，首先把这个SOCKET加入到一个多播组，使其能接收多播组的数据，然后它也可以用这个SOCKET向自己加入的多播组发送自己的状 态。这个SOCKET就应该是Level 2了 J

## 组播编程相关的socket结构和函数

       int setsockopt(SOCKET s, int level, int optname, const char FAR * optval, int optlen);

       int getsockopt(SOCKET s, int level, int optname, char FAR * optval, int FAR * optlen);

       level必须为IPPROTO_IP。不要问为什么。这两 个函数在组播干什么呢？获取系统对组播的设置（如ＴＴＬ），加入一个多播组，离开一个多播组就用这两个函数的其中一个，哪果你要问是哪个？就不要往下看了L

       optname就是在 组播起到最主要作用的一个字段，与组播相关的可取值

|可取值|setsockopt|getsockopt|
|----|----|----|
|IP_MULTICAST_LOOP|支持|支持|
|IP_MULTICAST_TTL|支持|支持|
|IP_MULTICAST_IF|支持|支持|
|IP_ADD_MEMBERSHIP|支持|不支持|
|IP_DROP_MEMBERSHIP|支持|不支持|

1.         IP_MULTICAST_LOOP

当接收者加入到一个多播组以后，再向这个多播组发送数据，这个字段的设置是是

       否允许再返回到本身。

2.         IP_MULTICAST_TTL

默认情况下，多播报文的ＴＴＬ被设置成了１，也就是说到这个报文在网络传送的

       时候，它只能在自己所 在的网络传送，当要向外发送的时候，路由器把ＴＴＬ减１以后

       变成了０，这个报文就 已经被Discard了。例：

    char ttl;

    ttl = 2;

    setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char*)ttl, sizeof(ttl));

3.         IP_MULTICAST_IF

发送多播报文时用的本地接口，默认情况下被设置成了本地接口的第一个地址。

       未完

4.         IP_ADD_MEMBERSHIP

这个option和下面的option是实现多播必不可少的，它用于加入一个多播组，例：

    struct ip_mreq ipmr;

    ipmr.imr_interface.s_addr = htonl(INADDR_ANY);

    ipmr.imr_multiaddr.s_addr = inet_addr("234.5.6.7");

    setsockopt(s, IPPROTO_IP, IP_ADDR_MEMBERSHIP, (char*)&ipmr, sizeof(ipmr));

5.         IP_DROP_MEMBERSHIP

用于离开一个多播组，使用方法同IP_ADDR_MEMBERSHIP。

    struct ip_mreq  ipmr;

    int             len;

    setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&ipmr, &len);

### 向一个特定的多播组发送数据

       向多播组发送数据最简 单了，多播只支持ＵＤＰ没有说吧？不知道在什么地方写L，下一版改过来J，发送数据不需要先加入这个多播 组。只是简单的把目的地址设成多播地址就可以了，如：

SOCKET      s;

    SOCKADDR_IN     multiaddr;

    s = socket(….);

    /*不需要bind*/

    /*添充结构，向234.5.6.7多播组发送数据*/

    ….

    multiaddr.sin_addr.s_addr = inet_addr(“234.5.6.7”);

    ….

    /*发送buf就可 以了*/

    sendto(s, buf, len, 0, &multiaddr, &len); 

### 从一个多播组接收数据

SOCKET      s;

    SOCKADDR_IN     multiaddr;

    s = socket(….);

       /*bind的目的就是要指定一个本地接口的特定端口。这点应该很好理解*/

    bind(s, …..);

    /*下面就把这个SOCKET加入到一个多播组, 比如加入到234.5.6.7*/

    struct ip_mreq remote;

    int len = sizeof(remote);

    remote.imr_multiaddr.s_addr = inet_addr(“234.5.6.7”);

    remote.imr_interface.s_addr = htonl(INADDR_ANY);

    /*执行了这句就已经加入到了多播组了*/

    setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &remote, len);

    /*可以接收这个多播组的数据了*/

    recvfrom(s, buf, maxlen, …….);

    /*离开这个多播组*/

    setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, &remote, &len);

