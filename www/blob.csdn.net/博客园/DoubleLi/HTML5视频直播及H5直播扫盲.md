# HTML5视频直播及H5直播扫盲 - DoubleLi - 博客园






章来源：[http://geek.csdn.net/news/detail/95188](http://geek.csdn.net/news/detail/95188)

分享内容简介： 
目前视频直播，尤其是移动端的视频直播已经火到不行了，基本上各大互联网公司都有了自己的直播产品，所以对于直播的一些基本知识和主要技术点也要有所了解，本次分享就向大家介绍一下其中的奥秘。

内容大体框架: 
1. 怎样利用 HTML5 来播放直播视频 
2. 怎样录制直播视频 
3. 怎样实时上传直播视频 
4. 直播中的用户交互

分享人介绍：

吕鸣 目前在腾讯SNG担任手Q的web前端开发工作 
博客：[http://www.nihaoshijie.com.cn/](http://www.nihaoshijie.com.cn/)

下面是本期分享内容整理

Hello, 大家好，我是吕鸣，目前是在腾讯 SNG 的即通应用部负责手Q的兴趣部落 Web 前端开发工作。

针对目前比较火的视频直播，我做了一些研究和探索，同时我们的项目将会用到直播为此打下技术基础，下面就向大家分享一下直播的整个流程和一些技术点。

## 一、移动视频直播发展

大家首先来看下面这张图:

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/101.jpg)

可以看到，直播从 PC 到一直发展到移动端，越来越多的直播类 App 上线，同时移动直播进入了前所未有的爆发阶段，但是对于大多数移动直播来说，还是要以 Native 客户端实现为主，但是 HTML5 在移动直播端也承载着不可替代的作用，例如 HTML5 有着传播快，易发布的优势，同时最为关键的时 HTML5 同样可以播放直播视频。

大家可以看下面这张大概的实现图

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/102.png)

完整的直播可以分为以下几块：
- 视频录制端：一般是电脑上的音视频输入设备或者手机端的摄像头或者麦克风，目前以移动端的手机视频为主。
- 视频播放端：可以是电脑上的播放器，手机端的 Native 播放器，还有就是 HTML5 的 `video` 标签等，目前还是已手机端的 Native 播放器为主。
- 视频服务器端：一般是一台 nginx 服务器，用来接受视频录制端提供的视频源，同时提供给视频播放端流服务。

大家可以看下大致的结构图：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/103.jpg)

## 二、HTML5 录制视频：

对于HTML5视频录制，可以使用强大的 webRTC （Web Real-Time Communication）是一个支持网页浏览器进行实时语音对话或视频对话的技术，缺点是只在 PC 的 Chrome 上支持较好，移动端支持不太理想。

使用 webRTC 录制视频基本流程是：
- 调用 `window.navigator.webkitGetUserMedia()` 获取用户的PC摄像头视频数据。
- 将获取到视频流数据转换成 `window.webkitRTCPeerConnection` (一种视频流数据格式)。
- 利用 webscoket 将视频流数据传输到服务端

由于许多方法都要加上浏览器前缀，所以很多移动端的浏览器还不支持 webRTC，所以真正的视频录制还是要靠客户端（iOS,Android）来实现,效果会好一些。

## 三、HTML5 播放直播视频：

对于视频播放，可以使用 HLS(HTTP Live Streaming)协议播放直播流，iOS和 Android 都天然支持这种协议，配置简单，直接使用 `video` 标签即可。

下面是简单的代码使用 `video` 播放直播视频：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/104.jpg)

### 1.什么是 HLS 协议:

简单讲就是把整个流分成一个个小的，基于 HTTP 的文件来下载，每次只下载一些，前面提到了用于 HTML5 播放直播视频时引入的一个 .m3u8 的文件，这个文件就是基于 HLS 协议，存放视频流元数据的文件。

每一个 .m3u8 文件，分别对应若干个 ts 文件，这些 ts 文件才是真正存放视频的数据，m3u8 文件只是存放了一些 ts 文件的配置信息和相关路径，当视频播放时，.m3u8 是动态改变的，`video` 标签会解析这个文件，并找到对应的 ts 文件来播放，所以一般为了加快速度，.m3u8 放在 Web 服务器上，ts 文件放在 CDN 上。

