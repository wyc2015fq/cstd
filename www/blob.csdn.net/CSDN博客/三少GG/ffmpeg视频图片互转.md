# ffmpeg视频图片互转 - 三少GG - CSDN博客
2013年01月30日 10:58:44[三少GG](https://me.csdn.net/scut1135)阅读数：18617

### [把图片转换成视频-ffmpeg](http://blog.csdn.net/huangxiansheng1980/article/details/6819271)
分类： [FFMpeg](http://blog.csdn.net/huangxiansheng1980/article/category/704716)2011-09-25
 12:511811人阅读[评论](http://blog.csdn.net/huangxiansheng1980/article/details/6819271#comments)(0)[收藏]()[举报](http://blog.csdn.net/huangxiansheng1980/article/details/6819271#report)
ffmpeg是一个非常强大的工具，使用他可以转换视频格式，包括视频容器格式，视频编码格式。这里所说的视频，包括了视频和音频。可以更改视频的其他的参数，比如帧率-每秒播放的帧数， 视频的大小-尺寸等等。还可以将图片转换视频。
图片转换成视频，实际上是通过内置的视频编码格式将你的图片编码，ffmpeg支持很多中视频格式，想知道ffmpeg支持哪些编码格式，可以用ffmpeg -codecs命令来查看。
你的图片可以是jpg， 可以bmp，可以是tiff。其他的格式可以自己测试。
运行cmd，cd到你ffmpeg所在的目录，如果你手上还没有ffmpeg的话，你可以通过一下连接下载：
<暂时空缺， 随后加上连接>
好了，有了这个软件，我们就可以开始生成视频的工作了。假如你的图片在c:\temp\下面。那么通过下面的命令就可以将这个目录下面的图片转换成视频。这里面有个要求就是你的图片全部是自然数为文件名， 001, 002, 003这样的。前面要几个0取决于你的图片的个数，如109张，那么就是3-1=2个0，从001 到109，如果是1009张就是0001到1009.
**ffmpeg -f image2 -i c:\temp\%02d.jpg test.mp4**
**你可以指定编码格式：**
**ffmpeg -f image2 -i c:\temp\%02d.jpg** -vcodec libx264 **test.mp4**
**也许你还想指定输出帧率：**
**ffmpeg -f image2 -i c:\temp\%02d.jpg** -vcodec libx264 -r 10  **test.mp4**
这样输出的test.mp4就是每秒播放10帧了
**如果你要指定码率：**
**ffmpeg -f image2 -i c:\temp\%02d.jpg** -vcodec libx264 -r 10 -b 200k  **test.mp4**
*注意这里的200k的单位bit/s.*
也许你要问那么到底最后生成的文件的参数都是什么样子，比如我们刚才设置的参数，还有我们没有设置的参数：
那么这个命令就可以帮你搞定：
**ffmpeg -i test.mp4**
看一看我的test.mp4的参数都是什么：
G:\Download\ffmpeg\ffmpeg-git-a254452-win32-static\bin>ffmpeg -i test.mp4
ffmpeg version N-32726-ga254452, Copyright (c) 2000-2011 the FFmpeg developers
  built on Sep 19 2011 19:49:13 with gcc 4.6.1
  configuration: --enable-gpl --enable-version3 --enable-runtime-cpudetect --ena
ble-avisynth --enable-bzlib --enable-frei0r --enable-libopencore-amrnb --enable-
libopencore-amrwb --enable-libfreetype --enable-libgsm --enable-libmp3lame --ena
ble-libopenjpeg --enable-librtmp --enable-libschroedinger --enable-libspeex --en
able-libtheora --enable-libvo-aacenc --enable-libvo-amrwbenc --enable-libvorbis
--enable-libvpx --enable-libx264 --enable-libxavs --enable-libxvid --enable-zlib
  libavutil    51. 16. 1 / 51. 16. 1
  libavcodec   53. 16. 0 / 53. 16. 0
  libavformat  53. 12. 0 / 53. 12. 0
  libavdevice  53.  4. 0 / 53.  4. 0
  libavfilter   2. 43. 2 /  2. 43. 2
  libswscale    2.  1. 0 /  2.  1. 0
  libpostproc  51.  2. 0 / 51.  2. 0
Input #0, mov,mp4,m4a,3gp,3g2,mj2, from 'test.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2avc1mp41
    creation_time   : 1970-01-01 00:00:00
    encoder         : Lavf53.12.0
  Duration: 00:00:10.00, start: 0.000000, bitrate: 2540 kb/s
    Stream #0.0(und): Video: h264 (High) (avc1 / 0x31637661), yuvj420p, 1280x720
 [SAR 1:1 DAR 16:9], 2539 kb/s, 10 fps, 10 tbr, 10 tbn, 20 tbc
    Metadata:
      creation_time   : 1970-01-01 00:00:00
At least one output file must be specified
前面是你用的ffmpeg的基本信息：
包括版本，编译时间，用的编译器，配置选项。
重点看绿色部分，这也是你会关心的东西：
duration告诉视频时长，这里是10秒， bitrate是码率。
video： 视频编码格式。 帧率是10fps。
### [视频转换诚图片-ffmpeg](http://blog.csdn.net/huangxiansheng1980/article/details/6820866)
分类： [FFMpeg](http://blog.csdn.net/huangxiansheng1980/article/category/704716)2011-09-25
 23:57492人阅读[评论](http://blog.csdn.net/huangxiansheng1980/article/details/6820866#comments)(2)[收藏]()[举报](http://blog.csdn.net/huangxiansheng1980/article/details/6820866#report)
利用ffmpeg这个强大的工具可以将一系列的图片转换成视频-这部分请参看我的文章：
http://blog.csdn.net/huangxiansheng1980/article/details/6819271
反过来也可以做到-视频转换成一张张的图片，命令格式：
**ffmpeg -i  ./test/video.mpg -r 1 -f image2 temp/%05d.png**
上面这个例子是将./test/目录下面的video.mpg按照每秒截取一张图片的频率方式，并且将得到的图片自然数递增的顺序命名规则命名，并且保存./temp下面。
如果你想每秒10帧，那就将1改成10：
**ffmpeg -i  ./test/video.mpg -r 10 -f image2 temp/%05d.png**
图片的格式也可以是jpg, png, bmp。如：
**ffmpeg -i  ./test/video.mpg -r 10 -f image2 temp/%05d.bmp**
就会把视频转换为位图，位图的格式默认是16位色（这个有待进一步验证，因为我的实验刚好能被转换成16位色的），关于这个如果有人发现有不一样的结果，希望共享一下。
