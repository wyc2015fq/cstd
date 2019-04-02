# RTSP协议笔记

   

------

抓包下载地址：<https://download.csdn.net/download/knowledgebao/10802206>

## RTSP简介

RTSP（Real Time Streaming Protocol）是由Real Network和Netscape共同提出的如何有效地在IP网络上传输流媒体数据的应用层协议。RTSP对流媒体提供了诸如暂停，快进等控制，而它本身并不传输数据，RTSP的作用相当于流媒体服务器的远程控制。服务器端可以自行选择使用TCP或UDP来传送串流内容，它的语法和运作跟HTTP 1.1类似，但并不特别强调时间同步，所以比较能容忍网络延迟。

## RTSP和HTTP的关系

- 联系：两者都用纯文本来发送消息，且rtsp协议的语法也和HTTP类似。Rtsp一开始这样设计，也是为了能够兼容使用以前写的HTTP协议分析代码 。
- 区别：rtsp是有状态的，不同的是RTSP的命令需要知道现在正处于一个什么状态，也就是说rtsp的命令总是按照顺序来发送，某个命令总在另外一个命令之前要发送。Rtsp不管处于什么状态都不会断掉连接。而http则不保存状态，协议在发送一个命令以后，连接就会断开，且命令之间是没有依赖性。rtsp协议默认使用554端口，http使用80端口。

## RTSP、RTP、RTCP

- **RTP：**Realtime Transport Potocol 实时传输协议，完成具体的数据传输。RTP提供时间标志,序列号以及其他能够保证在实时数据传输时处理时间的方法。RTP一般基于 UDP协议的， UDP不用建立连接，效率更高；但允许丢包， 这就要求在重新组装媒体的时候多做些工作。但RTP使用TCP协议发送的时候，实际使用的RTCP这个链接，又名rtp over rtsp。RTP只是包裹内容信息，而RTCP是交换控制信息的，Qos是通过RTCP实现的 
- **RTCP：**Realtime Transport Control Potocol 实时传输控制协议，RTCP是RTP的控制部分,用来保证服务质量和成员管理。RTP和RTCP是一起使用的。RTCP控制rtp的发送接收情况，监控rtp正常传输的一个协议。
- **RTSP：**RealTime Streaming Potocol 实时流协议，建立在TCP的基础之上。 RTSP具体数据传输交给RTP,提供对流的远程控制RTSP对应的方法包括play, seek, pause, stop等命令，RTSP则是处理这些方法命令。RTSP的client连接server通过SDP（会话描述协议）传递信息，详细请见：RTSP消息

## RTSP消息

RTSP的消息有两大类，一是请求消息(request)，一是回应消息(response)，两种消息的格式不同。 
**请求消息格式：**

```
方法 URI RTSP版本 CR LF 
消息头 CR LF CR LF 
消息体 CR LF
```

方法包括：OPTIONS、 DESCRIBE 、SETUP、PLAY、TEARDOWN等
URI是接收方（服务端）的地址，例如：rtsp://192.168.22.136:554/m1a1 
每行后面的CR LF表示回车换行，需要接收端有相应的解析，消息头需要有两个CR LF。

**回应消息格式：**

```
RTSP版本 状态码 解释 CR LF 
消息头 CR LF CR LF 
消息体 CR LF
```

**简单的rtsp交互过程:**
C表示rtsp客户端, S表示rtsp服务端

```
step1: 
C->S:OPTION request //询问S有哪些方法可用 
S->C:OPTION response //S回应信息中包括提供的所有可用方法
 
step2: 
C->S:DESCRIBE request //要求得到S提供的媒体初始化描述信息 
S->C:DESCRIBE response //S回应媒体初始化描述信息，主要是sdp
 
step3: 
C->S:SETUP request //设置会话的属性，以及传输模式，提醒S建立会话 
S->C:SETUP response //S建立会话，返回会话标识符，以及会话相关信息
 
step4: 
C->S:PLAY request //C请求播放 
S->C:PLAY response //S回应该请求的信息
 
S->C:发送流媒体数据
 
step5: 
C->S:TEARDOWN request //C请求关闭会话 
S->C:TEARDOWN response //S回应该请求
```

