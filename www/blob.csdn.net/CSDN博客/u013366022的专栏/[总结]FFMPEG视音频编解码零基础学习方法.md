# [总结]FFMPEG视音频编解码零基础学习方法 - u013366022的专栏 - CSDN博客
2014年07月10日 11:07:16[slitaz](https://me.csdn.net/u013366022)阅读数：346
在CSDN上的这一段日子，接触到了很多同行业的人，尤其是使用FFMPEG进行视音频编解码的人，有的已经是有多年经验的“大神”，有的是刚开始学习的初学者。在和大家探讨的过程中，我忽然发现了一个问题：在“大神”和初学者之间好像有一个不可逾越的鸿沟。“大神”们水平高超，探讨着深奥的问题；而初学者们还停留在入门阶段。究竟是什么原因造成的这种“两极分化”呢？最后，我发现了问题的关键：FFMPEG难度比较大，却没有一个循序渐进，由简单到复杂的教程。现在网上的有关FFMPEG的教程多半难度比较大，不太适合刚接触FFMPEG的人学习；而且很多的例子程序编译通不过，极大地打消了学习的积极性。我自己在刚开始学习FFMPEG的时候也遇到了很大的困难。为了帮助更多的人快速成为“大神”，我想总结一个学习FFMPEG的方法，方便大家循序渐进的学习FFMPEG。
![](https://img-blog.csdn.net/20140616102021781)
# **0. 背景知识**
本章主要介绍一下FFMPEG都用在了哪里（在这里仅列几个我所知的，其实远比这个多）。说白了就是为了说明：FFMPEG是非常重要的。
使用FFMPEG作为内核视频播放器：
> 
Mplayer，ffplay，射手播放器，暴风影音，KMPlayer，QQ影音...
使用FFMPEG作为内核的Directshow Filter：
> 
ffdshow，lav filters...
使用FFMPEG作为内核的转码工具：
> 
ffmpeg，格式工厂...
事实上，FFMPEG的视音频编解码功能确实太强大了，几乎囊括了现存所有的视音频编码标准，因此只要做视音频开发，几乎离不开它。
# **1. ffmpeg程序的使用（ffmpeg.exe，ffplay.exe，ffprobe.exe）**
本章主要介绍一下ffmpeg工程包含的三个exe的使用方法。
ffmpeg的官方网站是：[http://ffmpeg.org/](http://ffmpeg.org/)
编译好的windows可用版本的下载地址（官网中可以连接到这个网站，和官方网站保持同步）： [http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)
该网站中的FFMPEG分为3个版本：Static，Shared，Dev。
前两个版本可以直接在命令行中使用，他们的区别在于：Static里面只有3个应用程序：ffmpeg.exe，ffplay.exe，ffprobe.exe，每个exe的体积都很大，相关的Dll已经被编译到exe里面去了。Shared里面除了3个应用程序：ffmpeg.exe，ffplay.exe，ffprobe.exe之外，还有一些Dll，比如说avcodec-54.dll之类的。Shared里面的exe体积很小，他们在运行的时候，到相应的Dll中调用功能。
Dev版本是用于开发的，里面包含了库文件xxx.lib以及头文件xxx.h，这个版本不包含exe文件。
打开系统命令行接面，切换到ffmpeg所在的目录，就可以使用这3个应用程序了。
## **1.1 ffmpeg.exe**
ffmpeg是用于转码的应用程序。
一个简单的转码命令可以这样写：
将input.avi转码成output.ts，并设置视频的码率为640kbps
**[plain]**[view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/15811977#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/15811977#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/344587/fork)
- ffmpeg -i input.avi -b:v 640k output.ts  
具体的使用方法可以参考：[ffmpeg参数中文详细解释](http://blog.csdn.net/leixiaohua1020/article/details/12751349)
详细的使用说明（英文）：[http://ffmpeg.org/ffmpeg.html](http://ffmpeg.org/ffmpeg.html)
## **1.2 ffplay.exe**
ffplay是用于播放的应用程序。
一个简单的播放命令可以这样写：
播放test.avi
**[plain]**[view
 plain](http://blog.csdn.net/leixiaohua1020/article/details/15811977#)[copy](http://blog.csdn.net/leixiaohua1020/article/details/15811977#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/344587/fork)
- ffplay test.avi  
具体的使用方法可以参考：[ffplay的快捷键以及选项](http://blog.csdn.net/leixiaohua1020/article/details/15186441)
详细的使用说明（英文）：[http://ffmpeg.org/ffplay.html](http://ffmpeg.org/ffplay.html)
## **1.3 ffprobe.exe**
ffprobe是用于查看文件格式的应用程序。
这个就不多介绍了。
详细的使用说明（英文）：[http://ffmpeg.org/ffprobe.html](http://ffmpeg.org/ffprobe.html)
# **2. ffmpeg库的使用：视频播放器**
本章开始介绍使用ffmpeg的库进行开发。
## **2.1 ffmpeg库的配置**
从[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)网站上
1.下载Dev版本，里面包含了ffmpeg的xxx.h头文件以及xxx.lib库文件。
2.下载Shared版本，里面包含了ffmpeg的dll文件。
3.将这两部分文件拷贝到VC工程下面就可以了
注：可能会出现问题，参见：[FFMPEG 库移植到 VC 需要的步骤](http://blog.csdn.net/leixiaohua1020/article/details/12747899)
如果不想自己手动配置，可以下载已经配置好的工程：[最简单的基于FFMPEG+SDL的视频播放器](http://download.csdn.net/detail/leixiaohua1020/5122959)
## **2.2 最简单的视频播放器**
学习文章《[100行代码实现最简单的基于FFMPEG+SDL的视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/8652605)》中的代码，这是ffmpeg做视频播放器最简单的代码了，是我自己精简出来的，已经不能再简化了，每一行都很重要。
ffmpeg的函数介绍：[ffmpeg函数介绍](http://blog.csdn.net/leixiaohua1020/article/details/11979565)
注1：播放视频或音频数据的时候会用到SDL。有关SDL可以参考：[SDL介绍](http://blog.csdn.net/leixiaohua1020/article/details/11954039)
         SDL参考文档：[SDL GUIDE 中文译本](http://download.csdn.net/detail/leixiaohua1020/6389841)
注2：如果想查看解码后的数据，需要用到 YUV播放器：[YUV播放器源代码](http://download.csdn.net/detail/leixiaohua1020/6374065)或[YUV
 Player Deluxe](http://www.yuvplayer.com/)都可以
## **2.3 相关结构体的研究**
ffmpeg的结构体之间的关系参考文章：[FFMPEG中最关键的结构体之间的关系](http://blog.csdn.net/leixiaohua1020/article/details/11693997)
结构体中每个变量的分析，参考文章：
[FFMPEG结构体分析：AVFrame](http://blog.csdn.net/leixiaohua1020/article/details/14214577)
> 
[FFMPEG结构体分析：AVFormatContext](http://blog.csdn.net/leixiaohua1020/article/details/14214705)
> 
[FFMPEG结构体分析：AVCodecContext](http://blog.csdn.net/leixiaohua1020/article/details/14214859)
> 
[FFMPEG结构体分析：AVIOContext](http://blog.csdn.net/leixiaohua1020/article/details/14215369)
> 
[FFMPEG结构体分析：AVCodec](http://blog.csdn.net/leixiaohua1020/article/details/14215833)
> 
[FFMPEG结构体分析：AVStream](http://blog.csdn.net/leixiaohua1020/article/details/14215821)
[FFMPEG结构体分析：AVPacket](http://blog.csdn.net/leixiaohua1020/article/details/14215755)
# 3. ffmpeg库的使用：音频播放器
## 3.1 最简单的音频播放器
学习文章《[最简单的基于FFMPEG+SDL的音频播放器](http://blog.csdn.net/leixiaohua1020/article/details/10528443)》 中的代码，和最简单的视频播放器一样，这是最简单的音频播放器，每一行代码都很重要。
注：如果想要查看解码后的数据（PCM数据），需要用到Audition。
# **4. ffmpeg库的使用：一个真正的播放器——ffplay**
## 4.1 真正的播放器
ffplay流程图如文章《[FFplay源代码分析：整体流程图](http://blog.csdn.net/leixiaohua1020/article/details/11980843)》 所示。ffplay代码比较复杂，但是其核心代码和《[100行代码实现最简单的基于FFMPEG+SDL的视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/8652605)》 是一样的。可以两个工程结合着学习。
ffplay代码简介资料：[如何用FFmpeg编写一个简单播放器](http://download.csdn.net/detail/leixiaohua1020/6373783)
ffplay使用说明：[ffplay的快捷键以及选项](http://blog.csdn.net/leixiaohua1020/article/details/15186441)
ffplay已经移植到VC下的工程：[ffplay_vc2005](http://download.csdn.net/detail/sxcong/4672795)（别人做的，质量很不错）
ffplay移植到MFC下的工程，包含了简单的图形界面和一些控制按钮：[ffplay播放器移植VC的工程：ffplay for MFC](http://blog.csdn.net/leixiaohua1020/article/details/28685327)
# **5. ffmpeg库的使用：编码**
## 5.1 编码
ffmpeg编码我自己研究的不是很多，可以参考文章 ：[使用FFmpeg类库实现YUV视频序列编码为视频](http://blog.csdn.net/leixiaohua1020/article/details/11885429)
上面那篇文章是用的类库比较旧，新版类库的的使用可以参考下面几篇文章。
图像的编码可以参考：[最简单的基于FFMPEG的图像编码器（YUV编码为JPEG）](http://blog.csdn.net/leixiaohua1020/article/details/25346147)
音频的编码可以参考：[最简单的基于FFMPEG的音频编码器（PCM编码为AAC）](http://blog.csdn.net/leixiaohua1020/article/details/25430449)
视频的编码可以参考：[最简单的基于FFMPEG的视频编码器（YUV编码为H.264）](http://blog.csdn.net/leixiaohua1020/article/details/25430425)
## 5.2 转码
转码实际上是先解码然后编码。
不进行转码，只进行封装格式转换的程序可参考：[最简单的基于FFMPEG的封装格式转换器（无编解码）](http://blog.csdn.net/leixiaohua1020/article/details/25422685)
转码程序可参考：[最简单的基于FFMPEG的转码程序](http://blog.csdn.net/leixiaohua1020/article/details/26838535)
比较复杂的转码程序可以参考ffmpeg.c，它移植到MFC下的工程：[ffmpeg转码器移植VC的工程：ffmpeg for MFC](http://blog.csdn.net/leixiaohua1020/article/details/28744935)
# 6. ffmpeg源代码分析
通晓了ffmpeg库的使用以后，可以看一下ffmpeg的源代码。注意ffmpeg的源代码只有在linux下才能编译，在windows下可以使用MinGW进行编译。推荐使用Eclipse查看ffmpeg的源代码。
有一个很完整的ffmpeg源代码的分析文档：[ffdoc](http://download.csdn.net/detail/leixiaohua1020/6377803)
ffmpeg源代码分析文章列表如下。
库函数分析：
[图解FFMPEG打开媒体的函数avformat_open_input](http://blog.csdn.net/leixiaohua1020/article/details/8661601)
> 
[ffmpeg
 源代码简单分析 ： av_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677129)
> 
[ffmpeg
 源代码简单分析 ： avcodec_register_all()](http://blog.csdn.net/leixiaohua1020/article/details/12677265)
> 
[ffmpeg
 源代码简单分析 ： av_read_frame()](http://blog.csdn.net/leixiaohua1020/article/details/12678577)
[ffmpeg 源代码简单分析 ： avcodec_decode_video2()](http://blog.csdn.net/leixiaohua1020/article/details/12679719)
[FFMPEG源码分析：avformat_open_input()（媒体打开函数）](http://blog.csdn.net/leixiaohua1020/article/details/11885813)
ffmpeg.exe源代码分析：
[ffmpeg源码分析：transcode_init()函数](http://blog.csdn.net/leixiaohua1020/article/details/11851579)
[ffmpeg源码分析：transcode()函数](http://blog.csdn.net/leixiaohua1020/article/details/11851533)
# 7. ffmpeg相关工程的学习
学习完成ffmpeg，还可以了解一下基于ffmpeg的相关的多媒体开源工程，在这里推荐以下几个：
## **7.1 ffdshow**
ffdshow是基于ffmpeg的解码器类库libavcodec的DirectShow Filter。广泛安装在PC上。
![](https://img-blog.csdn.net/20140616103037171)
有关ffdshow的源代码分析文章（更新中）：
[ffdshow 源代码分析1 ： 整体结构](http://blog.csdn.net/leixiaohua1020/article/details/12013619)
[ffdshow 源代码分析 2： 位图覆盖滤镜（对话框部分Dialog）](http://blog.csdn.net/leixiaohua1020/article/details/12981725)
> 
[ffdshow
 源代码分析 3： 位图覆盖滤镜（设置部分Settings）](http://blog.csdn.net/leixiaohua1020/article/details/13004051)
[ffdshow 源代码分析 4： 位图覆盖滤镜（滤镜部分Filter）](http://blog.csdn.net/leixiaohua1020/article/details/13006213)
[ffdshow 源代码分析 5： 位图覆盖滤镜（总结）](http://blog.csdn.net/leixiaohua1020/article/details/13660583)
[ffdshow 源代码分析 6： 对解码器的dll的封装（libavcodec）](http://blog.csdn.net/leixiaohua1020/article/details/15493329)
[ffdshow 源代码分析 7： libavcodec视频解码器类（TvideoCodecLibavcodec）](http://blog.csdn.net/leixiaohua1020/article/details/15493521)
[ffdshow 源代码分析 8： 视频解码器类（TvideoCodecDec）](http://blog.csdn.net/leixiaohua1020/article/details/15493743)
[ffdshow 源代码分析 9： 编解码器有关类的总结](http://blog.csdn.net/leixiaohua1020/article/details/15493961)
## **7.2 LAV filters**
LAV Filter是基于ffmpeg的解码器类库libavcodec，以及解封装器类库libavformat的DirectShow Filter。广泛安装在PC上。
有关LAV Filter的源代码分析文章：
[LAV Filter 源代码分析 1： 总体结构](http://blog.csdn.net/leixiaohua1020/article/details/12711379)
[LAV Filter 源代码分析 2： LAV Splitter](http://blog.csdn.net/leixiaohua1020/article/details/12711723)
[LAV Filter 源代码分析 3： LAV Video （1）](http://blog.csdn.net/leixiaohua1020/article/details/13022201)
[LAV Filter 源代码分析 4： LAV Video （2）](http://blog.csdn.net/leixiaohua1020/article/details/13272409)
## **7.3 Mplayer**
Mplayer是Linux下使用最广泛的播放器，也有Windows版本的。其中使用了ffmpeg。
![](https://img-blog.csdn.net/20140616103059000)
有关Mplayer的源代码分析文章：
[MPlayer源代码分析](http://blog.csdn.net/leixiaohua1020/article/details/11885509)
## **7.4 Media Player Classic - HC**
现在广为使用很多播放器都是构建于Media Player Classic - HC的基础之上的。
![](https://img-blog.csdn.net/20140616103124140)
有关Media Player Classic - HC的源代码分析文章：
[Media Player Classic - HC 源代码分析 1：整体结构](http://blog.csdn.net/leixiaohua1020/article/details/13280659)
[Media Player Classic - HC 源代码分析 2：核心类 （CMainFrame）（1）](http://blog.csdn.net/leixiaohua1020/article/details/13290345)
[Media Player Classic - HC 源代码分析 3：核心类 （CMainFrame）（2）](http://blog.csdn.net/leixiaohua1020/article/details/13297291)
[Media Player Classic - HC 源代码分析 4：核心类 （CMainFrame）（3）](http://blog.csdn.net/leixiaohua1020/article/details/13298397)
[Media Player Classic - HC 源代码分析 5：关于对话框 （CAboutDlg）](http://blog.csdn.net/leixiaohua1020/article/details/13297555)
[Media Player Classic - HC 源代码分析 6：MediaInfo选项卡 （CPPageFileMediaInfo）](http://blog.csdn.net/leixiaohua1020/article/details/13297589)
[Media Player Classic - HC 源代码分析 7：详细信息选项卡（CPPageFileInfoDetails）](http://blog.csdn.net/leixiaohua1020/article/details/13297621)
## **7.5 XBMC**
XBMC是一个优秀的自由和开源的（GPL）媒体中心软件。
![](https://img-blog.csdn.net/20140616105517937)
有关XBMC源代码分析文章：
[XBMC源代码分析 1：整体结构以及编译方法](http://blog.csdn.net/leixiaohua1020/article/details/17454977)
[XBMC源代码分析 2：Addons（皮肤Skin）](http://blog.csdn.net/leixiaohua1020/article/details/17511993)
[XBMC源代码分析 3：核心部分（core）-综述](http://blog.csdn.net/leixiaohua1020/article/details/17512743)
[XBMC源代码分析 4：视频播放器（dvdplayer）-解码器（以ffmpeg为例）](http://blog.csdn.net/leixiaohua1020/article/details/17512509)
[XBMC源代码简析 5：视频播放器（dvdplayer）-解复用器（以ffmpeg为例）](http://blog.csdn.net/leixiaohua1020/article/details/17512633)
[XBMC源代码分析 6：视频播放器（dvdplayer）-文件头（以ffmpeg为例）](http://blog.csdn.net/leixiaohua1020/article/details/17512653)
[XBMC源代码分析 7：视频播放器（dvdplayer）-输入流（以libRTMP为例）](http://blog.csdn.net/leixiaohua1020/article/details/17512667)
# 8.FFmpeg其它方面的功能
在这里介绍一下FFmpeg中AVFilter的使用。AVFilter可以给试饮品添加各种处理效果。有一个简单的例子，是给视频添加水印：
[最简单的基于FFmpeg的AVfilter例子（水印叠加）](http://blog.csdn.net/leixiaohua1020/article/details/29368911)
