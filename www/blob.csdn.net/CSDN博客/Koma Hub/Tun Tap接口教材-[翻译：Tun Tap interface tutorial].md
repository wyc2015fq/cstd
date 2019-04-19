# Tun/Tap接口教材-[翻译：Tun/Tap interface tutorial] - Koma Hub - CSDN博客
2019年01月27日 12:53:02[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：77
个人分类：[TUN/TAP																[计算机网络](https://blog.csdn.net/Rong_Toa/article/category/7569489)](https://blog.csdn.net/Rong_Toa/article/category/8644578)
英文原文：[https://backreference.org/2010/03/26/tuntap-interface-tutorial/](https://backreference.org/2010/03/26/tuntap-interface-tutorial/)
翻译原文：[https://www.jianshu.com/p/6a2e5a0cceb2](https://www.jianshu.com/p/6a2e5a0cceb2)
**目录**
[工作原理](#%E5%B7%A5%E4%BD%9C%E5%8E%9F%E7%90%86)
[创建虚拟接口](#%E5%88%9B%E5%BB%BA%E8%99%9A%E6%8B%9F%E6%8E%A5%E5%8F%A3)
[使用](#%E4%BD%BF%E7%94%A8)
[一个简单的程序](#%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E7%A8%8B%E5%BA%8F)
[Tunnels](#Tunnels)
[Extensions and improvements](#Extensions%20and%20improvements)
*前言：请注意，这里提供的代码仅用于演示目的。 如果你想要认真，你必须使它更健壮，并与其他代码整合。 而且，这个描述绝不是关于这个问题的明确的参考，而是我实验的结果。 请报告您在代码或本文中找到的任何bug或错误。 谢谢。*
链接到文章中描述的源码tarball：[simpletun](http://backreference.org/wp-content/uploads/2010/03/simpletun.tar.bz2)。
**18/07/2010更新**：感谢[这篇文章](http://blog.bofh.it/debian/id_379)，现在我已经知道了最新版本的iproute2可以（终于）创建tun/tap设备，尽管到现在都没有完整的说明文档。再也不用为了创建tun设备，而安装tunctl（UML utilities）或OpenVPN了。以下是iproute2-2.6.34：
```
# ip tuntap help
Usage: ip tuntap { add | del } [ dev PHYS_DEV ] 
          [ mode { tun | tap } ] [ user USER ] [ group GROUP ]
          [ one_queue ] [ pi ] [ vnet_hdr ]
Where: USER  := { STRING | NUMBER }
       GROUP := { STRING | NUMBER }
```
**Tun/tap interfaces**是Linux（也可能是其他类UNIX操作系统）提供的一种功能，可以执行*userspace networking*，也就是允许userspace程序查看原始网络流量（在ethernet或IP level）并像它一样执行任何操作。本文档试图解释在Linux下tun/tap接口是如何工作的，以及一些示例代码来演示它们的用法。
# 工作原理
Tun/tap interfaces是*software-only interfaces*，这意味着它们只存在于内核中，与常规网络接口不同，它们没有物理硬件组件（因此没有物理“线”连接到它们）。当内核决定发送数据在这个接口（而不是发送到接口的某个用户空间程序），tun/tap就像一个普通的网络接口一样（使用具体程序见下文）。当程序连接到tun/tap接口时，它会得到一个特殊的文件描述符，从中读取接口发送的数据。以类似的方式，程序可以写入这个特殊的描述符，数据（必须正确格式化，我们将会看到）将作为tun/tap接口的输入。对于内核来说，看起来像tun/tap接口正在从“wire”接收数据。
![](https://img-blog.csdnimg.cn/20190127125046303.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
tap接口和tun接口的区别在于tap接口输出或接收完整的以太网帧，而tun接口输出或接收原始IP数据包（并且内核没有添加以太网报头）。在创建接口时，用一个标志指定接口的功能是像tun接口还是像tap接口一样。
接口可以是**暂时的**，也就是说它是由同一个程序创建，使用和销毁的; 当程序终止时，即使不明确销毁接口，接口也不复存在。 另一个选择（我更喜欢）是使接口**持久化**; 在这种情况下，专用程序（如tunctl或openvpn --mktun）创建它，正常的程序也可以附加其上; 当他们这样做的时候，他们必须使用相同类型（tun或tap）来创建接口进行连接，否则他们将无法连接。 我们将看到代码中是如何完成的。
一旦tun/tap接口就位，它可以像其他任何接口一样使用，这意味着可以分配IP地址，分析其流量，创建防火墙规则，建立指向它的路由等。
有了这些知识，让我们试着看看我们如何使用tun/tap接口以及如何使用它。
# 创建虚拟接口
创建全新interface和重新连接到持久interface的代码本质上是相同的; 不同之处在于前者必须以root身份运行（更确切地说，具有**CAP_NET_ADMIN**功能的用户），而后者则可以由普通用户在满足某些条件的情况下运行。 我们先从创建一个新的interface开始。
首先，设备**/dev/net/tun**必须以read/write的方式打开。 该设备也被称为**克隆设备**，它是创建任何tun/tap虚拟接口的起点。 操作（与任何open()调用一样）返回一个文件描述符。 但是这还不足以开始使用它与界面进行通信。
创建接口的下一步是使用一个特殊的**ioctl()**系统调用，它的参数包含上一步获得的文件描述符、**TUNSETIFF**常量，以及一个指向包含描述虚拟接口（基本上是 名称和所需的操作模式 - tun/tap）的结构体。 作为一种变体，虚拟接口的名称可以不指定，在这种情况下，内核将通过按序分配来为设备选择一个名称（例如，如果tap2已经存在，内核将尝试 分配tap3，等等）。 所有这些都必须由root用户来完成（或者由具有CAP_NET_ADMIN功能的用户来完成 - 我不会再重复一遍;假设它适用于我所说的“必须由root运行”）。
如果ioctl()成功，则创建虚拟接口，并且我们所拥有的文件描述符现在与其关联，并可用于通信。
针对接口有两种使用情形。 该程序可以立即开始使用接口（可能在此之前至少配置一个IP地址），当程序完成时，终止并销毁接口。 另一个选择是发出一些其他特殊的ioctl()调用来使接口持久化，停止的时候将其留在其他程序附加的地方。 例如，这就是像`tunctl`或`openvpn --mktun`这样的程序。 这些程序通常还可以选择将虚拟接口的所有权设置为非root用户和/或组，所以以非root用户身份运行但具有适当权限的程序可以稍后附加到接口。 我们将回到这个下面。
用于创建虚拟接口的基本代码显示在内核源代码树的**Documentation/networking/tuntap.txt**文件中。 修改一下，我们可以编写一个准系统函数来创建一个虚拟接口：
```cpp
#include <linux /if.h>
#include <linux /if_tun.h>
int tun_alloc(char *dev, int flags) {
  struct ifreq ifr;
  int fd, err;
  char *clonedev = "/dev/net/tun";
  /* 函数采用的参数：
   *
   * char *dev: 接口的名称（或 '\0'）。 必须有足够的空间来保存接口名称
   * int flags: 接口标志 (eg, IFF_TUN etc.)
   */
   /* open克隆设备 */
   if( (fd = open(clonedev, O_RDWR)) < 0 ) {
     return fd;
   }
   /* 初始化结构ifr或类型"struct ifreq */
   memset(&ifr, 0, sizeof(ifr));
   ifr.ifr_flags = flags;   /* IFF_TUN或IFF_TAP，也可能是IFF_NO_PI */
   if (*dev) {
     /* 如果指定了设备名称，则将其放入结构中；
      * 否则，内核将尝试分配指定类型的"next"设备
      */
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);
   }
   /* 尝试创建设备 */
   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
     close(fd);
     return err;
   }
  /* 如果创建成功，则将接口的名称写回变量“dev”，以便调用者知道它。
   * 请注意，调用者必须在* dev中保留空间（请参阅下面的调用代码）
   */
  strcpy(dev, ifr.ifr_name);
  /* 调用者用来与虚拟接口连接的特殊文件描述符
   */
  return fd;
}
```
**tun_alloc()**函数有两个参数：
- **char * dev**包含接口的名称（例如，tap0，tun2等）。任何名字都可以使用，但是最好选择一个名字来表明它是哪种类型的接口。实际上，通常使用像tunX或tapX这样的名称。如果`*dev`是`'\ 0'`，内核将尝试创建所需类型的“第一个”可用接口（例如，tap0，但是如果已经存在的话，tap1等等）。
- **int flags**包含通知内核我们需要哪种接口的标志（tun或tap）。基本上，它可以取值为**IFF_TUN**来指示一个TUN设备（数据包中没有以太网报头），或者**IFF_TAP**来指示一个TAP设备（数据包中有以太网报头）。
	另外，另一个标志**IFF_NO_PI**可以与基值进行或运算。 IFF_NO_PI告诉内核不提供数据包信息。 IFF_NO_PI的目的是告诉内核，数据包将是“纯”的IP数据包，没有添加字节。否则（如果IFF_NO_PI未设置），将在数据包的开头添加4个额外的字节（2个标志字节和2个协议字节）。 IFF_NO_PI不需要在接口创建和重新连接时间之间匹配。另外请注意，当使用Wireshark捕获接口上的流量时，这4个字节从不显示。
程序可以使用下面的代码来创建一个设备：
```cpp
char tun_name[IFNAMSIZ];
  char tap_name[IFNAMSIZ];
  char *a_name;
  ...
  strcpy(tun_name, "tun1");
  tunfd = tun_alloc(tun_name, IFF_TUN);  /* tun接口 */
  strcpy(tap_name, "tap44");
  tapfd = tun_alloc(tap_name, IFF_TAP);  /* tap接口 */
  a_name = malloc(IFNAMSIZ);
  a_name[0]='\0';
  tapfd = tun_alloc(a_name, IFF_TAP);    /* 内核自定义接口 */
```
如前所述，程序可以按照其用途使用接口，也可以将接口设置为持久性（并可选地将所有权分配给特定的用户/组）。 如果是前者，那就没什么好说的了。 但是如果是后者，接下来我们将看到发生了什么。
两个额外可用的ioctl()通常一起使用。第一个系统调用可以设置（或删除）接口上的持久状态。第二个允许将接口的所有权分配给常规（非root）用户。 这两个功能在程序tunctl（UML实用程序的一部分）和openvpn --mktun（可能还有其他）中实现。看看tunctl代码，它更简单，记住它只创建tap接口，因为这些是linux用户模式使用的（为了清晰起见，代码被稍微编辑和简化）
```cpp
...
  /* 如果用户想要删除（使其非持久化）现有接口，则设置"delete";
   * 否则，用户将创建一个新的接口
   */
  if(delete) {
    /* 删除持久化状态 */
    if(ioctl(tap_fd, TUNSETPERSIST, 0) < 0){
      perror("disabling TUNSETPERSIST");
      exit(1);
    }
    printf("Set '%s' nonpersistent\n", ifr.ifr_name);
  }
  else {
    /* 在TUNSETGROUP之前模拟行为 */
    if(owner == -1 && group == -1) {
      owner = geteuid();
    }
    if(owner != -1) {
      if(ioctl(tap_fd, TUNSETOWNER, owner) < 0){
        perror("TUNSETOWNER");
        exit(1);
      }
    }
    if(group != -1) {
      if(ioctl(tap_fd, TUNSETGROUP, group) < 0){
        perror("TUNSETGROUP");
        exit(1);
      }
    }
    if(ioctl(tap_fd, TUNSETPERSIST, 1) < 0){
      perror("enabling TUNSETPERSIST");
      exit(1);
    }
    if(brief)
      printf("%s\n", ifr.ifr_name);
    else {
      printf("Set '%s' persistent and owned by", ifr.ifr_name);
      if(owner != -1)
          printf(" uid %d", owner);
      if(group != -1)
          printf(" gid %d", group);
      printf("\n");
    }
  }
  ...
```
这些额外的ioctl()s仍然必须由root运行。但是现在我们拥有的是一个特定用户拥有的持久性接口，所以作为该用户运行的进程可以成功地连接到它。
如上所述，事实证明，（重新）附加到现有tun/tap接口的代码与用于创建它的代码相同;换句话说，可以再次使用tun_alloc（）。在这样做的时候，要取得成功，必然发生三件事情：
- 该接口必须已经存在，并且由正试图连接的同一个用户所拥有（可能是持久的）
- 用户必须对**/dev/net/tun**具有读/写权限
- 提供的标志必须与用于创建接口的标志相匹配（例如，如果它是使用IFF_TUN创建的，那么在重新连接时必须使用相同的标志）
如果用户发出它指定一个已经存在的接口的名称并且他是接口的所有者，就会发生这种事，因为内核允许**TUNSETIFF** ioctl()成功。在这种情况下，不需要创建新的接口，所以普通用户可以成功执行操作。
接下来将尝试解释当调用ioctl(TUNSETIFF)时发生什么，以及内核如何区分新接口的分配请求和连接到现有接口的请求：
- 如果指定了不存在或不存在的接口名称，则表示用户正在请求分配新的接口。内核因此创建一个使用给定名称的接口（或者如果给出空名称，则选择下一个可用名称）。这只适用于由root完成的工作。
- 如果指定了现有接口的名称，则表示用户想要连接到以前分配的接口。这可以由普通用户完成，前提是：用户对克隆设备具有适当的权限，并且是接口的所有者（在创建时设置），并且指定模式（tun或tap）与创建时设置的模式相匹配时间。
您可以看看在内核源文件drivers/net/tun.c中实现上述步骤的代码;重要的函数是**tun_attach()**，**tun_net_init()**，**tun_set_iff()**，**tun_chr_ioctl()*；这最后一个函数还实现了各种可用的ioctl()，包括TUNSETIFF，TUNSETPERSIST，TUNSETOWNER，TUNSETGROUP等。
在任何情况下，都不允许非root用户配置接口（即分配一个IP地址并启动它），但在任何常规接口上也是如此。如果非root用户需要执行某些需要root权限的操作，则可以使用通常的方法（suid二进制包装，sudo等）。
这是一个可能的使用场景（我一直在使用）：
- 虚拟接口被创建，持久化，分配给一个用户，并由root配置（例如，在启动时通过initscripts，使用tunctl或等价物）
- 然后，普通用户可以根据自己的虚拟接口随意连接和分离。
- 虚拟接口被root破坏，例如在关机时运行的脚本，可能使用tunctl -d或等价的
# 使用
经过这么漫长但必要的介绍，是时候尝试使用它了。 所以，由于这是一个普通的接口，我们可以像使用另一个常规接口一样使用它。 对于我们的目的，tun和tap接口没有区别; 创建或附加到它的程序，必须知道它的类型，并相应地期望或写入数据。 我们创建一个持久接口并为其分配一个IP地址：
```
# openvpn --mktun --dev tun2
Fri Mar 26 10:29:29 2010 TUN/TAP device tun2 opened
Fri Mar 26 10:29:29 2010 Persist state set to: ON
# ip link set tun2 up
# ip addr add 10.0.0.1/24 dev tun2
```
让启动一个网络分析仪，看看流量：
```
# tshark -i tun2
Running as user "root" and group "root". This could be dangerous.
Capturing on tun2
# 在另一个控制台
# ping 10.0.0.1
PING 10.0.0.1 (10.0.0.1) 56(84) bytes of data.
64 bytes from 10.0.0.1: icmp_seq=1 ttl=64 time=0.115 ms
64 bytes from 10.0.0.1: icmp_seq=2 ttl=64 time=0.105 ms
...
```
看着tshark的输出，我们看到...什么都没有。 没有流量通过接口。 这是正确的：因为我们正在ping接口的IP地址，所以操作系统正确地判定没有数据包需要“在线”发送，并且内核自己正在回应这些ping。 如果你仔细想想，如果你ping另一个接口的IP地址（例如eth0），会发生什么情况：没有数据包会被发送出去。 这听起来很明显，但起初可能是一个混乱的来源（这是对我来说）。
为一个接口分配一个 /24 IP地址将创建一个通过接口的连接路由。修改一下实验，迫使内核实际上从tun接口发出一些东西（**注意： 内核<2.6.36;后面的内核行为有所不同，如注释中所述**）：
```
# ping 10.0.0.2
PING 10.0.0.2 (10.0.0.2) 56(84) bytes of data.
From 10.0.0.1 icmp_seq=2 Destination Host Unreachable
From 10.0.0.1 icmp_seq=3 Destination Host Unreachable
...
# tshark控制台
...
  0.000000     10.0.0.1 -> 10.0.0.2     ICMP Echo (ping) request
  0.999374     10.0.0.1 -> 10.0.0.2     ICMP Echo (ping) request
  1.999055     10.0.0.1 -> 10.0.0.2     ICMP Echo (ping) request
```
现在我们终于看到了一些东西。 内核看到地址不属于本地接口，通过tun2接口存在10.0.0.0/24的路由。 所以它适时发送tun2的数据包。 请注意tun和tap接口之间的不同行为：使用tun接口，内核发送IP数据包（raw，没有其他头文件存在 - 请尝试使用tshark或wireshark进行分析），而使用tap接口作为以太网， 内核会尝试将ARP作为目标IP地址：
```
# pinging 10.0.0.2 now, but through tap2 (tap)
# ping 10.0.0.2
PING 10.0.0.2 (10.0.0.2) 56(84) bytes of data.
# tshark控制台
...
  0.111858 82:03:d4:07:62:b6 -> Broadcast    ARP Who has 10.0.0.2?  Tell 10.0.0.1
  1.111539 82:03:d4:07:62:b6 -> Broadcast    ARP Who has 10.0.0.2?  Tell 10.0.0.1
...
```
此外，通过一个tap接口，流量将由完整的以太网帧组成（也可以使用网络分析仪进行检查）。 请注意，tap接口的MAC地址是在创建接口时由内核自动生成的，但可以使用**SIOCSIFHWADDR** ioctl()进行更改（再次查看**drivers/net/tun.c**，函数**tun_chr_ioctl()**）。 最后，作为以太网接口，MTU被设置为1500：
```
# ip link show dev tap2
7: tap2:  mtu 1500 qdisc pfifo_fast state UNKNOWN qlen 500
    link/ether 82:03:d4:07:62:b6 brd ff:ff:ff:ff:ff:ff
```
当然，到目前为止，程序没有附加到接口，所有这些传出的数据包都丢失了。 所以让我们先行一步，写一个简单的程序，连接到接口并读取内核发出的数据包。
# 一个简单的程序
我们将编写一个附加到tun接口的程序，并读取内核发送到该接口的数据包。 请记住，如果接口是持久性的，则可以以普通用户身份运行该程序，前提是您拥有克隆设备**/dev/net/tun**所需的权限、是该接口的所有者、并选择正确的模式（tun或者tap）接口。 该程序实际上是一个框架，或者说是一个框架的开始，因为我们只演示如何从设备读取数据，并且只能解释程序获取数据后可以执行的操作。 我们假设我们之前定义的**tun_alloc()**函数可用于程序。 这里是代码：
```cpp
...
  /* tunclient.c */
  char tun_name[IFNAMSIZ];
  
  /* 连接到设备 */
  strcpy(tun_name, "tun77");
  tun_fd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI);  /* tun接口 */
  if(tun_fd < 0){
    perror("Allocating interface");
    exit(1);
  }
  /* 读取内核发出的数据 */
  while(1) {
    /* 请注意，“缓冲区”应至少为接口的MTU大小，例如1500字节 */
    nread = read(tun_fd,buffer,sizeof(buffer));
    if(nread < 0) {
      perror("Reading from interface");
      close(tun_fd);
      exit(1);
    }
    /* 处理数据 */
    printf("Read %d bytes from device %s\n", nread, tun_name);
  }
  ...
```
如果将tun77配置为IP地址为10.0.0.1/24，然后在尝试ping 10.0.0.2（或10.0.0.1以外的10.0.0.0/中的任何地址）时运行上述程序，则可以 从设备读取数据：
```
# openvpn --mktun --dev tun77 --user waldner
Fri Mar 26 10:48:12 2010 TUN/TAP device tun77 opened
Fri Mar 26 10:48:12 2010 Persist state set to: ON
# ip link set tun77 up
# ip addr add 10.0.0.1/24 dev tun77
# ping 10.0.0.2
...
# on another console
$ ./tunclient
Read 84 bytes from device tun77
Read 84 bytes from device tun77
...
```
如果你做数学计算，你会看到这84个字节是从哪里来的：其中20个用于IP头部，8个用于ICMP头部，56个是ICMP回显消息的有效载荷，正如你在运行ping命令时看到的那样：
```
$ ping 10.0.0.2
PING 10.0.0.2 (10.0.0.2) 56(84) bytes of data.
...
```
尝试使用上述程序通过接口发送各种流量类型（也可以尝试使用tap），并验证您正在读取的数据的大小对接口类型是否正确。每个read()返回一个完整的数据包（如果使用tap模式，则返回帧）;同样，如果我们要写，我们将不得不为每个write（）写一个完整的IP数据包（或以太网帧以tap模式）。
现在我们可以用这个数据做什么？那么，我们可以模仿我们正在阅读的交通目标的行为;再次，为了理解简单，再次使用ping示例。我们可以分析接收到的数据包，从IP头，ICMP头和有效负载中提取回复所需的信息，建立一个包含适当的ICMP回应应答消息的IP数据包，并将其发回（即写入与tun/tap设备）。这样ping的发起者实际上会收到一个答案。当然你不限于ping，所以你可以**实现各种网络协议**（读的我热血沸腾）。一般来说，这意味着解析接收到的数据包，并相应地采取行动。如果使用tap，要正确构建回复帧，您可能需要在代码中**实现ARP**。所有这一切正是[Linux用户模式](http://user-mode-linux.sourceforge.net/)所做的：它将在用户空间中运行的修改后的Linux内核连接到主机上存在的分接口，并通过该接口与主机进行通信。当然，作为一个完整的Linux内核，它确实实现了TCP/IP和以太网。较新的虚拟化平台，如[libvirt](https://libvirt.org/)，广泛使用tap接口来与支持它们的客户进行通信，如[qemu](https://wiki.qemu.org/Main_Page)/[kvm](https://www.linux-kvm.org/page/Main_Page);接口通常具有像**vnet0**，**vnet1**等名称，只有在连接到的guest虚拟机正在运行时存在，它们不会持久化，但是如果在客户端运行**ip link show**和/或**brctl show**，则可以看到它们。
以同样的方式，您可以将自己的代码附加到接口并练习网络编程或者以太网和TCP/IP堆栈实现。要开始，你可以看看（你猜对了）**drivers/net/tun.c**，函数**tun_get_user()**和**tun_put_user()**来看看在内核方面tun驱动程序是如何执行的（注意，它几乎不能抓住内核中完整网络数据包管理的表面，这非常复杂）。
# Tunnels
但是还有一件事我们可以用tun/tap接口来做。我们可以创建隧道tunnels。我们不需要重新实现TCP/IP;相反，我们可以编写一个程序，将原始数据来回传递给运行相同程序的远程主机，这种方式以镜面方式执行相同的操作。假设我们上面的程序除了连接到tun/tap接口之外，还建立到远程主机的网络连接，其中一个类似的程序（连接到本地tun/tap接口）也以服务器模式运行。 （实际上这两个程序是一样的，谁是服务器，谁是客户端是由命令行开关决定的）。一旦这两个程序正在运行，流量就可以向两个方向流动，因为代码的主体将在两个站点都做同样的事情。这里的网络连接是使用TCP实现的，但是也可以使用任何其他的意思（即UDP，甚至ICMP！）。你可以在这里下载完整的程序源代码：[simpletun](http://backreference.org/wp-content/uploads/2010/03/simpletun.tar.bz2)。
这里是程序的主循环，其中执行在tun/tap接口和网络隧道之间来回移动数据的实际工作。为了清楚起见，调试语句已被删除（您可以在源代码tarball中找到完整版本）。
```cpp
...
  /* net_fd是网络文件套接字 (to the peer),
   * tap_fd是连接到tun/tap接口的描述符 */
  /* 使用select()处理两个描述符 */
  maxfd = (tap_fd > net_fd)?tap_fd:net_fd;
  while(1) {
    int ret;
    fd_set rd_set;
    FD_ZERO(&rd_set);
    FD_SET(tap_fd, &rd_set); FD_SET(net_fd, &rd_set);
    ret = select(maxfd + 1, &rd_set, NULL, NULL, NULL);
    if (ret < 0 && errno == EINTR) {
      continue;
    }
    if (ret < 0) {
      perror("select()");
      exit(1);
    }
    if(FD_ISSET(tap_fd, &rd_set)) {
      /* 来自tun/tap的数据：只需读取并写入网络即可 */
      nread = cread(tap_fd, buffer, BUFSIZE);
      /* write length + packet */
      plength = htons(nread);
      nwrite = cwrite(net_fd, (char *)&plength, sizeof(plength));
      nwrite = cwrite(net_fd, buffer, nread);
    }
    if(FD_ISSET(net_fd, &rd_set)) {
      /* 来自网络的数据：读取它，并将其写入tun/tap接口。
       * 我们需要先读取长度，然后读取数据包 */
      /* 读取长度 */
      nread = read_n(net_fd, (char *)&plength, sizeof(plength));
      /* 读取数据包 */
      nread = read_n(net_fd, buffer, ntohs(plength));
      /* 现在buffer[]包含一个完整的数据包或帧，将其写入tun/tap接口 */
      nwrite = cwrite(tap_fd, buffer, nread);
    }
  }
...
```
（有关**read_n()**和**cwrite()**函数的详细信息，请参考源代码;他们所做的事情应该是显而易见的，上面的代码在select()方面不是100％正确的，并且做出了一些天真的假设 期望read_n（）和cwrite（）不会阻塞。正如我所说的，代码仅用于演示目的）
这是以上代码的主要逻辑：
- 程序使用**select()**来同时控制两个描述符。如果数据是从任何一个描述符进来的，则写入另一个描述符。
- 由于程序使用TCP，接收者将看到一个单一的数据流，这使得识别数据包边界变得困难。所以当一个数据包或帧被写入网络时，其长度被预先占用（2个字节）到实际数据包。
- 当数据从tap_fd描述符进入时，一次读取读取一个完整的数据包或帧;因此可以直接将其写入网络，其长度已预先确定。由于长度号码是一个短整型，因此长于一个字节，以“原始”二进制格式写入，所以使用**ntohs()/htons()**在具有不同字节顺序的机器之间进行互操作。
- 当数据从网络进入时，由于前面提到的技巧，我们可以通过读取数据流之前的两个字节的长度来知道下一个数据包将要处理多长时间。当我们读取数据包时，我们把它写到tun / tap接口描述符中，内核接收它作为“来自线路”。
那么你可以用这样的程序做什么？ 那么，你可以创建一个隧道！ 首先，在隧道两端的主机上创建并配置必要的tun / tap接口，包括分配一个IP地址。 对于这个例子，我将假定两个tun接口：本地计算机上的tun11，192.168.0.1/24，远程计算机上的tun3，192.168.0.2/24。 **simpletun**默认连接使用TCP端口55555的主机（您可以使用-p命令行开关更改该端口）。 远程主机将在服务器模式下运行simpletun，并且本地主机将以客户端模式运行。 所以有了下面的例子（远程服务器在10.2.3.4）：
```
[remote]# openvpn --mktun --dev tun3 --user waldner
Fri Mar 26 11:11:41 2010 TUN/TAP device tun3 opened
Fri Mar 26 11:11:41 2010 Persist state set to: ON
[remote]# ip link set tun3 up
[remote]# ip addr add 192.168.0.2/24 dev tun3
[remote]$ ./simpletun -i tun3 -s
# 服务器块等待客户端连接
[local]# openvpn --mktun --dev tun11 --user waldner
Fri Mar 26 11:17:37 2010 TUN/TAP device tun11 opened
Fri Mar 26 11:17:37 2010 Persist state set to: ON
[local]# ip link set tun11 up
[local]# ip addr add 192.168.0.1/24 dev tun11
[local]$ ./simpletun -i tun11 -c 10.2.3.4
# 无状况，用户现在已连接
[local]$ ping 192.168.0.2
PING 192.168.0.2 (192.168.0.2) 56(84) bytes of data.
64 bytes from 192.168.0.2: icmp_seq=1 ttl=241 time=42.5 ms
64 bytes from 192.168.0.2: icmp_seq=2 ttl=241 time=41.3 ms
64 bytes from 192.168.0.2: icmp_seq=3 ttl=241 time=41.4 ms
64 bytes from 192.168.0.2: icmp_seq=4 ttl=241 time=41.0 ms
--- 192.168.0.2 ping statistics ---
4 packets transmitted, 4 received, 0% packet loss, time 2999ms
rtt min/avg/max/mdev = 41.047/41.599/42.588/0.621 ms
# 尝试刺激的事情
[local]$ ssh waldner@192.168.0.2
waldner@192.168.0.2's password:
Linux remote 2.6.22-14-xen #1 SMP Fri Feb 29 16:20:01 GMT 2008 x86_64
Welcome to remote!
[remote]$
```
当建立如上所述的隧道时，从外部可以看到的只是两个同等简单隧道之间的连接（在这种情况下为TCP）。上面例子中的“真实”数据（即高层应用程序ping或ssh交换的数据）绝不会直接暴露在线路上（尽管它以明文形式发送，见下文）。如果在运行simpletun的主机上启用IP转发，并在其他主机上创建必要的路由，则可以通过隧道到达远程网络。
另外请注意，如果所涉及的虚拟接口是tap类型的，则可以透明地桥接两个地理上距离较远的以太网LAN，以便设备认为它们全部在相同的第2层网络上。为此，需要在网关（即运行simpletun的主机或使用tap接口的另一个隧道软件），本地LAN接口和虚拟分接接口上进行桥接。这样，从局域网接收到的帧也被发送到tap接口（由于网桥），隧道应用程序读取它们并将它们发送给远程对等体;在那里，另一个桥将确保如此接收的帧被转发到远程LAN。同样的事情会发生在相反的方向。由于我们在两个LAN之间传递以太网帧，因此两个LAN有效地桥接在一起。这意味着您可以在伦敦（例如）有10台机器，在柏林有50台机器，您可以使用192.168.1.0/24子网中的地址（或任何您想要的子网地址）创建60台计算机的以太网网络，只要它可以容纳至少60个主机地址）。但是，如果你想设置类似的东西，不要使用simpletun！
# Extensions and improvements
simpletun非常简单和简单，可以通过多种方式扩展。首先，可以添加连接到对等方的新方法。例如，可以实现UDP连接，或者如果你勇敢的话，也可以通过ICMP（也可以通过IPv6）。其次，数据通过网络连接以明文形式传递。但是，当数据在程序的缓冲区中时，它可能在被传输之前以某种方式被改变，例如它可以被加密（并且在另一端被类似地解密）。
但是，为了本教程的目的，程序的限制版本应该已经给你一个如何使用tun/tap的隧道工作的想法。虽然simpletun是一个简单的演示，但许多流行的使用tun/tap接口的程序也是如此，例如OpenVPN，vtun或Openssh的VPN特性。
最后，值得注意的是，如果隧道连接是通过TCP连接的，那么我们可能会遇到所谓的“tcp over tcp”的情况。有关更多信息，请参阅“为什么tcp over tcp是一个坏主意”。请注意，OpenVPN之类的应用程序默认情况下使用UDP，因此使用TCP以降低性能而闻名（尽管在某些情况下它是唯一的选择）。
善于交际，分享！
作者：御浅永夜
链接：https://www.jianshu.com/p/6a2e5a0cceb2
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