## RTSP中常用方法

**OPTION**
得到服务器提供的可用方法

```
OPTIONS rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 1 //每个消息都有序号来标记，第一个包通常是option请求消息
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
```

服务器的回应信息包括提供的一些方法,例如:

```
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1
Cseq: 1 //每个回应消息的cseq数值和请求消息的cseq相对应
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SCALE,GET_PARAMETER //服务器提供的可用的方法
```

**DESCRIBE**
C向S发起DESCRIBE请求,为了得到会话描述信息(SDP):

```
DESCRIBE rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 2
token: 
Accept: application/sdp
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

服务器回应一些对此会话的描述信息(sdp):

```
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
 
v=0 //以下都是sdp信息  
o=OnewaveUServerNG 1451516402 1025358037 IN IP4 192.168.20.136 
s=/xxx666 
u=http:/// 
e=admin@ 
c=IN IP4 0.0.0.0 
t=0 0 
a=isma-compliance:1,1.0,1 
 
a=range:npt=0- 
m=video 0 RTP/AVP 96 //m表示媒体描述，下面是对会话中视频通道的媒体描述
a=rtpmap:96 MP4V-ES/90000 
a=fmtp:96 profile-level-id=245;config=000001B0F5000001B509000001000000012000C888B0E0E0FA62D089028307 a=control:trackID=0 //trackID＝0表示视频流用的是通道0
```

**SETUP**
客户端提醒服务器建立会话,并确定传输模式:

```
SETUP rtsp://192.168.20.136:5000/xxx666/trackID=0 RTSP/1.0 
CSeq: 3 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
 //uri中 带有trackID＝0，表示对该通道进行设置。Transport参数设置了传输模式，包的结构。接下来的数据包头部第二个字节位置就是 interleaved，它的值是每个通道都不同的，trackID＝0的interleaved值有两个0或1，0表示rtp包，1表示rtcp包，接收端根据interleaved的值来区别是哪种数据包。
```

服务器回应信息:

```
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 3 
Session: 6310936469860791894 //服务器回应的会话标识符
Cache-Control: no-cache 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=6B8B4567
```

**PLAY**
客户端发送播放请求:

```
PLAY rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 4 
Session: 6310936469860791894 
Range: npt=0.000- //设置播放时间的范围
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10)
```

服务器回应信息:

```
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 4 
Session: 6310936469860791894 
Range: npt=0.000000- 
RTP-Info: url=trackID=0;seq=17040;rtptime=1467265309 
 //seq和rtptime都是rtp包中的信息
```

**TEARDOWN**
客户端发起关闭请求:

```
TEARDOWN rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 5 
Session: 6310936469860791894 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
```

服务器回应:

```
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 5 
Session: 6310936469860791894 
```

**SDP协议**
sdp的格式：

```
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
```

- 时间描述： 

t = （会话活动时间） 
r = * （0或多次重复次数）

- 媒体描述： 

m = （媒体名称和传输地址） 
i = * （媒体标题） 
c = * （连接信息 — 如果包含在会话层则该字段可选） 
b = * （带宽信息） 
k = * （加密密钥） 
a = * （0 个或多个媒体属性行）

SDP是一个会话描述协议一描述SIP、RTSR等会话的协议,是由服务器生成的描述媒体文件编码信息以及所在服务器的链接等的信息。在多媒体会话 中SDP传送有关媒体流的信息,使会话描述的参人方加人会话。SDP主要用于Intemet网中,但也可以在其它网络环境下使用。SDP十分通用,可描述其它网络环境中的会话,但主要用 于Intemet中。在Intemet环境下,SDP有两个主要目的:一是表明会话存在,二是传送足够信息给接收方,以便能加人、参加该会话。SDP所传达的信息包括:会话名称和目的,会话 活动时间,组成会话媒体种类,接收这些媒体的控制信息(如地址、端口、格式、带宽和会议管理人员资料等)。

**总结：**在RTSP交互过程中，只要在客户端发出Describe请求的时候，服务端回应的时候会有SDP消息发出，用SDP来描述会话情况和内容，方便客户端能够加入该会话。

负载一个抓包，如下：

<https://download.csdn.net/download/knowledgebao/10802206>

```
OPTIONS rtsp://10.1.33.13/trials/test.mp4 RTSP/1.0
CSeq: 1
User-Agent: Chnsys Streaming Media v2009.07.27
 
