# live555 - maopig的专栏 - CSDN博客
2011年08月05日 16:06:51[maopig](https://me.csdn.net/maopig)阅读数：7831
Real Time Streaming Protocol或 者RTSP（实时流媒体协议），是由Real network 和Netscape共同提出的如何有效地在IP网络上传输流媒体数据的应用层协议。RTSP提供一种可扩展的框架，使能够提供能控制的，按需传输实时数
 据，比如音频和视频文件。源数据可以包括现场数据的反馈和存贮的文件。rtsp对流媒体提供了诸如暂停，快进等控制，而它本身并不传输数据，rtsp作用 相当于流媒体服务器的远程控制。传输数据可以通过传输层的tcp，udp协议，rtsp也提供了基于rtp传输机制的一些有效的方法。
RTSP协议是一个非常类似HTTP协议的流控制协议。它们都使用纯文本来发送信息，而且rtsp协议的语法
 也和HTTP类似。Rtsp一开始这样设计，也是为了能够兼容使用以前写的HTTP协议分析代
 码。这是个好消息。
它们主要的区别是HTTP协议是没有状态的，http协议在发送一个命令后，连接会断 开，而且命令之间没有依赖性。不同的是RTSP的命令需要知道现在正处于一个什么状态，也就是说rtsp的命令总是
 按照顺序来发送，某个命令总在另外一个命令之前要发送。Rtsp不管处于什么状态都不会去断掉连接。
HTTP
协议默 认使用80端口，而RTSP
默认使用554端口。如果一些服务器因为某些安全的原因而封掉了这个端口，那代理和防火墙可能不让RTSP消息通 过，需要管理员去放开554端口，而使得rtsp协议能通过。
**基本类**
在Mplayer中如果使用live选项进行编译，则需要安装live555库。live555实现RTP/RTSP功能。
使用环境（usageEnvironment）：UsageEnvironment和TaskScheduler类用在调度不同事件。还有 HashTable类定义，这些都是抽象基类。在使用过程中可以利用环境的特定功能。
groupsock：封装网络接口和socket。特别是还封装了multicast应用，这个multicast并不是Mbone意义的 multicast，而是将多个写而不读socket组合处理，用来模拟multicast。
liveMedia：定义一个类栈，根类是Medium类－不同的流类型和编解码器。
BasicUsageEnvironment：定义一个usageEnvironment的实现, 这个里面除了有一个TaskScheduler以外，都是一些说明性的东西。TaskSheduler里面是一些调度相关的函数，其中 doEventLoop是主控函数，定义为纯虚函数。
testProgs：目录下是一个简单的实现，使用了BasicUsageEnvironment来展示如何使用这些库**。**BasicTaskScheduler0：主要成员变量有fdelayQueue, fReadHandlers, fLastHandledSocketNum;这里主要的处理函数是scheduleDelayedTask， 通过定时器触发一个时间，比如RTCP包的发送等。
BasicTaskScheduler：中又添加了fMaxNumSockets和fReadSet。其中freadHandlers类中定义一个链表，将所有的句柄，对应的处理函数 和处理函数参数作链接成一个链表，通过成员函数**assignHandler**管理。这里面主要的函数是turnOnBackgroundReadHandling，这个函数把句柄和处理函数注册到
 select中，这样可以完成数据包的等待及其处理操作。
MediaSession类中定义了一个mediaSubSession链表；MediaSubSession中又SessionId，服务端口 号，rtp/rtcp ChannelId和MediaSink指针，等等一些参数信息。
**H.264**
1. 基类
Medium：定义媒体名字和环境信息，处理任务名，静态函数close和lookupByName和一些虚函数来指明该medium类型，是媒体帧的基 类。
MediaSource类：实现基类中medium类型的虚函数，
FramedSoruce：定义了getNextFrame和doGetNextFrame纯虚函数是使用到的一些变量。
2. 相关类
H264VideoStreamFramer；H264VideoFileSink：H264VideoRTPSource：H264VideoRTPSinik
H.264利用NAL封装数据，通过RTP传输数据包。相关的处理在RTPSource/Sink中。
**Mplayer**
从RTSP或者SIP中渠道SDP描述，然后调用Live555中的mediaSession类创建Session。通过成员函数 initializeWithSDP分析SDP描述。
**OpenRTSP**
**1. Client**
1. 创建TaskScheduler和UsageEnvironment实例；
2. 调用createClient创建media实例；
在openRTSP.c中，main完成配置以后，开始如下循环：
startPlayingStreams();
env->taskScheduler().doEventLoop(); // does not return
在BasicTaskScheduler0类中，定义为while(1) SingleStep();
SingleStep的处理是通过select监听一组句柄，这组句柄通过iter组成的链表串接起来，对每个句柄有处理函数，如果有句柄上有数据，那么 调用对应的处理函数。
**2. liev555mediaserver**
创建过程：
1. 创建TaskScheduler：这里仅仅初始化一个fdset并且socket数目初始化为0。
2. 以TaskScheduler为参数创建UsageEnvironment对象。
3. 以前述environment和**服务端口号**（554/8554）以及用户认证对象为参数创建 RTSPServer对象，这里是用子类 DynamicRTSPServer 的创建函数创建。在createNew成员函数中建立socket，分配发送缓冲区，和创建RTSPServer对象。这里通过 turnOnBackgroundReadHandling函数将要处理的句柄和处理函数关联起来。
4. 执行env->taskScheduler().doEventLoop();
从RFC2326中可以看出通常的交互流程是发送describe，然后发送setup，再play。所以以请求MPG多媒体URI为例分析如下：
C->M: DESCRIBE rtsp://foo/twister RTSP/1.0
CSeq: 1
M->C: RTSP/1.0 200 OK
CSeq: 1
Content-Type: application/sdp
Content-Length: 164
v=0
o=- 2890844256 2890842807 IN IP4 172.16.2.93
s=RTSP Session
i=An Example of RTSP Session Usage
a=control:rtsp://foo/twister
t=0 0
m=audio 0 RTP/AVP 0
a=control:rtsp://foo/twister/audio
m=video 0 RTP/AVP 26
a=control:rtsp://foo/twister/video
C->M: SETUP rtsp://foo/twister/audio RTSP/1.0
CSeq: 2
Transport: RTP/AVP;unicast;client_port=8000-8001
M->C: RTSP/1.0 200 OK
CSeq: 2
Transport: RTP/AVP;unicast;client_port=8000-8001;
server_port=9000-9001
Session: 12345678
C->M: SETUP rtsp://foo/twister/video RTSP/1.0
CSeq: 3
Transport: RTP/AVP;unicast;client_port=8002-8003
Session: 12345678
M->C: RTSP/1.0 200 OK
CSeq: 3
Transport: RTP/AVP;unicast;client_port=8002-8003;
server_port=9004-9005
Session: 12345678
C->M: PLAY rtsp://foo/twister RTSP/1.0
CSeq: 4
Range: npt=0-
Session: 12345678
M->C: RTSP/1.0 200 OK
CSeq: 4
Session: 12345678
RTP-Info: url=rtsp://foo/twister/video;
seq=9810092;rtptime=3450012
C->M: PAUSE rtsp://foo/twister/video RTSP/1.0
CSeq: 5
Session: 12345678
M->C: RTSP/1.0 460 Only aggregate operation allowed
CSeq: 5
C->M: PAUSE rtsp://foo/twister RTSP/1.0
CSeq: 6
Session: 12345678
M->C: RTSP/1.0 200 OK
CSeq: 6
Session: 12345678
C->M: SETUP rtsp://foo/twister RTSP/1.0
CSeq: 7
Transport: RTP/AVP;unicast;client_port=10000
M->C: RTSP/1.0 459 Aggregate operation not allowed
CSeq: 7
函数 handleCmd_DESCRIBE 处理describe请求，生成含SDP信息响应消息，lookupServerMediaSession函数是虚函数，在创建RTSPServer对象 时，用的是子类DynamicRTSPServer的创建函数，所以上述函数使用的是类DynamicRTSPServer中的定义。函数打开，并且分析 流媒体文件。createNewSMS根据请求的文件后缀来调用对应的处理函数。如果是MPG，那么创建ServerMediaSession对象并添加 到RTSPserver中，这个对象是可以通过Hash类定位的。然后创建一个Mpeg1or2FileServerDemux对象demux,然后将创
 建demux对象中的音频，视频子会话对象并且通过函数 addSubsession 将他们添加到ServerMediaSession中链表中。响应函数会依次调用会话中注册的子会话的**sdpLines**函 数以取得SDP信息。sdpLines是一个纯虚函数，如果URI指定的MPG文件，那么sdpLines 函数在 OnDemandServerMediaSubsession 中定义实现。此时，对每个子会话那么会创建一个sink对象。对应MPG文件，在 MPEG1or2DemuxedServerMediaSubsession类定义的createNewRTPSink
 会创建对象:
音频：MPEG1or2AudioRTPSink**::**AudioRTPSink**::**MultiFramedRTPSink**::**RTPSink**::**MediaSink**::**Media
视频：MPEG1or2VideoRTPSink**::**VideoRTPSink**::**MultiFramedRTPSink**::**RTPSink**::**MediaSink**::**Media
而SDP信息的获取在函数 setSDPLinesFromRTPSink 中处理。（AudioRTPSink指定媒体类型，时间标签频率和载荷格式名“MPA”，视频名字是“MPV"）
这里类结构 MPEG1or2DemuxedServerMediaSubsession** ::**OnDemandServerMediaSubsession
**::**ServerMediaSubsession **::**Medium
函数 handleCmd_SETUP处理setup请求，
MPEG1or2Demux 类是mpeg相关的一个主要类，创建该类时会分析媒体文件。该类定义中有个数组，OutputDescriptor fOutput[256];MPEG1or2Demux的构造函数中初始化。
**                                                                在windows使用vs2008编译live555**
１．live555 源代码简介
liveMedia项目的源代码包括四个基本的库，各种测试代码以及IVE555 Media Server。
四 个基本的库分别是 UsageEnvironment&TaskScheduler，groupsock，liveMedia，BasicUsageEnvironment。
UsageEnvironment 和TaskScheduler类用于事件的调度，实现异步读取事件的句柄的设置以及错误信息的输出。另外，还有一个HashTable类定义了一个通用的 hash表，其它代码要用到这个表。这些都是抽象类，在应用程序中基于这些类实现自己的子类。
groupsock类是对网络接口的封装， 用于收发数据包。正如名字本身，Groupsock主要是面向多播数据的收发的，它也同时支持单播数据的收发。Groupsock定义了两个构造函数
Groupsock(UsageEnvironment& env, struct in_addr const& groupAddr,
Port port, u_int8_t ttl);
Groupsock(UsageEnvironment& env, struct in_addr const& groupAddr,
struct in_addr const& sourceFilterAddr,
Port port);
前 者是用于SIM（source-independent multicast）组，后者用于SSM（source-specific multicast）组。groupsock库中的Helper例程提供了读写socket等函数，并且屏蔽了不同的操作系统之间的区别，这是在 GroupsockHelper.cpp文件中实现的。
liveMedia库中有一系列类，基类是Medium，这些类针对不同的流媒体 类型和编码。
各种测试代码在testProgram目录下，比如openRTSP等，这些代码有助于理解liveMedia的应用。
LIVE555 Media Server是一个纯粹的RTSP服务器。支持多种格式的媒体文件:
* TS流文件，扩展名ts。
* PS流文件，扩展名mpg。
* MPEG-4视频基本流文件，扩展名m4e。
* MP3文件，扩展名mp3。
* WAV文件(PCM)，扩展名wav。
* AMR音频文件，扩展名.amr。
* AAC文件，ADTS格式，扩展名aac。
２．在ｗｉｎｄｏｗｓ下编译ｌｉｖｅ５５５
(1).下载 live555,http://www.live555.com/
(2). 编辑 win32config,TOOLS32=C:\Program Files\Microsoft Visual Studio 9.0\VC
即保证这个路径是你的Vs2008 路径。 
(3). 编辑"LINK_OPTS_0 = $(linkdebug) msvcirt.lib" in win32config to "LINK_OPTS_0 = $(linkdebug) msvcrt.lib"，由于编译器所要LINK的运行库不一样了
(4). 修改groupsock/makefile.head,替换"INCLUDES = -Iinclude -I../UsageEnvironment/include" 为"INCLUDES = -Iinclude -I../UsageEnvironment/include -DNO_STRSTREAM".
(5). 运行genWindowsMakefiles.cmd 生成VS能够编译的*.mak文件
(6). 将以下内容保存为live/compile.bat
call "C:\Program Files\Microsoft Visual Studio 9\VC\vcvarsall.bat"
cd liveMedia
nmake /B -f liveMedia.mak
cd ../groupsock
nmake /B -f groupsock.mak
cd ../UsageEnvironment
nmake /B -f UsageEnvironment.mak
cd ../BasicUsageEnvironment
nmake /B -f BasicUsageEnvironment.mak
cd ../testProgs
nmake /B -f testProgs.mak
cd ../mediaServer
nmake /B -f mediaServer.mak
有 关这一点来说，建议把vs2008的编译环境加入到环境变量中，那么以后需用命令行编译程序都可行了，可以参考 http://blog.chinaunix.net/u3/94873/showart_1907792.html的前部分设置VS2008的环境设 置。
(7). 在命令行下运行compile.bat,就会看到很多编译过程出现在CMD中了。
以上的编译并不是 DEBUG模式，要调试时，应先在win32config加入一行"NODEBUG=1"
进行编译后，可以在要调试的程序路径下输入 如：C:\works\VCCode\video\live555-latest\live\testProgs>devenv openRTSP.exe,这样就会把相关的调试环境搭建起来进行调试了。
liveMedia项目的源代码包括四个基本的库，各种测试代码以及IVE555 Media Server。
四个基本的库分别是 UsageEnvironment&TaskScheduler，groupsock，liveMedia，BasicUsageEnvironment。
UsageEnvironment 和TaskScheduler类用于事件的调度，实现异步读取事件的句柄的设置以及错误信息的输出。另外，还有一个HashTable类定义了一个通用的 hash表，其它代码要用到这个表。这些都是抽象类，在应用程序中基于这些类实现自己的子类。
groupsock类是对网络接口的封装， 用于收发数据包。正如名字本身，Groupsock主要是面向多播数据的收发的，它也同时支持单播数据的收发。Groupsock定义了两个构造函数
Groupsock(UsageEnvironment& env, struct in_addr const& groupAddr,
Port port, u_int8_t ttl);
Groupsock(UsageEnvironment& env, struct in_addr const& groupAddr,
struct in_addr const& sourceFilterAddr,
Port port);
前者是用于SIM（source-independent multicast）组，后者用于SSM（source-specific multicast）组。groupsock库中的Helper例程提供了读写socket等函数，并且屏蔽了不同的操作系统之间的区别，这是在 GroupsockHelper.cpp文件中实现的。
liveMedia库中有一系列类，基类是Medium，这些类针对不同的流媒体 类型和编码。
各种测试代码在testProgram目录下，比如openRTSP等，这些代码有助于理解liveMedia的应用。
LIVE555 Media Server是一个纯粹的RTSP服务器。支持多种格式的媒体文件:
* TS流文件，扩展名ts。
* PS流文件，扩展名mpg。
* MPEG-4视频基本流文件，扩展名m4e。
* MP3文件，扩展名mp3。
* WAV文件(PCM)，扩展名wav。
* AMR音频文件，扩展名.amr。
* AAC文件，ADTS格式，扩展名aac。 
用live555开发应用程序
基于 liveMedia的程序，需要通过继承UsageEnvironment抽象类和TaskScheduler抽象类，定义相应的类来处理事件调度，数据 读写以及错误处理。live项目的源代码里有这些类的一个实现，这就是“BasicUsageEnvironment”库。 BasicUsageEnvironment主要是针对简单的控制台应用程序，利用select实现事件获取和处理。这个库利用Unix或者 Windows的控制台作为输入输出，处于应用程序原形或者调试的目的，可以用这个库用户可以开发传统的运行与控制台的应用。
通过使用 自定义的“UsageEnvironment”和“TaskScheduler”抽象类的子类，这些应用程序就可以在特定的环境中运行，不需要做过多的修 改。需要指出的是在图形环境（GUI toolkit）下，抽象类 TaskScheduler 的子类在实现 doEventLoop()的时候应该与图形环境自己的事件处理框架集成。
先来熟悉在liveMedia库中Source，Sink以及 Filter等概念。Sink就是消费数据的对象，比如把接收到的数据存储到文件，这个文件就是一个Sink。Source就是生产数据的对象，比如通过 RTP读取数据。数据流经过多个'source'和'sink's，下面是一个示例：
'source1' -> 'source2' (a filter) -> 'source3' (a filter) -> 'sink'
从其它 Source接收数据的source也叫做"filters"。Module是一个sink或者一个filter。
数据接收的终点是 Sink类，MediaSink是所有Sink类的基类。MediaSink的定义如下：
class MediaSink: public Medium {
public:
static Boolean lookupByName(UsageEnvironment& env, char const* sinkName,
MediaSink*& resultSink);
typedef void (afterPlayingFunc)(void* clientData);
Boolean startPlaying(MediaSource& source,
afterPlayingFunc* afterFunc,
void* afterClientData);
virtual void stopPlaying();
// Test for specific types of sink:
virtual Boolean isRTPSink() const;
FramedSource* source() const {return fSource;}
protected:
MediaSink(UsageEnvironment& env); // abstract base class
virtual ~MediaSink();
virtual Boolean sourceIsCompatibleWithUs(MediaSource& source);
// called by startPlaying()
virtual Boolean continuePlaying() = 0;
// called by startPlaying()
static void onSourceClosure(void* clientData);
// should be called (on ourselves) by continuePlaying() when it
// discovers that the source we're playing from has closed.
FramedSource* fSource;
private:
// redefined virtual functions:
virtual Boolean isSink() const;
private:
// The following fields are used when we're being played:
afterPlayingFunc* fAfterFunc;
void* fAfterClientData;
};
Sink 类实现对数据的处理是通过实现纯虚函数continuePlaying()，通常情况下continuePlaying调用 fSource->getNextFrame来为Source设置数据缓冲区，处理数据的回调函数等，fSource是MediaSink的类型为 FramedSource*的类成员；
基于liveMedia的应用程序的控制流程如下:
应用程序是事件驱动的，使用如 下方式的循环
while (1) {
通过查找读网络句柄的列表和延迟队列（delay queue）来发现需要完成的任务
完成这个任务
}
对于每个sink，在进入这个循环之 前，应用程序通常调用下面的方法来启动需要做的生成任务：
someSinkObject->startPlaying();
任何时候，一个Module需要获取数据都通过调用刚好在它之前 的那个Module的FramedSource::getNextFrame()方法。这是通过纯虚函数FramedSource:oGetNextFrame() 实现的，每一个Source module都有相应的实现。
Each 'source' module's implementation of "doGetNextFrame()" works by arranging for an 'after getting' function to be called (from an event handler) when new data becomes available for the caller.
注意，任何应用程序都要处理从'sources'到'sinks' 的数据流，但是并非每个这样的数据流都与从网络接口收发数据相对应。
比如，一个服务器应用程序发送RTP数据包的时候用到一个或多 个"RTPSink" modules。这些"RTPSink" modules以别的方式接收数据，通常是文件 "*Source" modules (e.g., to read data from a file), and, as a side effect, transmit RTP packets.
一个简单的RTSP客户端程序
在另一个文章里,给出了这个简单的客户端的 程序的代码，可以通过修改Makefile来裁剪liveMedia，使得这个客户端最小化。此客户端已经正常运行。
首先是OPTION
然 后是DESCRIBE
建立Media Session，调用的函数是 MediaSession::createNew，在文件liveMedia/MediaSession.cpp中实现。
为这个Media Session建立RTPSource，这是通过调用 MediaSubsession::initiate来实现的的，这个方法在liveMedia/MediaSession.cpp中实现。
在然 后是SETUP
最后是PLAY
rtp数据的句 柄:MultiFramedRTPSource::networkReadHandler 在liveMedia/MultiFramedRTPSource.cpp中
rtcp数据处理的句 柄:RTCPInstance::incomingReportHandler 在liveMedia/RTCP.cpp中
rtp数据处 理的句柄的设置:MultiFramedRTPSource:oGetNextFrame 在liveMedia/MultiFramedRTPSource.cpp中, 被FileSink::continuePlaying调用在FileSink.cpp中.
rtcp数据处理的句柄设置 fRTCPInstance = RTCPInstance::createNew 在/liveMedia/MediaSession.cpp中调用,
createNew 调用了构造函数RTCPInstance::RTCPInstance,这个构造函数有如下调用
TaskScheduler::BackgroundHandlerProc* handler = (TaskScheduler::BackgroundHandlerProc*)&incomingReportHandler;
*********************************************************************************************************************
通 过分析live库提供的例子程序OpenRTSP，可以清晰地了解客户端接收来自网络上媒体数据的过程。注意，RTP协议和RTCP协议接收的数据分别是 视音频数据和发送/接收状况的相关信息，其中，RTP协议只负责接收数据，而RTCP协议除了接收服务器的消息之外，还要向服务器反馈。
A.        main函数流程
main(int argc,char *argv[])
{
1.            创建BasicTaskScheduler对象
2.            创建BisicUsageEnvironment对象
3.            分析argv参数，（最简单的用法是：openRTSP rtsp://172.16.24.240/mpeg4video.mp4）以便在下面设置一些相关参数
4.            创建RTSPClient对象
5.            由RTSPClient对象向服务器发送OPTION消息并接受回应
6.            产生SDPDescription字符串（由RTSPClient对象向服务器发送DESCRIBE消息并接受回应，根据回应的信息产生 SDPDescription字符串，其中包括视音频数据的协议和解码器类型）
7.            创建MediaSession对象（根据SDPDescription在MediaSession中创建和初始化MediaSubSession子会话对 象）
8.            while循环中配置所有子会话对象（为每个子会话创建RTPSource和RTCPInstance对象，并创建两个GroupSock对象，分别对应 RTPSource和RTCPInstance对象，把在每个GroupSock对象中创建的socket描述符置入 BasicTaskScheduler::fReadSet中，RTPSource对象的创建的依据是SDPDescription，例如对于MPEG4
 文件来说，视音频RTPSource分别对应MPEG4ESVideoRTPSource和MPEG4GenericRTPSource对象。 RTCPInstance对象在构造函数中完成将Socket描述符、处理接收RTCP数据的函数 (RTCPInstance::incomingReportHandler)以及RTCPInstance本身三者绑定在一个 HandlerDescriptor对象中，并置入BasicTaskScheduler::fReadHandler中。完成绑定后会向服务器发送一条 消息。）
9.            由RTSPClient对象向服务器发送SETUP消息并接受回应。
10.        while循环中为每个子会话创建接收器(FileSink对象)，在FileSink对象中根据子会话的codec等属性缺省产生记录视音频数据的文件 名，视音频文件名分别为：video-MP4V-ES-1和audio-MPEG4-GENERIC-2，无后缀名
11.        while循环中为每个子会话的视音频数据装配相应的接收函数，将每个子会话中的RTPSource中的GroupSock对象中的SOCKET描述符， 置入BasicTaskScheduler::fReadSet中，并将描述符、处理接收RTP数据的函数 (MultiFramedRTPSource::networkReadHandler)以及RTPSource本身三者绑定在一个 HandlerDescriptor对象中，并置入BasicTaskScheduler::fReadHandler中,并将FileSink的缓冲区
 和包含写入文件操作的一个函数指针配置给RTPSource对象，这个缓冲区将会在networkReadHandler中接收来自网络的视音频数据(分 析和去掉RTP包头的工作由RTPSource完成)，而这个函数指针在networkReadHandler中被调用以完成将缓冲区中的数据写入文件。
12.        由RTSPClient对象向服务器发送PLAY消息并接受回应。
13.        进入while循环，调用BasicTaskScheduler::SingleStep()函数接受数据，直到服务器发送TREADOWN消息给客户 端，客户端接收到该消息后释放资源，程序退出
