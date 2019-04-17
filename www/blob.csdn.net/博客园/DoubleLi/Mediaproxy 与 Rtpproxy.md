# Mediaproxy 与 Rtpproxy - DoubleLi - 博客园






**Mediaproxy：**

Mediaproxy是Opensips的一个模块，**它用来实现现有大多数****sip客户端的自动NAT****穿透。**这就意味着，当使用mediaproxy模块时，不需要对NAT盒子进行任何配置就能使位于NAT之后的客户端正常工作。

工作原理：

    此解决方案就是在两个sip用户代理之间放置一个媒体中继，并修改他们的sdp消息，使每个参与方都与中继交互，而对他们自身来说 就像直接与对端交互一样。

Mediaproxy由两部分组成：

    （1）Opensip的mediaproxy模块

    （2）外部应用程序MediaProxy，它使用一个分配器（dispatcher）和多级分布媒体中继。



     Meidaproxy的分配器与opensips运行在同一台机器上，他的主要作用就是呼叫选择媒体中继。媒体中继可以与分配器运行在同一台机器上，也可以运行在多台远程主机上，他的主要作用就是在通话双方之间转发媒体流。为了能够在两个用户代理之间起到中继的作用，运行模块/代理服务器的计算机必须拥有一个公共的ip地址。



**Rtpproxy:**



This module is used by OpenSIPS to communicate with RTPProxy,** a media relay proxy used to make the communication between user agents behind NAT possible.**

** 处理两个位于nat之后的客户端进行媒体转发**

This module is also used along with RTPProxy to record media streams between user agents or to play media to either UAc orUAs.



