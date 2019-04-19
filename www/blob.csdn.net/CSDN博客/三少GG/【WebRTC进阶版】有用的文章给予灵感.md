# 【WebRTC进阶版】有用的文章给予灵感 - 三少GG - CSDN博客
2011年10月24日 13:07:25[三少GG](https://me.csdn.net/scut1135)阅读数：4547标签：[视频会议																[codec																[android																[standards																[浏览器																[video](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=standards&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=视频会议&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
**1. Ericsson的浏览器应用示例**
[http://webrtc.labs.ericsson.net.sixxs.org/](http://webrtc.labs.ericsson.net.sixxs.org/)
爱立信官方重要网站，有示例，可细细研究：
[https://labs.ericsson.com/apis/web-real-time-communication/](https://labs.ericsson.com/apis/web-real-time-communication/)
**2.  优秀代码框架分析文章：**
**1)**[http://my.oschina.net/linuxred/blog/23576](http://my.oschina.net/linuxred/blog/23576)
1. 包含了使用STUN、ICE、TURN、RTP-over-TCP的关键NAT和防火墙穿越技术，并支持代理。
2. 构建在浏览器中，WebRTC通过提供直接映射到PeerConnection的信号状态机来抽象信号处理。Web开发人员因此可以选择适合应用场景的协议（例如：SIP、XMPP/Jingle等等）。[](http://my.oschina.net/linuxred/blog/23576)
**2) **[http://bbs.rosoo.net/thread-6196-1-1.html](http://bbs.rosoo.net/thread-6196-1-1.html)      流媒体开发论坛！[http://bbs.rosoo.net/forum-12-1.html](http://bbs.rosoo.net/thread-6196-1-1.html)
 WebRTC的video_engine系统架构图，如下：
      1. WebRTC提供了一个框架，对于视频部分而言，已经解决了从视频采集、编码、传输、显示的全部功能。
     在架构图中，有颜色的箭头表示了视频流的数据流向：从视频采集端开始，经过网络传输层，再到视频接收端。
      2. WebRTC的系统限制：
Device Manager最大可以管理10个输入设备；ChannelManager最大可以管理4个Channel；当然，你也可以修改这些最大值的。
Codec支持的最大分辨率是1920*1200。
      3. WebRTC的扩展
WebRTC不是一个封闭的系统，相反，它很容易扩展，包括Device，Codec,Render，Transport，都提供了可扩展的接口。
比如，可以增加H264的Codec，以方便加入对现有系统的支持。
对于视频会议，可以扩展网络传输层：当从视频采集端得到编码后的数据包时，直接通过视频会议的网络传输，由视频会议服务器发送到各个视频会议客户端，再交给WebRTC进行接收处理。这就达到了我们想要的功能。
3)   一个不错的ppt
# WebRTC - @eCommConf 2011  [http://www.slideshare.net/loopingrage/webrtc-ecommconf-2011](http://www.slideshare.net/loopingrage/webrtc-ecommconf-2011)
4[)** Who are we building WebRTC for?**](http://nextblitz.com/blog/webrtc/)
Not traditional carriers and vendors, not standards optimized for different use cases (SIP, H.323), not extinct business models. Need to aim ahead of the bird and this bird is flying quite quickly.
**3. 形势进展评论**
**1).[http://newphonesout.com/news/google-has-released-video-chat-source-code-looks-very-cool.html](http://newphonesout.com/news/google-has-released-video-chat-source-code-looks-very-cool.html)**
 Google wants third-party developers to use the allegation and video engines to achieve blubbering applications that can be run again from aural a browser. Global IP Solutions has complete WebRTC-based adjustable admirers for Android, Windows Adjustable
 and the[iPhone](http://newphonesout.com/news/apple-iwork-brought-into-iphones-really.html). Ericsson Labs complete a videoconference antecedent with the technology as well.
**2)**.[**596**
 results Found for "**WebRTC**".](http://tweets.seraph.me.sixxs.org/search/WebRTC)
发现浏览器这么普通的架构居然能够支撑这么多的应用，应该说是大部分的计算机应用。一方面是HTTP的精简，另一方面是HTML/CSS的强大表现能力和javascript的强大操控和通信能力。最近浏览器又打算集成webrtc，HTML5标准未来将支持video conference和P2P的直接通信。 (0 RT 0 Reply)
**4.  webrtc-->android  编译文章**
**1)**
## [Change android makefile to build webrtc for android/x86 (Closed)](http://webrtc-codereview.appspot.com.sixxs.org/70001/)不错的网站，有最新最新的code进展！
**2)**
**[build webrtc for android done!but run has error.](http://groups.google.com/group/discuss-webrtc/msg/b4a97baaf6b0874f?)**
ps: 有问题可直接给google开发人员发邮件。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
或许可以参照的一个android应用
http://code.google.com.sixxs.org/p/android-video-conference/  估计质量不高，可忽略。
