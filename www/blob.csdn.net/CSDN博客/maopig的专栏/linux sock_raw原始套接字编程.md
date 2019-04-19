# linux sock_raw原始套接字编程 - maopig的专栏 - CSDN博客
2012年03月14日 09:50:32[maopig](https://me.csdn.net/maopig)阅读数：7804
sock_raw原始套接字编程可以接收到本机网卡上的数据帧或者数据包,对与监听网络的流量和分析是很有作用的.一共可以有3种方式创建这种socket
1.socket(AF_INET, SOCK_RAW, IPPROTO_TCP|IPPROTO_UDP|IPPROTO_ICMP)发送接收ip数据包
2.socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP|ETH_P_ARP|ETH_P_ALL))发送接收以太网数据帧
3.socket(AF_INET, SOCK_PACKET, htons(ETH_P_IP|ETH_P_ARP|ETH_P_ALL))过时了,不要用啊
理解一下SOCK_RAW的原理, 比如网卡收到了一个 14+20+8+100+4 的udp的以太网数据帧.
首 先,网卡对该数据帧进行硬过滤(根据网卡的模式不同会有不同的动作,如果设置了promisc混杂模式的话,则不做任何过滤直接交给下一层输 入例程,否则非本机mac或者广播mac会被直接丢弃).按照上面的例子,如果成功的话,会进入ip输入例程.但是在进入ip输入例程之前,系统会检查系 统中是否有通过socket(AF_PACKET, SOCK_RAW, ..)创建的套接字.如果有的话并且协议相符,在这个例子中就是需要ETH_P_IP或者ETH_P_ALL类型.系统就给每个这样的socket接收缓 冲区发送一个数据帧拷贝.然后进入下一步.
其次,进入了ip输入例程(ip层会对该数据包进行软过滤,就是检查校验或者丢弃非本机ip 或者广播ip的数据包等,具体要参考源代码),例子 中就是如果成功的话会进入udp输入例程.但是在交给udp输入例程之前,系统会检查系统中是否有通过socket(AF_INET, SOCK_RAW, ..)创建的套接字.如果有的话并且协议相符,在这个例子中就是需要IPPROTO_UDP类型.系统就给每个这样的socket接收缓冲区发送一个数据 帧拷贝.然后进入下一步.
最后,进入udp输入例程 ...
ps:如果校验和出错的话,内核会直接丢弃该数据包的.而不会拷贝给sock_raw的套接字,因为校验和都出错了,数据肯定有问题的包括所有信息都没有意义了.
进一步分析他们的能力.
1. socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
能:该套接字可以接收协议类型为(tcp udp icmp等)发往本机的ip数据包,从上面看的就是20+8+100.
不能:不能收到非发往本地ip的数据包(ip软过滤会丢弃这些不是发往本机ip的数据包).
不能:不能收到从本机发送出去的数据包.
发送的话需要自己组织tcp udp icmp等头部.可以setsockopt来自己包装ip头部
这种套接字用来写个ping程序比较适合
2. socket(PF_PACKET, SOCK_RAW, htons(x));  
这个套接字比较强大,创建这种套接字可以监听网卡上的所有数据帧.从上面看就是20+20+8+100.最后一个以太网crc从来都不算进来的,因为内核已经判断过了,对程序来说没有任何意义了.
能: 接收发往本地mac的数据帧
能: 接收从本机发送出去的数据帧(第3个参数需要设置为ETH_P_ALL)
能: 接收非发往本地mac的数据帧(网卡需要设置为promisc混杂模式)
协议类型一共有四个
ETH_P_IP  0x800      只接收发往本机mac的ip类型的数据帧
ETH_P_ARP 0x806      只接受发往本机mac的arp类型的数据帧
ETH_P_ARP 0x8035     只接受发往本机mac的rarp类型的数据帧
ETH_P_ALL 0x3        接收发往本机mac的所有类型ip arp rarp的数据帧, 接收从本机发出的所有类型的数据帧.(混杂模式打开的情况下,会接收到非发往本地mac的数据帧)
发 送的时候需要自己组织整个以太网数据帧.所有相关的地址使用struct sockaddr_ll 而不是struct sockaddr_in(因为协议簇是PF_PACKET不是AF_INET了),比如发送给某个机器,对方的地址需要使用struct sockaddr_ll.
这种socket大小通吃,强悍
linux socket调用与arp报文发送 
Linux提供最常用的网络通信应用程序开发接口--Berkerley套接字(Socket)．它既适用于同一主机上进程间通信(IPC)，又适用于不同主机上的进程间通信。套接字的设置通过socket调用完成：
int socket(int family,int type,int protocol);
其中family指通信域或协议族，Linux系统支持的网络协议族有PF_UNIX，PF_IPX，PF_PACKET等几十种；type为套接字类型,目前有SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_PACKET等；protocol是套接字所用的特定协议类型号．
Linux系统提供的基于数据链路层开发应用程序的接口集成在套接字中，它是通过创建packet类型的套接宇．使应用程序可直接在数据链路层接收或发送未被系统处理的原始的数据报文(如ARP报文)，用户也可以使用packet类型的套接宇在物理层上定义自己特殊的网络协议。只有注册号为0的用户(超级用户)进程才能建立或打开用于访问网络低层的套接字．在Linux系统中，用以下三种方式创建的packet套接字可直接用于访问数据链路层：
(1)PF_INET协议族中SOCK_PACKEI类型的套接字
(2)PF_PACKET协议族中SOCK_RAW类型的套接字
(3)PF_PACKET协议族中SOCK_DGRAM类型的套接字
Linux 2．0中对数据链路层的操作主要使用SOCK_PACKET定义的packet套接字．初始化定义如下：
sockfd=socket(AF_INET,SOCK_PACKET,protocol)；
其中，protocol用于决定套接字所使用的物理层协议(在IEEE802.3中定义)．笔者在此选择常用的物理层协议ETH_P_IP(Internet协议)．SOCK_PACKET使用一种比较老的sockaddr_pkt数据结构来设置网络接口。
在Linux 2 2中使用PF_PACKET代替SOCK_PACKET来定义packet套接字．这种套接字的初始化定义如下：
sockfd=socket(PF_PACKET,socket_type,protocol)；
其中socket_type只能为SOCK_RAW或SOCK_DGRAM,protocol为物理层通信协议(同上)。SOCK_RAW和SOCK_DGRAM类型套接字使用一种与设备无关的标准物理层地址结构sockaddr_ll，但具体操作的报文格式不同。SOCK_RAW套接字直接向网络硬件驱动程序发送(或从网络硬件驱动程序接收)没有任何处理的完整数据报文(包括物理帧的帧头)，这就要求程序员必须了解对应设备的物理帧帧头结构，才能正确地装载和分析报文。SOCK_DGRAM套接字收到的数据报文的物理帧帧头会被系统自动去掉，同样,在发送时．系统将会根据sockaddr_ll结构中的目的地址信息为数据报文舔加一个台适的物理帧帧头。
默认情况下．从任何接口收到的符合指定协议的所有数据报文都会被传送到packet套接字。使用bind系统调用以一个sochddr_l1地址结构将paccket套接字与某个网络接口相绑定，可使套接字只接收指定接口的
数据报文．socaddr_ll地址结构定义如下：
struct sockaddr_ll
{
unsigned short sll_family; /* 总是 AF_PACKET */
unsigned short sll_protocol; /* 物理层的协议 */
int sll_ifindex; /* 接口号 */
unsigned short sll_hatype; /* 报头类型 */
unsigned char sll_pkttype; /* 分组类型 */
unsigned char sll_halen; /* 地址长度 */
unsigned char sll_addr[8]; /* 物理层地址 */
};
一、利用PF_lNET协议族中SOCK_PACKET类型的套接宇实现ARP
(1)建套接字
创建套接宇采用socket系统调用，格式如下：
sockfd=socket(PF_INET,SOCK_PACKET,htons(ETH_P_ARP));
(2)装载报文
对于SOCK_PACKET类型的套接字，以太网物理帧头应作为所发送报文一部分由程序员设置，物理帧头的格式定义如下:(in /usr/include/linux/if_ether.h)
92 struct ethhdr
93 {
94 unsigned char h_dest[ETH_ALEN]; /* destination eth addr */
95 unsigned char h_source[ETH_ALEN]; /* source ether addr */
96 unsigned short h_proto; /* packet type ID field */
97 };
实际发送的地址解析报文帧由以太网物理帧头与帧数据(ARP报文)共同组成，用结构体ARPPACKET表
示如下：
typedef struct {
struct ethhdr eth_header; //struct defined in linux/if_ether.h
ARPHDR arp_header;
}ARPPACKET;
上述报文结构的装载比较简单。对ARP部分，arp_header的设置如下：
ptk.arp_header.ar_hrd=htons(ARPHRD_ETHER); //ARPHRD_ETHER is defined in linux/if_arp.h
ptk.arp_header.ar_pro=htons(ETHERTYPE_IP);
ptk.arp_header.ar_hln=6;
ptk.arp_header.ar_pln=4;
ptk.arp_header.ar_op=htons(ARPOP_REQUEST);
pkt.arp_header.ar_sha[]、pkt.arp_header.ar_sip[]、pkt_arp_header.ar_tip[]分别填入本机的物理地址、ip地址和要解析的对方主机的ip地址．返回报文中pkt.arp_header.tha[]中的内容就是解析
得到的对方主机的物理地址。
对于以太网帧头部分，pkt.eth_header.h_dest[]为目的地址，即广播物理地址0xFFFFFF， pkt.eth_header.source[]为本机物理地址(同pkt.arp_header.ar_sha[])，
pkt.eth_header.h_proto赋值htons(ETHERTYPE_ARP)表示为地址解析类型报文。
ETHERTYPE_ARP与ETH_P_ARP的值都是0x0806，只是定义的文件不同。前者定义在net/ethernet.h,后者定义在linux/if_ether.h
(3)报文的发送与接收
在数据链路层发送/接收报文与在IP层发送/接收数据报文类似，分别用系统调用sendto()和recvfrom()
完成，只是要将配置好的含有目标地址的报文发往本地网络硬件而不是目标主机。相应的程序段如下：
struct sockaddr to,from;
int fromlen=0;
strcpy(to.sa_data,"eth0");
sendto(sockfd,pkt,sizeof(struct ARPPACKET),0,&to,sizeof(struct sockaddr));
recvfrom(sockfd,buf,PACKET_SIZE,0,&from,&fromlen);
其中buf为包含结构体ARPPACKET的字符型指针。
通过检验所接收到的ARP应答报文中arp_header.ar_op项是否为ARPOP_REPLY(ARP应答)同时arp_header.ar_tip是否为已知的对方主机的IP地址来判断所得到的解析地址是否正确．
二、利用PF_PACKET协议族中SOCK_RAW类型的套接字实现ARP
(1)创建套接字
sockfd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ARP));
(2)装载报文
与SOCK_PACKET类型的套接字相同，SOCK_RAW类型的套接字对链路层操作时，也要求以太网物理帧头应作为所发送报文一部分由程序员设置.
(3)报文的发送与接收
SOCK_RAW类型套接字使用标准的物理层地址结构sockaddr_ll,所以，报文发送之前，应将套接字绑定到(使用bind()系统调用)配置好的本地物理地址结构my_etheraddr，同时还需配置目的物理地址结构broad_etheraddr.
示例如下：
struct aockaddr_ll my_etheraddr,broad_etheraddr;
my_etheraddr.sll_family=AF_PACKET;
my_etheraddr.sll_protocol=htons(ETH_P_ARP);
my_etheraddr.sll_ifindex=2; /*接口号2表示是eth0*/
my_etheraddr.sll_hatype=ARPHRD_ETHER;
my_etheraddr.sll_pkttype=PACKET_HOST;
my_etheraddr.sll_halen=ETH_ALEN;
my_etheraddr.sll_addr[8]中放入本主机的物理地址。
broad_etheraddr的配置除了sll_pkttype取PACKET_BROADCAST和sll_addr取广播物理地址(0xFFFFFF)外，其他选项与my_etheraddr配置相同。
绑定格式如下：
bind(sockfd,(struct sockaddr *)&my_etheraddr,sizeof(my_etheraddr));
发送与接收调用程序如下：
sendto(sockfd,buf,sizeof(struct ARPPACKET),0,
(struct sockaddr *)&broad_etheraddr,sizeof(broad_etheraddr));
recvform(sockfd,buf,PACKET_SIZE,&from,&fromlen);
三、利用PF_PACKET协议族中SOCK_DGRAM类型的套接字实现ARP
SOCK_DGRAM类型的套接字不要求程序员配置以太网帧头，所以所发送的报文只有数据区(ARP报文)部分，其它与SOCK_RAW类型的套接字相同。 