![](http://img.blog.csdn.net/20160414192122646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 MediaProxy is a media relay for RTP/RTCP and UDP streams that works in tandem with OpenSIPS to provide NAT traversal capability for media streams from SIP user agents located behind NAT. **MediaProxy supports ICE negotiation by behaving like a TURN relay candidate and the policy can be controlled from OpenSIPS configuration.**

  rtp传输：当使用stun能够穿透nat，获取对方的ip地址后，可以直接进行端对端的传输。面对对称型nat，stun不能穿透的情况下，可以使用turn进行穿透，即通过mediaproxy进行转发。

 ---------------------------------------------------------

关于nat的一些东西

**NAT的作用**



NAT 不仅实现地址转换，同时还起到防火墙的作用，隐藏内部网络的拓扑结构，保护内部主机。 NAT 不仅完美地解决了 lP 地址不足的问题，而且还能够有效地避免来自网络外部的攻击，隐藏并保护网络内部的计算机。 这样对于外部主机来说，内部主机是不可见的。**但是，对于P2P 应用来说，却要求能够建立端到端的连接，所以如何穿透NAT 也是P2P 技术中的一个关键。**

**![](http://img.blog.csdn.net/20160414193334729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



  由于nat的透明性，所以clienta-1和clientb-1不用关心和server通信的过程，他们只需知道server开放服务的地址和端口即可。

**使用udp穿透nat**

 通常情况下，当进程使用udp和外部主机通信时，NAT会建立一个session,这个session能够保留多久并没有标准，或许几秒，几分钟，几小时。假设Clienta-1在应用程序中看到了Clientb-1在线，并且想和Clientb-1通信，一种办法是server作为中间人，负责转发Clienta-1和Clientb-1之间的消息，但是这样服务器太累。另一种方法就是让Clienta-1和Clinetb-1建立端到端的连接，然后他们自己通信。这也就是p2p连接。

  （1 ）**全锥NAT **，穿透全锥型NAT 很容易，根本称不上穿透，因为全锥型NAT 将内部主机的映射到确定的地址，不会阻止从外部发送的连接请求，所以可以不用任何辅助手段就可以建立连接。



 （2 ）**限制性锥NAT 和端口限制性锥NAT **（简称限制性NAT ），穿透限制性锥NAT 会丢弃它未知的源地址发向内部主机的数据包。所以如果现在ClientA-1 直接发送UDP 数据包到ClientB-1 ，那么数据包将会被NAT-B 无情的丢弃。

所以采用**下面的方法来建立ClientA-1 和ClientB-1 之间的通信**

**1** Clienta-1(202.103.142.29:5000)发送数据包到Server,请求和Clientb-1(221.10.145.84:6000)通信。

2 Server将Clienta-1的地址和端口（202.103.142.29:5000）发送给Clientb-1,告诉Clinetb-1,Clineta-1想和他通信。

3** Clinetb-1向Clineta-1(202.103.142.29:5000)发送udp数据包，当然这个包在到达NAT-A的时候，还是会被丢弃，这并不是关键，因为发送这个udp包只是为了让NAT-B记住这次通信的目的地址，端口号，当下次以这个地址和端口号为源的数据到达的时候就不会被NAT-B丢弃，这样就在NAT-B上打了一个从Clinetb-1到Clineta-1的洞。**

4 为了让Clineta-1知道什么时候才可以向Clinetb-1发送数据，所以Clientb-1在向Clienta-1(202.103.142.29:5000)打孔之后还要向Server发送一个消息，告诉Server它已经准备好了。

5 Server发送一个消息给Clineta-1,内容为：Clinetb-1已经准备好了，你可以向Clientb-1发送消息了。

6 **Clienta-1向Clientb-1发送udp数据包，这个数据包不会被NAT-B丢弃，以后Clinetb-1向Clineta-1发送的数据包也不会被Clienta-1丢弃，因为NAT-A已经知道是Clienta-1首先发起的通信.至此，Clineta-1和Clientb-1就可以通信了。**



**  使用TCP 穿透NAT**



使用TCP 协议穿透NAT 的方式和使用UDP 协议穿透NAT 的方式几乎一样，没有什么本质上的区别，只是将无连接的UDP 变成了面向连接的TCP 。值得注意是：



1.     ClientB-1 在向ClientA-1 打孔时，发送的SYN 数据包，而且同样会被NAT-A 丢弃。同时，ClientB-1 需要在原来的socket 上监听，由于重用socket ，所以需要将socket 属性设置为SO_REUSEADDR 。



2.     ClientA-1 向ClientB-1 发送连接请求。同样，由于ClientB-1 到ClientA-1 方向的孔已经打好，所以连接会成功，经过3 次握手后，ClientA-1 到ClientB-1 之间的连接就建立起来了。



**对称型NAT的穿透**



上面讨论的都是怎样穿透锥（Cone ）NAT ，对称NAT 和锥NAT 很不一样。对于 对称NAT ，当一个私网内主机和外部多个不同主机通信时，对称NAT 并不会像锥（Cone ，全锥，限制性锥，端口限制性锥）NAT 那样分配同一个端口。而是会新建立一个Session ，重新分配一个端口。参考上面穿透限制性锥NAT 的过程，在步骤3 时：ClientB-1 （221.10.145.84: ？）向ClientA-1 打孔的时候，对称NAT 将给ClientB-1 重新分配一个端口号，而这个端口号对于Server 、ClientB-1 、ClientA-1 来说都是未知的。同样， ClientA-1 根本不会收到这个消息，同时在步骤4 ，ClientB-1 发送给Server 的通知消息中，ClientB-1 的socket 依旧是（221.10.145.84:6000 ）。而且，在步骤6 时：ClientA-1 向它所知道但错误的ClientB-1 发送数据包时，NAT-1 也会重新给ClientA-1 分配端口号。所以，穿透对称NAT 的机会很小。下面是两种有可能穿透对称NAT 的策略。

**UDP 端口猜测策略**

   同时开放TCP 策略非常依赖于猜测对方的下一个端口，而且强烈依赖于发送连接请求的时机，而且还有网络的不确定性，所以能够建立的机会很小，即使Server 充当同步时钟的角色。下面是一种通过UDP 穿透的方法，由于UDP 不需要建立连接，所以也就不需要考虑“同时开放”的问题。

为了介绍ClientB-1 的诡计，先介绍一下STUN 协议。STUN （Simple Traversal of UDP Through NATs ）协议是一个轻量级协议，用来探测被NAT 映射后的地址：端口。STUN 采用C/S 结构，需要探测自己被NAT 转换后的地址：端口的Client 向Server 发送请求，Server 返回Client 转换后的地址：端口。

参考4.2 节中穿透NAT 的步骤2 ，当ClientB-1 收到Server 发送给它的消息后，ClientB-1 即打开3 个socket 。socket-0 向STUN Server 发送请求，收到回复后，假设得知它被转换后的地址：端口（ 221.10.145.84:600 5 ），socket-1 向ClientA-1 发送一个UDP 包，socket-2 再次向另一个STUN Server 发送请求，假设得到它被转换后的地址：端口（ 221.10.145.84:60 20 ）。通常，对称NAT 分配端口有两种策略，一种是按顺序增加，一种是随机分配。如果这里对称NAT 使用顺序增加策略，那么，ClientB-1 将两次收到的地址：端口发送给Server 后，Server 就可以通知ClientA-1 在这个端口范围内猜测刚才ClientB-1 发送给它的socket-1 中被NAT 映射后的地址：端口，ClientA-1 很有可能在孔有效期内成功猜测到端口号，从而和ClientB-1 成功通信。











