# Android 的用户层 uevent处理机制（转） - h13 - 博客园
摘录几篇android研发日志
1 http://blog.csdn.net/linweig/archive/2010/06/01/5640697.aspx
# Firmware 加载原理分析－－－－分析的比较透彻，我在wifimodule调试时遇到的问题几乎跟这个一样，不过我采取了另外一个方法，直接用kernel的read和write了，没有使用request_firmware,有空小结下。
前言      
              前段时间移植 wifi 驱动到 android 的内核上，发现 firmware 的加载始终出错，问了几个人，都不是很了解，没办法，只好自己研究一下。
# 原理分析
    从本质上来说， firmware 需要做的事情包括两件：
1，  通知用户态程序，我需要下载 firmware 了；
2，  用户态程序把用户态的数据 copy 到内核层；
3，  内核把内核态的数据写到设备上，比如 wifi 模块里；
其中第三步应该不难，关键是看看， linux 里面是如何实现第一、二步的；
# 实现机制
              简单的说，它的机制分成以下几部分：
1，  通过一定的方式，通知用户态程序，比如 init 程序，如图所示：
![](https://pic002.cnblogs.com/images/2012/121648/2012071522362354.jpg)
       显然是通过 kobject_uevent 的方式通知的 应用层，它的机制我有空再详细解释，简单的说，就是往一个 socket 广播一个消息，只需要在应用层打开 socket 监听 **NETLINK_KOBJECT_UEVENT **组的消息，就可以收到了。
          用户态的 init 是如何做的？
![](https://pic002.cnblogs.com/images/2012/121648/2012071522365262.jpg)
      可以看到 init 程序打开了一个 socket ，然后绑定它， 最后通过 select 来监听 socket 上来的数据，最后调用handle_device_fd 来处理收到的消息；当内核发送一个 KOBJ_ADD 的消息上来的时候，经过过 滤，判断是否是 firmware 要被加载的消息，然后调用
              handle_firmware_event 来处理；
2，  用户态的数据如何下载到内核；
       本质上它是内核创建了两个文件，一个文件 A 用来标志下载的开始和结 束，另外一个文件 B 用来接收用户层传下来的数据，当用户态的程序往 A 文件写入 1 的时候，标志用户态程序已经往里面写程序来，而往里面写入 0 的时候，就标志下载成功结束，如果写入 -1 就表示下载失败了；下面 看看这两个文件是如何被创建的 , 以及数据是如何写到内核的，请看图：
![](https://pic002.cnblogs.com/images/2012/121648/2012071522372537.jpg)
       这个图基本上就是两个文件被创立的过程，以及当这两个文 件被用户态程序访问的时候将要被调用的函数，比如对于标志文件，如果往里面写入数据，将会触发函数 **firmware_loading_store **函数，如果往 bin文件里面写入数据将会触发 bin 文件类型的 write 函数；
       用户态写数据的过程大约是这样的：当用户态收到 KOBJ_ADD 消息的时候 最终将会调用handle_firmware_event 的函数；
![](https://pic002.cnblogs.com/images/2012/121648/2012071522374774.jpg)
                      它的过程就是：
a, 先往标志文件里面写 1 ；
b, 从用户空间读取数据；
c, 往内核创建的文件里面写数据；
d, 如果成功写入 0 ，否则写入 -1 ；
下面看看内核是如何接受这些文件的，前面提到内核创建了一个 bin 文件，用来接收用户态的数据，下面看 看这个过程：
![](https://pic002.cnblogs.com/images/2012/121648/2012071522380674.jpg)
对于SYSFS_KOBJ_BIN_ATTR 属 性的文件，在 inode 初始化的时候，将会被赋予 bin_fops 的文件操作函数集，于是当上层调用 write 的时候，将会走到内核的 bin_fops.write 函数；这个函数干的事情很简单，就是把用户态的数据 copyright 到 bb->buffer ，而 bb->buffer 其 实是在 open 的 时候分配的空间，这样的话，就实现了用户态的数据到内核的 copy ；过程是不是完了？
还有一个步骤，这个 bb->buffer 本身是如何与 wifi 驱动交互的呢？这只是一个中间层，它的数据必须要写到 wifi 的驱动才应该算完整，而这一步其实 就是通过 flush_write 来完成的，下面看看这个过程：
![](https://pic002.cnblogs.com/images/2012/121648/2012071522385661.jpg)
这里可以清楚的看到， flush_write 做的事情就是把 bb->buffer 的内容 copy 到 wifi driver 分配的空间 fw->data 里面去了，至此，用户态的数据已经完整的写到了 wifi 的 driver 空间了；
3，  内核态的数据到 wifi 模块
       这个就比较简单了，通过函数 sdio_writesb 利用 sdio 总线把数据写到模块 里面去了；
# 总结
                  Firmware 的加载主要是利用了 uevent 的通讯机制实现用户态和内核 态的交互，另外还涉及了 sys文件系统里的文件创建 , 我加载 wifi firmware 始终出错的原因是 android 的文件系统要求把 wifi 的 firmware helper 放到 /etc/firmware 里面，而把真正 的 firmware sd8686.bin 放到 /etc/firmware/mrvl 里面，估计是 marvel修改后的结果，结论就是，这个设计真丑；
**背景知识**
2 http://blog.chinaunix.net/u3/90973/showart_1815538.html
**Netlink 套接字**
Netlink 用于在内核模块与在用户地址空间中的进程之间传递消息的。它包含了用于用户进程的基于标准套接字的接口和用于内核模块的一个内部核心 API。
Netlink 是一种特殊的 socket，它是 Linux 所特有的，类似于 BSD 中的AF_ROUTE 但又远比它的功能强大，目前在最新的 Linux 内核（2.6.14）中使用netlink 进行应用与内核通信的应用很多，包括：路由 daemon（NETLINK_ROUTE），1-wire 子系统（NETLINK_W1），用户态 socket 协议（NETLINK_USERSOCK），防火墙（NETLINK_FIREWALL），socket 监视（NETLINK_INET_DIAG），netfilter 日志（NETLINK_NFLOG），ipsec 安全策略（NETLINK_XFRM），SELinux 事件通知（NETLINK_SELINUX），iSCSI 子系统（NETLINK_ISCSI），进程审计（NETLINK_AUDIT），转发信息表查询 （NETLINK_FIB_LOOKUP），netlink connector(NETLINK_CONNECTOR),netfilter 子系统（NETLINK_NETFILTER），IPv6 防火墙（NETLINK_IP6_FW），DECnet 路由信息（NETLINK_DNRTMSG），内核事件向用户态通知（NETLINK_KOBJECT_UEVENT），通用 netlink（NETLINK_GENERIC）。
Netlink 是一种在内核与用户应用间进行双向数据传输的非常好的方式，用户态应用使用标准的 socket API 就可以使用 netlink 提供的强大功能，内核态需要使用专门的内核 API 来使用 netlink。
Netlink 相对于系统调用，ioctl 以及 /proc 文件系统而言具有以下优点：
1，为了使用 netlink，用户仅需要在 include/linux/netlink.h 中增加一个新类型的 netlink 协议定义即可， 如 #define NETLINK_MYTEST 17 然后，内核和用户态应用就可以立即通过 socket API 使用该 netlink 协议类型进行数据交换。但系统调用需要增加新的系统调用，ioctl 则需要增加设备或文件， 那需要不少代码，proc 文件系统则需要在 /proc 下添加新的文件或目录，那将使本来就混乱的 /proc 更加混乱。
2. netlink是一种异步通信机制，在内核与用户态应用之间传递的消息保存在socket缓存队列中，发送消息只是把消息保存在接收者的socket的接 收队列，而不需要等待接收者收到消息，但系统调用与 ioctl 则是同步通信机制，如果传递的数据太长，将影响调度粒度。
3．使用 netlink 的内核部分可以采用模块的方式实现，使用 netlink 的应用部分和内核部分没有编译时依赖，但系统调用就有依赖，而且新的系统调用的实现必须静态地连接到内核中，它无法在模块中实现，使用新系统调用的应用在 编译时需要依赖内核。
4．netlink 支持多播，内核模块或应用可以把消息多播给一个netlink组，属于该neilink 组的任何内核模块或应用都能接收到该消息，内核事件向用户态的通知机制就使用了这一特性，任何对内核事件感兴趣的应用都能收到该子系统发送的内核事件，在 后面的文章中将介绍这一机制的使用。
5．内核可以使用 netlink 首先发起会话，但系统调用和 ioctl 只能由用户应用发起调用。
6．netlink 使用标准的 socket API，因此很容易使用，但系统调用和 ioctl则需要专门的培训才能使用。
**用户态使用 netlink**
用户态应用使用标准的socket APIs， socket(), bind(), sendmsg(), recvmsg() 和 close() 就能很容易地使用 netlink socket，查询手册页可以了解这些函数的使用细节，本文只是讲解使用 netlink 的用户应该如何使用这些函数。注意，使用 netlink 的应用必须包含头文件 linux/netlink.h。当然 socket 需要的头文件也必不可少，sys/socket.h。
为了创建一个 netlink socket，用户需要使用如下参数调用 socket():
```
socket(AF_NETLINK, SOCK_RAW, netlink_type)
```
第一个参数必须是 AF_NETLINK 或 PF_NETLINK，在 Linux 中，它们俩实际为一个东西，它表示要使用netlink，第二个参数必须是SOCK_RAW或SOCK_DGRAM， 第三个参数指定netlink协议类型，如前面讲的用户自定义协议类型NETLINK_MYTEST， NETLINK_GENERIC是一个通用的协议类型，它是专门为用户使用的，因此，用户可以直接使用它，而不必再添加新的协议类型。内核预定义的协议类 型有：
```
#define NETLINK_ROUTE           0       /* Routing/device hook                          */
#define NETLINK_W1              1       /* 1-wire subsystem                             */
#define NETLINK_USERSOCK        2       /* Reserved for user mode socket protocols      */
#define NETLINK_FIREWALL        3       /* Firewalling hook                             */
#define NETLINK_INET_DIAG       4       /* INET socket monitoring                       */
#define NETLINK_NFLOG           5       /* netfilter/iptables ULOG */
#define NETLINK_XFRM            6       /* ipsec */
#define NETLINK_SELINUX         7       /* SELinux event notifications */
#define NETLINK_ISCSI           8       /* Open-iSCSI */
#define NETLINK_AUDIT           9       /* auditing */
#define NETLINK_FIB_LOOKUP      10
#define NETLINK_CONNECTOR       11
#define NETLINK_NETFILTER       12      /* netfilter subsystem */
#define NETLINK_IP6_FW          13
#define NETLINK_DNRTMSG         14      /* DECnet routing messages */
#define NETLINK_KOBJECT_UEVENT  15      /* Kernel messages to userspace */
#define NETLINK_GENERIC         16
```
对于每一个netlink协议类型，可以有多达 32多播组，每一个多播组用一个位表示，netlink 的多播特性使得发送消息给同一个组仅需要一次系统调用，因而对于需要多拨消息的应用而言，大大地降低了系统调用的次数。
函数 bind() 用于把一个打开的 netlink socket 与 netlink 源 socket 地址绑定在一起。netlink socket 的地址结构如下：
```
struct sockaddr_nl
{
  sa_family_t    nl_family;
  unsigned short nl_pad;
  __u32          nl_pid;
  __u32          nl_groups;
};
```
字段 nl_family 必须设置为 AF_NETLINK 或着 PF_NETLINK，字段 nl_pad 当前没有使用，因此要总是设置为 0，字段 nl_pid 为接收或发送消息的进程的 ID，如果希望内核处理消息或多播消息，就把该字段设置为 0，否则设置为处理消息的进程 ID。字段 nl_groups 用于指定多播组，bind 函数用于把调用进程加入到该字段指定的多播组，如果设置为 0，表示调用者不加入任何多播组。
传递给 bind 函数的地址的 nl_pid 字段应当设置为本进程的进程 ID，这相当于 netlink socket 的本地地址。但是，对于一个进程的多个线程使用 netlink socket 的情况，字段 nl_pid 则可以设置为其它的值，如：
```
pthread_self() << 16 | getpid();
```
因此字段 nl_pid 实际上未必是进程 ID,它只是用于区分不同的接收者或发送者的一个标识，用户可以根据自己需要设置该字段。函数 bind 的调用方式如下：
```
bind(fd, (struct sockaddr*)&nladdr, sizeof(struct sockaddr_nl));
```
fd为前面的 socket 调用返回的文件描述符，参数 nladdr 为 struct sockaddr_nl 类型的地址。为了发送一个 netlink 消息给内核或其他用户态应用，需要填充目标 netlink socket 地址，此时，字段 nl_pid 和 nl_groups 分别表示接收消息者的进程 ID 与多播组。如果字段 nl_pid 设置为 0，表示消息接收者为内核或多播组，如果 nl_groups为 0，表示该消息为单播消息，否则表示多播消息。使用函数 sendmsg 发送 netlink 消息时还需要引用结构 struct msghdr、struct nlmsghdr 和 struct iovec，结构 struct msghdr 需如下设置：
```
struct msghdr msg;
memset(&msg, 0, sizeof(msg));
msg.msg_name = (void *)&(nladdr);
msg.msg_namelen = sizeof(nladdr);
```
其中 nladdr 为消息接收者的 netlink 地址。
struct nlmsghdr 为 netlink socket 自己的消息头，这用于多路复用和多路分解 netlink 定义的所有协议类型以及其它一些控制，netlink 的内核实现将利用这个消息头来多路复用和多路分解已经其它的一些控制，因此它也被称为netlink 控制块。因此，应用在发送 netlink 消息时必须提供该消息头。
```
struct nlmsghdr
{
  __u32 nlmsg_len;   /* Length of message */
  __u16 nlmsg_type;  /* Message type*/
  __u16 nlmsg_flags; /* Additional flags */
  __u32 nlmsg_seq;   /* Sequence number */
  __u32 nlmsg_pid;   /* Sending process PID */
};
```
字段 nlmsg_len 指定消息的总长度，包括紧跟该结构的数据部分长度以及该结构的大小，字段 nlmsg_type 用于应用内部定义消息的类型，它对 netlink 内核实现是透明的，因此大部分情况下设置为 0，字段 nlmsg_flags 用于设置消息标志，可用的标志包括：
```
/* Flags values */
#define NLM_F_REQUEST           1       /* It is request message.       */
#define NLM_F_MULTI             2       /* Multipart message, terminated by NLMSG_DONE */
#define NLM_F_ACK               4       /* Reply with ack, with zero or error code */
#define NLM_F_ECHO              8       /* Echo this request            */
/* Modifiers to GET request */
#define NLM_F_ROOT      0x100   /* specify tree root    */
#define NLM_F_MATCH     0x200   /* return all matching  */
#define NLM_F_ATOMIC    0x400   /* atomic GET           */
#define NLM_F_DUMP      (NLM_F_ROOT|NLM_F_MATCH)
/* Modifiers to NEW request */
#define NLM_F_REPLACE   0x100   /* Override existing            */
#define NLM_F_EXCL      0x200   /* Do not touch, if it exists   */
#define NLM_F_CREATE    0x400   /* Create, if it does not exist */
#define NLM_F_APPEND    0x800   /* Add to end of list           */
```
标志NLM_F_REQUEST用于表示消息是一个请求，所有应用首先发起的消息都应设置该标志。
标志NLM_F_MULTI 用于指示该消息是一个多部分消息的一部分，后续的消息可以通过宏NLMSG_NEXT来获得。
宏NLM_F_ACK表示该消息是前一个请求消息的响应，顺序号与进程ID可以把请求与响应关联起来。
标志NLM_F_ECHO表示该消息是相关的一个包的回传。
标志NLM_F_ROOT 被许多 netlink 协议的各种数据获取操作使用，该标志指示被请求的数据表应当整体返回用户应用，而不是一个条目一个条目地返回。有该标志的请求通常导致响应消息设置 NLM_F_MULTI标志。注意，当设置了该标志时，请求是协议特定的，因此，需要在字段 nlmsg_type 中指定协议类型。
标 志 NLM_F_MATCH 表示该协议特定的请求只需要一个数据子集，数据子集由指定的协议特定的过滤器来匹配。
标志 NLM_F_ATOMIC 指示请求返回的数据应当原子地收集，这预防数据在获取期间被修改。
标志 NLM_F_DUMP 未实现。
标志 NLM_F_REPLACE 用于取代在数据表中的现有条目。
标志 NLM_F_EXCL_ 用于和 CREATE 和 APPEND 配合使用，如果条目已经存在，将失败。
标志 NLM_F_CREATE 指示应当在指定的表中创建一个条目。
标志 NLM_F_APPEND 指示在表末尾添加新的条目。
内核需要读取和修改这些标志，对于一般的使用，用户把它设置为 0 就可以，只是一些高级应用（如 netfilter 和路由 daemon 需要它进行一些复杂的操作），字段 nlmsg_seq 和 nlmsg_pid 用于应用追踪消息，前者表示顺序号，后者为消息来源进程 ID。下面是一个示例：
```
#define MAX_MSGSIZE 1024
char buffer[] = "An example message";
struct nlmsghdr nlhdr;
nlhdr = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_MSGSIZE));
strcpy(NLMSG_DATA(nlhdr),buffer);
nlhdr->nlmsg_len = NLMSG_LENGTH(strlen(buffer));
nlhdr->nlmsg_pid = getpid();  /* self pid */
nlhdr->nlmsg_flags = 0;
```
结构 struct iovec 用于把多个消息通过一次系统调用来发送，下面是该结构使用示例：
```
struct iovec iov;
iov.iov_base = (void *)nlhdr;
iov.iov_len = nlh->nlmsg_len;
msg.msg_iov = &iov;
msg.msg_iovlen = 1;
```
在完成以上步骤后，消息就可以通过下面语句直接发送：
```
sendmsg(fd, &msg, 0);
```
应用接收消息时需要首先分配一个足够大的缓存来保存消息头以及消息的数据部分，然后填充消息头，添完后就可以直接调用函数 recvmsg() 来接收。
```
#define MAX_NL_MSG_LEN 1024
struct sockaddr_nl nladdr;
struct msghdr msg;
struct iovec iov;
struct nlmsghdr * nlhdr;
nlhdr = (struct nlmsghdr *)malloc(MAX_NL_MSG_LEN);
iov.iov_base = (void *)nlhdr;
iov.iov_len = MAX_NL_MSG_LEN;
msg.msg_name = (void *)&(nladdr);
msg.msg_namelen = sizeof(nladdr);
msg.msg_iov = &iov;
msg.msg_iovlen = 1;
recvmsg(fd, &msg, 0);
```
注意：fd为socket调用打开的netlink socket描述符。
在消息接收后，nlhdr指向接收到的消息的消息头，nladdr保存了接收到的消息的目标地址，宏NLMSG_DATA(nlhdr)返 回指向消息的数据部分的指针。
在linux/netlink.h中定义了一些方便对消息进行处理的宏，这些宏包括：
```
#define NLMSG_ALIGNTO   4
#define NLMSG_ALIGN(len) ( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1) )
```
宏NLMSG_ALIGN(len)用于得到不小于len且字节对齐的最小数值。
```
#define NLMSG_LENGTH(len) ((len)+NLMSG_ALIGN(sizeof(struct nlmsghdr)))
```
宏NLMSG_LENGTH(len)用于计算数据部分长度为len时实际的消息长度。它一般用于分配消息缓存。
```
#define NLMSG_SPACE(len) NLMSG_ALIGN(NLMSG_LENGTH(len))
```
宏NLMSG_SPACE(len)返回不小于NLMSG_LENGTH(len)且字节对齐的最小数值，它也用于分配消息缓存。
```
#define NLMSG_DATA(nlh)  ((void*)(((char*)nlh) + NLMSG_LENGTH(0)))
```
宏NLMSG_DATA(nlh)用于取得消息的数据部分的首地址，设置和读取消息数据部分时需要使用该宏。
```
#define NLMSG_NEXT(nlh,len)      ((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), /
                      (struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))
```
宏NLMSG_NEXT(nlh,len)用于得到下一个消息的首地址，同时len也减少为剩余消息的总长度，该宏一般在一个消息被分成几个部分发 送或接收时使用。
```
#define NLMSG_OK(nlh,len) ((len) >= (int)sizeof(struct nlmsghdr) && /
                           (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && /
                           (nlh)->nlmsg_len <= (len))
```
宏NLMSG_OK(nlh,len)用于判断消息是否有len这么长。
```
#define NLMSG_PAYLOAD(nlh,len) ((nlh)->nlmsg_len - NLMSG_SPACE((len)))
```
宏NLMSG_PAYLOAD(nlh,len)用于返回payload的长度。
函数close用于关闭打开的netlink socket。
**netlink内核API**
netlink的内核实现在.c文件net/core/af_netlink.c中，内核模块要想使用netlink，也必须包含头文件 linux/netlink.h。内核使用netlink需要专门的API，这完全不同于用户态应用对netlink的使用。如果用户需要增加新的 netlink协议类型，必须通过修改linux/netlink.h来实现，当然，目前的netlink实现已经包含了一个通用的协议类型 NETLINK_GENERIC以方便用户使用，用户可以直接使用它而不必增加新的协议类型。前面讲到，为了增加新的netlink协议类型，用户仅需增 加如下定义到linux/netlink.h就可以：
```
#define NETLINK_MYTEST  17
```
只要增加这个定义之后，用户就可以在内核的任何地方引用该协议。
在内核中，为了创建一个netlink socket用户需要调用如下函数：
```
struct sock *
netlink_kernel_create(int unit, void (*input)(struct sock *sk, int len));
```
参数unit表示netlink协议类型，如NETLINK_MYTEST，参数input则为内核模块定义的netlink消息处理函 数，当有消息到达这个netlink socket时，该input函数指针就会被引用。函数指针input的参数sk实际上就是函数netlink_kernel_create返回的 struct sock指针，sock实际是socket的一个内核表示数据结构，用户态应用创建的socket在内核中也会有一个struct sock结构来表示。下面是一个input函数的示例：
```
void input (struct sock *sk, int len)
{
 struct sk_buff *skb;
 struct nlmsghdr *nlh = NULL;
 u8 *data = NULL;
 while ((skb = skb_dequeue(&sk->receive_queue)) 
       != NULL) {
 /* process netlink message pointed by skb->data */
 nlh = (struct nlmsghdr *)skb->data;
 data = NLMSG_DATA(nlh);
 /* process netlink message with header pointed by 
  * nlh and data pointed by data
  */
 }   
}
```
函数input()会在发送进程执行sendmsg()时被调用，这样处理消息比较及时，但是，如果消息特别长时，这样处理将增加系统调用 sendmsg()的执行时间，对于这种情况，可以定义一个内核线程专门负责消息接收，而函数input的工作只是唤醒该内核线程，这样sendmsg将 很快返回。
函数skb = skb_dequeue(&sk->receive_queue)用于取得socket sk的接收队列上的消息，返回为一个struct sk_buff的结构，skb->data指向实际的netlink消息。
函数skb_recv_datagram(nl_sk)也用于在netlink socket nl_sk上接收消息，与skb_dequeue的不同指出是，如果socket的接收队列上没有消息，它将导致调用进程睡眠在等待队列 nl_sk->sk_sleep，因此它必须在进程上下文使用，刚才讲的内核线程就可以采用这种方式来接收消息。
下面的函数 input就是这种使用的示例：
```
void input (struct sock *sk, int len)
{
  wake_up_interruptible(sk->sk_sleep);
}
```
当内核中发送netlink消息时，也需要设置目标地址与源地址，而且内核中消息是通过struct sk_buff来管理的， linux/netlink.h中定义了一个宏：
```
#define NETLINK_CB(skb)         (*(struct netlink_skb_parms*)&((skb)->cb))
```
来方便消息的地址设置。下面是一个消息地址设置的例子：
```
NETLINK_CB(skb).pid = 0;
NETLINK_CB(skb).dst_pid = 0;
NETLINK_CB(skb).dst_group = 1;
```
字段pid表示消息发送者进程ID，也即源地址，对于内核，它为 0， dst_pid 表示消息接收者进程 ID，也即目标地址，如果目标为组或内核，它设置为 0，否则 dst_group 表示目标组地址，如果它目标为某一进程或内核，dst_group 应当设置为 0。
在内核中，模块调用函数 netlink_unicast 来发送单播消息：
```
int netlink_unicast(struct sock *sk, struct sk_buff *skb, u32 pid, int nonblock);
```
参数sk为函数netlink_kernel_create()返回的socket，参数skb存放消息，它的data字段指向要发送的 netlink消息结构，而skb的控制块保存了消息的地址信息，前面的宏NETLINK_CB(skb)就用于方便设置该控制块， 参数pid为接收消息进程的pid，参数nonblock表示该函数是否为非阻塞，如果为1，该函数将在没有接收缓存可利用时立即返回，而如果为0，该函 数在没有接收缓存可利用时睡眠。
内核模块或子系统也可以使用函数netlink_broadcast来发送广播消息：
```
void netlink_broadcast(struct sock *sk, struct sk_buff *skb, u32 pid, u32 group, int allocation);
```
前面的三个参数与netlink_unicast相同，参数group为接收消息的多播组，该参数的每一个代表一个多播组，因此如果发送给 多个多播组，就把该参数设置为多个多播组组ID的位或。参数allocation为内核内存分配类型，一般地为GFP_ATOMIC或 GFP_KERNEL，GFP_ATOMIC用于原子的上下文（即不可以睡眠），而GFP_KERNEL用于非原子上下文。
在内核中使用 函数sock_release来释放函数netlink_kernel_create()创建的netlink socket：
```
void sock_release(struct socket * sock);
```
注意函数netlink_kernel_create()返回的类型为struct sock，因此函数sock_release应该这种调用：
```
sock_release(sk->sk_socket);
```
sk为函数netlink_kernel_create()的返回值。
NAT操作也是以netfilter节 点形式挂接在相应的处理 点上的，DNAT挂接在NF_IP_PRE_ROUTING点上，优先级高于 FILTER低于MANGLE，表示在mangle表后处理，但在filter表前处理数据包；SNAT挂接在NF_IP_POST_ROUTING点 上，优先级低于FILTER，表示在filter表后面处理数据包。
QUOTE:
由 于作NAT后要修改IP地址以及端口，因此原来的主连接中描述子连接的信息必须进行修改，(*help) 函数的功能就要要找到一个空闲的tuple对应新的子连接，修改期待的子连接，然后修改主连接的通信内容，修改关于IP地址和端口部分的描述信息为空闲 tuple的信息，由于修改了应用层数据，数据的校验和必须重新计算，而且如果数据长度发生变化，会引起TCP序列号的变化，在连接的协议相关数据中会记 录这些变化，对后续的所有数据都要进行相应的调整；该函数在do_bindings()函数中调用；
不是太清楚这段话。
感谢**独孤** 和**Godbach** 大哥们的精彩帖子，给我做毕业设计带了帮助！！
虽然目前差不多实现了一个多线程的ip数据包查看和ip简单过滤功能。 
但对nl(netlink通讯）还有一点疑问。 
下面也总结下我遇到的问题，也分享给大家。 
**Filter实现 **
**先说说大致思路： **
我用的JAVA SWING 做的UI 然后底层就是SO库和内核进行通讯了，用到就是NETLINK 和NETFILTER那套东西。
大致流程就不说了，上面两位大哥们还有很多前辈们都说明白了。而我只是运用到了实际的一个TOOL下面。自己的内核是2.6.25 的（和以前版本有点出处）。
______________________________________ 
java: 
Thread A   Thread B 
           /         / 
            /       / 
             /___/
so库:     static int skfd;(上层线程共用一个sock链接） 
                 ｜ 
      receive()｜send() 
                  ｜ 
内核层: 
         cj_fw.ko模块 
so库: static int skfd;(上层线程共用一个sock链接） 
－？疑问一： 我有尝试创建多个sock,但每次bind()失败，难道不能同时创建多个sock与内 核的一个sock通讯么？ 不能有“多对一”的关系么？ 
这个sock链接是绑定了nl协议的（skfd = socket(AF_NETLINK, SOCK_RAW, NL_FW);//NL_FW是自定义协议 
创建NETLINK连接 
struct sockaddr_nl local;//表示用户进程的nl地址 
struct sockaddr_nl kpeer;//表示内核进程的nl地址  
详细代码： 
**//初始化用户本地nl地址     memset(&local, 0, sizeof(local));     local.nl_family = AF_NETLINK;//设置环境     local.nl_pid =  pthread_self() << 16 | getpid();//设置通讯id     local.nl_groups = 0;//是否指定多播      //用于把一个打开的 netlink socket 与 netlink 源 socket 地址绑定在一起     if(bind(skfd, (struct sockaddr*)&local, sizeof(local)) != 0)       {         printf("bind() error in ip/n");         return 0;       }     //初始化内核进程nl地址     memset(&kpeer, 0, sizeof(kpeer));     kpeer.nl_family = AF_NETLINK;     kpeer.nl_pid = 0;  //0表示内核进程     kpeer.nl_groups = 0;  //初始化用户本地nl地址   memset(&local, 0, sizeof(local));   local.nl_family = AF_NETLINK;//设置环境   local.nl_pid =  pthread_self() << 16 | getpid();//设置通讯id   local.nl_groups = 0;//是否指定多播    //用于把一个打开的 netlink socket 与 netlink 源 socket 地址绑定在一起   if(bind(skfd, (struct sockaddr*)&local, sizeof(local)) != 0)     {       printf("bind() error in ip/n");       return 0;     }   //初始化内核进程nl地址   memset(&kpeer, 0, sizeof(kpeer));   kpeer.nl_family = AF_NETLINK;   kpeer.nl_pid = 0;  //0表示内核进程   kpeer.nl_groups = 0;**
发送消息给内核: 
**sendto(skfd, &message, message.hdr.nlmsg_len, 0, (struct sockaddr *)(&kpeer),                 sizeof(kpeer));  sendto(skfd, &message, message.hdr.nlmsg_len, 0, (struct sockaddr *)(&kpeer),                          sizeof(kpeer));**
接收内核的消息： 
**rcvlen = recvfrom(skfd, &info, sizeof(struct u_packet_info),           0, (struct sockaddr*)&kpeer, &kpeerlen);     rcvlen = recvfrom(skfd, &info, sizeof(struct u_packet_info),                                0, (struct sockaddr*)&kpeer, &kpeerlen); **
_______________________________________________- 
内核层: 
         cj_fw.ko模块 
详细注册代码： 
**struct sock *nlfd=NULL; static int  init(void)   {      //创建一个netlink     nlfd = netlink_kernel_create(&init_net,NL_FW,0, kernel_receive,NULL,THIS_MODULE);     if(!nlfd)       {         printk("can not create a netlink socket/n");       if (nlfd){               sock_release(nlfd->sk_socket);           }         return -1;       }   ....   }  static int  init(void){   //创建一个netlink  nlfd = netlink_kernel_create(&init_net,NL_FW,0, kernel_receive,NULL,THIS_MODULE);  if(!nlfd)    {      printk("can not create a netlink socket/n");        if (nlfd){                    sock_release(nlfd->sk_socket);                  }      return -1;    }....}**
接收用户消息: 
Java代码 
static void kernel_receive(struct sk_buff * __skb)  
static void kernel_receive(struct sk_buff * __skb)
发送用户消息： 
Java代码 
static int send_to_user(struct packet_info *info)  
static int send_to_user(struct packet_info *info)
-------------------------?疑问二： 一个模块只能创建一个netlink sock吗？我尝试过创建2个以上，但失败。具体原因又是什么呢？ 
**小结和问题： **
目前我觉得多线程共用一个sock和内核一个sock通讯的效率不高，内核模块稍微处理不好，就发生堵塞，所有要注意的地方是不管网络层是否有网络数据 包，内核模块都需要即使返回消息给用户态，这样才不会让用户态无限等下去.. 
但发生堵塞的原因还是共用了一个sock，不知道大家有什么改进的方法么？ 
**进度：**
   然后目前在看Godbach 大哥写的那篇“连接状态跟踪分析”了，目前自己也还在理解阶段。
   对于4个HOOK点分别对应了 ip_conntrack_in_ops ip_conntrack_local_out_ops  ip_conntrack_out_ops ip_conntrack_local_in_ops  和 4个 HOOK函数：ip_conntrack_in，ip_conntrack_local，ip_refrag， ip_confirm  
   最后，希望和大家多多交流！
已实现过滤功能如下：
**连接跟踪实现： **
貌似现在的思路越来越明朗了：
关于链接跟踪
**1，通过"proc"与nf_conntrack内核模块通讯，获取连接信息**
**2，通过之前自己已实现ip包过滤模块的hook,来获得ip头，tcp,udp头信息并通过netlink获取连接信息**
   类似“ipv4     2 tcp      6 5 TIME_WAIT src=192.168.0.25 dst=221.238.249.178 sport=39027 dport=80 packets=6 bytes=1119 src=221.238.249.178 dst=192.168.0.25 sport=80 dport=39027 packets=5 bytes=2474 [ASSURED] mark=0 secmark=0 use=1"
只是里面的bytes  mark secmark use  TIME_WAIT 这几个信息不知道是从哪来的??? 
ip包消息头：
struct iphdr { 
# if defined( __LITTLE_ENDIAN_BITFIELD) 
    __u8    ihl: 4, 
        version: 4; 
# elif defined ( __BIG_ENDIAN_BITFIELD) 
    __u8    version: 4, 
          ihl: 4; 
# else 
# error     "Please fix <asm/byteorder.h>" 
# endif 
    __u8    tos; 
    __be16    tot_len; 
    __be16    id; 
    __be16    frag_off; 
    __u8    ttl; 
    __u8    protocol;   __u8是unsigned char类型
    __sum16    check; 
    __be32    saddr; 
    __be32    daddr; 
    /*The options start here. */ 
} ; 
struct udphdr { 
    __be16    source; 
    __be16    dest; 
    __be16    len; 
    __sum16    check; 
} ; 
struct tcphdr { 
    __be16    source; 
    __be16    dest; 
    __be32    seq; 
    __be32    ack_seq; 
# if defined( __LITTLE_ENDIAN_BITFIELD) 
    __u16    res1: 4, 
        doff: 4, 
        fin: 1, 
        syn: 1, 
        rst: 1, 
        psh: 1, 
        ack: 1, 
        urg: 1, 
        ece: 1, 
        cwr: 1; 
# elif defined( __BIG_ENDIAN_BITFIELD) 
    __u16    doff: 4, 
        res1: 4, 
        cwr: 1, 
        ece: 1, 
        urg: 1,     
        ack: 1, 
        psh: 1, 
        rst: 1, 
        syn: 1, 
        fin: 1; 
# else 
# error     "Adjust your <asm/byteorder.h> defines" 
# endif     
    __be16    window; 
    __sum16    check; 
    __be16    urg_ptr; 
} ;
**3http://www.cublog.cn/u3/97568/showart_2085011.html**
**PF_NETLINK应用实例 NETLINK_KOBJECT_UEVENT具体实现－－udev实现原理－－－－－**
**有些地方有错误的，不过这个很接近android的init实现**
udev实现原理 
　　转载时请注明出处和作者联系方式：http://blog.csdn.net/absurd 
　　 作者联系方式：李先静 
　　更新时 间：2007-4-29 
　　相对于linux来说，udev还是一个新事物。然而，尽管它03年才出现，尽管它很低调(J)，但 它无疑已经成为linux下不可或缺的组件了。udev是什么？它是如何实现的？最近研究Linux设备管理时，花了一些时间去研究udev的实现。 
　 　udev是什么？u 是指user space，dev是指device，udev是用户空间的设备驱动程序吗？最初我也这样认为，调试内核空间的程序要比调试用户空间的程序复杂得多，内核 空间的程序的BUG所引起的后果也严重得多，device driver是内核空间中所占比较最大的代码，如果把这些device driver中硬件无关的代码，从内核空间移动到用户空间，自然是一个不错的想法。 
　　但我的想法并不正确，udev的文档是这 样说的， 
　 　1. dynamic replacement for /dev。作为devfs的替代者，传统的devfs不能动态分配major和minor的值，而major和minor非常有限，很快就会用完了。 udev能够像DHCP动态分配IP地址一样去动态分配major和minor。 
　　2. device naming。提供设备命名持久化的机制。传统设备命名方式不具直观性，像/dev/hda1这样的名字肯定没有boot_disk这样的名字直观。 udev能够像DNS解析域名一样去给设备指定一个有意义的名称。 
　　3. API to access info about current system devices 。提供了一组易用的API去操作sysfs，避免重复实现同样的代码，这没有什么好说的。 
　　我们知道，用户空间的程序与设备通信的方法，主要有以下几种方式， 
　　1. 通过ioperm获取操作IO端口的权限，然后用inb/inw/ inl/ outb/outw/outl等函数，避开设备驱动程序，直接去操作IO端口。（没有用过） 
　　2. 用ioctl函数去操作/dev目录下对应的设备，这是设备驱动程序提供的接口。像键盘、鼠标和触摸屏等输入设备一般都是这样做的。 
　　3. 用write/read/mmap去操作/dev目录下对应的设备，这也是设备驱动程序提供的接口。像framebuffer等都是这样做的。 
　　上面的方法在大多数情况下，都可以正常工作，但是对于热插拨(hotplug)的设备，比如像U盘，就有点困难了，因为你不知道：什么时 候设备插上了，什么时候设备拔掉了。这就是所谓的hotplug问题了。 
　 　处理hotplug传统的方法是，在内核中执行一个称为hotplug的程序，相关参数通过环境变量传递过来，再由hotplug通知其它关注 hotplug事件的应用程序。这样做不但效率低下，而且感觉也不那么优雅。新的方法是采用NETLINK实现的，这是一种特殊类型的socket，专门 用于内核空间与用户空间的异步通信。下面的这个简单的例子，可以监听来自内核hotplug的事件。 
　　#include 
　　#include 
　　#include 
　　#include 
　　#include 
　　#include 
　　 #include 
　　#include 
　　 #include 
　　#include 
　　 staticintinit_hotplug_sock(void) 
　　{ 
　　structsockaddr_nl snl 
　 　constintbuffersize= 16 * 1024 * 1024; 
　　intretval 
　　 memset(&snl, 0x00, sizeof(structsockaddr_nl)); 
　　snl.nl_family = AF_NETLINK; 
　　snl.nl_pid = getpid(); 
　　snl.nl_groups = 1; 
　　inthotplug_sock= socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT); 
　　if(hotplug_sock== -1) { 
　　 printf("error getting socket: %s", strerror(errno)); 
　　return-1; 
　 　} 
　　/* set receive buffersize */ 
　　 setsockopt(hotplug_sock, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize)); 
　　retval= bind(hotplug_sock, (structsockaddr*) &snl, sizeof(structsockaddr_nl)); 
　　 if(retval<0) { 
　　printf("bind failed: %s", strerror(errno)); 
　 　close(hotplug_sock); 
　　hotplug_sock= -1; 
　　return-1; 
　　} 
　　returnhotplug_sock 
　　} 
　　#define UEVENT_BUFFER_SIZE 2048 
　　intmain(intargc, char* argv[]) 
　 　{ 
　　inthotplug_sock = init_hotplug_sock(); 
　　while(1) 
　 　{ 
　　charbuf[UEVENT_BUFFER_SIZE*2] = {0}; 
　　recv(hotplug_sock, &buf, sizeof(buf), 0); 
　　printf("%s/n", buf); 
　　} 
　 　return0; 
　　} 
　　编译： 
　　gcc -g hotplug.c -o hotplug_monitor 
　　运行后插/拔U盘，可以看到： 
　　 add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1 
　　 add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/usbdev2.2_ep00 
　　 add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0 
　　 add@/class/scsi_host/host2 
　　 add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep81 
　 　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep02 
　 　add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep83 
　 　add@/class/usb_device/usbdev2.2 
　　 add@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/host2/target2:0:0/2:0:0:0 
　　add@/class/scsi_disk/2:0:0:0 
　　add@/block/sda 
　　 add@/block/sda/sda1 
　　add@/class/scsi_device/2:0:0:0 
　　 add@/class/scsi_generic/sg0 
　　 remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep81 
　 　remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep02 
　　 remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/usbdev2.2_ep83 
　 　remove@/class/scsi_generic/sg0 
　　remove@/class/scsi_device/2:0:0:0 
　 　remove@/class/scsi_disk/2:0:0:0 
　　remove@/block/sda/sda1 
　　 remove@/block/sda 
　　 remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0/host2/target2:0:0/2:0:0:0 
　　remove@/class/scsi_host/host2 
　　 remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/2-1:1.0 
　　 remove@/class/usb_device/usbdev2.2 
　　 remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1/usbdev2.2_ep00 
　　 remove@/devices/pci0000:00/0000:00:1d.1/usb2/2-1 
　　udev的主体部分在 udevd.c文件中，它主要监控来自4个文件描述符的事件/消息，并做出处理： 
　　1. 来自客户端的控制消息。这通常由udevcontrol命令通过地址为/org/kernel/udev/udevd的本地socket，向udevd发 送的控制消息。其中消息类型有： 
　　l UDEVD_CTRL_STOP_EXEC_QUEUE 停止处理消息队列。 
　　l UDEVD_CTRL_START_EXEC_QUEUE 开始处理消息队列。 
　　l UDEVD_CTRL_SET_LOG_LEVEL 设置LOG的级别。 
　　l UDEVD_CTRL_SET_MAX_CHILDS 设置最大子进程数限制。好像没有用。 
　　l UDEVD_CTRL_SET_MAX_CHILDS_RUNNING 设置最大运行子进程数限制(遍历proc目录下所有进程，根据session的值判断)。 
　　l UDEVD_CTRL_RELOAD_RULES 重新加载配置文件。 
　 　2. 来自内核的hotplug事件。如果有事件来源于hotplug，它读取该事件，创建一个udevd_uevent_msg对象，记录当前的消息序列号， 设置消息的状态为EVENT_QUEUED,然后并放入running_list和exec_list两个队列中，稍后再进行处理。 
　　3. 来自signal handler中的事件。signal handler是异步执行的，即使有signal产生，主进程的select并不会唤醒，为了唤醒主进程的select，它建立了一个管道，在 signal handler中，向该管道写入长度为1个子节的数据，这样就可以唤醒主进程的select了。 
　　4. 来自配置文件变化的事件。udev通过文件系统inotify功能，监控其配置文件目录/etc/udev/rules.d，一旦该目录中文件有变化，它 就重新加载配置文件。 
　　其中最主要的事件，当然是来自内核的hotplug事件，如何处理这些事件是udev的关键。udev 本身并不知道如何处理这些事件，也没有必要知道，因为它只实现机制，而不实现策略。事件的处理是由配置文件决定的，这些配置文件即所谓的rule。 
　　关于rule的编写方法可以参考《writing_udev_rules》，udev_rules.c实现了对规则的解析。 
　　在规则中，可以让外部应用程序处理某个事件，这有两种方式，一种是直接执行命令，通常是让modprobe去加载驱动程序，或者让 mount去加载分区。另外一种是通过本地socket发送消息给某个应用程序。 
　　在 udevd.c:udev_event_process函数中，我们可以看到，如果RUN参数以”socket:”开头则认为是发到socket，否则认 为是执行指定的程序。 
　　下面的规则是执行指定程序： 
　　60-pcmcia.rules: RUN+="/sbin/modprobe pcmcia" 
　　下面的规则是通过socket发送消息： 
　　 90-hal.rules:RUN+="socket:/org/freedesktop/hal/udev_event" 
　　 hal正是我们下一步要关心的，接下来我会分析HAL的实现原理。
4http://blog.chinaunix.net/u1/38994/showart_1288259.html
**浅析frmware的加载和init通过 netlink处理uevent事件的一般流程**
**这个对调试linux kernel并需要用户空间提供数据支持时有启发意义**
```
浅析frmware的加载和init通过netlink处理 uevent事件的一般流程
当总线检测代id相macth的设备或者驱动时调用, wlan_probe
= > wlan_probe
= > wlan_add_card
= > sbi_register_dev
= > priv- > hotplug_device = & func- > dev; 这样priv- > hotplug_device就指向了/ sys/ bus/ sdio/devices下的设备节点描述结构体
接下来下载wlan的firmware固件驱动, 
= > wlan_init_fw
= > request_firmware( & priv- > firmware, fw_name, priv- > hotplug_device) ; 给priv- >hotplug_device 设备申请名字为fw_name的firmware
  数据, 让后将结果放到& priv- > firmware 中, 
  struct firmware { 
    size_t size; 
    u8 * data; 
  } ; 
  可以看到, 如果应用层的程序成功load了 firmware固件文件, 那么firmware. data将指向固件数据,firmware. size 为固件大小. 
module_param( fw_name, charp, 0) ; 
MODULE_PARM_DESC( fw_name, "Firmware name" ) ; 这里可以看到fw_name是作为参数可以自由指定的, 如果没有指定, 那么将使用如下默认名：
# define DEFAULT_FW_NAME "mrvl/sd8688.bin" 
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = == = = = = = = = = = = = = = = 
request_firmware
= > _request_firmware
= > fw_setup_device
= > fw_register_device//添加临时download firmware的dev设备节点,注册该inode的class为 
= > fw_priv- > attr_data = firmware_attr_data_tmpl; //文件属性操作 函数集 
= > strlcpy( fw_priv- > fw_id, fw_name, FIRMWARE_NAME_MAX) ; //拷贝参数就是我们的fw_name 即默认的"mrvl/sd8688.bin" 
= > sysfs_create_bin_file( & f_dev- > kobj, & fw_priv- > attr_data) ; //创建DEVPATH/data文 件,操作该文件的方法为firmware_attr_data_tmpl 
//这样init程序就可以打开这个DEVPATH/data文件,然 后向这个DEVPATH/data文件写入firmware固件bin内容,然后kernel的driver就可以通过 
//firmware->data和 firmware->size来读取有uevent处理程序init加载进来的firmware数据了[luther.gliethttp] 
= > f_dev- > class = & firmware_class; 
= > if ( uevent) kobject_uevent( & f_dev- > kobj, KOBJ_ADD) ; 发送 uevent消息. 
= > wait_for_completion( & fw_priv- > completion) ; 等待完成
static struct bin_attribute firmware_attr_data_tmpl = { 
    . attr = { . name = "data" , . mode = 0644} , 
    . size = 0, 
    . read = firmware_data_read, 
    . write = firmware_data_write, 
} ; 
= > firmware_data_write
= > fw_realloc_buffer//类似realloc实现,释放原有的,申请新加的 
= > fw_priv- > fw- > data = new_data; 
= > kobject_uevent
= > kobject_uevent_env
/*
devices_init
=>devices_kset = kset_create_and_add("devices", &device_uevent_ops, NULL);
static struct kset_uevent_ops device_uevent_ops = {
    .filter =    dev_uevent_filter,
    .name =        dev_uevent_name,
    .uevent =    dev_uevent,
};
=>dev_uevent
=>dev->class->dev_uevent
void device_initialize(struct device *dev)
{
    dev->kobj.kset = devices_kset;
    ...
}
*/ 
= > uevent_ops = kset- > uevent_ops; //这里uevent_ops就指向 device_uevent_ops了 
= > uevent_ops- > uevent( kset, kobj, env) ; 建立环境变量
= > device_uevent_ops
= > dev_uevent
= > dev- > class - > dev_uevent就是firmware_class的 dev_uevent, 即：firmware_uevent
= > firmware_uevent
= > 
/*
static int firmware_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    struct firmware_priv *fw_priv = dev_get_drvdata(dev);
    if (add_uevent_var(env, "FIRMWARE=%s", fw_priv->fw_id))即默认的"mrvl/sd8688.bin"
        return -ENOMEM;
    if (add_uevent_var(env, "TIMEOUT=%i", loading_timeout))
        return -ENOMEM;
    return 0;
}
*/ 
= > if ( uevent_sock) { 那么通过netlink将该uevent事件广播出去, 
= > netlink_broadcast( uevent_sock, skb, 0, 1, GFP_KERNEL) ; 
= > 在include/ linux/ autoconf. h中
# define CONFIG_UEVENT_HELPER_PATH "/sbin/hotplug" 
= > 但是我们的root文件系统/ sbin/ 下没有hotplug这个文件, 只有一个adbd程序, 所以这样看来就不能运行了, 所以这里应该将char uevent_helper[ UEVENT_HELPER_PATH_LEN] = 清0才对, 让if( uevent_helper[ 0] )失败, 进而不继续执行和 hotplug的相关操作, 但是现在autoconf. h中
include / linux/ autoconf. h
# define CONFIG_NET 1
# define CONFIG_HOTPLUG 1 
仍然被设置成1, 不知道为什么kernel team还要这样设置[ luther. gliethttp] . 
. 
所以我们的系统使用的是netlink接收事件广播, 我的 ubuntu8. 04的/ sbin下也没有找到hotplug这个文件, 可能正如大家所说的, hotplug的诸多
缺陷导致它已经淡出了linux世界, 而天生丽质的 netlink已经在linx世界中全面开花[ luther. gliethttp] . 
然后init进程开始处理这个firmware请求, 
init
= > main
= > handle_device_fd调用uevent的NETLINK_KOBJECT_UEVENT的 socket处理函数
= > parse_event
= > handle_firmware_event
= > pid = fork( ) ; 子进 程执行process_firmware_event
= > process_firmware_event
# define SYSFS_PREFIX "/sys" 
= > asprintf( & root, SYSFS_PREFIX"%s/" , uevent- > path) ; 
//这里的uevent->path是parse_event函 数解析时对应的"DEVPATH="节内容,也就是dev设备路径
= > asprintf( & loading, "%sloading" , root) ; //在该路径下创建loading文件
= > asprintf( & data, "%sdata" , root) ; //该路径下的data文件
= > loading_fd = open ( loading, O_WRONLY) ; //创建该loading文件,然后向其中写入"1"表示开始加载,加载成功写入"0",失败写入"-1".
= > data_fd = open ( data, O_WRONLY
# define FIRMWARE_DIR "/system/lib/firmware" 原来路径是/ etc/ firmware, 我的 mrvl/ sd8688. bin也放在那里, 
//但是虽然ramdisk虽然经过压缩,可是存储 ramdisk.img的总大小才512k,所以不能将有可能不断扩大大小的firmware放到那里,
//于是最近将init进程搜索路径改为" /system/lib/firmware".
= > asprintf( & file , FIRMWARE_DIR"/%s" , uevent- > firmware) ; 
= > fw_fd = open ( file , O_RDONLY) ; //打开通过uevent传递过来的firmware文件,然后拷贝过去
= > load_firmware( fw_fd, loading_fd, data_fd) ) 这样加载
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = == = = = = = = = = = = = = = = 
# define module_param_named( name, value, type, perm)              /
    param_check_# # type( name, & ( value) ) ;                  /
    module_param_call( name, param_set_# # type, param_get_# # type, & value, perm) ; /
    __MODULE_PARM_TYPE( name, # type) 
# define module_param( name, type, perm)                 /
    module_param_named( name, name, type, perm) 
//对应param_check_##type检测参数类型函数如下, 可以检测如下参数类型[luther.gliehttp]
param_check_bool param_check_int param_check_short param_check_ushort
param_check_byte param_check_invbool param_check_uint param_check_proto_abbrev
param_check_charp param_check_long param_check_ulong param_check_scroll
= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = == = = = = = = = = = = = = = = 
比如usb通用驱动的使用, 这样所有usb设备都可以通过 serial方式进行访问, 因为不是该usb设备专有的 usb驱动, 所以速度可能慢一些[ luther. gliethttp] . 
sudo insmod / lib/ modules/ 2. 6. 22- 14- generic/ kernel/ drivers/ usb/ serial/ usbserial. ko vendor= 0x8086 product= 0xd001
在drivers/ usb/ serial/ generic. c驱 动中, 
module_param( vendor, ushort, 0) ; 
MODULE_PARM_DESC( vendor, "User specified USB idVendor" ) ; 
module_param( product, ushort, 0) ; 
MODULE_PARM_DESC( product, "User specified USB idProduct" ) ; 
所以通过module_param可以方便的给ko驱动传递参数, 很 方便的咚咚, kernel那群人真能整[ luther.gliethttp] .
```
转自：[http://blog.csdn.net/linphusen/article/details/5667647](http://blog.csdn.net/linphusen/article/details/5667647)
