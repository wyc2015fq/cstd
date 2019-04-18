# HTTP与TCP/IP的区别 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月23日 21:14:55[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：49标签：[HTTP																[TCP																[IP](https://so.csdn.net/so/search/s.do?q=IP&t=blog)](https://so.csdn.net/so/search/s.do?q=TCP&t=blog)](https://so.csdn.net/so/search/s.do?q=HTTP&t=blog)


转载:[http://www.cnblogs.com/renyuan/archive/2013/01/19/2867720.html](http://www.cnblogs.com/renyuan/archive/2013/01/19/2867720.html)

TPC/IP协议是传输层协议，主要解决数据如何在网络中传输，而HTTP是应用层协议，主要解决如何包装数据。关于TCP/IP和HTTP协议的关系，网络有一段比较容易理解的介绍：“我们在传输数据时，可以只使用（传输层）TCP/IP协议，但是那样的话，如果没有应用层，便无法识别数据内容，如果想要使传输的数据有意义，则必须使用到应用层协议，应用层协议有很多，比如HTTP、FTP、TELNET等，也可以自己定义应用层协议。WEB使用HTTP协议作应用层协议，以封装HTTP 文本信息，然后使用TCP/IP做传输层协议将它发到网络上。”

　　术语TCP/IP代表传输控制协议/网际协议，指的是一系列协议。“IP”代表网际协议，TCP和UDP使用该协议从一个网络传送数据包到另一个网络。把IP想像成一种高速公路，它允许其它协议在上面行驶并找到到其它电脑的出口。TCP和UDP是高速公路上的“卡车”，它们携带的货物就是像HTTP，文件传输协议FTP这样的协议等。 

    你应该能理解，TCP和UDP是FTP，HTTP和SMTP之类使用的传输层协议。虽然TCP和UDP都是用来传输其他协议的，它们却有一个显著的不同：TCP提供有保证的数据传输，而UDP不提供。这意味着TCP有一个特殊的机制来确保数据安全的不出错的从一个端点传到另一个端点，而UDP不提供任何这样的保证。

    HTTP(超文本传输协议)是利用TCP在两台电脑(通常是Web服务器和客户端)之间传输信息的协议。客户端使用Web浏览器发起HTTP请求给Web服务器，Web服务器发送被请求的信息给客户端。

　　下面的图表试图显示不同的TCP/IP和其他的协议在最初OSI模型中的位置：
|7|应用层|例如[HTTP](http://zh.wikipedia.org/wiki/HTTP)、[SMTP](http://zh.wikipedia.org/wiki/SMTP)、[SNMP](http://zh.wikipedia.org/wiki/SNMP)、[FTP](http://zh.wikipedia.org/wiki/FTP)、[Telnet](http://zh.wikipedia.org/wiki/Telnet)、[SIP](http://zh.wikipedia.org/wiki/SIP)、[SSH](http://zh.wikipedia.org/wiki/SSH)、[NFS](http://zh.wikipedia.org/wiki/NFS)、[RTSP](http://zh.wikipedia.org/wiki/RTSP)、[XMPP](http://zh.wikipedia.org/wiki/XMPP)、[Whois](http://zh.wikipedia.org/wiki/Whois)、[ENRP](http://zh.wikipedia.org/w/index.php?title=ENRP&action=edit&redlink=1)|
|----|----|----|
|6|表示层|例如[XDR](http://zh.wikipedia.org/w/index.php?title=External_Data_Representation&action=edit&redlink=1)、[ASN.1](http://zh.wikipedia.org/w/index.php?title=Abstract_Syntax_Notation_1&action=edit&redlink=1)、[SMB](http://zh.wikipedia.org/w/index.php?title=Server_message_block&action=edit&redlink=1)、[AFP](http://zh.wikipedia.org/w/index.php?title=Apple_Filing_Protocol&action=edit&redlink=1)、[NCP](http://zh.wikipedia.org/w/index.php?title=NetWare_Core_Protocol&action=edit&redlink=1)|
|5|会话层|例如[ASAP](http://zh.wikipedia.org/w/index.php?title=Aggregate_Server_Access_Protocol&action=edit&redlink=1)、[TLS](http://zh.wikipedia.org/wiki/Transport_Layer_Security)、[SSH](http://zh.wikipedia.org/wiki/SSH)、ISO 8327 / CCITT X.225、[RPC](http://zh.wikipedia.org/w/index.php?title=Remote_procedure_call&action=edit&redlink=1)、[NetBIOS](http://zh.wikipedia.org/w/index.php?title=NetBIOS&action=edit&redlink=1)、[ASP](http://zh.wikipedia.org/w/index.php?title=AppleTalk&action=edit&redlink=1)、[Winsock](http://zh.wikipedia.org/w/index.php?title=Winsock&action=edit&redlink=1)、[BSD sockets](http://zh.wikipedia.org/wiki/Berkeley_sockets)|
|4|传输层|例如[TCP](http://zh.wikipedia.org/wiki/TCP)、[UDP](http://zh.wikipedia.org/wiki/User_Datagram_Protocol)、[RTP](http://zh.wikipedia.org/w/index.php?title=Real-time_Transport_Protocol&action=edit&redlink=1)、[SCTP](http://zh.wikipedia.org/w/index.php?title=Stream_Control_Transmission_Protocol&action=edit&redlink=1)、[SPX](http://zh.wikipedia.org/w/index.php?title=Sequenced_packet_exchange&action=edit&redlink=1)、[ATP](http://zh.wikipedia.org/w/index.php?title=AppleTalk&action=edit&redlink=1)、[IL](http://zh.wikipedia.org/w/index.php?title=IL_Protocol&action=edit&redlink=1)|
|3|网络层|例如[IP](http://zh.wikipedia.org/wiki/%E7%BD%91%E9%99%85%E5%8D%8F%E8%AE%AE)、[ICMP](http://zh.wikipedia.org/wiki/ICMP)、[IGMP](http://zh.wikipedia.org/wiki/IGMP)、[IPX](http://zh.wikipedia.org/wiki/IPX)、[BGP](http://zh.wikipedia.org/wiki/BGP)、[OSPF](http://zh.wikipedia.org/wiki/OSPF)、[RIP](http://zh.wikipedia.org/wiki/RIP)、[IGRP](http://zh.wikipedia.org/wiki/IGRP)、[EIGRP](http://zh.wikipedia.org/wiki/EIGRP)、[ARP](http://zh.wikipedia.org/wiki/ARP)、[RARP](http://zh.wikipedia.org/wiki/RARP)、 [X.25](http://zh.wikipedia.org/wiki/X.25)|
|2|数据链路层|例如[以太网](http://zh.wikipedia.org/wiki/%E4%BB%A5%E5%A4%AA%E7%BD%91)、[令牌环](http://zh.wikipedia.org/wiki/%E4%BB%A4%E7%89%8C%E7%8E%AF)、[HDLC](http://zh.wikipedia.org/wiki/HDLC)、[帧中继](http://zh.wikipedia.org/wiki/%E5%B8%A7%E4%B8%AD%E7%BB%A7)、[ISDN](http://zh.wikipedia.org/wiki/ISDN)、[ATM](http://zh.wikipedia.org/wiki/%E5%BC%82%E6%AD%A5%E4%BC%A0%E8%BE%93%E6%A8%A1%E5%BC%8F)、[IEEE 802.11](http://zh.wikipedia.org/wiki/IEEE_802.11)、[FDDI](http://zh.wikipedia.org/wiki/FDDI)、[PPP](http://zh.wikipedia.org/wiki/PPP)|
|1|物理层|例如[线路](http://zh.wikipedia.org/w/index.php?title=%E7%BA%BF%E8%B7%AF&action=edit&redlink=1)、[无线电](http://zh.wikipedia.org/wiki/%E6%97%A0%E7%BA%BF%E7%94%B5)、[光纤](http://zh.wikipedia.org/wiki/%E5%85%89%E7%BA%A4)、[信鸽](http://zh.wikipedia.org/wiki/%E4%BF%A1%E9%B8%BD)|

1、HTTP协议的几个重要概念

 1.连接(Connection)：一个传输层的实际环流，它是建立在两个相互通讯的应用程序之间。

 2.消息(Message)：HTTP通讯的基本单位，包括一个结构化的八元组序列并通过连接传输。

 3.请求(Request)：一个从客户端到服务器的请求信息包括应用于资源的方法、资源的标识符和协议的版本号

 4.响应(Response)：一个从服务器返回的信息包括HTTP协议的版本号、请求的状态(例如“成功”或“没找到”)和文档的MIME类型。

 5.资源(Resource)：由URI标识的网络数据对象或服务。

 6.实体(Entity)：数据资源或来自服务资源的回映的一种特殊表示方法，它可能被包围在一个请求或响应信息中。一个实体包括实体头信息和实体的本身内容。

 7.客户机(Client)：一个为发送请求目的而建立连接的应用程序。

 8.用户[代理](http://search.china.alibaba.com/wiki/k-%B4%FA%C0%ED_n-y.html)(Useragent)：初始化一个请求的客户机。它们是浏览器、编辑器或其它用户工具。

 9.服务器(Server)：一个接受连接并对请求返回信息的应用程序。

 10.源服务器(Originserver)：是一个给定资源可以在其上驻留或被创建的服务器。

 11.代理(Proxy)：一个中间程序，它可以充当一个服务器，也可以充当一个客户机，为其它客户机建立请求。请求是通过可能的翻译在内部或经过传递到其它的服务器中。一个代理在发送请求信息之前，必须解释并且如果可能重写它。

 代理经常作为通过防火墙的客户机端的门户，代理还可以作为一个帮助应用来通过协议处理没有被用户代理完成的请求。

 12.网关(Gateway)：一个作为其它服务器中间媒介的服务器。与代理不同的是，网关接受请求就好象对被请求的资源来说它就是源服务器；发出请求的客户机并没有意识到它在同网关打交道。

 网关经常作为通过防火墙的服务器端的门户，网关还可以作为一个协议翻译器以便存取那些存储在非HTTP系统中的资源。

 13.通道(Tunnel)：是作为两个连接中继的中介程序。一旦激活，通道便被认为不属于HTTP通讯，尽管通道可能是被一个HTTP请求初始化的。当被中继的连接两端关闭时，通道便消失。当一个门户(Portal)必须存在或中介(Intermediary)不能解释中继的通讯时通道被经常使用。

 14.缓存(Cache)：反应信息的局域存储。

 2.发送请求

 打开一个连接后，客户机把请求消息送到服务器的停留端口上，完成提出请求动作。

 HTTP/1.0 请求消息的格式为：

 请求消息=请求行(通用信息|请求头|实体头)CRLF[实体内容]

 请求 行=方法 请求URL HTTP版本号 CRLF

 方 法=GET|HEAD|POST|扩展方法

 U R L=协议名称+宿主名+目录与文件名

 请求行中的方法描述指定资源中应该执行的动作，常用的方法有GET、HEAD和POST。不同的请求对象对应GET的结果是不同的，对应关系如下：

 对象 GET的结果

 文件 文件的内容

 程序 该程序的执行结果

 数据库查询 查询结果

 HEAD??要求服务器查找某对象的元信息，而不是对象本身。

 POST??从客户机向服务器传送数据，在要求服务器和CGI做进一步处理时会用到POST方法。POST主要用于发送HTML文本中FORM的内容，让CGI程序处理。

 一个请求的例子为：

 GEThttp://networking.zju.edu.cn/zju/index.htmHTTP/1.0 networking.zju.edu.cn/zju/index.htmHTTP/1.0 头信息又称为元信息，即信息的信息，利用元信息可以实现有条件的请求或应答。

 请求头??告诉服务器怎样解释本次请求，主要包括用户可以接受的数据类型、压缩方法和语言等。

 实体头??实体信息类型、长度、压缩方法、最后一次修改时间、数据有效期等。

 实体??请求或应答对象本身。

 3.发送响应

 服务器在处理完客户的请求之后，要向客户机发送响应消息。

 HTTP/1.0的响应消息格式如下：

 响应消息=状态行(通用信息头|响应头|实体头) CRLF 〔实体内容〕

 状态行=HTTP版本号 状态码 原因叙述

 状态码表示响应类型

 1×× 保留

 2×× 表示请求成功地接收

 3×× 为完成请求客户需进一步细化请求

 4×× 客户错误

 5×× 服务器错误

 响应头的信息包括：服务程序名，通知客户请求的URL需要[认证](http://search.china.alibaba.com/wiki/k-%C8%CF%D6%A4_n-y.html)，请求的资源何时能使用。

 4.关闭连接

 客户和服务器双方都可以通过关闭套接字来结束TCP/IP对话



计算机网络是什么？

简单地理解，计算机网络的任务就是传输数据。为了完成这一复杂的任务，国际标准化组织ISO提供了OSI参考模型，这种模型把互联网网络氛围7层，分别是物理层、数据链路层、网络层、传输层、会话层、表示层和应用层。每个曾有明确的分工，并且在层与层之间，下层为上层提供服务。这种分层的思想简化了网络系统的设计过程，例如在设计应用层时候只需要考虑创建满足用户实际需求的应用；在设计传输层时，只需要考虑如何在两个主机之间传输数据；在设计网络层时，只需要考虑如何在网络上找到一条发送数据的路径，即路由。

由于OSI参考模型过于庞大和复杂，使它难以投入到实际运用中。与OSI参考模型相似的TCP/IP参考模型洗去了网络分层的思想，但是对网络的层次做了简化，并且在网络各层（除了主机-网络层外）都提供了完善的协议，这些协议构成了TCP/IP协议集，简称TCP/IP协议。TCP/IP参考模型氛围4个层：应用层、传输层、网络互联层和主机-网络层。在每一层都有相应的协议，IP协议和TCP协议是协议集中最核心的两个协议。

IP协议位于网络互联曾，用IP地址来标识网络上的各个主机，IP协议把数据氛围若干数据包，然后为这些数据包确定合适的路由。路由就是把数据包从源主机发送到目标主机的路径。

TCP协议位于传输层，保证两个进程之间可靠地传输数据。每当两个进程之间进行通信，就会建立一个TCP连接，TCP协议用端口来标识TCP连接的两个端点。在传输层还有一个UDP协议，它与TCP协议的区别是，UDP不保证可靠地传输数据。

建立在TCP/IP协议基础上的网络程序一般都采用客户端/服务器通信模拟股市。服务器提供服务，客户程序获得服务。服务器程序一般昼夜运行，时刻等待客户的请求并及时作出响应。

Java网络程序致力于实现应用层，传输层向应用层提供了套接字Socket接口，Socket封装了下层的数据传输细节，应用层的程序通过Socket来建立与远程主机的连接，以及进行数据传输。在Java中，有三种套接字类：java.net.Socket、java.net.ServerSocket和DatagramSocket。其中Socket和ServerSocket类建立在TCP协议基础上；DatagramSocket类建立在UDP协议基础上。

socket连接和http连接的区别

简单说，你浏览的网页（网址以http://开头)都是http协议传输到你的浏览器的, 而http是基于socket之上的。socket是一套完成tcp，udp协议的接口。

HTTP协议：简单对象访问协议，对应于应用层  ，HTTP协议是基于TCP连接的

tcp协议：    对应于传输层

ip协议：     对应于网络层 

TCP/IP是传输层协议，主要解决数据如何在网络中传输；而HTTP是应用层协议，主要解决如何包装数据。

Socket是对TCP/IP协议的封装，Socket本身并不是协议，而是一个调用接口（API），通过Socket，我们才能使用TCP/IP协议。

http连接：http连接就是所谓的短连接，即客户端向服务器端发送一次请求，服务器端响应后连接即会断掉；

socket连接：socket连接就是所谓的长连接，理论上客户端和服务器端一旦建立起连接将不会主动断掉；但是由于各种环境因素可能会是连接断开，比如说：服务器端或客户端主机down了，网络故障，或者两者之间长时间没有数据传输，网络防火墙可能会断开该连接以释放网络资源。所以当一个socket连接中没有数据的传输，那么为了维持连接需要发送心跳消息~~具体心跳消息格式是开发者自己定义的

我们已经知道网络中的进程是通过socket来通信的，那什么是socket呢？socket起源于Unix，而Unix/Linux基本哲学之一就是“一切皆文件”，都可以用“打开open –> 读写write/read –> 关闭close”模式来操作。我的理解就是Socket就是该模式的一个实现，socket即是一种特殊的文件，一些socket函数就是对其进行的操作（读/写IO、打开、关闭），这些函数我们在后面进行介绍。

[**Socket连接与HTTP连接**](http://www.cnblogs.com/devinzhang/archive/2012/01/13/2321826.html)

我们在传输数据时，可以只使用（传输层）TCP/IP协议，但是那样的话，如果没有应用层，便无法识别数据内容，如果想要使传输的数据有意义，则必须使用到应用层协议，应用层协议有很多，比如HTTP、FTP、TELNET等，也可以自己定义应用层协议。WEB使用HTTP协议作应用层协议，以封装HTTP文本信息，然后使用TCP/IP做传输层协议将它发到网络上。

1)Socket是一个针对TCP和UDP编程的接口，你可以借助它建立TCP连接等等。而TCP和UDP协议属于传输层 。

 而http是个应用层的协议，它实际上也建立在TCP协议之上。 

 (HTTP是轿车，提供了封装或者显示数据的具体形式；Socket是发动机，提供了网络通信的能力。)

 2）Socket是对TCP/IP协议的封装，Socket本身并不是协议，而是一个调用接口（API），通过Socket，我们才能使用TCP/IP协议。Socket的出现只是使得程序员更方便地使用TCP/IP协议栈而已，是对TCP/IP协议的抽象，从而形成了我们知道的一些最基本的函数接口。

下面是一些的重要的概念，特在此做摘抄和总结。

**一。什么是TCP连接的三次握手**

第一次握手：客户端发送syn包(syn=j)到服务器，并进入SYN_SEND状态，等待服务器确认；

第二次握手：服务器收到syn包，必须确认客户的SYN（ack=j+1），同时自己也发送一个SYN包（syn=k），即SYN+ACK包，此时服务器进入SYN_RECV状态；

第三次握手：客户端收到服务器的SYN＋ACK包，向服务器发送确认包ACK(ack=k+1)，此包发送完毕，客户端和服务器进入ESTABLISHED状态，完成三次握手。

握手过程中传送的包里不包含数据，三次握手完毕后，客户端与服务器才正式开始传送数据。理想状态下，TCP连接一旦建立，在通信双方中的任何一方主动关闭连接之前，TCP 连接都将被一直保持下去。断开连接时服务器和客户端均可以主动发起断开TCP连接的请求，断开过程需要经过“四次握手”（过程就不细写了，就是服务器和客户端交互，最终确定断开）

**二。利用Socket建立网络连接的步骤**

建立Socket连接至少需要一对套接字，其中一个运行于客户端，称为ClientSocket ，另一个运行于服务器端，称为ServerSocket 。

套接字之间的连接过程分为三个步骤：服务器监听，客户端请求，连接确认。

1。服务器监听：服务器端套接字并不定位具体的客户端套接字，而是处于等待连接的状态，实时监控网络状态，等待客户端的连接请求。

2。客户端请求：指客户端的套接字提出连接请求，要连接的目标是服务器端的套接字。为此，客户端的套接字必须首先描述它要连接的服务器的套接字，指出服务器端套接字的地址和端口号，然后就向服务器端套接字提出连接请求。

3。连接确认：当服务器端套接字监听到或者说接收到客户端套接字的连接请求时，就响应客户端套接字的请求，建立一个新的线程，把服务器端套接字的描述发给客户端，一旦客户端确认了此描述，双方就正式建立连接。而服务器端套接字继续处于监听状态，继续接收其他客户端套接字的连接请求。

**三。HTTP链接的特点**

HTTP协议即超文本传送协议(Hypertext Transfer Protocol )，是Web联网的基础，也是手机联网常用的协议之一，HTTP协议是建立在TCP协议之上的一种应用。

HTTP连接最显著的特点是客户端发送的每次请求都需要服务器回送响应，在请求结束后，会主动释放连接。从建立连接到关闭连接的过程称为“一次连接”。

**四。TCP和UDP的区别**

1。TCP是面向链接的，虽然说网络的不安全不稳定特性决定了多少次握手都不能保证连接的可靠性，但TCP的三次握手在最低限度上（实际上也很大程度上保证了）保证了连接的可靠性；而UDP不是面向连接的，UDP传送数据前并不与对方建立连接，对接收到的数据也不发送确认信号，发送端不知道数据是否会正确接收，当然也不用重发，所以说UDP是无连接的、不可靠的一种数据传输协议。

2。也正由于1所说的特点，使得UDP的开销更小数据传输速率更高，因为不必进行收发数据的确认，所以UDP的实时性更好。

