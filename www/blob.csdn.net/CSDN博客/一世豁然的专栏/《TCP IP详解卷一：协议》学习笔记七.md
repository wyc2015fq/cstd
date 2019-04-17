# 《TCP/IP详解卷一：协议》学习笔记七 - 一世豁然的专栏 - CSDN博客





2017年04月27日 21:49:39[一世豁然](https://me.csdn.net/Explorer_day)阅读数：473








一、Ping程序

1、一般来说，如果不能ping到某台主机，那就不能Telnet或FTP到那台主机。反过来，如果不能Telnet到某台主机，那么通常可以使用Ping来确定问题出在哪里。Ping程序还能测出到这台主机的往返时间，以表明该主机离我们有“多远”。




2、发送回显请求的Ping程序为客户端，而被Ping的主机位服务器。大多数的TCP/IP实现都在内核中直接支持Ping服务器——这种服务器不是一个用户进程。




3、ICMP回显请求和回显应答报文如下所示

![](https://img-blog.csdn.net/20170424202114525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、对于其他类型的ICMP查询报文，服务器必须响应标识符和序列号字段。另外，客户发送的选项数据必须回显，假设客户对这些信息都会感兴趣。




4、Unix系统在实现Ping程序时是把ICMP报文中的标识符字段置成发送进程的ID。

 1）、序列号从0开始，每发送一次新的回显请求就加1。Ping打印出返回的每个分组的序列号，允许我们查看是否有分组丢失，失序或重复。




一）、LAN输出

1、在局域网上运行Ping程序的结果输出一般有如下格式

![](https://img-blog.csdn.net/20170424203508495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、当返回ICMP回显回答时，要打印出序列号和TTL，并计算往返时间。从上图的输出中可以看出，回显应答是以发送的次序返回的。

 2）、Ping程序在ICMP报文数据中存放发送请求的时间值来计算往返时间。当应答返回时，用当前时间减去存放在ICMP报文中的时间值，即是往返时间。




2、上例中的tcpdump输出如图所示：

![](https://img-blog.csdn.net/20170426202915614?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、从发送回显请求到收到回显应答，时间间隔始终为3.7s。还可以看到，回显请求大约每隔一秒钟发送一次。

 2）、通常，第1个往返时间值要比其他的大。这是由于目的端的硬件地址不在ARP高速缓存中的缘故。




二）、wlan输出

1、在一个广域网上，结果会有很大的不同。internet具有正常通信量时的运行结果：

![](https://img-blog.csdn.net/20170426204859529?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、这里，序列号为1、2、3、4、6、10、11、12和13的回显请求或回显应答在某个地方丢失了。

 2）、通过广域网还有可能看到重复的分组（即相同的序列号的分组被打印两次获多次），失序的分组（序列号为N+1的分组在序列号为N的分组之前被打印）。




三）、线路SLIP链接

1、往返时间估算

 1）、首先，默认情况下发送的ICMP报文有56个字节。再加上20个字节的IP首部和8个字节的ICMP首部，IP数据包的总长度为84个字节。

 2）、另外，至少要添加两个额外的字节：在数据报的开始和结尾加上END字符。

 3）、此外，SLIP帧还可能再添加一些字节，但这取决于数据报中的每个字节的值。

 4）、对于1200b/s这个速率来说，由于每个字节含有8 bit数据，1 bit起始位和1bit结束位，因此传输速率是每秒120个字节。或者说每个字节8.33秒。所以估计需要1433ms（86x8.33x2，乘2是因为我们计算往返时间）。




2、实例输出

![](https://img-blog.csdn.net/20170426211713279?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





四）、拨号SLIP链路

1、影响计算参数的因素

 1）、调制解调器带来了时延。

 2）、随着数据的压缩，分组长度可能会减少，但是由于使用了错误控制协议，分组长度又可能添加。

 3）、接收端的调制解调器只能在验证了循环检验字符（检验和）后才能释放收到的数据。

 4）、还要处理每一端计算机异步串行接口，许多操作系统只能在固定的时间间隔内，或者收到若干字符后才去读这些接口。




2、实例

![](https://img-blog.csdn.net/20170426212918932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、注意，第一行的RTT不是10ms的整数倍，但是其他行都是10ms的整数倍。

 2）、另外注意，第一个RTT要比其他的大，而且一次递减，然后徘徊在280ms~300ms之间。







二、IP记录路由选项

一）、概述

1、大多数Ping程序都提供-R选项，以提供记录路由的功能。

 1）、首先，-R选项使得Ping程序在发送出去的IP数据报中设置IPRR选项（该IP数据包包括ICMP回显请求报文）。

 2）、接着，每个处理该数据报的路由器都把它的IP地址放入选项字段中。当数据报到达目的端时，IP地址清单应该复制到ICMP回显应答中，这样返回途中所经过的路由器地址也被加入到清单中。

 3）、最后，当Ping程序收到回显应答时，它就打印出这份IP地址清单。




2、Ping程序-R选项的缺点

 1）、源端主机生成RR选项，中间路由器对RR选项的处理，以及把ICMP回显请求中的RR清单复制到ICMP回显应答中，所有这些都是选项功能。绝大多数都支持这些功能，但是有的系统不把ICMP请求中的IP清单复制到ICMP应答中。

 2）、IP首部中只有有限的空间来存放IP地址。




3、IP数据报中的RR选项的一般格式

![](https://img-blog.csdn.net/20170427210139694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、Code是一个字节，指明IP选项的类型。对于RR选项来说，他的值为7。

 2）、len是RR选项总字节长度，在这种情况下为9。

 3）、ptr称作指针字段。他是一个基于1的指针，，指向存放下一个IP地址的位置。



二）、通常的例子

1、在主机svr4上运行Ping程序到主机slip。一个中间路由器将处理这个数据报，下面是A的输出结果

![](https://img-blog.csdn.net/20170427211850041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





2、分组所经过的四站如下图所示，每一站都把自己的IP地址加入RR清单：

![](https://img-blog.csdn.net/20170427212120230?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、路由器bsdi在不同方向上分别加入了不同地IP地址。它始终是把出口的IP地址加入清单。

 2）、当ICMP回显应答达到原始系统（svr4）时，他把自己的入口地址也加入到清单中。




3、tcpdump的-v选项查看主机sun上进行的分组交换

![](https://img-blog.csdn.net/20170427212650571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、输出中的optlen=40表示在IP首部中有40个字节的选项空间。

 2）、RR{39}表示记录路由选项已被设置，它的长度是39。

 3）、接着是9个IP地址，符号“#”用来标记RR选项中的ptr字段所指向的I跑地址。

 4）、路由器末尾的标记EOL表示IP地址“清单结束”的值。







三、IP时间戳选项

1、IP时间戳选项与记录路由选项相似。IP时间戳选项的格式如下图所示：

![](https://img-blog.csdn.net/20170427213739067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、时间戳选项的代码为0x44。其他两个字段len和ptr与记录路由器选项相同：选项的总长度（一般为36或40）和指向下一个可用空间的指针。

 2）、接下来的两个字段是4 bit的值：OF表示溢出字段，FL表示标志字段。时间戳选项的操作根据标志字段来进行

![](https://img-blog.csdn.net/20170427214242647?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


I、如果路由器由于没有空间而不能添加时间戳选项，那么它将添加溢出字段的值。

 3）、与记录路由选项所受到的限制相比，时间戳选项遇到的情况要更坏一些。






