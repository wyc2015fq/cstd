# webrtc doubango linphone - DoubleLi - 博客园






1、doubango官网：[http://www.doubango.org/](http://www.doubango.org/)

2、doubango是一个开源的VOIP基础平台， 并能用于嵌入式和桌面系统的开源框架，该[框架](http://baike.baidu.com/view/66971.htm)使用ANSCI-C编写，具有很好的可移植性。

3、根据doubango的框架、特点与linphone的对比，显然doubango更适合做视频会议系统和视频会议终端的选择，理由？根据linphone的rtp会话管理可见只适合2方进行视频通话，如要支持多方（>3）视频会议，需MCU配合server进行音视频转发和mix，linphone对于nat穿透的方案只有stun和转发，linphone适合更适合当做一款网络电话终端供普通网民当做视频电话用。

4、doubango包含了 SIP/IMS (VoIP) 的客户端、服务器端的组件。


|Client-side components| |
|----|----|
|[sipML5](http://sipml5.org/index.html?svn=20)|HTML5 SIP client using [webrtc2sip](http://www.webrtc2sip.org/) Gateway.[Enjoy our live demo »](http://sipml5.org/call.htm?svn=170)|
|[clik2dial](http://click2dial.org/index.html?svn=170)|A complete Click-to-Call Solution using [webrtc2sip](http://www.webrtc2sip.org/) Gateway and [sipML5](http://sipml5.org/call.htm?svn=170).[Enjoy our live demo »](http://click2dial.org/?svn=170)|
|[webrtc4all](http://code.google.com/p/webrtc4all/)|WebRTC extension for Safari, Opera, Firefox and IE.|
|[Boghe](http://code.google.com/p/boghe/)|- SIP video client for **Windows Phone 8** and **Surface Pro**- IMS/RCS Client for Windows XP, Vista, 7 and 8|
|[iDoubs](http://code.google.com/p/idoubs/)|SIP/IMS VideoPhone for iOS (iPhone, iPad and iPod Touch) and MAC OS X.|
|[IMSDroid](http://code.google.com/p/imsdroid/)|SIP/IMS Client for Android.|
|[libSigComp](http://code.google.com/p/libsigcomp/)|Signaling Compression (SigComp) library|
|Server-side components| |
|[webrtc2sip](http://www.webrtc2sip.org/)|Smart SIP and Media Gateway to connect [WebRTC](http://en.wikipedia.org/wiki/WebRTC) endpoints to any SIP-legacy network.[Enjoy our live demo »](http://sipml5.org/call.htm?svn=15)|
|[telepresence](http://code.google.com/p/telepresence/)|Open Source [TelePresence](http://en.wikipedia.org/wiki/Telepresence) system with a smart and powerful [MCU](http://en.wikipedia.org/wiki/Multipoint_Control_Unit). [Enjoy our live demo »](http://conf-call.org/?svn=15)|
|[Flash2IMS](http://code.google.com/p/flash2ims/)|Adobe® Flash® to SIP/IMS Gateway.|





5、doubango的主要特点

 Powerful MCU (Multipoint Control Unit) for audio and video mixing


 Stereoscopic (spatial) 3D and stereophonic audio
 Full (1080p) and Ultra (2160p) HD video up to 120fps
 Conference recording to a file (containers: *.mp4, *.avi, *.mkv or *.webm)
 Smart adaptive audio and video bandwidth management
 Congestion control mechanism
 SIP registrar
 4 SIP transports (WebSocket, TCP, TLS and UDP)
 SA (direct connection to SIP clients) and AS (behind a server, such as Asterisk, reSIProcate,
openSIPS, Kamailio…) modes
 Support for any WebRTC-capable browser (WebRTC demo client at http://conf-call.org/)
 Mixing different audio and video codecs on a single bridge (h264, vp8, h263, mp4v-es,
theora, opus, g711, speex, g722, gsm, g729, amr, ilbc)
 Protecting a bridge with PIN code

 Unlimited number of bridges and participants

 Connecting any SIP endpoint
 Easy interconnection with PSTN
 NAT traversal (Symmetric RTP, RTCP-MUX, ICE, STUN and TURN)
 RTCP Feedbacks (NACK, PLI, FIR, TMMBN, REMB…) for better video experience
 Secure signalling (WSS, TLS) and media (SDES-SRTP and DTLS-SRTP)
 Continuous presence
 Smart algorithm to detect speakers and listeners
 Different video patterns/layouts
 Multiple operating systems (Linux, OS X, Windows…)
 100% open source and free (no locked features)
 Full documentation
 …and many others

    6、**doubango架构图**

![](http://img.blog.csdn.net/20130627165633046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3Bob25lODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**说明：**

tinySAK (Swiss Army Knife): Utilities functions (SHA-1, MD5, HMAC, String, List, Timers, Thread, Mutex, Semaphore, ...)
tinyNET: Networking (DNS, DHCPv4/v6, STUN, TURN, ICE, ENUM, Sockets, ...)
tinyHTTP: HTTP stack (CONNECT, DELETE, GET, HEAD, OPTIONS, PATCH, POST, PUT, TRACE, ...)
tinyXCAP: XCAP stack (AUID manager, URL generator) without XML parser (See Java code for parsers)
tinyIPSec: IPSec SA manager. Useless for Android but you MUST have it
tinySMS: SMS over IP (SM-TL, SM-RL) for IMS/LTE networks
tinySIGCOMP: Signaling Compression
tinySDP: SDP protocol
tinyRTP: RTP/RTCP protocols
tinyMSRP: MSRP protocol (Chat and File Transfer)
tinyMEDIA: Media plugins manager (Audio, video, Codecs, sessions, MSRP, QoS, ...)
tinyDAV(Doubango Audio Video): Media plugins implementation
tinySIP: SIP/IMS stack















最新要做一个移动端视频通话软件，大致看了下现有的开源软件

一) sipdroid
1）架构
sip协议栈使用JAVA实现，音频Codec使用skype的silk（Silk编解码是Skype向第三方开发人员和硬件制造商提供免版税认证(RF)的Silk宽带音频编码器）实现。NAT传输支持stun server.
2）优缺点：
NAT方面只支持STUN，无ICE框架，如需要完全实现P2P视频通话需要实现符合ICE标准的客户端,音频方面没看到AEC等技术，视频方面还不是太完善，目前只看到调用的是系统自带的MediaRecorder，并没有自己的第三方音视频编解码库。
3）实际测试：
基于sipdroid架构的话，我们要做的工作会比较多，（ICE支持，添加回音消除，NetEQ等gips音频技术，添加视频硬件编解码codec.）,所以就不做测试了。

二) imsdroid
1)架构：
基于doubango(Doubango 是一个基于3GPP IMS/RCS 并能用于嵌入式和桌面系统的开源框架。该框架使用ANSCI-C编写，具有很好的可移植性。并且已经被设计成非常轻便且能有效的工作在低内存和低处理能力的嵌入式系统上。苹果系统上的idoubs功能就是基于此框架编写) .音视频编码格式大部分都支持（H264(video)，VP8(video)，iLBC(audio),PCMA,PCMU,G722,G729）。NAT支持ICE（stun+turn）
2）效果实测
测试环境：公司局域网内两台机器互通，服务器走外网sip2sip
音频质量可以，但是AEC打开了还是有点回音（应该可以修复）。视频马赛克比较严重，延迟1秒左右。
3）优缺点
imsdroid目前来说还是算比较全面的，包括音视频编解码，传输（RTSP，ICE），音频处理技术等都有涉猎。doubango使用了webrtc的AEC技术，但是其调用webrtc部分没有开源，是用的编译出来的webrtc的库。如果要改善音频的话不太方便，Demo的音频效果可以，视频效果还是不太理想。

三）csipsimple
1）sip协议栈用的是pjsip,音视频编解码用到的第三方库有ffmpeg（video）,silk(audio),webrtc.默认使用了webrtc的回声算法。支持ICE协议。
2）优缺点：
csipsimple架构比较清晰，sip协议由C实现，java通过JNI调用，SIP协议这一块会比较高效。其VOIP各个功能也都具备，包括NAT传输，音视频编解码。并且该项目跟进新技术比较快，官方活跃程度也比较高。如果做二次开发可以推荐这个。
3）实测效果
测试环境：公司局域网内两台机器互通，服务器走外网sip2sip
音频质量可以，无明显回音，视频需要下插件，马赛克比imsdroid更严重。

四）Linphone

这个是老牌的sip，支持平台广泛 windows, mac,ios,android,linux，技术会比较成熟。但是据玩过的同事说linphone在Android上的bug有点多，由于其代码实在庞大，所以我暂时放弃考虑Linphone.不过如果谁有跨平台的需要，可以考虑Linphone或者imsdroid和下面的webrtc.。。。好像现在开源软件都跨平台了。。。

五) webrtc

imsdroid,csipsimple,linphone都想法设法调用webrtc的音频技术，本人也测试过Android端的webrtc内网视频通话，效果比较满意。但是要把webrtc做成一个移动端的IM软件的话还有一些路要走，不过webrtc基本技术都已经有了，包括p2p传输，音视频codec,音频处理技术。不过其因为目前仅支持VP8的视频编码格式（QQ也是）想做高清视频通话的要注意了。VP8在移动端的硬件编解码支持的平台没几个（RK可以支持VP8硬件编解码）。不过webrtc代码里看到可以使用外部codec,这个还是有希望调到H264的。

总结：sipdroid比较轻量级，着重基于java开发（音频codec除外），由于其音视频编码以及P2P传输这一块略显不足，不太好做定制化开发和优化。imsdroid,遗憾就是直接调用webrtc的库，而最近webrtc更新的比较频繁，开发比较活跃。如果要自己在imsdroid上更新webrtc担心兼容性问题，希望imsdroid可以直接把需要的webrtc相关源码包进去。csipsimple的话，都是围绕pjsip的，webrtc等都是以pjsip插件形式扩充的,类似gstreamer. webrtc如果有技术实力的开发公司个人还是觉得可以选择这个来做，一个是google的原因，一个是其视频通话相关关键技术都比较成熟的原因。个人觉得如果能做出来，效果会不错的。









[linphone安装和使用教程](http://blog.csdn.net/ws84643557/article/details/7740564)





刚刚搞通linphone，终于能连上sip.linphone.org了，中间过程太心酸了。

一开始下载了linphone，申请了免费了linphone帐号，却不会填帐号密码，以致一直连不上服务器。

后来有开始在自己电脑上装SIP服务器，装的是opensips，又折腾了2天，服务器也没装成功（毅种循环）。

后来google到了windows上面的[sip server 2008 + x-lite](http://blog.csdn.net/probezy/article/details/4740938)的挺简单的（带图文教程），所以照着做了一下，也可以连上自己的服务器。



但是用这种在同一个子网的方式连接方式，有时会有各种错误，比如我用教育网的时候就连接不上，而且现在我在用的时候，x-lite可以

注册上服务器，但是两台机子通讯的时候，就显示 bad gateway 错误（还不知道什么原因，以后再找）。后来用linphone也连上sip server 2008。

就是在这个时候才知道了要怎么填opensips帐号密码的。



讲了那么多废话，现在开始进入正题。

一开始我也是下载源代码安装的。后来因为各种不能用，被打击得不行，以为自己没编译好。后来才知道原来是一些没有看到的东西。

然后我用synaptic把linphone全删了，直接用sudo apt-get 安装。

sudo apt-get install linphone-nox            #安装linphone，没有图形化界面，用linphone就有。



然后就直接执行   $   linphonec



我错误的原因就是因为SIP的端口5060被占用了，一直没有弄成功。

注意Warning哪句话，Warning: UDP port 5060 seems already in use ! Cannot initialize Ready（原谅我不会画重点）。

这样的话是注册不到服务器的（就是这个害我搞了那么久）。

解决办法是修改端口，不要用这个端口，改端口就可以了。配置文件和一般配置文件一样，在用户的家目录。

可以执行     vim ~/.linphonerc

把sip_port=5060改成其他端口比如5061。

这个时候再执行就没问题了。



这个时候就可以注册进服务器了，注册帐号可以上linphone官网注册。

我这里显示注册成功是因为之前注册过，信息写到.linphonerc里面了，所以会自动登录（也可以改里面的注册信息，自动登录）。

但是第一次登录时，需要注册。

比如我的帐号为 tao@sip.linphone.org 密码为ws123123（注意前面要加sip：）。



然后就注册成功了，这个时候就可以打电话了。



然后就可以用了。

比如 call sip:fxuk@sip.linphone.org

还有很多命令，自己用help查吧。

图形界面的设置之后再传上来吧。









