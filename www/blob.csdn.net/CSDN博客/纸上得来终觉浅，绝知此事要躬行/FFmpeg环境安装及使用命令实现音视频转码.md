# FFmpeg环境安装及使用命令实现音视频转码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月28日 14:56:08[boonya](https://me.csdn.net/boonya)阅读数：290








官方地址：[http://www.ffmpeg.org/](http://www.ffmpeg.org/)

FFmpeg是领先的多媒体框架，能够解码，编码，转码，复用，解复用，流式传输，过滤和播放人类和机器创建的任何内容。 它支持最晦涩难懂的古代格式，直至最前沿。 无论它们是由某些标准委员会，社区还是公司设计的。 它还具有高度可移植性：FFmpeg在各种构建环境，机器架构和配置下，跨Linux，Mac OS X，Microsoft Windows，BSD，Solaris等编译，运行和传递我们的测试基础架构FATE。

## Windows下安装FFmpeg环境

Windows下安装FFmpeg，只需要[下载](https://ffmpeg.zeranoe.com/builds/)对应32bit或64bit的版本:

[https://ffmpeg.zeranoe.com/builds/](https://ffmpeg.zeranoe.com/builds/)

![](https://img-blog.csdn.net/20180928150310584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解压，将以下三个文件放置于系统System32下面即可。

![](https://img-blog.csdn.net/20180928150135753?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## Linux下FFmpeg安装及配置

**ffmpeg-4.0.tar.bz2 **

```
tar -zxvf ffmpeg-3.4.tar.bz2

cd ffmpeg-3.4

./configure --enable-shared --prefix=/usr/local/ffmpeg --enable-libmp3lame --enable-gpl --enable-version3 --enable-nonfree --enable-pthreads --enable-postproc --enable-ffplay --enable-libx264 --enable-libxvid

make

make install

make clean
```

编译完成后，进入/etc/profile中将ffmpeg加入到环境变量后面加上如下内容：
`export PATH=/usr/local/ffmpeg/bin:$PATH`
把ffmpeg的库加入/etc/ld.so.conf中
`sudo gedit /etc/ld.so.conf`
检查安装是否成功：
`ffmpeg -version`
## FFmpeg常用命令

参考:[https://www.cnblogs.com/dwdxdy/p/3240167.html](https://www.cnblogs.com/dwdxdy/p/3240167.html)

或参考：[https://www.cnblogs.com/xuan52rock/p/7929509.html](https://www.cnblogs.com/xuan52rock/p/7929509.html)

**1.分离视频音频流**

```
ffmpeg -i input_file -vcodec copy -an output_file_video　　//分离视频流
ffmpeg -i input_file -acodec copy -vn output_file_audio　　//分离音频流
```

**2.视频解复用**

```
ffmpeg –i test.mp4 –vcodec copy –an –f m4v test.264
ffmpeg –i test.avi –vcodec copy –an –f m4v test.264
```

**3.视频转码**

```
ffmpeg –i test.mp4 –vcodec h264 –s 352*278 –an –f m4v test.264              //转码为码流原始文件
ffmpeg –i test.mp4 –vcodec h264 –bf 0 –g 25 –s 352*278 –an –f m4v test.264  //转码为码流原始文件
ffmpeg –i test.avi -vcodec mpeg4 –vtag xvid –qsame test_xvid.avi            //转码为封装文件
//-bf B帧数目控制，-g 关键帧间隔控制，-s 分辨率控制
```

**4.视频封装**
`ffmpeg –i video_file –i audio_file –vcodec copy –acodec copy output_file`
**5.视频剪切**

```
ffmpeg –i test.avi –r 1 –f image2 image-%3d.jpeg        //提取图片
ffmpeg -ss 0:1:30 -t 0:0:20 -i input.avi -vcodec copy -acodec copy output.avi    //剪切视频
//-r 提取图像的频率，-ss 开始时间，-t 持续时间
```

**6.视频录制**
`ffmpeg –i rtsp://192.168.3.205:5555/test –vcodec copy out.avi`
**7.YUV序列播放**
`ffplay -f rawvideo -video_size 1920x1080 input.yuv`
**8.YUV序列转AVI**
`ffmpeg –s w*h –pix_fmt yuv420p –i input.yuv –vcodec mpeg4 output.avi`
**常用参数说明：**

**主要参数：**

-i 设定输入流

-f 设定输出格式

-ss 开始时间
**视频参数：**

-b 设定视频流量，默认为200Kbit/s

-r 设定帧速率，默认为25

-s 设定画面的宽与高

-aspect 设定画面的比例

-vn 不处理视频

-vcodec 设定视频编解码器，未设定时则使用与输入流相同的编解码器
**音频参数：**

-ar 设定采样率

-ac 设定声音的Channel数

-acodec 设定声音编解码器，未设定时则使用与输入流相同的编解码器

-an 不处理音频

## FFmpeg将mp4转成h264

```
C:\Users\Administrator\Desktop\Desktop\SDL>ffmpeg -i chenyixun.mp4  -vcodec copy -acodec copy chenyixun-new.ts
ffmpeg version 4.0.2 Copyright (c) 2000-2018 the FFmpeg developers
  built with gcc 7.3.1 (GCC) 20180722
  configuration: --enable-gpl --enable-version3 --enable-sdl2 --enable-bzlib --enable-fontconfig --enable-gnutls --enable-iconv --enable-libass --enable-libbluray --enable-libfreetype --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libopenjpeg --enable-libopus --enable-libshine --enable-libsnappy --enable-libsoxr --enable-libtheora --enable-libtwolame --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx264 --enable-libx265 --enable-libxml2 --enable-libzimg --enable-lzma --enable-zlib --enable-gmp --enable-libvidstab --enable-libvorbis --enable-libvo-amrwbenc --enable-libmysofa --enable-libspeex --enable-libxvid --enable-libaom --enable-libmfx --enable-amf --enable-ffnvcodec --enable-cuvid --enable-d3d11va --enable-nvenc --enable-nvdec --enable-dxva2 --enable-avisynth
  libavutil      56. 14.100 / 56. 14.100
  libavcodec     58. 18.100 / 58. 18.100
  libavformat    58. 12.100 / 58. 12.100
  libavdevice    58.  3.100 / 58.  3.100
  libavfilter     7. 16.100 /  7. 16.100
  libswscale      5.  1.100 /  5.  1.100
  libswresample   3.  1.100 /  3.  1.100
  libpostproc    55.  1.100 / 55.  1.100
Input #0, mov,mp4,m4a,3gp,3g2,mj2, from 'chenyixun.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2avc1mp41
    encoder         : Lavf54.63.104
  Duration: 00:04:03.72, start: 0.000000, bitrate: 2135 kb/s
    Stream #0:0(und): Video: h264 (High) (avc1 / 0x31637661), yuv420p(tv, bt709), 1920x1080 [SAR 1:1 DAR 16:9], 2004 kb/s, 23.98 fps, 23.98 tbr, 90k tbn, 47.95 tbc (default)
    Metadata:
      handler_name    : VideoHandler
    Stream #0:1(und): Audio: aac (LC) (mp4a / 0x6134706D), 44100 Hz, stereo, fltp, 125 kb/s (default)
    Metadata:
      handler_name    : SoundHandler
Output #0, mpegts, to 'chenyixun-new.ts':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2avc1mp41
    encoder         : Lavf58.12.100
    Stream #0:0(und): Video: h264 (High) (avc1 / 0x31637661), yuv420p(tv, bt709), 1920x1080 [SAR 1:1 DAR 16:9], q=2-31, 2004 kb/s, 23.98 fps, 23.98 tbr, 90k tbn, 90k tbc (default)
    Metadata:
      handler_name    : VideoHandler
    Stream #0:1(und): Audio: aac (LC) (mp4a / 0x6134706D), 44100 Hz, stereo, fltp, 125 kb/s (default)
    Metadata:
      handler_name    : SoundHandler
Stream mapping:
  Stream #0:0 -> #0:0 (copy)
  Stream #0:1 -> #0:1 (copy)
Press [q] to stop, [?] for help
frame= 5842 fps=0.0 q=-1.0 Lsize=   69231kB time=00:04:03.69 bitrate=2327.3kbits/s speed= 412x
video:59608kB audio:3736kB subtitle:0kB other streams:0kB global headers:0kB muxing overhead: 9.293417%

C:\Users\Administrator\Desktop\Desktop\SDL>
```

注：建议采用copy的方式，速度相当快，不用重新转码。如果你愿意等，也可以用最简单的方式进行转码：
`C:\Users\Administrator\Desktop\Desktop\SDL>ffmpeg -i chenyixun.mp4  chenyixun-new.ts`
 FFmpeg会根据目标文件的后缀名决定转码出什么格式，切忌不要直接写成:
`C:\Users\Administrator\Desktop\Desktop\SDL>ffmpeg -i chenyixun.mp4  chenyixun-new.h264`
注意：mp4和h264是视频容器，如果要将mp4转成h264你的后缀应该是".ts"。



