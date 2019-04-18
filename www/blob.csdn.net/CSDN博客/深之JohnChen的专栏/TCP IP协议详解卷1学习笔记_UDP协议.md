# TCP/IP协议详解卷1学习笔记_UDP协议  - 深之JohnChen的专栏 - CSDN博客

2006年04月13日 10:07:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2280标签：[服务器																[路由器																[header																[socket																[多线程																[tcp](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=路由器&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[网络编程](https://blog.csdn.net/byxdaz/article/category/166215)


UDP是无连接。基本格式：
+---- 20 bytes ----+--- 8 bytes ----+------- n bytes-----+
+    IP header     +   UDP header   +        Data        +
+------------------+----------------+--------------------+
每次发送即产生一个UDP数据报。

UDP头结构：
+--- 16 bits ------+--- 16 bits ------+
+  Source Port     +    Dest Port     +
+------------------+------------------+
+     length       +      Checksum    +
+------------------+------------------+
+             data(if any)            +
+-------------------------------------+
UDP长度是UDP头和数据域的字节长度。最小为8字节（即前8字节，没有数据）。
UDP的校验和覆盖UDP首部和数据域。与IP不同，IP头校验仅覆盖IP头。与TCP不同，UDP的校验是可选的。但是一般应该加上。计算校验和时，算的是（USHORT,unsigned short)，即16位。而UDP加上数据域后，可能是奇数位。这时需要在UDP数据域后填充为0的字节以计算校验和。
另外在计算校验和时，TCP和UDP都有一个伪头，主要是用来计算校验和的方便。
UDP的伪头格式：
+---- 16 bits ----+---- 16 bits ----+
+       32 bits Source Addr         +
+-----------------------------------+
+       32 bits Dest Addr           +
+--------+--------+-----------------+
+   0    +   17   +   Udp length    +
+--------+--------+-----------------+
17 为UDP协议的类型码

如果不需要修改UDP头的内容，只是希望利用UDP的通信功能是很简单的。以下流程即可（不是实际源码）：

服务器端： 

> WSAStartup();
sck=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
sin.sin_family =AF_INET;
sin.sin_addr.S_un.S_addr =htonl(INADDR_ANY);
sin.sin_port =htons(51888);
bind(sck,sin,sizeof(sin));
while(TRUE)
{
  recvfrom(sck,buf,256,0,&from,&flen);
  sendto(...);
  其他处理
}
closesocket(sck);
WSACleanup();

客户端： 

> WSAStartup();
sck=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
sin.sin_family =AF_INET;
sin.sin_addr.S_un.S_addr =inet_addr("192.168.1.x");
sin.sin_port =htons(51888);
while(TRUE)
{
  sendto(sck,buf....);
  recvfrom(...);
  其他处理
}
closesocket(sck);
WSACleanup();
服务器和客户端都是简单的框架，填上一些自定义的处理过程就可以了。
I/O是阻塞的，意味着在调用 recvfrom()时，如果缓冲区内还没有数据，程序将一直等到收到数据才返回。sendto()也类似。采用异步I/O可以提高效率，多线程也可以用于提高效率。但由于本章内容并不是讲述如何使用UDP来通信，因此不继续发挥下去。

后部分讨论了IP数据报的分片。分片是由于要传送的数据太长，超过传输链路的MTU（最大传输单元）而产生的。不仅在发送数据报的源端可能分片，途经的每一个路由器都可能根据链路情况分片。所有的分片在接收端才重新组装起来。
可以阻止IP层的分片，通过设置IP数据报的 "DF”不可分片标志来实现。这样IP层在收到一个太长而需要分片的数据时，不主动分片，而是返回一个错误，报告数据需要事先分片。可以利用这一点来实现查找通信链路的最小MTU。预先设定一个较大MTU，如果收到ICMP错误报文，则减小MTU继续发送，直到收不到错误报文为止。获知MTU可以帮助在应用程序端预先分片，减小传送一个大报文时，由于某一个分片的遗失而需要重传整个数据报的风险.

==> 需要明确的几个术语：

　　1、IP数据报 — 是指IP层端到端的传输单元（在分片之前和重新组装之后）；

　　2、IP分组 — 是指在IP层和链路层之间传送的数据单元。一个分组可以是一个完整的IP数据报，也可以是IP数据报的一个分片。

==> 用ICMP不可达差错（需要分片）来发现路径ＭＴＵ：

　　在IP首部设置不分片标志比特（DF），当增大分组的数据长度时，一旦中间的某个路由器需要将该数据报进行分组才能继续发送，就会出现这个差错。首先出现差错时的分组长度即是路径MTU。

==> 用traceroute来发现路径ＭＴＵ：

　　修改traceroute程序：发送分组，设置"不分片"标志比特，发送的第一个分组长度正好与出口MTU相等，每次收到ICMP不能分片差错时就减小分组的长度，如果返回的ICMP报文是新格式，则用其中包含的MTU值发送，否则尝试用下一个最小的MTU值发送。

==> UDP对中间连路的MTU来说过长产生的一系列作用：

　　先在源主机尝试进行分片，如果不行，再把DF置0，但是每隔一定时间又会将DF置1看看MTU是否增大了。

==> UDP和ARP之间的相互作用：

　　如果发生分片且ARP缓存被清空，那么第一个分片到达可能会超时造成组装超时从而使UDP被丢弃，因为每个分片发送之前要先发送ARP请求，而目的段处理ARP请求的队列是后进先出的。

==> ICMP源站抑制差错:

    当系统的接收速度比处理速度快时（如某个中间路由器由于下一连路的速度慢而导致缓存被塞满），可能发生此差错。

==> 设计UDP服务器：

　　*　UDP服务器一般是交互式服务器，即某个UDP端口的所有客户请求只由单个服务器进程来进行处理；
　　*　通常一个UDP端口对应一个有限大小的输入队列；
　　*　输入队列是先进先出的；
　　*　来自客户端的UDP数据报：IP首部包含了源和目的段的IP地址，UDP首部包含了源和目的端的端口号；
　　*　可以对UDP服务器的本地IP地址和远程（客户端）IP地址进行限制；
　　*  某些服务器可以使同一IP地址和端口上有多个接收者（程序端点）。


