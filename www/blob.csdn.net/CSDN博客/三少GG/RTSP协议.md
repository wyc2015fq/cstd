# RTSP协议 - 三少GG - CSDN博客
2012年03月26日 12:30:41[三少GG](https://me.csdn.net/scut1135)阅读数：2780标签：[服务器																[tcp																[session																[扩展																[网络																[防火墙](https://so.csdn.net/so/search/s.do?q=防火墙&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=扩展&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
[http://shinfocom.iteye.com/blog/1231501](http://shinfocom.iteye.com/blog/1231501)
RTSP是由Real network 和Netscape共同提出的如何有效地在IP网络上传输流媒体数据的应用层协议。实时流协议（RTSP）建立并控制一个或几个时间同步的连续流媒体，如音频和视频。尽管连续媒体流与控制流交叉是可能的，RTSP 本身并不发送连续媒体流。换言之，RTSP 充当多媒体服务器的网络远程控制。RTSP 提供了一个可扩展框架，实现实时数据（如音频与视频）的受控、按需传送。数据源包括实况数据与存储的剪辑。RTSP 用于控制多个数据发送会话，提供了选择发送通道（如 UDP、组播 UDP 与 TCP
 等）的方式，并提供了选择基于 RTP 的发送机制的方法。
目前还没有 RTSP 连接的概念；服务器维护由识别符标识的会话。RTSP 会话不会绑定到传输层连接，如 TCP。在 RTSP 会话期间，RTSP 客户端可打开或关闭多个对服务器的可靠传输连接以发出 RTSP 请求。它也可选择使用无连接传输协议，如 UDP。
RTSP 控制的流可能用到 RTP，但 RTSP 操作并不依赖用于传输连续媒体的传输机制。RTSP 在语法和操作上与 HTTP/1.1 类似，因此 HTTP 的扩展机制在多数情况下可加入 RTSP。然而，在很多重要方面 RTSP 仍不同于 HTTP ：
- 
RTSP 引入了大量新方法并具有一个不同的协议标识符：
- 
在大多数情况下，RTSP 服务器需要保持缺省状态，与 HTTP 的无状态相对；
- 
RTSP 中客户端和服务器都可以发出请求；
- 
在多数情况下，数据由不同的协议传输；
- 
RTSP 使用 ISO 10646 （UTF-8）而并非 ISO 8859-1，与当前的国际标准 HTML 相一致；
- 
URI 请求总是包含绝对 URI。为了与过去的错误相互兼容，HTTP/1.1 只在请求过程中传送绝对路径并将主机名置于另外的头字段。
该协议支持如下操作：
- 
从媒体服务器上检索媒体：用户可通过 HTTP 或其它方法提交一个演示描述请求；
- 
媒体服务器邀请进入会议： 媒体服务器可被邀请参加正进行的会议，或回放媒体，或记录部分或全部演示；
- 
将新媒体加到现有演示中：如服务器能告诉客户端接下来可用的媒体内容，对现场直播显得尤其有用。
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - --
 - - - - - - - - -
|![](http://www.networkdictionary.cn/images/arrow.gif)|协议结构|
RTSP 是一种文本协议，采用 UTF-8 编码中的 ISO 10646 字符集。一行可通过 CRLF 终止，但接收端需要做好解释 CR 和 LF 作为一行终止符的准备。关于头字段概述如下：
|**Header**|**Type**|**Support**|**Methods**|
|----|----|----|----|
|Accept|R|opt.|entity|
|Accept-Encoding|R|opt.|entity|
|Accept-Language|R|opt.|all|
|Allow|R|opt.|all|
|Authorization|R|opt.|all|
|Bandwidth|R|opt.|all|
|Blocksize|R|opt.|All but OPTIONS, TEARDOWN|
|Cache-Control|G|opt.|SETUP|
|Conference|R|opt.|SETUP|
|Connection|G|req.|all|
|Content-Base|E|opt.|entity|
|Content-Encoding|E|req.|SET_PARAMETER|
|Content-Encoding|E|req.|DESCRIBE, ANNOUNCE|
|Content-Language|E|req.|DESCRIBE, ANNOUNCE|
|Content-Length|E|req.|SET_PARAMETER, ANNOUNCE|
|Content-Length|E|req.|entity|
|Content-Location|E|opt.|entity|
|Content-Type|E|req.|SET_PARAMETER, ANNOUNCE|
|Content-Type|R|req.|entity|
|CSeq|G|req.|all|
|Date|G|opt.|all|
|Expires|E|opt.|DESCRIBE, ANNOUNCE|
|From|R|opt.|all|
|If-Modified-Since|R|opt.|DESCRIBE, SETUP|
|Last-Modified|E|opt.|entity|
|Proxy-Authenticate||||
|Proxy-Require|R|req.|all|
|Public|R|opt.|all|
|Range|R|opt.|PLAY, PAUSE, RECORD|
|Range|R|opt.|PLAY, PAUSE, RECORD|
|Referer|R|opt.|all|
|Require|R|req.|all|
|Retry-After|R|opt.|all|
|RTP-Info|R|req.|PLAY|
|Scale|Rr|opt.|PLAY, RECORD|
|Session|Rr|req.|All but SETUP, OPTIONS|
|Server|R|opt.|all|
|Speed|Rr|opt.|PLAY|
|Transport|Rr|req.|SETUP|
|Unsupported|R|req.|all|
|User-Agent|R|opt.|all|
|Via|G|opt.|all|
|WWW-Authenticate|R|opt.|all|
类型 "g" 表示请求和响应中的通用请求头；类型 "R" 表示请求头；类型 "r" 表示响应头；类型 "e" 表示实体头字段。在 "support" 一栏中 标有 "req." 的字段 必须由接收者以特殊的方法实现；而 "opt." 的字段是可选的。注意，不是所有 "req." 字段在该类型的每个请求中都会被发送。 "req." 只表示客户机（支持响应头）和服务器（支持请求头）必须执行该字段。最后一栏列出了关于头字段产生作用的方法；其中 "entity" 针对于返回一个信息主体的所有方法。
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - -
RTSP消息格式:
RTSP的消息有两大类 ---请求消息(request),回应消息(response)。
请求消息：
方法 URI RTSP版本 CR LF
消息头 CR LF CR LF
消息体 CR LF
其中方法包括OPTION回应中所有的命令,URI是接受方的地址,例如:rtsp://192.168.20.136。RTSP版本一般都是 RTSP/1.0。每行后面的CR LF表示回车换行，需要接受端有相应的解析，最后一个消息头需要有两个CR LF
回应消息：
RTSP版本 状态码 解释 CR LF
消息头 CR LF CR LF
消息体 CR LF
其中RTSP版本一般都是RTSP/1.0, 状态码是一个数值, 200表示成功, 解释是与状态码对应的文本解释.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - -
简单的rtsp交互过程:
C表示rtsp客户端, S表示rtsp服务端
1.C->S:OPTION request//询问S有哪些方法可用
1.S->C:OPTION response//S回应信息中包括提供的所有可用方法
2.C->S:DESCRIBE request//要求得到S提供的媒体初始化描述信息
2.S->C:DESCRIBE response//S回应媒体初始化描述信息，主要是sdp
3.C->S:SETUP request//设置会话的属性，以及传输模式，提醒S建立会话
3.S->C:SETUP response//S建立会话，返回会话标识符，以及会话相关信息
4.C->S:PLAY request//C请求播放
4.S->C:PLAY response//S回应该请求的信息
S->C:发送流媒体数据
5.C->S:TEARDOWN request//C请求关闭会话
5.S->C:TEARDOWN response//S回应该请求
上述的过程是标准的、友好的rtsp流程，但实际的需求中并不一定按部就班来。 其 中第3和4步是必需的！第一步，只要服务器客户端约定好，有哪些方法可用，则option请求可以不要。第二步，如果我们有其他途径得到媒体初始化描述信 息（比如http请求等等），则我们也不需要通过rtsp中的describe请求来完成。第五步，可以根据系统需求的设计来决定是否需要。
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - -
rtsp中常用方法:
1.OPTION
目的是得到服务器提供的可用方法:
OPTIONS rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 1//每个消息都有序号来标记，第一个包通常是option请求消息
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器的回应信息包括提供的一些方法,例如:
RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 1//每个回应消息的cseq数值和请求消息的cseq相对应
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SCALE,GET_PARAMETER//服务器提供的可用的方法
2. DESCRIBE
C向S发起DESCRIBE请求,为了得到会话描述信息(SDP):
DESCRIBE rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 2
token:
Accept: application/sdp
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器回应一些对此会话的描述信息(sdp):
RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 2
x-prev-url: rtsp://192.168.20.136:5000
x-next-url: rtsp://192.168.20.136:5000
x-Accept-Retransmit: our-retransmit
x-Accept-Dynamic-Rate: 1
Cache-Control: must-revalidate
Last-Modified: Fri, 10 Nov 2006 12:34:38 GMT
Date: Fri, 10 Nov 2006 12:34:38 GMT
Expires: Fri, 10 Nov 2006 12:34:38 GMT
Content-Base: rtsp://192.168.20.136:5000/xxx666/
Content-Length: 344
Content-Type: application/sdp
v=0//以下都是sdp信息
o=OnewaveUServerNG 1451516402 1025358037 IN IP4 192.168.20.136
s=/xxx666
u=http:///
e=admin@
c=IN IP4 0.0.0.0
t=0 0
a=isma-compliance:1,1.0,1
a=range:npt=0-
m=video 0 RTP/AVP 96//m表示媒体描述，下面是对会话中视频通道的媒体描述
a=rtpmap:96 MP4V-ES/90000
a=fmtp:96 profile-level-id=245;config=000001B0F5000001B509000001000000012000C888B0E0E0FA62D089028307 a=control:trackID=0//trackID＝0表示视频流用的是通道0
3.SETUP
客户端提醒服务器建立会话,并确定传输模式:
SETUP rtsp://192.168.20.136:5000/xxx666/trackID=0 RTSP/1.0
CSeq: 3
Transport: RTP/AVP/TCP;unicast;interleaved=0-1
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
//uri中 带有trackID＝0，表示对该通道进行设置。Transport参数设置了传输模式，包的结构。接下来的数据包头部第二个字节位置就是 interleaved，它的值是每个通道都不同的，trackID＝0的interleaved值有两个0或1，0表示rtp包，1表示rtcp包，接 受端根据interleaved的值来区别是哪种数据包。
服务器回应信息:
RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 3
Session: 6310936469860791894//服务器回应的会话标识符
Cache-Control: no-cache
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=6B8B4567
4.PLAY
客户端发送播放请求:
PLAY rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 4
Session: 6310936469860791894
Range: npt=0.000-//设置播放时间的范围
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器回应信息:
RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 4
Session: 6310936469860791894
Range: npt=0.000000-
RTP-Info: url=trackID=0;seq=17040;rtptime=1467265309
//seq和rtptime都是rtp包中的信息
5.TEARDOWN
客户端发起关闭请求:
TEARDOWN rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 5
Session: 6310936469860791894
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
服务器回应:
RTSP/1.0 200 OK
Server: UServer 0.9.7_rc1
Cseq: 5
Session: 6310936469860791894
Connection: Close
以上方法都是交互过程中最为常用的, 其它还有一些重要的方法如get/set_parameter,pause,redirect等等
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -- - - - - - - - - -
sdp的格式：
v=<version>
o=<username> <session id> <version> <network type> <address type> <address>
s=<session name>
i=<session description>
u=<URI>
e=<email address>
p=<phone number>
c=<network type> <address type> <connection address>
b=<modifier>:<bandwidth-value>
t=<start time> <stop time>
r=<repeat interval> <active duration> <list of offsets from start-time>
z=<adjustment time> <offset> <adjustment time> <offset> ....
k=<method>
k=<method>:<encryption key>
a=<attribute>
a=<attribute>:<value>
m=<media> <port> <transport> <fmt list>
v =（协议版本）
o =（所有者/创建者和会话标识符）
s =（会话名称）
i = *（会话信息）
u = *（URI 描述）
e = *（Email 地址）
p = *（电话号码）
c = *（连接信息）
b = *（带宽信息）
z = *（时间区域调整）
k = *（加密密钥）
a = *（0 个或多个会话属性行）
时间描述：
t =（会话活动时间）
r = *（0或多次重复次数）
媒体描述：
m =（媒体名称和传输地址）
i = *（媒体标题）
c = *（连接信息 — 如果包含在会话层则该字段可选）
b = *（带宽信息）
k = *（加密密钥）
a = *（0 个或多个媒体属性行）
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [RTP/RTCP/RTSP协议初探](http://sabolasi.iteye.com/blog/1254142)
![](http://hi.csdn.net/attachment/201007/4/0_1278249841jy77.gif)
RTP/RTCP,RTSP图例
![](http://hi.csdn.net/attachment/201007/4/0_1278249855vIni.gif)
协议关系图
**一．产生的背景**
随着互连网的发展，人们已经不满足于传统的HTTP，FTP和电子邮件等文本信息和服务，而对内容丰富多彩的多媒体信息，服务以及多媒体通信方式提出了需求，包括声音，图象，图形，视频信息等等，而这些不但传输的数据量大而且对交互性和实时性要求很高。
这时,基于HTTP的TCP协议无法达到要求,故产生RTP协议来进行多媒体数据实时传输.
**二．RTP/RTCP/RTSP协议与TCP/IP协议对比**
那么，现在有个疑问是:为什么TCP/IP协议就不能满足多媒体通信的要求呢？
这是因为TCP有以下4个特点:
1.TCP重传机制
2.TCP拥塞控制机制
3.TCP报文头比UDP报文头要大
4.TCP的启动速度慢
RTP由IETF（Internet Engineering Task Force，互联网工程任务组）的音频/视频传输工作组制定，主要实现实时数据的传输，它在包头中提供编码类型，包中数据的采样时刻和数据包的序号，根据这些信息发送和接受方可以协商编码类型，可以对接收到的数据包进行排序等工作；RTCP主要负责传输质量的监控以及传送发送者的一些标志信息。试验和研究表明，RTP/RTCP所提出的实时数据的传输机制是行之有效的。
**对比记忆**
IP:数据传输
RTP:多媒体数据实时传输
TCP:保证数据传输可靠
RTCP:保证多媒体数据传输的可靠
**三．RTP/RTCP,RTSP协议说明**
**RTP**：
Realtime Transport Potocol 实时传输协议
RTP提供时间标志,序列号以及其他能够保证在实时数据传输时处理时间的方法
[RTP:A Transport Protocol for Real-Time Applications](http://www.rfc-archive.org/getrfc.php?rfc=3550)
**RTCP：**
Realtime Transport Control Potocol 实时传输控制协议
RTCP是RTP的控制部分,是用来保证服务质量和成员管理的
[Symmetric RTP / RTP Control Protocol (RTCP)](http://www.rfc-archive.org/getrfc.php?rfc=4961)
[Framing Real-time Transport Protocol (RTP) and RTP Control Protocol (RTCP) Packets over Connection-Oriented Transport](http://www.rfc-archive.org/getrfc.php?rfc=4571)
**RTSP：**
RealTime Streaming Potocol 实时流协议
RTSP具体数据传输交给RTP,提供对流的远程控制
[Real Time Streaming Protocol (RTSP)](http://www.rfc-archive.org/getrfc.php?rfc=2326)
**RSVP：**
Resource Reserve Potocol 资源预留协议
RSVP预留带宽，提高QoS(Quality of Sever)
[Resource ReSerVation Protocol (RSVP) – Version 1 Functional Specification](http://www.rfc-archive.org/getrfc.php?rfc=2205)
[Resource ReSerVation Protocol (RSVP) – Version 1 Applicability Statement Some Guidelines on Deployment](http://www.rfc-archive.org/getrfc.php?rfc=2208)
**原文：**《[RTP/RTCP/RTSP协议初探](http://blog.ccidnet.com/blog-htm-do-showone-uid-38648-itemid-98289-type-blog.html)》
+++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [RTP/RTSP/RTCP的区别](http://shinfocom.iteye.com/blog/1231496)
用一句简单的话总结：**RTSP发起/终结流媒体、RTP传输流媒体数据 、RTCP对RTP进行控制，同步。**
之所以以前对这几个有点分不清，是因为CTC标准里没有对RTCP进行要求，因此**在标准RTSP的代码中没有看到相关的部分。而在私有RTSP的代码中，有关控制、同步等，是在RTP Header中做扩展定义实现的**。
另外，RFC3550可以看作是RFC1889的升级文档，只看RFC3550即可。
- RTP：实时传输协议（Real-time Transport Protocol）- RTP/RTCP是实际传输数据的协议
- RTP传输音频/视频数据，如果是PLAY，Server发送到Client端，如果是RECORD，可以由Client发送到Server
- 整个RTP协议由两个密切相关的部分组成：RTP数据协议和RTP控制协议（即RTCP）
- RTSP：实时流协议（Real Time Streaming Protocol，RTSP）- RTSP的请求主要有DESCRIBE,SETUP,PLAY,PAUSE,TEARDOWN,OPTIONS等，顾名思义可以知道起对话和控制作用
- RTSP的对话过程中SETUP可以确定RTP/RTCP使用的端口，PLAY/PAUSE/TEARDOWN可以开始或者停止RTP的发送，等等
- RTCP：- RTP/RTCP是实际传输数据的协议
- RTCP包括Sender Report和Receiver Report，用来进行音频/视频的同步以及其他用途，是一种控制协议
以下是每个协议的概要介绍：
**一、RTP数据协议**
RTP数据协议负责对流媒体数据进行封包并实现媒体流的实时传输，每一个RTP数据报都由头部（Header）和负载（Payload）两个部分组成，其中头部前12个字节的含义是固定的，而负载则可以是音频或者视频数据。RTP数据报的头部格式如图1所示：
![](http://www-128.ibm.com/developerworks/cn/linux/l-mdst/image001.jpg)
其中比较重要的几个域及其意义如下：
- 
CSRC记数（CC）：表示CSRC标识的数目。CSRC标识紧跟在RTP固定头部之后，用来表示RTP数据报的来源，RTP协议允许在同一个会话中存在多个数据源，它们可以通过RTP混合器合并为一个数据源。例如，可以产生一个CSRC列表来表示一个电话会议，该会议通过一个RTP混合器将所有讲话者的语音数据组合为一个RTP数据源。
- 
负载类型（PT）：标明RTP负载的格式，包括所采用的编码算法、采样频率、承载通道等。例如，类型2表明该RTP数据包中承载的是用ITU G.721算法编码的语音数据，采样频率为8000Hz，并且采用单声道。
- 
序列号：用来为接收方提供探测数据丢失的方法，但如何处理丢失的数据则是应用程序自己的事情，RTP协议本身并不负责数据的重传。
- 
时间戳：记录了负载中第一个字节的采样时间，接收方能够时间戳能够确定数据的到达是否受到了延迟抖动的影响，但具体如何来补偿延迟抖动则是应用程序自己的事情。
从RTP数据报的格式不难看出，它包含了传输媒体的类型、格式、序列号、时间戳以及是否有附加数据等信息，这些都为实时的流媒体传输提供了相应的基础。RTP协议的目的是提供实时数据（如交互式的音频和视频）的端到端传输服务，因此**在RTP中没有连接的概念**，它可以建立在底层的面向连接或面向非连接的传输协议之上；RTP也不依赖于特别的网络地址格式，而仅仅只需要底层传输协议支持组帧（Framing）和分段（Segmentation）就足够了；另外**RTP本身还不提供任何可靠性机制**，这些都要由传输协议或者应用程序自己来保证。在典型的应用场合下，RTP一般是在传输协议之上作为应用程序的一部分加以实现的，如图2所示：
![](http://www-128.ibm.com/developerworks/cn/linux/l-mdst/image002.jpg)
**二、RTCP控制协议**RTCP控制协议需要与RTP数据协议一起配合使用，当应用程序启动一个RTP会话时将同时占用两个端口，分别供RTP和RTCP使用。**RTP本身并不能为按序传输数据包提供可靠的保证，也不提供流量控制和拥塞控制，这些都由RTCP来负责完成**。通常RTCP会采用与RTP相同的分发机制，向会话中的所有成员周期性地发送控制信息，应用程序通过接收这些数据，从中获取会话参与者的相关资料，以及网络状况、分组丢失概率等反馈信息，从而能够对服务质量进行控制或者对网络状况进行诊断。
RTCP协议的功能是通过不同的RTCP数据报来实现的，主要有如下几种类型：
- 
SR：发送端报告，所谓发送端是指发出RTP数据报的应用程序或者终端，发送端同时也可以是接收端。
- 
RR：接收端报告，所谓接收端是指仅接收但不发送RTP数据报的应用程序或者终端。
- 
SDES：源描述，主要功能是作为会话成员有关标识信息的载体，如用户名、邮件地址、电话号码等，此外还具有向会话成员传达会话控制信息的功能。
- 
BYE：通知离开，主要功能是指示某一个或者几个源不再有效，即通知会话中的其他成员自己将退出会话。
- 
APP：由应用程序自己定义，解决了RTCP的扩展性问题，并且为协议的实现者提供了很大的灵活性。
RTCP数据报携带有服务质量监控的必要信息，能够对服务质量进行动态的调整，并能够对网络拥塞进行有效的控制。由于RTCP数据报采用的是多播方式，因此会话中的所有成员都可以通过RTCP数据报返回的控制信息，来了解其他参与者的当前情况。
在一个典型的应用场合下，发送媒体流的应用程序将周期性地产生发送端报告SR，该RTCP数据报含有不同媒体流间的同步信息，以及已经发送的数据报和字节的计数，接收端根据这些信息可以估计出实际的数据传输速率。另一方面，接收端会向所有已知的发送端发送接收端报告RR，该RTCP数据报含有已接收数据报的最大序列号、丢失的数据报数目、延时抖动和时间戳等重要信息，发送端应用根据这些信息可以估计出往返时延，并且可以根据数据报丢失概率和时延抖动情况动态调整发送速率，以改善网络拥塞状况，或者根据网络状况平滑地调整应用程序的服务质量。
**三、RTSP实时流协议**
作为一个应用层协议，RTSP提供了一个可供扩展的框架，它的意义在于使得实时流媒体数据的受控和点播变得可能。总的说来，RTSP是一个流媒体表示协议，主要用来控制具有实时特性的数据发送，但它本身并不传输数据，而是必须依赖于下层传输协议所提供的某些服务。**RTSP可以对流媒体提供诸如播放、暂停、快进等操作，它负责定义具体的控制消息、操作方法、状态码等，此外还描述了与RTP间的交互操作（RFC2326）**。
**RTSP在制定时较多地参考了HTTP/1.1协议，甚至许多描述与HTTP/1.1完全相同**。RTSP之所以特意使用与HTTP/1.1类似的语法和操作，在很大程度上是为了兼容现有的Web基础结构，正因如此，HTTP/1.1的扩展机制大都可以直接引入到RTSP中。
由RTSP控制的媒体流集合可以用表示描述（Presentation Description）来定义，所谓表示是指流媒体服务器提供给客户机的一个或者多个媒体流的集合，而表示描述则包含了一个表示中各个媒体流的相关信息，如数据编码/解码算法、网络地址、媒体流的内容等。
虽然RTSP服务器同样也使用标识符来区别每一流连接会话（Session），但RTSP连接并没有被绑定到传输层连接（如TCP等），也就是说在整个RTSP连接期间，RTSP用户可打开或者关闭多个对RTSP服务器的可靠传输连接以发出RTSP 请求。此外，RTSP连接也可以基于面向无连接的传输协议（如UDP等）。
RTSP协议目前支持以下操作：
- 
检索媒体：允许用户通过HTTP或者其它方法向媒体服务器提交一个表示描述。如表示是组播的，则表示描述就包含用于该媒体流的组播地址和端口号；如果表示是单播的，为了安全在表示描述中应该只提供目的地址。
- 
邀请加入：媒体服务器可以被邀请参加正在进行的会议，或者在表示中回放媒体，或者在表示中录制全部媒体或其子集，非常适合于分布式教学。
- 
添加媒体：通知用户新加入的可利用媒体流，这对现场讲座来讲显得尤其有用。与HTTP/1.1类似，RTSP请求也可以交由代理、通道或者缓存来进行处理。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[rtsp : Real Time Streaming Protocol](http://jishudaima.iteye.com/blog/919570)
实时流协议RTSP(RealTimeStreamingProtocol)是由RealNetworks和Netscape共同提出的，该协议定义了一对多应用程序如何有效地通过IP网络传送多媒体数据。RTSP在体系结构上位于RTP和RTCP之上，它使用TCP或RTP完成数据传输。HTTP与RTSP相比，HTTP传送HTML，而RTP传送的是多媒体数据。HTTP请求由客户机发出，服务器作出响应；使用RTSP时，客户机和服务器都可以发出请求，即RTSP可以是双向的。
6.3 RTSP协议
　 实时流协议（RTSP）是应用级协议，控制实时数据的发送。RTSP提供了一个可扩展框架，使实时数据，如音频与视频，的受控、点播成为可能。数据源包括现场数据与存储在剪辑中数据。该协议目的在于控制多个数据发送连接，为选择发送通道，如UDP、组播UDP与TCP，提供途径，并为选择基于RTP上发送机制提供方法。
　 6.3.1 简介
　 6.3.1.1 目的
　 实时流协议（RTSP）建立并控制一个或几个时间同步的连续流媒体。尽管连续媒体流与控制流交叉是可能的，通常它本身并不发送连续流。换言之，RTSP充当多媒体服务器的网络远程控制。RTSP连接没有绑定到传输层连接，如TCP。在RTSP连接期间，RTSP用户可打开或关闭多个对服务器的可靠传输连接以发出RTSP
 请求。此外，可使用无连接传输协议，如UDP。RTSP流控制的流可能用到RTP，但RTSP操作并不依赖用于携带连续媒体的传输机制。实时流协议在语法和操作上与HTTP/1.1类似，因此HTTP的扩展机制大都可加入RTSP。协议支持的操作如下：
　 从媒体服务器上检索媒体：
　 用户可通过HTTP或其它方法提交一个演示描述。如演示是组播，演示式就包含用于连续媒体的的组播地址和端口。如演示仅通过单播发送给用户，用户为了安全应提供目的地址。
　 媒体服务器邀请进入会议：
　 媒体服务器可被邀请参加正进行的会议，或回放媒体，或记录其中一部分，或全部。这种模式在分布式教育应用上很有用，会议中几方可轮流按远程控制按钮。
　 将媒体加到现成讲座中：
　 如服务器告诉用户可获得附加媒体内容，对现场讲座显得尤其有用。如HTTP/1.1中类似，RTSP请求可由代理、通道与缓存处理。
　 6.3.1.2 协议特点
　 RTSP 特性如下：
　 可扩展性：
　 新方法和参数很容易加入RTSP。
　 易解析：
　 RTSP可由标准 HTTP或MIME解吸器解析。
　 安全：
　 RTSP使用网页安全机制。
　 独立于传输：
　 RTSP可使用不可靠数据报协议（UDP）、可靠数据报协议（RDP），如要实现应用级可靠，可使用可靠流协议。
　 多服务器支持：
　 每个流可放在不同服务器上，用户端自动同不同服务器建立几个并发控制连接，媒体同步在传输层执行。
　 记录设备控制：
　 协议可控制记录和回放设备。
　 流控与会议开始分离：
　 仅要求会议初始化协议提供，或可用来创建唯一会议标识号。特殊情况下， SIP或H.323
　 可用来邀请服务器入会。
　 适合专业应用：
　 通过SMPTE 时标，RTSP支持帧级精度，允许远程数字编辑
　 演示描述中立：
　 协议没强加特殊演示或元文件，可传送所用格式类型；然而，演示描述至少必须包含一个RTSP URI。
　 代理与防火墙友好：
　 协议可由应用和传输层防火墙处理。防火墙需要理解SETUP方法，为UDP媒体流打开一个"缺口"。
　 HTTP友好：
　 此处，RTSP明智的采用HTTP观念，使现在结构都可重用。结构包括Internet 内容选择平台（PICS）。由于在大多数情况下控制连续媒体需要服务器状态， RTSP不仅仅向HTTP 添加方法。
　 适当的服务器控制：
　 如用户启动一个流，他必须也可以停止一个流。
　 传输协调；
　 实际处理连续媒体流前，用户 可协调传输方法。
　 性能协调：
　 如基本特征无效，必须有一些清理机制让用户决定那种方法没生效。这允许用户提出适合的用户界面。
　 6.3.1.3扩展RTSP
　 由于不是所有媒体服务器有着相同的功能，媒体服务器有必要支持不同请求集。RTSP 可以如下三种方式扩展，这里以改变大小排序：
　 以新参数扩展。如用户需要拒绝通知，而方法扩展不支持，相应标记就加入要求的段中。
　 加入新方法。如信息接收者不理解请求，返回501错误代码（还未实现），发送者不应再次尝试这种方法。用户可使用OPTIONS方法查询服务器支持的方法。服务器使用公共响应头列出支持的方法。
　 定义新版本协议，允许改变所有部分。（除了协议版本号位置）
　 6.3.1.4操作模式
　 每个演示和媒体流可用RTSP URL识别。演示组成的整个演示与媒体属性由演示描述文件定义。使用HTTP或其它途径用户可获得这个文件，它没有必要保存在媒体服务器上。
　 为了说明，假设演示描述描述了多个演示，其中每个演示维持了一个公共时间轴。为简化说明，且不失一般性，假定演示描述的确包含这样一个演示。演示可包含多个媒体流。除媒体参数外，网络目标地址和端口也需要决定。下面区分几种操作模式：
　 单播：
　 以用户选择的端口号将媒体发送到RTSP请求源。
　 组播，服务器选择地址：
　 媒体服务器选择组播地址和端口，这是现场直播或准点播常用的方式。
　 组播，用户选择地址：
　 如服务器加入正在进行的组播会议，组播地址、端口和密匙由会议描述给出。
　 6.3.1.5 RTSP状态
　 RTSP控制通过单独协议发送的流，与控制通道无关。例如，RTSP控制可通过TCP连接，而数据流通过UDP。因此，即使媒体服务器没有收到请求，数据也会继续发送。在连接生命期，单个媒体流可通过不同TCP连接顺序发出请求来控制。所以，服务器需要维持能联系流与RTSP请求的连接状态。RTSP中很多方法与状态无关，但下列方法在定义服务器流资源的分配与应用上起着重要的作用：
　 SETUP：
　 让服务器给流分配资源，启动RTSP连接。
　 PLAY与RECORD：
　 启动SETUP 分配流的数据传输。
　 PAUSE：
　 临时停止流，而不释放服务器资源。
　 TEARDOWN：
　 释放流的资源，RTSP连接停止。
　 标识状态的RTSP方法使用连接头段识别RTSP连接，为响应SETUP请求，服务器连
　 接产生连接标识。
　 6.3.1.6 与其他协议关系
　 RTSP在功能上与HTTP有重叠，与HTTP相互作用体现在与流内容的初始接触是通过网页的。目前的协议规范目的在于允许在网页服务器与实现RTSP媒体服务器之间存在不同传递点。例如，演示描述可通过HTTP和RTSP检索，这降低了浏览器的往返传递，也允许独立RTSP 服务器与用户不全依靠HTTP。
　 但是，RTSP与HTTP 的本质差别在于数据发送以不同协议进行。HTTP是不对称协议，用户发出请求，服务器作出响应。RTSP中，媒体用户和服务器都可发出请求，且其请求都是无状态的；在请求确认后很长时间内，仍可设置参数，控制媒体流。重用HTTP功能至少在两个方面有好处，即安全和代理。要求非常接近，在缓存、代理和授权上采用HTTP功能是有价值的。
　 当大多数实时媒体使用RTP作为传输协议时，RTSP没有绑定到RTP。RTSP假设存在演示描述格式可表示包含几个媒体流的演示的静态与临时属性。
　 6.3.2 协议参数
　 6.3.3 RTSP 信息
　 RTSP是基于文本的协议，采用ISO 10646 字符集，使用UTF-8编码方案。行以CRLF中断，但接收者本身可将CR和LF解释成行终止符。基于文本的协议使以自描述方式增加可选参数更容易。由于参数的数量和命令的频率出现较低，处理效率没引起注意。如仔细研究，文本协议很容易以脚本语言（如：Tcl、Visual
 Basic与Perl）实现研究原型。
　 10646字符集避免敏感字符集切换，但对应用来说不可见。RTCP也采用这种编码方案。带有重要意义位的ISO 8859-1字符表示如100001x 10xxxxxx.。RTSP信息可通过任何低层传输协议携带。
　 请求包括方法、方法作用于其上的对象和进一步描述方法的参数。方法也可设计为在服务器端只需要少量或不需要状态维护。当信息体包含在信息中，信息体长度有如下因素决定：
　 不管实体头段是否出现在信息中，不包括信息体的的响应信息总以头段后第一和空行结束。
　 如出现内容长度头段，其值以字节计，表示信息体长度。如未出现头段，其值为零。
　 服务器关闭连接。
　 注意：RTSP目前并不支持HTTP/1.1"块"传输编码，需要有内容长度头。假如返回适度演示描述长度，即使动态产生，使块传输编码没有必要，服务器也应该能决定其长度。如有实体，即使必须有内容长度，且长度没显式给出，规则可确保行为合理。
　 从用户到服务器端的请求信息在第一行内包括源采用的方法、源标识和所用协议版本。RTSP定义了附加状态代码，而没有定义任何HTTP代码。
　 6.3.4 实体
　 如不受请求方法或响应状态编码限制，请求和响应信息可传输实体，实体由实体头文件和试题体组成，有些响应仅包括实体头。在此，根据谁发送实体、谁接收实体，发送者和接收者可分别指用户和服务器。
　 实体头定义实体体可选元信息，如没有实体体，指请求标识的资源。扩展头机制允许定义附加实体头段，而不用改变协议，但这些段不能假定接收者能识别。不可识别头段应被接收者忽略，而让代理转发。
　 6.3.5 连接
　 RTSP请求可以几种不同方式传送：
　 1、持久传输连接，用于多个请求/响应传输。
　 2、每个请求/响应传输一个连接。
　 3、无连接模式。
　 传输连接类型由RTSP URI来定义。对 "rtsp" 方案，需要持续连接；而"rtspu"方案，调用RTSP 请求发送，而不用建立连接。
　 不象HTTP，RTSP允许媒体服务器给媒体用户发送请求。然而，这仅在持久连接时才支持，否则媒体服务器没有可靠途径到达用户，这也是请求通过防火墙从媒体服务器传到用户的唯一途径。
　 6.3.6 方法定义
　 方法记号表示资源上执行的方法，它区分大小写。新方法可在将来定义，但不能以$开头。
　 某些防火墙设计与其他环境可能要求服务器插入RTSP方法和流数据。由于插入将使客户端和服务器操作复杂，并强加附加开销，除非有必要，应避免这样做。插入二进制数据仅在RTSP通过TCP传输时才可使用。流数据（如RTP包）用一个ASCII美圆符号封装，后跟一个一字节通道标识，其后是封装二进制数据的长度，两字节整数。流数据紧跟其后，没有CRLF，但包括高层协议头。每个$块包含一个高层协议数据单元。
　 当传输选择为RTP，RTCP信息也被服务器通过TCP连接插入。缺省情况下，RTCP包在比RTP通道高的第一个可用通道上发送。客户端可能在另一通道显式请求RTCP包 ，这可通过指定传输头插入参数中的两个通道来做到。当两个或更多流交叉时，为取得同步，需要RTCP。而且，这为当网络设置需要通过TCP控制连接透过RTP/RTCP提供了一条方便的途径，可能时，在UDP上进行传输。
　 6.3.7 流水线操作
　 支持持久连接或无连接的客户端可能给其请求排队。服务器必须以收到请求的同样顺序发出响应。如果请求不是发送给组播组，接收者就确认请求，如没有确认信息，发送者可在超过一个来回时间（RTT）后重发同一信息。
　 RTT在TCP中估计，初始值为500 ms。应用缓存最后所测量的RTT，作为将来连接的初始值。如使用一个可靠传输协议传输RTSP，请求不允许重发，RTSP应用反过来依赖低层传输提供可靠性。如两个低层可靠传输（如TCP 和RTSP）应用重发请求，有可能每个包损失导致两次重传。由于传输栈在第一次尝试到达接收着者前不会发送应用层重传，接收者也不能充分利用应用层重传。如包损失由阻塞引起，不同层的重发将使阻塞进一步恶化。时标头用来避免重发模糊性问题，避免对圆锥算法的依赖。每个请求在CSeq头中携带一个系列号，每发送一个不同请求，它就加一。如由于没有确认而重发请求，请求必须携带初始系列号。
　 实现RTSP的系统必须支持通过TCP传输RTSP ，并支持UDP。对UDP和TCP，RTSP服务器的缺省端口都是554。许多目的一致的RTSP包被打包成单个低层PDU或TCP流。RTSP数据可与RTP和RTCP包交叉。不象HTTP，RTSP信息必须包含一个内容长度头，无论信息何时包含负载。否则，RTSP包以空行结束，后跟最后一个信息头。
