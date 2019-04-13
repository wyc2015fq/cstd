
# 网络设备开军火发库（libnet,libpcap,pfring,dpdk） - 3-Number - CSDN博客


置顶2015年08月14日 16:09:45[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：3745


### 1
### 缘起
实际工作中开发网络设备时，常用到的一些开源网络库，做一些底层的包的捕获、发送、修改等。下面简单介绍一下有哪些开源工具，以及相应的简单用法。
有一些特殊的数据包要做特殊处理，如vlan环境下的数据包。
### 2 libnet
### （发包）
### 2.1
### 简介
libnet是一个小型的接口函数库，主要用C语言写成，提供了低层网络数据包的构造、处理和发送功能。libnet的开发目的是：建立一个简单统一的[网络编程](http://baike.baidu.com/view/1317473.htm)接口以屏蔽不同操作系统底层网络编程的差别，使得程序员将精力集中在解决关键问题上。
其他相关介绍，以及源码解析，网上查阅。
### 2.2
### 关键数据结构
### 2.3
### 常用函数介绍
以下函数的使用需要包含头文件libnet.h。
libnet_t *libnet_init(int injection_type, char *device, char *err_buf)
功能：数据包内存初始化及环境建立
参数：
injection_type：构造的类型
LIBNET_LINK，链路层
LIBNET_RAW4，网络接口层（网络层）
LIBNET_LINK_ADV，链路层高级版本
LIBNET_RAW4_ADV，  网络层高级版本
device：网络接口，如"eth0"，或IP地址，亦可为NULL (自动查询搜索)
err_buf：存放出错的信息
返回值：
成功：一个libnet *类型的指针，后面的操作都得使用这个指针
失败：NULL
void libnet_destroy(libnet_t *l)
功能：释放资源
参数：
l：libnet_init()返回的libnet *指针
返回值：无
char* libnet_addr2name4(u_int32_t in, u_int8_t use_name)
功能：将网络字节序转换成点分十进制数串
参数：
in：网络字节序的ip地址
use_name：
LIBNET_RESOLVE，  对应主机名
LIBNET_DONT_RESOLVE，对应点分十进制IPv4地址
返回值：
成功：点分十进制ip地址
失败：NULL
u_int32_t libnet_name2addr4(libnet_t *l,char *host_name,u_int8_t use_name)
功能：将点分十进制数串转换为网络字节序ip地址
参数：
l：libnet_init()返回的libnet *指针
host_name：
LIBNET_RESOLVE，  对应主机名
LIBNET_DONT_RESOLVE，对应点分十进制IPv4地址
返回值：
成功：网络字节序ip地址
失败：-1
u_int32_t libnet_get_ipaddr4(libnet_t *l)
功能：获取接口设备ip地址
参数：
l：libnet_init()返回的libnet *指针
返回值：
成功：网络字节序的ip地址
失败：-1
struct libnet_ether_addr* libnet_get_hwaddr(libnet_t *l)
功能：获取接口设备硬件地址
参数：
l：libnet_init()返回的libnet *指针
返回值：
成功：指向MAC地址的指针
失败：NULL
libnet_ptag_t libnet_build_udp(u_int16_t sp, u_int16_t dp,u_int16_t len, u_int16_t sum,u_int8_t *payload, u_int32_t payload_s,libnet_t *l, libnet_ptag_t ptag)
功能：构造udp数据包
参数：
sp:源端口号
dp：目的端口号
len：udp包总长度
sum：校验和，设为0，libnet自动填充
payload：负载，为给应用程序发送的文本内容，没有内容时可设置为NULL
payload_s：负载长度，给应用程序发送文本内容的长度，或为0
l：libnet_init()返回的libnet *指针
ptag：协议标记，第一次组新的发送包时，这里写0，同一个应用程序，下一次再组包时，这个位置的值写此函数的返回值。
返回值：
成功：协议标记
失败：-1
libnet_ptag_t libnet_build_tcp(u_int16_t sp, u_int16_t dp,u_int32_t seq, u_int32_t ack,u_int8_t control, u_int16_t winu_int16_t sum, u_int16_t urg,u_int16_t len, u_int8_t *payload,u_int32_t payload_s, libnet_t *l,libnet_ptag_t ptag )
功能：构造tcp数据包
参数：
sp：源端口号
dp：目的端口号
seq：序号
ack：ack标记
control：控制标记
win：窗口大小
sum：校验和，设为0，libnet自动填充
urg：紧急指针
len：tcp包长度
payload：负载，为给应用程序发送的文本内容，可设置为NULL
payload_s：负载长度，或为0
l：libnet_init()返回的libnet *指针
ptag：协议标记，第一次组新的发送包时，这里写0，同一个应用程序，下一次再组包时，这个位置的值写此函数的返回值。
返回值：
成功：协议标记
失败：-1
libnet_ptag_t libnet_build_tcp_options(u_int8_t *options,u_int32_t options_s,libnet_t *l,libnet_ptag_t ptag )
功能：构造tcp选项数据包
参数：
options：tcp选项字符串
options_s：选项长度
l：libnet句柄，libnet_init()返回的libnet *指针
ptag：协议标记，第一次组新的发送包时，这里写0，同一个应用程序，下一次再组包时，这个位置的值写此函数的返回值。
返回值：
成功：协议标记
失败：-1
libnet_ptag_t libnet_build_ipv4(u_int16_t ip_len, u_int8_t tos,u_int16_t id, u_int16_t flag,u_int8_t ttl, u_int8_t prot,u_int16 sum, u_int32_t src,u_int32_t dst, u_int8_t *payload,u_int32_t payload_s,libnet_t *l,libnet_ptag_t ptag )
功能：构造一个IPv4数据包
参数：
ip_len：ip包总长
tos：服务类型
id：ip标识
flag：片偏移
ttl：生存时间
prot：上层协议
sum：校验和，设为0，libnet自动填充
src：源ip地址
dst：目的ip地址
payload：负载，可设置为NULL（这里通常写NULL）
payload_s：负载长度，或为0（这里通常写0）
l：libnet句柄，libnet_init()返回的libnet *指针
ptag：协议标记，第一次组新的发送包时，这里写0，同一个应用程序，下一次再组包时，这个位置的值写此函数的返回值。
返回值：
成功：协议标记
失败：-1
libnet_ptag_t libnet_build_ipv4_options(u_int8_t*options, u_int32_t options,libnet_t*l, libnet_ptag_t ptag)
功能：构造IPv4选项数据包
参数：
options：tcp选项字符串
options_s：选项长度
l：libnet句柄，libnet_init()返回的libnet *指针
ptag：协议标记，若为0，建立一个新的协议
返回值：
成功：协议标记
失败：-1
libnet_ptag_t libnet_build_arp(u_int16_t hrd, u_int16_t pro,u_int8_t hln, u_int8_t pln，u_int16_t op, u_int8_t *sha,u_int8_t *spa, u_int8_t *tha，u_int8_t *tpa, u_int8_t *payload，u_int32_t payload_s, libnet_t *l,libnet_ptag_t ptag )
功能：构造arp数据包
参数：
hrd：硬件地址格式，ARPHRD_ETHER（以太网）
pro：协议地址格式，ETHERTYPE_IP（IP协议）
hln：硬件地址长度
pln：协议地址长度
op：ARP协议操作类型（1：ARP请求，2：ARP回应，3：RARP请求，4：RARP回应）
sha：发送者硬件地址
spa：发送者协议地址
tha：目标硬件地址
tpa：目标协议地址
payload：负载，可设置为NULL（这里通常写NULL）
payload_s：负载长度，或为0（这里通常写0）
l：libnet句柄，libnet_init()返回的libnet *指针
ptag：协议标记，第一次组新的发送包时，这里写0，同一个应用程序，下一次再组包时，这个位置的值写此函数的返回值。
返回值：
成功：协议标记
失败：-1
libnet_ptag_t libnet_build_ethernet(u_int8_t*dst, u_int8_t *src,u_int16_ttype, u_int8_t*payload,u_int32_tpayload_s, libnet_t*l,libnet_ptag_t ptag )
功能：构造一个以太网数据包
参数：
dst：目的mac
src：源mac
type：上层协议类型
payload：负载，即附带的数据，可设置为NULL（这里通常写NULL）
payload_s：负载长度，或为0（这里通常写0）
l：libnet句柄，libnet_init()返回的libnet *指针
ptag：协议标记，第一次组新的发送包时，这里写0，同一个应用程序，下一次再组包时，这个位置的值写此函数的返回值。
返回值：
成功：协议标记
失败：-1
int libnet_write(libnet_t * l)
功能：发送数据包
参数：
l：libnet句柄，libnet_init()返回的libnet *指针
返回值：
成功：发送数据包的长度
失败：返回-1
### 2.4
### 开发基本流程
利用libnet函数库开发应用程序的基本步骤：
1）数据包内存初始化
2）构造数据包
3）发送数据
4）释放资源
以发送UDP数据包为例，流程图如下：
注意组包的顺序和层次。
### 2.5
### 代码示例
参照源码中的例程。
### 3 libpcap
### （捕获包）
### 3.1
### 简介
libpcap（Packet Capture Library），即数据包捕获函数库，是Unix/Linux平台下的网络数据包捕获函数库。它是一个独立于系统的用户层包捕获的API接口，为底层网络监测提供了一个可移植的框架。
libpcap主要由两部份组成：网络分接头(Network Tap)和数据过滤器(Packet Filter)。网络分接头从网络设备驱动程序中收集数据拷贝，过滤器决定是否接收该数据包。Libpcap利用BSD Packet Filter(BPF)算法对网卡接收到的链路层数据包进行过滤。BPF算法的基本思想是在有BPF监听的网络中，网卡驱动将接收到的数据包复制一份交给BPF过滤器，过滤器根据用户定义的规则决定是否接收此数据包以及需要拷贝该数据包的那些内容，然后将过滤后的数据给与过滤器相关联的上层应用程序。
libpcap的包捕获机制就是在数据链路层加一个旁路处理。当一个数据包到达网络接口时，libpcap首先利用已经创建的Socket从链路层驱动程序中获得该数据包的拷贝，再通过Tap函数将数据包发给BPF过滤器。BPF过滤器根据用户已经定义好的过滤规则对数据包进行逐一匹配，匹配成功则放入内核缓冲区，并传递给用户缓冲区，匹配失败则直接丢弃。如果没有设置过滤规则，所有数据包都将放入内核缓冲区，并传递给用户层缓冲区。
### 3.2Libpcap
### 开发库使用说明
基于pcap的嗅探器程序的总体架构，其流程如下：
1）选择嗅探接口：在Linux中，这可能是eth0，而在BSD系统中则可能是xl1等等。也可以用一个字符串来定义这个设备，或者采用pcap提供的接口名来工作。
2）初始化pcap：告诉pcap对何设备进行嗅探，使用文件句柄进行设备的区分，必须命名该嗅探“会话”，以此使它们各自区别开来。
3）创建规则集合：用于只想嗅探特定的传输，这个过程分为三个相互紧密关联的阶段。规则集合被置于一个字符串内，并且被转换成能被pcap读的格式(因此编译它)。编译实际上就是在程序里调用一个不被外部程序使用的函数。接下来告诉 pcap使用它来过滤出所要的那一个会话。(此步骤可选)
4）进入主体执行循环：在这个阶段内pcap一直工作到它接收了所有我们想要的包为止。每当它收到一个包就调用另一个已经定义好的函数，这个函数可以实现任何要求，它可以剖析所部获的包并给用户打印出结果，它可以将结果保存为一个文件。
5）关闭会话：在嗅探到所需的数据后，关闭会话并结束。
### 3.3
### 常用
### API
### 函数
1）获取数据包捕获描述字
函数名称：pcap_t *pcap_open_live(char *device, int snaplen, int promisc, int to_ms, char *ebuf)
函数功能：获得用于捕获网络数据包的数据包捕获描述字。
参数说明：device参数为指定打开的网络设备名。snaplen参数定义捕获数据的最大字节数。Promisc 指定是否将网络接口置于混杂模式。to_ms参数指*定超时时间（毫秒）。ebuf参数则仅在pcap_open_live()函数出错返回NULL时用于传递错误消息。
2）打开保存捕获数据包文件
函数名称：pcap_t *pcap_open_offline(char *fname, char *ebuf)
函数功能：打开以前保存捕获数据包的文件，用于读取。
参数说明：fname参数指定打开的文件名。该文件中的数据格式与tcpdump和tcpslice兼容。”-“为标准输入。ebuf参数则仅在pcap_open_offline()函数出错返回NULL时用于传递错误消息。
3）转储数据包
函数名称：pcap_dumper_t *pcap_dump_open(pcap_t *p, char *fname)
函数功能：打开用于保存捕获数据包的文件，用于写入。
参数说明：fname参数为”-“时表示标准输出。出错时返回NULL。p参数为调用pcap_open_offline()????? 或pcap_open_live()函数后返回的pcap结构指针，即网卡句柄。fname参数指定打开的文件名，存盘的文件名。如果返回NULL，则可调用pcap_geterr()函数获取错误消息。
4）查找网络设备
函数名称：char *pcap_lookupdev(char *errbuf)
函数功能：用于返回可被pcap_open_live()或pcap_lookupnet()函数调用的网络设备名指针。
返回值：如果函数出错，则返回NULL，同时errbuf中存放相关的错误消息。
5）获取网络号和掩码
函数名称：int pcap_lookupnet(char *device, bpf_u_int32 *netp,bpf_u_int32 *maskp, char *errbuf)
函数功能：获得指定网络设备的网络号和掩码。
参数说明：netp参数和maskp参数都是bpf_u_int32指针。
返回值：如果函数出错，则返回-1，同时errbuf中存放相关的错误消息。
6)捕获并处理数据包
函数名称：int pcap_dispatch(pcap_t *p, int cnt,pcap_handler callback, u_char *user)?
函数功能：捕获并处理数据包。?
参数说明：cnt参数指定函数返回前所处理数据包的最大值。cnt=? -1表示在一个缓冲区中处理所有的数据包。cnt=0表示处理所有数据包，直到产生以下错误之一：读取到EOF；超时读取。callback参数指定一个带有三个参数的回调函数，这三个参数为：一个从pcap_dispatch()函数传递过来的u_char指针，一个pcap_pkthdr结构的指针，和一个数据包大小的u_char指针。
返回值：如果成功则返回读取到的字节数。读取到EOF时则返回零值。出错时则返回-1，此时可调用pcap_perror()或pcap_geterr()函数获取错误消息。
7)捕获和处理数据包
函数名称：int pcap_loop(pcap_t *p, int cnt,pcap_handler callback, u_char *user)
函数功能：功能基本与pcap_dispatch()函数相同，只不过此函数在cnt个数据包被处理或出现错误时才返回，但读取超时不会返回。而如果为pcap_open_live()函数指定了一个非零值的超时设置，然后调用pcap_dispatch()函数，则当超时发生时pcap_dispatch()函数会返回。cnt参数为负值时pcap_loop()函数将始终循环运行，除非出现错误。
8)输出数据包
函数名称：void pcap_dump(u_char *user, struct pcap_pkthdr *h,u_char *sp)
函数功能：向调用pcap_dump_open()函数打开的文件输出一个数据包。该函数可作为pcap_dispatch()函数的回调函数。
参数说明: 参数1: 所建立的文件pcap_dump_open()的返回值,要进行强制转换.;参数2: 数据包特有的内容.;参数 3: 数据包内容指针
9)编译字串至过滤程序
函数名称：int pcap_compile(pcap_t *p, struct bpf_program *fp,char *str, int optimize, bpf_u_int32 netmask)
函数功能：将str参数指定的字符串编译到过滤程序中。?
参数说明：fp是一个bpf_program结构的指针，在pcap_compile()函数中被赋值。optimize参数控制结果代码的优化。netmask参数指定本地网络的网络掩码。
10)指定过滤程序
函数名称：int pcap_setfilter(pcap_t *p, struct bpf_program *fp)
函数功能：指定一个过滤程序。?
参数说明：fp参数是bpf_program结构指针，通常取自pcap_compile()函数调用。
返回值：出错时返回-1；成功时返回0
11)获取下一个数据包
函数名称：u_char *pcap_next(pcap_t *p, struct pcap_pkthdr *h)
函数功能：返回指向下一个数据包的u_char指针
12)获取数据链路层类型
函数名称：int pcap_datalink(pcap_t *p)
函数功能：返回数据链路层类型，例如DLT_EN10MB
13)获取快照参数值
函数名称：int pcap_snapshot(pcap_t *p)
函数功能：返回pcap_open_live被调用后的snapshot参数值
14)检测字节顺序
函数名称：int pcap_is_swapped(pcap_t *p)
函数功能：返回当前系统主机字节与被打开文件的字节顺序是否不同
15)获取主版本号
函数名称：int pcap_major_version(pcap_t *p)
函数功能：返回写入被打开文件所使用的pcap函数的主版本号
16)获取辅版本号
函数名称：int pcap_minor_version(pcap_t *p)
函数功能：返回写入被打开文件所使用的pcap函数的辅版本号
17)结构赋值
函数名称：int pcap_stats(pcap_t *p, struct pcap_stat *ps)
函数功能：向pcap_stat结构赋值。成功时返回0。这些数值包括了从开始捕获数据以来至今共捕获到的数据包统计。如果出错或不支持数据包统计，则返回-1，
且可调用pcap_perror()或pcap_geterr()函数来获取错误消息。
18)获取打开文件名
函数名称：FILE *pcap_file(pcap_t *p)
函数功能：返回被打开文件的文件名。
19)获取描述字号码
函数名称：int pcap_fileno(pcap_t *p)?
函数功能：返回被打开文件的文件描述字号码。
20)显示错误消息
函数名称：void pcap_perror(pcap_t *p, char *prefix)
函数功能：在标准输出设备上显示最后一个pcap库错误消息。以prefix参数指定的字符串为消息头。
21）创建pcap_t对象
函数名称：pcap_t * pcap_create(const char *source, char *errbuf)
### 4 PF_RING
### （捕获包）
### 4.1
### 简介
PF_RING是Luca Deri发明的提高内核处理数据包效率，并兼顾应用程序的补丁，如Libpcap和TCPDUMP等，以及一些辅助性程序（如ntop查看并分析网络流量等）。PF_RING是一种新型的网络socket，它可以极大的改进包捕获的速度。并且有如下特征：
1）可以用于Linux 2.6.18以上的内核；
2）4.x版本的PF_RING可以直接应用于内核，不需要给内核打补丁；
3）PF_RING驱动可以进行包捕获的加速；
4）支持使用商用网络适配器的10 GB的硬件包过滤
5）设备驱动无关（推荐使用支持NAPI的网卡（intel网卡）来获得最好的性能）；
6）基于内核的包捕获和采样；
7）Lipcap支持与基于pcap的应用程序的无缝集成；
8）可以指定上百个头过滤到BPF中；
9）内容检查，以至于只有符合负载过滤的包才能通过;
10）PF_RING的插件可以用于增强包解析和内容过滤；
11)可以工作在混杂模式（经过网卡的报文全部可以被捕获到）；
### 4.2
### 编译安装
### 4.2.1
### 最新版本的目录结构：
Drivers/
Drivers/Broadcom/
Drivers/Intel/
Drivers/myricom/
Kernel/    内核有关的补丁
Userland/  用户空间代码
Userland/lib 用户空间库用于控制PF_RING
Userland/libpcap-xxx-ring PF_RING支持的Libpcap增强库
Userland/examples/ 包计数应用（使用它作为测试使用）
### 4.2.2
### 编译
编译安装PF_RING之前需要卸载原来的网卡驱动，卸载之前使用ethtool命令查看当前网卡的类型和驱动版本。
\#ethtool -i ethx
\#lsmod | grep e1000e
\#rmmod e1000e    此处为e1000e驱动
注：如果使用ssh远程卸载驱动会造成网络不能连接，务必现场操作。
### 4.2.3.
### 编译内核
解压缩PF_RING安装包，进入到Kernel目录下编译和安装内核补丁。
\# tar -zxf PF_RING.4.7.0.tar.gz
\# cd PF_RING.4.7.0/kernel
\# make
\# sudo make install             内核安装需要root用户权限
insmod <PF_RINGPATH>/kernel/pf_ring.ko [transparent_mode=0|1|2]
[min_num_slots=x][enable_tx_capture=1|0][enable_ip_defrag=1|0][quick_mode=1|0]
实例：
\# sodu insmod pf_ring.ko transparent_mode=1
当PF_RING激活时，会创建/proc/net/pf_ring目录，使用cat命令查看设置：
\# cat /proc/net/pf_ring/info
注1：为了编译PF_RING内核模块，你需要安装Linux内核的头文件（或者内核源代码）。
### 4.2.4
### 编译用户空间
### PF_RING
### 库
进入到用户空间库userland/lib下，编译和安装。
\# cd ../userland/lib
\# ./configure
\# make
\# sodu make install
如果需要使用libpcap抓包分析，请卸载之前安装的libpcap，然后进入/userland/libpcap-xxx-ring/目录下配置、编译和安装驱动。
\# rpm -qa libpcap    查看安装的libpcap，如果有libpcap则强制卸载
\# rpm -e libpcap --nodefs
\# cd ../libpcap
\# ./configure
\# make
\# sudo make install
注：为了使用PF_RING的优点，请使用PF_RING使能的libpcap.a重新编译应用。
进入到userland/examples目录编译例子程序。
\# cd <PF_RING PATH>/userland/examples
\# make
\# ./pfcount -i eth0     捕获eth0网口的数据报文
注：使用drivers/intel/ixgbe下的驱动（支持DNA的ixgbe驱动的网卡）+DNA驱动技术可以达到线速采集，PF_RING模块必须在DNA驱动之前加载。
### 4.2.5.
### 编译网卡的驱动
进入到drivers目录下，根据ethtool -i ethx命令查看的网卡类型和驱动进入指定的目录进行编译和安装。
\# cd ../../ drivers/intel/e1000e/e1000e-1.3.10a/src
\# make
\# sodu make install
开始安装驱动，进入/lib/modules/<redhat-version>/kernel/net目录，可以看到有pf_ring目录，进入到该目录下进行PF_RING模块的安装。
\# sodu insmod pf_ring.ko transparent_mode=1
安装网卡驱动，进入到目录lib/modules/<redhat-version>/kernel/drivers/net下进行网卡驱动安装。
\# sodu insmod e1000e.ko
\# sodu modprobe e1000e  (只能载入/lib/modules/<kernel ver>/中模块)
安装完毕，使用dmesg命令查看驱动是否安装成功，如果成功的话，可以看到：
\# dmesg
[PR_RING]…… 信息。
PF_RING会安装一个类型为27的协议簇，可以使用sock（PF_RING, SOCK_RAW,0）打开一个socket，使用libpcap的朋友不需要修改程序，需要重新编译，链接的时候请加上libpfring.so。
启动自动加载PF_RING模块，修改/etc/modprobe.conf文件
\# vi /etc/modprobe.conf
在文件的尾部增加一行 modprobe pf_ring
系统重启后内核会自动加载pf_ring模块，modprobe只能加载/lib/modules下的模块。
### 4.2.6
### 使用
使用用户空间中的PF_RING库API来编写代码，并且使用用户空间中的libpfring.a和libpcap.a编译代码就可以使用PF_RING来提高包捕获的性能。
### 4.3
### 开发流程
参照pfring源码目录中的例程。
### 5DPDK


