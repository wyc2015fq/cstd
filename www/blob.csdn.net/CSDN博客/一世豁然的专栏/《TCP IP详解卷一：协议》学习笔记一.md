# 《TCP/IP详解卷一：协议》学习笔记一 - 一世豁然的专栏 - CSDN博客





2017年03月17日 20:14:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：8207标签：[TCPIP](https://so.csdn.net/so/search/s.do?q=TCPIP&t=blog)
个人分类：[协议](https://blog.csdn.net/Explorer_day/article/category/5567509)









一、分层

1、网络协议通常分不同层次进行开发，每一层分别负责不同的通信功能。




2、TCO/IP通常被认为是一个四层协议系统：

![](https://img-blog.csdn.net/20170315213510057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、链路层，有时候也被称作数据链路层或网络接口层，通常包括操作系统中的设备驱动程序和计算机中对应的网络接口卡。它们一起处理与电缆（或其他任何传输媒介）的物理接口细节。

 2）、网络层，有时也称作互联网层，处理分组在网络中的活动。在TCP/IP协议族中，网络层协议包括IP协议（网际协议），ICMP协议（internet互联网控制报文协议），以及IGMP协议（internet组管理协议）。

 3）、运输层主要为两台主机上的应用程序提供端到端的通信。在TCP/IP协议族中，有两个互不相同的传输协议：TCP（传输控制协议）和UDP（用户数据报协议）。

 4）、应用层负责处理特定的应用程序细节。




3、在TCP/IP协议族中，网络层IP提供的是一个不可靠的服务，它只是尽可能快地把分组从源节点送到目的节点，不提供任何可靠性的保证。另一方面，TCP在不可靠的IP层上提供一个可靠的运输层。




二、TCP/IP的分层

1、TCP/IP协议族中不同层次的协议

![](https://img-blog.csdn.net/20170316193108571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、TCP使用不可靠的IP服务，但是它提供一种可靠的运输层服务。




3、UDP为应用程序接收和发送数据报。但是EDP是不可靠的，它不保证数据报能安全无误的到达目的地。




4、IP是网际层的主要协议，同时被TCP和UDP使用。




5、ICMP是IP的附属协议。IP层用它来与其他主机或路由器交换错误报文和其他重要的信息。




6、IGMP是Internet组管理协议。它用来把一个UDP数据报多播到多个主机。




7、ARP（地址解析协议）和RARP（逆地址解析协议）是某些网络接口使用的特殊协议，用来转换IP层和网络接口层使用的地址。







三、互联网的地址

1、互联网上的每个接口必须有一个唯一的Internet地址（也称作IP地址）。IP地址长32bit。Internet地址并不采用平面形式的地址空间。IP地址具有一定的接口，五类不同的互联网地址格式如下：

![](https://img-blog.csdn.net/20170316194613794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、32位IP地址“点分十进制表示法”

![](https://img-blog.csdn.net/20170316194825017?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3、有三类IP地址

 1）、单播地址（目标为单个主机）

 2）、广播地址（目的端为给定网络上的左右主机）

 3）、多播地址（目的端为同一组类的所有主机）




四、域名系统

1、域名系统是一个分布的数据库，由它来提供IP地址和主机名之间的映射信息。







五、封装

1、数据进入协议栈的封装过程：

![](https://img-blog.csdn.net/20170316195918735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


 1）、帧头和帧尾所标注的数字是典型以太网首部的长度。

 2）、以太网数据帧的物理特性是其长度必须在46~1500字节之间。

 3）、图中IP和网络接口层传送的数据单元应该是分组。分组既可以是一个IP数据报，也可以是IP数据报的一个片。

 4）、UDP数据和TCP数据基本一致。唯一不同的是UDP传送给IP的信息单元称作UDP数据报，而UDP首部的长度为8位。

 5）、由于TCP、UDP、ICMP、IGMP都要向IP传送数据，因此IP必须在生成的IP首部加入某种标识，以表明数据属于那一层。IP在首部存入一个长度为8位的数值，称作协议域。1表示IGMP协议，2表示ICMP协议，6表示TCP协议，17表示UDP协议。

 6）、TCP、UDP、网络接口也要在首部加入标识符。




2、当应用程序用TCP传送数据时，数据被送入协议栈中，然后逐个通过每一层直到被当作一串比特流送入网络。其中每一层对收到的数据都要添加一些首部信息（有时还要添加尾部信息）。




3、TCP传给IP的数据单元称作TCP报文段或简为TCP段，IP传给网络接口层的数据单元称作IP数据报。通过以太网传输的比特流称作数据帧。







六、分用

1、当目的主机收到一个以太网数据帧时，数据就开始从协议栈中由低往上升，同时去掉各层协议加上的报文首部。每层协议盒都要去检查报文首部中的协议标识，以确定接受数据的上层协议。这个过程称作分用。




2、以太网数据帧的分用过程

![](https://img-blog.csdn.net/20170317195200126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





七、客户-服务器模型

1、服务器提供的服务分两种类型：

 1）、重复型

 2）、并发型




2、重复型服务器通过以下步骤进行交互：

 I、等待一个客户请求的到来

 II、处理客户请求

 III、发送响应给发送请求的客户

 IV、返回第I步




3、并发型服务器采用以下步骤：

 I、等待一个客户请求的到来

 II、启动一个新的服务器来处理这个请求

 III、返回第I步




4、一般来说，TCP服务器是并发的，而UDP服务器是重复的，但也存在一些例外。







八、端口号

1、服务器一般都是通过知名端口号来识别的。客户使用临时设定的端口号。




2、大多数TCP/IP实现给临时端口分配1024~5000之间的端口号。大于5000的端口号是为其他服务器预留的。




3、Uinx系统有保留端口号的概念。只有超级用户特权的进程才允许给它自己分配一个保留端口号。



