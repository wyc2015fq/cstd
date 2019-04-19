# 基于ICE的VoIP穿越NAT改进方案 - 三少GG - CSDN博客
2012年11月26日 21:20:03[三少GG](https://me.csdn.net/scut1135)阅读数：2692
### [stun turn ice等穿越NAT方法](http://blog.csdn.net/lionzl/article/details/7215978)
**基于ICE的VoIP穿越NAT改进方案**
1 引言
    近年来，随着数据网络通信逐渐融入传统的话音业务领域，VoIP技术越来越成为当前商业考虑的对象，并正在向一种正式的商业电话模式演进，而会话初始协议(SIP，Session Initiation Protoc01)就是用来确保这种演进能够实现而需要的NGN(下一代网络)系列协议中重要的一员。SIP是一个用于建立，更改和终止多媒体会话的应用层控制I办议。SIP因其简睢、灵活、可扩展性强的特点，已经成为实现VolP系统的热点技术。
    随着计算机网络技术的不断发展，互联网规模飞速膨胀，大量企业和驻地网采用了私有网络通过NAT/防火墙出口来接入公共网络。而由于SIP包头中含有很多对于路由、接续SIP信令和建立呼叫连接必不可少的地址信息，这样引发了业界对于SIP2穿越NAT/防火墙问题的研究。
    目前，IETF已经对该问题提出了多种解决方案。例如：ALCes(Application Layer Gateways)、MiddleboxControl Protocol、STUN Simple Traversal of UDPthrough NAT)、TURN(Traversal Using Relay NAT)、RSIP(Realm Specific IP)、Symmetric RTP等。然而，当这些技术应用于不同的网络拓扑时都有着显著的利弊，以至于只能根据不同的接入方式来应用不同的方案，所以，未能很好地解决A11-NATⅢ的问题，同时还会给系统引入许多复杂性和脆弱性因素。此外，由于NAT/防火墙已经大量应用，SIP设备也已经比较成熟，对它们进行升级来支持多媒体通信穿越NAT/防火墙的代价将相当的大。因此，一种不需要升级任何现有网络设备，能够穿越各种NAT/防火墙并且方便在现有网络中实施的解决方案成为迫切的需要。
    本文试图寻找一种能够穿越各种类型的NAT/防火墙，无需对现有NAT/防火墙没备做任何改动的解决方案——ICE解决方案，这种方式比以前的解决方案更加灵活，具有广阔的应用前景。
**2 现有NAT解决方案的比较分析**
    主流的NAT穿越解决方案包括STUN、TURN、Proxy及隧道穿越等，这几种方式各具优缺点，比较如下：
    (1)STUNml(simple traversal of UDP over NAT)的原理是通过某种机制预先得到内部私有IP地址对应在出口NAT上的对外公网IP地址，然后在报文负载中所描述的地址信息就直接填写出口NAT上的对外IP地址。其最大的优点是无需对现有NAT/防火墙设备做任何改动。局限性在于需要应用程序支持STUN CLIENT的功能，同时STUN并不适合支持TCP连接的穿越。
    (2)TURN即通过Relay方式穿越NAT，也是私网中的SIP终端通过某种机制预先得剑TURN SeI-ver上的公网地址，私网终端发出的报文都要经过TURN Serve：进行Relay转发。这种方式除了具有STUN方式的优点外，还解决了STUN应用无法穿透对称NAT(SymmetricNAT)以及类似的Firewall设备的缺陷，局限性在于需要SIP终端支持TURN Client，并增大了包的延迟和丢包的可能性。
    (3)Proxy方式是指通过对私网内用户呼叫的信令和媒体||d时做Relay来实现对NAT/防火墙的穿越。由于不用对运营商和客户端的现有网络设备进行任何改造，具有很强的适应性，组网灵活，可满是NGN初期多样化的组网和用户接入。
    (4)隧道穿越技术的基本思想是通过把需要穿越的数据流封装征某种隧道中，从而绕过NAT/防火墙。它在很大程度上解决了对于不问应用协议需要开发不同穿越策略的办法，但是必须多媒体终端和服务器能够支持隧道，这是一个比较大的限制条件。
