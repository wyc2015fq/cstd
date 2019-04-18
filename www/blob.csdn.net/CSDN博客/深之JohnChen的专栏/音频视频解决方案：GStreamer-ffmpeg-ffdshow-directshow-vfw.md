# 音频视频解决方案：GStreamer-ffmpeg-ffdshow-directshow-vfw - 深之JohnChen的专栏 - CSDN博客

2012年03月03日 23:23:35[byxdaz](https://me.csdn.net/byxdaz)阅读数：5520


linux和window下几种流行的音频视频编程框架作一个总结，防止自己迷惘，免于晕头转向。

一、GStreamer

GStreamer is a library that allows the construction of graphs of media-handlingcomponents, ranging from simple Ogg/Vorbis playback to complex audio (mixing)and video (non-linear editing) processing.
GStreamer是一个开源的多媒体框架库。利用它，可以构建一系列的媒体处理模块，包括从简单的ogg播放功能到复杂的音频（混音）和视频（非线性编辑）的处理。
应用程序可以透明的利用解码和过滤技术。开发者可以使用简洁通用的接口来编写一个简单的插件来添加新的解码器或滤镜
gstreamer是linux上的(也支持其它系统) multi-media framework 多媒体应用框架。一直以来，linux上的多媒体开发都很麻烦，由于没有统一模型，所以很多东西都需要自己实现（或者code copy）。gstreamer的目的就是弥补这一缺陷。gstreamer有一个核心，核心管理许多的elements。每个element都有自己的功能，比如：音量控制、mp3解码、从硬盘读取多媒体数据、把声音发往声卡等。用户变成所需要做的，就是把用得着的elements串成线(pipeline)。多媒体流就可以在pipeline里流动了，一旦pipeline正常工作，就意味着音乐出来了、vedio出来了...pipeline是在单独的线程中运行的，所以你不必担心自己的进程会阻塞。并且pipeline线程的管理(创建、销毁、IPC等)都是gstreamer自己负责的，用户程序只需要对其发出控制信息久可以了。
在Window平台上有DirectShow体系，则在linux平台上有GStreamer框架.

二、ffmpeg

FFmpeg is a complete solution to record, convert and stream audio and video. Itincludes libavcodec, the leading audio/video codec library. FFmpeg is developedunder Linux, but it can compiled under most operating systems, includingWindows.

ffmpeg项目由以下几部分组成: 
ffmpeg 视频文件转换命令行工具,也支持经过实时电视卡抓取和编码成视频文件. 
ffserver 基于HTTP(RTSP正在开发中)用于实时广播的多媒体服务器.也支持时间平移 
ffplay 用 SDL和FFmpeg库开发的一个简单的媒体播放器 
libavcodec 一个包含了所有FFmpeg音视频编解码器的库.为了保证最优性能和高可复用性,大多数编解码器从头开发的. 
libavformat 一个包含了所有的普通音视格式的解析器和产生器的库.

三、ffdshow

ffdshow is DirectShow and VFW codec for decoding/encoding many video and audioformats, including DivX and XviD movies using libavcodec, xvid and otheropensourced libraries with a rich set of postprocessing filters. 

ffdshow是对一些codec（ffmpeg,xvid， and other）的封装，封装成了DirectShow和VFW的标准组件。比如对于xvid来讲，ffdshow是可以选择具体使用那个codec的， ffmpeg(libavcodec) or xvid。那么封装有没有额外的成本哪？有，但对大部分应用来讲，可以忽略不计。就如c++和c。
vfw和dshow里的CODEC分别是通过fourcc码和guid机制寻找的,可以在系统注册codec后调用,比自带编解码库形式更加统一,便于使用。此外，vfw和dshow是代表了两个微软不同时期的音视频处理封装库，里面包含了音视频驱动，音视频处理的一整套方案。 

四、DirectShow
DirectShow是微软公司在ActiveMovie和Videofor Windows的基础上推出的新一代基于COM的流媒体处理的开发包，与DirectX开发包一起发布。目前，DirectX最新版本为9.0。 DirectShow为多媒体流的捕捉和回放提供了强有力的支持。运用DirectShow，我们可以很方便地从支持WDM驱动模型的采集卡上捕获数据，并且进行相应的后期处理乃至存储到文件中。这样使在多媒体数据库管理系统（MDBMS）中多媒体数据的存取变得更加方便。DirectShow是微软公司提供的一套在Windows平台上进行流媒体处理的开发包，与DirectX开发包一起发布。
运用DirectShow，我们可以很方便地从支持WDM驱动模型的采集卡上捕获数据，并且进行相应的后期处理乃至存储到文件中。它广泛地支持各种媒体格式，包括Asf、Mpeg、Avi、Dv、Mp3、Wave等等，使得多媒体数据的回放变得轻而易举。另外，DirectShow还集成了DirectX 其它部分（比如DirectDraw、DirectSound）的技术，直接支持DVD的播放，视频的非线性编辑，以及与数字摄像机的数据交换。

五、vfw
VFW(Videofor Windows)是Microsoft推出的关于数字视频的一个软件开发包，VFW的核心是AVI文件标准。AVI(Audio Video Interleave)文件中的音、视频数据帧交错存放。围绕AVI文件，VFW推出了一整套完整的视频采集、压缩、[解压缩](http://baike.baidu.com/view/845020.htm)、回放和编辑的[应用程序接口](http://baike.baidu.com/view/592964.htm)(API)。它引进[AVI](http://baike.baidu.com/view/7697.htm)的文件标准，该标准未规定如何对视频进行捕获、压缩及播放，仅规定视频和音频该如何存储在硬盘上，在AVI文件中交替存储视频帧和与之相匹配的音频数据。VFW给程序员提供.VBX和AVICap窗口类的高级编程工具，使程序员能通过发送消息或设置属性来捕获、播放和编辑视频剪辑。现在用户不必专门安装VFW了，Windows95本身包括了Videofor Windows1.1，当用户在安装Windows时，安装程序会自动地安装配置视频所需的组件，如设备驱动程序、视频压缩程序等。由于AVI[文件格式](http://baike.baidu.com/view/1066926.htm)推出较早且在数字视频技术中有广泛的应用，所以VFW仍然有很大的实用价值，而且进一步发展的趋势。 

　　VFW主要由以下六个模块组成： 

　　（1）AVICAP.DLL：包含了执行视频捕获的函数，它给AVI文件I/O和视频、音频设备驱动程序提供一个高级接口； 

　　（2）MSVIDEO.DLL：用一套特殊的DrawDib函数来处理屏幕上的视频操作； 

　　（3）MCIAVI.DRV：此驱动程序包括对VFW的MCI命令的解释器 

　　（4）AVIFILE.DLL：支持由标准多媒体I/O（mmio）函数提供的更高的命令来访问.AVI文件； 

　　（5）压缩管理器（ICM）：管理用于视频压缩－解压缩的编解码器（CODEC）； 

　　（6）音频压缩管理器ACM：提供与ICM相似的服务，不同的是它适于波形音频。 

　　Visual C++在支持VFW方面提供有vfw32.lib、 msacm32.lib 、winmm.lib等类似的库。特别是它提供了功能强大、简单易行、类似于MCIWnd的窗口类AVICap。AVICap为应用程序提供了一个简单的、基于消息的接口，使之能访问视频和波形音频硬件，并能在将视频流捕获到硬盘上的过程中进行控制 

在VC++开发环境中调用VFW和使用其它开发包没有什么不同，只是需要将VFW32.lib文件加入工程中，但在开放视频捕捉与压缩管理程序时需要其它软件硬件设置。VFW为AVI文件提供了丰富的处理函数和宏定义，AVI文件的特点在于它是典型的数据流文件，它由视频流、音频流、文本流组成。所以对AVI文件的处理主要是处理文件流。

在Windows平台上建立GStreamer开发环境: http://blog.csdn.net/joliny/article/details/4385585


