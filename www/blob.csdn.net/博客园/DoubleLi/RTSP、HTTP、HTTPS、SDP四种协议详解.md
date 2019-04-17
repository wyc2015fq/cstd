# RTSP、HTTP、HTTPS、SDP四种协议详解 - DoubleLi - 博客园






我们将主要讲解RTSP，HTTP,HTTPS, SDP四种协议。

** 一：RTSP协议简介**
　　实时流协议RTSP是一个应用层协议，用于控制具有实时特性的数据（例如[多媒体](http://www.c114.net/keyword/%B6%E0%C3%BD%CC%E5)流）的传送。

    RTSP协议一般与RTP/RTCP和RSVP等底层协议一起协同工作，提供基于Internet的整套的流服务。它可以选择发送通道（例如：UDP、组播UDP和TCP）和基于RTP的发送机制。它可以应用于组播和点播。RTP, RTCP,RSVP 定义如下：

　　1. 实时传输协议RTP(Real-time Transport protocol)

　　2. 实时传输控制协议RTCP(Real-time Transport Control protocol)

　　3. 实时流协议RTSP(Real Time Streaming protocol)

　　4. 资源预留协议RSVP(Resource Reserve Protocol)

RTSP协议机理：

   客户机在向视频服务器请求视频服务之前，首先通过HTTP协议从Web服务器获取所请求视频服务的演示描述（Presentation description ）文件，在RTSP中，每个演示（Presentation）及其所对应的媒体流都由一个RTSP URL标识。整个演示及媒体特性都在一个演示描述（Presentation description ）文件中定义，该文件可能包括媒体编码方式、语言、RTSP URLs、目标地址、端口及其它参数。用户在向服务器请求某个连续媒体流的服务之前，必须首先从服务器获得该媒体流的演示描述（Presentation description ）文件以得到必需的参数，演示描述文件的获取可采用HTTP、email或其他方法。利用该文件提供的信息定位视频服务地址（包括视频服务器地址和端口号）及视频服务的编码方式等信息。然后客户机根据上述信息向视频服务器请求视频服务。视频服务初始化完毕，视频服务器为该客户建立一个新的视频服务流，客户端与服务器运行实时流控制协议RTSP，以对该流进行各种VCR控制信号的交换，如播放（PLAY）、停止（PAUSE）、快进、快退等。当服务完毕，客户端提出拆线（TEARDOWN）请求。服务器使用RTP/UDP协议将媒体数据传输给客户端，一旦数据抵达客户端，客户端应用程序即可播放输出。在流式传输中，使用RTP/RTCP/UDP和RTSP/TCP两种不同的通信协议在客户端和服务器间建立联系。如下图：

![](http://img.my.csdn.net/uploads/201209/19/1348038661_5384.jpg)
     RTSP中的所有的操作都是通过服务器和客户方的消息应答来完成的，其消息包括请求（Request）和响应（Response）两种，RTSP正是通过服务器和客户端的消息应答来完成媒体流的创建、初始化（SETUP）、VCR控制（PLAY、PAUSE）以及拆线（TEARDOWN）等操作的。如下图：

![](http://img.my.csdn.net/uploads/201209/19/1348038646_9359.jpg)

RSTP 一些基本方法及用途：

**OPTIONS**  获得有效方法

**SETUP**    建立传输

**ANNOUNCE **改变媒体文件的类型

**DESCRIBE** 获得媒体文件的类型

**PLAY**     播放

**RECORD **  刻录

**REDIRECT**  转换客户端到新的服务器

**PAUSE    ** 暂停

**SET PARAMETER** 设置设备，编码等参数

**TEARDOWN ** 移除状态



完整的播放过程：

GET 过程：

C->W: GET /twister.sdp HTTP/1.1

Host: www.example.com

Accept: application/sdp

W->C: HTTP/1.0 200 OK

Content-Type: application/sdp

v=0

o=- 2890844526 2890842807 IN IP4 192.16.24.202

s=RTSP Session

m=audio 0 RTP/AVP 0

a=control:rtsp://audio.com/twister/audio.en

m=video 0 RTP/AVP 31

a=control:rtsp://video.com/twister/video

SETUP过程：

C->A（audio）: SETUP rtsp://audio.com/twister/audio.en RTSP/1.0

CSeq: 1

Transport: RTP/AVP/UDP;unicast

;client_port=3056-3057

A->C: RTSP/1.0 200 OK

CSeq: 1

Session: 12345678

Transport: RTP/AVP/UDP;unicast

;client_port=3056-3057;

;server_port=5000-5001

C->V（video）: SETUP rtsp://video.com/twister/video RTSP/1.0

CSeq: 1

Transport: RTP/AVP/UDP;unicast

;client_port=3058-3059



V->C: RTSP/1.0 200 OK

CSeq: 1

Session: 23456789

Transport: RTP/AVP/UDP;unicast

;client_port=3058-3059

;server_port=5002-5003



PLAY 过程：

C->V: PLAY rtsp://video.com/twister/video RTSP/1.0

CSeq: 2

Session: 23456789

Range: smpte=0:10:00-

V->C: RTSP/1.0 200 OK

CSeq: 2

Session: 23456789

Range: smpte=0:10:00-0:20:00

RTP-Info: url=rtsp://video.com/twister/video

;seq=12312232;rtptime=78712811

C->A: PLAY rtsp://audio.com/twister/audio.en RTSP/1.0

CSeq: 2

Session: 12345678

Range: smpte=0:10:00-



A->C: RTSP/1.0 200 OK

CSeq: 2

Session: 12345678

Range: smpte=0:10:00-0:20:00

RTP-Info: url=rtsp://audio.com/twister/audio.en

;seq=876655;rtptime=1032181

close 过程：

C->A: TEARDOWN rtsp://audio.com/twister/audio.en RTSP/1.0

CSeq: 3

Session: 12345678

A->C: RTSP/1.0 200 OK

CSeq: 3

C->V: TEARDOWN rtsp://video.com/twister/video RTSP/1.0

CSeq: 3

Session: 23456789

V->C: RTSP/1.0 200 OK

CSeq: 3




关于RTSP的一些时间概念：



**normal play time** (NPT): seconds, microseconds

**MPTE timestamps** (seconds, frames)

**absolute time** (for live events)


**二　HTTP协议简介**

　　HTTP是一个属于应用层的面向对象的协议，由于其简捷、快速的方式，适用于分布式超媒体信息系统。它于1990年提出，经过几年的使用与发展，得到不断地完善和扩展。目前在WWW中使用的是HTTP/1.0的第六版，HTTP/1.1的规范化工作正在进行之中，而且HTTP-NG(Next Generation of HTTP)的建议已经提出。

1：**HTTP协议的主要特点可概括如下：**

　　1.支持客户/服务器模式。

　　2.简单快速：客户向服务器请求服务时，只需传送请求方法和路径。请求方法常用的有GET、HEAD、POST。每种方法规定了客户与服务器联系的类型不同。

　　由于HTTP协议简单，使得HTTP服务器的程序规模小，因而通信速度很快。

　　3.灵活：HTTP允许传输任意类型的数据对象。正在传输的类型由Content-Type加以标记。

　　4.无连接：无连接的含义是限制每次连接只处理一个请求。服务器处理完客户的请求，并收到客户的应答后，即断开连接。采用这种方式可以节省传输时间。

　　5.无状态：HTTP协议是无状态协议。无状态是指协议对于事务处理没有记忆能力。缺少状态意味着如果后续处理需要前面的信息，则它必须重传，这样可能导致每次连接传送的数据量增大。另一方面，在服务器不需要先前信息时它的应答就较快。



2：**HTTP协议的几个重要概念**

　　1.连接(Connection)：一个传输层的实际环流，它是建立在两个相互通讯的应用程序之间。

　　2.消息(Message)：HTTP通讯的基本单位，包括一个结构化的八元组序列并通过连接传输。

　　3.请求(Request)：一个从客户端到服务器的请求信息包括应用于资源的方法、资源的标识符和协议的版本号

　　4.响应(Response)：一个从服务器返回的信息包括HTTP协议的版本号、请求的状态(例如“成功”或“没找到”)和文档的MIME类型。

　　5.资源(Resource)：由URI标识的网络数据对象或服务。

　　6.实体(Entity)：数据资源或来自服务资源的回映的一种特殊表示方法，它可能被包围在一个请求或响应信息中。一个实体包括实体头信息和实体的本身内容。

　　7.客户机(Client)：一个为发送请求目的而建立连接的应用程序。

　　8.用户代理(User agent)：初始化一个请求的客户机。它们是浏览器、编辑器或其它用户工具。

　　9.服务器(Server)：一个接受连接并对请求返回信息的应用程序。

　　10.源服务器(Origin server)：是一个给定资源可以在其上驻留或被创建的服务器。

　　11.代理(Proxy)：一个中间程序，它可以充当一个服务器，也可以充当一个客户机，为其它客户机建立请求。请求是通过可能的翻译在内部或经过传递到其它的服务器中。一个代理在发送请求信息之前，必须解释并且如果可能重写它。

　　代理经常作为通过防火墙的客户机端的门户，代理还可以作为一个帮助应用来通过协议处理没有被用户代理完成的请求。

　　12.[网关](http://www.c114.net/keyword/%CD%F8%B9%D8)(Gateway)：一个作为其它服务器中间媒介的服务器。与代理不同的是，网关接受请求就好象对被请求的资源来说它就是源服务器；发出请求的客户机并没有意识到它在同网关打交道。
　　网关经常作为通过防火墙的服务器端的门户，网关还可以作为一个协议翻译器以便存取那些存储在非HTTP系统中的资源。

　　13.通道(Tunnel)：是作为两个连接中继的中介程序。一旦激活，通道便被认为不属于HTTP通讯，尽管通道可能是被一个HTTP请求初始化的。当被中继的连接两端关闭时，通道便消失。当一个门户(Portal)必须存在或中介(Intermediary)不能解释中继的通讯时通道被经常使用。

　　14.缓存([Cache](http://www.c114.net/keyword/Cache))：反应信息的局域存储。

**3：建立连接的方式**

HTTP支持2中建立连接的方式：非持久连接和持久连接(HTTP1.1默认的连接方式为持久连接)。

1) 非持久连接

让我们查看一下非持久连接情况下从服务器到客户传送一个Web页面的步骤。假设该贝面由1个基本HTML文件和10个JPEG图像构成，而且所有这些对象都存放在同一台服务器主机中。再假设该基本HTML文件的URL为：gpcuster.cnblogs.com/index.html。

下面是具体步骡:

1.HTTP客户初始化一个与服务器主机gpcuster.cnblogs.com中的HTTP服务器的TCP连接。HTTP服务器使用默认端口号80监听来自HTTP客户的连接建立请求。

2.HTTP客户经由与TCP连接相关联的本地套接字发出—个HTTP请求消息。这个消息中包含路径名/somepath/index.html。

3.HTTP服务器经由与TCP连接相关联的本地套接字接收这个请求消息，再从服务器主机的内存或硬盘中取出对象/somepath/index.html，经由同一个套接字发出包含该对象的响应消息。

4.HTTP服务器告知TCP关闭这个TCP连接(不过TCP要到客户收到刚才这个响应消息之后才会真正终止这个连接)。

5.HTTP客户经由同一个套接字接收这个响应消息。TCP连接随后终止。该消息标明所封装的对象是一个HTML文件。客户从中取出这个文件，加以分析后发现其中有10个JPEG对象的引用。

6.给每一个引用到的JPEG对象重复步骡1-4。

上述步骤之所以称为使用非持久连接，原因是每次服务器发出一个对象后，相应的TCP连接就被关闭，也就是说每个连接都没有持续到可用于传送其他对象。每个TCP连接只用于传输一个请求消息和一个响应消息。就上述例子而言，用户每请求一次那个web页面，就产生11个TCP连接。

2) 持久连接

非持久连接有些缺点。首先，客户得为每个待请求的对象建立并维护一个新的连接。对于每个这样的连接，TCP得在客户端和服务器端分配TCP缓冲区，并维持TCP变量。对于有可能同时为来自数百个不同客户的请求提供服务的web服务器来说，这会严重增加其负担。其次，如前所述，每个对象都有2个RTT的响应延长——一个RTT用于建立TCP连接，另—个RTT用于请求和接收对象。最后，每个对象都遭受TCP缓启动，因为每个TCP连接都起始于缓启动阶段。不过并行TCP连接的使用能够部分减轻RTT延迟和缓启动延迟的影响。

在持久连接情况下，服务器在发出响应后让TCP连接继续打开着。同一对客户/服务器之间的后续请求和响应可以通过这个连接发送。整个Web页面(上例中为包含一个基本HTMLL文件和10个图像的页面)自不用说可以通过单个持久TCP连接发送:甚至存放在同一个服务器中的多个web页面也可以通过单个持久TCP连接发送。通常，HTTP服务器在某个连接闲置一段特定时间后关闭它，而这段时间通常是可以配置的。持久连接分为不带流水线(without pipelining)和带流水线(with pipelining)两个版本。如果是不带流水线的版本，那么客户只在收到前一个请求的响应后才发出新的请求。这种情况下，web页面所引用的每个对象(上例中的10个图像)都经历1个RTT的延迟，用于请求和接收该对象。与非持久连接2个RTT的延迟相比，不带流水线的持久连接已有所改善，不过带流水线的持久连接还能进一步降低响应延迟。不带流水线版本的另一个缺点是，服务器送出一个对象后开始等待下一个请求，而这个新请求却不能马上到达。这段时间服务器资源便闲置了。

HTTP/1.1的默认模式使用带流水线的持久连接。这种情况下，HTTP客户每碰到一个引用就立即发出一个请求，因而HTTP客户可以一个接一个紧挨着发出各个引用对象的请求。服务器收到这些请求后，也可以一个接一个紧挨着发出各个对象。如果所有的请求和响应都是紧挨着发送的，那么所有引用到的对象一共只经历1个RTT的延迟(而不是像不带流水线的版本那样，每个引用到的对象都各有1个RTT的延迟)。另外，带流水线的持久连接中服务器空等请求的时间比较少。与非持久连接相比，持久连接(不论是否带流水线)除降低了1个RTT的响应延迟外，缓启动延迟也比较小。其原因在于既然各个对象使用同一个TCP连接，服务器发出第一个对象后就不必再以一开始的缓慢速率发送后续对象。相反，服务器可以按照第一个对象发送完毕时的速率开始发送下一个对象。

**4： 缓存的机制**

HTTP/1.1中缓存的目的是为了在很多情况下减少发送请求，同时在许多情况下可以不需要发送完整响应。前者减少了网络回路的数量；HTTP利用一个“过期（expiration）”机制来为此目的。后者减少了网络应用的带宽；HTTP用“验证（validation）”机制来为此目的。具体可以参考：

[http://www.chedong.com/tech/cache_docs.html](http://www.chedong.com/tech/cache_docs.html)





**三 RTSP协议与HTTP协议的联系与区别**

      RTSP协议负责在服务器和客户端之间建立并控制一个或多个时间上同步的连续流媒体，其目标是象HTTP协议为用户提供文字和图形服务那样为用户提供连续媒体服务。因此，RTSP协议的设计在语法和操作上与HTTP协议很相似，这样，对于HTTP的大部分扩展也适用于RTSP。
　　但是RTSP协议和HTTP协议在很多方面有着区别：
　　1. HTTP是一个无状态协议，而RTSP协议是有状态的。
　　2. HTTP本质上是一个非对称协议，客户端提出请求而服务器响应；而RTSP是对称的，服务器和客户端都可发送和响应请求。



**四  HTTPS传输协议**

    HTTPS（Secure Hypertext Transfer Protocol）安全超文本传输协议，它是一个安全通信通道，它基于HTTP开发，用于在客户计算机和服务器之间交换信息。它使用安全套接字层(SSL)进行信息交换，简单来说它是HTTP的安全版。
它是由Netscape开发并内置于其浏览器中，用于对数据进行压缩和解压操作，并返回网络上传送回的结果。HTTPS实际上应用了Netscape的安全全套接字层（SSL）作为HTTP应用层的子层。（HTTPS使用端口443，而不是象HTTP那样使用端口80来和TCP/IP进行通信。）SSL使用40 位关键字作为RC4流加密算法，这对于商业信息的加密是合适的。HTTPS和SSL支持使用X.509数字认证，如果需要的话用户可以确认发送者是谁。

HTTPS和HTTP的区别：

1：http和https使用的是完全不同的连接方式,用的端口也不一样,前者是80,后者是443。
2：https协议需要到ca申请证书，一般免费证书很少，需要交费。
3：http是超文本传输协议，信息是明文传输，https 则是具有安全性的ssl加密传输协议
4：http的连接很简单,是无状态的，而HTTPS协议是由SSL+HTTP协议构建的可进行加密传输、身份认证的网络协议，要比http协议安全

HTTPS解决的问题：
1 . 信任主机的问题. 采用https 的server 必须从CA 申请一个用于证明服务器用途类型的证书. 改证书只有用于对应的server 的时候,客户度才信任次主机. 所以目前所有的银行系统网站,关键部分应用都是https 的. 客户通过信任该证书,从而信任了该主机. 其实这样做效率很低,但是银行更侧重安全. 这一点对我们没有任何意义,我们的server ,采用的证书不管自己issue 还是从公众的地方issue, 客户端都是自己人,所以我们也就肯定信任该server.
2 . 通讯过程中的数据的泄密和被窜改
1. 一般意义上的https, 就是 server 有一个证书.
a) 主要目的是保证server 就是他声称的server. 这个跟第一点一样.
b) 服务端和客户端之间的所有通讯,都是加密的.
i. 具体讲,是客户端产生一个对称的密钥,通过server 的证书来交换密钥. 一般意义上的握手过程.
ii. 加下来所有的信息往来就都是加密的. 第三方即使截获,也没有任何意义.因为他没有密钥. 当然窜改也就没有什么意义了.
2. 少许对客户端有要求的情况下,会要求客户端也必须有一个证书.
a) 这里客户端证书,其实就类似表示个人信息的时候,除了用户名/密码, 还有一个CA 认证过的身份. 应为个人证书一般来说上别人无法模拟的,所有这样能够更深的确认自己的身份.
b) 目前少数个人银行的专业版是这种做法,具体证书可能是拿U盘作为一个备份的载体.
HTTPS 一定是繁琐的.
a) 本来简单的http协议,一个get一个response. 由于https 要还密钥和确认加密算法的需要.单握手就需要6/7 个往返.
i. 任何应用中,过多的round trip 肯定影响性能.
b) 接下来才是具体的http协议,每一次响应或者请求, 都要求客户端和服务端对会话的内容做加密/解密.
i. 尽管对称加密/解密效率比较高,可是仍然要消耗过多的CPU,为此有专门的SSL 芯片. 如果CPU 信能比较低的话,肯定会降低性能,从而不能serve 更多的请求.
ii. 加密后数据量的影响. 所以，才会出现那么多的安全认证提示。



