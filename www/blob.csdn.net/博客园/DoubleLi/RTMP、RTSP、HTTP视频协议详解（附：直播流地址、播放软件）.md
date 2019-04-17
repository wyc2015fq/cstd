# RTMP、RTSP、HTTP视频协议详解（附：直播流地址、播放软件） - DoubleLi - 博客园






一、RTMP、RTSP、HTTP协议

这三个协议都属于互联网 **TCP/IP **五层体系结构中应用层的协议。理论上这三种都可以用来做视频直播或点播。但通常来说，直播一般用 **RTMP**、**RTSP**。而点播用 **HTTP**。下面分别介绍下三者的特点。



1，RTMP协议

（1）是流媒体协议。

（2）RTMP协议是 **Adobe **的私有协议，未完全公开。

（3）RTMP协议一般传输的是 **flv**，**f4v **格式流。

（4）RTMP一般在 **TCP ****1**个通道上传输命令和数据。



2，RTSP协议

（1）是流媒体协议。

（2）RTSP协议是共有协议，并有专门机构做维护。.

（3）RTSP协议一般传输的是 **ts**、**mp4 **格式的流。

（4）RTSP传输一般需要 **2-3** 个通道，命令和数据通道分离。



3，HTTP协议

（1）不是是流媒体协议。

（2）HTTP协议是共有协议，并有专门机构做维护。 

（3）HTTP协议没有特定的传输流。 

（4）HTTP传输一般需要 **2-3 **个通道，命令和数据通道分离。





二、可用的直播流地址



通常我们进行 **RTMP**/**RTSP** 开发时，除了可以自己搭建视频服务器来进行测试外。也可以直接使用一些电视台的直播地址，省时省力。
下面是我收集汇总的一些视频直播地址，亲测可用。 


![原文:RTMP、RTSP、HTTP视频协议详解（附：直播流地址、播放软件）](http://www.hangge.com/blog_uploads/201608/201608111539172975.png)




1，RTMP协议直播源

香港卫视：rtmp://live.hkstv.hk.lxdns.com/live/hks



2，RTSP协议直播源

珠海过澳门大厅摄像头监控：rtsp://218.204.223.237:554/live/1/66251FC11353191F/e7ooqwcfbqjoo80j.sdp

大熊兔（点播）：rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov



3，HTTP协议直播源

香港卫视：http://live.hkstv.hk.lxdns.com/live/hks/playlist.m3u8

CCTV1高清：http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8

CCTV3高清：http://ivi.bupt.edu.cn/hls/cctv3hd.m3u8

CCTV5高清：http://ivi.bupt.edu.cn/hls/cctv5hd.m3u8

CCTV5+高清：http://ivi.bupt.edu.cn/hls/cctv5phd.m3u8

CCTV6高清：http://ivi.bupt.edu.cn/hls/cctv6hd.m3u8

苹果提供的测试源（点播）：http://devimages.apple.com.edgekey.net/streaming/examples/bipbop_4x3/gear2/prog_index.m3u8





**三、播放软件推荐：VLC**



要播放视频直播流，或者测试一个直播视频地址是否可以使用。这里推荐 **VLC **媒体播放器。功能强大且跨平台。支持 **Windows**、**Mac OS**、**Linux**、**Android**、**iOS**。

官网地址：[http://www.videolan.org/](http://www.videolan.org/)

![原文:RTMP、RTSP、HTTP视频协议详解（附：直播流地址、播放软件）](http://www.hangge.com/blog_uploads/201608/2016081114204239799.png)




打开播放器，选择菜单中“**媒体**”->“**打开网络串流...**”。在弹出页面中填入视频地址即可。


![原文:RTMP、RTSP、HTTP视频协议详解（附：直播流地址、播放软件）](http://www.hangge.com/blog_uploads/201608/2016081114444040415.png)



原文出自：[www.hangge.com](http://www.hangge.com/)  转载请保留原文链接：[http://www.hangge.com/blog/cache/detail_1325.html](http://www.hangge.com/blog/cache/detail_1325.html)









