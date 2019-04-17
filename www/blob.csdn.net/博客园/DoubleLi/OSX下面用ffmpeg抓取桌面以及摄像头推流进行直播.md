# OSX下面用ffmpeg抓取桌面以及摄像头推流进行直播 - DoubleLi - 博客园






首先查看ffmpeg是否支持对应的设备，在OSX下面，Video和Audio设备使用的是avfoundation，所以可以使用avfoundation来查看





点击(此处)折叠或打开


- [StevenLiu@localhost ffmpeg]$ ./ffmpeg -f avfoundation -list_devices true -i ""
- ffmpeg version N-68139-g24e7e02 Copyright (c) 2000-2014 the FFmpeg developers
-   built on Dec 2 2014 09:46:29 with Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
-   configuration: --enable-opengl --enable-libx265 --enable-libx264 --enable-libmp3lame --enable-gpl --enable-libfaac --enable-nonfree --prefix=/usr/ --enable-libfreetype --enable-libass
-   libavutil 54. 15.100 / 54. 15.100
-   libavcodec 56. 13.100 / 56. 13.100
-   libavformat 56. 15.101 / 56. 15.101
-   libavdevice 56. 3.100 / 56. 3.100
-   libavfilter 5. 2.103 / 5. 2.103
-   libswscale 3. 1.101 / 3. 1.101
-   libswresample 1. 1.100 / 1. 1.100
-   libpostproc 53. 3.100 / 53. 3.100
- [AVFoundation input device @ 0x7f99f1c00b80] AVFoundation video devices:
- [AVFoundation input device @ 0x7f99f1c00b80] [0] FaceTime HD Camera (Built-in)
- [AVFoundation input device @ 0x7f99f1c00b80] [1] Capture screen 0
- [AVFoundation input device @ 0x7f99f1c00b80] AVFoundation audio devices:
- [AVFoundation input device @ 0x7f99f1c00b80] [0] Built-in Microphone
- : Input/output error
- [StevenLiu@localhost ffmpeg]$






通过输出内容可以看到，video部分支持的时两个设备





点击(此处)折叠或打开


- [0] bbs.chinaffmpeg.com 孙悟空 FaceTime HD Camera (Built-in)
- [1] Capture screen 0




音频部分支持的是一个设备




点击(此处)折叠或打开


- [0] Built-in Microphone




如果希望将桌面录制或者分享，可以使用命令行如下：




点击(此处)折叠或打开


- ./ffmpeg -f avfoundation -i "1" -vcodec libx264 -preset ultrafast -acodec libfaac -f flv rtmp://ble.ossrs.net/live/xxxx




这个只能够推桌面。



如果需要桌面+麦克风，比如一般做远程教育分享ppt或者桌面，有音频讲解，例如bbs.chinaffmpeg.com 孙悟空 命令行如下：




点击(此处)折叠或打开


- ./ffmpeg -f avfoundation -i "1:0" -vcodec libx264 -preset ultrafast -acodec libfaac -f flv rtmp://ble.ossrs.net/live/xxxx




这个可以推桌面+麦克风。





如果需要桌面+麦克风，并且还要摄像头拍摄到自己，比如一般用于互动主播，游戏主播，命令行如下




点击(此处)折叠或打开


- ./ffmpeg -f avfoundation -i "1:0" -f avfoundation -i "0" -filter_complex 'overlay=10:main_h-overlay_h-10' -vcodec libx264 -b:v 800k -r:v 25-s 480x320 -preset ultrafast -acodec libfaac -f flv rtmp://ble.ossrs.net/live/xxxxx




这个可以推桌面+麦克风，并且摄像头把人头放在界面下面



下面来个截图
![](http://blog.chinaunix.net/attachment/201412/2/11344913_1417490291lWzz.jpg)









