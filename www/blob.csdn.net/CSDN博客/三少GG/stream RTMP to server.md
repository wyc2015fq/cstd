# stream RTMP to server - 三少GG - CSDN博客
2013年06月16日 18:40:56[三少GG](https://me.csdn.net/scut1135)阅读数：4356
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)

1. 
# [Bambuser: Using FFmpeg as streaming application](http://cymaphore.net/journal/67-Bambuser-Using-FFmpeg-as-streaming-application)
[Bambuser](http://bambuser.com/) is a nice streaming service. It's extremely popular among political activists to provide
 video feeds from meetings and demonstrations alike. When traditional media are not interested, ordinary people step in using Bambuser and similar services. If you're not using an Android- or iOS-based mobile device, its flash based service allows to stream
 from within your webbrowser. However, it has some serious flaws about quality and capabilities. On the other hand there are also a few desktop applications job. Bambuser recommends Adobe Flash Media Live Encoder (Windows), Telestream Wirecast (Mac OS X) and
 Vidblaster (Windows). They are widely used, but in many aspects not very useful. They all have huge overhead in resource consumption. If you want to stream to Bambuser using GNU/Linux, you'll find only crap like Webcam Studio.
But you don't need all of that. Since Bambuser uses [RTMP](http://en.wikipedia.org/wiki/Real_Time_Messaging_Protocol) as
 upstream protocol, you can easily resort to simpler, more powerful tools, like [FFmpeg](http://ffmpeg.org/). However, you need to run it from command line, since no FFmpeg UI I
 know has acceptable support for the required features.
Just login to your Bambuser account, at the “Dashboard” select “Flash Media Live Encoder”. There is a link to your personal “authentication profile”. This xml file contains the required upstream URL.
 Download it and open it in an editor of your choice (the file is encoded in UTF-16, in case your editor doesn't detect that automatically). Within the file you find the rtmp-tag (flashmediaencoder_profile > output > rtmp). Take the content of the stream tag,
 append it to the content of the url tag (separated by a slash) – that's your personal upstream url. It should finally look something like:
> 
rtmp://509544.fme.bambuser.com/b-fme/8dd88a433140fea71510c307871386113116ca1c
Now you need [FFmpeg](http://ffmpeg.org/). There should be a package for your GNU/Linux distribution. Watch out to install
 H264 support as well (Debian/Ubuntu: Packages libx264-120 and x264). [FFmpeg for Windows](http://ffmpeg.zeranoe.com/builds/) has H264 support built-in.
Now you are ready to start streaming. The following commands demonstrate how to use FFmpeg to stream video from a Webcam or other (USB-)Camera, draw a png-logo on the it, shrink it to a appropriate
 size, stream it to Bambuser and simultaneously store the unmodified video on your hard drive. All in one command :-)
## Linux
> 
ffmpeg -f video4linux2 -i /dev/video0 -f alsa -i hw:1 -ab 64k -ar 44100 -b:v 200k -r 10 -s 320x240 -vf "movie=piraten-stuttgart.de.png [logo];[in][logo] overlay=W-w-10:H-h-10 [out]" -f flv rtmp://509544.fme.bambuser.com/b-fme/8dd88a433140fea71510c307871386113116ca1c
 -c copy "recording-$(date +%d.%m.%Y-%H.%M.%S).mkv"
Uses v4l2 interface using video-device /dev/video0 and alsa-audio-device hw:1. Date and time is appended to the recording file.
## Windows
> 
ffmpeg -f dshow -i video="USB-Videogerät":audio="HX-WA20" -ab 64k -ar 44100 -b:v 200k -r 10 -s 320x240 -vf "movie=piraten-stuttgart.de.png [logo];[in][logo] overlay=W-w-10:H-h-10 [out]" -f flv rtmp://509544.fme.bambuser.com/b-fme/8dd88a433140fea71510c307871386113116ca1c
 -c copy "recording-1.mkv"
Using Direct Show, selecting Camera “USB-Videogerät” and Audio-Input “HX-WA20”.
## Linux & Windows
To find out what Video- and Audio-Devices are present and where they are located you can use[VideoLAN](http://www.videolan.org/).
 Just open it, select menu “Media”, “Open Capture Device” and examine the devices listed at “Video device name” and “Audio device name”.
By the way, these instructions should basically work with other RTMP-based streaming services, like [Ustream](http://ustream.tv/) and [Piraten-Streaming](http://www.piraten-streaming.de/),
 as well.
2. 利用ffmpeg 和 raspivid
[http://www.slickstreamer.info/2013/06/use-raspberrypi-csi-camera-module-to.html](http://www.slickstreamer.info/2013/06/use-raspberrypi-csi-camera-module-to.html)
`raspivid -t 9999999 -w 960 -h 540 -fps 25 -b 500000 -o - | ffmpeg -i - -vcodec copy -an -metadata title="Streaming from raspberry pi camera" -f flv [url]/[id]`
3.  Android和java客户端
[破解某国外收费的RTMP Client并成功在Android和Java上调用](http://blog.csdn.net/visualcatsharp/article/details/6616209)
Red5流媒体服务器免费并且是开源的，与Flash搭配的时候可谓是天生一对，但使用Java和Android作为客户端调用却可谓一波三折。
         Adobe的Red5源代码里有一个RTMPClient的类，这个类在使用上其实不复杂，但却没办法成功调用。观察日志，发现是连接成功后在开始创建流的时候，服务端把连接断开了。我能想到的解释就是可能公司现在所使用的Red5服务器的版本与这个RTMPClient不兼容。
         国内有人把Red5的RTMPClient精简了出来作为一个开源的类库放在google的svn上，网址如下：[http://code.google.com/p/android-rtmp-client/](http://code.google.com/p/android-rtmp-client/)。这个类库同样也是没办法成功连接服务器。
         国外还有一个收费的RTMPClient，价值是395刀。具体网址和产品的名称我就不指出了，有心人肯定会找得到。这个客户端类库很强大，使用也很方便，我注册了一个试用的key，发现能和Red5服务器成功连接并且通讯良好。
由于实在是找不到其它的方法了，而且自己去摸索实现Red5的rtmp协议这基本上不太现实，于是我反编译了一下这个类库，发现除了几个入口类之外，其它类全是混淆过的。
[Android采集视频用H264编码以RTMP协议发送到FMS服务器](http://www.eoeandroid.com/thread-188523-1-1.html)
1、项目采用 rtmp_client 开源jar包。
2、rtmp_client 用google了一下，是收费的。但是一切收费的东东对大牛来说都是幻象，哈哈哈哈。网上有牛人破解了这个 rtmp_client ,详细请见：http://blog.csdn.net/visualcatsharp/article/details/6616209
3、破解之后就可以登入到 FMS 或 Red5 服务器了，（我自己用的FMS服务器，不出意外，Red5也应该同样ok）
4、这个IVideoChat项目，是一个网友共享给我的（在这里谢谢那位大侠，嘿嘿）。我稍微的做了一下修改
5、net.john.util 包的下 RTMPConnectionUtil.java 类：
       private static final String red5_url = "rtmp://192.168.1.97/live";
       192.168.1.97 这个是服务器地址    live 是服务器配置的 资源名
6、net.john.activity 包下的 VideoActivity.java
  登入之后会返回你一些信息：
NetConnection#onNetStatus: {level=status, code=NetConnection.Connect.Success, description=Connection succeeded., objectEncoding=3, data={version=4,5,0,297}}
    RTMPConnectionUtil.netStream.publish("aaa", NetStream.RECORD);这个方法是向FMS服务器发布视频数据，aaa 是发布人，
![未命名.jpg](http://www.eoeandroid.com/data/attachment/forum/201208/02/1027543nnfmffrmzp5tprp.jpg)
    采集视频：public void onPreviewFrame(byte[] arg0, Camera arg1){
                          ..........
/**将采集的YUV420SP数据转换为RGB格式*/
 byte[] current = RemoteUtil.decodeYUV420SP2RGB(arg0, width, height);
            try {
//              int byte_result = Decode(arg0);/**将采集到的每一帧视频数据用H264编码*/
//           byte[] bytes1 = copyOf(out,byte_result);
//           Log.i(TAG, "byte数组的长度："+bytes1.length);
              /**打包该编码后的H264数据*/
                final byte[] packet = RemoteUtil.encode(current, previous, blockWidth, blockHeight, width, height);
           fireOnVideoData(new MediaDataByteArray(timeBetweenFrames, new ByteArray(packet)));
                previous = current;
                if (++frameCounter % 10 == 0) previous = null;
            }
            catch (Exception e) {
                e.printStackTrace();
            }
                          ...........
}
7、现在有个问题没有解决，望大牛们帮助解决:
    我将采集到得每帧数据用H264编码后，然后打包，FMS服务器一直无法收到我采集编码后的数据，我想应该是 我的 打包 那个方法有问题，编码成RGB是可以将采集到得数据发送到FMS服务器的。
**注： 根据文章后评论貌似此代码有严重问题，不推荐使用。**
4.   【】
# [Creating an RTSP Stream with GStreamer](http://weeklybuild.com/2013/01/creating-an-rtsp-stream-with-gstreamer/)
Before we start coding, we’ll have to install a couple packages. You’ll need to install libgstreamer & gstrtspserver. I’m using Ubuntu so I installed them with the following command at the terminal prompt.
```
sudo apt-get install libgstreamer0.10-dev libgstrtspserver-0.10-dev
```
Next I created a makefile to tell GCC which libraries I will be linking to.
Create a Makefile and paste in the contents below.
5. 【不推荐】live555方案
[在IPCAM上实现RTSP协议直播-live555](http://blog.csdn.net/taixinlfx/article/details/7557103)
1，下载live555，网址：[http://www.live555.com/liveMedia/](http://www.live555.com/liveMedia/)
2，编译之，在该网站上有说明，基本命令：
./genMakefile linux
make
3，打开live/testProgs/testOnDemandRTSPServer.cpp，修改reuseFirstSource的值为True，并编译之
4，mkfifo live/testProgs/test.264
5，写自己的程序，将H264编码的数据write到test.264管道中
6，使用vlc打开串流
成功！
感谢ghostyu的文章
感谢live555
C++不懂，代码一行也没看懂，居然就出来了。
之前一直存在一个问题：大码流时，播放会卡。原因应该是#define BANK_SIZE 150000 这个宏的值，太小了。改大它试一下，应该可以解决掉。
使用 live555 直播来自 v4l2 的摄像头图像
6. 【不推荐】VLC
# [Make public webcam. Which protocol, which codec. (Using VLC)](http://askubuntu.com/questions/27562/make-public-webcam-which-protocol-which-codec-using-vlc)
I thought of using VLC and already tried using http output but it was not really good. Too cpu hungry, too big stream (kBps), not stable...
I been reading VLC how-to's but thre is still a question.
I certainly wouldn't use VLC. It complicates things for users and is far too high-bandwidth for public consumption.
Hvae you looked at the package `webcam-server`.
 You tell it what webcam to show and it provides a networked video source. It also comes with a java applet that you can host on a web page that connects directly to the `webcam-server` daemon.
Tanks for suggestion,
 but all those programs just opes up stram, and send MJPG video stream that get from WebCAM. MJPG isn't really good codec, I want to make it smaller. – [gsedej](http://askubuntu.com/users/1982/gsedej)[Mar
 24 '11 at 21:12](http://askubuntu.com/questions/27562/make-public-webcam-which-protocol-which-codec-using-vlc#comment35715_28032)
================参考======================
[android视频录制、另一部手机实时观看方案](http://www.eoeandroid.com/forum.php?mod=viewthread&tid=277414)
最近调研android视频录制、另一部手机实时观看，大致有以下几种思路。
1. android手机充当服务器，使用NanoHTTPD充当服务器，另一部手机或者pc通过输入http://手机的ip:8080网址观看。
这种方案可以参考 ipcamera-for-android开源项目，网址  http://code.google.com/p/ipcamera-for-android/source/checkout
可以运行的源代码可以在这下载            http://download.csdn.net/detail/xiaoliouc/4933558
缺点：android手机必须支持MP4+ARM_BN格式，有些手机不兼容，延时有点厉害
2. android手机采用juv-rtmp-client.jar包，网上有收费的包，但可以搜索破解包，通过rtmp协议发布服务到red5服务器。服务器当然是red5了，可以用red5自带的的OFLA Demo做测试。客户端代码可以参考ivideochat，下载地址     http://download.csdn.net/detail/xiaoliouc/4933594
电脑可以直接通过安装了flash player浏览器观看，手机呢，可以找一个支持rtmp协议的播放器了。
缺点：需要flash支持
3.android手机通过camera类拍摄视频。把一帧一帧的图像压缩通过socket发送到服务器，服务器可以直接观看。而要想让另一台手机也能观看，可以让服务器转发来实现。
例子网上有很多，        http://download.csdn.net/detail/xiaoliouc/4933610
缺点：通过一帧一帧的发送数据，传输过程耗费大量流量。玩玩可以，但实际项目中不可取。
4.android手机通过camera类拍摄视频，把拍摄的视频通过h264编码，可以采用软编码（使用x264库或者opencore软件库），java类通过jni调用编译后的so文件来实现。然后通过基于udp的rtp协议传输到服务器。为什么不使用tcp协议呢，因为tcp的重传机制会产生延时和抖动，而单独使用udp传输协议本身是面向无连接的，不能提供质量保证，需要在udp协议只上采用rtp或者rtcp提供流量控制和拥塞控制服务。服务器通过ffmpeg对接收的h264解码并播放。播放可以使用VLC
 media player。如果对c++比较熟悉，可以看看live555这个开源项目。
缺点：需要懂得的知识很多，jni啊，h264编码解码 ，rtp协议等。使用软编码，效率比较低，耗cpu耗电啊。
5.android手机通过mediaRecorder类拍摄视频，其中当然包括音频了。把拍摄的视频通过h264编码，可以采用硬编码（面向手机的硬件直接操作），只能针对3gp，mp4视频格式。方法参考  http://blog.csdn.net/zblue78/article/details/6078040
这篇博客，里面讲的很详细，提取h264的sps，pps，可以参考      http://blog.csdn.net/peijiangping1989/article/details/6934317     
winHex是一款好用的16进制查看工具，下载地址           http://download.csdn.net/detail/xiaoliouc/4928773
代码网上有很多，个人理解是：mediaRecorder录制视频（3gp，MP4），可以通过mMediaRecorder.setPreviewDisplay(mSurfaceHolder.getSurface());预览视频，通过localsocket发送到本地的localserversocket的h264实时视频流。
这个过程涉及到硬编码，硬编码个人理解是，在预览过程或者提前确定视频的sps，pps，head（一般为0x00000001），不同的手机硬件不一样。把得到的这些参数写入h264，得到正确的h264视频流文件，然后把流通过rtp协议（或者其他）发送到服务器。服务器端对得到的h264用ffmepg解码，播放。
貌似项目客户需要在苹果电脑，手机上播放。这个时候HLS协议就出来了，头疼的东西，服务器采用nginx，ffmpeg解码。nginx服务器搭建过程，ffmpeg安装过程  见我前几篇文章。   
然后用ffmpeg对解码后的mp4文件进行ts切片，生成带有索引的m3u8文件，然后客户端就可以通过浏览器http://ip :port/ *.m3u8访问。
过程貌似是这样的，但自己由于刚接触不到一个周，还不太理解。
6.前面讲的都是单向视频，如果是双向视频，其实就是视频会议了，可以参考sipdroid开源源代码了，网址           http://code.google.com/p/sipdroid/source/checkout
另外大家不感觉关于视频这个方向上面的源码有点少吗？基本都不能用。
================================
[Linuxaria](http://www.linuxaria.com/) 上提供了两份教程，关于如何用Linux PC 上的摄像头做简单的远程监控系统，可以用于家庭环境。*注：原教程都是基于Ubuntu 10.04的。*
第一份方案是基于 [Motion](http://www.lavrsen.dk/twiki/bin/view/Motion/WebHome) 软件的。能够在特定环境下触发，如声音、特定操作等，并能用邮件、SMS远程操纵，同时支持远程调节亮度、对比度等。
  [原文](http://www.linuxaria.com/article/realizzare-un-webcam-server?lang=en)
第二份方案是基于 [webcam-server](http://webcamserver.sourceforge.net/) 的。使用更为简单，并有GUI。
第三份方案是基于 [ZoneMinder](http://www.zoneminder.com/) 的。这是一个专业的商业方案，能够使用多摄像头和闭路电视，并支持数据的采集、分析、记录，以及远程和半自动处理。[官方教程](http://www.zoneminder.com/wiki/index.php/Documentation#Tutorial)[原文](http://www.linuxaria.com/article/webcam-server-su-linux-2?lang=en)
