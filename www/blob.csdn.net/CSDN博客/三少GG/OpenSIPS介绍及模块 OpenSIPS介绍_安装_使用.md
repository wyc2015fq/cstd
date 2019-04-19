# OpenSIPS介绍及模块||OpenSIPS介绍_安装_使用 - 三少GG - CSDN博客
2012年11月26日 22:47:46[三少GG](https://me.csdn.net/scut1135)阅读数：5902

## OpenSIPS介绍及模块架构
1、引入
随着通信[IP](http://www.c114.net/keyword/IP)化的发展，IP传输的高带宽、低成本等优势使得越来越多的企业、[电信](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD)[运营商](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%D3%AA%EF%BF%BD%EF%BF%BD)加快建设基于IP的各种通信应用。在通信协议IP化发展中，[SIP](http://www.c114.net/keyword/SIP)协议毫无争议地成为各大电信运营商构建其未来[网络](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD)的基础协议，越来越多的SIP软件产品也不断出现在行业应用中。SIP协议的标准化，同时也造就了一大批优秀的开源软件产品，包括Asterisk、SipXecs、FreeSWITCH、OpenSIPS等SIP服务端软件，也包括X-lite、LinPhone、eyeBeam等SIP客户端软件。本文介绍了国外成熟的SIPServer开源项目OpenSIPS，并结合其它通信方面的开源项目对其应用情况进行了一些介绍。
2、OpenSIPS介绍
OpenSIPS是一个成熟的开源SIP[服务器](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD)，除了提供基本的SIP代理及SIP路由功能外，还提供了一些应用级的功能。OpenSIPS的结构非常灵活，其核心路由功能完全通过脚本来实现，可灵活定制各种路由策略，可灵活应用于语音、视频通信、[IM](http://www.c114.net/keyword/IM)以及Presence等多种应用。同时OpenSIPS性能上是目前最快的SIP服务器之一，可用于电信级产品构建。
2.1、功能特点
凭借其可扩展、模块化的系统架构，OpenSIPS提供了一个高度灵活的、用户可配置的路由引擎，可以为voice、video、IM和presence等服务提供强大高效的路由、鉴权、NAT、[网关](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD)协议转化等功能。由于其稳定高效等特点，OpenSIPS已经被诸多电信运营商应用在自己的网络体系中。其主要功能如下：
SIP注册服务器/代理服务器（lcr、dynamicrouting、dialplan）/重定向服务器
 SIP presence agent
 SIP B2BUA
 SIP IM Server
 SIP to SMS/XMPP网关
 SIP to XMPP网关
 SIP 负载均衡
 SIP NAT traversal
2.2、基本应用配置
OpenSIPS不但提供了丰富的功能，还具有操作简单的特点。所有OpenSIPS的应用功能都可以通过一个配置文件opensips.cfg来实现的。该配置文件主要分为三个部分，第一部分主要是全局变量的设置；第二部分主要是加载模块，并设置模块的相应参数；第三部分主要是路由的策略和功能应用。为了更清晰的呈现opensips.cfg配置文件带来的强大功能，接下来对这三个部分以简单的示例进一步说明。
第一部分全局变量的设置，通过一条语句就能指定用于侦听接收sip消息的端口和传输层协议。
第二部分负责模块的加载和参数配置。以之前的OpenSIPS的负载均衡功能配置为例，需要加载load_balancer模块，并进行配置。
第三部分是sip消息的路由和功能应用。还是以OpenSIPS的负载均衡功能为例。
从以上应用示例可以看到，通过在opensips.cfg中进行简单的配置，就能实现强大的功能。2.3、系统结构
OpenSIPS的架构开放灵活，其核心功能控制均可通过脚本控制实现，各个功能也通过模块加载的方式来构建。采用lex和yacc工具构建的配置文件分析器是其架构设计中的重要部分之一。通过这个分析器，opensips设计了自己的语法规则，使得我们可以适合[SIP](http://www.c114.net/keyword/SIP)规范的语言来进行配置文件中的脚本编写，从而达到简化程序以及方便代码阅读的目的。同时这样的设计也使opensips.cfg配置文件的执行速度达到了C语言的级别。其体系结构大体如下图：
框架的最上层是用于实现sip消息路由逻辑的opensips.cfg脚本配置，在配置文件中，可以使用Core提供的Parameter和Function，也可以使用众多Modules提供的Function。比如在之前的负载均衡示例中，is_method(“INVITE”)就属于textops模块提供的功能，src_ip和src_port都属于Core提供的参数。下层，提供了[网络](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD)传输、sip消息解析等基本功能。在左侧，通过相应的数据库适配器，可是使用多种数据库存取数据。在这样的体系结构下，我们就可以方便地通过增加功能module来添加我们需要的功能，而不会对原有系统造成影响。
除了以上所述的OpenSIPS的优点，OpenSIPS还提供了一系列的管理维护命令的接口。我们可以通过Core和Module提供的MI管理接口，方便的监控系统以及模块的状态。比如，通过Core的fifo ps命令，可以获取当前进程的状态；通过Core的fifoget_statistics命令，可以获得当前共享内存以及各进程私有内存的使用情况等等。通过MI管理接口，我们还可以方便地在运行时修改部分参数，比如，对于load_balancer模块，我们可以通过fifo
 lb_reload命令，更新目标组的配置信息，可以通过fifolb_status命令激活或关闭某个目标，这些命令在实际应用中都非常实用。如果希望通过WEB图形界面管理OpenSIPS，OpenSIPS社区还提供了OpenSIPS Control Panel 4.0产品。
3、与其它开源项目共同搭建[VOIP](http://www.c114.net/keyword/VOIP)服务
OpenSIPS提供了以上那么多的功能，那OpenSIPS是不是已经实现了PBX的功能了？不是！OpenSIPS并不具备一个媒体[服务器](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD)(MediaServer)的功能。媒体服务器主要提供了类似VoiceMail、呼叫中语音交换、会议服务、视频服务等一系列和语音、视频相关的服务；而OpenSIPS的主要功能主要在于代理、路由和[网关](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD)。因此，单独的OpenSIPS并不能够提供VOIP服务，只有和Asterisk等具备媒体功能的软件[整合](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD)，才能构建可靠的语音服务体系。
对于媒体服务器，开源世界也提供了很多选择，如老牌的Asterisk，以及功能全面的sipXecs以及专注于[IVR](http://www.c114.net/keyword/IVR)功能的FreeSwitch等，他们都是非常优秀的开源项目。其中Asterisk功能全面、灵活，但主要面向企业应用，在性能上稍差。但Asterisk提供了完善的PBX功能，可以连接多种不同的电话终端，支持多种主流的[IP电话](http://www.c114.net/keyword/IP%EF%BF%BD%E7%BB%B0)协议和系统接口。FreeSwtich专注于IVR功能，性能、可靠性非常高。近期FreeSwitch已被sipXecs采用作为其IVR部分功能。sipXecs则是一个功能比较全面的产品，包括IVR、VoiceMail、人工坐席等等，更难得的是SipXecs提供了良好的配置、管理界面，易于使用。
只要将OpenSIPS作为前置接入，将多个Asterisk、FreeSwitch、sipXecs挂接在其后，由OpenSIPS实现SIP消息的转发和负载均衡，就可以轻松地实现各种语音业务以及规模扩展。如下图所示。
4、小结
从文中介绍可以看出，OpenSIPS是一个成熟的[电信](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD)级SIPServer平台，可广泛应用于SIP应用的路由分发、负载均衡，可用于搭建SIP代理，提供SIP注册服务等。而且目前OpenSIPS自身也提供SIPPresence以及[IM](http://www.c114.net/keyword/IM)功能。同时，应该注意的是OpenSIPS本身并不提供媒体相关服务，如[呼叫中心](http://www.c114.net/keyword/%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD%EF%BF%BD)、VoiceMail等业务，该部分业务可通过FreeSwtich、sipXecs等平台实现。

运行：
1、运行mysql：
Service mysqld start
2、开启opensips：
opensipsctl restart
3、configure two users accounts.(格式：opensipsctl add user password)
opensipsctl add 1001 1001
opensipsctl add 1002 1002
注：opensipsctl rm -------remove users
    opensipsctl passwd-----change a password
4、  用已有用户和密码注册两个sip softphone（我用的X-lite和eyebeam）
5、  检查是否注册成功
opensipsctl ul show 
6、  检查是否在线
opensipsctl online
注：两个sip softphone注册必须使用数据库中已有的用户名和密码，否则将不能通过认证。
7、make a call from one phone to the other。
**OpenSips介绍**
# 1. SIP介绍：
## 1.1        SIP协议
SIP是类似于HTTP的基于文本的协议。SIP可以减少应用特别是高级应用的开发时间。由于基于IP协议的SIP利用了IP网络，固定网运营商也会逐渐认识到SIP技术对于他们的深远意义。
SIP(Session Initiation Protocol)是一个应用层的信令控制协议。用于创建、修改和释放一个或多个参与者的会话。这些会话可以好似Internet多媒体会议、IP电话或多媒体分发。会话的参与者可以通过组播（multicast）、网状单播（unicast）或两者的混合体进行通信。
　　使用 SIP，服务提供商可以随意选择标准组件。不论媒体内容和参与方数量，用户都可以查找和联系对方。SIP
对会话进行协商，以便所有参与方都能够就会话功能达成一致以及进行修改。它甚至可以添加、删除或转移用户。
SIP它既不是会话描述协议，也不提供会议控制功能。为了描述消息内容的负载情况和特点，SIP
使用 Internet 的会话描述协议 (SDP) 来描述终端设备的特点。SIP
自身也不提供服务质量 (QoS)，它与负责语音质量的资源预留协议(RSVP)
互操作。它还与若干个其他协议进行协作，包括负责定位的轻型目录访问协议(LDAP)、负责身份验证的远程身份验证拨入用户服务 (RADIUS)
以及负责实时传输的 RTP 等多个协议。
最新关于SIP协议的RFC请参考RFC3261
## 1.2        SIP基础
SIP在工作的方式上与HTTP协议相类似。SIP的地址像是e-mail的地址。SIP代理中使用的一个比较有趣的特性就是“别名（alias）”，也就是说你可以有多个SIP地址，譬如：[johndoe@sipA.com](mailto:johndoe@sipA.com)、[+554845678901@sipA.com](mailto:+554845678901@sipA.com)、[45678901@sipA.com](mailto:45678901@sipA.com)如果没有域名也可能是toto@10.10.10.10这样的SIP地址。
在SIP的体系结构中，有多个用户代理和提供不同服务的服务器。SIP使用点对点（peer-to-peer）的分布模型来和服务器进行消息的交互。服务器只进行消息（signaling）的处理，而用户代理的客户端和服务端既可以处理消息也可以处理媒体。下面的图描述了这样的一个体系：
在SIP模型中，用户代理，通常是一台SIP话机与它的SIP代理进行交互，从上图可以看到，外呼代理（outgoing
 proxy）将使用INVITE消息向外发出通话请求。
外呼代理将观察这通通话是否是被定向到外部的域名。然后它将向DNS服务器发出请求将目标域名解析为对应的IP地址。然后再将通话请求发送给DomainB对应的SIP代理。
呼入代理（incoming proxy）将在地址列表（location table）中查询agentB的IP地址。如果在地址（实际应用中相当于我们的手机号码）列表这个地址与之前在注册过程中的IP地址对应，那么呼入代理就可以定位这个地址了。现在就可以使用这个地址将通话请求发送到agentB了。
agentB收到这个SIP消息后（INVITE），就拥有了可以与agentA建立RTP会话（通常是音频方面的会话）所需要的信息。使用BYE消息可以终止这个会话。
## 1.3        SIP代理在VoIP提供者里的作用
通常VoIP服务的提供者们并不会实现像上幅图那样的纯粹的SIP四边形结构，他们不会允许你向一个外部的域名发送通话请求，因为如果这样，那么将影响他们的收入（revenue
 stream）。取而代之的是一个接近三角形的SIP网络结构。（如下图所示）
## 1.4        SIP工作原理
在上图中，你可以看到SIP体系结构中的主要的构成部件。所有的SIP消息都会经过SIP代理服务器。另一方面，由RTP协议承载的媒体流则是从一端直接流向另一端。我们将在下面的列表中简要的对其中的一些构成部件进行解释。
l用户代理客户端（UAC user agent client）——发起SIP消息的客户端或终端
l用户代理服务端（UAS user agentserver）——对接收到的从用户代理客户端发起的SIP消息进行相应的服务端。
l用户代理（UA user agent）——SIP终端（IP电话，电话适配器（ATA），软电话（softphone）如linphone等）
l代理服务器（Proxy Server）——从用户代理接收请求，并且如果指定的被请求的终端不在其域中时，会将请求发送给另外的SIP代理。
l重定向服务器（Redirect Server）——接收请求，但是不直接发送给被叫用户，而是向主叫方发送目的地址的信息。
l定位服务器（Location Server）——向代理服务器和重定向服务器提供被叫者的联系地址。
通常，物理上，代理服务器，重定向服务器和定位服务器存在于同一台电脑和软件中。
## 1.5        SIP注册过程
SIP协议中使用了一个构件叫做注册服务器。它不仅能够接收REGISTER消息请求，还能够将收到的消息包中的信息保存到管理对应域名的定位服务器上面。SIP协议具有发现能力；换句话说，就是如果一个用户要与另外一个用户开始会话，那么SIP协议必须要发现这个用户能够到达的主机存在。由于定位服务器可以收到请求消息并找到向什么地方发送，所以这个发现过程由定位服务器来完成。而这则是基于管理每个域的定位服务器维护着一个定位数据库的事实来实现的。注册服务器不仅可以接收客户端的IP地址，还能够接收其他类型的消息。比如，能够收到服务器上面的CPL（Call
 Processing Language）脚本。
在一台话机能够接收一通通话之前，它需要在定位数据库中有注册信息。在这个数据库中我们要拥有所有电话的各自的相关的IP地址。在我们的例子中，你将看到SIP用户8590＠voffice.com.br注册到200.180.1.1上面的过程。
RFC3665定义实现了一个最小的功能集合，这是使得SIP进行IP网络交互时的最好实践。下面的图就是根据RFC3665中的描述所画出的注册事务处理流程。
按照rfc3665中所说，与注册一个用户代理的过程相关的有五个基本的流程，如下所述：
1. 一个新的成功的注册（A successful new registration）——用户代理在发送Register请求后，将收到认证过程的挑战。我们将在阐述验证过程的章节中看到这个过程的细节。
2. 联系列表的更新（An update of the contact list）——由于不再是新的注册，消息中已经包含了摘要（digest），那么不会返回401消息。为了改变联系列表，用户代理仅仅需要发送一条在CONTACT头中带有新的联系信息的注册信息即可。
3. 请求获得当前的联系列表——在这种情况下，用户代理将把发送消息中的CONTACT头置空，表明用户希望向服务器询问当前的联系列表。在回复的200OK消息中，SIP服务器将把当前的联系列表放在其CONTACT的头中。
4.   取消注册（Cancellation of a registration）——用户代理在发送的消息中将EXPIRES头置成0，并且将CONTACT头设置为“＊”表示将此过程应用到所有存在的联系信息。
5.   不成功的注册（Unsuccessful Registration）——用户代理客户端（UAC）发送一条Register请求消息，收到一条“401Unauthorized”消息，事实上，这个过程同成功注册过程相同。但是接下来，它进行哈希运算尝试进行认证。而服务器检测到的是一个无效的密码，继续发送“401
 Unauthorized”消息。这个过程一直重复直到重复次数超过在UAC设置的最大值。
## 1.6        作为SIP代理服务器(我们采用的方式)
在SIP代理模式下，所有的IP消息都要经过SIP代理。这种行为在向诸如计费（billing）的过程中帮助很大，而且迄今为止，这也是一种最普遍的选择。但是它的缺点就是在会话建立过程中的所有的SIP交互中，服务器造成的额外开销也是客观的。要记住的是，即使服务器作为SIP代理在工作时，RTP包也总是直接从一端传送到另一端，而不会经过服务器。（只有业务管理流需要经过服务器）
注意：如1.1所描述，SIP既不是会话描述协议，也不提供会议控制功能。为了描述消息内容的负载情况和特点，SIP 使用 Internet 的会话描述协议 (SDP) 来描述终端设备的特点。SIP 自身也不提供服务质量 (QoS)，它与负责语音质量的资源保留设置协议 (RSVP) 互操作。它还与若干个其他协议进行协作，包括负责定位的轻型目录访问协议 (LDAP)、负责身份验证的远程身份验证拨入用户服务 (RADIUS) 以及负责实时传输的 RTP 等多个协议。
　　SIP 规定了以下基本的通信要求：
　　1．用户定位服务
　　2．会话建立
　　3．会话参与方管理
4． 特点的有限确定
## 1.7        SIP对话流程
这一节将通过一个简单的例子来介绍一些基本的SIP操作。先让我们来诊视下图展示的两个用户代理之间的消息顺序。你可以看到伴随这RFC3665描述的会话建立过程还有几个其它的流程。
我们在这些消息上标上了序号。在这个例子中用户A使用IP电话向网络上的另外一台IP电话发出通话请求。为了完成通话，使用了两个SIP代理。
用户A使用称为SIP URI的SIP标识向用户发出通话。URI就像是一个电子邮件地址，比如sip:[userA@sip.com](mailto:userA@sip.com)。一种可靠安全的SIPURI也可以被使用，譬如sips:[userA@sip.com](mailto:userA@sip.com)。使用SIPS建立的通话将会在主叫和被叫之间使用安全可靠的传输机制（TLS－Transport
 Layer Security）
事务（transaction）的建立始于用户A向用户B发送INVITE请求消息。INVITE请求中包含一些特定头域。这些头域被称之为属性，为消息提供了额外的一些信息。包括唯一的标识，目的地，还有关于会话（session）的信息。
第一行是消息的方法名（the method name）。接下来是列出的头域。这个例子包含了所需要的头的最小集合。我们将在下面简要的描述这些头域。
l*VIA：它包含了用户A等待发出请求对应响应的所在地址。还包含了一个叫做“branch”的参数，这个参数用来唯一的标识这个事务（transaction）。VIA头将最近从“SIP跳”（SIP hop）定义为IP，传输，和指定事务参数（The VIA header defines the last SIP hop as IP，transport，and transaction-specific parameters）。VIA专门用来路由响应消息。请求经过的每一个代理都会增加一个VIA头。而对于响应消息而言，相对于再次向定位服务器或是DNS服务器进行定位请求，使用VIA头进行路由将更加容易。*
*lTO ：它包含了名字（显示名(display name)）和最初选择的目的地的SIP URI（这里是sip：userB＠sip.com）。TO头域不被用来路由消息包。*
*lFROM：它包含了名字和表明主叫ID（caller ID）的SIP URI（这里是sip：*[*userA@sip.com*](mailto:userA@sip.com)*）。这个头域有一个tag参数，而这个参数包含了被IP电话添加进URI的一个随机字符串。是被用来进行辨识唯一性的。tag参数被用在TO和FROM头域中。作为一种普遍的机制用来标识对话（dialog），对话是CALL－ID和两个tag的结合，而这两个tag分别来自参与对话的双方。Tags在并行派生（parallel forking）中作用显著。*
*lCALL -ID：它包含了一个作为这通通话全局性的唯一的标识，而这个唯一标识是有一个随机字符串，来自IP电话的主机名或是IP地址结合而成的。TO，FROM的tag和CALL－ID的结合完整的定义了一个端到端的SIP关系，这种关系就是我们所知道的SIP对话（SIP dialog）*
*lCSEQ：CSEQ或者称之为命令序列（command sequence）包含了一个整数和一个方法名。CSEQ数对于每一个在SIP对话中的新请求都会递增，是一个传统的序列数。*
*lCONTACT：它包含一个代表直接路由可以联系到用户A的SIP URI，通常是有一个用户名和FQDN（fully qualified domain name）。有时候域名没有被注册，所以，IP地址也是允许使用的。VIA头告诉其他的组件向什么地方发送响应消息，而CONTACT则告诉其他组件向什么地方发送将来的请求消息。*
*lMAX-FORWARDS：它被用来限制请求在到达最终目的地的路径中被允许的最大跳数（hops）。由一个整数构成，而这个整数在每一跳中将会递减。*
*lCONTENT-TYPE：它包含了对内容消息的描述。*
*lCONTENT-LENGTH：它用来告知内容消息的字节数。*
会话的一些细节，像媒体类型和编码方式并不是使用SIP进行描述的。而是使用叫做会话描述协议（SDP RFC2327）来进行描述。SDP消息由SIP消息承载，就像是一封电子邮件的附件一样。
过程如下：
话机开始并不知道用户B和负责域B的服务器的位置。因此，它向负责sipA域的服务器发送INVITE消息请求。发送地址在用户A的话机中进行设置或通过DHCP发现。服务器sipA.com也就是我们知道的域sipA.com的SIP代理服务器。
*1. 在这个例子中，代理服务器收到INVITE请求消息并发送“100 trying”响应消息给用户A，表明代理服务器已经收到了INVITE消息并正在转发这个请求。SIP的响应消息使用一个三个数字组成的数字码和一条描述语句说明响应的类型。并拥有和INVITE请求一样的TO，FROM，CALL－ID和CSEQ等头域，以及VIA和其“branch”参数。这就使得用户A的话机同发出的INVITE请求联系在一起。*
*2. 代理A定位代理B的方法是向DNS服务器（SRV记录）进行查询以找到负责sipB的SIP域的服务器地址并将INVITE请求转发给它。在向代理B（译者注：这里作者写的是proxyA，但是应该是B）发送INVITE消息前，代理A将其自己的地址通过VIA头添加进INVITE，这就使得用户A的话机同INVITE请求的响应消息联系在了一起。*
*3. 代理B收到INVITE请求，返回“100 Trying”消息响应，表明其正在处理这个请求。*
*4. 代理B查询自己的位置数据库以找到用户B的地址，然后将自己的地址也通过VIA头域添加进INVITE消息发送给用户B的IP地址。*
*5. 用户B的话机收到INVITE消息后开始振铃。话机为了要表明这种情况（振铃），发送回“180 Ringing”响应消息。*
*6. 这个消息以相反的方向路由通过那两个代理服务器。每一个代理利用VIA头域来决定向哪里发送响应消息并从顶部将其自己的VIA头去除。结果就是，180 Ringing消息不需要任何的DNS查询，不需要定位服务的响应，也不需要任何的状态处理就能够返回到用户那里。这样的话，每一个代理服务器都能够看到由INVITE开始的所有消息。*
*7. 当用户A的话机收到“180 Ringing”响应消息后开始“回铃”，表明另一端的用户正在振铃。一些话机是通过显示一些信息进行表示的。*
*8. 在这个例子中，用户B对对方发起的通话进行了响应。当用户B响应时，话机发送”200 Ok“响应消息以表明通话被接起。“200 Ok”的消息体中包含了会话的描述信息，这些信息包括指定了编码方式，端口号，以及从属于会话的所有事情。作这项工作的就是SDP协议。结果就是，在从A到B（INVITE）和从B到A（200 OK）的两个阶段，双方交换了一些信息，以一种简单的“请求/响应”的模式协商了在这通通话中所需的资源和所需要的能力要求。如果用户B不想得到这通通话或是此刻处于忙线中，200 Ok将不会发出，取代它的是描述这种状况（这里是486 Busy Here）的消息。*
第一行是响应码和描述信息（OK）。接下来是头域行。VIA，TO，FROM，CALL－ID和CSEQ是从INVITE请求中拷贝的。有三个VIA头，一个是用户A添加的，另一个是代理A添加的，最后一个则是代理B添加的。用户B的SIP话机在对话的双方加入了一个TAG参数，这个参数在这通通话的以后的请求和响应消息中都将出现。
CONTACT头域中包含了URI信息，这个URI信息是用户B能够直接被联系到他们自己的IP话机的地址。
CONTENT－TYPE和CONTENT－LENGTH头域先给出了关于SDP头的一些信息。而SDP头则包含了用来建立RTP会话的媒体相关的参数。
*1. 在这个例子中，“200 Ok”消息通过两个代理服务器被送回给用户A，之后用华A的话机停止“回铃”表明通话被接起。*
*2. 最后用户A向用户B的话机发送ACK消息确认收到了“200 Ok”消息。在这里，ACK避开了两个代理服务器直接发送给用户B。ACK是SIP中唯一不需要进行响应的消息请求。两端在INVITE的过程中从CONTACT消息中了解双方的地址信息。也结束了INVITE/200 OK/ACK的过程，这个过程也就是我们所熟知的SIP三次握手。*
*3. 这个时候两个用户之间开始进行会话，他们以用SDP协议协商好的方式来发送媒体包。通常这些包是端对端进行传送的。在会话中，通话方可以通过发送一个新的INVITE请求来改变会话的一些特性。这叫做re－invite。如果re-invite不被接受，那么“488 Not Acceptable Here”响应就会被发出，但是会话不会因此而失败。*
*4. 要结束会话的时候，用户B产生BYE消息来中断通话。这个消息绕过两个代理服务器直接路由回用户A的软电话上。*
*5. 用户A发出“200 OK”响应消息以确认收到了BYE消息请求，从而结束会话。这里，不会发出ACK。ACK只在INVITE请求过程中出现。*
有些情况下，在整个会话过程中，对于代理服务器来说，能够待在消息传输的中间位置来观察两端的所有消息交互是很重要的。如果代理服务器想在INVITE请求初始化完成后还待在此路径中，可以在请求消息中添加RECORD－ROUTE头。用户B的话机得到了这个消息，之后在其消息中也会带有这个头，并且会将消息发送回代理。记录路由（Record
 Routing）在大多数的方案中都会被使用。
REGISTER请求是代理B用来定位用户B的方法。当话机初始化的时候或是在通常的时间间隔中，软电话B向在域sipB中的一个服务器（SIPREGISTRAR）发送REGISTER请求。注册服务器（REGISTER）将URI与一个IP地址联系在一起，这种绑定被存储在定位服务器上面的数据库里。通常，注册服务器，定位服务器，和代理服务器在同一台物理机器上，并使用相同的软件。OpenSER就能够扮演这三种角色。一个URI只能够在一个特定的时间内由一个单独的机器注册。
详细的各个流程，在后续文档中再详细说明。
# 2. OpenSips安装配置：
## 2.1.          环境：
1. 安装的版本：opensips 1.7.0，Mysql5.1.59。
2. 安装系统环境，由于opensips和mysql都不需要root权限，为了系统安全，特意建了一个work帐号为opensips的工作帐号，方便以后拷贝部署。
3. 硬件环境，公司标准Linux系统以及机房通用服务器，外网IP：113.57.248.49。
## 2.2.          Mysql安装：
1. Mysql5.1.59编译配置命令：
./configure  
'--prefix=/home/work/mysql''--with-plugin-innobase' '--enable-assembler' '--enable-profiling''--with-unix-socket-path=/home/work/mysql/tmp/mysql.sock''--with-charset=latin1' '--with-extra-charsets=binary,latin1,gbk,utf8,gb2312''--enable-local-infile' '--enable-thread-safe-client'
 '--without-docs''--without-man' '--without-bench'
这里需要注意的是，默认字符集需要选latin1，如果选gbk、utf8之类的字符集的话，在后面OpenSIPS创建数据库的时候会遇到字符集越位错误。
2. 安装完成以后根据需要（这里不做强制说明），修改mysql配置文件my.cnf
3. 启动命令：
cd /home/work/mysql/bin;
./mysqld_safe--defaults-file=/home/work/mysql/etc/my.cnf &
4. 初始化数据库：
cd /home/work/mysql/bin;
./mysql_install_db
5. 方便后续使用mysql，追加mysql环境变量
vi .bash_profile
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/work/mysql/lib/mysql
export PATH=$PATH:/home/work/mysql/bin
6. 用户授权：（需要远程管理数据库的情况下，对mysql进行帐号授权）
grant all privileges on opensips.* toroot@119.57.33.198 identified by 'letv123';
flush privileges;
## 2.3.          OpenSIPS安装：
1. 下载OpenSIPS 1.7.0源码包。opensips-1.7.0_src.tar.gz
2.解压OpenSIPS软件包后，进入目录，修改Makefile文件，删除Makefile 中的 exclude_modules 的 db-mysql,使OpenSIPS使用Mysql数据库。
3. 由于我们安装的mysql不是默认安装到/usr/目录下，因此需要特别修改以下文件，以便能顺利编译过：
vi Makefile.rules
把以下语句加入到文件头部的注释行之后（根据自己安装的mysql目录进行相应修改，如果默认安装在/usr/目录下，无需此步）：
#mysql defs, added by liuzhaohui
DEFS+= -I/home/work/mysql/include/
LIBS+= -L/home/work/mysql/lib/mysql/
修改Makefile
删除exclude_modules的db-mysql,使opensips使用mysql
4. 编译opensips，我们同样不默认安装opensips，而是把它安装在我们自己的目录下，因此需要增加prefix参数：
make prefix=/home/work/opensips all
make prefix=/home/work/opensips install
5. 进入opensips已安装目录，修改以下配置文件：
1.修改配置文件opensipsctlrc：
cd /home/work/opensips/etc/opensips/
vi opensipsctlrc
为保证opensips链接数据库不出现连接错误以及其他异常的问题，建议把数据库主机配置项DBHOST的值由localhost修改成ip地址（127.0.0.1）
## database host
 DBHOST=127.0.0.1
2.修改配置文件opensips.cfg
修改Debug和错误日志输出配置项：
cd /home/work/opensips/etc/opensips/
vi opensips.cfg
A. 取消不必要项的注释（将默认的注释放开）
debug=6
log_stderror=yes
….
loadmodule "db_mysql.so"
loadmodule "auth.so"
loadmodule "auth_db.so"
B. 修改db_default_url：
db_default_url="mysql://opensips:opensipsrw@127.0.0.1:3306/opensips"
3.不吻合修正：（个性配置和默认配置不一致）
如：Mysql服务提供的服务端口不是默认的3306，则需要修改相应的数据库访问命令配置，否则容易出现连错数据库资源或者连接异常等情况。
A.修改opensipsdbctl.mysql
vilib64/opensips/opensipsctl/opensipsdbctl.mysql
修改以下这行内容，增加端口参数（如8888）：
CMD="mysql -h $DBHOST -u$DBROOTUSER–P8888"
同时也修改opensipsctl.mysql文件，以便于后续可以添加用户：
vilib64/opensips/opensipsctl/opensipsctl.mysql
修改以下行内容，增加端口参数（如8888）：
$MYSQL $2 -h $DBHOST –P8888 -u $DBRWUSER"-p$DBRWPW" -e "$1 ;" $DBNAME
6.初始化opensips的mysql数据库：
cd /home/work/opensips/sbin/
./opensipsdbctl create
7. 至此，opensips的安装配置完成了，现在可以启动了：
cd /home/work/opensips/sbin/
A. 开启opensips服务：
./ opensipsctl start  
B. 停止opensips服务：
./ opensipsctl stop
C. 重启opensips服务：
./ opensipsctl restart
D. 检查服务端口：
E. 检查进程状态：
## 2.4.          如何使用OpenSIPS：
1. 下载网络软电话，如：linphone，express talk softphone。
2. 本次为了测试opensips可用性，下载了上述两个softphone，如下图所示。
3. 将两个客户端的sip代理服务器配置成我们搭建好的opensips服务器（113.57.248.49），设置方式如下图所示：
4. 没有权限认证的情况下，设定好以后，双方即可相互拨号通话：
A. 5001（ExpressTalk）向1001（Linphone）拨号，下图表示正在响铃：
B. 拨通后相互通话：
C. 通话挂断
5.追加权限认证的情况下，客户端需要权限认证、注册成功以后才可以正常使用。
A. 权限认证，需要服务器端配置，配置方式如下（将默认注释掉的部分放开）：
cd /home/work/opensips/etc/opensips
if (is_method("REGISTER"))
{
# authenticate the REGISTER requests (uncomment to enable auth)
if (!www_authorize("", "subscriber"))
{
www_challenge("", "0");
exit;
}
##
if (!db_check_to())
{
sl_send_reply("403","Forbidden auth ID");
exit;
}
if (!save("location"))
sl_reply_error();
exit;
}
上面的代码是利用mysql来进行认证的。如果认证不成功，就直接退出，不进行下一步的操作。默认这部分是注释掉的，这样无论是谁都可以注册成功。
注册成功后，会调用
if (!save("location"))
    sl_reply_error();
如果你配置了相对应的数据库配置项，这里实际上是把一些注册时候的sip信息写到opensips数据库的location数据表中。数据库配置项如下：
# ----- usrloc params -----
#modparam("usrloc","db_mode",  
0)
/* uncomment the following lines if youwant to enable DB persistency
for location entries */
modparam("usrloc","db_mode",  
2)
modparam("usrloc","db_url",
"mysql://opensips:opensipsrw@127.0.0.1/opensips")
这样的话会在opensips数据库中的location表中看到这些sip信息如下图所示。
B. 用户的添加和删除：
cd /home/work/opensips/sbin
添加：（add<username> <password>）
./opensipsctl add 5001 5001
删除：
./opensipsctl rm 5001
C. 权限验证不通过的用户，注册失败，如图：
更具体的opensips配置和客户端配置请参考官方文档说明
附上两个注册、拨号通话挂断的分析日志：
