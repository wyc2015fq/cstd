# TCP/IP学习笔记（二） - 深之JohnChen的专栏 - CSDN博客

2006年04月13日 09:48:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1832


**TCP/IP学习笔记（二）**

一、 ARP：地址解析协议
    先看：
    C:/Documents and Settings/xiaoj>arp -a
    Interface: 192.1.8.84 on Interface 0x1000003
    Internet Address Physical Address Type
    192.1.8.12 00-0b-cd-03-c4-27 dynamic
    192.1.8.15 00-0d-9d-93-09-f5 dynamic
    192.1.8.18 00-e0-18-c1-86-3d dynamic
    192.1.8.26 00-80-2d-78-4f-81 dynamic
    192.1.8.33 00-0b-cd-0f-a4-c5 dynamic
    192.1.8.38 00-30-6e-36-5f-99 dynamic
    192.1.8.86 00-0b-cd-b8-0c-ae dynamic
    192.1.8.247 00-50-ba-e5-20-af dynamic
    192.1.8.248 00-0d-56-19-ba-56 dynamic
    第一列显示的是主机的IP地址，第二列就是主机的网卡MAC地址。
    ARP协议就是提供IP地址到对应的硬件地址提供动态映射。这个过程一般是系统自动完成的。ARP发送一份ARP请求给以太网上的主机,该请求中包含目的主机的IP地址，其意思是“如果你是这个IP的拥有者，请回答你的硬件地址”。当目的主机的ARP层收到这份广播报文时，识别出这是发送端在询问它的IP地址，于是就会发送一个ARP应答，这个ARP应答包括IP地址及对应的硬件地址。

二、 ARP分组格式 

|以太网目的地址6|以太网源地址6|帧类型2|硬件类型2|协议类型2|硬件地址长度1|协议地址长度1|请求应答1|发送者硬件地址6|发送者IP地址4|目的硬件地址6|目的IP地址4|

先看定义：
//ARP首部
struct arphdr(
  u_short ar_hrd; //硬件类型
  u_short ar_pro; //协议类型
  u_char ar_hln; //硬件地址长度
  u_char ar_pln; //协议地址长度
  u_short ar_op; // 请求还是应答

  u_char arp_sha[6]; //发送者硬件地址
  u_char arp_spa[4]; //发送者IP地址
  u_char arp_tha[6]; //目的硬件地址
  u_char arp_tpa[4]; //目的IP地址
  u_char arp_zero[18]; //填充字段
};
说明：◆以太网目的地址全为1的特殊地址为广播地址。
      ◆ 以太网帧类型ARP对应的值为0x0806。
      ◆ 硬件类型表示硬件地址的类型，以太网为1。
      ◆ 协议类型字段表示要映射的协议地址类型，IP地址对应的值为0x0800。
      ◆ 硬件地址长度：以太网为6。
      ◆ 协议地址长度：以太网为4，即IP地址长度。
      ◆ 操作字段：ARP请求＝1，ARP应答＝2，RARP请求＝3，RARP应答＝4。
      ◆ 当系统收到一份目的端为本机的ARP请求报文时，它就把硬件地址填进去，然后用两个目的端地址分别替换两个发送端地址，并把操作字段置为2，最后把它发送出去。
三、 ARP代理：
    如果ARP请求是从一个网络的主机发往另一个网络的主机，那么连接这两个网络的路由器可以回答该请求，这个过程称为ARP代理，这样可以欺骗发起ARP请求的发送端，使它误以为路由器就是目的主机。
四、 免费ARP
     指主机启动时发送ARP查找自己的IP地址，主要作用：
     1、 查找网络上有没有相同的IP地址。
     2、 更新ARP缓存。
五、 RARP协议：逆地址解析协议，这里就不详细介绍了。
     RARP协议一般用于无盘工作站，根据硬件地址查找IP地址。
     RARP分组格式与ARP分组格式相同。
六、 ICMP协议：Internet控制报文协议：
     ICMP经常被认为是IP层的一个组成部分，它传递差错报文以及其他需要注意的事项。ICMP报文通常被IP层或更高层协议（TCP或UDP）使用。ICMP报文是在IP数据报内部被传输的。ICMP报文包差错报文和查询报文。报文格式如下：

|8位类型|8位代码|16位检验和|
|----|----|----|
|不同类型和代码有不同的内容| | |

    类型字段可以有15个不同的值（0、3－5、8－18）。某些报文还使用代码字段来进一步描述不同的条件。检验和字段覆盖整个ICMP报文，与IP首部检验和算法是一样的。

    以下情况不会产生ICMP差错报文：
    ◆ ICMP差错报文
    ◆ 目的地址为广播地址或者多播地址
    ◆ 作为链路层的数据报
    ◆ 不是IP分片的第一片
    ◆ 源地址不是单个主机的数据报，也即源地址不能为0地址、环回地址、广播地址或多播地址

七、 ICMP地址掩码请求与应答：
     ICMP地址掩码请求用于无盘系统在引导过程中获取自己的子网掩码，系统广播它的ICMP请求报文。ICMP地址掩码请求和应答报文格式如下： 

|类型（17或18）|代码（0）|检验和|
|----|----|----|
|标识符|序列号| |
|32位子网掩码| | |

ICMP报文中的标识符和序列号由发送端任意选择设定，这些值在应答中将被返回。
struct icmp_mask{
  unsigned char icmp_type; //类型
  unsigned char icmp_code; //代码
  unsigned short icmp_checksum; //检验和
  unsigned short icmp_id; //标识符
  unsigned short icmp_sequence; //序列号
  unsigned long icmp_mask; //32位子网掩码
};
说明：RFC规定，除非是地址掩码的授权代理，否则不能发送地址掩码应答。
向本机IP地址和环回地址发送地址掩码请求结果是一样的。

八、 ICMP时间戳请求与应答：
     ICMP时间戳请求允许系统向另一个系统查询当前时间，返回的是自午夜开始记算的毫秒数。调用者必须通过其他方法获取当前时间。
     ICMP时间戳请求与应答报文格式如下：

|类型（13或14）|代码（0）|检验和|
|----|----|----|
|标识符|序列号| |
|发起时间戳| | |
|接收时间戳| | |
|传送时间戳| | |

    请求端填写发起时间戳，然后发送报文。应答系统收到报文填写接收时间戳，发送应答时填写发送时间戳。实际上，大多数实现将后两个字段一般设置为一样的。
struct icmp_time{
  unsigned char icmp_type; //类型
  unsigned char icmp_code; //代码
  unsigned short icmp_checksum; //检验和
  unsigned short icmp_id; //标识符
  unsigned short icmp_sequence; //序列号
  unsigned long icmp_request_time; //发起时间戳
  unsigned long icmp_receive_time; //接收时间戳
  unsigned long icmp_send_time; //传送时间戳
};
九、 ICMP端口不可达差错
    主机如果收到一份UDP数据报而目的端口与某个正在使用的进程，那么UDP返回一个ICMP不可达报文。报文格式如下：

|类型3|代码（0－15）|检验和|
|----|----|----|
|保留（必须为0）| | |
|IP首部＋原始IP数据报中数据的前8个字节| | |

当代码为4时，路径MTU发现机制允许路由器把外出接口的MTU填在这个32bit的低16bit中。
struct icmp_unreach{
  unsigned char icmp_type; //类型
  unsigned char icmp_code; //代码
  unsigned short icmp_checksum; //检验和
  unsigned long icmp_zero; //保留
  unsigned char icmp_ip[28]; //IP首部＋原始IP数据报中前8个字节，也就是地址信息
}