.m3u8 文件，其实就是以 UTF-8 编码的 m3u 文件，这个文件本身不能播放，只是存放了播放信息的文本文件。

打开之后就是这个样子：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/105.jpg)

下面这个是 ts 文件，就是存放视频的文件：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/106.jpg)

### 2.HLS 的请求流程：
- HTTP 请求 m3u8 的 url。
- 服务端返回一个 m3u8 的播放列表，这个播放列表是实时更新的，一般一次给出5段数据的 url。
- 客户端解析 m3u8 的播放列表，再按序请求每一段的 url，获取 ts 数据流。

大概是这个流程：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/107.jpg)

### 3.HLS 直播延时:

我们知道 hls 协议是将直播流分成一段一段的小段视频去下载播放的，所以假设列表里面的包含5个 ts 文件，每个 TS 文件包含5秒的视频内容，那么整体的延迟就是25秒。因为当你看到这些视频时，主播已经将视频录制好上传上去了，所以时这样产生的延迟。当然可以缩短列表的长度和单个 ts 文件的大小来降低延迟，极致来说可以缩减列表长度为1，并且 ts 的时长为1s，但是这样会造成请求次数增加，增大服务器压力，当网速慢时回造成更多的缓冲，所以苹果官方推荐的 ts 时长时10s，所以这样就会大改有30s的延迟。所以服务器接收流，转码，保存，切块，再分发给客户端，这里就延时的根本原因。

更多关于延迟的问题可以参考苹果官方地址： 
[https://developer.apple.com/library/ios/documentation/NetworkingInternet/Conceptual/StreamingMediaGuide/FrequentlyAskedQuestions/FrequentlyAskedQuestions.html](https://developer.apple.com/library/ios/documentation/NetworkingInternet/Conceptual/StreamingMediaGuide/FrequentlyAskedQuestions/FrequentlyAskedQuestions.html)

但是 HTML5 直播视频却有一些不可替代的优势:
- 传播性好，利于分享等操作。
- 可以动态发布，有利于实时迭代产品需求并迅速上线。
- 不用安装 App，直接打开浏览器即可。

## 四、iOS 采集（录制）音视频数据OS

关于音视频采集录制，首先明确下面几个概念：
- 视频编码：所谓视频编码就是指通过特定的压缩技术，将某个视频格式的文件转换成另一种视频格式文件的方式，我们使用的 iPhone 录制的视频，必须要经过编码，上传，解码，才能真正的在用户端的播放器里播放。
- 编解码标准：视频流传输中最为重要的编解码标准有国际电联的 H.261、H.263、H.264，其中 HLS 协议支持 H.264 格式的编码。
- 音频编码：同视频编码类似，将原始的音频流按照一定的标准进行编码，上传，解码，同时在播放器里播放，当然音频也有许多编码标准，例如 PCM 编码，WMA 编码，AAC 编码等等，这里我们 HLS 协议支持的音频编码方式是 AAC 编码。

利用 iOS 上的摄像头，进行音视频的数据采集，主要分为以下几个步骤：
- 音视频的采集，iOS 中，利用 AVCaptureSession 和 AVCaptureDevice 可以采集到原始的音视频数据流。
- 对视频进行 H264 编码，对音频进行 AAC 编码，在 iOS 中分别有已经封装好的编码库来实现对音视频的编码。
- 对编码后的音、视频数据进行组装封包；
- 建立 RTMP 连接并上推到服务端。

下面是具体的采集音视频数据的流程：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/108.jpg)

### 1.关于 RTMP：

Real Time Messaging Protocol（简称 RTMP）是 Macromedia 开发的一套视频直播协议，现在属于 Adobe。和 HLS 一样都可以应用于视频直播，区别是 RTMP 基于 flash 无法在 iOS 的浏览器里播放，但是实时性比 HLS 要好。所以一般使用这种协议来上传视频流，也就是视频流推送到服务器。

