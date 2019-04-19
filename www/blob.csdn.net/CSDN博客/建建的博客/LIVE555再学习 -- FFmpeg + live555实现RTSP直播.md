# LIVE555再学习 -- FFmpeg + live555实现RTSP直播 - 建建的博客 - CSDN博客
2018年07月21日 22:41:59[纪建](https://me.csdn.net/u013898698)阅读数：154
个人分类：[Live555](https://blog.csdn.net/u013898698/article/category/6686402)
# 一、简单实验
首先先简单的实验了一下。
## 运行 FFmpeg 将 MP4文件转成 264 文件 
ffmpeg -i Tai.mp4 test.264
## 执行 testOnDemandRTSPServer  单播 264 文件
./testOnDemandRTSPServer 
## 在 VLC 添加 URL 地址
rtsp://192.168.x.xx:8554/h264ESVideoTest
OK，可以播放。但这样测肯定有问题嘛，显然FFmpeg转264的速度跟不上播放速度。
也有可以利用管道的解决这个问题的，**参看：[使用live555 直播来自v4l2的摄像头数据](http://blog.csdn.net/li_wen01/article/details/59523963)**
# 二、FFmpeg 解码
再有网上有这样的例子，**参看：[live555接收实时视频流+ffmpeg解](http://blog.csdn.net/dzhuang123/article/details/45045807)[基于live555的rtsp客户端接收及ffmpeg解码](http://blog.chinaunix.net/uid-15063109-id-4482932.html)**
效果和我之前讲的利用MP4V2编码很类似吧，**参看：[LIVE555再学习 -- testRTSPClient 实例](http://blog.csdn.net/qq_29350001/article/details/78051767)**
都是可以将RTSP接收到的264文件转换成MP4文件，FFmpeg的可能解码为YUV，这样子。
**参看：[ffmpeg API录制rtsp视频流](http://blog.csdn.net/sloan6/article/details/8190638#reply)**
