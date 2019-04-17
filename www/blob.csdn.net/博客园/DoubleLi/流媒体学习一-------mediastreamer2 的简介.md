# 流媒体学习一-------mediastreamer2 的简介 - DoubleLi - 博客园






Mediastreamer2 是一个功能强大且小巧的流引擎，专门为音视频电话应用而开发。这个库为linphone中所有的接收、发送多媒体流提供处理，包括音/视频捕获，编码和解码，渲染。



特性：
- 接收、发送RTP数据包。
- 支持如下的音视频编解码格式：音频**speex, G711, GSM, iLBC, AMR, AMR-WB, G722, SILK, G729****；** 视频：**H263, theora, MPEG4, H264 and VP8.**
- 捕获摄像头的YUV图片格式(基于平台独立的API)。
- 立体声输出。
- 回声消除，基于speex库 和Android上的webrtc AEC机制。
- 支持音频参数均衡。
- 具有IEC NAT穿越动能。
- 自适应比特率控制算法：根据接收的RTCP反馈信息自动适应编码速率。

mediastream2以扩展插件的形式支持H264, ILBC, SILK, AMR, AMR-WB and G729。


Mediastreamer2可以使用动态插件进行扩展，当前可用的插件有h264，ILBCt和AMR
可移植性： linux/x86、linux/x86_64、Embedded linux:ARM and Blackfin 、windows XP、Vista and 7、 Mac Os X、Google Android



设计和原理：

mediastream2中每一个处理实体都包含一个MSFilter结构，每一个MSFiter有一个或者若干个输入和输出，通过这些输入和输出可以将各个MSFilter连接起来。

下面为一个简单的例子：
- MSSpeexDec：接收输入的音频数据包(假设音频用speex编码)，解码并输出到下一个MSFilter。
- MSRtpRecv --> MSSpeexDec --> MSFileRec  

媒体处理中的调度对象为MSTicker，它一个独立的线程，其每10ms被唤醒一次，然后它会处理他所管理的媒体链的数据。几个MSTicker可以同时运行，例如，一个负责处理音频，一个负责处理视频，或在不同的处理器中运行不同的MSTicker。



使用简单：
如果你想创建音视频流，要调用audiostream.h和videostream.h的定义来创建音视频流。
如果你想添加mediastreamer2的功能，实现一个mediastreamer2 filter是非常简单的，因为mediastreamer2是非常轻量级的封装。
由于mediastreamer2是一个轻框架，工程师可以集中精力在于：图像/信号算法的实现。

mediastreamer2适用于嵌入式系统
mediastreamer2是纯C开发的
mediastreamer2可以用gcc编译
mediastreamer2最小依赖只需oRTP和libc。其他（ffmpeg, speex, alsa...）根据需要添加。
由于是插件框架，mediastreamer2可以通过硬件codecs接口扩展，如video codecs dsp
ediastreamer2是轻量级的。例如，在linux/x86平台上，使用共享库的时全功能大约只有800k