下面是 HLS 和 RTMP 的对比：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/109.jpg)

### 2.推流

所谓推流，就是将我们已经编码好的音视频数据发往视频流服务器中，在 iOS 代码里面一般常用的是使用 RTMP 推流，可以使用第三方库 librtmp-iOS 进行推流，librtmp 封装了一些核心的 API 供使用者调用。例如推流 API 等等，配置服务器地址，即可将转码后的视频流推往服务器。

那么如何搭建一个推流服务器呢？

简单的推流服务器搭建，由于我们上传的视频流都是基于 RTMP 协议的，所以服务器也必须要支持 RTMP 才行，大概需要以下几个步骤：
- 安装一台 nginx 服务器。
- 安装 nginx 的 RTMP 扩展，目前使用比较多的是 [https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)
- 配置 nginx 的 conf 文件
- 重启 nginx，将 RTMP 的推流地址写为 rtmp://ip:1935/hls/mystream， 其中 hls_path 表示生成的 .m3u8 和 ts 文件所存放的地址，hls_fragment 表示切片时长，mysteam 表示一个实例，即将来要生成的文件名可以先自己随便设置一个。

更多配置可以参考：[https://github.com/arut/nginx-rtmp-module/wiki/](https://github.com/arut/nginx-rtmp-module/wiki/)

下面是 nginx 的配置文件

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/110.jpg)

## 五、直播中的用户交互：

对于直播中的用户交互大致可以分为：
- 送礼物
- 发表评论或者弹幕

对于送礼物，在 HTML5 端可以利用 DOM 和 CSS3 实现送礼物逻辑和一些特殊的礼物动画，实现技术难点不大。

对于弹幕来说，要稍微复杂一些，可能需要关注以下几点：
- 弹幕实时性，可以利用 webscoket 来实时发送和接收新的弹幕并渲染出来。
- 对于不支持 webscoket 的浏览器来说，只能降级为长轮询或者前端定时器发送请求来获取实时弹幕。
- 弹幕渲染时的动画和碰撞检测(即弹幕不重叠)等等

## 六、总结

目前较为成熟的直播产品，大致都是以 Server 端和 HTML5 和 Native(android,ios)搭配实现直播：

基本是下图这个套路：

