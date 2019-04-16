# 通过Stratus 服务器在Flash Player中使用RTMFP 开发P2P应用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月31日 23:46:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：208









通过Stratus 服务器在Flash Player中使用RTMFP 开发 点对点应用（网文转摘）
 作者：Jozsef Vass 译者：巴巴鲁 （请转载时注明和改编时出处，谢谢）



Adobe Flash Player 10 and Adobe AIR 1.5 introduce a new communications protocol, Real-Time Media Flow Protocol (RTMFP), whose low latency, end-to-end peering capability, security, and scalability make it especially well suited for developing real-time collaboration applications by not only providing superior user experience but also reducing operators' costs.

Adobe Flash Player 10 和 Adobe AIR 1.5 引入了一个新的通讯协议，Real-Time Media Flow Protocol (RTMFP)，其低延迟，端到端的对等功能，安全性和可扩展性使它特别适合开发实时协作应用，不仅提供卓越的用户体验，而且运营商降低成本。

Earlier versions of Flash Player use Real-Time Messaging Protocol (RTMP) and require Adobe Flash Media Server (FMS) for interactive collaboration applications (such as Adobe Acrobat Connect Pro) or audio/video streaming. While RTMP is an excellent choice for streaming media, shared objects, or remoting, it has limited ability of meeting real-time requirements of interactive audio and video communications.

早前的Flash Player 版本使用Real-Time Messaging Protocol (RTMP) 需要Adobe Flash Media Server (FMS)提供合作应用(例如 Adobe Acrobat Connect Pro)或者音频视频流。RTMP是streaming media, shared objects, 和 remoting连接的卓越选择，它满足实时性要求的交互式音频和视频通信的能力有限。

In order to use RTMFP, Flash Player endpoints must connect to an RTMFP-capable server, such as the Adobe Stratus beta service or a future version of FMS. Stratus is a hosted rendezvous service that aids establishing communications between Flash Player endpoints. Unlike FMS, Stratus does not support media relay, shared objects, scripting, etc. So by using Stratus, you can develop applications only where Flash Player endpoints are directly communicating with one another.

为了能够使用RTMFP，Flash Player 客户端必须连接到一个支持RTMFP协议的服务器，例如Adobe Stratus beta 服务器或者是新版本的FMS。Stratus 是一台用于Flash Player 客户端间通信的主机。不同于FMS, Stratus 不支持视频转播，shared objects,脚本，等。因此，使用Stratus，只可以开发Flash Player的客户端直接相互交流的应用程序。

Flash Player is already the market leader in online video distribution over the web. With the introduction of RTMFP and advanced media compression technologies, Flash Player 10 is well positioned as the leader in real-time communications as well.

Flash Player 已经在web视频领域占有重要的市场份额。由于采用RTMFP和先进的媒体压缩技术，将有利于Flash Player 10处于实时通信的领先地位。

In this article, I first highlight the benefits of using RTMFP in real-time communications applications. Second, I describe the new ActionScript 3.0 API for managing direct end-to-end RTMFP connections. Finally, I present our VideoPhone sample application.


 在这篇文章中，我首先强调在即时通讯领域使用RTMFP的优势。其次，我将要介绍直接管理点对点RTMFP管理所对应的新的ActionScript 3.0 API。最后，我介绍我们的VideoPhone应用实例。

Requirements


 要求

In order to make the most of this article, you need the following software and files:

为了使用文中的文件，你需要以下软件和文件：

Flex Builder 3

Try
 Buy
 Note: Please follow the instructions in Introducing Flex SDK 3.2 and Flex Builder 3.0.2 to install Flex Builder 3.0.2. This also includes Flex SDK 3.2, which is required to build applications targeting Flash Player 10. Although you may also use Adobe Flash Professional CS4 for development, this article instructs how to build a sample application using Flex Builder 3.