**3 穿越NAT/防火墙方案的实现**
**3．1 ICE方式**
    交互式连通建立方式ICE(Interactive ConnectivityEstablishment)并非一种新的协议，它不需要对STUN，TURN或RSIP进行扩展就可适用于各种NAT。ICE是通过综合运用上面某几种协议，使之征最适合的情况下工作，以弥补单独使用其中任何一种所带来的固有缺陷。对于SIP来说，ICE只需要定义一些SDP(Sessionescription Protoc01)附加属性即可，对于别的多媒体信令协议也需要制定一些相应的机制来实现。本文是针对SIP呼叫流程实现ICE的功能。
    这种方式的优点是可以根据通讯双方所处的网络环境，选取适合穿越NAT/防火墙的方式。首先，获取用户所征网络中NAT的类型，如果用户没有设置使用何种方式连接，那么默队首先使用UDP连接，如果一定时间内没有连接成功，接着使用TCP连接，同样如果没有在一定时间内连接成功，那么将采用其他方式如Upnp、Httptunnel。如果所有穿越方案都失败后，将结果返回给用户，由用户决定是否重试。
**3．2 ICE算法流程**
    ICE算法流程分为以F几个过程：
    (1)收集本地传输地址
    会话者从服务器上获得主机上一个物理(或虚拟)接口绑定一个端口的本地传输地址。
    (2)启动STUN
    与传统的STUN不同，ICE用户名和密码可以通过信令协议进行交换。
    (3)确定传输地址的优先级
    优先级反映了UA在该地址上接收媒体流的优先级别，取值范围0到1之间，按照被传输媒体流量来确定。
    (4)构建初始化信息(Initiate Message)
    初始化消息由一系列媒体流组成，每个媒体流的任意Peer之间实现最人连通可能性的传输地址是由公网L转发服务器(如TURN)提供的地址。
    (5)响应处理
    连通性检查和执行ICE算法中描述的地址收集过程。
    (6)生成接受信息(Accept Message)
    若接受则发送Accept消息，其构造过程与InitiateMessage类似。
    (7)接受信息处理
    接受过程需要发起者使用Send命令，由服务器转发至响应者。
    (8)附加ICE过程
    Initiate或Accept消息交换过程结束后，双方可能仍将继续收集传输地址。
**3．3 ICE算法实现**
    当将ICE算法集成到SIP呼叫过程的时候，流程应该是：(1)SIP终端注册，并且访问STUN(STUNT)服务器，判断NAT/防火墙类型，以及TCP时三种序列的包的过滤情况。(2)当发起呼叫信息(INVITE)或接收到呼叫信息回应(200 OK)之前根据NAT/防火墙类型进行对RTP进行地址收集(任非对称性NAT/防火墙后需要收集NAT映射地址，在对称性NAT/防火墙后还需要收集TURN地址)。(3)在RTP的地址端口启动接收线程RSTUN服务程序。(4)发送SIP消息，收集的地址放列SDP消息中的alt属性中。(5)SIP终端得到通讯双方地址后进行地址配对(将双方地址进行组合)，并且根据双方网络情况去掉无效的地址对。(6)根据地址对发
    送STUN check的包，其中STUN消息的用户名，密码从alt属性中得到，标识该地址对。(7)当检测到有效的地址对时(可以发送RTP媒体流的地址)，停止接收线程STUN服务)，开始传输RTP流。
    本文实现采用Winpcap API首先捕获TCP连接的SYN--out包，修改lP包头的TTL的值，用pcap—sendpacket()。然后使该socket调用listen函数。实现过程中对应于ICE收集地址的算法描述为：