![](http://oa5504rxk.bkt.clouddn.com/dev_Club_07/111.jpg)

所以 HTML5 在整个直播中，还是有着重要的地位的！

## Demo 分享

最后，根据本次分享的内容，我这边实现了一个 iOS 端录制，推流，NGINX 接收流，同时分发的 HLS 直播流的一整套 Demo，感兴趣的同学可以看下面这个链接:

[https://github.com/lvming6816077/LMVideoTest](https://github.com/lvming6816077/LMVideoTest)

好了，本次分享先到这里了，谢谢大家~

## 互动问答环节

Q1: Demo 包含 iOS 端的 RTMP 播放不？

> 
答：Demo 里面没有 RTMP 的播放，Demo 主要是提供录制，推流的。


Q2: 对于 HTML5 HLS 播放 卡顿问题，前端与 server 端，有什么配置上的优化吗？

> 
答：server 端要做好分片策略，同时要将 ts 文件放在 CDN 上，前端这边可以尽量做到 DNS 缓存等，由于HTML5是使用的 video 标签，所以要修改 video 的播放优化，还是不那么容易。


Q3: 在手机推流时的码率是根据怎样的策略做选择的？不同机型和网络下如何保持流畅？

> 
答：可以提供不同的视频码率来供用户选择，例如网速差的可以选择较为低清晰度的码率，网络好的用户可以选择更加清晰的码率，同时做好视频播放端的容错和异常处理等等。


Q4: RTMP 比起 HTTP 他的优势主要是几种在哪里？

> 
答：RTMP 是基于 TCP 的保持的是长连接，而 HTTP 是一次性的，每次都要三次握手，所以对于直播来说还是 RTMP 好一些


Q5: 据我所知 nginx rtmp-module 好像性能不是很高…..为什么会采用这个来作为后端服务？

> 
答：这里只是 Demo 用了这个 nginx rtmp-module，其实也可已选择 SRS(simple-rtmp-server)都是可以的哈


Q6: 移动端这边怎么进行编码转码？用 ffmpeg 编译时很麻烦

> 
答：关于 iOS 这边，其实不用关心转码问题，因为已经有了很多开源的库提供给我们了例如： 
x264 编码：[https://github.com/kewlbear/x264-ios](https://github.com/kewlbear/x264-ios)
faac 编码：[https://github.com/fflydev/faac-ios-build](https://github.com/fflydev/faac-ios-build)


Q7: 您介绍的都是 Native 播放和还有 HTML5 的 video 标签播放, iOS 端有没有考虑过整个用原生的 OC 或者 Swift 实现?

> 
答：关于播放端，其实真正体验好的还是要用 native 来实现的，而且 native 实现可以用 RTMP 来播放直播，延迟会好很多，HTML5 来播直播主要是考虑到易传播性好。


Q8: 在用户非常多的情况下，或者网络慢的情况下，有什么策略可以保证质量？

> 
答：可以提供不同的视频码率来供用户选择，例如网速差的可以选择较为低清晰度的码率，网络好的用户可以选择更加清晰的码率，同时做好视频播放端的容错和异常处理等等。


Q9: 请问直播这块的测试中关注的几个指标是什么，有什么比较好的测试方法呢？

> 
答：主要就是： 
1. 首次打开的白屏时间 
2. 直播中的卡顿和缓冲 
3. 直播的延时


Q10: 您提供的 Demo 为什么不是 HTML5 的呢 iOS 推流和 nginx 服务器都有，能不能提供一个前面第二张叶子美女直播那个页面的 Demo？

> 
答：这个 Demo 你下载下拉运行的话，根据配置就可直接自己实现一个利用 HTML5 直播的页面，很简单，就像使用 video 标签一样，其他的样式你可以自己定制的。


Q11: HLS 的延时有没有比较好的方法解决？

> 
答：HLS 确实是会有延迟，相对比较优的策略是调整好分片策略，在保证性能的情况下，和延迟达到平衡。


Q12: 如果加入视频电话功能，上面的结构需要作什么改变？视频电话的目的大概是：直播可以选择某一观众或者多个观众视频对话

> 
答：视频电话，也就是说作为视频录制端的同时也作为视频播放端，所以实现实时电话简单就是：我在直播的同时观看别人的直播视频，别人在直播的同时观看我的直播视频，可以这样理解，上面的结构复制一份对调即可。


Q13: 如何实现滤镜功能？

> 
答：一般是在视频录制之后，在转码前给视频数据增加滤镜功能，在 iOS 里可以使用一些滤镜库等等实现滤镜功能


Q14: 在 App 端如果不利用 HTML5 能实现直播吗？

> 
答：可以啊，app 有更加丰富的播放接口，和开源播放器可以实现直播的。


Q15: 既然 HLS 有较高的延迟 为什么苹果推荐的的方式却是 HLS？

> 
答：并不是说苹果主要推荐使用 HLS，对于 HTML5 来说目前只有这一种比较好的方式来播放直播视频，所以还是很期待苹果能对延迟问题做一些改进的。


Q16: 同滤镜问题，音频变声是如何实现的？

> 
答：同样是可以在对音频转码前操作。


Q17: 如果针对网络较差的观看用户，是需要直播推流到服务器后做多份不同分辨率的拷贝，以适应不同网络的用户观看？如果是这样的话，对延迟会不会影响很大？ 毕竟编解码也是需要时间的.

> 
答：这个其实本身就应该做的，对于网络差的用户，完全可以提供给他们较低码率的直播流来减少卡顿问题，延迟问题的话还是要根据具体使用哪种协议来定。


Q18: 推流目前大部分都是第三方在做，难度点在哪？然后目前业内比较成熟的主要哪些？

> 
答：难点主要是服务器端的性能压力和分发直播流的效率，业界都已经有了较成熟的方案，例如腾讯云的直播。


[点此查看 Dev Club 往期分享汇总](http://dev.qq.com/topic/57918dc9bbd0729d27abeb50)

[腾讯 Bugly](https://bugly.qq.com/)是一款专为移动开发者打造的质量监控工具，帮助开发者快速，便捷的定位线上应用崩溃的情况以及解决方案。智能合并功能帮助开发同学把每天上报的数千条[Crash](https://bugly.qq.com/) 根据根因合并分类，每日日报会列出影响用户数最多的崩溃，精准定位功能帮助开发同学定位到出问题的代码行，实时上报可以在发布后快速的了解应用的质量情况，适配最新的 iOS、Android 官方操作系统。



下面是H5相关扫盲方面的知识：



文章来源：[https://www.nihaoshijie.com.cn/index.php/archives/615](https://www.nihaoshijie.com.cn/index.php/archives/615)

视频直播这么火，再不学就out了。

为了紧跟潮流，本文将向大家介绍一下视频直播中的基本流程和主要的技术点，包括但不限于前端技术。

## 1 H5到底能不能做视频直播？

当然可以， H5火了这么久，涵盖了各个方面的技术。

对于视频录制，可以使用强大的webRTC（Web Real-Time Communication）是一个支持网页浏览器进行实时语音对话或视频对话的技术，缺点是只在PC的chrome上支持较好，移动端支持不太理想。

对于视频播放，可以使用HLS(HTTP Live Streaming)协议播放直播流，ios和android都天然支持这种协议，配置简单，直接使用video标签即可。

webRTC兼容性：

![](https://oc5n93kni.qnssl.com/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202016-05-23%2010.24.48.png)

video标签播放hls协议视频：
<video controls autoplay>  
    <source src="http://10.66.69.77:8080/hls/mystream.m3u8" type="application/vnd.apple.mpegurl" />  
    <p class="warning">Your browser does not support HTML5 video.</p>  
</video>
## 2 到底什么是HLS协议？

当简单讲就是把整个流分成一个个小的基于HTTP的文件来下载，每次只下载一些，前面提到了用于H5播放直播视频时引入的一个.m3u8的文件，这个文件就是基于HLS协议，存放视频流元数据的文件。

每一个.m3u8文件，分别对应若干个ts文件，这些ts文件才是真正存放视频的数据，m3u8文件只是存放了一些ts文件的配置信息和相关路径，当视频播放时，.m3u8是动态改变的，video标签会解析这个文件，并找到对应的ts文件来播放，所以一般为了加快速度，.m3u8放在web服务器上，ts文件放在cdn上。

.m3u8文件，其实就是以UTF-8编码的m3u文件，这个文件本身不能播放，只是存放了播放信息的文本文件：
​#EXTM3U                     m3u文件头
#EXT-X-MEDIA-SEQUENCE       第一个TS分片的序列号
#EXT-X-TARGETDURATION       每个分片TS的最大的时长
#EXT-X-ALLOW-CACHE          是否允许cache
#EXT-X-ENDLIST              m3u8文件结束符
#EXTINF                     指定每个媒体段(ts)的持续时间（秒），仅对其后面的URI有效
mystream-12.ts
ts文件：

![](https://oc5n93kni.qnssl.com/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202016-05-23%2011.09.29.png)

HLS的请求流程是：
1 http请求m3u8的url
2 服务端返回一个m3u8的播放列表，这个播放列表是实时跟新的，一般一次给出3段数据的url
3 客户端解析m3u8的播放列表，再按序请求每一段的url，获取ts数据流

简单流程：

![](https://oc5n93kni.qnssl.com/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202016-05-23%2011.20.29.png)

## 3 HLS直播延时

当我们知道hls协议是将直播流分成一段一段的小段视频去下载播放的，所以假设列表里面的包含5个TS文件，每个TS文件包含5秒的视频内容，那么整体的延迟就是25秒。因为当你看到这些视频时，主播已经将视频录制好上传上去了，所以时这样产生的延迟。当然可以缩短列表的长度和单个TS文件的大小来降低延迟，极致来说可以缩减列表长度为1，并且TS的时长为1s，但是这样会造成请求次数增加，增大服务器压力，当网速慢时回造成更多的缓冲，所以苹果官方推荐的ts时长时10s，所以这样就会大改有30s的延迟。参考资料：[https://developer.apple.com/library/ios/documentation/NetworkingInternet/Conceptual/StreamingMediaGuide/FrequentlyAskedQuestions/FrequentlyAskedQuestions.html](https://developer.apple.com/library/ios/documentation/NetworkingInternet/Conceptual/StreamingMediaGuide/FrequentlyAskedQuestions/FrequentlyAskedQuestions.html)

## 4 视频直播的整个流程是什么？

当视频直播可大致分为：

1 视频录制端：一般是电脑上的音视频输入设备或者手机端的摄像头或者麦克风，目前已移动端的手机视频为主。

2 视频播放端：可以是电脑上的播放器，手机端的native播放器，还有就是h5的video标签等，目前还是已手机端的native播放器为主。

3 视频服务器端：一般是一台nginx服务器，用来接受视频录制端提供的视频源，同时提供给视频播放端流服务。

简单流程：

![](https://oc5n93kni.qnssl.com/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202016-05-23%2011.33.20.png)





## 5 怎样进行音视频采集？

当首先明确几个概念：

视频编码：所谓视频编码就是指通过特定的压缩技术，将某个视频格式的文件转换成另一种视频格式文件的方式，我们使用的iphone录制的视频，必须要经过编码，上传，解码，才能真正的在用户端的播放器里播放。

编解码标准：视频流传输中最为重要的编解码标准有国际电联的H.261、H.263、H.264，其中HLS协议支持H.264格式的编码。
音频编码：同视频编码类似，将原始的音频流按照一定的标准进行编码，上传，解码，同时在播放器里播放，当然音频也有许多编码标准，例如PCM编码，WMA编码，AAC编码等等，这里我们HLS协议支持的音频编码方式是AAC编码。

下面将利用ios上的摄像头，进行音视频的数据采集，主要分为以下几个步骤：



1 音视频的采集，ios中，利用AVCaptureSession和AVCaptureDevice可以采集到原始的音视频数据流。
2 对视频进行H264编码，对音频进行AAC编码，在ios中分别有已经封装好的编码库来实现对音视频的编码。
3 对编码后的音、视频数据进行组装封包；
4 建立RTMP连接并上推到服务端。



ps：由于编码库大多使用c语言编写，需要自己使用时编译，对于ios，可以使用已经编译好的编码库。

x264编码：[https://github.com/kewlbear/x264-ios](https://github.com/kewlbear/x264-ios)

faac编码：[https://github.com/fflydev/faac-ios-build](https://github.com/fflydev/faac-ios-build)

ffmpeg编码：[https://github.com/kewlbear/FFmpeg-iOS-build-script](https://github.com/kewlbear/FFmpeg-iOS-build-script)

关于如果想给视频增加一些特殊效果，例如增加滤镜等，一般在编码前给使用滤镜库，但是这样也会造成一些耗时，导致上传视频数据有一定延时。

简单流程：

![](https://oc5n93kni.qnssl.com/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202016-05-23%2012.07.49.png)

## 6 前面提到的ffmpeg是什么？

和之前的x264一样，ffmpeg其实也是一套编码库，类似的还有Xvid，Xvid是基于MPEG4协议的编解码器，x264是基于H.264协议的编码器，ffmpeg集合了各种音频，视频编解码协议，通过设置参数可以完成基于MPEG4,H.264等协议的编解码，demo这里使用的是x264编码库。

## 7 什么是RTMP？

Real Time Messaging Protocol（简称 RTMP）是 Macromedia 开发的一套视频直播协议，现在属于 Adobe。和HLS一样都可以应用于视频直播，区别是RTMP基于flash无法在ios的浏览器里播放，但是实时性比HLS要好。所以一般使用这种协议来上传视频流，也就是视频流推送到服务器。

这里列举一下hls和rtmp对比：

![](https://oc5n93kni.qnssl.com/%E5%B1%8F%E5%B9%95%E5%BF%AB%E7%85%A7%202016-05-25%2010.43.02.png)

## 8 推流

简所谓推流，就是将我们已经编码好的音视频数据发往视频流服务器中，一般常用的是使用rtmp推流，可以使用第三方库[librtmp-iOS](https://github.com/ifactorylab/librtmp-iOS)进行推流，librtmp封装了一些核心的api供使用者调用，如果觉得麻烦，可以使用现成的ios视频推流sdk，也是基于rtmp的，[https://github.com/runner365/LiveVideoCoreSDK](https://github.com/runner365/LiveVideoCoreSDK)

## 9 推流服务器搭建

简简单的推流服务器搭建，由于我们上传的视频流都是基于rtmp协议的，所以服务器也必须要支持rtmp才行，大概需要以下几个步骤：

1 安装一台nginx服务器。

2 安装nginx的rtmp扩展，目前使用比较多的是[https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)

3 配置nginx的conf文件：
rtmp {  
  
    server {  
  
        listen 1935;  #监听的端口
  
        chunk_size 4000;  
        
         
        application hls {  #rtmp推流请求路径
            live on;  
            hls on;  
            hls_path /usr/local/var/www/hls;  
            hls_fragment 5s;  
        }  
    }  
}
4 重启nginx，将rtmp的推流地址写为rtmp://ip:1935/hls/mystream，其中hls_path表示生成的.m3u8和ts文件所存放的地址，hls_fragment表示切片时长，mysteam表示一个实例，即将来要生成的文件名可以先自己随便设置一个。更多配置可以参考：[https://github.com/arut/nginx-rtmp-module/wiki/](https://github.com/arut/nginx-rtmp-module/wiki/)

根据以上步骤基本上已经实现了一个支持rtmp的视频服务器了。

## 10 在html5页面进行播放直播视频？

简单来说，直接使用video标签即可播放hls协议的直播视频：
<video autoplay webkit-playsinline>  
    <source src="http://10.66.69.77:8080/hls/mystream.m3u8" type="application/vnd.apple.mpegurl" />  
    <p class="warning">Your browser does not support HTML5 video.</p>  
</video>
需要注意的是，给video标签增加webkit-playsinline属性，这个属性是为了让video视频在ios的uiwebview里面可以不全屏播放，默认ios会全屏播放视频，需要给uiwebview设置allowsInlineMediaPlayback＝YES。业界比较成熟的[videojs](http://videojs.com/)，可以根据不同平台选择不同的策略，例如ios使用video标签，pc使用flash等。

## 11 坑点总结

简根据以上步骤，笔者写了一个demo，从实现ios视频录制，采集，上传，nginx服务器下发直播流，h5页面播放直播视频者一整套流程，总结出以下几点比较坑的地方：

1 在使用AVCaptureSession进行采集视频时，需要实现AVCaptureVideoDataOutputSampleBufferDelegate协议，同时在- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection捕获到视频流，要注意的是didOutputSampleBuffer这个方法不是didDropSampleBuffer方法，后者只会触发一次，当时开始写的是didDropSampleBuffer方法，差了半天才发现方法调用错了。

2 在使用rtmp推流时，rmtp地址要以rtmp://开头，ip地址要写实际ip地址，不要写成localhost，同时要加上端口号，因为手机端上传时是无法识别localhost的。

这里后续会补充上一些坑点，有的需要贴代码，这里先列这么多。

## 12 业界支持

目前，[腾讯云](https://www.qcloud.com/solution/video.html)，百度云，阿里云都已经有了基于视频直播的解决方案，从视频录制到视频播放，推流，都有一系列的sdk可以使用，缺点就是需要收费，如果可以的话，自己实现一套也并不是难事哈。



demo地址：[https://github.com/lvming6816077/LMVideoTest/](http://blob/master/LMVideoTest/ViewController.m)







更多直播相关知识：[http://lib.csdn.net/base/liveplay](http://lib.csdn.net/base/liveplay)









