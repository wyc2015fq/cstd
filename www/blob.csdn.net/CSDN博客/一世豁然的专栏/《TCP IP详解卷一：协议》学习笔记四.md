# 《TCP/IP详解卷一：协议》学习笔记四 - 一世豁然的专栏 - CSDN博客





2017年04月09日 20:09:14[一世豁然](https://me.csdn.net/Explorer_day)阅读数：356标签：[TCPIP](https://so.csdn.net/so/search/s.do?q=TCPIP&t=blog)
个人分类：[协议](https://blog.csdn.net/Explorer_day/article/category/5567509)









一、引言

1、ARP和RARP的关系

![](https://img-blog.csdn.net/20170407201354092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、ARP为IP地址到对应的硬件地址之间提供动态映射

 2）、RARP是被那些没有磁盘驱动器的系统使用，它需要系统管理员进行设置。







二、例子

1、输入“ftp 主机名”时的ARP操作

![](https://img-blog.csdn.net/20170407202503724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170407202526287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、应用程序FTP客户端把主机名转换为32 bit的IP地址。

 2）、FTP客户端请求TCP用得到的IP地址建立连接。

 3）、TCP发送一个连接请求分段到远程的主机，即用上述IP地址发送一份IP数据报。

 4）、如果目的主机在本地网络上，那么IP数据报可以直接送到目的主机上。如果目的主机在一个远程网络上，那么就通过IP选路函数来确定位于本地上的下一站路由器地址，并让它转发数据包。

 5）、假定是一个以太网，那么发送端主机必须把32 bit的IP地址变换成48 bit的以太网地址。从逻辑Internet地址到对应的物理地址需要进行翻译。

 6）、ARP发送一份称作ARP请求的以太网数据帧给以太网上的每个主机。这个过程称作广播。

 7）、目的主机的ARP层收到这份广播报文后，识别出这是发送端在询问它的IP地址，于是发送一个ARP应答。

 8）、收到ARP应答后，使ARP进行请求一应答交换的IP数据报现在可以传送了。

 9）、发送IP数据报到目的主机。




2、ARP背后有一个基本概念，那就是网络接口有一个硬件地址（一个48 bit的值，标识不同的以太网或令牌环网络接口）。点对点链路不使用ARP。







三、ARP高速缓存

1、ARP高效运行的关键是由于每个主机上都有一个ARP高速缓存。这个高速缓存存放了最近Internet地址到硬件地址之间的映射记录。高速缓存中每一项的生存时间一般为20分钟，起始时间从被创建时开始算起。







四、ARP分组格式

1、用于以太网的ARP请求或应答分组格式

![](https://img-blog.csdn.net/20170407205105829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、以太网报头的两个字段是以太网源地址和目的地址。目的地址为全1的特殊地址是广播地址，

 2）、两个字节长的以太网帧类型表示后面数据的类型。对于ARP请求或应答来说，该字段的值为0x0806。

 3）、硬件类型字段表示硬件地址的类型。它的值为1表示以太网类型。

 4）、协议类型字段表示要映射的协议地址类型。它的值为0x08000表示IP地址。它的值与包含IP数据报的以太网数据帧中的类型字段的值相同。

 5）、硬件地址长度和协议地址长度分别指向硬件地址和协议地址的长度。对于以太网上IP地址ARP请求或应答来说，它们的值分别是6和4。

 6）、OP（操作字段）指出四种操作类型，它们是ARP请求（1）、ARP应答（2）、RARP请求（3）和RARP应答（4）。




2、对于一个ARP请求来说，出目的端硬件地址外的所有其他的字段都有填充物。当系统收到一份目的端为本机的ARP请求报文后，它就把硬件地址填充进去，然后用两个目的端地址分别替换两个发送端地址，并把操作字段置位2，最后把它发送回去。







五、ARP举例

一）、一般的例子

1、执行telnet命令与无效的服务器连接

![](https://img-blog.csdn.net/20170409110001801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2、在另一个系统上执行带有-e的tcpdump命令的输出（删除了结束连接信息的最后四行）

![](https://img-blog.csdn.net/20170409110247653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、在第1行中，源端主机的硬件地址是0:0:c0:6f:2d:40。目的端主机的硬件地址ff:ff:ff:ff:ff::ff。紧接着一个输出字段是arp，表明帧类型字段的值是0x0806，说明此数据帧是一个ARP请求或应答。

 2）、在每行中，单词arp或ip后面的60表示以太网数据帧的长度。由于ARP请求或应答的数据帧长都是42字节，因此，每一帧都必须加入填充字符以达到以太网d额最小长度要求：60字节。

 3）、第1行中的下一个输出字段arp who-has表示作为ARP请求的这个数据帧中，目的IP地址是svr4的地址，发送端的IP地址是bsdi。

 4）、从第2行中可以看到，尽管ARP请求时广播的，但是ARP应答的目的地址确实bsdi。

 5）、第3行是第一个请求建立连接的TCP字段。它的目的硬件地址是目的主机（svr4）。

 6）、在每一行中，行号后面的数字表示tcpdump收到的分组时间（以秒为单位）。除第一行外，其他每行在括号中还包含了与上一行的时间差异。




二）、对不存在主机的ARP请求

1、举例

![](https://img-blog.csdn.net/20170409112148476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2、tcpdump输出

![](https://img-blog.csdn.net/20170409112228579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、多次ARP请求：第一次请求发生后5.5s进行第二次请求。在24s又进行第3次请求。

 2）、注意，在线路上始终看不到TCP的报文段。能看到的是ARP请求。直到ARP回答返回时，TCP报文才可以被发送，因为硬件地址到这时才可能知道。




三）、ARP高速缓存超时设置

1、在ARP高速缓存中的表项一般都要设置超时值。




2、从伯克利系统演变而来的系统一般对完整的表项设置超时值为20min，而对不完整的表项设置超时值为3min，当这些表项再次使用时，这些实现一般都把超时值重新设置为20min。







六、ARP代理

1、如果ARP请求是从一个网络的主机发往另一个网络的主机，那么连接这两个网络的路由器就可以回答该请求，这个过程称作委托ARP或ARP代理。

 1）、此可以欺骗发起ARP请求的发送端，让它误以为路由器就是目的主机，而实际上目的主机在路由器的另一边。

 2）、路由器的功能相当于目的主机的代理，把分组从其他主机转发给它。




2、ARP代理也称作混合ARP或ARP出租其说明ARP代理的其他用途，通过两个物理网络之间的路由器可以相互隐藏物理网络。







七、免费ARP

1、免费ARP：指的是主机发送ARP查找自己的IP地址。




2、免费ARP的作用

 1）、一个主机可以通过它来确定另一个主机是否设置了相同的IP地址。

 2）、如果发送免费ARP的主机正好改变了硬件地址，那么这个分组就可以使其他主机高速缓存中的硬件地址进行相应的更新。












