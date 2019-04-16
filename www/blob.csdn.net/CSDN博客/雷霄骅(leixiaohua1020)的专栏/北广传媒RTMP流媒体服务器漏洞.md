# 北广传媒RTMP流媒体服务器漏洞 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月15日 11:44:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：118









北广传媒移动电视（[http://www.bj-mobiletv.com](http://www.bj-mobiletv.com)/）的RTMP流媒体服务器有漏洞

该漏洞可以上我们通过他们的服务器向互联网直播视频



使用任意可以发布RTMP流媒体的客户端（例如：Adobe Flash Media Encoder，或者自己做一个Flash文件）

随便给流媒体起个名字，可以直接Publish到他们的服务器的名字叫“live”的地址上。URL：rtmp://www.bj-mobiletv.com:8000/live

例如取名“myCamera”

然后使用Flash播放器（JWPlayer...），或者FFPLAY之类支持接收RTMP流媒体的播放器。

输入之前发布的完整URL，例如rtmp://www.bj-mobiletv.com:8000/live/myCamera

就可以收看自己发布的视频了






