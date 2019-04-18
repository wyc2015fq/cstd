# [SimplePlayer] 实现一个简单的播放器 - TaigaComplex求职中 - 博客园







# [[SimplePlayer] 实现一个简单的播放器](https://www.cnblogs.com/TaigaCon/p/9584112.html)





简单的播放器需要实现一个最基本的功能：播放视频文件。

实现这个功能需要包含以下几个步骤：
- [从视频文件中提取视频图像](https://www.cnblogs.com/TaigaCon/p/9603854.html)
- [在屏幕上显示视频图像](https://www.cnblogs.com/TaigaCon/p/9622016.html)
- [视频帧的同步，也就是保证视频图像在合适的时间在屏幕上显示](https://www.cnblogs.com/TaigaCon/p/9819819.html)
- [从视频文件中提取音频](https://www.cnblogs.com/TaigaCon/p/9901527.html)
- [向音频设备输出音频](https://www.cnblogs.com/TaigaCon/p/9906737.html)
- [音频同步，也就是保证合适的时间输出合适的音频](https://www.cnblogs.com/TaigaCon/p/9906812.html)
- [多线程处理](https://www.cnblogs.com/TaigaCon/p/9986827.html)
- [音视频同步](https://www.cnblogs.com/TaigaCon/p/10010490.html)

本实现是通过ffmpeg来实现音视频的提取，通过sdl2来实现音视频的输出，版本如下：
libavutil      56. 19.100 / 56. 19.100
libavcodec     58. 23.100 / 58. 23.100
libavformat    58. 17.103 / 58. 17.103
libavdevice    58.  4.101 / 58.  4.101
libavfilter     7. 26.100 /  7. 26.100
libswscale      5.  2.100 /  5.  2.100
libswresample   3.  2.100 /  3.  2.100
libpostproc    55.  2.100 / 55.  2.100

sdl2       2.0.5


[Github Repository](https://github.com/Minohh/SimplePlayer)












