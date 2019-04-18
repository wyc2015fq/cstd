# RTSP常用命令解释 - 爱写代码的自由人 - CSDN博客





2018年02月26日 10:03:09[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：354








**1. RTSP的常用命令与解释**其中C是客户端，S是服务端。
**1.1  OPTIONS**
       C->S:       OPTION request //询问S有哪些方法可用
       S->C:       OPTION response //S回应信息中包括提供的所有可用方法
使用举例：
客户端到服务端：

OPTIONS rtsp://218.207.101.236:554/mobile/3/67A451E937422331 RTSP/1.0
Cseq: 1

服务端对OPTIONS的回应：

RTSP/1.0 200 OK
Server: PVSS/1.4.8 (Build/20090111; Platform/Win32; Release/StarValley; )
Cseq: 1
Public: DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, OPTIONS, ANNOUNCE, RECORD

**1.2  DESCRIBE**

      C->S:      DESCRIBE request //要求得到S提供的媒体初始化描述信息
      S->C:      DESCRIBE response //S回应媒体初始化描述信息，主要是sdp

使用举例：
客户端到服务端：

DESCRIBE rtsp://218.207.101.236:554/mobile/3/67A451E937422331/8jH5QPU5GWS07Ugn.sdp RTSP/1.0
Cseq: 2

服务端对OPTIONS的回应：

RTSP/1.0 200 OK
Server: PVSS/1.4.8 (Build/20090111; Platform/Win32; Release/StarValley; )
Cseq: 2
Content-length: 421
Date: Mon, 03 Aug 2009 08:21:33 GMT
Expires: Mon, 03 Aug 2009 08:21:33 GMT
Content-Type: application/sdp
x-Accept-Retransmit: our-retransmit
x-Accept-Dynamic-Rate: 1
Content-Base: rtsp://218.207.101.236:554/mobile/3/67A451E937422331/8jH5QPU5GWS07Ugn.sdp/

v=0
o=MediaBox 127992 137813 IN IP4 0.0.0.0
s=RTSP Session
i=Starv Box Live Cast
c=IN IP4 218.207.101.236
t=0 0
a=range:npt=now-
a=control:*
m=video 0 RTP/AVP 96
b=AS:20
a=rtpmap:96 MP4V-ES/1000
a=fmtp:96 profile-level-id=8; config=000001b008000001b5090000010000000120008440fa282c2090a31f; decode_buf=12586
a=range:npt=now-
a=framerate:5
a=framesize:96 176-144
a=cliprect:0,0,144,176
a=control:trackID=1

**1.3  SETUP **
        C->S:        SETUP request //设置会话的属性，以及传输模式，提醒S建立会话
        S->C:        SETUP response //S建立会话，返回会话标识符，以及会话相关信息
客户端到服务端的请求举例：

SETUP rtsp://218.207.101.236:554/mobile/3/67A451E937422331/8jH5QPU5GWS07Ugn.sdp/trackID=1
 RTSP/1.0
Cseq: 3
Transport: RTP/AVP;UNICAST;client_port=16264-16265;mode=play

服务端对客户端的回应举例：

RTSP/1.0 200 OK
Server: PVSS/1.4.8 (Build/20090111; Platform/Win32; Release/StarValley; )
Cseq: 3
Session: 26633092229589
Date: Mon, 03 Aug 2009 08:21:33 GMT
Expires: Mon, 03 Aug 2009 08:21:33 GMT
Transport: RTP/AVP;UNICAST;mode=play;client_port=16264-16265;server_port=20026-20027

**1.4  PLAY**
        C->S:      PLAY request //C请求播放
        S->C:      PLAY response //S回应该请求的信息
客户端到服务端的请求举例:

PLAY rtsp://218.207.101.236:554/mobile/3/67A451E937422331/8jH5QPU5GWS07Ugn.sdp RTSP/1.0
Session: 26633092229589
Cseq: 4

服务端对客户端的回应举例：

RTSP/1.0 200 OK
Server: PVSS/1.4.8 (Build/20090111; Platform/Win32; Release/StarValley; )
Cseq: 4
Session: 26633092229589
RTP-Info: url=rtsp://218.207.101.236:554/mobile/3/67A451E937422331/8jH5QPU5GWS07Ugn.sdp/trackID=1;seq=0;rtptime=0

**1.5  PAUSE**        C->S:      PAUSE request //C请求暂停播放
        S->C:      PAUSE response //S回应该请求的信息
客户端到服务端的请求举例:

PAUSE rtsp://218.207.101.236:554/mobile/3/67A451E937422331/8jH5QPU5GWS07Ugn.sdp/ RTSP/1.0
Cseq: 5
Session: 26633092229589

服务端对客户端的回应举例：

RTSP/1.0 200 OK
Server: PVSS/1.4.8 (Build/20090111; Platform/Win32; Release/StarValley; )
Cseq: 5
Session: 26633092229589

**1.6  TEARDOWN **
        C->S:        TEARDOWN request //C请求关闭会话
        S->C:        TEARDOWN response //S回应该请求
客户端到服务端的请求举例:

TEARDOWN rtsp://218.207.101.236:554/mobile/3/67A451E937422331/8jH5QPU5GWS07Ugn.sdp/ RTSP/1.0
Cseq: 6
User-Agent: RealMedia Player HelixDNAClient/10.0.0.11279 (win32)
Session: 26633092229589



RTSP/1.0 200 OK
Server: PVSS/1.4.8 (Build/20090111; Platform/Win32; Release/StarValley; )
Cseq: 6
Session: 26633092229589
Connection: Close



**2. 一个RTSP客户端和服务器报文交互的例子**

**OPTIONS rtsp://192.168.40.81 RTSP/1.0CSeq: 2User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)RTSP/1.0 200 OKCSeq: 2Public: SETUP,TEARDOWN,OPTIONS,PLAY,ANNOUNCE,DESCRIBE,SET_PARAMETER,DESCRIBE rtsp://192.168.40.81 RTSP/1.0CSeq: 3User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)Accept: application/sdpRTSP/1.0 200 OKCSeq: 3Content-Base: rtsp://192.168.40.81Content-Length: 312Content-Type: application/sdpv=0o=- 1001 1 IN IP4 192.168.40.81s=H3C IPC Realtime streamc=IN IP4 192.168.40.81m=video 0 RTP/AVP 105a=control:rtsp://192.168.40.81/media/video1/videoa=rtpmap:105 H264/90000a=fmtp:105 DecoderTag=h3c-v3 RTCP=0m=audio 0 RTP/AVP 0a=fmtp:RTCP=0a=control:rtsp://192.168.40.81/media/video1/audioSETUP rtsp://192.168.40.81/media/video1/video RTSP/1.0CSeq: 4User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)Transport: RTP/AVP;unicast;client_port=54618-54619RTSP/1.0 200 OKCSeq: 4Transport: RTP/AVP/UDP;unicast;destination=192.168.120.32;client_port=54618-54619;server_port=10108-10109;ssrc=FE3C916;mode="PLAY"Session: 3d097a874af97a8722f97a87bcf97a8SETUP rtsp://192.168.40.81/media/video1/audio RTSP/1.0CSeq: 5User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)Transport: RTP/AVP;unicast;client_port=54620-54621Session: 3d097a874af97a8722f97a87bcf97a8RTSP/1.0 200 OKCSeq: 5Transport: RTP/AVP/UDP;unicast;destination=192.168.120.32;client_port=54620-54621;server_port=10110-10111;ssrc=FE3C916;mode="PLAY"Session: 3d097a874af97a8722f97a87bcf97a8PLAY rtsp://192.168.40.81 RTSP/1.0CSeq: 6User-Agent: LibVLC/2.0.5 (LIVE555 Streaming Media v2012.09.13)Session: 3d097a874af97a8722f97a87bcf97a8Range: npt=0.000-RTSP/1.0 200 OKCSeq: 6**

**Session: 3d097a874af97a8722f97a87bcf97a8**

注意：服务器没有设置密码登录认证，所以报文中没有Authorization:字段。



