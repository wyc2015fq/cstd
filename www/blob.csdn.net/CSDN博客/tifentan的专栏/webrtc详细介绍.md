# webrtc详细介绍 - tifentan的专栏 - CSDN博客

2017年09月12日 09:29:00[露蛇](https://me.csdn.net/tifentan)阅读数：2900


自作笔记，来自[https://hpbn.co/webrtc/](https://hpbn.co/webrtc/)，出自一本好书《High Performance 

Browser Networking》

[w3c webrtc文档](https://www.w3.org/TR/webrtc/#simple-peer-to-peer-example)

web层主要接口： 

MediaStream: 采集音视频 

RTCPeerConnection: 传输音视频 

RTCDataChannel: 传输自定义数据

前言一大段废话，强调了一下webrtc使用的udp，但不是普通的udp，在udp上做了很多优化云云。。

## webrtc标准和开发
- Web Real-Time Communications (WEBRTC) W3C Working Group是负责定义浏览器接口部分标准的组织
- Real-Time Communication in Web-browsers (RTCWEB) 是 IETF 工作组，负责定义协议，数据格式，安全，以及一切技术底层。 

webrtc具有很强的扩展性，容易跟其他现有的音视频通讯系统结合。符合现在网速的爆发性增长，很有前瞻性。

## 音视频引擎

1.在发送端，问题点在于应对不断波动的带宽和延迟，对音视频流进行处理。 

2.在接收端，流程是相反的，客户端必须实时解码流，并能够适应网络抖动和延迟延迟。 

总之，捕获和处理音频和视频是一个复杂的问题。 

**音视频获取及处理（getUserMedia）**

媒体捕获和流W3C规范定义了一组新的JavaScript API，使应用程序能够从平台请求音频和视频流，以及一组API来处理和处理所获取的媒体流。（图18-2）mediastream对象是主要的接口。 

- mediastream对象包含一个或多个单独的轨道（mediastreamtrack）。
- 同属于一个mediastream的track间彼此同步。
- 输入源可以是物理设备，如麦克风、网络摄像机或来自用户硬盘或远程网络对等点的本地或远程文件
- mediastream的输出可以被发送到一个或多个目的地：本地video或audio元素，用JavaScript进行后期处理，或发送到远程节点。

我们可以对任意一个track进行操作，可以提出限制来获取适应的音视频源。另外，可以克隆track，修改track等等。

获取到track后，还能与其他web接口进行互动：
- Web Audio API允许在浏览器中处理音频。
- Canvas API 可对单个frame进行处理
- 
CSS3和WebGL API可以对输出流添加各种2D和3D效果。

[Media Capture and Streams APIs](https://www.w3.org/TR/mediacapture-streams/)

目前webrtc默认音频编码格式为opus，视频vp8. 

opus码率：6-510kbit/s 

vp8码率：100-2000kbit/s 

720p at 30 FPS: 1.0~2.0 Mbps 

360p at 30 FPS: 0.5~1.0 Mbps 

180p at 30 FPS: 0.1~0.5 Mbps 

一般一个peer端需要2.5mbps的带宽可进行高清视频聊天。

## 实时网络传输

实时通信是对时间非常敏感的。因此，音视频流应该可以容忍间歇性的分组丢失：音频和视频编解码器可以填充小的虚假数据，通常对输出质量影响极小。另外，应用程序可以传送额外的数据来恢复丢失或延迟的数据包，但是及时性和低延迟比可靠性更重要。 

由于实时性的需要选择udp。

补充说明（有空再看看）： 
[Building Blocks of UDP](https://hpbn.co/building-blocks-of-udp/)

webrtc协议栈： 

ICE: Interactive Connectivity Establishment (RFC 5245) 

STUN: Session Traversal Utilities for NAT (RFC 5389) 

TURN: Traversal Using Relays around NAT (RFC 5766) 

SDP: Session Description Protocol (RFC 4566) 

DTLS: Datagram Transport Layer Security (RFC 6347) 

SCTP: Stream Control Transport Protocol (RFC 4960) 

SRTP: Secure Real-Time Transport Protocol (RFC 3711)

## RTCPeerConnection

RTCPeerConnection 功能：
- 负责整个ICE流程
- 负责发送automatic (STUN) keepalives， 保持peers的连接
- 管理本地媒体流
- 管理远程流
- 触发各种跟媒体流有关的事件
- 提供offer，answer，连接状态查询等API

DataChannel可配置成可靠的，不可靠的，有序的，无序的。

## 建立Peer-to-Peer连接

通知（信令）和初始会话协商的交付留给应用程序 

不展开，可看我其他文章 
[浏览器网络连接](https://hpbn.co/primer-on-browser-networking/#application-apis-and-protocols)

ICE的工作模式有3种：
- 
**交互式ICE**

ICE agent： 

每个RTCPeerConnection都包含一个ICE agent，负责收集本地ip，port对；负责进行peer间的连接检查；负责发送keepalives。

一旦sdp（本地或者远端）设置好，ICE agent开始自动发现本地可能的ip：port对： 

1.获取本地ip 

2.如果有配置，从STUN获取公网ip:port 

3.如果有配置，TURN作为最后的手段，对数据包进行转发。

当一个新的candidate被发现，agent自动添加到RTCPeerConnection，并通过onicecandidate回调。当ICE收集完成，也通过这个回调告诉你。

```
var ice = {"iceServers": [
  {"url": "stun:stun.l.google.com:19302"}, 
  {"url": "turn:turnserver.com", "username": "user", "credential": "pass"} 
]};

var signalingChannel = new SignalingChannel();
var pc = new RTCPeerConnection(ice);

navigator.getUserMedia({ "audio": true }, gotStream, logError);

function gotStream(stream) {
  pc.addStream(stream);

  pc.createOffer(function(offer) {
    pc.setLocalDescription(offer); 
  });
}

pc.onicecandidate = function(evt) {
  if (evt.target.iceGatheringState == "complete") { 
      local.createOffer(function(offer) {
        console.log("Offer with ICE candidates: " + offer.sdp);
        signalingChannel.send(offer.sdp); 
      });
  }
}

...

// Offer with ICE candidates:
// a=candidate:1862263974 1 udp 2113937151 192.168.1.73 60834 typ host ... 
// a=candidate:2565840242 1 udp 1845501695 50.76.44.100 60834 typ srflx ...
```

这种ICE等candidates流程完成后才发送sdp。

双方拿到sdp时，同时也拿到candidates，就开始用这些candidates的ip：port对尝试连接。对每个ip:port对，agent发送STUN binding request到对端，对端要回STUN response。一旦收到STUN response，就代表连接成功了。 

连接成功后，会周期性的发送 binding request，作为心跳包。

一个turn binging例子： 
![这里写图片描述](https://hpbn.co/assets/diagrams/a2813f1c382a12a4b0b0b229ca22ee9f.png)
- **渐进式ICE**

跟上面的很不一样，交换sdp不需要带上candidates，candidates发现一个就发送一个，存在一个新的ip：port对就可以立刻尝试连接。

```java
var ice = {"iceServers": [
  {"url": "stun:stun.l.google.com:19302"},
  {"url": "turn:turnserver.com", "username": "user", "credential": "pass"}
]};

var pc = new RTCPeerConnection(ice);
navigator.getUserMedia({ "audio": true }, gotStream, logError);

function gotStream(stream) {
  pc.addStream(stream);

  pc.createOffer(function(offer) {
    pc.setLocalDescription(offer);
    signalingChannel.send(offer.sdp); 
  });
}

pc.onicecandidate = function(evt) {
  if (evt.candidate) {
    signalingChannel.send(evt.candidate); 
  }
}

signalingChannel.onmessage = function(msg) {
  if (msg.candidate) {
    pc.addIceCandidate(msg.candidate); 
  }
}
```

pc.addIceCandidate(msg.candidate)这语句注册一个对面的ip：port对，并开始尝试连接操作。
- **跟踪ICE流程跟连接状态**

RTCPeerConnection 内置的ICE agent完成candidate发现、连接尝试、心跳功能，这本来都是透明的，我们没必要关心内部的状态。但是我们依然可以获取agent的状态。

```java
var ice = {"iceServers": [
  {"url": "stun:stun.l.google.com:19302"},
  {"url": "turn:turnserver.com", "username": "user", "credential": "pass"}
]};

var pc = new RTCPeerConnection(ice);

logStatus("ICE gathering state: "  + pc.iceGatheringState); 
pc.onicecandidate = function(evt) { 
   logStatus("ICE gathering state change: " + evt.target.iceGatheringState);
}

logStatus("ICE connection state: " + pc.iceConnectionState); 
pc.oniceconnectionstatechange = function(evt) { 
  logStatus("ICE connection state change: " + evt.target.iceConnectionState);
}
```

iceGatheringState有3个状态：
- new 刚创建，还没网络信息
- gathering  正在获取本地candidates
- complete  已经完成所有candidates获取

iceConnectionState 有7个状态：
- new  正在获取本地candidates，也在等待对端的candidates
- checking  正在尝试连接，起码收到一个对端的candidate，但同时可能还在获取candidates中
- connected 已经找到一个连接是有效的，但还在尝试其他的。
- completed 已经确定一个将用来使用的连接。
- failed 所有candidate都尝试过都失败，可能试试turn？
- disconnected 心跳包失败？可能自己内部解决。
- closed      表示    ICE agent关闭

由于p2p的不可靠天性，连接过程中重连可能经常出现。即使已经连上了，ICE  agent也有可能周期性的检查网络，寻找跟好的网络连接替代品。

谷歌有个webrtc的跟踪工具  chrome://webrtc-internals 

能看到动作流，视频流信息等等。

**结合在一起**

1.初始化一个webrtc连接

```xml
<video id="local_video" autoplay></video> 
<video id="remote_video" autoplay></video> 

<script>
  var ice = {"iceServers": [
    {"url": "stun:stunserver.com:12345"},
    {"url": "turn:turnserver.com", "username": "user", "credential": "pass"}
  ]};

  var signalingChannel = new SignalingChannel(); 
  var pc = new RTCPeerConnection(ice); 

  navigator.getUserMedia({ "audio": true, "video": true }, gotStream, logError); 

  function gotStream(evt) {
    pc.addStream(evt.stream); 

    var local_video = document.getElementById('local_video');
    local_video.src = window.URL.createObjectURL(evt.stream); 

    pc.createOffer(function(offer) { 
      pc.setLocalDescription(offer);
      signalingChannel.send(offer.sdp);
    });
  }

  pc.onicecandidate = function(evt) { 
    if (evt.candidate) {
      signalingChannel.send(evt.candidate);
    }
  }

  signalingChannel.onmessage = function(msg) { 
    if (msg.candidate) {
      pc.addIceCandidate(msg.candidate);
    }
  }

  pc.onaddstream = function (evt) { 
    var remote_video = document.getElementById('remote_video');
    remote_video.src = window.URL.createObjectURL(evt.stream);
  }

  function logError() { ... }
</script>
```

一旦建立连接，仍然可以添加或删除rtcpeerconnection的streams。每次发生这种情况，SDP协商流程重现，重复与初始化相同的流程。

**2.应答一个webrtc连接**

```xml
<video id="local_video" autoplay></video>
<video id="remote_video" autoplay></video>

<script>
  var signalingChannel = new SignalingChannel();

  var pc = null;
  var ice = {"iceServers": [
    {"url": "stun:stunserver.com:12345"},
    {"url": "turn:turnserver.com", "username": "user", "credential": "pass"}
  ]};

  signalingChannel.onmessage = function(msg) {
    if (msg.offer) { 
      pc = new RTCPeerConnection(ice);
      pc.setRemoteDescription(msg.offer);

      pc.onicecandidate = function(evt) {
        if (evt.candidate) {
          signalingChannel.send(evt.candidate);
        }
      }

      pc.onaddstream = function (evt) {
        var remote_video = document.getElementById('remote_video');
        remote_video.src = window.URL.createObjectURL(evt.stream);
      }

      navigator.getUserMedia({ "audio": true, "video": true },
        gotStream, logError);

    } else if (msg.candidate) { 
      pc.addIceCandidate(msg.candidate);
    }
  }

  function gotStream(evt) {
    pc.addStream(evt.stream);

    var local_video = document.getElementById('local_video');
    local_video.src = window.URL.createObjectURL(evt.stream);

    pc.createAnswer(function(answer) { 
      pc.setLocalDescription(answer);
      signalingChannel.send(answer.sdp);
    });
  }

  function logError() { ... }
</script>
```

## 发送媒体数据

为了媒体流的质量，不能直接使用udp裸流。
- DTLS 传输层加密
- SRTP 用来传输音频和视频流
- SCTP 用于传输应用数据，媒体流控制？

**DTLS**
[TLS](https://hpbn.co/transport-layer-security-tls/#tls-handshake)只适用tcp，DTLS可以用于udp。 

有点复杂，有空看一下

**SRTP and SRTCP**

从低的码率（＜500 Kbps）开始调整流的质量匹配可用的带宽。 

webrtc可能由于网络情况降低视频质量，但不会反过来把视频质量提高到比你约束的高。

Secure Real-time Transport Protocol (SRTP) 

Secure Real-time Control Transport Protocol (SRTCP) 

rtp/rtcp安全版 RFC 3550 

RFC 3711 

就其本身而言，SRTP不提供任何机制或保证及时性，可靠性，或传送的数据错误恢复。相反，它只是用附加的metadata来帮助接收方处理每个流。 

SRTCP协议跟踪SRTP发送的字节和数据包的丢失，最后收到的序列号，每个数据包到达间隔抖动，和其他统计。然后，周期性地，两个节点交换这些数据，并使用它来调整每个流的发送速率、编码质量和其他参数。

为了适配webrtc，SRTP,SRTCP也做出一些调整。 

SRTP,SRTCP有加密的机制，但没交换钥匙的功能，这就是为什么需要DTLS。 

SRTP,SRTCP各有端口，但是webrtc里面利用一个附加的复用拓展将多个流和控制通道整合在一个端口里。 

IETF的工作组也正在开发新的拥塞控制算法，利用SRTCP的反馈优化webrtc的音频和视频流。

**使用SCTP传输应用数据**

RTCDataChannel 使用SCTP来传送，基于DTLS协议上。 

 Stream Control Transmission Protocol (SCTP)

应用数据协议要求： 

传输必须支持多个独立信道的多路复用。 

每个通道必须支持顺序或非顺序交付。 

每个通道必须支持可靠或不可靠的交付。 

每个通道可能具有应用程序定义的优先级级别。 

每个应用程序的消息可以分割和重新组装。 

传输必须实现拥塞控制机制。 

传输必须保证数据的机密性和完整性。

最后两项由DTLS支持，其他的SRTCP能满足。
|协议|TCP|UDP|SCTP|
|----|----|----|----|
|可靠性|是|否|可配置|
|顺序到达|是|否|可配置|
|传输|字节流|报文|报文|
|流控制|是|否|是|
|拥塞控制|是|否|是|

SCTP是一种TCP、UDP同级的传输协议，可以直接在IP协议上面运行。然而，在WebRTC中，SCTP运行在一个安全的DTLS上，DTLS又基于UDP上。 

一些SCTP的术语：

Association 

连接. 

Stream 

默认单向的，有序的报文流，也可以配置成无序的。 

Message 

提交给协议的用户数据. 

Chunk 

SCTP包，传送的最小单位.

单一的SCTP通道可以携带多个独立的数据流（Stream），Message可以分为多个Chunk，通过 SCTP packets传输，在接收方重组。 

一个SCTP包由一个共同的头和一个或多个控制或数据块组成。头有12字节，包括源和目的端口，一个随机生成的验证码，一个整个数据包的校验和。头后面是一个或多个控制或数据块；前面的图是一个只有一个数据块的SCTP包： 

Type: 固定0 

U: 是否无序 

B和E: 标识被分割成多个chunk的message的状态。B = 1，E = 0表示一个消息的第一个片段；B = 0，E = 0表示中段；B = 0，E = 1表示最后的片段；B = 1，E = 1表明一个完整的message。 

Length:  DATA chunk（数据块）的长度。包括16字节的块头加payload。 

Transmission sequence number (TSN)： 序列号 

Stream identifier： 流标识 

Stream sequence number ： 流的序列号，被分包的message同一个流序列号 

Payload protocol identifier (PPID)： payload的自定义协议号，例如0×51 for UTF-8 and 0×52 for binary application payloads.

SCTP怎么开始协商通讯？每一个SCTP连接需要一个类似TCP的handshake sequence！同样，SCTP也实现了TCP类似的拥塞控制机制和窗口管理机制，两个协议使用同一个初始窗口大小？ 
[Building Blocks of TCP](https://hpbn.co/building-blocks-of-tcp/)

为了适配webrtc,SCTP需要进行拓展：

1.SCTP标准（RFC 4960）能提供无序的消息传递，但不保证可靠性（有序的保证吗？）。为了解决这个问题，“部分可靠性扩展”（RFC 3758），它扩展了SCTP协议允许支持一定的可靠性？这是DataChannel的关键特征。

2.SCTP不支持优先级管理。可在上层自己实现。

3.SCTP本可以裸上，不需要叠加到udp跟dtls上，但是现有的交换机多数支持不好。

## DataChannel

一旦rtcpeerconnection建立，连接节点可以打开一个或多个DataChannel交换文本或二进制数据

```
function handleChannel(chan) { 
  chan.onerror = function(error) { ... }
  chan.onclose = function() { ... }

  chan.onopen = function(evt) {
    chan.send("DataChannel connection established. Hello peer!")
  }

  chan.onmessage = function(msg) {
    if(msg.data instanceof Blob) {
      processBlob(msg.data);
    } else {
      processText(msg.data);
    }
  }
}

var signalingChannel = new SignalingChannel();
var pc = new RTCPeerConnection(iceConfig);

var dc = pc.createDataChannel("namedChannel", {reliable: false}); 

... 

handleChannel(dc); 
pc.ondatachannel = handleChannel;
```

可以看出跟[websocket](https://hpbn.co/websocket/#websocket-api)很像的，包括回调api，但是也能看出一些不同：
- 不同于websocket构造需要一个url，DataChannel 需要一个RTCPeerConnection对象.
- peer两端都可以发起通讯通道
- DataChannel 可配置可靠性和有序性。
|协议|WebSocket|DataChannel|
|----|----|----|
|可靠性|是|可配置|
|顺序到达|是|可配置|
|传输|消息流|消息流|
|加密|可配置|一定有|
|可复用|否（可拓展）|是|
|二进制数据|是|是|
|UTF-8|是|是|
|压缩|否（可拓展）|否|

**设置和协商**

在sdp里面描述与传输有关的信息：

```
(... snip ...)
m=application 1 DTLS/SCTP 5000 
c=IN IP4 0.0.0.0 
a=mid:data
a=fmtp:5000 protocol=webrtc-datachannel; streams=10 
(... snip ...)
```

0.0.0.0表示使用ICE。

也可以创建一个单纯传输数据的RTCPeerConnection 

```
var signalingChannel = new SignalingChannel();
var pc = new RTCPeerConnection(iceConfig);

var dc = pc.createDataChannel("namedChannel", {reliable: false}); 

var mediaConstraints = { 
  mandatory: {
      OfferToReceiveAudio: false,
      OfferToReceiveVideo: false
  }
};

pc.createOffer(function(offer) { ... }, null, mediaConstraints); 

...
```

但可以看出sdp中并没有每个DataChannel的参数设定（数据类型、可靠性、使用的应用协议和其他参数），这些设定是通过一个DATA_CHANNEL_OPEN 信息发送的： 

DATA_CHANNEL_OPEN 有点像[HTTP / 2的消息头](https://hpbn.co/http2/#initiating-a-new-stream)，DataChannel协议可参考[http://tools.ietf.org/html/draft-jesup-rtcweb-data-protocol](http://tools.ietf.org/html/draft-jesup-rtcweb-data-protocol)

（通过什么途径传送这个DATA_CHANNEL_OPEN ，信令机制还是DataChannel本身？看下面有个通过信令机制交换channel参数的例子，这说明DATA_CHANNEL_OPEN 应该是DataChannel内部交换的）

Out-of-Band Channel 协商： 

你可以在createDataChannel 时指定negotiated参数为true,他就会跳过DATA_CHANNEL_OPEN消息，两个端点也必须指定相同的id，否则该参数将自动由浏览器生成：

```java
signalingChannel.send({ 
  newchannel: true,
  label: "negotiated channel",
  options: {
    negotiated: true,
    id: 10, 
    reliable: true,
    ordered: true,
    protocol: "appProtocol-v3"
  }
});

signalingChannel.onmessage = function(msg) {
  if (msg.newchannel) { 
    dc = pc.createDataChannel(msg.label, msg.options);
  }
}
```

以上通过信令机制发送channel参数，可以看到id是10，收到对方的channel信息后才使用这些信息createDataChannel。 

peers少的情况下不建议这样启动channel，但是peers很多的情况下，通过信令服务器发送给所有的peers会比较好。

**设置message的可靠性和顺序到达**

可靠性可要求严格的可靠性或者是部分可靠性。 

在要求部分可靠的情况下，这还能有两种不同的配置：
- 指定可重发的最大次数
- 指定可重发的超时时间
|配置|顺序|可靠|部分可靠性策略|
|----|----|----|----|
|有序+可靠|是|是|n/a|
|无序+可靠|否|是|n/a|
|有序+部分可靠（计数）|是|部分|最大次数|
|无序+部分可靠（计数）|否|部分|最大次数|
|有序+部分可靠（计时）|是|部分|超时|
|无序+部分可靠（计时）|否|部分|超时|

```
conf = {}; 
conf = { ordered: false };  
conf = { ordered: true,  maxRetransmits: customNum };  
conf = { ordered: false, maxRetransmits: customNum };  
conf = { ordered: true,  maxRetransmitTime: customMs };  
conf = { ordered: false, maxRetransmitTime: customMs };  

conf = { ordered: false, maxRetransmits: 0 };  

var signalingChannel = new SignalingChannel();
var pc = new RTCPeerConnection(iceConfig);

...

var dc = pc.createDataChannel("namedChannel", conf);  

if (dc.reliable) {
  ...
} else {
  ...
}
```

**Partially Reliable Delivery and Message 大小**

注意不要在部分可靠传输比较大的message，一丢包就丢很多数据。

## webrtc使用场景

**提高性能的做法**

信令服务：
- 低延迟协议最好
- 提供足够的容量
- 在连接后建议使用DataChannel 代替

防火墙及NAT
- 在初始化RTCPeerConnection时提供一个STUN服务器
- 尽可能使用 trickle ICE（渐进式），即使通信量会多，但建立连接的速度更快
- 提供一个TURN作为p2p连接失败的后备
- 注意TURN的资源配备

数据布局
- 考虑使用一个supernode或者一个专用的中转服务器。
- 在中转服务器上做一些数据处理。

数据约束
- 对音视频进行格式，分辨率等等约束
- DataChannel的二进制数据可进行优化压缩后再传输
- 使用UTF-8发送DataChannel数据
- 可以根据DataChannel的缓冲来调整发送量。

可靠性和有序
- 使用无序传输避免线头阻塞
- 如果使用有序，请减小message，这样尽量避免线头阻塞
- 发送小的message，这样不容易丢包太多。
- 根据数据优先级，延迟，message大小来设置部分可靠传输的最大重传测试和最大重传时间。

