# 《TCP/IP详解卷一：协议》学习笔记六 - 一世豁然的专栏 - CSDN博客





2017年04月18日 20:48:27[一世豁然](https://me.csdn.net/Explorer_day)阅读数：442标签：[TCPIP](https://so.csdn.net/so/search/s.do?q=TCPIP&t=blog)
个人分类：[协议](https://blog.csdn.net/Explorer_day/article/category/5567509)









一、引言

1、ICMP经常被认为是IP层的一个组成部分。它传递差错报文以及其他需要注意的信息。ICMP报文通常被IP层或更高层协议（TCP或UDP）使用。一些ICMP报文把差错报文返回给用户进程。




2、ICMP报文是在IP数据报内部被传输的：

![](https://img-blog.csdn.net/20170414191233143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3、ICMP报文的格式如下：

![](https://img-blog.csdn.net/20170414191324769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、所有报文前4个字节都是一样的，但是剩下的其他字节则互不相同。

 2）、类型字段可以有15个不同的值，以描述特定类型的ICMP报文。某些ICMP报文还使用代码字段的值来进一步描述不同的条件。ICMP的校检和是必须的。







二、ICMP报文的类型

1、各种类型的ICMP报文如下在所示：

![](https://img-blog.csdn.net/20170414192106737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170414192112389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2、ICMP报文的不同类型由报文中的类型字段和代码字段来共同决定。图中最后两列表明ICMP报文时一份查询报文还是一份差错报文。




3、下面各种情况都不会导致产生ICMP差错报文。

 1）、ICMP差错报文（但是，ICMP查询报文可能会产生差错报文）。

 2）、目的地址是广播地址或多播地址的IP数据报。

 3）、作为链路层广播的数据包。

 4）、不是IP分片的第一片。

 5）、源地址不是单个主机的数据报。这就是说，源地址不能为零地址，环回地址，广播地址或多播地址。







三、ICMP地址掩码请求与应答

1、ICMP时间戳请求允许系统向另一个系统查询当前时间。返回的建议值是自午夜开始计算的毫秒数，协调的同一时间（UTC）。




2、ICMP时间戳请求和应答报文格式如下图：

![](https://img-blog.csdn.net/20170418195041015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


  1）、请求端填写发起时间戳，然后发送报文。应答系统收到请求报文时填写接收时间戳，在发送应答时填写发送时间戳。但是，实际上，大多数的实现后面两个字段都设成相同的值。




一）、举例

1、icmptime小程序发送ICMP时间戳请求

![](https://img-blog.csdn.net/20170418195940301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、icmptime打印报文中的三个时间戳：发起时间戳（orig），接收时间戳（recv），发送时间戳（xmit）。

 2）、计算往返时间（rtt），它的值是收到应答时的时间值减去发送请求时的时间值。difference的值是接收时间戳的值减去发起时间戳的值。

![](https://img-blog.csdn.net/20170418200642774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：时间戳是自午夜开始计算的毫秒数。




二）、另一种方法

1、获取时间和日期的另一种方法

 1）、本书所阐述的日期服务程序和时间服务程序。

 2）、严格的计时使用网络时间协议（NTP）。

 3）、开发软件基金会的分布式计算环境定义了分布式时间服务，它也提供计算机之间的时钟同步。

 4）、Unix系统提供时间守护程序timed（8），来同步局域网上的系统时钟。







四、ICMP端口不可达错误

1、UDP的规则之一是，如果收到一份UDP数据报而目的端口与某个正在使用的进程不相符，那么UDP返回一个ICMP不可达报文。




2、TFTP强制生成的端口不可达错误

![](https://img-blog.csdn.net/20170418202140219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、在UDP数据报到达主机之前，要先发送一份ARP请求来确定它的硬件地址（第1行）。接着返回ARP应答（第2行），然后才发送UP数据报。

 2）、一个端口不可达错误是立即返回的（第4行），但是TFTP客户端看上去似乎忽略了这个ICMP报文，而在5秒之后又发送了另一份UDP数据报。

 3）、注意ICMP报文是在主机之间交换的，而不用目的端口号，而每20个字节的UDP数据报则是从一个特定的端口（2924）发送到另一个特定的端口。




3、ICMP的一个规则是，ICMP差错报文必须包含生成该错报文的数据报IP首部（包含任何选项），还必须至少包括跟在该IP首部后面的前8个字节。




4、ICMP不可达报文的一般格式

![](https://img-blog.csdn.net/20170418203421209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、尽管图指出了在ICMP报文中的第二个32 bit字必须为0，但是当代码为4时（需要分片但设置了不分片比特），路径MTU发现机制却允许路由器把外出接口的MTU填在这个32 bit字的第16 bit中。




5、Tcpdump时间系列

![](https://img-blog.csdn.net/20170418204438776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170418204442388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








五、ICMP报文的4.4BSD处理

![](https://img-blog.csdn.net/20170418204654152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170418204659204?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







