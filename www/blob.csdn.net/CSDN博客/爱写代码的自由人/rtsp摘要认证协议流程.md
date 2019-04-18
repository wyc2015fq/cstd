# rtsp摘要认证协议流程 - 爱写代码的自由人 - CSDN博客





2018年02月26日 10:41:12[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：465










### 1. rtsp摘要认证协议流程

RTSP协议，全称Real Time Streaming Protocol，是应用层的协议，它主要实现的功能是传输并控制具有实时特性的媒体流，如音频（Audio）和视频（Video）。Rtsp认证主要分为两种：基本认证（basic authentication）和摘要认证（ digest authentication　）。基本认证是http 1.0提出的认证方案，其消息传输不经过加密转换因此存在严重的安全隐患。摘要认证是http 1.1提出的基本认证的替代方案，其消息经过MD5哈希转换因此具有更高的安全性。下面主要介绍摘要认证：

**摘要认证 Digest authentication**

客户端第一次发起连接请求：

OPTIONS rtsp://192.168.123.158:554/11 RTSP/1.0

CSeq: 1

User-Agent: LibVLC/2.0.5(LIVE555 Streaming Media v2012.09.13)

服务器端返回服务端信息及public方法：

RTSP/1.0 200 OK

Server: HiIpcam/V100R003 VodServer/1.0.0

Cseq: 1

Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY,GET_PARAMETER

客户端再次发起连接：

DESCRIBE rtsp://192.168.123.158:554/11 RTSP/1.0

CSeq: 2

User-Agent: LibVLC/2.0.5(LIVE555 Streaming Media v2012.09.13)

Accept: application/sdp

服务器端返回401错误，提示未认证并以nonce质询：

RTSP/1.0 401 Unauthorized

Server: HiIpcam/V100R003 VodServer/1.0.0

Cseq: 2

WWW-Authenticate:Digest realm="HipcamRealServer",

nonce="3b27a446bfa49b0c48c3edb83139543d"

客户端以用户名，密码，nonce，HTTP方法，请求的URI等信息为基础产生response信息进行反馈（计算方法参考说明）：

DESCRIBErtsp://192.168.123.158:554/11 RTSP/1.0

CSeq: 3

Authorization: Digest username="admin",realm="Hipcam RealServer", nonce="3b27a446bfa49b0c48c3edb83139543d",uri="rtsp://192.168.123.158:554/11", response="258af9d739589e615f711838a0ff8c58"

User-Agent: LibVLC/2.0.5(LIVE555 Streaming Media v2012.09.13)

Accept: application/sdp

服务器对客户端反馈的response进行校验，通过则返回如下字段：

RTSP/1.0 200 OK

Server: HiIpcam/V100R003 VodServer/1.0.0

Cseq: 3

Content-Type: application/sdp

Cache-Control: must-revalidate

Content-length: 306

Content-Base: rtsp://192.168.123.158:554/11/



v=0

o=StreamingServer 3331435948 1116907222000 IN IP4192.168.123.158

s=\11

c=IN IP4 0.0.0.0

b=AS:1032

t=0 0

a=control:*

m=video 0 RTP/AVP 96

b=AS:1024

a=control:trackID=0

a=rtpmap:96 H264/90000

a=fmtp:96 packetization-mode=1;sprop-parameter-sets=Z0LgHtoCgPRA,aM4wpIA=

a=framesize:96 640-480

然后，客户端发起建立连接请求（用同样的方法计算response）：

SETUP rtsp://192.168.123.158:554/11/trackID=0 RTSP/1.0

CSeq: 4

Authorization: Digest username="admin", realm="HipcamRealServer", nonce="3b27a446bfa49b0c48c3edb83139543d",uri="rtsp://192.168.123.158:554/11/", response="7251f3cd9dec6d89fc948e4c50e0b1cf"

User-Agent: LibVLC/2.0.5(LIVE555 Streaming Media v2012.09.13)

Transport:RTP/AVP;unicast;client_port=4074-4075

服务器端验证客户端返回的response字段，通过则返回通信参数：

RTSP/1.0 200 OK

Server: HiIpcam/V100R003 VodServer/1.0.0

Cseq: 4

Session: 430786884314920

Cache-Control: must-revalidate

Transport: RTP/AVP;unicast;mode=play;source=192.168.123.158;client_port=4074-4075;server_port=5000-5001;ssrc=542289ec

最后，客户端发起播放请求(同样需计算response字段)：

PLAY rtsp://192.168.123.158:554/11/ RTSP/1.0

CSeq: 5

Authorization: Digest username="admin", realm="HipcamRealServer", nonce="3b27a446bfa49b0c48c3edb83139543d",uri="rtsp://192.168.123.158:554/11/",response="9bfb25badcf52e6826cae5688e26cf6d"

User-Agent: LibVLC/2.0.5(LIVE555 Streaming Media v2012.09.13)

Session: 430786884314920

Range: npt=0.000-

服务器端校验response字段，通过则返回视频数据。



**说明：**

**1. 请求头字段说明**

例如：OPTIONS rtsp://192.168.123.158:554/11RTSP/1.0

**字段依次包含：public_method,uri地址，协议版本**



**2. response字段的计算**

RTSP客户端应该使用username + password并计算response如下:

(1)当password为MD5编码,则

response = md5(password:nonce:md5(public_method:url));

(2)当password为ANSI字符串,则

response= md5(md5(username:realm:password):nonce:md5(public_method:url));

客户端在每次发起不同的请求方法时都需要计算response字段，同样在服务器端校验时也默认采取同样的计算方法。

### 2. RTSP Digest Authentication报文例子

下面是VLC请求RTSP 服务器的交互报文（URL：rtsp://**admin:admin**@192.168.60.62/defaultPrimary?streamType=u）：




OPTIONS rtsp://192.168.60.62:554/defaultPrimary?streamType=u RTSP/1.0
CSeq: 3
Authorization: Digest username="admin", realm="Avigilon-13415614", nonce="1N9A1isAlc05nlS4XuSpZ523rA5tC280mG80upbf050000", uri="rtsp://192.168.60.62:554/defaultPrimary?streamType=u", response="2c514169823e91d57368baff11ebfef8"
User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)

