# 《TCP/IP详解卷一：协议》学习笔记五 - 一世豁然的专栏 - CSDN博客





2017年04月10日 20:49:41[一世豁然](https://me.csdn.net/Explorer_day)阅读数：343








一、引言

1、具有本地磁盘的系统引导时，一般是从磁盘上的配置文件读取IP地址。但是无盘机，则需要采取其他方法来获取IP。




二、RARP分组格式

1、RARP的分组格式与ARP的分组基本一致。它们之间的主要区别是RARP请求或应答的帧类型代码为0x8035，而且RARP的操作码为3，应答操作码为4。




2、对于ARP，RARP请求以广播方式传送，而RARP应答一般是单播传送的。







三、RARP举例

1、在互联网中，我们可以强制主机sun从互联网上引导，而不是从本地磁盘引导。如果在主机bsdi上运行RARP服务程序和tcpdump命令，可以得到以下输出：

![](https://img-blog.csdn.net/20170410201130604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、RARP请求是广播方式（第1行），而第2行的RARP应答是单播方式。第2行输出的at sun表示RARP的应答包含sun主机的地址（140.252.13.33）。

 2）、在第3行中，一旦sun收到IP地址，它就发送一个TTP读请求（RRQ）给文件8CFC0D21.SUN4C。文件名中的8个十六进制数字表示主机sun的IP地址140.252.13.33，这个IP地址在RARP应答后返回，文件名的后缀SUN4C表示被引导系统的类型。

 3）、第2行中的以太网数据帧长度比最小长度还要小。其原因是我们在发送该以太网数据帧的系统上运行tcpdump命令，应用程序rarpd写42字节到BSD分组过滤器上，这就是tcpdump收到的副本。




2、在网络上没有RARP服务器时，其结果如下图：

![](https://img-blog.csdn.net/20170410203059331?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、每个分组的硬件地址都是以太网广播地址，。在who-后面的以太网地址是目的硬件地址，跟在tell后面的以太网地址是发送端硬件地址。

 2）、重发频率。







四、RARP服务器的设置

一）、作为用户进程的RARP服务器

1、RARP服务器的复杂性在于，服务器一般要为多个主机一共硬件地址到IP地址的映射。该映射包含在一个磁盘文件中。但是内核一般不都去磁盘文件，因此RARP服务器的功能就由用户进程来提供，不作为内核TCP/IP实现的部分。



2、更为复杂的是，RARP请求是作为一个特殊类型的以太网数据帧来传送的。这说明RARP服务器必须能够接收和发送这种类型的以太网类型。




二）、每个网络有多个RARP服务器

1、RARP服务器实现的一个复杂因素是RARP请求是在硬件层上进行广播的，这意味着它经过路由器转发。为了让无盘系统在RARP服务器关机的情况下也能进行引导，通常在一个网络上要提供多个RARP服务器。当服务器增加时，网络流量也会增加。




2、同时可能发送每个RARP服务器同时应答的情况，这样会增加以太网络发生冲突的概率。



