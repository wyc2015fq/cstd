# rtp、rtsp客户端开发流程 - 建建的博客 - CSDN博客
2017年09月08日 11:54:33[纪建](https://me.csdn.net/u013898698)阅读数：304
个人分类：[Rtsp协议](https://blog.csdn.net/u013898698/article/category/6698534)
环境：VLC做服务器，url:rtsp://192.168.100.77:8554/1
              rtsp链路采用tcp链接
              rtp采用udp
关于rtp、rtsp、rtcp的知识，这里不做详细解释，网上有大把的资料，用户可以自己问度娘。这里直接切入正题。
PS：这里还需要唠叨一点，rtsp为字符协议，字符协议中含大量不可见字符空格、tab、换行符等等，但是这些不可见字符如果不对，服务器是无法解析协议的。所以在调试字符协议时最好的方法是使用抓包工具，抓取正确的数据包，然后自己跟着模仿最快，当然你也可以看rfc文档。
## 1、rtsp客户端
step1:创建一个tcp socket，链接到192.168.100.77：8554上
step2:发送OPTIONS：
OPTIONS rtsp://192.168.100.77:8554/1 RTSP/1.0
CSeq: 1
User-Agent: LibVLC/2.2.4 (LIVE555 Streaming Media v2016.02.22)
等待应答，接收到应答如下：
RTSP/1.0 200 OK
Server: VLC/2.2.4
Content-Length: 0
Cseq: 1
Public: DESCRIBE,SETUP,TEARDOWN,PLAY,PAUSE,GET_PARAMETER
#PS:CSeq 表示sn,每条指令自动加1即可
step3:发送DESCRIBE
DESCRIBE rtsp://192.168.100.77:8554/1 RTSP/1.0
CSeq: 2
User-Agent: LibVLC/2.2.4 (LIVE555 Streaming Media v2016.02.22)
Accept: application/sdp
等待应答，接收到应答如下：
RTSP/1.0 200 OK
Server: VLC/2.2.4
Date: Fri, 24 Feb 2017 02:01:14 GMT
Content-Type: application/sdp
Content-Base: rtsp://192.168.100.77:8554/1
Content-Length: 563
Cache-Control: no-cache
Cseq: 2
v=0
o=- 15878023685032985736 15878023685032985736 IN IP4 XZS201603082
s=Unnamed
i=N/A
c=IN IP4 0.0.0.0
t=0 0
a=tool:vlc 2.2.4
a=recvonly
a=type:broadcast
a=charset:UTF-8
a=control:rtsp://192.168.100.77:8554/1
m=audio 0 RTP/AVP 14
b=AS:128
b=RR:0
a=rtpmap:14 MPA/90000/2
a=control:rtsp://192.168.100.77:8554/1/trackID=2
m=video 0 RTP/AVP 96
b=RR:0
a=rtpmap:96 H264/90000
a=fmtp:96 packetization-mode=1;profile-level-id=64001f;sprop-parameter-sets=Z2QAH6zZQPARfvARAAADAAEAAAMAMo8YMZY=,aOvjyyLA;
a=control:rtsp://192.168.100.77:8554/1/trackID=3
#PS：这里的trackID=3需要解析处理，下面setup指令要用到。
a=control的值可能是相对的URL，也可能是绝对的URL。判断相对还是绝对的方法参见RFC1808。（估计就是是否有串“://”）
如果a=control的值是相对的则从Content-Base，Content-Location，request URL中去获得基路径。如果a=control的值为*，则使用的值即为Content-Base，Content-Location，request URL的值。request URL指DESCRIBE时的url。因为a=control是在DESCRIBE后返回的sdp中存放的。
![](https://img-blog.csdn.net/20170608134229908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveXVhbmJpbnF1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
step4、发送SETUP
SETUP rtsp://192.168.100.77:8554/1/trackID=3 RTSP/1.0
CSeq: 3
User-Agent: LibVLC/2.2.4 (LIVE555 Streaming Media v2016.02.22)
Transport: RTP/AVP;unicast;client_port=52930-52931
#PS：这里注意client_port=52930-52931，client的后面52930是rtp端口，52931是rtcp端口，这里告诉了服务器，客户端接收rtp数据端口，服务器发送rtp数据时发送到52930端口上
等待应答，接收到应答如下：
RTSP/1.0 200 OK
Server: VLC/2.2.4
Date: Fri, 24 Feb 2017 02:01:16 GMT
Transport: RTP/AVP/UDP;unicast;client_port=52930-52931;server_port=54545-54546;ssrc=7009A25D;mode=play
Session: 1cc1373972ac1e0b;timeout=60
Content-Length: 0
Cache-Control: no-cache
Cseq: 3
#PS:这里注意三个字段，需要做解析。Session: 1cc1373972ac1e0b，一个64字节的session,setup获取session之后，rtsp所有的指令都需要发送该session，表明你当前会话窗口；timeout=60超时时间，如果超过60s
 rtsp链路上没有任何数据，当前session的所有链接、活动都会结束；server_port=54545-54546，54545表示服务器rtp数据发送端口，54546表示服务器rtcp端口
step5:发送PLAY
PLAY rtsp://192.168.100.77:8554/1 RTSP/1.0
CSeq: 4
User-Agent: LibVLC/2.2.4 (LIVE555 Streaming Media v2016.02.22)
Session: 1cc1373972ac1e0b
Range: npt=0.000-
等待应答，接收到应答如下：
RTSP/1.0 200 OK
Server: VLC/2.2.4
Date: Fri, 24 Feb 2017 02:01:18 GMT
RTP-Info: url=rtsp://192.168.100.77:8554/1/trackID=3;seq=50578;rtptime=3808865604
Range: npt=3582.742781-
Session: 1cc1373972ac1e0b;timeout=60
Content-Length: 0
Cache-Control: no-cache
Cseq: 4
#PS:在客户端发送play指令之后，需要启动rtp客户端在指定的端口（52930）上，接收UDP数据
step6:每隔固定时间发送心跳指令GET_PARAMETER
GET_PARAMETER rtsp://192.168.100.77:8554/1 RTSP/1.0
CSeq: 5
User-Agent: LibVLC/2.2.4 (LIVE555 Streaming Media v2016.02.22)
Session: 1cc1373972ac1e0b
x-Timeshift_Range:
等待应答，接收到应答如下：
RTSP/1.0 200 OK
Server: VLC/2.2.4
Date: Fri, 24 Feb 2017 02:01:40 GMT
Session: 1cc1373972ac1e0b;timeout=60
Content-Length: 0
Cache-Control: no-cache
Cseq: 5
step7：发送TEARDOWN接收session
TEARDOWN rtsp://192.168.20.136:5000/xxx666 RTSP/1.0
CSeq: 6
User-Agent: LibVLC/2.2.4 (LIVE555 Streaming Media v2016.02.22)
Session: 1cc1373972ac1e0b
等待应答，接收到应答如下：
RTSP/1.0 200 OK
Server: VLC/2.2.4
Cseq: 6
Session: 1cc1373972ac1e0b
Connection: Close
## 2、rtp客户端
step1、创建一个udp socket,绑定到固定的端口上
step2、将端口号传递给rtsp 客户端线程，rtsp 会在setup指令时通知服务器，rtp客户端udp所使用的端口
step3、一直在该socket上接收rtp服务器发送过来的udp数据包，进行组包、重传控制
