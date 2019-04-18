# TCP/IP 协议栈及 OSI 参考模型详解 - gauss的专栏 - CSDN博客
2015年09月20日 09:43:30[gauss](https://me.csdn.net/mathlmx)阅读数：569
个人分类：[服务器架构与网络编程](https://blog.csdn.net/mathlmx/article/category/944322)
**OSI参考模型**
OSI RM：开放系统互连参考模型（open systeminterconnection reference model）
OSI参考模型具有以下优点：
- 
- 
简化了相关的网络操作；
- 
提供设备间的兼容性和标准接口；
- 
促进标准化工作；
- 
结构上可以分隔；
- 
易于实现和维护。
20世纪60年代以来，计算机网络得到了飞速增长。各大厂商为了在数据通信网络领域占据主导地    位，纷纷推出了各自的网络架构体系和标准，如IBM公司的SNA，Novell IPX/SPX协议，Apple公司的AppleTalk协议，DEC公司的DECnet，以及广泛流行的TCP/IP协议。同时，各大厂商针对自己的协议生产出了不同的硬件和软件。各个厂商的共同努力促进了网络技术的快速发展和网络设备种类的迅速增长。但由于多种协议的并存，也使网络变得越来越复杂；而且，厂商之间的网络设备大部分不能兼容，很难进行通信。
为了解决网络之间的兼容性问题，帮助各个厂商生产出可兼容的网络设备，国际标准化组织ISO于1984年提出了OSI RM（OpenSystem Interconnection Reference Model，开放系统互连参考模型）。OSI 参考模型很快成为计算机网络通信的基础模型。在设计OSI 参考模型时，遵循了以下原则：各个层之间有清晰的边界，实现特定的功能；层次的划分有利于国际标准协议的制定；层的数目应该足够多，以避免各个层功能重复。
**OSI****分层**
![wKioL1SIBIaiOFczAADimSwcq6s561.jpg](http://s3.51cto.com/wyfs02/M02/54/A0/wKioL1SIBIaiOFczAADimSwcq6s561.jpg)
        通常OSI参考模型第一层到第三层称为底层（lower layer），又叫介质层（media layer），底层负责数据在网络中的传送，网络互连设备往往位于下三层，以硬件和软件的方式来实现。OSI参考模型的第五层到第七层称为高层（upper layer），又叫住几层（host layer），高层用于保障数据的正确传输，以软件方式来实现。
**OSI七层功能：**
![wKiom1SIA_nwv_AgAAHIC52DZA4848.jpg](http://s3.51cto.com/wyfs02/M02/54/A1/wKiom1SIA_nwv_AgAAHIC52DZA4848.jpg)
- 
**TCP/IP协议栈**
![wKioL1SIBJKy7q8oAAEMHfbl-0E582.jpg](http://s3.51cto.com/wyfs02/M00/54/A0/wKioL1SIBJKy7q8oAAEMHfbl-0E582.jpg)
由于OSI模型和协议比较复杂，所以并没有得到广泛的应用。
而TCP/IP(transfer control protocol/internet protocol,传输控制协议/网际协议)模型因其开放性和易用性在实践中得到了广泛的应用，TCP/IP协议栈也成为互联网的主流协议。
![wKiom1SIBAHw1MybAAE_fDStMQg817.jpg](http://s3.51cto.com/wyfs02/M00/54/A1/wKiom1SIBAHw1MybAAE_fDStMQg817.jpg)
TCP/IP模型各个层次分别对应于不同的协议。TCP/IP协议栈是数据通信协议的集合 ，包含许多协议。其协议栈名字来源于其中最主要的两个协议TCP（传输控制协议）和IP（网际协议）。TCP/IP协议栈负责确保网络设备之间能够通信。它是一组规则，规定了信息如何在网络中传输。
**TCP/IP模型的层间通信与数据封装**
![wKiom1SIBN_iV-a8AAHEz78AQQE658.jpg](http://s3.51cto.com/wyfs02/M00/54/A1/wKiom1SIBN_iV-a8AAHEz78AQQE658.jpg)
TCP/IP每一层都让数据得以通过网络进行传输，这些层之间使用**PDU（协议数据单元）**彼此交换信息，确保网络设备之间能够通信。
![wKioL1SIBXngTSn2AAFtzBd_5_4918.jpg](http://s3.51cto.com/wyfs02/M01/54/A0/wKioL1SIBXngTSn2AAFtzBd_5_4918.jpg)
A.传输层数据中加入TCP报头后得到PDU被称为**segment（数据段）**；
B.数据段被传递给网络层，网络层添加IP报头得到的PDU被称为**packet（数据包）**；
C.数据包被传递到数据链路层，封装数据链路层报头得到的PDU被称为**frame（数据帧）**；
D.帧被转换为**比特**，通过网络介质传输。
    这种协议栈向下传递数据，并添加报头和报尾的过程称为封装，数据被封装并通过网络传输后，接收设备将删除添加的信息，并根据报头中的信息决定如何将数据沿协议栈上传给合适的应用程序，这个过程称为解封装。不同设备的对等层之间依靠封装和解封装来实现相互间的通信。
**物理层**
**物理层功能：**
- 
规定介质类型、接口类型、信令类型；
- 
规范在终端系统之间激活、维护和关闭物理链路的电气、机械、流程和功能等方面的要求；
- 
规范电平、数据速率、最大传输距离和物理接头等特征。
![wKiom1SIBOXxKFUYAACaTcRu0rw177.jpg](http://s3.51cto.com/wyfs02/M01/54/A1/wKiom1SIBOXxKFUYAACaTcRu0rw177.jpg)
物理层标准规定了物理介质和用于将设备与物理介质相连的接头。
如上图，局域网常用的物理层标准有IEEE指定的以太网标准802.3、令牌总线标准802.4、令牌环网标准802.5以及美国国家标准组织ANSI的X3T9.5委员会制订的光缆标准FDDI（fiber distributed data interface，光纤分布式数据接口）等。广域网常用的物理层标准有电子工业协会和电信工业协会EIA/TIA制定的公共物理层接口标准EIA/TIA-232（即RS-232）、国际电信联盟ITU制定的串行线路接口标准V.24和V.35、以及有关各种数字接口的物理和电气特性的标准G.703等。
物理层介质和物理层设备：
**物理层介质：**
- 
- 
同轴电缆（coaxical cable）
- 
双绞线（twisted pair）
- 
光纤（fiber）
- 
无线电波（wireless raido）
**同轴电缆：**
![wKiom1SIBdmQ2tgzAADxPQFWWRg801.jpg](http://s3.51cto.com/wyfs02/M02/54/A1/wKiom1SIBdmQ2tgzAADxPQFWWRg801.jpg)
  同轴电缆是一种早期使用的传输介质，同轴电缆的标准分为两种，10BASE2和10BASE5.这两种标准都支持10Mbps的传输速率，最长传输距离分别为185米和500米。10BASE5和10BASE2的同轴电缆使用
的同轴电缆的直径分别为9.5mm和5mm，所以前者又称为粗缆，后者又称为细缆。一般情况下，10BASE2同轴电缆使用BNC接头，10BASE5同轴电缆使用N型接头。目前，10Mbps的传输速率早已不能满足目前企业网络需求，因此同轴电缆在目前企业网络中很少应用。
**双绞线**
![wKiom1SIBOqRwiqQAAD9zkp39Q0530.jpg](http://s3.51cto.com/wyfs02/M02/54/A1/wKiom1SIBOqRwiqQAAD9zkp39Q0530.jpg)
双绞线采用了一对互相绝缘的金属导线互相绞合的方式来抵御一部分外界电磁波干扰。把两根绝缘的铜导线按一定密度互相绞在一起，可以降低信号干扰的程度，每一根导线在传输中辐射的电波会被另一根线上发出的电波抵消，“双绞线”的名字也是由此而来的。
与同轴电缆相比双绞线（twistedpair）具有更低的制造和部署成本，因此在企业网络中被广泛应用。双绞线可分为屏蔽双绞线（shieldedtwisted pair，STP）和非屏蔽双绞线（unshieldedtwisted pair，UTP）。屏蔽双绞线在双绞线与外层绝缘封套之间有一个金属屏蔽层，可以屏蔽电磁干扰。
双绞线有很多种类型，不同类型的 双绞线所支持的传输速率一般也不相同。例如，3类双绞线支持10Mbps传输速率；5类双绞线支持100Mbps传输速率，满足快速以太网标准；超5类双绞线及更高级别的双绞线支持千兆以太网传输。
**双绞线线序：**
568A线序：
1-绿白，2-绿，3-橙白，4-蓝，5-蓝白，6-橙，7-棕白，8-棕
568B线序：
   1-橙白，2-橙，3-绿白，4-蓝，5-蓝白，6绿，7-棕白，8-棕
根据网线两端连接网络设备的不同，网线又分为直通线（平行线）和交叉线两种。
直通线（平行线）就是按照前面介绍的568A标准或568B标准制作（即双绞线两端的线序一样，568A的线序不常用，现主流用的都是568B的线序）
交叉线的一端保持原来的线序，另一端把1和3对调，2和5对调。
**直通线和交叉线的应用：**
1.同种类型设备之间使用交叉线连接，不同类型设备之间使用直通线连接；
2.路由器和PC属于DTE（DataTerminal
 Equipment，数据终端设备）类型设备，交换机和HUB数据DCE（Data Circuit-terminatingEquipment，数据通信设备）类型设备。
**光纤**
![wKioL1SIBtTQDsL7AAHLv3k9JBw286.jpg](http://s3.51cto.com/wyfs02/M02/54/A0/wKioL1SIBtTQDsL7AAHLv3k9JBw286.jpg)
双绞线和同轴电缆传输数据时使用的是电信号，而光纤传输数据时使用的是光信号。光纤支持的传输速率包括10Mbps，100Mbps，1Gbps，10Gbps，甚至更高。根据光纤传输光信号模式的不同，光纤又可分为单模光纤和多模光纤。单模光纤只能传输一种模式的光，不存在模间色散，因此适用于长距离高速传输。多模光纤允许不同模式的光在一根光纤上传输，由于模间色散较大而导致信号脉冲展宽严重，因此多模光纤主要用于局域网中的短距离传输。光纤连接器种类喝多，常用的连接器包括ST,FC,SC,LC连接器。
**串口电缆**
![wKioL1SIBtuh_7oPAAE_HOI2lt4917.jpg](http://s3.51cto.com/wyfs02/M00/54/A0/wKioL1SIBtuh_7oPAAE_HOI2lt4917.jpg)
网络通信中常常会用到各种各样的串口电缆。常用的串口电缆标准为RS-232，同时也是推荐的标准。但是RS-232的传输速率有限，传输距离仅为6米。其他的串口电缆标准可以支持更长的传输距离，例如RS-422和RS-485的传输距离可达1200米。RS-422和RS-485串口电缆通常使用V.35接头，这种接头在上世纪80年代已经淘汰，但是现在仍在帧中继、ATM等传统网络上使用。V.24是RS-232标准的欧洲版。RS-232本身没有定义接头标准，常用的接头类型为DB-9和DB-25。现在，RS-232已逐渐被FireWire、USB等新标准取代，新产品和新设备已普遍使用USB标准。
**冲突域**
![wKiom1SIBkjjixwzAACvWv9IRbc487.jpg](http://s3.51cto.com/wyfs02/M00/54/A2/wKiom1SIBkjjixwzAACvWv9IRbc487.jpg)
如图是一个10BASE5以太网，每个主机都是用同一根同轴电缆来与其它主机进行通信，因此，这里的同轴电缆又被称为共享介质，相应的网络被称为共享介质网络，或简称为共享式网络。共享式网络中，不同的主机同时发送数据时，就会产生信号冲突的问题，解决这一问题的方法一般是采用载波侦听多路访问/冲突检测技术（carrier sense multiple
 access/collisiondetection）。
       CSMA/CD的基本工作过程如下：
1.终端不停地检测共享线路的状态。如果线路空闲，则可以发送数据；如果线路不空闲，则等待一段时间后继续检测（延时时间由退避算法决定）。
2.如果有另一个设备同时发送数据，两个设备发送的数据会产生冲突。
3.终端设备检测到冲突之后，马上停止发送自己的数据，并发送特殊阻塞信息，以强化冲突信号，使线路上其他站点能够尽早检测到冲突。
4.终端设备检测到冲突后，等待一段时间之后再进行数据发送（延时时间由退避算法决定）。
CSMA/CD的工作原理可简单总结为：先听后发，边发边听，冲突停发，随机延迟后重发。
**物理层设备：中继器和集线器**
**数据链路层**
       数据链路层又分为MAC子层和LLC子层
![wKioL1SIBuDToShPAACwexBlz_8850.jpg](http://s3.51cto.com/wyfs02/M01/54/A0/wKioL1SIBuDToShPAACwexBlz_8850.jpg)
**MACSub-layer****：media access controlsub-layer介质访问控制子层**
MAC子层负责指定数据如何通过物理线路进行传输，并向下与物理层通信，它定义了物理编址、网络拓扑、线路规范、错误通知、按序传递和流量控制等功能。
**LLCSub-layer****：logic link control sub-layer逻辑链路控制子层**
    LLC子层负责识别协议类型并对数据进行封装以便通过网络进行传输。LLC子层主要执行数据链路层的大部分功能和网络层的部分功能。如帧的收发功能，在发送时，帧由发送的数据加上地址和CRC校验等构成，接收时将帧拆开，执行地址识别、CRC校验，并具有帧顺序控制、差错控制、流量控制等功能。此外，它还执行数据报、虚电路、多路复用等部分网络层的功能。
**数据链路层协议**
![wKiom1SIBk7Cs5SUAAFC3wxBffU801.jpg](http://s3.51cto.com/wyfs02/M01/54/A2/wKiom1SIBk7Cs5SUAAFC3wxBffU801.jpg)
     数据链路层协议规定了数据链路层帧的封装方式。
         局域网常用的数据链路层协议有IEEE802.2 LLC标准。
         广域网常用的数据链路层协议有：
**HDLC（high-level data link control，高级数据链路控制）**
**              PPP（point-to-point protocol，点到点协议）**
**              FR（frame relay，帧中继）**
**数据链路层-以太网地址（MAC地址）**
![wKiom1SIBlGzRYY_AADHj1rzMTc659.jpg](http://s3.51cto.com/wyfs02/M02/54/A2/wKiom1SIBlGzRYY_AADHj1rzMTc659.jpg)
       网络设备的MAC地址是全球唯一的。MAC地址由48个二进制位组成，通常我们用十六进制数字来表示。其中前6位十六进制数字由IEEE统一分配给设备制造商，后6位十六进制数字由厂商自行分配。
**网络层**
       功能：在不同的网络之间转发数据包
- 
- 
提供逻辑地址，如果数据跨网络传递，则需要使用逻辑地址来寻址。
- 
路由：将数据报文从一个网络转发到另一个网络。
设备：路由器、三层交换机
**网络层协议**
![wKioL1SIBu2A4HS5AAFf0Wv8N9I094.jpg](http://s3.51cto.com/wyfs02/M00/54/A0/wKioL1SIBu2A4HS5AAFf0Wv8N9I094.jpg)
**常用网络层协议有：**
**IP（Internet Protocol）**：IP为网络层最主要的协议，其功能即为网络层的主要功能，一是提供逻辑编址，二是提供路由功能，三是报文的封装和解封装。ICMP、ARP、RARP协议辅助IP工作。
**  ICMP（Internet Control Message Protocol）**是一个管理协议并为IP提供信息服务，ICMP消息承载在IP报文中。
**ARP（Address Resolution Protocol）**实现IP地址到硬件地址的动态映射，即根据已知的IP地址获得相应的硬件地址。
**RARP（Reverse Address Resolution Protocol）**实现硬件地址到IP地址的动态映射，即根据已知的硬件地址获得相应的IP地址。
       网络层地址：网络地址在网络层唯一标识一台网络设备。
       网络地址包含两部分：网络ID+主机ID（下节主要内容）
**传输层**
主要功能：
- 
- 
分段上层数据；
- 
建立端到端连接；
- 
将数据从一端主机传送到另一端主机；
- 
保证数据按序、可靠、正确传输。
**传输层协议：**
![wKiom1SIBljRr48mAAFPpXRKpXU847.jpg](http://s3.51cto.com/wyfs02/M00/54/A2/wKiom1SIBljRr48mAAFPpXRKpXU847.jpg)
传输层协议主要包含传输控制协议TCP（transfer control protocol）和用户数据报文协议UDP（user datagram protocol）
![wKiom1SIBzHSMZ78AAF3CFFhIxI894.jpg](http://s3.51cto.com/wyfs02/M02/54/A2/wKiom1SIBzHSMZ78AAF3CFFhIxI894.jpg)
TCP提供面向连接的、可靠的字节流服务。面向连接意味着使用TCP协议作为传输层协议的两个应用之间在相互交换数据之前必须建立一个TCP连接。TCP通过确认、校验、重组等机制为上层应用提供可靠的传输服务。但是TCP连接的建立以及确认、校验等机制都需要耗费大量的工作并且会带来大量的开销。
UDP提供简单的、面向数据报的服务。UDP不保证可靠性，即不保证报文能够到达目的地。UDP适用于更关注传输效率的应用，如SNMP、Radius等，SNMP监控网络并断续发送告警等消息，如果每次发送少量信息都需要建立TCP连接，无疑会降低传输效率，所以诸如SNMP、Radius等更注重传输效率的应用程序都会选择UDP作为传输层协议。另外，UDP还适用于本身具备可靠性机制的应用层协议。
**应用层功能**
- 
- 
为用户提供接口、处理特定的应用；
- 
数据加密、解密、压缩、解压缩；
- 
定义数据表示的标准。
**应用层协议**
![wKiom1SIBziAnfTLAAIEZXvr3G4915.jpg](http://s3.51cto.com/wyfs02/M00/54/A2/wKiom1SIBziAnfTLAAIEZXvr3G4915.jpg)
应用层有许多协议，以下协议可以帮助您使用和管理 TCP/IP 网络：
**FTP(File TransferProtocol)** 文件传输协议。用于传输独立的文件，通常用于交互式用户会话。
**HTTP(HypertextTransfer Protocol)**超文本传输协议。 用于传输那些构成万维网上的页面的文件。
**TELNET** ：远程终端访问。用于传送具有TELNET控制信息的数据。它提供了与终端设备或终端进程交互的标准方法，支持终端到终端的连接及进程到进程分布式计算的通信。
**SMTP(Simple MessageTransfer Protocol)**简单邮件传输协议 和
**POP3(Post OfficeProtoco**l)邮局协议用于发送和接收邮件。
**DNS(Domain NameServer)**是一个域名服务的协议，提供域名到IP地址的转换，允许对域名资源进行分散管理。
**TFTP(Trivial FileTransfer Protocol)**简单文件传输协议。设计用于一般目的的、高吞吐量的文件传输。
**RIP(RoutingInformation Protocol)**路由器用来在 IP 网络上交换路由信息的协议。
**SNMP(Simple NetworkManagement Protocol)**用于收集网络管理信息，并在网络管理控制台和网络设备（例如路由器、网桥和服务器）之间交换网络管理信息。
**Radius(RemoteAuthentication Dial In User Service)**拨号接入远端认证协议完成接入用户的认证、授权、计费功能的协议。
- 
**TCP/IP协议栈的封装过程**
![wKioL1SIB9bQaJBVAAGxIUxvNyc832.jpg](http://s3.51cto.com/wyfs02/M01/54/A0/wKioL1SIB9bQaJBVAAGxIUxvNyc832.jpg)
以传输层采用TCP或者UPD、网络层采用IP、链路层采用Ethernet为例，可以看到TCP/IP中报文的封装过程如上图所示。用户数据经过应用层协议封装后传递给传输层，传输层封装TCP头部，交给网络层，网络层封装IP头部后，再交给数据链路层，数据链路层封装Ethernet帧头和帧尾，交给物理层，物理层以比特流的形式将数据发送到物理线路上。
**TCP Segment**
![wKiom1SIB0CA7CtDAADq-b6k7KM867.jpg](http://s3.51cto.com/wyfs02/M01/54/A2/wKiom1SIB0CA7CtDAADq-b6k7KM867.jpg)
TCP协议概述：
TCP为应用程序提供一种面向连接的、可靠的服务。
TCP的可靠性：
- 
- 
面向连接的传输
- 
最大报文段长度
- 
传输确认机制
- 
首部和数据的检验和
- 
流量控制
TCP首部格式
![wKiom1SIB0LBnFOsAAEujCB4yys663.jpg](http://s3.51cto.com/wyfs02/M00/54/A2/wKiom1SIB0LBnFOsAAEujCB4yys663.jpg)
TCP使用IP作为网络层协议，TCP数据段被封装在一个IP数据包内。TCP数据段由TCP Head（头部）和TCP Data（数据）组成。
TCP最多有60个字节的首部,如果没有任选字段，正常的长度是20字节。TCP Head如上图标识的一些字段组成，这里列出几个常用的字段。
**16位源端口号**：TCP会为源应用程序分配一个源端口号。
**16位目的端口号**：目的应用程序的端口号。每个TCP段都包含源和目的端的端口号，用于寻找发端和收端应用进程。这两个值加上IP首部中的源端IP地址和目的端IP地址可以唯一确定一个TCP连接。
**32位序列号**：用于标识从TCP发端向TCP收端发送的数据字节流。
**32位确认序列号**：确认序列号包含发送确认的一端所期望收到的下一个序号。确认序列号为上次成功收到的数据序列号加1。
**4位首部长度**：表示首部占32bit字的数目。因为TCP首部的最大长度为60字节。
**16位窗口大小**：表示接收端期望接收的字节，由于该字段为16位，因而窗口大小最大值为65535字节。
**16位检验和**：检验和覆盖了整个TCP报文段，包括TCP首部和TCP数据。该值由发端计算和存储并由接收端进行验证。
**TCP的三次握手（建立连接）和四次挥手（断开连接）**
TCP连接的建立是一个三次握手的过程。如图所示：
![wKioL1SIB9rCarKmAAC_CU7_mao308.jpg](http://s3.51cto.com/wyfs02/M02/54/A0/wKioL1SIB9rCarKmAAC_CU7_mao308.jpg)
1、请求端（通常也称为客户端）发送一个SYN段表示客户期望连接服务器端口，初始序列号为a。
2、服务器发回序列号为b的SYN段作为响应。同时设置确认序号为客户端的序列号加1（a+1）作为对客户端的SYN报文的确认。
3、客户端设置序列号为服务器端的序列号加1（b+1）作为对服务器端SYN报文段的确认。
这三个报文段完成TCP连接的建立。
TCP连接的建立是一个三次握手的过程，而TCP连接的终止则要经过四次握手。
如图所示：
![wKiom1SIB0aiDqNnAADKHCMfIgI643.jpg](http://s3.51cto.com/wyfs02/M02/54/A2/wKiom1SIB0aiDqNnAADKHCMfIgI643.jpg)
1、请求端（通常也称为客户端）想终止连接则发送一个FIN段，序列号设置为a。
2、服务器回应一个确认序号为客户端的序列号加1（a+1）的ACK确认段，作为对客户端的FIN报文的确认。
3、服务器端向客户端发送一个FIN终止段（设置序列号为b，确认号为a+1）。
4、客户端返回一个确认报文（设置序列号为b+1）作为响应。
以上四次交互完成双方向的连接的关闭。
**TCP滑动窗口机制：**
**![wKioL1SIB-byaWruAAImM3ycikA350.jpg](http://s3.51cto.com/wyfs02/M00/54/A0/wKioL1SIB-byaWruAAImM3ycikA350.jpg)**
TCP滑动窗口技术通过动态改变窗口大小来调节两台主机间的数据传输。每个TCP/IP主机支持全双工数据传输，因此TCP有两个滑动窗口：一个用于接收数据，另一个用于发送数据。TCP使用肯定确认技术，其确认号指的是下一个所期待的字节。
如图中所示以数据单方向发送为例，介绍滑动窗口如何实现流量
控制。服务器端向客户端发送4个大小为1024字节的数据段，其中发送端的窗口大小为4096，客户端到以ACK4097响应，窗口大小调整为2048，表明客户端（即接收端）缓冲区只能处理2048个字节的数据段。于是发送端改变其发送速率。发送接收端能够接收的数据段大小2048的数据段。
**UDP协议概述**
- 
UDP为应用程序提供面向无连接的服务。传输数据之前源端和目的端不需要建立连接。
- 
不需要维持连接状态，收发状态等，因此服务器可同时向多个客户端传输相同的消息。
- 
UDP适用于对传输效率要求高的运用。
**UDP首部格式**
![wKiom1SIB8TRO_zhAADNA8HrkkQ733.jpg](http://s3.51cto.com/wyfs02/M01/54/A2/wKiom1SIB8TRO_zhAADNA8HrkkQ733.jpg)
    UDP和TCP一样都使用IP作为网络层协议，TCP数据报被封装在一个IP数据包内。由于UDP不象TCP一样提供可靠的传输，因此UDP的报文格式相对而言较简单。
    整个UDP首部有如下标识：
16位源端口号：为源端应用程序分配的一个源端口号。
16位目的端口号：目的应用程序的端口号
16位UDP长度：是指UDP首部和UDP数据的字节长度。该字段的最小值为8。
16位UDP检验和：该字段提供与TCP检验和同样的功能，只不过在UDP协议中该字段是可选的。
**TCP VS UDP**
![wKioL1SICFyyZNXSAAFY4J3hgbU169.jpg](http://s3.51cto.com/wyfs02/M02/54/A0/wKioL1SICFyyZNXSAAFY4J3hgbU169.jpg)
**IP packet**
![wKiom1SIB8ugRyWwAAGRNKHx1IE758.jpg](http://s3.51cto.com/wyfs02/M02/54/A2/wKiom1SIB8ugRyWwAAGRNKHx1IE758.jpg)
网络层收到传输层的TCP数据段后会再加上网络层IP头部信息。普通的IP头部固定长度为20个字节（不包含IP选项字段）。
IP报文头主要由以下字段组成：报文长度是指头部占32比特字的个数，包括任何选项。由于它是一个4比特字段，24=16，除掉全0项共有15个有效值比特字段，其中最大值也为15，表示头部占15个32比特。因此32*15/8=60字节，头部最长为60字节。
版本号（Version）字段标明了IP协议的版本号，目前的协议版本号为4。下一代IP协议的版本号为6。
8比特的服务类型（TOS，Type of Service）字段包括一个3比特的优先权字段（COS，Class of Service），4比特TOS字段和1比特未用位。4比特TOS分别代表最小时延、最大吞吐量、最高可靠性和最小费用。
总长度（Total length）是整个IP数据报长度，包括数据部分。由于该字段长16比特，所以IP数据报最长可达65535字节。尽管可以传送一个长达65535字节的IP数据报，但是大多数的链路层都会对它进行分片。而且，主机也要求不能接收超过576字节的数据报。UDP限制用户数据报长度为512字节，小于576字节。而事实上现在大多数的实现（特别是那些支持网络文件系统NFS的实现）允许超过8192字节的IP数据报。
标识符（Identification）字段唯一地标识主机发送的每一份数据包。通常每发送一份报文它的值就会加1。
生存时间（TTL，Time to Live）字段设置了数据包可以经过的路由器数目。一旦经过一个路由器，TTL值就会减1，当该字段值为0时，数据包将被丢弃。
协议字段确定在数据包内传送的上层协议，和端口号类似，IP协议用协议号区分上层协议。TCP协议的协议号为6，UDP协议的协议号为17。
报头校验和（Head checksum）字段计算IP头部的校验和，检查报文头部的完整性。
源IP地址和目的IP地址字段标识数据包的源端设备和目的端设备IP地址信息。
**Ethernet frame**
![wKiom1SIB9CgS-M7AAEfkSYePsA236.jpg](http://s3.51cto.com/wyfs02/M00/54/A2/wKiom1SIB9CgS-M7AAEfkSYePsA236.jpg)
以太网头部由三个字段组成：
DMAC：表示目的终端MAC地址。
SMAC：表示源端MAC地址。
LENGTH/TYPE字段：根据值的不同有不同的含义：
当LENGHT/TYPE > 1500时，代表该数据帧的类型（比如
上层协议类型）常见的协议类型有：
0X0800 IP数据包
0X0806 ARP请求/应答报文
0X8035 RARP请求/应答报文。
当LENGTH/TYPE < 1500时，代表该数据帧的长度。
**案例分析**
![wKiom1SIFQ-AU8LgAABPLDaZkWk637.jpg](http://s3.51cto.com/wyfs02/M01/54/A3/wKiom1SIFQ-AU8LgAABPLDaZkWk637.jpg)
如上图所示，通过例举出的TELNET协议的抓包实例，进一步加深对报文封装的理解。
![wKioL1SIFaeiK8bgAAKorRwk0sc836.jpg](http://s3.51cto.com/wyfs02/M02/54/A1/wKioL1SIFaeiK8bgAAKorRwk0sc836.jpg)
上图为AR1使用TELNET协议远程登录AR2进行的TCP三次握手过程。
![wKioL1SIFajAi0-nAAHJcfjBW4s739.jpg](http://s3.51cto.com/wyfs02/M00/54/A1/wKioL1SIFajAi0-nAAHJcfjBW4s739.jpg)
上图为数据链路层封装。如图可知使用的是Ethernet II格式封装。
    DMAC为：00e0:fc3b:6792
    SMAC为：00e0:fc80:64f3
    type:字段为0x0800表明数据字段封装是IP报文。
![wKiom1SIFRTiFYjwAALqjueaz2o705.jpg](http://s3.51cto.com/wyfs02/M00/54/A3/wKiom1SIFRTiFYjwAALqjueaz2o705.jpg)
上图为网络层报文封装。一个网络层IP包是由IP头部和IP数据组成。
    上图表明是一个IPv4的报文
    报文头为20字节
    协议字段为0x06，表明数据封装的是一个TCP报文。
    数据的源IP地址为12.12.12.1，目的IP地址为12.12.12.2
![wKioL1SIFazyFJs8AAJF_RjjIv8392.jpg](http://s3.51cto.com/wyfs02/M01/54/A1/wKioL1SIFazyFJs8AAJF_RjjIv8392.jpg)
上图为传输层数据封装。如图所示的传输层使用的是TCP协议
    源端口号为随机端口号49895，目的端口号为公认TELNET协议端口号23
**附：**
常用默认端口号 网络层---数据包的包格式里面有个很重要的字段叫做协议号。比如在传输层如果是TCP连接那么在网络层IP包里面的协议号就将会有个值是6如果是UDP的话那个值就是17---传输层。
传输层---通过接口关联(端口的字段叫做端口)---应用层。
用netstat –an 可以查看本机开放的端口号。
代理服务器常用以下端口:
HTTP协议代理服务器常用端口号80/8080/3128/8081/9080
SOCKS代理协议服务器常用端口号1080
FTP文件传输协议代理服务器常用端口号21
Telnet远程登录协议代理服务器常用端口23
HTTP服务器默认的端口号为80/tcp木马Executor开放此端口
HTTPSsecurely transferring web pages服务器默认的端口号为443/tcp 443/udp
Telnet不安全的文本传送默认端口号为23/tcp木马Tiny Telnet Server所开放的端口
FTP默认的端口号为21/tcp木马Doly Trojan、Fore、InvisibleFTP、WebEx、WinCrash和Blade Runner所开放的端口
TFTPTrivial File Transfer Protocol 默认的端口号为69/udp
SSH安全登录、SCP文件传输、端口重定向默认的端口号为22/tcp
SMTP Simple Mail Transfer Protocol (E-mail)默认的端口号为25/tcp木马Antigen、EmailPassword Sender、Haebu Coceda、ShtrilitzStealth、WinPC、WinSpy都开放这个端口
POP3 Post Office Protocol (E-mail) 默认的端口号为110/tcp
WebLogic默认的端口号为7001
Webshpere应用程序默认的端口号为9080
webshpere管理工具默认的端口号为9090
JBOSS默认的端口号为8080
TOMCAT默认的端口号为8080
WIN2003远程登陆默认的端口号为3389
Symantec AV/Filter for MSE ,默认端口号为8081 Oracle 数据库默认的端口号为1521
ORACLE EMCTL默认的端口号为1158
Oracle XDB XML 数据库默认的端口号为8080
Oracle XDB FTP服务默认的端口号为2100
MS SQL*SERVER数据库server默认的端口号为1433/tcp 1433/udp
MS SQL*SERVER数据库monitor默认的端口号为1434/tcp 1434/udp
QQ默认的端口号为1080/udp 
