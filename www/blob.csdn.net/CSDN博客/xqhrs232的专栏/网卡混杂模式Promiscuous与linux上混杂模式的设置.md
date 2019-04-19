# 网卡混杂模式Promiscuous与linux上混杂模式的设置 - xqhrs232的专栏 - CSDN博客
2017年08月16日 17:11:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：474
原文地址::[http://blog.csdn.net/bytxl/article/details/46862207](http://blog.csdn.net/bytxl/article/details/46862207)
相关文章
1、Linux下网卡混杂模式设置和取消----[http://www.linuxidc.com/Linux/2016-07/132890.htm](http://www.linuxidc.com/Linux/2016-07/132890.htm)
# 混杂模式
混杂模式就是接收所有经过网卡的数据包，包括不是发给本机的包，即不验证MAC地址。普通模式下网卡只接收发给本机的包（包括广播包）传递给上层程序，其它的包一律丢弃。
一般来说，混杂模式不会影响网卡的正常工作，多在网络监听工具上使用。
以太网（Ethernet）具有共享介质的特征，信息是以明文的形式在网络上传输，当网络适配器设置为监听模式（混杂模式，Promiscuous）时，由于采用以太网广播信道争用的方式，使得监听系统与正常通信的网络能够并联连接，并可以捕获任何一个在同一冲突域上传输的数据包。IEEE802.3 标准的以太网采用的是持续
 CSMA 的方式，正是由于以太网采用这种广播信道争用的方式，使得各个站点可以获得其他站点发送的数据。运用这一原理使信息捕获系统能够拦截的我们所要的信息，这是捕获数据包的物理基础。
以太网是一种总线型的网络，从逻辑上来看是由一条总线和多个连接在总线上的站点所组成各个站点采用上面提到的 CSMA/CD 协议进行信道的争用和共享。每个站点（这里特指计算机通过的接口卡）网卡来实现这种功能。网卡主要的工作是完成对于总线当前状态的探测，确定是否进行数据的传送，判断每个物理数据帧目的地是否为本站地址，如果不匹配，则说明不是发送到本站的而将它丢弃。如果是的话，接收该数据帧，进行物理数据帧的 CRC 校验，然后将数据帧提交给LLC 子层。
网卡具有如下的几种工作模式：
1) 广播模式（Broad Cast Model）:它的物理地址（MAC）地址是 0Xffffff 的帧为广播帧，工作在广播模式的网卡接收广播帧。
2）多播传送（MultiCast Model）：多播传送地址作为目的物理地址的帧可以被组内的其它主机同时接收，而组外主机却接收不到。但是，如果将网卡设置为多播传送模式，它可以接收所有的多播传送帧，而不论它是不是组内成员。
3）直接模式（Direct Model）:工作在直接模式下的网卡只接收目地址是自己 Mac地址的帧。
4）混杂模式（Promiscuous Model）:工作在混杂模式下的网卡接收所有的流过网卡的帧，信包捕获程序就是在这种模式下运行的。
网卡的缺省工作模式包含广播模式和直接模式，即它只接收广播帧和发给自己的帧。如果采用混杂模式，一个站点的网卡将接受同一网络内所有站点所发送的数据包这样就可以到达对于网络信息监视捕获的目的。
# 集线器与交换机环境下混杂模式
![](https://img-blog.csdn.net/20150713144124179)
在集线器模式下，源主机“主机2”要向目标主机“主机3”发送数据，当数据到达集线器后，集线器会向除了源主机“主机2”本身之外的所有主机发送数据，即除了源主机“主机2”之外的所有主机都可以收到“主机2”所发的数据。目标主机“主机3”判断到该数据是给自己的，就进行处理；其余主机判断到所发的数据是给“主机3”的，而不是给自己的，就不进行处理。该模式下，硬件设计简单、速度快，并且对主机的开机（新主机加入）和关机（当前在线主机的离开）都具有很好的适应性，只要发送的时候，目标主机在线，就可以马上进入工作状态。但缺点是该模式对冲突的解决是非常麻烦的，例如，当“主机2”向某一台主机发送数据的期间，“主机4”也再向另一台主机发送数据，则二者的数据会出现冲突，这种冲突会导致二主机所发的数据全部出错。解决办法就是二者各“等”一会儿再重发，但再重发并不能避免产生新的冲突。这种现象在频繁数据交换的时候，冲突的现象非常明显，会大大减低网络带宽，现在这种模式已被交换机模式所代替。
在交换机模式下，源主机“主机2”要向目标主机“主机3”发送数据，当数据到达交换机后，交换机会分析目标主机“主机3”所在的端口，然后只向该端口发送数据，这样，其余主机就接不到由源主机“主机2”所发出来的。这样，使得别的主机就不需要处理不属于自己的数据，从而减少了负荷，而且如果此前“主机5”要向“主机1”发送数据时，就不会出现冲突。在该模式下，由于有效地减少了“冲突”的可能，所以整个带宽的利用率大大提高，并且各个主机也减少了处理不属于自己数据包的工作。但这样做也有一些缺点，比如新主机的加入和当前在线主机的离开，通常都需要交换机花费一定时间来判断新的变化。当前绝大多数以太网所使用的都是交换机模式。
通过上面的分析，不难看出，当前以交换机作为交换设备的网络中，即使是将某一个网卡设置成“混杂模式”，除非使用一些特殊技术，网卡本身一般是接不到不是发送给自身的数据，所接到的只是给本身IP的数据和广播数据，但这时RAW Socket可以监听所有的数据了。这点与“常规模式”不同
。
# linux下混杂模式设置
## 命令方式设置
ifconfig eth1 promisc  设置混杂模式
ifconfig eth1 -promisc 取消混杂模式
执行结果如下
[root@localhost tftpboot]# ifconfig
eth6      Link encap:Ethernet  HWaddr 08:00:27:70:1D:79  
          inet6 addr: fe80::a00:27ff:fe70:1d79/64 Scope:Link
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:100124 errors:0 dropped:0 overruns:0 frame:0
          TX packets:8795 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:12986638 (12.3 MiB)  TX bytes:6452270 (6.1 MiB)
lo        Link encap:Local Loopback  
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:16436  Metric:1
          RX packets:1303 errors:0 dropped:0 overruns:0 frame:0
          TX packets:1303 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0 
          RX bytes:303973 (296.8 KiB)  TX bytes:303973 (296.8 KiB)
[root@localhost tftpboot]# ifconfig eth6 promisc
[root@localhost tftpboot]# ifconfig
eth6      Link encap:Ethernet  HWaddr 08:00:27:70:1D:79  
          inet6 addr: fe80::a00:27ff:fe70:1d79/64 Scope:Link
          UP BROADCAST RUNNING PROMISC MULTICAST  MTU:1500  Metric:1
          RX packets:100154 errors:0 dropped:0 overruns:0 frame:0
          TX packets:8795 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:13007885 (12.4 MiB)  TX bytes:6452270 (6.1 MiB)
lo        Link encap:Local Loopback  
          inet addr:127.0.0.1  Mask:255.0.0.0
          inet6 addr: ::1/128 Scope:Host
          UP LOOPBACK RUNNING  MTU:16436  Metric:1
          RX packets:1303 errors:0 dropped:0 overruns:0 frame:0
          TX packets:1303 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:0 
          RX bytes:303973 (296.8 KiB)  TX bytes:303973 (296.8 KiB)
[root@localhost tftpboot]# 
## C语言编程方式设置
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <string.h>
#define ETH_NAME    "eth1"
int do_promisc(void) {
    int f, s;
    struct ifreq ifr;
    if ( (f=socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))<0){ 
        return -1;
    }
    strcpy(ifr.ifr_name, ETH_NAME);
    if ((s = ioctl(f, SIOCGIFFLAGS, &ifr))<0){
      close(f);
      return-1;
    }
    if(ifr.ifr_flags & IFF_RUNNING){
        printf("eth link up\n");
    }else{
        printf("eth link down\n");
    }
    ifr.ifr_flags |= IFF_PROMISC;
    if ((s = ioctl(f, SIOCSIFFLAGS, &ifr)) < 0){
      return -1;
    }
    printf("Setting interface ::: %s ::: to promisc\n\n", ifr.ifr_name);
    return 0;
}
int check_nic(void)
{
    struct ifreq ifr;
    int skfd = socket(AF_INET, SOCK_DGRAM, 0);
    strcpy(ifr.ifr_name, ETH_NAME);
    if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
    {
        close(skfd);
        return -1;
    }
    if(ifr.ifr_flags & IFF_RUNNING){
        printf("link up\n");
        close(skfd);
        return 0; // 网卡已插上网线
    }else {
        printf("link down\n");
        close(skfd);
        return -1;
    }
} 
int main(void) {
    do_promisc();
    return 0;
}
[http://blog.csdn.net/abc78400123/article/details/6851566](http://blog.csdn.net/abc78400123/article/details/6851566)
[http://book.51cto.com/art/201202/316585.htm](http://book.51cto.com/art/201202/316585.htm)
[http://www.2cto.com/net/201212/177509.html](http://www.2cto.com/net/201212/177509.html)
