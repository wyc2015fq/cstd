# live555 类之间的依属关系和功能分析 - 建建的博客 - CSDN博客
2017年03月01日 15:07:07[纪建](https://me.csdn.net/u013898698)阅读数：398
今天给大家分析下周末看live555的心得。
首先呢，不要指望一口气把live555能够肯下,那样代码你看着看着你就会发现涉及到的类越来，很快你自然的就投入很多时间去分析作者的设计架构.
再坚固的堡垒，只要咱们保持着恒心，强攻总有一天可以攻下。
今天就来分析下live555中设计的几个核心类.
在接收到客户端发送的Descripture命令后，服务器会创建ServerMediaSession和ServerMediaSubSession,创建了ServerMediaSession后会添加进RtspServer的HashTable：fServerMediaSessions中。
但是这里有一个问题，困扰了我很长时间.
ServerMediaSession里面以链表的形式存储ServerMediaSubSession，但是我看代码觉得始终ServerMediaSession里面只可能会存在一个ServerMediaSubSession，还有访问的资源，比如tv.264，跟ServerMediaSession和ServerMediaSubSession的对应关系是如何的?
在RtspClientSession中:
    unsigned fNumStreamStates;//有什么意义？
    struct streamState {
      ServerMediaSubsession* subsession;
      void* streamToken;//streamState
    } * fStreamStates;
fNumStreamStates的意义何在，是专门用于处理什么情况的?
第二部分接受到了Setup命令，客户端和服务器基本就已经协商好了，双边通信的端口，这个时候引入的主要类是;
FramedSource,RTPSink,GroupSock,BasicUDPSink,StreamState.
但是根据面向对象的设计原则，RtspClientSession肯定不会跟上面这么多类通信，这样RtspClientSession抗的风险就太大了.
它只需要与StreamState沟通就好了,最后借助ServerMediaSession::getStreamParameters，fStreamStates结构体的成员void* streamToken = StreamState.
StreamState拥有一个参数RTPSink，RTPSink拥有2个参数:FramedSource，GroupSocket,这样基本上他们的功能就理清了.
第三部分在接收到了Play命令后，主要调用ServerMediaSubSession的startStream来完成,而ServerMediaSubSession的主要调用的StreamState::startPlaying
咱门看下StreamState::startPlaying的核心代码:
fRTPSink->startPlaying(*fMediaSource, afterPlayingStreamState, this);
fRTCPInstance
      = RTCPInstance::createNew(fRTPSink->envir(), fRTCPgs,
    fTotalBW, (unsigned char*)fMaster.fCNAME,
    fRTPSink, NULL /* we're a server */);
看代码意图很明显:
开发RTP数据包，处理RTCP数据包完成和客户端交互.
好，那么通过上面的分析，基本上可以猜测出设计到的类的意图:
FrameSource:
完成对访问的资源的解析，提取出H264NALUE单元，并通过函数指针回调给RTPSink.
RTPSink对FramedSource传递过来的H264 NALU单元根据RFC3984的协议进行封包处理，封包处理好后，然后借助GroupSocket将数据推送出去.
StreamState的主要功能控制数据的发送，暂停，停止。
那么既然StreamState已经实现了这些最核心的功能，那么为何还需要ServerMediaSubSession和ServerMediaSession.
对于这2个类我存在的疑惑还真不少。
不过可以肯定得是，他们的存在应该与N个用户请求同一个资源文件有关。
比如N个用户同事请求:rtsp://127.0.0.1/tv.264，这样的有关。
作者的设计意图很可能是想复用相关的数据.
