# android 音频采集1 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月16日 19:57:00[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：408
所属专栏：[编解码](https://blog.csdn.net/column/details/12721.html)

声道数一般表示声音录制时的音源数量或回放时相应的扬声器数量。
假设某通道的音频信号是采样率为8kHz，位宽为16bit，20ms一帧，双通道，则一帧音频数据的大小为：
int size = 8000 x 16bit x 0.02s x 2 = 5120 bit = 640 byte
Android提供了哪些音频开发相关的API？
音频采集： MediaRecoder，AudioRecord
音频播放： SoundPool，MediaPlayer，AudioTrack （它们之间的区别可以参考这篇文章）
音频编解码： MediaCodec
NDK API： OpenSL ES
音频开发的延时标准是什么？
可接受的时延是300ms
Android SDK 提供了两套音频采集的API，分别是：MediaRecorder 和 AudioRecord，前者是一个更加上层一点的API，它可以直接把手机麦克风录入的音频数据进行编码压缩（如AMR、MP3等）并存成文件，而后者则更接近底层，能够更加自由灵活地控制，可以得到原始的一帧帧PCM音频数据。
如果想简单地做一个录音机，录制成音频文件，则推荐使用 MediaRecorder，而如果需要对音频做进一步的算法处理、或者采用第三方的编码库进行压缩、以及网络传输等应用，则建议使用 AudioRecord，其实 MediaRecorder 底层也是调用了 AudioRecord 与 Android Framework 层的 AudioFlinger 进行交互的。
AudioRecord 的工作流程：
（1） 配置参数，初始化内部的音频缓冲区
（2） 开始采集
（3） 需要一个线程，不断地从 AudioRecord 的缓冲区将音频数据“读”出来，注意，这个过程一定要及时，否则就会出现“overrun”的错误，该错误在音频开发中比较常见，意味着应用层没有及时地“取走”音频数据，导致内部的音频缓冲区溢出。
（4） 停止采集，释放资源
构造函数参数解释：
输入源:
常用的值包括：DEFAULT（默认），VOICE_RECOGNITION（用于语音识别，等同于DEFAULT），MIC（由手机麦克风输入），VOICE_COMMUNICATION（用于VoIP应用）等等。
44100Hz是唯一可以保证兼容所有Android手机的采样率。
audioFormat:
常用的是 ENCODING_PCM_16BIT（16bit），ENCODING_PCM_8BIT（8bit），注意，前者是可以保证兼容所有Android手机的。
bufferSizeInBytes：配置的是 AudioRecord 内部的音频缓冲区的大小，该缓冲区的值不能低于一帧“音频帧”（Frame）的大小，有函数getMinBufferSize可以算出（内部应该是根据公式得到一帧的大小，然后设置成一帧大小的2～N倍）
权限：
<uses-permission android:name="android.permission.RECORD_AUDIO" />
TODO：AudioCapturer.java如何集成
[http://ticktick.blog.51cto.com/823160/1748506](http://ticktick.blog.51cto.com/823160/1748506)
[http://ticktick.blog.51cto.com/823160/1749719](http://ticktick.blog.51cto.com/823160/1749719) 有采集源码
