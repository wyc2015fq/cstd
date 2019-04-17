# FFmpeg实现监控摄像头的RTSP协议转RTMP协议直播 - DoubleLi - 博客园






文章来源：http://www.cuplayer.com/player/PlayerCode/RTSP/2014/0706/1419.html





FFmpeg实现监控摄像头的RTSP协议转RTMP协议直播,我们使用的摄像头是海康的可以通过rtsp协议获取到摄像头的直播数据。所以可以通过


### 整理了一下要解决如下问题：

1、  摄像头的数据采集。

2、  如何将采集到的数据交给Red5处理。

3、  [[FFmpeg]图文介绍windows下实现编译ffmpeg工程的详细步骤](http://www.cuplayer.com/player/PlayerCode/FFmpeg/2014/0706/1401.html)

  解决方法：

1、  我们使用的摄像头是海康的可以通过rtsp协议获取到摄像头的直播数据。所以可以通过 ：

2、  rtsp://admin:12345@192.168.1.77:554/h264/ch1/main/av_stream

3、  实现视频数据的采集。

4、  获取到数据流后如何利用Red5发布出去呢？

5、  因为获取到的数据是rtsp协议的，所以要转协议成rtmp，ffmpeg可以实现这个功能。



### 具体步骤：

1、  首先要安装流媒体服务器Red5用于rtmp数据的接收与发布。

l         下载Red5

l         我下载的版本是setup-Red5-1.0.1-java6.exe

l         安装Red5  [http://www.cuplayer.com/player/PlayerCode/Red5/2013/0319/760.html](http://www.cuplayer.com/player/PlayerCode/Red5/2013/0319/760.html)

l         在安装的过程会弹窗要求设置一些服务器信息。

l         设置服务器的IP

![](http://www.cuplayer.com/player/uploads/allimg/140706/1ZF12508-0.png)

设置HTTP访问端口

![](http://www.cuplayer.com/player/uploads/allimg/140706/1ZF11118-1.jpg)

 后面一直“next”即可。

 安装完成后访问：[http://localhost:5080/](http://localhost:5080/) 如果可以看到

![](http://www.cuplayer.com/player/uploads/allimg/140706/1ZF11S7-2.png)

 Red5默认是会安装成系统的服务的，如果重复启动会报端口占用的错误。

在http://localhost:5080/installer/ 页面中安装Red5自带的Demo OflaDemo。安装好后访问http://localhost:5080/oflaDemo/index.html 即可已看到通过rtmp 点播的视频文件视频。这样你的Red5服务器就安装完成了。

1、  下载安装FFmpeg,** 安装 ffmpeg教程：**[http://www.cuplayer.com/player/PlayerCode/FFmpeg/2014/0706/1401.html](http://www.cuplayer.com/player/PlayerCode/FFmpeg/2014/0706/1401.html)

这里下载的是编译好的[ffmpeg-20130703-git-e0be3cb-win32-shared](http://www.cuplayer.com/player/PlayerCode/FFmpeg/2014/0706/1401.html), 直接解压好，并将bin目录加入到path环境变量下即可。      

2、  [测试](http://lib.csdn.net/base/softwaretest)

l         运行Red5

l         在命令行下执行：


- ffmpeg -i "rtsp://admin:12345@192.168.1.77:554/h264/ch1/main/av_stream" -f flv -r 25-s 640x480-an "rtmp://localhost/oflaDemo/hello"

l         修改Red5安装目录下的 webapps\oflaDemo\index.html 文件，将jwplayer的 file 属性改成 “hello”。

l         访问 http://localhost:5080/oflaDemo/index.html 在你所修改的播放器上是否可以看到摄像头的直播了。