![](http://www.e-works.net.cn/fileupload/images/128855757641875000_new.jpg)
    类中m_nCandidateID对应地址序号，m_nPriority表示地址优先级，m_CandidateAddr表示地址(IP地址，端口)。实现ICE算法的实体算法描述为：
![](http://www.e-works.net.cn/fileupload/images/128855758172500000_new.jpg)
![](http://www.e-works.net.cn/fileupload/images/128855758378906250_new.jpg)
    实现ICE中会话发起者和接收者的步骤基本一样，仅任处理流程中先后次序稍微有些不同，本文中实现的会话流程如图l所示。
![](http://www.e-works.net.cn/fileupload/images/128855759359062500_new.jpg)
    图1会话流程
**4 测试**
    以安装了SIP软终端的双方都在Full ConeNATNAT/防火墙后为例，进行实例测试。测试过程：
    (1)将两台PC的IP的配置分别为公网59．64．148．187122和私网10．0．0．5/8l
    (2)从公网中的用户代理向私网内的用户代理呼叫，结果能够建立会话，无明显的延时，话音质量良好；
    (3)从私网内的用户代理向公网中的用户代理呼叫，结果能够建立会话，且话音质量良好；
    通过抓包分析可以确定，使用该算法可以成功地穿越NAT/防火墙设备。
**5 结论**
    ICE方式的优势是显而易见的，它消除了现有的机制的许多脆弱性。例如，传统的STUN有几个脆弱点，其中一个就是发现过程需要客户端自己去判断所在NAT类型，这实际上不是一个可取的做法。而应用ICE之后，这个发现过程己经不需要了。另一点脆弱性在于STUN，TURN等机制都完全依赖于一个附加的服务器，而ICE利用服务器分配单边地址的同时，还允许客户端直接相连，因此即使STUN或TRUN服务器中有任何一个失败了，ICE方式仍可让呼叫过程继续下去。此外，传统的STUN最大的缺陷在于，它不能保证在所有网络拓扑结构中都正常工作，对于TURN或类似转发方式工作的协议来说，由于服务器的负担过重，很容易出现丢包或者延迟情况。而ICE方式正好提供了一种负载均衡的解决方案，它将转发服务作为优先级最低的服务，从而在最大程度上保证了服务的可靠性和灵活性。此外，ICE的优势还在于对IPv6的支持。由于广泛的适应能力以及对未来网络的支持，ICE作为一种综合的解决方案将有着非常广阔的应用前景。
**STUN**（**Simple Traversal of User Datagram Protocol through Network Address Translators (NATs)**，NAT的UDP简单穿越）是一种[网络协议](http://blog.csdn.net/wiki/%E7%BD%91%E7%BB%9C%E5%8D%8F%E8%AE%AE)，它允许位于[NAT](http://blog.csdn.net/wiki/NAT)（或多重NAT）后的客户端找出自己的公网地址，查出自己位于哪种类型的NAT之后以及NAT为某一个本地端口所绑定的Internet端端口。这些信息被用来在两个同时处于NAT
 路由器之后的主机之间建立UDP通信。该协议由[RFC 3489](http://blog.csdn.net/lionzl/article/details/7215978)定义。
## [[编辑](http://blog.csdn.net/w/index.php?title=STUN&action=edit&section=1)]方案
一旦客户端得知了Internet端的UDP端口，通信就可以开始了。如果NAT是完全圆锥型的，那么双方中的任何一方都可以发起通信。如果NAT是受限圆锥型或端口受限圆锥型，双方必须一起开始传输。
需要注意的是，要使用STUN RFC中描述的技术并不一定需要使用STUN协议——还可以另外设计一个协议并把相同的功能集成到运行该协议的服务器上。
[SIP](http://blog.csdn.net/wiki/SIP)之类的协议是使用UDP分组在Internet上传输音频和／或视频数据的。不幸的是，由于通信的两个末端往往位于NAT之后，因此用传统的方法是无法建立连接的。这也就是STUN发挥作用的地方。
STUN是一个[客户机－服务器](http://blog.csdn.net/w/index.php?title=%E5%AE%A2%E6%88%B7%E6%9C%BA%EF%BC%8D%E6%9C%8D%E5%8A%A1%E5%99%A8&action=edit&redlink=1)协议。一个VoIP电话或软件包可能会包括一个STUN客户端。这个客户端会向STUN服务器发送请求，之后，服务器就会向STUN客户端报告NAT路由器的公网IP地址以及NAT为允许传入流量传回内网而开通的端口。
以上的响应同时还使得STUN客户端能够确定正在使用的[NAT](http://blog.csdn.net/wiki/NAT)类型——因为不同的NAT类型处理传入的UDP分组的方式是不同的。四种主要类型中有三种是可以使用的：[完全圆锥型NAT](http://blog.csdn.net/w/index.php?title=%E5%AE%8C%E5%85%A8%E5%9C%86%E9%94%A5%E5%9E%8BNAT&action=edit&redlink=1)、[受限圆锥型NAT](http://blog.csdn.net/w/index.php?title=%E5%8F%97%E9%99%90%E5%9C%86%E9%94%A5%E5%9E%8BNAT&action=edit&redlink=1)和[端口受限圆锥型NAT](http://blog.csdn.net/w/index.php?title=%E7%AB%AF%E5%8F%A3%E5%8F%97%E9%99%90%E5%9C%86%E9%94%A5%E5%9E%8BNAT&action=edit&redlink=1)——但大型公司网络中经常采用的对称型NAT（又称为双向NAT）则不能使用。
## [[编辑](http://blog.csdn.net/w/index.php?title=STUN&action=edit&section=2)]算法
STUN 使用下列的算法（取自 [RFC 3489](http://blog.csdn.net/lionzl/article/details/7215978)）来发现 NAT gateways 以及防火墙（firewalls）:
![](http://hi.csdn.net/attachment/201201/27/0_1327648623eoaC.gif)
一旦路经通过红色箱子的终点时，UDP的沟通是没有可能性的。一旦通过黄色或是绿色的箱子，就有连线的可能。
# Traversal Using Relays around NAT
From Wikipedia, the free encyclopedia
Jump to: [navigation](http://blog.csdn.net/lionzl/article/details/7215978#mw-head), [search](http://blog.csdn.net/lionzl/article/details/7215978#p-search)
"TURN" redirects here. For other uses, see [Turn (disambiguation)](http://blog.csdn.net/wiki/Turn_%28disambiguation%29).
**Traversal Using Relays around NAT** (**TURN**) is a[protocol](http://blog.csdn.net/wiki/Communications_protocol) that allows for an element behind a[Network
 address translator](http://blog.csdn.net/wiki/Network_address_translator) (NAT) or [firewall](http://blog.csdn.net/wiki/Firewall_%28networking%29) to receive incoming data over [TCP](http://blog.csdn.net/wiki/Transmission_control_protocol) or [UDP](http://blog.csdn.net/wiki/User_Datagram_Protocol) connections. It is most useful for elements behind symmetric NATs or firewalls that wish to be on the receiving end of a connection to a single peer. TURN does not allow for users to run[servers](http://blog.csdn.net/wiki/Server_%28computing%29)
 on well known ports if they are behind a NAT; it supports the connection of a user behind a NAT to only a single[peer](http://blog.csdn.net/wiki/Peer-to-peer). In that regard, its role is to provide the same security functions
 provided by symmetric NATs and firewalls, but to*turn* the tables so that the element on the inside can be on the receiving end, rather than the sending end, of a connection that is requested by the client.
TURN is specified by [RFC 5766](http://blog.csdn.net/lionzl/article/details/7215978). An update to TURN for[IPv6](http://blog.csdn.net/wiki/IPv6) is specified in[RFC 6156](http://blog.csdn.net/lionzl/article/details/7215978).
## Contents
[[hide](http://blog.csdn.net/lionzl/article/details/7215978#)]
- [1Introduction](http://blog.csdn.net/lionzl/article/details/7215978#Introduction)
- [2See also](http://blog.csdn.net/lionzl/article/details/7215978#See_also)
- [3External links](http://blog.csdn.net/lionzl/article/details/7215978#External_links)- [3.1Implementations](http://blog.csdn.net/lionzl/article/details/7215978#Implementations)
## [[edit](http://blog.csdn.net/w/index.php?title=Traversal_Using_Relays_around_NAT&action=edit&section=1)]Introduction
NATs, while providing many benefits, also come with many drawbacks. The most troublesome of those drawbacks is the fact that they break many existing IP applications, and make it difficult to deploy new ones. Guidelines have been developed that describe
 how to build "NAT friendly" protocols, but many protocols simply cannot be constructed according to those guidelines. Examples of such protocols include multimedia applications and file sharing.
[Session Traversal Utilities for NAT](http://blog.csdn.net/wiki/Session_Traversal_Utilities_for_NAT) (STUN) provides one means for an application to traverse a NAT. STUN allows a
 client to obtain a transport address (an IP address and port) which may be useful for receiving packets from a peer. However, addresses obtained by STUN may not be usable by all peers. Those addresses work depending on the topological conditions of the network.
 Therefore, STUN by itself cannot provide a complete solution for NAT traversal.
A complete solution requires a means by which a client can obtain a transport address from which it can receive media from any peer which can send packets to the public Internet. This can only be accomplished by relaying data through a server that resides
 on the public Internet. This specification describes Traversal Using Relay NAT (TURN), a protocol that allows a client to obtain IP addresses and ports from such a relay.
Although TURN will almost always provide connectivity to a client, it comes at high cost to the provider of the TURN server. It is therefore desirable to use TURN as a last resort only, preferring other mechanisms (such as STUN or direct connectivity) when
 possible. To accomplish that, the [Interactive Connectivity Establishment](http://blog.csdn.net/wiki/Interactive_Connectivity_Establishment) (ICE) methodology can be used to discover the optimal means of connectivity.
　一、NAT/ALG 方式
　　普通NAT是通过修改UDP或TCP报文头部地址信息实现地址的转换，但对于VOIP应用，在TCP/UDP净载中也需带地址信息，ALG方式是指在私网中的VOIP终端在净载中填写的是其私网地址，此地址信息在通过NAT时被修改为NAT上对外的地址。
　　此时当然要求ALG功能驻留在NAT/Firewall设备中，要求这些设备本身具备应用识别的智能。支持IP 语音和视频协议(H323、SIP、 MGCP/H248)的识别和对NAT/Firewall的控制，同时每增加一种新的应用都将需要对 NAT/Firewall进行升级。
　　在安全要求上还需要作一些折衷，因为ALG 不能识别加密后的报文内容，所以必须保证报文采用明文传送，这使得报文在公网中传送时有很大的安全隐患。
　　NAT/ALG是支持VOIP NAT穿透的一种最简单的方式，但由于网络实际情况是已部署了大量的不支持此种特性的NAT/FW设备，因此，实际应用中，很难采用这种方式。
　　二、MIDCOM 方式
　　与NAT/ALG不同的是，MIDCOM的基本框架是采用可信的第三方(MIDCOM Agent)对Middlebox (NAT/FW)进行控制，VOIP协议的识别不由Middlebox完成，而是由外部的MIDCOM Agent完成，因此VOIP使用的协议对Middlebox是透明的 .
　　由于识别应用协议的功能从Middlebox移到外部的MIDCOM Agent上，根据MIDCOM 的构，在不需要更改Middlebox基本特性的基础上，通过对MIDCOM Agent的升级就可以支持更多的新业务，这是相对NAT/ALG方式的一个很大的优势。
　　在VOIP实际应用中，Middlebox功能可驻留在NAT/Firewall，通过软交换设备(即MIDCOM Agent)对IP语音和视频协议(H323、SIP、MGCP/H248)的识别和对NAT/Firewall的控制，来完成VOIP应用穿越 NAT/Firewall .在安全性上，MIDCOM方式可支持控制报文的加密，可支持媒体流的加密，因此安全性比较高。
　　如果在软交换设备上实现对SIP/H323/MGCP/H248协议的识别，就只需在软交换和NAT/FW设备上增加MIDCOM协议即可，而且以后新的应用业务识别随着软交换的支持而支持，此方案是一种比较有前途的解决方案，但要求现有的NAT/FW设备需升级支持MIDCOM协议，从这一点上来说，对已大量布署的NAT/FW设备来说，也是很困难的，同NAT/ALG方式有相同的问题。
　　三、STUN 方式
　　解决穿透NAT问题的另一思路是，私网中的VOIP终端通过某种机制预先得到出口NAT上的对外地址，然后在净载中所填写的地址信息直接填写出口 NAT上的对外地址，而不是私网内终端的私有IP地址，这样净载中的内容在经过NAT时就无需被修改了，只需按普通NAT流程转换报文头的IP地址即可，净载中的 IP地址信息和报文头地址信息是一致的。STUN协议就是基于此思路来解决应用层地址的转换问题。
　　STUN的全称是Simple Traversal of UDP Through Network Address Translators，即 UDP对NAT的简单穿越方式。 应用程序(即STUN CLIENT)向NAT外的STUN SERVER通过UDP发送请求STUN 消息， STUN SERVER收到请求消息，产生响应消息，响应消息中携带请求消息的源端口，即STUN CLIENT在NAT上对应的外部端口。然后响应消息通过NAT发送给STUN CLIENT，STUN CLIENT通过响应消息体中的内容得知其NAT上的外部地址，并将其填入以后呼叫协议的UDP负载中，告知对端，本端的RTP接收地址和端口号为NAT
 外部的地址和端口号。由于通过STUN协议已在NAT上预先建立媒体流的NAT映射表项，故媒体流可顺利穿越NAT.
　　STUN协议最大的优点是无需现有NAT/FW设备做任何改动。由于实际应用中，已有大量的NAT/FW，并且这些NAT/FW并不支持VoIP的应用，如果用MIDCOM或NAT/ALG方式来解决此问题，需要替换现有的NAT/FW，这是不太容易的。而采用STUN方式无需改动NAT/FW，这是其最大优势，同时STUN方式可在多个NAT串联的网络环境中使用，但MIDCOM方式则无法实现对多级NAT的有效控制。
　　STUN的局限性在于需要VOIP终端支持STUN CLIENT的功能，同时STUN并不适合支持TCP连接的穿越，因此不支持H323.另外 STUN方式不支持对防火墙的穿越，不支持对称NAT (Symmetric NAT)类型(在安全性要求较高的企业网中，出口NAT通常是这种类型)穿越。
　　四、TURN方式
　　TURN方式解决NAT问题的思路与STUN相似，也是私网中的VOIP终端通过某种机制预先得公网上的服务地址(STUN方式得到的地址为出口 NAT上外部地址，TURN方式得到地址为TURN Server上的公网地址)，然后在报文净载中所要求的地址信息就直接填写该公网地址。
　　TURN的全称为Traversal Using Relay NAT，即通过Relay方式穿越NAT.TURN应用模型通过分配 TURN Server的地址和端口作为私网中VOIP终端对外的接受地址和端口，即私网终端发出的报文都要经过TURN Server进行Relay转发，这种方式除了具有STUN方式的优点外，还解决了STUN应用无法穿透对称NAT(Symmetric NAT)以及类似的Firewall设备的缺陷，同时TURN支持基于TCP的应用，如H323协议。此外TURN Server控制分配地址和端口，能分配RTP/RTCP地址对(RTCP端口号为RTP端口号加1)作为私网终端用户的接受地址，避免了STUN方式中出口NAT对RTP/RTCP地址端口号的任意分配，使得客户端无法收到对端发来的RTCP报文(对端发RTCP报文时，目的端口号缺省按RTP端口号加
 1发送)。
　　TURN的局限性在于需要VOIP终端支持TURN Client，这一点同STUN一样对网络终端有要求。此外，所有报文都必须经过TURN Server转发，增大了包的延迟和丢包的可能性。
