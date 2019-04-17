# 通过rtmpdump推送海康视频流到red5服务器 - DoubleLi - 博客园






现在主流的网络摄像机都支持标准H264视频格式，例如 海康网络摄像机， 通过海康提供的网络SDK可以获取到视频码流。我测试的这款相机，视频编码采用的是H264，音频编码采用的是G711a。 这里，我仅测试了视频。 通过海康SDK获取到视频流为PS流，根据PS协议格式，抽取其中的H264帧数据，SPS PPS 信息。  

     推送RTMP流采用开源库 rtmpdump， RTMP服务器采用开源RED5。  连接上red5 后 首先 要发送 meta信息，否则通过flash 端无法打开视频流。之后再依次发送视频帧数据即可。效果如下:

![](http://img.blog.csdn.net/20141010135719560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXRvdXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

     与之前拿264视频文件测试不同， 此次测试采用了全新的代码架构，可接入不同的设备，例如 可以扩展大华IPC，USB摄像头等。 协议层也做了分类，方便以后扩展 RTSP流推送，私有格式推送等。

     发送视频文件到red5服务器 参考代码在这里下载:          [http://download.csdn.net/detail/mtour/7652907](http://download.csdn.net/detail/mtour/7652907)









