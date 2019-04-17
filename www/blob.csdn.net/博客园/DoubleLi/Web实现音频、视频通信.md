# Web实现音频、视频通信 - DoubleLi - 博客园






**[Google开源实时通信项目WebRTC](http://www.infoq.com/cn/news/2011/06/google-webrtc)**

Google正式开源了[WebRTC实时通信项目](http://sites.google.com/site/webrtc/home)，希望浏览器厂商能够将该技术内建在浏览器中，从而使Web应用开发人员能够通过HTML标签和JavaScript API就实现Web音频、视频通信功能。

WebRTC（Web Real Time Communication）并不是Google原来自己的技术。在2010年，Google以大约6820万美元[收购](http://www.computerworld.com/s/article/9176906/Google_to_acquire_voice_and_video_codec_company)了VoIP软件开发商Global IP Solutions公司，并因此获得了该公司拥有的WebRTC技术。如今，互联网的音频、视频通信服务技术一般都是私有技术，如[Skype](http://www.skype.com/)， 需要通过安装插件或者桌面客户端来实现通信功能。Google希望Web开发人员能够直接在浏览器中创建视频或语音聊天应用，Global IP Solutions公司之前已经针对Android、Windows Mobile、iPhone制作了基于WebRTC的移动客户端。Google此次将WebRTC开源出来，就是希望浏览器厂商能够将该技术直接内嵌到浏 览器中，从而方便Web开发人员。

WebRTC的[博客](http://sites.google.com/site/webrtc/blog)说明了WebRTC的优势和发展方向：

直到现在，实时通信仍然需要私有的信号处理技术，大部分都是通过插件和客户端来安装使用。我们通过WebRTC开源了收购GIPS后获得的音频、视频引擎技术，让开发人员能够了解信号处理技术，并使用了BSD风格的[授权](http://sites.google.com/site/webrtc/license-rights/license)。这会支持开发人员通过简单的HTML和JavaScript API创建音频和视频聊天应用。

我们正在与其他浏览器开发厂商Mozilla和Opera等紧密合作，尽快在浏览器中实现这项技术以便于Web社区使用。此外，我们还积极地参与IETF和W3C工作组的活动，定义和实现一套实时通信标准。

其[官网](http://sites.google.com/site/webrtc/faq)上列表了使用WebRTC技术的四个理由：
- 互联网成功的一个关键因素是一些核心技术如HTML、HTTP和TCP/IP是开放和免费实现的。目前，在浏览器通信领域还没有免费、高质量、完整的解决方案。WebRTC就是这样的技术。
- 该技术已经集成了最佳的音频、视频引擎，并被部署到数以百万级的终端中，经过超过8年的磨练。Google不会从该技术中收取费用。
- 包含了使用STUN、ICE、TURN、RTP-over-TCP的关键NAT和防火墙穿越技术，并支持代理。
- 构建在浏览器中，WebRTC通过提供直接映射到PeerConnection的信号状态机来抽象信号处理。Web开发人员因此可以选择适合应用场景的协议（例如：SIP、XMPP/Jingle等等）。

WebRTC的架构图如下所示：

![](https://pic002.cnblogs.com/images/2012/3423/2012033101224064.png)



图 1  WebRTC架构图（图片来源：WebRTC官方网站）

关于架构图的组成部分，包括：

Web API——第三方开发人员用来开发基于Web的应用，如视频聊天。
WebRTC Native C++ API——浏览器厂商用于实现Web API的函数集。
Session Management——抽象session层，支持调用构建和管理层，由应用开发者来决定如何实现协议。
VoiceEngine——音频媒体链的框架，从声卡到网络。
iSAC——一种用于VoIP和流音频的宽带和超宽带音频编解码器，iSAC采用16 kHz或32 kHz的采样频率和12—52 kbps的可变比特率。
iLBC——用于VoIP和流音频的窄带语音编解码器，使用8 kHZ的采样频率，20毫秒帧比特率为15.2 kbps，30毫米帧的比特率为13.33 kbps，标准由IETF RFC 3951和3952定义。
NetEQ for Voice——动态抖动缓存和错误隐藏算法，用于缓解网络抖动和丢包引起的负面影响。在保持高音频质量的同时尽可能降低延迟。
VideoEngine——视频媒体链的框架，从相机像头到网络，从网络到屏幕。
VP8——来自于WebM项目的视频编解码器，非常适合RTC，因为它是为低延迟而设计开发的。
Image enhancements——消除通过摄像头获取的图片的视频噪声等。
其他更详细的架构分析可以查看这里的[文档](http://sites.google.com/site/webrtc/reference/webrtc-components)。

PeerConnection位于WebRTC Native C++ API的最上层，它的代码实现来源于[libjingle](http://code.google.com/apis/talk/libjingle/index.html)（一款p2p开发工具包），目前被应用于WebRTC中。其中关键的两个类定义是：

class  PeerConnectionObserver {
public:
 virtual void OnError();
 virtual void OnSignalingMessage(const std::string& msg);
 virtual void OnAddStream(const std::string& stream_id,
                          int channel_id,
                          bool video);
 virtual void OnRemoveStream(const std::string& stream_id,
                             int channel_id,
                             bool video);
};

该类定义了一个抽象的观察者。开发人员应该继承实现自己的观察者类。

class  PeerConnection {
public:
 explicit PeerConnection(const std::string& config);
 bool Initialize();
 void RegisterObserver(PeerConnectionObserver* observer);
 bool SignalingMessage(const std::string& msg);
 bool AddStream(const std::string& stream_id, bool video);
 bool RemoveStream(const std::string& stream_id);
 bool Connect();
 void Close();
 bool SetAudioDevice(const std::string& wave_in_device,
                     const std::string& wave_out_device);
 bool SetLocalVideoRenderer(cricket::VideoRenderer* renderer);
 bool SetVideoRenderer(const std::string& stream_id,
                       cricket::VideoRenderer* renderer);
 bool SetVideoCapture(const std::string& cam_device);
};

具体的函数说明可以查看相应的[API介绍](http://sites.google.com/site/webrtc/reference#TOC-PeerConnection-Native-APIs)。

正如Google所说的，它一直在参与制定和实现HTML 5标准中的[视频会议和p2p通信部分](http://www.whatwg.org/specs/web-apps/current-work/complete/video-conferencing-and-peer-to-peer-communication.html)，虽然还不是正式标准，但是我们可以从草案的示例中看到未来Web开发人员的使用情况：

// the first argument describes the STUN/TURN server configuration
var local = new PeerConnection('TURNS example.net', sendSignalingChannel);
local.signalingChannel(...); // if we have a message from the other side, pass it along here
// (aLocalStream is some GeneratedStream object)
local.addStream(aLocalStream); // start sending video
function sendSignalingChannel(message) {
 ... // send message to the other side via the signaling channel
}
function receiveSignalingChannel (message) {
 // call this whenever we get a message on the signaling channel
 local.signalingChannel(message);
}
local.onaddstream = function (event) {
 // (videoElement is some <video> element)
 videoElement.src = URL.getObjectURL(event.stream);
};

目前有关Web实时通信的技术标准正在制定当中，W3C的[Web Real-Time Communication工作组](http://www.w3.org/2011/04/webrtc-charter.html)今年五月份刚刚正式成立，并计划在今年第三季度发布第一个公开草案，从其工作组的路线图中可以看出，正式的推荐标准预计将在2013年第一季度发布，有关W3C标准是否会对WebRTC项目的发展有所影响，未来还要看草案的具体细节。

Google希望开源的WebRTC技术能够获得越来越多的浏览器厂商支持，WebRTC的网站已经宣布将在Chrome、Firefox和Opera上实现相应的API接口。Opera首席技术官Håkon Wium Lie对媒体[表示](http://www.theregister.co.uk/2011/06/01/google_open_sources_webrtc/)，Google能够把价值不菲的代码贡献出来非常了不起，Opera一直希望能够在浏览器中实现实时通信技术。

提到实时通信技术，不得不让人想起行业巨头Skype。巧合的是，就在前不久，微软刚刚斥资85亿美元现金[收购](http://news.ifeng.com/gundong/detail_2011_05/11/6302654_0.shtml)网络电话服务商Skype，当时有许多分析师指出，[微软的收购将直面Google的竞争](http://www.infoq.com/cn/news/2011/05/microsoft-skype)：

…...收购也挫败了竞争对手Google利用Skype技术完善通话服务的计划。

MSN即时通讯、必应（bing）搜索、网络广告等成了微软在互联网领域迎战谷歌的三大阵地。然而，相对于谷歌在互联网行业的呼风唤雨，微软的互联网战略一直没有真正展现出让业界看到其能够挑战谷歌的核心优势。

谷歌和苹果都已经在网络电话上投入了数年时间。苹果一直在力推iChat和FaceTime，谷歌也在普及其网络通信相关产品Chat和Voice。不过，谷歌和苹果都有一个共同的问题，他们很难在竞争对手的平台上生存。苹果用户无法和谷歌以及微软的用户进行视频聊天，谷歌在跨 平台方面做了很多努力，但是仍有不少障碍。

不过Skype则没有这一问题，作为免费服务的Skype可以运行在苹果、Linux以及Windows电脑上，也支持Android和iPhone等手机平台，甚至可以在电视机上运行。

收购Skype，微软不仅仅是为了从中受益，微软还希望借此使Skype远离谷歌的“魔爪”。如果谷歌收购了Skype，这对微软来说无疑是一个沉重的打击。想想吧，如果谷歌收购了Skype，并将Skype整合进Google Apps、Gmail、Google Talk和Google Voice等谷歌产品之中，再想想如果谷歌将Skype与Android平台进行一些深度整合，这对微软来说，无疑是一场灾难。即便微软并未很好的将Skype整合进自家产品之中，但这也避免了谷歌通过收购Skype来获得巨大的价值。

就在微软收购Skype不久，Google就宣布对WebRTC开源，这不免让人有所联想。有国外媒体[评论](http://thenextweb.com/google/2011/06/01/google-releases-developer-preview-of-webrtc-its-open-real-time-voice-and-video-platform/)说，Google此次开源是想让广大用户远离该行业的领导者如Skype和Apple的FaceTime平台。通过浏览器来支持实时通信技术，Google希望开发社区能够在自己的应用中集成该功能，从而削弱其他对手。

这已经不是Google第一次大方地开源关键项目。去年，Google曾经将VP8视频编解码项目开源，以支持HTML 5技术中的视频媒体播放。如今，多种主流浏览器都已经支持VP8格式，成为主要的多媒体格式，相比伴随着专利、付费纠纷的H.264，开放、免费的VP8更让浏览器厂商放心。