RTSP/1.0 200 OK
Server: AvigilonOnvifNvt/2.4.4.38
Authentication-Info: qop=auth, rspauth="969d1dbab73198a859f41574cf5681fe", cnonce="", nc=00000000
CSeq: 3
Content-Length: 0
Date: Sat, 01 Jan 2011 00:24:27 GMT
Public: OPTIONS, DESCRIBE, SETUP, SET_PARAMETER, GET_PARAMETER, PLAY, PAUSE, TEARDOWN




DESCRIBE rtsp://192.168.60.62:554/defaultPrimary?streamType=u RTSP/1.0
CSeq: 4
Authorization: Digest username="admin", realm="Avigilon-13415614", nonce="1N9A1isAlc05nlS4XuSpZ523rA5tC280mG80upbf050000", uri="rtsp://192.168.60.62:554/defaultPrimary?streamType=u", response="45633280f1d55bf1810cb579180d2265"
User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)
Accept: application/sdp

RTSP/1.0 200 OK
Server: AvigilonOnvifNvt/2.4.4.38
Authentication-Info: qop=auth, rspauth="969d1dbab73198a859f41574cf5681fe", cnonce="", nc=00000000
CSeq: 4
Content-Length: 668
Content-Type: application/sdp
Date: Sat, 01 Jan 2011 00:24:27 GMT


v=0
o=- 1293841467625 1293841467625 IN IP4 192.168.60.62
s=Live
t=0 0
m=audio 0 RTP/AVP 0
c=IN IP4 192.168.60.62
a=control:rtsp://192.168.60.62/defaultPrimary/mic0/trackID=1
m=video 0 RTP/AVP 96
c=IN IP4 192.168.60.62
a=control:rtsp://192.168.60.62/defaultPrimary/cam0/trackID=2
a=fmtp:96 packetization-mode=0; profile-level-id=42A01E; sprop-parameter-sets=Z00AKZpmA8ARPy4C1AQEBQAAAwPoAADqYOhlBQu8uNDKChd5cKA=,aO48gA==
a=rtpmap:96 H264/90000
a=x-avg-params:96 source-height=1080; source-width=1920
m=application 0 RTP/AVP 98
c=IN IP4 192.168.60.62
a=control:rtsp://192.168.60.62/defaultPrimary/metadata0/trackID=1
a=rtpmap:98 vnd.onvif.metadata/90000


SETUP rtsp://192.168.60.62/defaultPrimary/mic0/trackID=1 RTSP/1.0
CSeq: 5
Authorization: Digest username="admin", realm="Avigilon-13415614", nonce="1N9A1isAlc05nlS4XuSpZ523rA5tC280mG80upbf050000", uri="rtsp://192.168.60.62:554/defaultPrimary?streamType=u", response="1bf97fd72be8268c087cc9921ed789de"
User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)
Transport: RTP/AVP;unicast;client_port=61408-61409

RTSP/1.0 200 OK
Server: AvigilonOnvifNvt/2.4.4.38
Authentication-Info: qop=auth, rspauth="969d1dbab73198a859f41574cf5681fe", cnonce="", nc=00000000
CSeq: 5
Session: vJ0dL0GbIR2m63tHIIPv9L;timeout=60
Transport: RTP/AVP;unicast;destination=192.168.120.32;client_port=61408-61409;server_port=51278-51279;ssrc=7fbfa4bd
Content-Length: 0
Date: Sat, 01 Jan 2011 00:24:27 GMT

PLAY rtsp://192.168.60.62:554/defaultPrimary?streamType=u RTSP/1.0
CSeq: 7
Authorization: Digest username="admin", realm="Avigilon-13415614", nonce="1N9A1isAlc05nlS4XuSpZ523rA5tC280mG80upbf050000", uri="rtsp://192.168.60.62:554/defaultPrimary?streamType=u", response="72aa634c21a51ec77c51115d455d6a4b"
User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)
Session: vJ0dL0GbIR2m63tHIIPv9L
Range: npt=0.000-

RTSP/1.0 200 OK
Server: AvigilonOnvifNvt/2.4.4.38
Authentication-Info: qop=auth, rspauth="969d1dbab73198a859f41574cf5681fe", cnonce="", nc=00000000
CSeq: 7
Session: vJ0dL0GbIR2m63tHIIPv9L
Range: npt=now-
RTP-Info: url=rtsp://192.168.60.62/defaultPrimary/cam0/trackID=2;seq=27800;rtptime=2122794782, url=rtsp://192.168.60.62/defaultPrimary/mic0/trackID=1;seq=53177;rtptime=1279412205
Content-Length: 0
Date: Sat, 01 Jan 2011 00:24:27 GMT            