**五 SDP协议**

 SDP会话描述协议：为会话通知、会话邀请和其它形式的多媒体会话初始化等目的提供了多媒体会话描述。会话目录用于协助多媒体会议的通告，并为会话参与者传送相关设置信息。 SDP 即用于将这种信息传输到接收端。 SDP 完全是一种会话描述格式――它不属于传输协议 ――它只使用不同的适当的传输协议，包括会话通知协议 （SAP） 、会话初始协议（SIP）、实时流协议 （RTSP）、 MIME 扩展协议的电子邮件以及超文本传输协议 （HTTP）。SDP 的设计宗旨是通用性，它可以应用于大范围的网络环境和应用程序，而不仅仅局限于组播会话目录。

SDP是会话描述协议的缩写，是描述流媒体初始化参数的格式，由IETF作为RFC 4566颁布。流媒体是指在传输过程中看到或听到的内容，SDP包通常包括以下信息：

（1）会话信息· 会话名和目的

      　　   · 会话活动时间

      　　      由于参与会话的资源是受限制的，因此包括以下附加信息是非常有用的

      　　   · 会话使用的带宽信息

      　　   · 会话负责人的联系信息

（2）媒体信息

      　　   · 媒体类型，例如视频和音频

      　　   · 传输协议，例如RTP/UDP/IP和H.320。

             · 多播地址和媒体传输端口（IP多播会话）

      　　   · 用于联系地址的媒体和传输端口的远端地址（IP单播会话）

