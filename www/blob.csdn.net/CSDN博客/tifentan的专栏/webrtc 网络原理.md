# webrtc 网络原理 - tifentan的专栏 - CSDN博客





2017年09月11日 18:23:32[露蛇](https://me.csdn.net/tifentan)阅读数：776








本文章来自 
[https://www.html5rocks.com/en/tutorials/webrtc/infrastructure/](https://www.html5rocks.com/en/tutorials/webrtc/infrastructure/)

不一一翻译，捡我认为重要的记录下来。
1.会话的发起机制，webrtc没定义，需自行定义。可参考网上的很多在线例子，房间号的形式实现。

2.信令的交换机制，webrtc没定义，也是自行定义。主要完成网络ip地址等网络信息交换，媒体信息sdp交换。整个流程遵循这个JSEP, [the JavaScript Session Establishment Protocol](https://tools.ietf.org/html/draft-ietf-rtcweb-jsep-03#section-1.1)。

3.信令交互流程大概如下：
- offer 提供一个sdp给另外一方
- answer 另外一方回复一个sdp
- candidates 整个流程中双方发送自己的外网地址信息给对方，并持续进行尝试连接，看意思是发起方在offer完后就开始发送candidate给对面，由发送方尝试连接？

4.信令服务的构建可通过以下传输技术：
- EventSource API 单向web server到client的异步传输，可通过其他手段（XHR？）构建双向信息通道。
- WebSocket全双工完美兼容。
- Google App Engine Channel API 

大规模的可参考：
- XMPP
- ZeroMQ
- 有商业云直接用websocket

5.有一个基于node,socket-io的完整例子

6.RTCDataChannel 可在连接完成后代替信令机制以减轻服务压力

7.现成的信令服务器
- webRTC.io
- easyRTC
- Signalmaster
- 爱立信的[php建的服务器](https://www.ericsson.com/blog/a-web-rtc-tutorial)

8.成熟的供应商 

 vLine, OpenTok and Asterisk.

9.在信令过程后，使用ice打洞

10.ice机制原理
- 先使用网卡本地地址
- 不行使用stun
- 再不行就turn转发 

所有turn支持stun 
[ice测试页面](https://webrtc.github.io/samples/src/content/peerconnection/trickle-ice/)
11.stun原理 

主要是用户发送包到服务器，服务器返回你的公网地址，然后你通过信令机制发送给你要连接的对方。

12.turn原理 

纯转发

13.webrtc多播 

Multipoint Control Unit (MCU) 

开源mcu： 

licode  

mantis
14.跟Voip，电话系统，视频会议系统结合
- SIP  VoIP和视频会议使用的信令协议，需要代理服务器来处理信令转换，但一旦建立后能直接通过p2p传输音视频SRTP
- PSTN 公共电话交换网络， 代理服务器一样要
- XMPP im系统，libJingle是webrtc的前身，就是在xmpp上的拓展，为了音视频通讯，处理手段同上

完成以上转换可用的库（某些直接在客户端使用也行吧）：
- sipML5 ：一个开源的javascript sip 客户端
- jsSIP： javascript SIP 库
- Phono ：开源javascript phone api，作为一个plugin？
- Zingaya：一个嵌入式phone widget
- Twilio：音频 和 信息
- Uberconference：会议库
- webrtc2sip ： 一个网关
- OpenBTS：是一个基于软件的GSM接入点，允许标准的GSM兼容手机作为SIP端点在IP电话（VoIP）网络中使用






