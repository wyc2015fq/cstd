# ffplay播放视频源延时的参数设置 - xcyl的口袋 - CSDN博客
2016年09月23日 13:05:42[xcyl](https://me.csdn.net/cai6811376)阅读数：3063
使用ffplay播放视频源时，rtsp/rtmp等，会有一定的延时，这里我们可以通过设置ffplay播放参数将延时控制到最小。
> 
ffplay.exe -i rtmp://xxxxxxx -fflags nobuffer 
  减少缓冲
也可以减少分析码流的时间
> 
ffplay.exe -i rtmp://xxxxxxx -analyzeduration 1000000 
  码流分析时间设置，单位为微秒
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
