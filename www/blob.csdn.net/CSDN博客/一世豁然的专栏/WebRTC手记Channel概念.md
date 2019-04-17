# WebRTC手记Channel概念 - 一世豁然的专栏 - CSDN博客





2017年02月17日 16:18:38[一世豁然](https://me.csdn.net/Explorer_day)阅读数：659标签：[webrtc](https://so.csdn.net/so/search/s.do?q=webrtc&t=blog)
个人分类：[WebRTC](https://blog.csdn.net/Explorer_day/article/category/6289311)









本博客转载于：[http://www.cnblogs.com/fangkm/p/4401075.html](http://www.cnblogs.com/fangkm/p/4401075.html)






前面两篇博文完整地介绍了WebRTC音视频的采集模块，接下来应该开始介绍关键的音视频编码模块。不过在介绍音视频编码模块之前，需要介绍一下Channel概念，WebRTC的每路数据的传输流程都封装成一个Channel对象。详细UML图如下：

![](http://images.cnitblog.com/blog2015/57211/201504/072318549938237.png)

MediaChannel及其派生类封装了待传输的编解码、RTP/RTCP封包解包等逻辑，具体对象由相应的Media Engine类创建，如图：视频Channel最终的实现类WebRtcVideoChannel2由WebRtcVideoEngine2创建，音频Channel最终的实现类WebRtcVoiceMediaChannel由WebRtcVoiceEngine创建。

Channel部分暴露给外界的操作接口还是ChannelManager类中管理的BaseChannel及其派生类，通过这些类，外部模块可以设置音视频的采集源（如VideoCapturer）、为网络发送过来的音视频数据指定渲染器（如AudioRenderer/VideoRenderer），这些类对MediaChannel及其派生类的基础上再包装了一层，如图所示，BaseChannel实现MediaChannel的NetworkInterface接口完成封装好的RTP/RTCP数据包包的发送操作，具体纯数据的网络发送请求最终委托给TransportChannel对象。TransportChannel对象的逻辑以后介绍网络层的时候再讲。

好了，下一篇开始介绍WebRtcVideoEngine2和由它创建的视频Channel类WebRtcVideoChannel2。