注意：请按照Introducing Flex SDK 3.2 and Flex Builder 3.0.2指南中的方法去安装Flex Builder 3.0.2.这里面包括Flex SDK 3.2, 这是为了建立 Flash Player 10应用。尽管你也可以使用Adobe Flash Professional CS4 作为开发工具，但这篇文件指导你如果使用Flex Builder 3建立一个简单的应用.

Flash Player 10

Download
 Sample files:

stratus_article_assets.zip (ZIP, 13K)
 Prerequisite knowledge

预备知识

Familiarity with ActionScript 3.0 and Flex Builder is required.

熟悉ActionScript 3.0 和 Flex Builder 是必要的。

Benefits of RTMFP

RTMFP的好处

Real-Time Media Flow Protocol (RTMFP) is a new communications protocol introduced in Flash Player 10 and also available in AIR 1.5. One of its major differentiators from Real-Time Messaging Protocol (RTMP), which is based on the Transmission Control Protocol (TCP) and exclusively used in previous versions of Flash Player, is that RTMFP is built on User Datagram Protocol (UDP).


 Real-Time Media Flow Protocol (RTMFP)是一个引入到Flash Player 10 和 in AIR 1.5新的通讯协议。同Real-Time Messaging Protocol (RTMP)的主要不同点之一是，RTMP基于传输控制协议 (TCP) 和 先前版本的Flash Player，RTMFP建立在 User Datagram Protocol (UDP)协议.

