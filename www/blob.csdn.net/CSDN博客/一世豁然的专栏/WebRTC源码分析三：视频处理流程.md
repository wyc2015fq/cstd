# WebRTC源码分析三：视频处理流程 - 一世豁然的专栏 - CSDN博客





2017年05月23日 22:24:41[一世豁然](https://me.csdn.net/Explorer_day)阅读数：744








本博客转载于：[http://blog.csdn.net/neustar1/article/details/19480863](http://blog.csdn.net/neustar1/article/details/19480863)









 文本介绍视频的处理流程。图1中显示了两路视频会话视频信号流过程。

![](https://img-blog.csdn.net/20140219113157156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbmV1c3RhcjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



图1 视频流程示意图

以一路视频会话为例，主要分为以下几个线程：

1）视频源产生线程：Camera生产视频画面，封装成视频帧，以一定帧率投递到下一个模块。；

2）采集线程：由Capturer负责采集视频帧，并对视频帧进行一定处理，如调整画面亮度。并送入各个发送链路的编码模块编码并发送出去；

3）接收线程：Rtp/Rtcp负责接收RTP/RTCP数据包，并进行包解析；

4）解码线程：Decoder解码编码后的视频帧；

5）投递线程：Render接收解码后的视频帧并缓存，投递给显示设备；

6）显示线程：Player负责绘制或输出视频画面，可以做多个窗口显示或者一个窗口多画面显示。
    视频解码时间相对编码时间要长，因此开辟了单独线程完成解码；另外多路视频画面接收后没有类似多路音频混音的操作，而是分别进行渲染显示。



