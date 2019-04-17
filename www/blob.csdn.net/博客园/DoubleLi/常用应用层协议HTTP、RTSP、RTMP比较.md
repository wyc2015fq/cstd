# 常用应用层协议HTTP、RTSP、RTMP比较 - DoubleLi - 博客园






HTTP（超文本传输协议）、RTSP（Real Time Streaming Protocol实时流传输协议）、RTMP（Routing Table Maintenance Protocol路由选择表维护协议）是应用层协议，理论上都可以做直播、点播，实际上直播多采用RTMP和RTSP、点播则多用RTSP和HTTP。

一、常用领域：

> 
HTTP（HTTPS）所有数据都作为文本处理，广泛应用于网络访问，是公有协议，有专门机构维护。

RTSP流媒体协议，多用在监控领域视频直播点播；是公有协议，有专门机构维护。

RTMP流媒体协议，多用在互联网直播领域（RTMP+CDN）、视频会议（取代原来SIP），是Adobe的私有协议,未完全公开。

RTSP传输一般需要2-3个通道，命令和数据通道分离，HTTP和RTMP一般在TCP一个通道上传输命令和数据。RTMP协议一般传输的是flv，f4v格式流，RTSP协议一般传输的是ts,mp4格式的流。HTTP没有特定的流。


二、协议实现

RTSP的开源实现


* live.com   C/S   C++   http://www.live555.com

* darwin     S     C++   http://www.opensource.apple.com/darwinsource/

* VLC        C     C     http://www.videolan.org

* mplayer    C     C     http://www.mplayerhq.hu

 * omsp       C/S   C     http://streaming.polito.it/

  * spook      S     C     http://www.litech.org/spook/




RTMP的实现，不源开的FMS，开源的RMTP服务器有：



C++实现的crtmpserver，支持Flash（RTMP,RTMPE,RTMPS,RTMPS,RTMPT,RTMPTE），Linux平台；

Java实现的Red5，支持Flash（RTMP协议），全平台Linux,Windows。

C#实现的FluorineFx，支持Flash（RTMP协议），Windows平台。











