# C#使用APlayer开发自制媒体播放器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月07日 22:48:23[boonya](https://me.csdn.net/boonya)阅读数：4667








首先简单来了解下什么是APlayer。下面的内容你都可以通过[http://aplayer.open.xunlei.com/](http://aplayer.open.xunlei.com/)轻松地进行查看。

## 引擎介绍

APlayer 媒体播放引擎是迅雷公司从 2009 年开始开发的通用音视频媒体文件播放内核。迅雷看看播放器和迅雷影音就是使用 APlayer 作为播放内核，目前迅雷看看播放器在PC播放器市场占有率排第一。本质上APlayer播放引擎是一个ActiveX控件，可以被本地其他外部程序嵌入调用，也可以直接被网页嵌入，类似于(Adobe Flash Player)。APlayer不包含界面，但另一个依赖于APlayer的ActiveX控件APlayerUI可以提供丰富的界面元素(播放控制条，Flash广告等)，APlayerUI 也被包含在 APlayerSDK 中。




APlayer媒体播放引擎使用了以下第三方库： [ffmpeg](http://ffmpeg.org/)[zlib](http://www.zlib.org/)[libpng](http://www.libpng.org/)[dlib](http://dlib.net/)[](http://dlib.net/)





## 基于Windows平台的播放内核


- 封闭式 DirectShow 架构，不受系统解码环境干扰
- 全媒体文件格式支持
- 丰富的媒体文件传输协议(http/https/ftp/mms/rtsp/rtmp/hls等)
- 强大功能(字幕、音轨、转码、画质增强、截GIF、人脸检测……)
- 支持将多个 ts/flv/mp4 分段切片文件组成 m3u8 无缝播放 
- 支持最新的 H.265(HEVC) 解码 (Powered by[北京视骏科技有限公司](http://www.strongene.com/))
- 支持全景视频和虚拟现实（VR）眼镜 Oculus DK2 的播放
- 支持播放USB摄像头和屏幕或窗口的动态截图




    无法演示, APlayer 未安装! 
请下载 [APlayerSDK](http://aplayer.open.xunlei.com/APlayerSDK.zip) 解压后，使用其中的 install.bat 安装。





以下文章转载自：[APlayer组件自制播放器](http://www.cnblogs.com/Leo_wl/p/4267811.html)



## [.NET中使用APlayer组件自制播放器](http://www.cnblogs.com/xiaozhi_5638/p/4266855.html)



2015-02-02 09:46 by xiaozhi_5638, 4259

**目录**
- 说明
- APlayer介绍
- APlayer具备功能
- APlayer使用
- 自制播放器Demo
- 未完成工作
- 源码下载



**说明**

由于需求原因，需要在项目中（桌面程序）集成一个在线播放视频的功能。大概要具备“流式”边下载边播放的能力。自带的media player不太好看又不让改动，网上找了很多开源的播放器，效果也都不佳（可能还没碰到比较好的）。后来找到了迅雷公司的APlayer播放引擎（其实就是一个ActiveX控件），感觉官网上的介绍还行，后来试了一下发现不错。

官方参考文档以及demo都是C++版本的，或者是Web版的，网上几乎也找不到用.NET开发的相关资料，不过幸好也不太难，:-)



**APlayer介绍**

APlayer 媒体播放引擎是迅雷公司从 2009 年开始开发的通用音视频媒体文件播放内核。

迅雷看看播放器和迅雷影音就是使用 APlayer 作为播放内核，目前迅雷看看播放器在PC播放器市场占有率排第一。

本质上APlayer播放引擎是一个ActiveX控件，可以被本地其他外部程序嵌入调用，也可以直接被网页嵌入，类似于(Adobe Flash Player)。APlayer不包含界面，但另一个依赖于APlayer的ActiveX控件APlayerUI可以提供丰富的界面元素(播放控制条，Flash广告等)，APlayerUI 也被包含在 APlayerSDK 中。

”

具体不多说了，参见官方网站的介绍 [http://aplayer.open.xunlei.com/](http://aplayer.open.xunlei.com/)。



**APlayer具备功能**
- 封闭式 DirectShow 架构，不受系统解码环境干扰
- 全媒体文件格式支持
- 支持丰富的媒体文件传输协议(http/ftp/mms/rtsp/rtmp/hls等)
- 支持将多个 ts/flv/mp4 分段切片文件组成 m3u8 无缝播放
- 强大的附加功能(字幕、音轨、转码/转格式、画质增强、截GIF……)
- 支持最新的 H.265(HEVC) 解码

由上可以看出，它是以DirectShow为基础开发出来的。基本上你能想到的功能它几乎都包含（注，我没有全部测试，只是使用了一部分相当简单的功能），你从开发文档中关于SetConfig/GetConfig API的使用就可以看出，它包含的功能之多。下图是调用SetConfig API对APlayer进行设置的文档截图：

![](http://images.cnitblog.com/blog/104032/201502/020921165154079.png)

以上只是一部分，并不全。



**APlayer使用**

APlayer可以用在Web中，也可以用在桌面程序中，据说还出了Android、IOS以及Windows Phone版本的。由于我只试过桌面版的，所以在Web中使用效果我只截一个自带的效果图。
- **Web中的使用：**

**![](http://images.cnitblog.com/blog/104032/201502/020922184069253.png)**
- **.NET Winform中的使用：**

其实也很简单，先要注册一下COM组件。

![](http://images.cnitblog.com/blog/104032/201502/020923199219314.png)

如上图，运行从官网上下载的SDK包中的install.bat。注册成功后，在VS的工具箱中添加COM控件：

注意，如果组件不知道怎么配置可以参考：[C# 调用APlayer教程](http://blog.csdn.net/qq_19527641/article/details/52486996) 主要是运用到官方的SDK和编码库。




### APlayer下载
- [APlayerSDK(版本：3.9.1.763)](http://aplayer.open.xunlei.com/APlayerSDK.zip)
- [APlayer完整解码库(版本同上)](http://aplayer.open.xunlei.com/codecs.zip)



建议直接到[APlayer官网](http://aplayer.open.xunlei.com/)下载最新版本，如果编码库已经覆盖了SDK bin下的codecs还是不能加载出来组件需要用“管理员权限”运行cmd 到SDK路径运行install.bat.



![](http://images.cnitblog.com/blog/104032/201502/020924287039894.png)

选择 APlayer3 Control后，点击确定。之后你就可以在工具箱中看到APlayer播放器控件：

![](http://images.cnitblog.com/blog/104032/201502/020925222188876.png)

之后的用法与其他.NET控件差别不大。注意向窗体设计器中拖入控件后，项目中自动会添加两个引用：

![](http://images.cnitblog.com/blog/104032/201502/020926049535966.png)

添加引用的过程不需要人工添加。这里注意要将这两个引用的“独立”属性改为true。这样的话，编译成功后，VS自动会将非托管DLL（官网上下载的SDK中的dll）复制到debug目录中（注意必须这样做，不然程序发布后，换一台电脑运行不了，提示要安装COM组件）

![](http://images.cnitblog.com/blog/104032/201502/020927091719284.png)
- **成员方法和事件**

APlayer的成员方法和事件理解起来相当简单，基本上就是对应“打开”、“播放”、“暂停”、“关闭”、“设置音量”、“获取视频长度”、“缓冲进度”、“快进/倒退”等等。几乎跟我们平时使用播放器的动作一致，下面一张图显示的是成员方法和事件，用法很简单，我就不具体说了。

![](http://images.cnitblog.com/blog/104032/201502/020927515319858.png)

名字很好理解，稍微懂点英文的都知道。

另外，需要重点说一下的是OnMessage事件，控件的鼠标、键盘等事件都需要通过该事件来捕获，好像没有其他方式。



**自制播放器Demo**

我自己花一点时间做了一个demo，顺便也是为了熟悉它的使用。Demo源码的注释已经非常清楚详细，我就不贴代码了，贴几张图：

播放本地视频：

![](http://images.cnitblog.com/blog/104032/201502/020929247031858.png)

播放网络视频（缓冲中）：

![](http://images.cnitblog.com/blog/104032/201502/020930062655787.png)

模仿播放器部分功能：

![](http://images.cnitblog.com/blog/104032/201502/020930544681581.jpg)

只是简单的模仿播放器的部分功能（显示字幕、截图）。还有很多功能有待发掘，基本上都是使用APlayer.SetConfig和GetConfig两个方法来完成的，具体参数请参见帮助文档。



**未完成工作**

未完成的工作太多了，多得数不过来，毕竟这只是一个demo :-)。

但是我目前有一个疑问，好像没有公开“获取已缓冲数据”的API，我怎么知道已经缓冲了多少数据呢？没有这个API的话，就做不出来播放器进度条上“显示已经缓冲完成帧”的效果。可能我还没发现，比较仓促。

注意每种视频格式都需要对应的解码库，可以从官网上下载完整解码库，不过比较大，全部解压完成后，80多M。所以实际应用中，最好不要包含全部的解码库。当需要的解码库不在../codecs目录中时，APlayer会激发OnDownloadCodec事件，提示下载解码器。



**源码下载**

源码下载地址：[http://files.cnblogs.com/files/xiaozhi_5638/VideoPlayer.rar](http://files.cnblogs.com/files/xiaozhi_5638/VideoPlayer.rar)（注意由于解码库太大，所以debug目录中的codecs目录为空，如果要运行请自己下载解码库）

开发SDK下载地址：[http://aplayer.open.xunlei.com/APlayerSDK.zip](http://aplayer.open.xunlei.com/APlayerSDK.zip)

完整解码库下载地址：[http://aplayer.open.xunlei.com/codecs.zip](http://aplayer.open.xunlei.com/codecs.zip)（84M）




