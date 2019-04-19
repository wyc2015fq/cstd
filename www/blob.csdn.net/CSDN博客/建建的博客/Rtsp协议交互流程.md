# Rtsp协议交互流程 - 建建的博客 - CSDN博客
2017年08月01日 18:09:14[纪建](https://me.csdn.net/u013898698)阅读数：1772
**Real Time Streaming Protocol或者RTSP（实时流媒体协议），是由Real network 和 Netscape共同提出的如何有效地在IP网络上传输流媒体数据的应用层协议。RTSP提供一 种可扩展的框架，使能够提供可控制的，按需传输实时数据，比如音频和视频文件。源数据可以包括现场数据的反馈和存贮的文件。rtsp对流媒体提供了诸如暂停，快进等控制，而它本身并不传输数据，rtsp作用相当于流媒体服务器的远程控制。传输数据可以通过传输层的tcp，udp协议，rtsp也提供了基于rtp传输机制的一些有效的方法。**
目录 [[hide](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/)]
- [1 RTSP消息格式](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#RTSP)
- [2 简单的rtsp交互过程](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#rtsp)
- [3 rtsp中常用方法](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#rtsp-2)- [3.1 OPTION](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#OPTION)
- [3.2 DESCRIBE](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#DESCRIBE)
- [3.3 SETUP](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#SETUP)
- [3.4 PLAY](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#PLAY)
- [3.5 PAUSE](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#PAUSE)
- [3.6 TEARDOWN](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#TEARDOWN)
- [3.7 其他方法](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#i)
- [4 sdp的格式](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#sdp)
- [5 RTSP点播消息流程实例](http://lnmcc.net/2014/01/26/rtsp%E4%BA%A4%E4%BA%92%E5%91%BD%E4%BB%A4%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%BF%87%E7%A8%8B%E5%8F%82%E6%95%B0%E6%8F%8F%E8%BF%B0/#RTSP-2)
# RTSP消息格式
RTSP的消息有两大类,一是请求消息(request),一是回应消息(response),两种消息的格式不同.
**请求消息:**
方法 URI RTSP版本 CR LF 
消息头 CR LF CR LF 
消息体 CR LF 
其中方法包括OPTION回应中所有的命令,URI是接受方的地址,例如
rtsp://192.168.20.136
RTSP版本一般都是 RTSP/1.0.每行后面的CR LF表示回车换行，需要接受端有相应的解析，最后一个消息头需要有两个CR LF
**回应消息:**
RTSP版本 状态码 解释 CR LF 
消息头 CR LF CR LF 
消息体 CR LF
其中RTSP版本一般都是RTSP/1.0,状态码是一个数值,200表示成功,解释是与状态码对应 的文本解释。
# 简单的rtsp交互过程
C表示rtsp客户端,S表示rtsp服务端
1.C->S:OPTION request //询问S有哪些方法可用 
1.S->C:OPTION response //S回应信息中包括提供的所有可用方法 
2.C->S:DESCRIBE request //要求得到S提供的媒体初始化描述信息 
2.S->C:DESCRIBE response //S回应媒体初始化描述信息，主要是sdp 
3.C->S:SETUP request //设置会话的属性，以及传输模式，提醒S建立会话 
3.S->C:SETUP response //S建立会话，返回会话标识符，以及会话相关信息 
4.C->S:PLAY request //C请求播放 
4.S->C:PLAY response //S回应该请求的信息 
5.S->C:发送流媒体数据 
6.C->S:TEARDOWN request //C请求关闭会话 
6.S->C:TEARDOWN response //S回应该请求 
上述的过程是标准的、友好的rtsp流程，但实际的需求中并不一定按部就班来。
其中第3和4步是必需的！
第一步，只要服务器客户端约定好，有哪些方法可用，则option请求可以不要。第二步，如果我们有其他途径得到媒体初始化描述信息（比如http请求等等），则我们也不需要通过rtsp中的describe请求来完成。第五步，可以根据系统需求的设计来决定是否需要。
# rtsp中常用方法
## OPTION
目的是得到服务器提供的可用方法:
OPTIONS rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 1 //每个消息都有序号来标记，第一个包通常是option请求消息 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
服务器的回应信息包括提供的一些方法,例如:
 
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 1 //每个回应消息的cseq数值和请求消息的cseq相对应 
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SCALE, 
GET_PARAMETER //服务器提供的可用的方法 
## DESCRIBE
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
a=fmtp:96 
profile-level-id=245;config=000001B0F5000001B509000001000000012000C888B0E0E0FA62D089028307 
a=control:trackID=0//trackID＝0表示视频流用的是通道0 
## SETUP
客户端提醒服务器建立会话,并确定传输模式:
SETUP rtsp://192.168.20.136:5000/xxx666/trackID=0 RTSP/1.0 
CSeq: 3 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
uri中带有trackID＝0，表示对该通道进行设置。Transport参数设置了传输模式，包
的结构。接下来的数据包头部第二个字节位置就是interleaved，它的值是每个通道都
不同的，trackID＝0的interleaved值有两个0或1，0表示rtp包，1表示rtcp包，接受端
根据interleaved的值来区别是哪种数据包。
服务器回应信息:
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 3 
Session: 6310936469860791894 //服务器回应的会话标识符 
Cache-Control: no-cache 
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=6B8B4567 
## PLAY
客户端发送播放请求:
PLAY rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 4 
Session: 6310936469860791894 
Range: npt=0.000- //设置播放时间的范围 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
服务器回应信息:
 
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 4 
Session: 6310936469860791894 
Range: npt=0.000000- 
RTP-Info: url=trackID=0;seq=17040;rtptime=1467265309 
//seq和rtptime都是rtp包中的信息 
## PAUSE
客户端发起暂停请求:
PAUSE rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
Cseq: 5
Session: 6310936469860791894
服务器回应:
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 5 
Session: 6310936469860791894
## TEARDOWN
客户端发起关闭请求:
TEARDOWN rtsp://192.168.20.136:5000/xxx666 RTSP/1.0 
CSeq: 6
Session: 6310936469860791894 
User-Agent: VLC media player (LIVE555 Streaming Media v2005.11.10) 
服务器回应:
 
RTSP/1.0 200 OK 
Server: UServer 0.9.7_rc1 
Cseq: 6
Session: 6310936469860791894 
Connection: Close 
## 其他方法
以上方法都是交互过程中最为常用的,其它还有一些重要的方法如:
get/set_parameter,pause,redirect等等
# sdp的格式
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
v = （协议版本） 
o = （所有者/创建者和会话标识符） 
s = （会话名称） 
i = * （会话信息） 
u = * （URI 描述） 
e = * （Email 地址） 
p = * （电话号码） 
c = * （连接信息） 
b = * （带宽信息） 
z = * （时间区域调整） 
k = * （加密密钥） 
a = * （0 个或多个会话属性行） 
时间描述： 
t = （会话活动时间） 
r = * （0或多次重复次数） 
媒体描述： 
m = （媒体名称和传输地址） 
i = * （媒体标题） 
c = * （连接信息 — 如果包含在会话层则该字段可选） 
b = * （带宽信息） 
k = * （加密密钥） 
a = * （0 个或多个媒体属性行） 
# RTSP点播消息流程实例
客户端：VLC
RTSP服务器：LIVE555 Media Server
1)C(Client)-> M(Media Server) 
OPTIONS rtsp://192.168.1.109/1.mpg RTSP/1.0 
CSeq: 1 
user-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 
1)M -> C 
RTSP/1.0 200 OK 
CSeq: 1 
Date: wed, Feb 20 2008 07:13:24 GMT 
Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE 
2)C -> M 
DESCRIBE rtsp://192.168.1.109/1.mpg RTSP/1.0 
CSeq: 2 
Accept: application/sdp 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 
2)M -> C 
RTSP/1.0 200 OK 
CSeq: 2 
Date: wed, Feb 20 2008 07:13:25 GMT 
Content-Base: rtsp://192.168.1.109/1.mpg/ 
Content-type: application/sdp 
Content-length: 447 
v=0 
o =- 2284269756 1 IN IP4 192.168.1.109 
s=MPEG-1 or 2 program Stream, streamed by the LIVE555 Media Server 
i=1.mpg 
t=0 0 
a=tool:LIVE555 Streaming Media v2008.02.08 
a=type:broadcast 
a=control:* 
a=range:npt=0-66.181 
a=x-qt-text-nam:MPEG-1 or Program Stream, streamed by the LIVE555 Media Server 
a=x-qt-text-inf:1.mpg 
m=video 0 RTP/AVP 32 
c=IN IP4 0.0.0.0 
a=control:track1 
m=audio 0 RTP/AVP 14 
c=IN IP4 0.0.0.0 
a=control:track2 
3)C -> M 
SETUP rtsp://192.168.1.109/1.mpg/track1 RTSP/1.0 
CSeq: 3 
Transport: RTP/AVP; unicast;client_port=1112-1113 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 
3)M -> C 
RTSP/1.0 200 OK 
CSeq: 3 
Date: wed, Feb 20 2008 07:13:25 GMT 
Transport: RTP/AVP;unicast;destination=192.168.1.222;source=192.168.1.109;client_port=1112-1113;server_port=6970-6971 
Session: 3 
4)C -> M 
SETUP rtsp://192.168.1.109/1.mpg/track2 RTSP/1.0 
CSeq: 4 
Transport: RTP/AVP; unicast;client_port=1114-1115 
Session: 3 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 
4)M -> C 
RTSP/1.0 200 OK 
CSeq: 4 
Date: wed, Feb 20 2008 07:13:25 GMT 
Transport: RTP/AVP;unicast;destination=192.168.1.222;source=192.168.1.109;client_port=1114-1115;server_port=6972-6973 
Session: 3 
5)C -> M 
PLAY rtsp://192.168.1.109/1.mpg/ RTSP/1.0 
CSeq: 5 
Session: 3 
Range: npt=0.000- 
User-Agent: VLC media player(LIVE555 Streaming Media v2007.02.20) 
5)M -> C 
RTSP/1.0 200 OK 
CSeq: 5 
Range: npt=0.000- 
Session: 3 
RTP-Info: url=rtsp://192.168.1.109/1.mpg/track1;seq=9200;rtptime=214793785,url=rtsp://192.168.1.109/1.mpg/track2;seq=12770;rtptime=31721
(开始传输流媒体…)