RTSP/1.0 200 OK
Server: ZMedia Server(2.0)
CSeq: 1
Public: OPTIONS, DESCRIBE, SETUP, PLAY, PAUSE, TEARDOWN, GET_PARAMETER
 
DESCRIBE rtsp://10.1.33.13/trials/test.mp4 RTSP/1.0
CSeq: 2
Accept: application/sdp
User-Agent: Chnsys Streaming Media v2009.07.27
 
RTSP/1.0 200 OK
Server: ZMedia Server(2.0)
CSeq: 2
Content-Length: 536
 
v=0
o=ZMediaServer 1 1645103501 IN IP4 10.1.33.13
s=ZMediaServer
c=IN IP4 0.0.0.0
t=0 0
a=tool: ZMediaServer
a=range:npt=0-7200.000000.
m=audio 0 RTP/AVP 98
a=rtpmap:98 mpeg4-generic/1000/2
a=fmtp:98 streamtype=5; profile-level-id=21; mode=generic; config=1610; SizeLength=13;IndexLength=3; IndexDeltaLength=3; Profile=1;
a=control:trackID=0
m=video 0 RTP/AVP 96
a=rtpmap:96 H264/90000
a=fmtp:96 packetization-mode=1; sprop-parameter-sets=Z2QAKq2EAQwgCGEAQwgCGEAQwgCEK1A8ARPywgAAAwACAAADAHkI,aO48sA==
a=control:trackID=1
SETUP rtsp://10.1.33.13/trials/test.mp4/trackID=0 RTSP/1.0
CSeq: 3
Transport: RTP/AVP;unicast;client_port=49970-49971
User-Agent: Chnsys Streaming Media v2009.07.27
 
RTSP/1.0 200 OK
Server: ZMedia Server(2.0)
CSeq: 3
timestamp: 7200.000
Session: -1729321850532498058
Transport: RTP/AVP;unicast;client_port=49970-49971;server_port=6982-6983
 
SETUP rtsp://10.1.33.13/trials/test.mp4/trackID=1 RTSP/1.0
CSeq: 4
Transport: RTP/AVP;unicast;client_port=49972-49973
Session: -1729321850532498058
User-Agent: Chnsys Streaming Media v2009.07.27
 
RTSP/1.0 200 OK
Server: ZMedia Server(2.0)
CSeq: 4
timestamp: 7200.000
Session: -1729321850532498058
Transport: RTP/AVP;unicast;client_port=49972-49973;server_port=6982-6983
 
PLAY rtsp://10.1.33.13/trials/test.mp4 RTSP/1.0
CSeq: 5
Session: -1729321850532498058
Range: npt=0.000-
User-Agent: Chnsys Streaming Media v2009.07.27
 
RTSP/1.0 200 OK
Server: ZMedia Server(2.0)
CSeq: 5
Session: -1729321850532498058
Range: npt=0.000-7200.000
 
TEARDOWN rtsp://10.1.33.13/trials/test.mp4 RTSP/1.0
CSeq: 6
Session: -1729321850532498058
User-Agent: Chnsys Streaming Media v2009.07.27
 
RTSP/1.0 200 OK
Server: ZMedia Server(2.0)
CSeq: 6
Session: -1729321850532498058
Connection: Close
```

------

参考资料：

1，

------

有任何问题请联系：knowledgebao@163.com
 