# WEBRTC基本介绍 - DoubleLi - 博客园






**“WebRTC，名称源自网页实时通信（Web Real-Time Communication）的缩写，是一个支持网页浏览器进行实时语音对话或视频对话的技术，是谷歌2010年以6820万美元收购Global IP Solutions公司而获得的一项技术，Google开源了这项技术！”**

就 冲着6000多万美金的技术，就很值得研究一下！WebRTC提供了视频会议的核心技术，包括音视频采集、编解码、网络传输、显示等功能，并且还支持跨平 台：Windows，Linux，Mac，Android，尤其是在实时通信方面的很多技术可以灵活嫁接到非RTC类流媒体系统中，例如，在低带宽下的视 频抖动控制，图像效果增强，通用NAT穿透过程等等（后面咱们再具体细说如何嫁接），那么这就需要我们对WEBRTC的整体流程以及深层次的框架进行熟悉 和掌控，这一篇里，我们先通过浏览器端WEBRTC视频通话过程，了解WEBRTC浏览器基本接口和一次RealTime视频通话的流程。

![](http://www.kwstu.com/Content/uploadFiles/images/20140511212859140.jpg)



WEBRTC结构

　　首先，从WEB-RTC方面分析WEBRTC在浏览器上的接口结构，在浏览器端，WEBRTC主要实现了三个接口
- MediaStream， 实现对本地音视频资源的封装，例如从Camera、Microphone、远端Stream等等，MediaStream表示一个媒体数据流，一个 MediaStreamTrack表示MediaStream中的一个媒体源，如音频、视频、字幕等等。
- RTCPeerConnection，语音或者视频通话过程，内部涵盖呼叫、应答、穿透、加密、传输及会话管理等一系列流程，一个RTCPeerConnection代表一对通话过程中的一端。
- RTCDataChannel，在PeerConnection之上，传输自定义数据。



**#MediaStream**

在浏览器端，MediaStream接口名称为getUserMedia， 该接口为上层提供同步的音视频流，比如在本地媒体资源获取的时候，一路MediaStream可以是一路本地Camera提供的视频Track与一路本地 Microphone提供的音频Track经过同步后的Stream。当然，在浏览器端获取到音视频数据后，可以做本地化的各种处理，例如抓图、图像样式 变化、本地显示滤镜等等。





**#RTCPeerConnection**

RTCPeerConnection主要是用来处理点到点之间的连接和数据 传输，使整个过程能够稳定且高效。在RTCPeerConnection下，封装了大量的编解码、通信协议的工作来实现整个实时通信过程，甚至是在不能提 供稳定带宽情况下的实时通信，主要功能点包括：


- 丢包补偿(packet loss concealment)
- 回音消除(echo cancellation)
- 自适应带宽(bandwidth adaptivity)
- 视频抖动缓冲器(dynamic jitter buffering)
- 自动增益控制(automatic gain control)
- 噪声降低和抑制(noise reduction and suppression)
- 图像清理(image 'cleaning')(!暂且这么翻译吧)



**#RTCDataChannel**

RTCDataChannel提供了在RTCPeerConnection 之上交换自定义数据的方法，相比于流媒体数据，在PeerConnection上传输自定义数据，不仅是在量上，而且在可靠性、安全性、灵活性方面，远能 够满足需求。这样在开发基于音视频的游戏和应用上，提供了较大的方便。



WEBRTC组件



**#Transport/Session**
- 完整的RTP/SRTP协议栈
- STUN、TURN、ICE过程
- Session管理机制。

**#VoiceEngine**

**WebRTC极具价值的技术之一，支持722，PCM，ILBC，ISAC等编码，在VoIP上，技术业界领先!**

> 
NetEQ算法：自适应抖动控制算法以及语音包丢失隐藏算法。使其能够快速且高解析度地适应不断变化的网络环境，确保音质优美且缓冲延迟最小。能够有效的处理由于网络抖动和语音包丢失时候对语音质量产生的影响。NetEQ也是WebRTC中一个极具价值的技术，对于提高VoIP质量有明显效果，加以AEC（回声消除）\NR（噪声抑制）\AGC等模块集成使用，效果更好。


**#VideoEngine**

> 
VP8视频图像编解码器，是WebRTC视频引擎的默认的编解码器，VP8适合实时通信应用场景，因为它主要是针对低延时而设计的编解码器。 

视频抖动缓冲器，可以降低由于视频抖动和视频信息包丢失带来的不良影响。

图像质量增强模块对网络摄像头采集到的图像进行处理，包括明暗度检测、颜色增强、降噪处理等功能，用来提升视频质量。










