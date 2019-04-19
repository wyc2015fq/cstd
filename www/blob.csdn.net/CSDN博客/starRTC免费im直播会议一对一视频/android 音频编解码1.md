# android 音频编解码1 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月16日 22:14:51[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：459
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)

**1. Android 官方的 MediaCodec API**
该 API 是在 Andorid 4.1 （API 16） 版本引入的
MediaCodec 使用的基本流程是：
|1234567891011|- createEncoderByType/createDecoderByType- configure- start- while(1) {- dequeueInputBuffer- queueInputBuffer- dequeueOutputBuffer- releaseOutputBuffer}- stop- release|
1，从 input 缓冲区队列申请 empty buffer [dequeueInputBuffer]
2，把需要编解码的数据拷贝到 empty buffer，然后放入 input 缓冲区队列 [queueInputBuffer] 
3，MediaCodec 模块从 input 缓冲区队列取一帧数据进行编解码处理
4，编解码处理结束后，MediaCodec 将原始数据 buffer 置为 empty 后放回 input 缓冲区队列，将编解码后的数据放入到 output 缓冲区队列
5，从 output 缓冲区队列申请编解码后的 buffer [dequeueOutputBuffer]
6，对编解码后的 buffer 进行渲染/播放
7，渲染/播放完成后，再将该 buffer 放回 output 缓冲区队列 [releaseOutputBuffer]
第3方库：
**（1） opus 编解码库**
opus 官网地址：[https://www.opus-codec.org](https://www.opus-codec.org/)
注：如今 Android 5.0 已经官方支持 opus 格式了，关于 Android 支持的多媒体格式列表可以查看 Android developer guide：[《Supported
 Media Formats》](http://developer.android.com/intl/zh-cn/guide/appendix/media-formats.html)
**（2） Speex 编解码库**
老牌的音频处理库，不仅是编解码，还提供了包括音频去噪、回声消除、静音检测等功能，官网地址：[http://www.speex.org](http://www.speex.org/)
**（3） ffmpeg**
**iLBC-Android**
iLBC 是著名的 WebRTC 项目的音频编解码模块，iLBC-Android 是从中抽取 iLBC 模块移植到 Android 平台的个人项目，地址：[https://github.com/lukeweber/iLBC-Android](https://github.com/lukeweber/iLBC-Android)
参考
[http://ticktick.blog.51cto.com/823160/1760191](http://ticktick.blog.51cto.com/823160/1760191)
