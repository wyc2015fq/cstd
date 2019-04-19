# WebRTC国内外介绍系列 - 三少GG - CSDN博客
2011年10月02日 20:09:45[三少GG](https://me.csdn.net/scut1135)阅读数：3915
### Google 的音视频会话开源库WebRTC2011-06-03 14:52:00| 分类： 程序设计大概看了一下，Google好像是想把这个WebRTC加到HTML5里面去吧。WebRTC 视频部分用到自己的VP8，音频使用的收购的一个公司的iLBC库。Google就是好阿，花钱买下一些公司，然后开源让大家免费用。+++++++++++++++++++++++++++++++++++++++++++++++++++++
[](http://blog.csdn.net/markman101/article/details/6739873)
### WebRTC与SIP
2011-09-01 15:57166人阅读[评论](http://blog.csdn.net/markman101/article/details/6739873#comments)(0)[收藏](http://blog.csdn.net/markman101/article/details/6739873)[举报](http://blog.csdn.net/markman101/article/details/6739873#report)
毫无疑问，WebRTC是个好东西。之所以这么说，是因为他居然开源了GIPS的audio引擎。GIPS的回声抑制、噪声消除等方面的技术，几乎独步天下。当年GIPS仅靠这些个算法包，就活得有滋有味。Skype、MSN、QQ等等，凡是做IP语音通信的，都无一例外地使用了GIPS的技术，这里还没包括各硬件芯片厂商。
Google居然将它开源了，牛啊！实在是让人佩服！
既然已经开源了，我们也希望在已有的free项目中引入webrtc的相关模块（主要是EC, NS等）。看了一下webrtc的文档（目前还是非常简陋），忽然有个想法，其实我们没有必要将webrtc的模块引入我们的项目，相反，我们只需要基于webrtc，将我们已经实现的SIP会话层以及GUI层添加到webrtc中。从webrtc的模块分层看，这样似乎更可行一些。
替换掉webrtc的会话层，或者新增SIP会话层似乎都是可行的。不过编译webrtc实在是麻烦，居然要vc2005（还不能是express版本）/ Win7 SDK / DirectX SDK等等，个个都是巨无霸。
另外，这个对Speex项目应该也有影响吧？Speex项目自己实现了一个audio引擎，不过其中的EC，NS等关键部件效果还是不太让人满意，不知道他们会不会从webrtc中获得灵感。
++++++++++++++++++++++++++++++++
### [google开源了WebRTC项目（2）](http://blog.csdn.net/cymlife/article/details/6461345)
分类： [音视频技术](http://blog.csdn.net/cymlife/article/category/837655)2011-06-02
 15:061998人阅读[评论](http://blog.csdn.net/cymlife/article/details/6461345#comments)(0)[收藏](http://blog.csdn.net/cymlife/article/details/6461345)[举报](http://blog.csdn.net/cymlife/article/details/6461345#report)
======cymlife原创，转载请注明。======
WebRTC的视频处理分析（windows平台）
WebRTC的视频部分，包含采集、编解码(I420/VP8)、加密、媒体文件、图像处理、显示、网络传输与流控(RTP/RTCP)等功能。
**视频采集---video_capture**
    源代码在webrtc/modules/video_capture/main目录下，包含接口和各个平台的源代码。
    在windows平台上，WebRTC采用的是dshow技术，来实现枚举视频的设备信息和视频数据的采集，这意味着可以支持大多数的视频采集设备；对那些需要单独驱动程序的视频采集卡（比如海康高清卡）就无能为力了。
    视频采集支持多种媒体类型，比如I420、YUY2、RGB、UYUY等，并可以进行帧大小和帧率控制。
**视频编解码---video_coding**
    源代码在webrtc/modules/video_coding目录下。
    WebRTC采用I420/VP8编解码技术。VP8是google收购ON2后的开源实现，并且也用在WebM项目中。VP8能以更少的数据提供更高质量的视频，特别适合视频会议这样的需求。
**视频加密--video_engine_encryption**
    视频加密是WebRTC的video_engine一部分，相当于视频应用层面的功能，给点对点的视频双方提供了数据上的安全保证，可以防止在Web上视频数据的泄漏。
    视频加密在发送端和接收端进行加解密视频数据，密钥由视频双方协商，代价是会影响视频数据处理的性能；也可以不使用视频加密功能，这样在性能上会好些。
    视频加密的数据源可能是原始的数据流，也可能是编码后的数据流。估计是编码后的数据流，这样加密代价会小一些，需要进一步研究。
**视频媒体文件--media_file**
    源代码在webrtc/modules/media_file目录下。
    该功能是可以用本地文件作为视频源，有点类似虚拟摄像头的功能；支持的格式有Avi。
    另外，WebRTC还可以录制音视频到本地文件，比较实用的功能。
**视频图像处理--video_processing**
    源代码在webrtc/modules/video_processing目录下。
    视频图像处理针对每一帧的图像进行处理，包括明暗度检测、颜色增强、降噪处理等功能，用来提升视频质量。
**视频显示--video_render**
    源代码在webrtc/modules/video_render目录下。
    在windows平台，WebRTC采用direct3d9和directdraw的方式来显示视频，只能这样，必须这样。
**网络传输与流控**
    对于网络视频来讲，数据的传输与控制是核心价值。WebRTC采用的是成熟的RTP/RTCP技术。
