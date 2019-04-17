# webrtc开源代码阅读1 分享 - 一世豁然的专栏 - CSDN博客





2017年05月16日 15:31:55[一世豁然](https://me.csdn.net/Explorer_day)阅读数：344








本博客转载于：[http://blog.sina.com.cn/s/blog_40d608bb01010n73.html](http://blog.sina.com.cn/s/blog_40d608bb01010n73.html)









一、简介。

    WebRTC是一项在浏览器内部进行实时视频和音频通信的技术，是谷歌2010年以6820万美元收购收购Global IT Solutions公司而获得一项技术。

    在今年6月2日，也就是前天，WebRTC正式被Google向公众开放了其源代码。网址是：[http://code.google.com/p/webrtc/WebRTC](http://code.google.com/p/webrtc/WebRTC)，WebRTC使用了iLBC
 iSAC，G711和G722语音协议以及VP8视频协议。实现了一种基于网页的视频会议，开发者可以通过浏览器提供简单的javascript就可以达到实时通讯（RTC）能力。开发人员也可以访问 [http://sites.google.com/site/webrtc](http://sites.google.com/site/webrtc) 页面获取WebRTC的源代码、规格说明和工具等。

二、源码分析：

    WebRTC源码文件如下：

![](http://1882.img.pp.sohu.com.cn/images/blog/2011/6/4/9/3/u78925689_1311318fd86g214.jpg)

    1，总体上，WebRTC提供了视频会议的核心技术，包括音视频的采集、编解码、网络传输、显示等功能，并且还支持跨平台：windows，linux，mac，android等操作系统。

    2，WebRTC视频处理分析：

        WebRTC视频部分主要包括采集、编解码（I420/VP8）、加密、媒体文件、图像处理、显示、网络传输与流控（RTP/RTCP）等功能。

**视频采集--video_capture**：

        源代码在webrtc\modules\video_capture\main目录下，包含接口和各个平台的源代码。如图所示：

![](http://1812.img.pp.sohu.com.cn/images/blog/2011/6/4/9/12/u78925689_13113217a8cg213.jpg)        在windows平台上，WebRTC采用的是dshow技术，来实现枚举视频的设备信息和视频数据的采集，这意味着可以支持大多数的视频采集设备；对那些需要单独驱动程序的视频采集卡（比如海康高清卡）就无能为力了。

        视频采集支持多种媒体类型，比如I420、YUY2、RGB、UYUY等，并可以进行帧大小和帧率控制。

**视频编解码---video_coding**

        源代码在webrtc\modules\video_coding目录下。如图所示：

![](http://1823.img.pp.sohu.com.cn/images/blog/2011/6/4/9/16/u78925689_13113257191g213.jpg)

        WebRTC采用I420/VP8编解码技术。VP8是google收购ON2后的开源实现，并且也用在WebM项目中。VP8能以更少的数据提供更高质量的视频，特别适合视频会议这样的需求。

**视频加密--video_engine_encryption**

        源代码在webrtc\video_engine\main中

        视频加密是WebRTC的video_engine一部分，相当于视频应用层面的功能，给点对点的视频双方提供了数据上的安全保证，可以防止在Web上视频数据的泄漏。

        视频加密在发送端和接收端进行加解密视频数据，密钥由视频双方协商，代价是会影响视频数据处理的性能；也可以不使用视频加密功能，这样在性能上会好些。

        视频加密的数据源可能是原始的数据流，也可能是编码后的数据流。估计是编码后的数据流，这样加密代价会小一些，需要进一步研究。

**视频媒体文件--media_file**

        源代码在webrtc\modules\media_file目录下。

        该功能是可以用本地文件作为视频源，有点类似虚拟摄像头的功能；支持的格式有Avi。另外，WebRTC还可以录制音视频到本地文件，比较实用的功能。
**视频图像处理--video_processing**

        源代码在webrtc\modules\video_processing目录下。

        视频图像处理针对每一帧的图像进行处理，包括明暗度检测、颜色增强、降噪处理等功能，用来提升视频质量。

**视频显示--video_render**

        源代码在webrtc\modules\video_render目录下。

        在windows平台，WebRTC采用direct3d9和directdraw的方式来显示视频，只能这样，必须这样。
**       网络传输与流控**

        源代码在

        对于网络视频来讲，数据的传输与控制是核心价值。WebRTC采用的是成熟的RTP/RTCP技术。

    3，WebRTC的音频处理分析：

        WebRTC的音频部分，包含设备、编解码(iLIBC/iSAC/G722/PCM16/RED/AVT、NetEQ)、加密、声音文件、声音处理、声音输出、音量控制、音视频同步、网络传输与流控(RTP/RTCP)等功能。      

**音频设备---audio_device**        源代码在webrtc\modules\audio_device\main目录下，包含接口和各个平台的源代码。

        在windows平台上，WebRTC采用的是Windows Core Audio和Windows Wave技术来管理音频设备，还提供了一个混音管理器。

        利用音频设备，可以实现声音输出，音量控制等功能。

**音频编解码---audio_coding**        源代码在webrtc\modules\audio_coding目录下。

        WebRTC采用iLIBC/iSAC/G722/PCM16/RED/AVT编解码技术。

        WebRTC还提供NetEQ功能---抖动缓冲器及丢包补偿模块，能够提高音质，并把延迟减至最小。

        另外一个核心功能是基于语音会议的混音处理。

**声音加密--voice_engine_encryption **

源代码在webrtc\voice_engine\main

        和视频一样，WebRTC也提供声音加密功能。

**声音文件--media_file **源代码在webrtc\modules\media_file目录下

        该功能是可以用本地文件作为音频源，支持的格式有Pcm和Wav。同样，WebRTC也可以录制音频到本地文件。

**声音处理--audio_processing**        源代码在webrtc\modules\audio_processing目录下。

        声音处理针对音频数据进行处理，包括回声消除(AEC)、AECM、自动增益(AGC)、降噪处理等功能，用来提升声音质量。

**网络传输与流控**

        和视频一样，WebRTC采用的是成熟的RTP/RTCP技术。

    4，在webrtc\peerconnection文件夹下有一个通讯的例子。


后续准备将每一个模块详细的分析一下。



