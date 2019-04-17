# rtsp和sdp协议简介 - DoubleLi - 博客园






RTSP是由Real network 和Netscape共同提出的如何有效地在IP网络上传输流媒体数据的应用层协议。 实时流协议（RTSP）建立并控制一个或几个时间同步的连续流媒体，如音频和视频。尽管连续媒体流与控制流交叉是可能的，RTSP 本身并不发送连续媒体流。换言之，RTSP 充当多媒体服务器的网络远程控制。RTSP 提供了一个可扩展框架，实现实时数据（如音频与视频）的受控、按需传送。数据源包括实况数据与存储的剪辑。RTSP 用于控制多个数据发送会话，提供了选择发送通道（如 UDP、组播 UDP 与 TCP 等）的方式，并提供了选择基于 RTP 的发送机制的方法。

目前还没有 RTSP 连接的概念；服务器维护由识别符标识的会话。RTSP 会话不会绑定到传输层连接，如 TCP。在 RTSP 会话期间，RTSP 客户端可打开或关闭多个对服务器的可靠传输连接以发出 RTSP 请求。它也可选择使用无连接传输协议，如 UDP。

RTSP 控制的流可能用到 RTP，但 RTSP 操作并不依赖用于传输连续媒体的传输机制。RTSP 在语法和操作上与 HTTP/1.1 类似，因此 HTTP 的扩展机制在多数情况下可加入 RTSP。然而，在很多重要方面 RTSP 仍不同于 HTTP ：
- RTSP 引入了大量新方法并具有一个不同的协议标识符：
- 在大多数情况下，RTSP 服务器需要保持缺省状态，与 HTTP 的无状态相对；
- RTSP 中客户端和服务器都可以发出请求；
- 在多数情况下，数据由不同的协议传输；
- RTSP 使用 ISO 10646 （UTF-8）而并非 ISO 8859-1，与当前的国际标准 HTML 相一致；
- URI 请求总是包含绝对 URI。为了与过去的错误相互兼容，HTTP/1.1 只在请求过程中传送绝对路径并将主机名置于另外的头字段。

该协议支持如下操作：
- 从媒体服务器上检索媒体：用户可通过 HTTP 或其它方法提交一个演示描述请求；
- 媒体服务器邀请进入会议： 媒体服务器可被邀请参加正进行的会议，或回放媒体，或记录部分或全部演示；
- 将新媒体加到现有演示中：如服务器能告诉客户端接下来可用的媒体内容，对现场直播显得尤其有用

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



RTSP消息格式:

RTSP的消息有两大类 --- 请求消息(request), 回应消息(response)。

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

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

简单的rtsp交互过程: 

C表示rtsp客户端, S表示rtsp服务端

1. C->S:OPTION request //询问S有哪些方法可用
1. S->C:OPTION response //S回应信息中包括提供的所有可用方法

2. C->S:DESCRIBE request //要求得到S提供的媒体初始化描述信息
2. S->C:DESCRIBE response //S回应媒体初始化描述信息，主要是sdp

3. C->S:SETUP request //设置会话的属性，以及传输模式，提醒S建立会话
3. S->C:SETUP response //S建立会话，返回会话标识符，以及会话相关信息

4. C->S:PLAY request //C请求播放
4. S->C:PLAY response //S回应该请求的信息

S->C:发送流媒体数据

5. C->S:TEARDOWN request //C请求关闭会话
5. S->C:TEARDOWN response //S回应该请求

上述的过程是标准的、友好的rtsp流程，但实际的需求中并不一定按部就班来。 其 中第3和4步是必需的！第一步，只要服务器客户端约定好，有哪些方法可用，则option请求可以不要。第二步，如果我们有其他途径得到媒体初始化描述信 息（比如http请求等等），则我们也不需要通过rtsp中的describe请求来完成。第五步，可以根据系统需求的设计来决定是否需要。

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

rtsp中常用方法: 

1. OPTION
目的是得到服务器提供的可用方法:
OPTIONS rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 1 //每个消息都有序号来标记，第一个包通常是option请求消息
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)

服务器的回应信息包括提供的一些方法,例如:
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1
Cseq: 1 //每个回应消息的cseq数值和请求消息的cseq相对应
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SCALE,GET_PARAMETER //服务器提供的可用的方法

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

v=0 //以下都是sdp信息
o=OnewaveUServerNG 1451516402 1025358037 IN IP4 192.168.20.136 
s=/xxx666 
u=http:/// 
e=admin@ 
c=IN IP4 0.0.0.0 
t=0 0 
a=isma-compliance:1,1.0,1 

a=range:npt=0- 
m=video 0 RTP/AVP 96 //m表示媒体描述，下面是对会话中视频通道的媒体描述
a=rtpmap:96 MP4V-ES/90000 
a=fmtp:96 profile-level-id=245;config=000001B0F5000001B509000001000000012000C888B0E0E0FA62D089028307 a=control:trackID=0 //trackID＝0表示视频流用的是通道0

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
Session: 6310936469860791894 //服务器回应的会话标识符
Cache-Control: no-cache 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=6B8B4567

4.PLAY 
客户端发送播放请求:
PLAY rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 4 
Session: 6310936469860791894 
Range: npt=0.000- //设置播放时间的范围
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



- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


sdp的格式：

v=<version> o=<username> <session id> <version> <network type> <address type> <address> s=<session name> i=<session description> u=<URI> e=<email address>
p=<phone number>
c=<network type> <address type> <connection address> b=<modifier>:<bandwidth-value> t=<start time> <stop time> r=<repeat interval> <active duration> <list of offsets from start-time> z=<adjustment time> <offset> <adjustment time> <offset> .... k=<method>
k=<method>:<encryption key> a=<attribute>
a=<attribute>:<value> m=<media> <port> <transport> <fmt list> v = （协议版本） o = （所有者/创建者和会话标识符） s = （会话名称） i = * （会话信息） u = * （URI 描述） e = * （Email 地址） p = * （电话号码） c = * （连接信息） b = * （带宽信息） z = * （时间区域调整） k = * （加密密钥） a = * （0 个或多个会话属性行）
时间描述： t = （会话活动时间） r = * （0或多次重复次数）
媒体描述： m = （媒体名称和传输地址） i = * （媒体标题） c = * （连接信息 — 如果包含在会话层则该字段可选） b = * （带宽信息） k = * （加密密钥） a = * （0 个或多个媒体属性行）[http://hi.baidu.com/007%B4%F3%BB%A8%C3%A8/blog/item/7d8679057baf01f608fa93fc.html](http://hi.baidu.com/007%B4%F3%BB%A8%C3%A8/blog/item/7d8679057baf01f608fa93fc.html)









