# 降低FFmpeg的编码延时 - 爱写代码的自由人 - CSDN博客





2018年02月21日 12:27:24[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1139








   之前用FFmpeg的H264编码器发现编码出来的视频延时很大，在解码器一端优化各种可能产生延时的地方之后，延时还是有一秒，上网一查发现很多人遇到相同的问题，不过好在看到一个帖子给出了这个问题的解决方法。

   原来，FFmpeg的H264编码器为了提高编码的图像质量，默认会缓冲多帧图像，这个缓冲帧数跟GOP大小有关。我程序设置GOP等于100，结果缓冲了20多帧。为了降低编码延时，可以添加-preset:v ultrafast和-tune:v zerolatency两个选项，例如下面的   FFmpeg命令行采集摄像头的图像用H264编码并推流到一个服务器地址：

```cpp
ffmpeg -f dshow -i video="USB Camera" -vcodec libx264 -preset:v ultrafast -tune:v zerolatency -f rtp rtp://233.233.233.223:6666>test.sdp
```

preset可设置的等级可分为：

```cpp
ultrafast,superfast, veryfast, faster, fast, medium, slow, slower, veryslow
```

其中ultrafast编码速度最快。

一般发送网络流都需要增加这两个参数，否则播放图像的延时很大。

 如果用代码，可以这样设置：

```cpp
/**
 * ultrafast,superfast, veryfast, faster, fast, medium
 * slow, slower, veryslow, placebo.　
 */
AVDictionary *param = 0;
av_dict_set(¶m, "preset", "fast", 0); // av_opt_set(pCodecCtx->priv_data,"preset","fast",0);
av_dict_set(¶m, "tune", "zerolatency", 0);
```











