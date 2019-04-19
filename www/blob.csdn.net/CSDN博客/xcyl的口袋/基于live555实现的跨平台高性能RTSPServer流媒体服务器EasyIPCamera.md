# 基于live555实现的跨平台高性能RTSPServer流媒体服务器EasyIPCamera - xcyl的口袋 - CSDN博客
2016年08月30日 11:26:53[xcyl](https://me.csdn.net/cai6811376)阅读数：1111
本文由EasyDarwin团队Kim贡献 
[http://blog.csdn.net/jinlong0603/article/details/52366412](http://blog.csdn.net/jinlong0603/article/details/52366412)
## 简介
EasyIPCamera是由EasyDarwin团队开发的一套非常稳定、易用、支持多种平台（包括Windows/Linux 32&64，ARM，Android）的RTSPServer服务器，接口调用非常简单成熟，无需关注RTSPServer中关于客户端监听接入、音视频多路复用、RTSP具体流程、RTP打包与发送等相关问题。 
Android版本EasyIPCamera的出现，可以方便开发者在各种Android开发板、手机等设备中轻松搭建RTSPServer服务器，使用任意一款RTSP播放器，就可以实现实时监控、直播的功能。
## 功能介绍
Android版本的代码使用Android自带的Camera和音频作为数据源。调用EasyIPCamera的SDK，当有客户端视频请求的时候，上层收到回调后打开Camera、Audio开始推流，收到结束回调时关闭Camera、Audio设备。 
也可以使用EasyRTSPClient或其他方式将远端的数据源拉来作为自己RTSPServer的数据源，其他版本(EasyIPCamera_RTSP)的EasyIPCamera中有这样的Demo，可供参考。
## 演示效果
我在Android手机红米Note2中运行了EasyIPCamera APP, 在windows上用VLC播放器请求了手机上的RTSP流。 
![这里写图片描述](https://img-blog.csdn.net/20160830100816447)
## 版本及源码下载
EasyIPCamera SDK： 
[https://github.com/EasyDarwin/EasyIPCamera](https://github.com/EasyDarwin/EasyIPCamera)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
