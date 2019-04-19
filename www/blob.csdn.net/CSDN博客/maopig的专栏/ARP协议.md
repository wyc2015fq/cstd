# ARP协议 - maopig的专栏 - CSDN博客
2012年02月28日 18:16:36[maopig](https://me.csdn.net/maopig)阅读数：890标签：[路由器																[网络																[工作																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=路由器&t=blog)
个人分类：[网络																[c/C++](https://blog.csdn.net/maopig/article/category/847675)](https://blog.csdn.net/maopig/article/category/869532)
**ARP协议**（**A**ddress **R**esolution
**P**rotocol），或称**地址解析协议**。ARP协议的基本功能就是通过目标设备的IP地址，查询目标设备的MAC地址，以保证通信的顺利进行。他是[IPv4](http://zh.wikipedia.org/wiki/IPv4)中网络层必不可少的协议，不过在[IPv6](http://zh.wikipedia.org/wiki/IPv6)中已不再适用，并被[icmp
 v6](http://zh.wikipedia.org/wiki/%E4%BA%92%E8%81%94%E7%BD%91%E6%8E%A7%E5%88%B6%E6%B6%88%E6%81%AF%E5%8D%8F%E8%AE%AE)所替代。
## 目录
- [1基本功能](http://zh.wikipedia.org/wiki/ARP#.E5.9F.BA.E6.9C.AC.E5.8A.9F.E8.83.BD)
- [2原理](http://zh.wikipedia.org/wiki/ARP#.E5.8E.9F.E7.90.86)
- [3另见](http://zh.wikipedia.org/wiki/ARP#.E5.8F.A6.E8.A6.8B)
- [4外部链接](http://zh.wikipedia.org/wiki/ARP#.E5.A4.96.E9.83.A8.E9.93.BE.E6.8E.A5)
## [[编辑](http://zh.wikipedia.org/w/index.php?title=%E5%9C%B0%E5%9D%80%E8%A7%A3%E6%9E%90%E5%8D%8F%E8%AE%AE&action=edit&section=1)]基本功能
在[以太网](http://zh.wikipedia.org/wiki/%E4%BB%A5%E5%A4%AA%E7%BD%91)协议中规定，同一局域网中的一台主机要和另一台主机进行直接通信，必须要知道目标主机的MAC地址。而在TCP/IP协议栈中，网络层和传输层只关心目标主机的IP地址。这就导致在以太网中使用IP协议时，数据链路层的以太网协议接到上层IP协议提供的数据中，只包含目的主机的IP地址。于是需要一种方法，根据目的主机的IP地址，获得其MAC地址。这就是ARP协议要做的事情。所谓**地址解析（address resolution）**就是主机在发送帧前将目标IP地址转换成目标MAC地址的过程。
另外，当发送主机和目的主机不在同一个[局域网](http://zh.wikipedia.org/wiki/%E5%B1%80%E5%9F%9F%E7%BD%91)中时，即便知道目的主机的MAC地址，两者也不能直接通信，必须经过[路由](http://zh.wikipedia.org/wiki/%E8%B7%AF%E7%94%B1)转发才可以。所以此时，发送主机通过ARP协议获得的将不是目的主机的真实MAC地址，而是一台可以通往局域网外的路由器的MAC地址。于是此后发送主机发往目的主机的所有帧，都将发往该路由器，通过它向外发送。这种情况称为**ARP代理（ARP Proxy）**。
## [[编辑](http://zh.wikipedia.org/w/index.php?title=%E5%9C%B0%E5%9D%80%E8%A7%A3%E6%9E%90%E5%8D%8F%E8%AE%AE&action=edit&section=2)]原理
在每台安装有[TCP/IP协议](http://zh.wikipedia.org/wiki/TCP/IP%E5%8D%8F%E8%AE%AE)的电脑或路由器里都有一个ARP缓存表，表里的IP地址与MAC地址是一对应的，如下表所示。
|主机名称|IP地址|MAC地址|
|----|----|----|
|A|192.168.38.10|00-AA-00-62-D2-02|
|B|192.168.38.11|00-BB-00-62-C2-02|
|C|192.168.38.12|00-CC-00-62-C2-02|
|D|192.168.38.13|00-DD-00-62-C2-02|
|E|192.168.38.14|00-EE-00-62-C2-02|
|...|...| |
以主机A（192.168.38.10）向主机B（192.168.38.11）发送数据为例。当发送数据时，主机A会在自己的ARP缓存表中寻找是否有目标IP地址。如果找到了，也就知道了目标MAC地址为（00-BB-00-62-C2-02）， 直接把目标MAC地址写入帧里面发送就可以了；如果在ARP缓存表中没有找到相对应的IP地址，主机A就会在网络上发送一个广播（ARP request），目标MAC地址是“FF.FF.FF.FF.FF.FF”，这表示向同一网段内的所有主机发出这样的询 问：“192.168.38.11的[MAC地址](http://zh.wikipedia.org/wiki/MAC%E5%9C%B0%E5%9D%80)是什么？”网络上其他主机并不响应ARP询问，只有主机B接收到这个帧时，才向主机A做出这样的回应（ARP
 response）：“192.168.38.11的MAC地址是（00-BB-00-62-C2-02）”。 这样，主机A就知道了主机B的MAC地址，它就可以向主机B发送信息了。同时它还更新了自己的ARP缓存表，下次再向主机B发送信息时，直接从ARP缓存 表里查找就可以了。ARP缓存表采用了老化机制，在一段时间内如果表中的某一行没有使用，就会被删除，这样可以大大减少ARP缓存表的长度，加快查询速 度。
2. ARP协议
还记得数据链路层的以太网的协议中，每一个数据包都有一个MAC地址头么？我们知道每一块以太网卡都有一个MAC地址，这个地址是唯一的，那么IP包是如何知道这个MAC地址的？
这就是ARP协议的工作。
ARP（地址解析）协议是一种解析协议，本来主机是完全不知道这个IP对应的是哪个主机的哪个接口，当主机要发送一个IP包的时候，会首先查一下自己的ARP高速缓存
（就是一个IP-MAC地址对应表缓存），如果查询的IP－MAC值对不存在，那么主机就向网络发送一个ARP协议广播包，这个广播包里面就有待查询的IP地址，而直接收到这
份广播的包的所有主机都会查询自己的IP地址，如果收到广播包的某一个主机发现自己符合条件，那么就准备好一个包含自己的MAC地址的ARP包传送给发送ARP广播的主机，
而广播主机拿到ARP包后会更新自己的ARP缓存（就是存放IP-MAC对应表的地方）。发送广播的主机就会用新的ARP缓存数据准备好数据链路层的的数据包发送工作。
一个典型的arp缓存信息如下，在任意一个系统里面用“arp -a”命令: 
