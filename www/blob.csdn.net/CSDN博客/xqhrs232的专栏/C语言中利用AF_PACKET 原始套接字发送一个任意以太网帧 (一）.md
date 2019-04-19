# C语言中利用AF_PACKET 原始套接字发送一个任意以太网帧 (一） - xqhrs232的专栏 - CSDN博客
2018年04月11日 10:07:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：266
原文地址::[http://blog.chinaunix.net/uid-16813896-id-5086439.html](http://blog.chinaunix.net/uid-16813896-id-5086439.html)
相关文章
1、[C语言中利用AF_PACKET
 原始套接字发送一个任意以太网帧 (二）](http://blog.chinaunix.net/uid-16813896-id-5087846.html)----[http://blog.chinaunix.net/uid-16813896-id-5087846.html](http://blog.chinaunix.net/uid-16813896-id-5087846.html)
# 目标
利用AF_PACKET 套接字发送一个任意的以太网帧
# 背景
以太网是一个链路层协议。大多数网络程序员关注网络栈的传输层及以上，所以不需要直接处理以太网帧，但是某些场景下关注传输层以下也是有必要的。如：
    1）实现网络协议栈里面没有内置的以太网协议类型
    2）为测试目的，产生一个畸形或者其它非常规帧
# 应用场景
假设你希望发送一个目的IP地址为192.168.0.83的ARP request报文。这个请求报文是以广播mac地址从eth0口发出
## 方法概要
    1. 选择需要的以太网类型
    2. 创建一个AF_PACKET套接字
    3. 决定使用的以太网接口的索引值
    4. 构造目的地址
    5. 发送以太网帧
以下是使用到的头文件
|Header|Used by|
|----|----|
||errno|
||memcpy, strerror, strlen|
||in_addr_t, htons|
||ETHER_ADDR_LEN, ETH_P_*|
||struct ifreq|
||struct ether_arp|
||struct sockaddr_ll|
||SIOCGIFINDEX, ioctl|
||struct sockaddr, struct iovec, struct msghdr, AF_PACKET, SOCK_DGRAM, socket, sendto, sendmsg|
    AF_PACKET是针对于Linux的。
    利用AF_PACKET套接字发送广播帧时，设置SO_BROADCAST不是必要的。有一些程序员一定设置，这个没有啥坏处，并且可以看做是推荐的，因为这样可以防止以后套接字行为上的任何变化。
### 1. 选择需要的以太网类型
以太网帧中的以太网类型指定了它包含的负载的类型。有很多途径可以获取以太网类型：
1）头文件Linux/if_ether.h 定义了大多数常用的以太网类型。包括以太网协议的ETH_P_IP(0x8000)、arp的ETH_P_ARP(0x0806)
  和IEEE 802.1Q VLAN tags的ETH_P_8021Q(0x8100)
2)IEEE维护的注册以太网类型列表
3）半官方的列表由IANA维护
ETH_P_ALL允许任何在没有使用多个套接字的情况下接受所有以太网类型的报文。
0x88b5和0x88b6是保留以太网类型，供实验或私人使用。
### 2. 创建一个AF_PACKET套接字
函数：
    socket(domain, type, protocol)
    1）domain为AF_PACKET.
    2）套接字类型：
          SOCK_DGRAM----以太网头已经构造好了
          SOCK_RAW------自己构造以太头
3）协议类型
    这里协议类型等同于前面提到的以太网类型（转换成网络字节序），用于过滤接收的报文
    在这个列子中socket是发送arp报文，protocol用hton(ETH_P_ARP),type使用SOCK_DGRAM,这样就不需要自己构造以太头了。  
点击(此处)折叠或打开
- int fd;
- 
fd = socket(AF_PACKET,SOCK_DGRAM,hton(ETH_P_ARP))
- if(fd ==-1){
- 
     perror("%s",strerror(errno));
- exit(1);
- }
### 3.  决定使用的以太网接口的索引值
点击(此处)折叠或打开
- struct ifreq ifr;
- 
size_t if_name_len = strlen(if_name);
- if(if_name_len < sizeof(ifr.ifr_name)){
- 
     memcpy(ifr.ifr_name,if_name,if_name_len);
- }else{
- 
     perror("interface name is too long");
- exit(1);
- }
- if(-1 == ioctl(fd,SIOCGIFINDEX,&ifr)){
- 
    perror("get if index error :%s",strerror(errno));
- exit(1);
- }
- int ifindx = ifr.ifr_ifindex;
### 4.  构造目的地址
为了使用AF_PACKET套接字发送一个帧时，其目的地址必须以sockaddr_ll的形式给出。
需要指定的域是sll_family,sll_addr,sll_halen,sll_ifindex和sll_protocol.其它的为0.
点击(此处)折叠或打开
- struct sockaddr_ll{
- 
    unsigned short sll_family;/* 总是 AF_PACKET */
- 
    unsigned short sll_protocol;/* 物理层的协议 */
- int sll_ifindex;/* 接口号 */
- 
    unsigned short sll_hatype;/* 报头类型 */
- 
    unsigned char sll_pkttype;/* 分组类型 */
- 
    unsigned char sll_halen;/* 地址长度 */
- 
    unsigned char sll_addr[8];/* 物理层地址 */
- };
- const unsigned char ether_broadcast_addr[]={0xff,0xff,0xff,0xff,0xff,0xff};
- 
- 
struct sockaddr_ll addr={0};
- 
addr.sll_family=AF_PACKET;
- 
addr.sll_ifindex=ifindex;
- 
addr.sll_halen=ETHER_ADDR_LEN;
- 
addr.sll_protocol=htons(ETH_P_ARP);
- 
memcpy(addr.sll_addr,ether_broadcast_addr,ETHER_ADDR_LEN);
  （在写这个文档的时候，packet(7)的帮助文档说只要提供sll_family,sll_addr,sll_halen和sll_ifindex就可以发送了，但是这个是错误的，
在打开套接字时候制定的以太网类型是用来过滤接收端报文的，而不是构造发送端报文）
笔者在实验的时候也证实了这一点，如果没有指定sll_protocol，报问将无法发送成功。在实际中只要指定如下两项就可以：
    addr.sll_ifindex = ifr.ifr_ifindex;
    addr.sll_protocol = htons(ETH_P_ARP);
### 5. 发送以太网帧
原则上，帧可以通过使用任何具备写文件描述符的函数发送，然而如果你选择自动构造链路层头的方式，那么要使用sendto或者sendmsg，以便目的地址可以被指定。这两种方式中sendmsg更灵活，但是是以更复杂的接口为代价。下面是用每一个函数的具体实现
无论你选择了哪个函数，每个函数调用将会导致一个单独报文的发送。因为这个原因，你必须在将所有负载数据包含在一个报文中，或者使用使用sendmsg提供的scatter/gather功能。
在这个特定的场景中，发送的报文是一个ARP请求报文。为了完整性，这里给出了报文构造的一个例子：
点击(此处)折叠或打开
- struct ether_arp req;
- 
req.arp_hrd = htons(ARPHRD_ETHER);
- 
req.arp_pro = htons(ETH_P_IP);
- 
req.arp_hln = ETHER_ADDR_LEN;
- 
req.arp_pln = sizeof(in_addr_t);
- 
req.arp_op = htons(ARPOP_REQUEST);
- 
memset(&req.arp_tha,0,sizeof(req.arp_tha));
- const char * target_ip_string ="1.1.1.1";
- 
struct in_addr target_ip_addr={0};
- if(!inet_aton(target_ip_string,&target_ip_addr)){
- 
    perror("%s is not a valid IP address",target_ip_string);
- exit(1);
- }
- 
memcpy(&req.arp_tpa,&target_ip_addr.s_addr,sizeof(req.arp_tpa));
#### 5.1)利用ioctl的SIOCGTIFADDR获取以太网接口的ip地址
点击(此处)折叠或打开
- if(ioctl(fd,SIOCGIFADDR,&ifr)==-1){
- 
    perror("%s",strerror(errno));
- exit(1);
- }
- 
struct sockaddr_in* ipaddr =(struct sockaddr_in*)&ifr.ifr_addr;
- 
printf("IP address: %s\n",inet_ntoa(ipaddr->sin_addr));
#### 5.2)利用ioctl的SIOCGTIFHWADDR获取以太网接口的mac地址
网络接口的硬件地址和格式取决于接口的所属，所以不能假定它是一个以太网MAC地址。可以通过检查sockaddr结构中的sa_family来决定。
如果是一个以太网接口，sa_family应该等于ARPHRD_ETHER.
点击(此处)折叠或打开
- if(ioctl(fd,SIOCGIFHWADDR,&ifr)==-1){
- 
    perror("%s",strerror(errno));
- exit(1);
- }
- if(ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER){
- 
    perror("not an Ethernet interface");
- exit(1);
- }
从ifreq结构中提取硬件地址
在检查完接口类型以后，地址可以安全地从 req.ifr_hwaddr.sa_data中提取出来。sa_data的类型是char，有符号字符型。
所以如果你希望解析它的时候应该先转换成无符号类型。一个粗鲁而直接的方法是通过(unsigned char*)进行强制转换。
点击(此处)折叠或打开
- const unsigned char * mac =(unsigned
 char*)ifr.ifr_hwaddr.sa_data;
- 
            printf("%02x:%02x:%02x:%02x:%02x:%02x\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5])
#### 5.3) 使用sendto发送帧
要调用sendto，你必须要提供帧的内容和远端地址。
点击(此处)折叠或打开
- If(sendto(fd,&req,sizeof(req),0,(struct
 sockaddr*)&addr,sizeof(addr))==-1){
- 
    perror(“%s”,strerror(errno));
- exit(1);
- }
Sento 函数的第四个参数是改变sendto行为的指定标记，这个例子中不需要使用。
Sendto的返回值是发送的字节数，或者如果出错返回-1. AF_PACKET帧是自动发送的，所以不像写TCP套接字时需要循环调用发送函数来发送分开发送的数据。
#### 5.4) 使用sendmsg发送帧
####    调用sendmsg时，除了数据包内容和远端地址外，你还必须构造一个iovec数组和一个msghdr结构
点击(此处)折叠或打开
- Struct iovec iov[1];
- 
Iov[0].iov_base=req;
- 
Iov[0].iov_len=sizeof(req);
- 
- 
Struct msghdr message;
- 
Message.msg_name =&addr;
- 
Message.msg_namelen=sizeof(addr);
- 
Message.msg_iov=iov;
- 
Message.msg_iovlen=1;
- 
Message.msg_control=0;
- 
Message.msg_controllen=0;
- If(sendmsg(fd,&message,0)==-1){
- 
    Perror(“%s”,strerror(errno));
- }
Iovec数组的目的是提供scatter/gather能力以便报文内容不需要存储在一个连续的内存区域。在这个例子里整个负载都是存储在一个buffer中，因此只需要一个数组元素。
Msghdr结构的存在使得recvmsg和sendmsg的参数个数下降到可管理的数目。在进入Sendmsg时，msghdr指定了目的地址，报文内容和辅助数据。
