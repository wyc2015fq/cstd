# FreeSWITCH第三方库（视频）的简单介绍（二） - DoubleLi - 博客园







FreeSWITCH使用了大量的第三方库，本文档主要介绍视频相关库的信息：

音频相关库的信息介绍参考：[http://www.cnblogs.com/yoyotl/p/5486753.html](http://www.cnblogs.com/yoyotl/p/5486753.html)

其他相关库的信息介绍参考：[http://www.cnblogs.com/yoyotl/p/5489315.html](http://www.cnblogs.com/yoyotl/p/5489315.html)



```
① openh264
```

OpenH264 是思科公司以BSD协议开放的一个开源的 H.264 编码器和解码器。

Cisco 将以 BSD 协议开放其 H264 解码器实现，并提供针对主流操作系统的预编译库文件下载。Cisco 将负责承担 MPEG LA 的专利使用费，并和 Mozilla 共同管理 OpenH264 开源项目。

Firefox 以后默认支持H264，但依然支持 VP8 格式，并可在 WebRTC 中使用。

Mozilla 已经组成了开源编码器专家小组开发下一代无专利限制视频编码格式 Daala，将以与 H265 和 VP9 截然不同的方式绕过可能的专利雷区。但是依然还需要解决同样被专利困扰的 AAC 音频解码器。



```
② libav
```

Libav是一个自由软件，可以执行音讯和视讯多种格式的录影、转档、串流功能。主要包含以下几个部分：

libavcodec （一个用于多个专案中音讯和视讯的解码器函式库）

libavformat （一个音讯与视讯格式转换函式库）

libavutil（包含解压缩和各种实用功能库）

libavfilter（提供一个改变解码音频和视频的拦截器链）

libavdevice（提供捕获和重置设备的抽象访问能力）

libavresample（实现音频混合和重采样能力）

libswscale（实现颜色的转换和缩放能力）



```
③ libvpx
```

libvpx是开源的支持VP8/VP9编解码的SDK。

全面支持WebM。x86平台必须先安装yasm库。

WebM由Google提出，是一个开放、免费的媒体文件格式。

WebM 影片格式其实是以 Matroska（即 MKV）容器格式为基础开发的新容器格式，里面包括了 VP8 影片轨和 Ogg Vorbis 音轨。

其中Google将其拥有的VP8视频编码技术以类似BSD授权开源，Ogg Vorbis 本来就是开放格式。

WebM标准的网络视频更加偏向于开源并且是基于HTML5标准的，WebM 项目旨在为对每个人都开放的网络开发高质量、开放的视频格式，其重点是解决视频服务这一核心的网络用户体验。



```
④ libyuv
```

libyuv是Google开源的实现各种YUV与RGB之间相互转换、旋转、缩放的库。

它是跨平台的，可在Windows、Linux、Mac、Android等操作系统，x86、x64、arm架构上进行编译运行，支持SSE、AVX、NEON等SIMD指令加速。



```
⑤ vlc
```

vlc是一个流行的开源媒体播放器和多媒体引擎，已移植到大多数的计算平台，包括GNU / Linux，Windows，Mac OS X，BSD，iOS和Android。

vlc可以播放大多数多媒体文件，光盘，流，允许回放设备，并能够以各种格式转换媒体流。



```
⑥ libpng
```

PNG的官方库，项目开发包的地址：[http://libpng.sourceforge.net/](http://libpng.sourceforge.net/index.html)。

该项目依赖zlib库，当前最新的版本是libpng 1.6.21。



```
⑦ librtmp
```

实时流协议（Real-TimeMessaging Protocol，RTMP）是用于互联网上传输视音频数据的网络协议。librtmp库提供了支持RTMP， RTMPT,RTMPE, RTMP RTMPS及以上几种协议的变种(RTMPTE, RTMPTS)协议所需的大部分客户端功能以及少量的服务器功能。




上善若水，水利万物而不争。









