# 开源直播OBS ( Open Broadcaster Software ) 多机位，多场景切换 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月27日 17:00:25[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1191

项目主页：[https://obsproject.com/download](https://obsproject.com/download)
软件是基于MFC的，下载源码后一次性就可以编译通过，使用很简单。
使用了以下开源库：
  x264                   - H.264编码库
  libmp3lame         - MP3编码库
  libfaac                - AAC编码库
  librtmp                - RTMP协议工具库
  libsamplerate        -音频重采样库
有些什么功能
推流设置
![](http://note.youdao.com/yws/res/100990/WEBRESOURCE76ac3b3993be63f2711bbbf1be47659a)
obs设置重置
you can reset everything including the scenes by closing obs and deleting %AppData%\OBS\ obs will set everything back to default when it doesn't detect the settings folder..
: C:\Users\MYUSER\AppData\Roaming\obs-studio
还可以游戏直播（来源->游戏捕获），还可以窗口捕获
场景切换，多机位切换：新建多个场景，后面就可以直接点击不同的场景快速切换了。
![](http://note.youdao.com/yws/res/101005/WEBRESOURCE49e8e6c357a047be8603c785a184d4c7)
剪切：直接切换，无动画效果
[https://zhuanlan.zhihu.com/p/28000157](https://zhuanlan.zhihu.com/p/28000157)
如果出现2个窗口，是进入了工作室模式，再点下工作室模式就关闭了。
