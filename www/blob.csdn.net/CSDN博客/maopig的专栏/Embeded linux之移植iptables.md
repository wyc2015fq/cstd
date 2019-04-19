# Embeded linux之移植iptables - maopig的专栏 - CSDN博客
2017年09月20日 15:20:17[maopig](https://me.csdn.net/maopig)阅读数：924
# 一、内核环境：
linux-3.4.35
-*- Networking support  --->
　　Networking options  --->
　　　　[*] Network packet filtering framework (Netfilter)  --->
　　　　　　IP: Netfilter Configuration  --->
　　　　　　　　<*> IP tables support (required for filtering/masq/NAT) //iptables
　　　　　　　　<*> "ah" match support //允许对IPSec包头的AH字段进行匹配 
　　　　　　　　<*> "ecn" match support //允许对TCP/IP包头的ECN(Explicit Congestion Notification)字段进行匹配.ECN是一种显式拥塞通知技术,它不但要求路由器支持而且要求端到端主机的支持,其基本思想是当路由器发生早期拥塞时不是丢弃包而是尽量对包进行标记,接收方接到带有ECN提示的包时,通知发送方网络即将发生拥塞,也就是它通过对包的标记提示TCP源即将发生拥塞,从而引发拥塞避免算法 
　　　　　　　　<*> "rpfilter" reverse path filter match support //rpfilter反向路径过滤
　　　　　　　　<*> "ttl" match support //允许对ip包头的TTL(生存期)字段进行匹配 
　　　　　　　　<*> Packet filtering //定义filter表以允许对包进行过滤 
　　　　　　　　<*> REJECT target support //允许返回一个ICMP错误而不是简单的丢弃包 
　　　　　　　　[*] Force socket error when rejecting with icmp* 
　　　　　　　　<*> ULOG target support //透过netlink socket将符合条件的封包交给用户空间的ulogd守护进程.反对使用该选项,因为它已经被NETFILTER_NETLINK_LOG代替 
　　　　　　　　<*> Packet mangling //在iptables中启用mangle表以便对包进行各种修改,常用于改变包的路由 
　　　　　　　　<*> ECN target support 
　　　　　　　　<*> "TTL" target support 
　　　　　　　　<*> raw table support (required for NOTRACK/TRACE) //在iptables中添加一个'raw'表,该表在netfilter框架中非常靠前,并在PREROUTING和OUTPUT链上有钩子,从而可以对收到的数据包在连接跟踪前进行处理
二、软件移植
基于：iptables-1.4.22.tar.bz2
解压：tar xvf iptables-1.4.22.tar.bz2
配置：./configure --host=arm-hisiv400-linux --enable-static --disable-shared --prefix=/home/liuzhengwu/share/Source/App/iptables/iptables-1.4.12/_install
 --disable-ipv6 --disable-largefile
编译：make -j4
　　如果编译报错：
![](http://images2015.cnblogs.com/blog/760772/201706/760772-20170617170730228-1178031769.png)
　　解决方法：sed  -i  '/if_packet/i#define __aligned_u64 __u64 __attribute__((aligned(8)))'    extensions/libxt_pkttype.c
安装：make install
去符号表：arm-hisiv400-linux-strip _install/sbin/xtables-multi
拷贝给开发板：mv _install/sbin/xtables-multi /开发板/rootfs/usr/sbin/iptables
//---------------------------------------------------------------------------------------------------------------------//
基于：iptables-1.6.1.tar.bz2
解压：tar xvf iptables-1.6.1.tar.bz2
配置：./configure --host=arm-hi3516cv300-linux --enable-static --disable-shared --prefix=/home/liuzhengwu/share/Source/App/iptables/iptables-1.6.1/_install
 --disable-ipv6 --disable-largefile --disable-nftables
编译：make -j4
安装：make install
去符号表：arm-hi3516cv300-linux-strip _install/sbin/xtables-multi
拷贝给开发板：mv _install/sbin/xtables-multi /开发板/rootfs/usr/sbin/iptables
ERROR: ld.so: object '/media/work/cz/work/WiFi_Evaluate/BRCM/hangshen/wpa_supplicant_build_new/need_libs/openssl-1.0.1g/util/../libcrypto.so.1.0.0' from LD_PRELOAD cannot be preloaded (wrong ELF class: ELFCLASS32): ignored.
解决方法：
```
# echo "" > /etc/ld.so.preload
```
64位系统没有这个文件导致报错
