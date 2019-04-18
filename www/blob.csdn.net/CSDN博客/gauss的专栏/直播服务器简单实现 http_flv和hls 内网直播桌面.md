# 直播服务器简单实现 http_flv和hls 内网直播桌面 - gauss的专栏 - CSDN博客
2017年01月12日 10:09:05[gauss](https://me.csdn.net/mathlmx)阅读数：11258
个人分类：[流媒体](https://blog.csdn.net/mathlmx/article/category/6676923)
直播都不陌生了，如今主流的协议分析的对比图，个人见解。
|协议|httpflv|rtmp|hls|dash|
|----|----|----|----|----|
|传输层|http流|tcp流|http|http|
|视频格式|flv|flv tag|Ts文件|Mp4 3gp webm|
|延时|低|低|高|高|
|数据分段|连续流|连续流|切片文件|切片文件|
|Html5播放|可通过html5解封包播放(flv.js)|不支持|可通过html5解封包播放(hls.js)|如果dash文件列表是mp4webm文件，可直接播放|
http_flv&rtmp
这两个协议实际上传输数据是一样的，数据都是flv文件的tag。http_flv是一个无限大的http流的文件，相比rtmp就只能直播，而rtmp还可以推流和更多的操作。但是http有个好处，就是是以80http通信的，穿透性强，而且rtmp是非开放协议。
这两个协议是如今直播平台主选的直播方式，主要原因就是延时极低。
hls
hls是Apple推出的直播协议，是通过视频流切片成文件片段来直播的。客户端首先会请求一个m3u8文件，里面会有不同码率的流，或者直接是ts文件列表，通过给出的ts文件地址去依次播放。在直播的时候，客户端会不断请求m3u8文件，检查ts列表是否有新的ts切片。
这种方式直播的主要弊端就是延迟过大，最小延时为ts单个文件的时长。
dash
dash实际工作原理和hls一样的，只不过不是mpegts文件，dash可以支持多种切片文件，比如mp4切片。当为mp4切片，客户端直接可用js控制利用html5直接播放。同样的，dash有延时。
http-flv到底是怎么直播？
这里我们主要研究httpflv和hls。看了主流几个web直播平台，发现几乎都是以httpflv为主来直播的，那么这么火httpflv到底是怎么达到直播的？
首先我们都知道在媒体格式里，几乎都以h264视频编码。如今httpflv直播的flv数据也都是h264&aac为主的。flv封装单元是以tag来表示的，一个tag可以是音频tag或者视频tag，或者脚本tag及其其他类型。
值得注意的是flv里面字节序是网络字节序，
flv的格式：
```
```
flvheader+[脚本tag(metadata)]+[第一个视频tag(h264_spspps)]+[第一个音频tag(aac_header)]+[第二个视频tag(h264第一个关键帧)]+
 后面就是音频和视频tag交互存在
```
```
tag的格式：
```
```
TYPE[1byte]
 + body size[3byte] + timestamp [4byte] +streamID [3byte] +[body data]+[previousTagSize 4byte]
```
```
这里的timestamp是这样存在的[ time tamp 3b,time tamp ex 1b]
h264视频tagbody:
这里存储的h264是没有nal分割符的，在t的body里面是这样存储的,
```
```
[isKeyFrame(1byte)]+0x01+[compositionTime
 3byte]+[h264 size 4byte]
```
```
compositionTime是h264编码结果dts和pts之间的偏移。
aac视频tag的body：
```
```
0xaf+0x01+aac
 raw
```
```
以上就是flv格式的详细说明，可以看出格式简单，封装的前后数据关联很小，当我们得到音频头和视频头，就可以从后面任意的一个关键帧开始播放。
当然想要httpflv正常播放，不能缺少matedata，就是第一个脚本tag里面，这里面指定了分辨率，音视频编码格式等。
 httpflv直播实际上单纯就是往客户端发送flvtag，当然是先发送flv前面那几个关键的tag，然后第一帧是关键帧。
假如客户端是obs推流软件，以rtmp方式向服务器推流，在开始握手及其创建stream完成以及发送Metadata完成，及其一系列数据发送完毕，服务器就向obs发送publish result指令，此后，obs就开始向服务器推送flv
 tag数据，开始直播，而服务器也得到了flv数据。
当某个客户端想要来获取直播数据，比如httpflv的方式来观看直播，服务器会怎么做呢？
服务器会先发送前面几个flvtag，header+metadata+spspps+aacheader，当这几个tag发送完毕，服务器会从直播流tag中，找到最新的视频关键帧tag，从这个关键帧tag开发数据，为什么呢？因为视频流是IBP帧相互存在的，I是完整数据，BP解码都需要I帧和前面的帧，所以，正常视频数据，必须是从I帧开始发送的。这里就涉及到gop间距了，rtmp低延时秒开就是这个原理。当然发送的时候，每个连接的tag开始时间戳戳要从0开始递增的。
至此，httpflv客户端就可以接受到flv流数据，解码观看直播了。
hls到底是怎么直播？
hls就相对简单粗暴了，服务器将直播流数据的h264和aac，封装切片成一个个的ts文件。客户端获取直播的数据的时候，先请求m3u8文件，下面就是一个m3u8的文件，
`#EXTM3U`
`#EXT-X-VERSION:3`
```
#EXT-X-TARGETDURATION:5
 ts最大时常5s
```
```
#EXT-X-MEDIA-SEQUENCE:2
 第一个ts文件的标识
```
`#EXTINF:4.993,//第一个ts文件，时长4.993，url地址/hls/2.ts`
`/hls/2.ts`
`#EXTINF:4.034,`
`/hls/3.ts`
`#EXTINF:4.980,`
`/hls/4.ts`
如果是直播，客户端会不停的去请求这个m3u8文件，当这个列表有新的ts文件，客户端会请求新的ts文件追加到本地播放序列。
关于ts的封包，ts的封装格式要比flv更复杂，主要的数据单元是ts包，每个包有pid，一个包固定大小普通没有crc的为188，主要分为三类ts包，pat，pmt，pes，pat就是第一个包，当解析的时候会在ts包列表里找pid为0x0的包，就是pat包，pat大概作用就是入口的意思，pat里面有pmt包的pid，pmt里面存储的是流的包的pid，比如指定音频包pid是0x102，视频包pid是0x101，后面的0x102和0x101的包就是pes包了，将pes包解析并合并出原始流，就能解码播放了。
 小程序
知道了如何玩直播，于是写了一个小的程序，该程序会录制本机桌面和输出音频以及麦克风，编码为h264和aac，同时在本机利用IOCP简易的创建一个服务器，提供web服务和直播服务，支持httpflv和hls直播。
以下是小程序的架构图：
![](http://images2015.cnblogs.com/blog/524900/201611/524900-20161119010854404-2135512941.jpg)
运行截图：
![](http://images2015.cnblogs.com/blog/524900/201611/524900-20161119011500467-349608286.jpg)
启动后可选择，填写端口号和码率，然后选择直播方式，黑屏换low api。
![](http://images2015.cnblogs.com/blog/524900/201611/524900-20161119011508248-366933723.jpg)![](http://images2015.cnblogs.com/blog/524900/201611/524900-20161119011516404-113547836.jpg)
![](http://images2015.cnblogs.com/blog/524900/201611/524900-20161119011531779-1230335281.jpg)
程序使用的三个库：
> 
libx264 视频编码
libfaac 音频编码
swscale brga转yuv420
