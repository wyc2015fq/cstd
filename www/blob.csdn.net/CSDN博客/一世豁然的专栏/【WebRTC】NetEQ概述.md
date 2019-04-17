# 【WebRTC】NetEQ概述 - 一世豁然的专栏 - CSDN博客





2017年05月23日 22:18:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：973








本博客转载于：[http://blog.csdn.net/boywgw/article/details/48311987#comments](http://blog.csdn.net/boywgw/article/details/48311987#comments)









WebRTC由语音引擎，视频引擎和网络传输三大模块组成，其中语音引擎是WebRTC中最具价值的技术之一。




WebRTC语音引擎由一系列音频和网络处理模块组成，包括了从音频采集到网络传输等处理流程的完整解决方案。

![](https://img-blog.csdn.net/20150909140339416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



语音引擎工作流程图

语音引擎的一般工作流程如下：

1.发起端进行声音采集

2.采集到的声音信号进行回声消除，噪音抑制，自动增益控制处理

3.语音压缩编码

4.通过Internet网路传输到接收端

5.到达接收端，先进入NetEQ模块进行抖动消除，丢包隐藏解码等操作

6.将处理过后的音频数据送入声卡设备进行播放




NetEQ模块是Webrtc语音引擎中的核心模块

![](https://img-blog.csdn.net/20150909140111831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



NetEQ模块框图

从上图看，NetEQ模块基本上分为：自适应缓冲器(Adaptive Packet Buffer)，语音解码器(Speech Decoder)，抖动控制和丢包隐藏(Jitter Control and Error Concealment) 以及播放(Play Out)四大部分。其中抖动控制和丢包隐藏模块是NetEQ的核心[算法](http://lib.csdn.net/base/datastructure)，既控制着自适应缓冲器，又与解码器进行紧密的交互，并且将最终的计算结果交给声卡去播放。




WebRTC的语音引擎在运行时会启动两个线程：一个线程用于接收来自于网络的数据包，并将其插入到抖动缓冲区中；另外一个线程每隔10ms从NetEQ中提取10ms语音数据进行播放。

网络数据包进入抖动缓冲区的过程在：





**[cpp]**[view
 plain](http://blog.csdn.net/boywgw/article/details/48311987#)[copy](http://blog.csdn.net/boywgw/article/details/48311987#)



- int32_t ACMNetEQ::RecIn(const uint8_t* incoming_payload, const int32_t length_payload, const WebRtcRTPHeader& rtp_info, uint32_t receive_timestamp)  


提取10ms语音数据到声卡的过程在：






**[cpp]**[view
 plain](http://blog.csdn.net/boywgw/article/details/48311987#)[copy](http://blog.csdn.net/boywgw/article/details/48311987#)



- int32_t ACMNetEQ::RecOut(AudioFrame& audio_frame)  

![](http://static.blog.csdn.net/images/save_snippets_01.png)








