# Android网络API(android.net.rtp)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月11日 07:40:27[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1430








为RTP（实时传输协议）提供API，允许应用程序管理按需或交互式数据流。 特别地，提供VOIP，即按即说，会议和音频流的应用程序可以使用这些API来发起会话，并通过任何可用的网络发送或接收数据流。




要支持音频会议和类似的使用，您需要将两个类作为流的端点实例化：


1、AudioStream指定一个远程端点，由网络映射和已配置的AudioCodec组成。

2、AudioGroup表示一个或多个AudioStream的本地端点。 AudioGroup可以混合所有的AudioStream，并且可以同时与设备扬声器和麦克风进行交互。




最简单的用法涉及单个远程端点和本地端点。 有关更复杂的用法，请参阅AudioGroup所描述的限制。





注意：要使用RTP API，您必须在清单文件中请求INTERNET和RECORD_AUDIO权限。








一、类

[AudioCodec](https://developer.android.google.cn/reference/android/net/rtp/AudioCodec.html)


该类定义了与AudioStreams一起使用的音频编解码器集合。





[AudioGroup](https://developer.android.google.cn/reference/android/net/rtp/AudioGroup.html)


AudioGroup是扬声器，麦克风和AudioStreams的音频中枢。





[AudioStream](https://developer.android.google.cn/reference/android/net/rtp/AudioStream.html)


音频流是通过实时传输协议（RTP）承载音频有效载荷的Rtp流。





[RtpStream](https://developer.android.google.cn/reference/android/net/rtp/RtpStream.html)


RtpStream表示通过实时传输协议（RTP）发送和接收带有媒体有效载荷的网络数据包的流的基类。