While TCP provides reliable data delivery (well applicable for file transfer, e-mail, etc.), it does not provide any end-to-end delay guarantees. Reliable data transmission in TCP is achieved by re-transmission of lost data, which introduces latency. Because minimizing end-to-end delay is one of the most important goals in real-time communications (a few hundred milliseconds' delay may render a conversation unusable), TCP is not well suited for this purpose. Transmission error resilience and recovery form an integral part of most advanced audio and video compression techniques—such as Speex audio and H.264 video codec, both available in Flash Player 10. Reliable delivery provided by TCP is therefore not needed. As a result, UDP, which provides an efficient and rapid data delivery, is popularly used in real-time collaboration applications where minimizing end-to-end delay is of paramount importance. Another advantage of UDP over TCP that it enables end-to-end peering—that is, direct data transmission between two clients located behind network address translators (NATs).

TCP提供了可靠的数据传输（也适用于文件传输，电子邮件等） ，它没有提供任何端到端延迟保证。可靠的数据传输的TCP实现了重新传输丢失的数据，其中包括了延迟。由于尽量减少端到端延迟是实时通信 （几百毫秒的延迟可能使一个会话不可用）中最重要的一个目标，TCP是不适合用于这一目的。传输错误复原和恢复不可分割的组成部分，最先进的音频和视频压缩技术，如Speex音频和H.264视频编解码器，可同时在Flash Player 10 。因此，可靠的交付所提供的TCP没有必要的。因此， UDP连接，它提供了一个高效，快速的数据传输，是普遍使用的实时协作应用，尽量减少端到端延迟是至关重要的。另一个优势在于 UDP连接，它使端到端的对等，也就是说，数据直接的传输客户之间后面网络地址转换（ NAT ） 。

When compared to RTMP, RTMFP provides the following advantages for real-time communications:

当同RTMP做对比，RTMFP提供下列高级的即时通讯：

Low latency: Since RTMFP is built on top of UDP, it provides minimal latency for real-time communications. It is important to note that RTMFP provides both reliable and unreliable service. When sending data between two Flash Player instances (for example, using the NetStream.send() method), reliable data transmission is used. When sending Speex audio between two Flash Player instances, unreliable delivery is used, providing the smallest possible latency.
 低延迟：一但RTMFP建立起了UDP，它将给即时通讯提供最小的延迟。重要的是要注意RTMFP提供了可靠和不可靠的服务。当在两个Flash Player 实例之间发送数据的时候（例如，使用NetStream.send()方法），可靠的数据传输被使用。当在两个Flash Player 实例之间发送Speex 音频的时候，不可靠的交互方式被使用，以提供最小的延迟。
 End-to-end media delivery: Media is sent directly between two Flash Player instances without routing through a central relay server. When compared to RTMP, where all data is sent through Flash Media Server, RTMFP not only further reduces end-to-end delay, but also eliminates costs associated with a central data relay, thus lending itself to extremely scalable deployments.
 点对点媒体传输：媒体直接发送给两个Flash Player的情况下，不通过路由，而是通过一个中央中继服务器。当同RTMP相比发现，在所有通过Flash Media Server传送的数据， RTMFP不仅进一步降低了端到端的延迟，而且也消除了中央数据中继的相关开销，因此，有助于自身的可扩展性部署。
 Data prioritization: Audio is transmitted with higher priority than video and non-time critical data (such as instant message, etc.). This can significantly enhance user experience over a bandwidth constrained communications channel.

数据的优先次序：音频传输具有较高优先于视频和非时间关键数据（如即时信息等） 。这可以通过带宽通信通道的限制大大提高在用户体验。
 All of these features represent tremendous benefits for real-time communications, providing a significantly greater user experience than is achievable with earlier versions of Flash Player.

所有这些功能代表了应用于实时通信的巨大优势，提供了一个极大的用户体验，其成就比早期版本的Flash播放器的效应更为巨大。

Firewall traversal

穿越防火墙

RTMFP is built on top of UDP, which enables direct connection between clients even if they are located behind NATs or firewalls. In order for RTMFP to work, your firewall must be configured to allow outgoing UDP traffic. While this is the case with most consumer or small office/home office (SOHO) firewalls, many corporate firewalls block UDP traffic altogether.


 RTMFP是建立在UDP连接的基础上，使客户端直接的通信，即使它们位于NATs(译者注：是一个网络协议允许客户端后面的NAT （网络地址转换） ，以找出其公共地址，类型的NAT是延迟和互联网方面的端口相关的网络地址转换，尤其本地端口。此信息是用来建立UDP连接（用户数据报协议）之间的沟通 两个主机都是延迟的NAT路由器。该议定书是指在RFC 3489)或防火墙。为了RTMFP工作，您的防火墙必须配置为允许即将发出的UDP通信。大多数的消费者或小型办公室/家庭办公室（ SOHO ）的防火墙是这种情况，许多企业防火墙完全阻止UDP通信。

One solution is to configure Flash Player to use a TURN proxy (Traversal Using Relays around NAT). Flash Player supports IETF Internet Draft draft-ietf-behave-turn-08 without authentication. If the network administrator configures a TURN proxy that allows outgoing UDP, Flash Player can be configured by adding the following line in mms.cfg (for more information on Flash Player configuration and the location of mms.cfg, please read the Adobe Flash Player Administration Guide for Flash Player 10):

一种解决办法是配置的Flash Player使用转向代理 （遍历周围可用的NAT ） 。 Flash Player的支持IETF的因特网草案draft-ietf-behave-turn-08。如果网络管理员配置转向代理，允许即将发送的UDP连接， Flash播放器可以增加在mms.cfg的配置（更多的信息Flash Player的配置和位置mms.cfg ，请阅读Adobe Flash Player的管理指南的Flash Player 10 ） ：

RTMFPTURNProxy=ip_address_or_hostname_of_TURN_proxy

Direct UDP traffic is always attempted and the TURN proxy is only used as a backup: it is used for UDP traffic that cannot flow between Flash Player and Stratus (in case of UDP blocking firewall) or between Flash Player endpoints.

直接UDP通信总是被尝试，转向代理只是用来作为备份：它是用于UDP通信，不能在Flash Player和Stratus （如UDP协议封锁防火墙）之间或Flash Player的端点之间流动。

Even if your firewall enables outgoing UDP traffic, it is possible that end-to-end peering cannot be established due to a combination of firewalls. When one endpoint is located behind a so-called "symmetric firewall," end-to-end communications may not be possible. (For a classification of firewalls, please see the Network address translation entry on Wikipedia.) In this situation, you may use a TURN proxy to aid firewall traversal.

即使你的防火墙使即将发送的UDP通信的用，可能和你对应端的防火墙不能够允许通过。当一个端点设在一个所谓的“对称的防火墙， ” 的后面，端到端的通信可能是不可以实现。 （对于多种防火墙，请进入维基百科参阅 网络地址转换。 ）在这种情况下，你可以使用转代理援助你防火墙穿越。

Stratus service
 Stratus 服务器

Flash Player instances must connect to the Adobe Stratus service (using rtmfp://stratus.adobe.com) in order to communicate with one another. Stratus is a hosted rendezvous service that helps Flash Player instances contact one another even if they are located behind NATs. Although connecting to Stratus service is very similar to connecting to Flash Media Server, Stratus does not provide any of the typical Flash Media Server features (media relay, shared objects, remoting, etc.). Flash Player endpoints must stay connected to Adobe Stratus during the entire time of communication. In order to access Stratus, you will need a developer key that is generated when you create your Adobe Developer ID.

Flash Player实例必须连接到[Adobe Stratus service](http://labs.adobe.com/technologies/stratus/)（使用rtmfp ： / / stratus.adobe.com ） ，用以彼此的通讯。 Stratus是提供会合服务的主机，帮助Flash Player实例间的互相联系，即使它们位于NATs的后面。虽然连接到Stratus服务非常相似连接到Flash Media Server，Stratus没有提供任何Flash Media Server 典型的功能（媒体中继，共享对象，远程等）。 Flash Player客户端必须保持在整个通讯期间一直与Adobe Stratus连接。为了获得Stratus，您将需要您从Adobe公司申请来的开发密钥。

RTMFP support is being planned for future version of Flash Media Server (no release date). With Flash Media Server, it will be possible to enable communications between Flash Player 9 or earlier clients (using RTMP) and Flash Player 10 clients (using RTMFP).

RTMFP支持正在计划添加到未来版本的Flash Media Server（无发行日期） 。这样Flash Media Server，将有可能同Flash Player 9或更早的客户（使用RTMP ）通信和Flash Player 10个客户端（使用RTMFP ） 通信。


 Security

安全

RTMFP provides secure communications between endpoints. It uses a 128-bit AES with the key negotiated using the Diffie-Hellmann key exchange method. However, it does not provide strong endpoint authentication such as SSL or RTMPS. To aid endpoint authentication, RTMFP and ActionScript expose secure nonces to application developers. These nonces are available at both communicating Flash Player endpoints and are guaranteed to match. By verifying these nonces, end users can ensure that there is no man-in-the-middle attack. These nonces can also be used to develop key continuity mechanism.

RTMFP 提供终端设备之间的安全通信。它的密钥采用128位AES谈判使用Diffie-Hellmann密钥交换方法。不过，这并不提供强大的终端认证，如SSL或RTMPS 。为了帮助端点认证， RTMFP和ActionScript揭露给应用开发者secure nonces。这些nonces可在双方沟通的Flash Player的终点，并保证比赛。通过核实这些nonces ，最终用户可以确保没有人在中间攻击。这些nonces还可以用来开发关键的连续性机制。

It is important to note that Flash Player only enables sending media from your microphone and webcam devices to other Flash Player endpoints that subscribe to your media streams. Flash Player does not relay data on behalf of any other Flash Player endpoints (such as in a multicast scenario).

重要的是要注意到， Flash播放器不仅从您的麦克风和摄像头设备发送媒体，其他的Flash Player端点订阅您的媒体流。代表Flash播放器并不中继任何其他Flash Player的端点数据（如在一个多播的情况） 。

For more information on RTMFP, please read the FAQ on Adobe Labs:

对于更多关于RTMFP的信息，请阅读Adobe Labs 上的帮助：

Real‐Time Media Flow Protocol FAQ (PDF, 166K)
 ActionScript 3.0 API supporting RTMFP
 ActionScript 3.0 API 支持 RTMFP
 There is a new ActionScript 3.0 API in Flash Player 10 to support RTMFP. Connecting to the Stratus service and creating end-to-end media streams are analogous to working with Flash Media Server. Please note that you must use ActionScript 3.0 with either Flash Professional CS4 or Flex Builder 3 targeting Flash Player 10 or AIR 1.5.

有一个新版本的ActionScript 3.0API支持Flash Player 10的RTMFP 。连接到Stratus错服务和创造端到端媒体流的方法类似于Flash Media Server的工作方法。请注意，您必须使用的ActionScript 3.0或者Flash Professional CS4或Flex Builder 3 构建目标于Flash Player 10或AIR 1.5 。

As I mentioned before, first you must connect to the Adobe Stratus service:

正如我前面提到的，首先你必须连接到Adobe公司Stratus的服务：

private const StratusAddress:String = "rtmfp://stratus.adobe.com";private const DeveloperKey:String = "your-developer-key";private var netConnection:NetConnection; netConnection = new NetConnection();netConnection.addEventListener(NetStatusEvent.NET_STATUS,netConnectionHandler);netConnection.connect(StratusAddress + "/" + DeveloperKey);The developer key is issued when you sign up for an Adobe Developer Connection account and is available on the Adobe Stratus beta service site.

开发者钥匙是你通过登陆你的Adobe公司开发者帐户申请得到，这个申请在Adobe Stratus beta服务的网站。

Upon successful connection to Stratus, you get a NetConnection.Connect.Success event. There could be several reason for connection failure. If you provide an invalid developer key or incorrectly specify Stratus address, you'll receive NetConnection.Connect.Failed. If your firewall blocks outgoing UDP traffic, you'll receive the NetConnection.Connect.Failed event after a 90-second timeout.

在成功连接到Stratus，你得到NetConnection.Connect.Success事件。失败可能有几个方面的原因。如果您提供了一个无效的开发者或不正确的钥匙指定地址，您将收到NetConnection.Connect.Failed 。如果你的防火墙阻挡即将发送的UDP通信，您会收到的NetConnection.Connect.Failed事件后， 90秒超时。

After successfully establishing a connection to the Stratus service, you are assigned a unique 256-bit peer ID (NetConnection.nearID). Other Flash Player endpoints must know this peer ID in order to receive your published audio/video streams. It is out of the scope of Flash Player or the Stratus service how these peer IDs are exchanged among Flash Player endpoints. For exchanging peer IDs, you may use an XMPP service or a simple web service, as the Video Phone sample application does.

在成功建立连接的Stratus服务中，您被分配一个独特的256位peer ID（ NetConnection.nearID ） 。其他Flash Player的端点必须知道这个peer ID，以便收到您发表的音频/视频流。Flash Player或Stratus的服务是如何将这些ID在需要通讯的Flash Player客户端内传递，不在文章讨论范围内。对于交换ID ，你可以使用一个XMPP协议的服务或一个简单的网络服务，如视频电话样本应用程序。

Direct communications between Flash Player instances is conducted using unidirectional NetStream channels. That is, if you want two-way voice conversation, each Flash Player endpoint must create a sending NetStream and a receiving NetStream.

Flash Player实例直接通讯使用单向网流渠道。也就是说，如果你想双向语音交谈，每个Flash Player的端点必须建立一个发送NetStream和接收NetStream。




 First, create a sending NetStream:

首先创建一个发送NetStream：



```
private var sendStream:NetStream; 
sendStream = new NetStream(netConnection,NetStream.DIRECT_CONNECTIONS);
sendStream.addEventListener(NetStatusEvent.NET_STATUS,netStreamHandler);
sendStream.publish("media");
sendStream.attachAudio(Microphone.getMicrophone());
sendStream.attachCamera(Camera.getCamera());
```





This means that media is published as an end-to-end stream. Since Stratus cannot relay media, you can publish only end-to-end streams. This stream will include both audio and video from your local default devices chosen by the Settings Manager.

这意味着，媒体作为一个端到端的流发布。由于Stratus不能中继媒体，您只可以发布端到端的流。从您的设置管理器选择本地默认设备发出的流媒体将包括音频和视频。

Note: Audio/video is not sent out until another Flash Player endpoint subscribes to your media stream.

注：音频/视频无法发送，直到另一Flash Player的客户端订阅您的媒体流。

Now, create the receiving NetStream:

现在，创建接收NetStream：



```
private var recvStream:NetStream; 
recvStream = new NetStream(netConnection, id_of_publishing_client);
recvStream.addEventListener(NetStatusEvent.NET_STATUS, netStreamHandler);
recvStream.play("media");
```





At this point, you hear audio and you can create a Video object to display video. In order to create the receiving NetStream, you must know the 256-bit peer ID of the publisher (id_of_publishing_client). In order to receive audio/video, you must know the name of the stream being published.

在这一点上，你听到声音，你可以创建一个视频对象显示视频。为了创造接收NetStream，您必须知道发布者的256位peer ID（发布客户端的 id_ ） 。为了接收音频/视频，您必须知道被发布出来的流的名字。

Advanced topics
 高级主题

The publisher has fine control over which endpoint can receive its published stream. When a subscriber attempts to receive a published stream, the onPeerConnect() method is invoked (default implementation simply returns true) on the published NetStream. The publisher could disallow certain Flash Player endpoints to receive its media:

发布者有良好的控制权而接收端可以接收其发布的流。当一个用户试图获得发布的流时，onPeerConnect （）方法被调用（默认简单执行返回true ）对发布的NetStream。发布者可以禁止某些Flash Player的终端接收媒体：



```
var o:Object = new Object();
o.onPeerConnect = function(subscriberStream:NetStream):Boolean{   
if (accept)    { 
     return true;    
}   else   { 
     return false;   
 }}
sendStream.client = o;
```





On the publisher side, the NetStream.peerStreams property holds all the subscribing instances of the publishing NetStream. For example, using sendStream.send() will send the same data to all subscribers. You can use the following to send information to a specific subscriber:

在发布方， NetStream.peerStreams属性中拥有所有订阅发布的实例。例如，使用sendStream.send （）将发送相同的数据到所有用户。您可以使用下面的方法将信息发送到一个特定的用户：

sendStream.peerStreams[i].send()The NetConnection.maxPeerConnections property specifies the number of peer streams that are allowed to connect to the publisher. The default value is set to 8 but, in practice, depending on your application, you must consider that most ISPs provide asymmetric Internet access. Figure 1 illustrates the direct communication among three instances of Flash Player. Each Flash Player endpoint sends and receives two streams, creating a fully connected mesh. Since Internet download capacity is generally much higher than upload capacity, you must be extra careful not to overload the end-user's uplink.

NetConnection.maxPeerConnections属性指定被允许连接发布者的peer流的数量。默认值是设定为8但在实践中，这取决于您的应用程序时，必须考虑到大多数互联网服务供应商提供非对称互联网接入服务的许可。图1说明了直接和三个Flash Player的实例通讯 。每个Flash Player客户端发送和接收两个流，建立一个完全连接网格。从互联网下载的能力普遍高于上传的能力，你必须要格外小心，不要超负荷用户终端的上行能力。

![](https://img-blog.csdn.net/20131026131115062)

Figure 1. End-to-end connections using the Stratus service

图1 使用Stratus服务点对点连接

The NetConnection.unconnectedPeerStreams property is an array of NetStreams that are not associated with a publishing NetStream yet. When a publishing stream matches a subscribing stream name, the subscribing NetStream is moved from this array to the publishing NetStream.peerStreams array.

NetConnection.unconnectedPeerStreams属性是一个没有相关发布的NetStreams数组。当一个发部流同一个订阅流相互竞争时，订阅NetStream从NetStream.peerStreams的数组中移除。

Exploring the Video Phone sample application

探索视频电话示例应用程序

We have developed a sample video phone application for illustrating how to use end-to-end capabilities of Flash Player 10. It is also available as part of this article.