SDP描述由许多文本行组成，文本行的格式为<类型>=<值>，<类型>是一个字母，<值>是结构化的文本串，其格式依<类型>而定。

SDP格式（带*为可选）:

        Session description

          v=   (protocol version) //该行指示协议的版本

          o=   (owner/creator and session identifier)

例如：**    o=mhandley 2890844526 2890842807 IN IP4 126.16.64.4   //o行中包含与会话所有者有关的参数（1：**第一个参数表明会话发起者的名称，该参数可不填写，如填写和SIP消息中，from消息头的内容一致:2：第二个参数为主叫方的会话标识符:3：第三个参数为主叫方会话的版本，会话数据有改变时，版本号递增:4：第四个参数定义了网络类型，IN表示Internet网络类型，目前仅定义该网络类型:5：第五个参数为地址类型，目前支持IPV4和IPV6两种地址类型:6：第六个参数为地址：表明会话发起者的IP地址，该地址为信令面的IP地址，信令PDP激活时为手机分配。）

          s=   (session name) //表明本次会话的标题，或会话的名称

          i=* (session information)

          u=* (URI of description)

          e=* (email address)

          p=* (phone number)

          c=* (connection information - not required if included in all media)

          b=* (zero or more bandwidth information lines)

          One or more time descriptions ("t=" and "r=" lines, see below)

          z=* (time zone adjustments)

          k=* (encryption key)

          a=* (zero or more session attribute lines)

          Zero or more media descriptions

       Time description

          t=   (time the session is active)

          r=* (zero or more repeat times)

       Media description, if present

          m=   (media name and transport address)

**    例如： m=audio 3458  RTP/AVP  0   96   97   // m行又称媒体行，描述了发送方所支持的媒体类型等信息**（1： 第一个参数为媒体名称：表明支持音频类型。2： 第二个参数为端口号，表明UE在本地端口为3458上发送音频流。3： 第三个参数为传输协议，一般为RTP/AVP协议。4：四-七参数为所支持的四种净荷类型编号）

**m=video 3400 RTP/AVP 98  99 //m行又称媒体行，描述了发送方所支持的媒体类型等信息**

          i=* (media title)

          c=* (connection information - optional if included at

               session-level)

          b=* (zero or more bandwidth information lines)

          k=* (encryption key)

          a=* (zero or more media attribute lines)









