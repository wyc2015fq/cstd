# OBS (open boardcast server)结构分析 - 一世豁然的专栏 - CSDN博客





2017年11月27日 16:33:46[一世豁然](https://me.csdn.net/Explorer_day)阅读数：361








本博客转载于：[https://www.cnblogs.com/xylc/p/3640994.html](https://www.cnblogs.com/xylc/p/3640994.html)






## 一，简介

> 
OBS（open boardcast server），是一个用于直播的开源软件。

官方网站：[https://obsproject.com/](https://obsproject.com/)

代码托管地址：[https://github.com/jp9000/OBS](https://github.com/jp9000/OBS)

git地址：[https://github.com/jp9000/OBS.git](https://github.com/jp9000/OBS.git)


## 二，结构

> 
项目文件结构如下图：

![obs](http://images.cnitblog.com/blog/296875/201404/021626166407415.png)


## 三，分析

> 
最新的OBS只能在vista以上操作系统中运行，显卡必须至少支持DX10。

OBS的编译需要visual studio 2013（C++11重写了部分代码），当然你也可以switch到老版本下进行编译，但新的特性或bug修正就无法启用了。

OBS包括五大组件，以及一些附加功能：

### video source ：

a，DShow的capture filter；

b，游戏画面，hook部分通过minihook实现，获得的surface分别通过D3D10共享在OBS主窗口上显示，以及通过共享内存发送到OBS进程中进行encode；

c，桌面窗口画面，通过GDI实现；

d，图片；

### audio source：

a，麦克风及桌面声音，通过MMDevice获取；

b，blank，无声音；

### video encoder：

a，x264；

b，nvidia cuda video encoder；

c，intel QSV video encode；

### audio encoder：

a，MP3，通过lame库实现；

b，aac，通过libfaac库实现；

### stream destination：

a，rtmp stream，通过librtmp实现；

b，file stream，可生成FLV/MP4的文件；

### 附加功能：

a，音频重采样，通过libsamplerate库实现；

b，音频去噪；

c，自动更新；


## 四，总结：

> 
OBS逻辑上区分的很清楚，但是代码部分非常杂揉。因而作者正在写一个新的跨windows/linux/mac平台的C++11适配的project。

地址：[https://github.com/jp9000/obs-studio](https://github.com/jp9000/obs-studio)










