# EMIPLIB简介 - DoubleLi - 博客园






EMIPLIB([http://research.edm.uhasselt.be/emiplib](http://research.edm.uhasselt.be/emiplib))的全称是'EDM Media over IP libray' 。EDM是[Hasselt University](http://www.uhasselt.be/)大学的[Expertise Centre for Digital Media](http://www.edm.uhasselt.be/)，这个库就是这里开发的。

这个库能做什么呢？官方是这么描述的（版本0.14.0）：

· Soundcard input (using [OSS](http://www.opensound.com/), [WinMM](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/multimed/htm/_win32_waveform_audio.asp) and [Jack](http://jackit.sourceforge.net/))

· Soundcard output (using [OSS](http://www.opensound.com/), [ALSA](http://www.alsa-project.org/), [ESD](http://www.tux.org/~ricdude/EsounD.html), [WinMM](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/multimed/htm/_win32_waveform_audio.asp), [Jack](http://jackit.sourceforge.net/) and [SDL](http://www.libsdl.org/))

· WAV file input (using [libsndfile](http://www.mega-nerd.com/libsndfile/), [libaudiofile](http://www.68k.org/~michael/audiofile/) or a simple internal WAV reader)

· WAV file output (using [libsndfile](http://www.mega-nerd.com/libsndfile/) or a simple internal WAV writer (8 bit, mono))

· Webcam input (using [Video4Linux](http://linuxtv.org/v4lwiki/) and [DirectShow](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directshow/htm/directshow.asp))

· [Speex](http://www.speex.org/) compression

· U-law audio encoding

· H.263+ compression (using [libavcodec](http://ffmpeg.sourceforge.net/index.php))

· Mixing of incoming audio streams

· Synchronization of RTP streams based on RTCP information

· 3D sound effects using HRIR/HRTF data from the [LISTEN](http://recherche.ircam.fr/equipes/salles/listen/) project

· Easy to use voice over IP (VoIP) and video over IP sessions

基本上这个库的目的就是要完成一个多媒体的网络传输库，方便多种媒体在网络上进行传输。从上面可以看到，它提供了从底层的数据采集、编码解码、传输等功能。当然，它不是独立的，比如，我是在window下做一个开发，对于音频它使用了winmm.lib库，对于视频使用了Dshow以及QT for windows。 另外，它也依赖于下面两个库：jrtplib以及jthread。比较详细的可以到官方网站找到参考。[http://research.edm.uhasselt.be/emiplib/documentation/index.html](http://research.edm.uhasselt.be/emiplib/documentation/index.html)

下面我们就针对windows平台下展开吧，因为我的EMIPLIB经验基本上是在windows平台地...

第一步：编译库(最基本编译)

先说说我的平台及环境：windows xp sp2 , vc8(第一次使用这个新的IDE，因为这个lib只提供了VC8的项目文件，时间原因我没办法尝试mingw及vc6等)。

可以到下面地址下载到源代码：

EMIPLIB [http://research.edm.uhasselt.be/emiplib/emiplib-0.14.0.zip](http://research.edm.uhasselt.be/emiplib/emiplib-0.14.0.zip)

JTHREAD [http://research.edm.uhasselt.be/jori/jthread/jthread-1.2.1.zip](http://research.edm.uhasselt.be/jori/jthread/jthread-1.2.1.zip)

JRTPLIB [http://research.edm.uhasselt.be/jori/jrtplib/jrtplib-3.7.0.zip](http://research.edm.uhasselt.be/jori/jrtplib/jrtplib-3.7.0.zip)

下载完以后，开始编译JTHREAD，用vc8打开源代码中的sln文件，在项目设置中C++项目下代码生成中选择多线程调试（/Mtd）。以同样方法编译JRTPLIB。

这两个库使用vc6和vc8都可以很轻松的编译，需要注意的是生成静态库时候要统一代码生成中的选项，我这里使用了/Mtd。完成后，在你的debug或者release目录线面会有这两个库：JRTPLIB.LIB JTHREAD.lib。如果编译这两个库的时候有问题，可以给我mail，或者直接去mail到作者那里，之前我也问过他一些问题，他是个不错的人。

然后我们需要来编译最大个的EMIPLIB库了，在EMIPLIB的源代码目录有README文件，上面有在windows下编译此库的参考。比较麻烦的问题是对于视频，我们可能需要添加QT支持（这可能需要我们在windows下先安装好QT，并自己手动完成一些文件）。对于最基本的编译我们先不去管它。上面的README文件中也说明了，在源代码目录的src/core/目录下面有一个配置文件mipconfig_win.h，我们可以在编译前进行一些选择，来得到更多功能支持的库。不过现在我们只使用默认的，只有speex支持被添加了。

用vc8打开sln文件吧，要正确的进行编译，我们最好先把上面需要的两个库，以及一些头文件放在一个合适的地方，让IDE可以找到它们。这需要你设置vc的目录（在工具--选项里面）添加include目录及lib目录。然后也在项目设置中设定多线程调试（/Mtd），现在应该可以编译EMIPLIB了。（PS：我们可能需要安装Dshow，新的版本MS 已经把它从DxSDK中踢出来鸟，放到了PlatFormSDK 中）。

最基本的编译应该不会花去我们太多时间。让我们先尝尝鲜，看看怎么用这个库。

第二步：hello world

在EMIPLIB源代码目录里面有一个examples目录，里面给了几个例子。我们找一个功能比较全的叫做feedbackexample.cpp，来看看怎么使用这个库：

我们省略了其中一些代码，只贴出来比较重要的。

/* 这个demo它打开一个叫做soundfile.wav的文件，并对其进行采样、uLaw编码、RTP编码然后设定好RTP的目的地（这里它发送给了自己），再经过RTP解码、uLAW解码、采样编码，发给声卡让它播放出来。 */



MIPTime interval(0.020); //设定间隔为20ms
MIPAverageTimer timer(interval); //设定计时器 
MIPWAVInput sndFileInput; //声音文件输入
MIPSamplingRateConverter sampConv, sampConv2; //采样转换
MIPSampleEncoder sampEnc, sampEnc2, sampEnc3; //采样编码
MIPULawEncoder uLawEnc; //uLaw编码
MIPRTPULawEncoder rtpEnc; //rtp编码
MIPRTPComponent rtpComp; //rtp组建 
MIPRTPDecoder rtpDec; //rtp解码
MIPRTPULawDecoder rtpULawDec; //rtp ulaw解码
MIPULawDecoder uLawDec; //uLaw解码 
MIPAudioMixer mixer; //混音
MIPWinMMOutput sndCardOutput; //声卡输出
MyChain chain("Sound file player"); //链：用来把上面组建串起来的对象。 
RTPSession rtpSession; //RTP会话，来自jrtplib 
bool returnValue; //返回值

int samplingRate = 8000; //采样率默认为8kHz 
int numChannels = 1; //声道设置默认为单声道



//打开一个wav文件。

returnValue = sndFileInput.open("soundfile.wav", interval);

//初始化采样转换，设置了采样率以及声道

returnValue = sampConv.init(samplingRate, numChannels);

//初始化采样编码，参数用于设定音频数据在计算机中的保存形式
returnValue = sampEnc.init(MIPRAWAUDIOMESSAGE_TYPE_S16);

//初始化ulaw编码
returnValue = uLawEnc.init();

//初始化rtp编码，这个组建可以创建发送到RTP组件的RTP消息。

returnValue = rtpEnc.init();

//初始化RTPSession，需要设置包括端口采样率等等与传输有关的参数
RTPUDPv4TransmissionParams transmissionParams;
RTPSessionParams sessionParams;
int portBase = 60000;
int status;

transmissionParams.SetPortbase(portBase);
sessionParams.SetOwnTimestampUnit(1.0/((double)samplingRate));
sessionParams.SetMaximumPacketSize(64000);
sessionParams.SetAcceptOwnPackets(true);
//建立一个RTPSession
status = rtpSession.Create(sessionParams,&transmissionParams);

// 添加一个目的地，RTPSession将往此目的发送RTP数据。这里我们发送给自己。
status = rtpSession.AddDestination(RTPIPv4Address(ntohl(inet_addr("127.0.0.1")),portBase));

//初始化RTP组件，使用我们刚才定义的RTPSession为参数
returnValue = rtpComp.init(&rtpSession);
//初始化RTP解码器
returnValue = rtpDec.init(true, 0, &rtpSession);
//设置RTP解码器使用ulaw规则

returnValue = rtpDec.setPacketDecoder(0,&rtpULawDec);
// 初始化ulaw解码，转换被U-law 解码的采样数据为线性解码采样
returnValue = uLawDec.init();

// 初始化采样编码，转换接受到的音频数据为浮点数形式存储

returnValue = sampEnc2.init(MIPRAWAUDIOMESSAGE_TYPE_FLOAT);

//初始化采样转换,设置参数采样率及声道
returnValue = sampConv2.init(samplingRate, numChannels);
//初始化混音

returnValue = mixer.init(samplingRate, numChannels, interval);
//初始化声卡输出
returnValue = sndCardOutput.open(samplingRate, numChannels, interval);
//声卡输出使用的数据形式

returnValue = sampEnc3.init(MIPRAWAUDIOMESSAGE_TYPE_S16LE);

// 建立整个链：看看这个顺序，这是整个程序工作的逻辑
returnValue = chain.setChainStart(&timer);

returnValue = chain.addConnection(&timer, &sndFileInput);

returnValue = chain.addConnection(&sndFileInput, &sampConv);

returnValue = chain.addConnection(&sampConv, &sampEnc);

returnValue = chain.addConnection(&sampEnc, &uLawEnc);
returnValue = chain.addConnection(&uLawEnc, &rtpEnc);

returnValue = chain.addConnection(&rtpEnc, &rtpComp);
returnValue = chain.addConnection(&rtpComp, &rtpDec);

returnValue = chain.addConnection(&rtpDec, &uLawDec, true);

returnValue = chain.addConnection(&uLawDec, &sampEnc2, true);
returnValue = chain.addConnection(&sampEnc2, &sampConv2, true);

returnValue = chain.addConnection(&sampConv2, &mixer, true);

returnValue = chain.addConnection(&mixer, &sampEnc3);

returnValue = chain.addConnection(&sampEnc3, &sndCardOutput);

//启动这个链

returnValue = chain.start();

//这里添加等待停止的代码

//停止

returnValue = chain.stop();

rtpSession.Destroy();



要想运行这个程序，需要我们的编译器可以找到jrtplib.lib,jrtplib.lib,emiplib.lib 以及 这些lib导出符号需要的一些.H文件。比较好的方法是，把它们分别放到相应目录中，在vc目录中设置。另外，还需要在lib中添加 ws2_32.lib, winmm.lib 。运行时库选项统一为"/MTd" 。其它的问题，可能还需要你在忽略库列表中把一些冲突的库加上,比如libc.lib、libcmt.lib、msvcrt.lib、libcd.lib、msvcrtd.lib ...

源代码中没有给你提供上面所说的那个wav文件，你需要自己找一个。

能听到那个XXX.wav的声音了，大功告成。

第三步：一些介绍

我们来大体了解一下EMIPLIB，以及怎么使用它。

为了提供一个灵活性比较好的框架，此库德目的是建立一些列相关的小组件，每个组件提供一些特定的工作，如音频取样，写音频到文件或者传输音频包等等。这些组件可以放置到一个chain(链)中，在这个链上，这些组件可以相互通信。这样，（按照需求）通过连接这些组件到一条链上，可以建立复杂的引用程序。

另外，EMIPLIB为了大家使用方便也提供了一些更高级的类，它们包装好了一些常用的功能，给我们更好的interface。比如上面hello world中的那一串的定义，初始化，貌似都差不多的chain，MIPAudioSession类都给我们包装好了。如果这些功能已经提供了足够的功能，使用它们就好了，如果你有特殊的要求，你也可以像上面例子一样，自己写那一堆组件，按照你的思路让他们工作。

大部分可以放到[**MIPComponentChain**](http://research.edm.uhasselt.be/emiplib/documentation/classMIPComponentChain.html#67ffc6ba96eb8b20edc3465a0ae83538)上的组件都派生自[**MIPComponent**](http://research.edm.uhasselt.be/emiplib/documentation/classMIPComponent.html)类。我们需要作的工作就是从这些派生类里面选择合适的组件，把它们放在一个chain上面，它就可以开始工作了。即使不使用EMIPLIB提供给我们的包装类，这些工作也不会很繁琐。比较详细的描述，大家可以参考EMIPLIB主页上的Library core部分。

第四步：一个语音会议的demo

最后我们来做一个局域网的语音会议的demo，也算做个入门结业。

我们的语音会议的demo设计为这样：没有服务器，会议中每一方都可以listen来自局域网的声音，也可以往自己的列表中添加目标位置，向它(们)发送你的语音。

这样要完成一个三人的对话，我们需要三方都打开listen，并且添加另外两个人的地址，发送自己的声音。

我们开两个独立的线程，一个负责在端口12000接受来自局域网的声音，一个负责往发送列表中的目的地发送本地语音。

之前我们的例子中只是在本地做了一个wav文件的播放，我们需要解决另外几个问题：得到声卡输入、如何向目的地发送RTP、如何接收RTP。

第一个问题比较好解决，我们仿照例子中声卡播放使用的对象，找到了这个类：MIPWAVInput，把它放在某个chain的最开始处，我们就可以得到声音的输入了。

代码类似于下面：



MIPWinMMInput sndCardInput;

bool returnValue;

int samplingRate = 8000;
int numChannels = 1;
returnValue=sndCardInput.open(samplingRate,numChannels,interval,buffertime,false);



在初始化chain时候：



chain.setChainStart(&sndCardInput);



就可以了。



RTP的发送和接收主要是对于RTPsession的参数设置。接收端代码我们设置端口为监听端口，发送端SetPortbase(portBase)为发送端口，而在RTPSession::AddDestination函数中端口使用目的地的监听端口;



我们的发送语音的线程大致是这样的：



DWORD WINAPI WorkerThread(LPVOID lpParameter)
{
IpAddrList *ipl=(IpAddrList*)lpParameter; //把目的列表传过来鸟

WSADATA dat;
WSAStartup(MAKEWORD(2,2),&dat);

MIPTime interval(0.020); // We'll use 20 millisecond intervals.
MIPTime buffertime(10);
MIPAverageTimer timer(interval);
MIPWAVInput sndFileInput;
MIPSamplingRateConverter sampConv;
MIPSampleEncoder sampEnc;
MIPULawEncoder uLawEnc;
MIPRTPULawEncoder rtpEnc;
MIPRTPComponent rtpComp;


MIPWinMMInput sndCardInput;
MIPWinMMOutput sndCardOutput;
MyChain chain("MPVC");
RTPSession rtpSession;
bool returnValue;

int samplingRate = 8000;
int numChannels = 1;

//We'll start microphone
returnValue=sndCardInput.open(samplingRate,numChannels,interval,buffertime,false);
returnValue = sampEnc.init(MIPRAWAUDIOMESSAGE_TYPE_S16);
returnValue = uLawEnc.init();
returnValue = rtpEnc.init();

RTPUDPv4TransmissionParams transmissionParams;
RTPSessionParams sessionParams;
int portBase = BASEPORT_SEND;
int status;

transmissionParams.SetPortbase(portBase);
sessionParams.SetOwnTimestampUnit(1.0/((double)samplingRate));
sessionParams.SetMaximumPacketSize(64000);

status = rtpSession.Create(sessionParams,&transmissionParams);

for(int i=0;i<ipl->count;i++) //这个for是从一个列表中读目的地址，大家可以自己按自己的要求实现，几个目的地就调用几个AddDestination。
{
status = rtpSession.AddDestination(RTPIPv4Address(ntohl(inet_addr(ipl->ipaddr[i])),BASEPORT_LISTEN));
}

returnValue = rtpComp.init(&rtpSession);
returnValue = chain.setChainStart(&sndCardInput);
returnValue = chain.addConnection(&sndCardInput, &sampEnc);
returnValue = chain.addConnection(&sampEnc, &uLawEnc);
returnValue = chain.addConnection(&uLawEnc, &rtpEnc);
returnValue = chain.addConnection(&rtpEnc, &rtpComp);
returnValue = chain.start();

while(stop_f)
{
Sleep(SLEEP_TIME);
}

rtpSession.BYEDestroy(RTPTime(10,0),0,0);
returnValue = chain.stop();
rtpSession.Destroy();

WSACleanup();
return 0;
}



listen端的代码类似下面：

DWORD WINAPI WorkerThread_Listen(LPVOID lpParameter)
{
WSADATA dat;
WSAStartup(MAKEWORD(2,2),&dat);

MIPTime interval(0.020); // We'll use 20 millisecond intervals.
MIPTime buffertime(10);
MIPAverageTimer timer(interval);
MIPWAVInput sndFileInput;
MIPSamplingRateConverter sampConv, sampConv2;
MIPSampleEncoder sampEnc, sampEnc2, sampEnc3;
MIPULawEncoder uLawEnc;
MIPRTPULawEncoder rtpEnc;
MIPRTPComponent rtpComp;
MIPRTPDecoder rtpDec;
MIPRTPULawDecoder rtpULawDec;
MIPULawDecoder uLawDec;
MIPAudioMixer mixer;

MIPWinMMOutput sndCardOutput;
MyChain chain("MPVClisten");
RTPSession rtpSession;
bool returnValue;

int samplingRate = 8000;
int numChannels = 1;


RTPUDPv4TransmissionParams transmissionParams;
RTPSessionParams sessionParams;

int portBase = BASEPORT_LISTEN;
int status;

transmissionParams.SetPortbase(portBase);
sessionParams.SetOwnTimestampUnit(1.0/((double)samplingRate));
sessionParams.SetMaximumPacketSize(64000);
status = rtpSession.Create(sessionParams,&transmissionParams);

returnValue = rtpComp.init(&rtpSession);
returnValue = rtpDec.init(true, 0, &rtpSession);
returnValue = rtpDec.setPacketDecoder(0,&rtpULawDec);
returnValue = uLawDec.init();
returnValue = sampEnc2.init(MIPRAWAUDIOMESSAGE_TYPE_FLOAT);
returnValue = sampConv2.init(samplingRate, numChannels);
returnValue = mixer.init(samplingRate, numChannels, interval);
returnValue = sndCardOutput.open(samplingRate, numChannels, interval);
returnValue = sampEnc3.init(MIPRAWAUDIOMESSAGE_TYPE_S16LE);
returnValue = chain.setChainStart(&timer);
returnValue = chain.addConnection(&timer, &rtpComp);
returnValue = chain.addConnection(&rtpComp, &rtpDec);
returnValue = chain.addConnection(&rtpDec, &uLawDec, true);
returnValue = chain.addConnection(&uLawDec, &sampEnc2, true);
returnValue = chain.addConnection(&sampEnc2, &sampConv2, true);
returnValue = chain.addConnection(&sampConv2, &mixer, true);
returnValue = chain.addConnection(&mixer, &sampEnc3);
returnValue = chain.addConnection(&sampEnc3, &sndCardOutput);

// Start the chain
returnValue = chain.start();

while(lstop_f)
{
Sleep(SLEEP_TIME);
}

rtpSession.BYEDestroy(RTPTime(10,0),0,0);
returnValue = chain.stop();
rtpSession.Destroy();

WSACleanup();
return 0;
}



其他的一些UI方面的事情大家随意吧...

你要是在你的局域网里面已经已经成功实现了一个语音会议demo...呵呵，恭喜你。我的文章就先写到这里。更高级的...等我会了就写。

大家如果在上面这章有什么问题，可以看看 源代码src/sessions目录下的mipaudiosession.cpp，里面有MIPAudioSession类的实现，它对我们理解这个chain很有帮助。









