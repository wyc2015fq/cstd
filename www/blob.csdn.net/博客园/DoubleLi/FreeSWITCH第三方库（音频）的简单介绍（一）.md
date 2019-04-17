# FreeSWITCH第三方库（音频）的简单介绍（一） - DoubleLi - 博客园







FreeSWITCH使用了大量的第三方库，本文档主要介绍音频相关库的信息：

视频相关库的信息介绍参考：[http://www.cnblogs.com/yoyotl/p/5488890.html](http://www.cnblogs.com/yoyotl/p/5488890.html)

其他相关库的信息介绍参考：[http://www.cnblogs.com/yoyotl/p/5489315.html](http://www.cnblogs.com/yoyotl/p/5489315.html)



```
① libbroadvoice
```

BroadVoice是一种高清语音编解码，支持窄带8kHz的BV16和宽带16kHz的BV32。

BroadVoice使用LGPL（Lesser General Public License）发布，完全免费，因而被广泛使用在许多开源的电话系统中。

BV16和BV32编码质量很高，低延迟（5ms），和ITU的编码相比，可节省1/2到1/3的CPU。

Bradvoice编码是自去年以来第三个加入FreeSWITCH的高清语音编码，其它两个免费、高质量的语音编码是CELT和G.722。 



```
② libflite
```

Flite是一个小型、快速的TTS系统，是著名的语音合成系统festival的c版本。可用于嵌入式系统。

官网：[http://www.speech.cs.cmu.edu/flite/](http://www.speech.cs.cmu.edu/flite/)



```
③ libg7221
```

G.722.1基于 Polycom 的第三代 Siren 7 压缩技术，1999年被ITU-T批准为G.722.1标准。G.722.1采用16 KHZ 采样频率，16 位数据量化，支持从 50 Hz 至 7 kHz频率范围的音频采样，并将其压缩为 32 与 24 Kbit/s。

它采用20 ms封帧，提供40ms的算法延迟。

G722.1可实现比 G.722 编解码器更低的比特率以及更大的压缩。目标是以大约一半的比特率实现与 G.722 大致相当的质量。这种编码使用许可需要获得Polycom公司的授权。



```
④ libilbc
```

iLBC是一种专为包交换网络通信设计的编解码，优于目前流行的G.729、G.723.1，对丢包进行了特有处理，即使在丢包率相当高的网络环境下，仍可获得非常清晰的语音效果。



```
⑤ libcodec2
```

codec2是开源的低比特率的音频编解码器。codec2压缩语音编码方法，支持3200bit/s及更低的带宽。codec2被设计用于无线电和其他高压缩语音应用的领域。

官网地址：[http://rowetel.com/codec2.html](http://rowetel.com/codec2.html)



```
⑥ libsilk
```

SILK是一个语音和音频编解码算法, 对于音频带宽、网络带宽和算法复杂度都具有很好的弹性。

支持四种采样率：8KHz、12KHz、16KHz、24KHz。三种复杂度：低、中、高。编码码率在 6~40kbps（不同采样率具有不同的码率范围）以及还支持VAD、DTX、FEC等模块，感觉还是比较全面。

最重要的一点是提供了定点C代码，非常有利于向ARM、DSP移植和优化。



```
⑦ opus
```

Opus编码器 是一个有损声音编码的格式，由互联网工程任务组（IETF）近来开发，适用于网络上的实时声音传输，标准格式为RFC 6716。opus是一个开源的使用上没有任何专利或限制软件。

Opus的前身是celt编码器。在当今的有损音频格式争夺上，拥有众多不同编码器的AAC格式打败了同样颇有潜力的Musepack、Vorbis等格式，而在Opus格式诞生后，情况似乎不同了。

通过诸多的对比测试，低码率下Opsu完胜曾经优势明显的HE AAC，中码率就已经可以媲敌码率高出30%左右的AAC格式，而高码率下更接近原始音频。



```
⑧ libsoundtouch
```

SoundTouch是一个音频处理开源库，允许改变声音的节奏，音高和播放速率等各种独立的参数。



```
⑨ openal
```

OpenAL（Open Audio Library）是自由软件界的跨平台音效API。它设计给多通道三维位置音效的特效表现。其 API 风格模仿自OpenGL。


上善若水，水利万物而不争。









