# FFmpeg获取DirectShow设备数据（摄像头，录屏） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年08月02日 00:57:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：232
个人分类：[FFMPEG](https://blog.csdn.net/leixiaohua1020/article/category/1360795)










这两天研究了FFmpeg获取DirectShow设备数据的方法，在此简单记录一下以作备忘。本文所述的方法主要是对应Windows平台的。

## **1. 列设备**



`ffmpeg -list_devices true -f dshow -i dummy`



命令执行后输出的结果如下（注：中文的设备会出现乱码的情况）。列表显示设备的名称很重要，输入的时候都是使用“-f dshow -i video="{设备名}"”的方式。

![](https://img-blog.csdn.net/20140730013108120)

我自己的机器上列出了以下设备：




```
[dshow @0388f5e0] DirectShow video devices
[dshow @0388f5e0]  "Integrated Camera"
[dshow @0388f5e0] "screen-capture-recorder"
[dshow @0388f5e0] DirectShow audio devices
[dshow @0388f5e0]  "鍐呰楹﹀厠椋?(Conexant20672 SmartAudi"
[dshow @0388f5e0]  "virtual-audio-capturer"
```




下文的测试中，使用其中的两个视频输入："Integrated Camera"和"screen-capture-recorder"。

注：音频设备出现乱码，这个问题还没有解决。

## **2. 获取摄像头数据（保存为本地文件或者发送实时流）**

#### 2.1. 编码为H.264，保存为本地文件

下面这条命令，实现了从摄像头读取数据并编码为H.264，最后保存成mycamera.mkv。



`ffmpeg -f dshow -i video="Integrated Camera" -vcodec libx264 mycamera.mkv`

#### 2.2. 直接播放摄像头的数据

使用ffplay可以直接播放摄像头的数据，命令如下：



`ffplay -f dshow -i video="Integrated Camera"`



如果设备名称正确的话，会直接打开本机的摄像头，如图所示。

![](https://img-blog.csdn.net/20140730012916906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注：除了使用DirectShow作为输入外，使用VFW也可以读取到摄像头的数据，例如下述命令可以播放摄像头数据：


`ffplay -f vfwcap -i 0`
#### 2.3. 编码为H.264，发布UDP

下面这条命令，实现了：获取摄像头数据->编码为H.264->封装为UDP并发送至组播地址。



`ffmpeg -f dshow -i video="Integrated Camera" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f h264 udp://233.233.233.223:6666`



注1：考虑到提高libx264的编码速度，添加了-preset:v ultrafast和-tune:v zerolatency两个选项。

注2：高分辨率的情况下，使用UDP可能出现丢包的情况。为了避免这种情况，可以添加–s 参数（例如-s 320x240）调小分辨率。

#### 2.4. 编码为H.264，发布RTP

下面这条命令，实现了：获取摄像头数据->编码为H.264->封装为RTP并发送至组播地址。



`ffmpeg -f dshow -i video="Integrated Camera" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f rtp rtp://233.233.233.223:6666>test.sdp`



注1：考虑到提高libx264的编码速度，添加了-preset:v ultrafast和-tune:v zerolatency两个选项。

注2：结尾添加“>test.sdp”可以在发布的同时生成sdp文件。该文件可以用于该视频流的播放。

#### 2.5. 编码为H.264，发布RTMP

下面这条命令，实现了：获取摄像头数据->编码为H.264->并发送至RTMP服务器。



`ffmpeg -f dshow -i video="Integrated Camera" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f flv rtmp://localhost/oflaDemo/livestream`




#### 2.6. 编码为MPEG2，发布UDP

与编码为H.264类似，指明-vcodec即可。



`ffmpeg -f dshow -i video="Integrated Camera" -vcodec mpeg2video -f mpeg2video udp://233.233.233.223:6666`




播放MPEG2的UDP流如下。指明-vcodec为mpeg2video即可



`ffplay -vcodec mpeg2video udp://233.233.233.223:6666`





## **3. 屏幕录制（Windows平台下保存为本地文件或者发送实时流）**

Linux下使用FFmpeg进行屏幕录制相对比较方便，可以使用x11grab，使用如下的命令：



`ffmpeg -f x11grab -s 1600x900 -r 50 -vcodec libx264 –preset:v ultrafast –tune:v zerolatency -crf 18 -f mpegts udp://localhost:1234`



详细时使用方式可以参考这篇文章：[DesktopStreaming With FFmpeg for Lower Latency](http://www.waitwut.info/blog/2013/06/09/desktop-streaming-with-ffmpeg-for-lower-latency/)

Linux录屏在这里不再赘述。在Windows平台下屏幕录像则要稍微复杂一些。在Windows平台下，使用-dshow取代x11grab。一句话介绍：注册录屏dshow滤镜（例如screen-capture-recorder），然后通过dshow获取录屏图像然后编码处理。

因此，在使用FFmpeg屏幕录像之前，需要先安装dshow滤镜。在这里推荐一个软件：screen capture recorder。安装这个软件之后，就可以通过FFmpeg屏幕录像了。



screen capture recorder项目主页：

[http://sourceforge.net/projects/screencapturer/](http://sourceforge.net/projects/screencapturer/)

下载地址：

[http://sourceforge.net/projects/screencapturer/files](http://sourceforge.net/projects/screencapturer/files)

下载完后，一路“Next”即可安装完毕。注意，需要Java运行环境（Java Runtime Environment），如果没有的话下载一个就行。

screen capture recorder本身就可以录屏，不过这里我们使用FFmpeg进行录屏。

![](https://img-blog.csdn.net/20140730013631727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

#### 3.1. 编码为H.264，保存为本地文件

下面的命令可以将屏幕录制后编码为H.264并保存为本地文件。



`ffmpeg -f dshow -i video="screen-capture-recorder" -r 5 -vcodec libx264 -preset:v ultrafast -tune:v zerolatency MyDesktop.mkv`



注：“-r 5”的意思是把帧率设置成5。

最后得到的效果如下图。

![](https://img-blog.csdn.net/20140730013701321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此外，也可以录声音，声音输入可以分成两种：一种是真人说话的声音，通过话筒输入；一种是虚拟的声音，即录屏的时候电脑耳机里的声音。下面两条命令可以分别录制话筒的声音和电脑耳机里的声音。

录屏，伴随话筒输入的声音



`ffmpeg -f dshow -i video="screen-capture-recorder" -f dshow -i audio="鍐呰楹﹀厠椋?(Conexant 20672SmartAudi" -r 5 -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -acodec libmp3lame MyDesktop.mkv`



上述命令有问题：audio那里有乱码，把乱码ANSI转UTF-8之后（乱码变成“内装麦克”）依然不行，这个问题先留着，以后有时间再解决。

PS：感觉这条命令适合做讲座之类的时候使用



录屏，伴随耳机输入的声音



`ffmpeg -f dshow -i video="screen-capture-recorder" -f dshow -i audio="virtual-audio-capturer" -r 5 -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -acodec libmp3lame MyDesktop.mkv`



PS：测这条命令的时候，这在听歌，因此录制的视频中的音频就是那首歌曲。



#### 3.2. 编码为H.264，发布UDP

下面的命令可以将屏幕录制后编码为H.264并封装成UDP发送到组播地址



`ffmpeg -f dshow -i video="screen-capture-recorder" -r 5 -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f h264 udp://233.233.233.223:6666`



注1：考虑到提高libx264的编码速度，添加了-preset:v ultrafast和-tune:v zerolatency两个选项。

注2：高分辨率的情况下，使用UDP可能出现丢包的情况。为了避免这种情况，可以添加–s 参数（例如-s 320x240）调小分辨率。

#### 3.3. 编码为H.264，发布RTP

下面的命令可以将屏幕录制后编码为H.264并封装成RTP并发送到组播地址



`ffmpeg -f dshow -i video="screen-capture-recorder" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f rtp rtp://233.233.233.223:6666>test.sdp`



注1：考虑到提高libx264的编码速度，添加了-preset:v ultrafast和-tune:v zerolatency两个选项。

注2：结尾添加“>test.sdp”可以在发布的同时生成sdp文件。该文件可以用于该视频流的播放。如下命令即可播放：



`ffplay test.sdp`



#### 3.4. 编码为H.264，发布RTMP

原理同上，不再赘述。



`ffmpeg -f dshow -i video="Integrated Camera" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f flv rtmp://localhost/oflaDemo/livestream`



注意：播放RTMP的时候，-max_delay参数会比较明显的影响延迟，将此参数值设定小一些，有利于降低延时。



`ffplay -max_delay 100000 "rtmp://localhost/oflaDemo/livestream live=1"`








