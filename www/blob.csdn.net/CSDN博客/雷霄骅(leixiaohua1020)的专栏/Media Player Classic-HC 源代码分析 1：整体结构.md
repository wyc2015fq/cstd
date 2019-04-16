# Media Player Classic - HC 源代码分析 1：整体结构 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月28日 00:26:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30









注：写了一系列分析Media Player Classic - HC 源代码的文章，在此列一个列表：

[Media Player Classic - HC 源代码分析 1：整体结构](http://blog.csdn.net/leixiaohua1020/article/details/13280659)
[Media Player Classic - HC 源代码分析 2：核心类 （CMainFrame）（1）](http://blog.csdn.net/leixiaohua1020/article/details/13290345)
[Media Player Classic - HC 源代码分析 3：核心类 （CMainFrame）（2）](http://blog.csdn.net/leixiaohua1020/article/details/13297291)
[Media Player Classic - HC 源代码分析 4：核心类 （CMainFrame）（3）](http://blog.csdn.net/leixiaohua1020/article/details/13298397)
[Media Player Classic - HC 源代码分析 5：关于对话框 （CAboutDlg）](http://blog.csdn.net/leixiaohua1020/article/details/13297555)
[Media Player Classic - HC 源代码分析 6：MediaInfo选项卡 （CPPageFileMediaInfo）](http://blog.csdn.net/leixiaohua1020/article/details/13297589)
[Media Player Classic - HC 源代码分析 7：详细信息选项卡（CPPageFileInfoDetails）](http://blog.csdn.net/leixiaohua1020/article/details/13297621)

![](https://img-blog.csdn.net/20140616104406062)


Media Player Classic - Home Cinema (mpc-hc)播放器一个经典的影音播放器，可播放CD、VCD、DVD、及MP3、MP4、AVI、AAC等多种影音格式。与此同时，它还是开源的。很多知名的视频播放器都是在它的基础上建立起来的，在这里就不例举了。本文将会对其源代码进行简要的分析。

之前一篇博客中曾经介绍了它的编译过程：[VC2010 编译 Media Player Classic - Home Cinema (mpc-hc)](http://blog.csdn.net/leixiaohua1020/article/details/11694711)

在这里就不再重复说明了，直入主题，看看它的工程是什么样子：



![](https://img-blog.csdn.net/20131028000023140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

相信大部分人看到这个工程的第一反应就是：好大啊！确实，我看到这个工程的时候也是这个反应。mpc-hc总体上分为3个部分：Apps，Filters，Libraries。其中Apps是其主程序；Filters是其附带的一些directshow filter，比如说AVI分离器，FLV分离器等等；Libraries则是其依赖的一些库：像Zlib这类的。

来细看看mpc-hc都有什么directshow filter吧（截图都放不下了...）







![](https://img-blog.csdn.net/20131028000615062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







Filters分为以下几种：

Muxer（封装），Parser（解封装，或称为分离器），Reader（读取），Renderer（显示），Source（源），Switcher（这个我不懂），Transform（处理）

在这里就不一一例举各种Filter了，因为数量实在太多，大部分Filter工程都可以通过名称来判断其功能。

再看看主程序Apps工程吧：







![](https://img-blog.csdn.net/20131028001204375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可见主程序包含了巨量的代码，截图也只能显示其中的一部分。因此在代码分析的时候，不可能做到面面俱到，只能选择其中的重点部分进行详细的分析。

mpc-hc的对话框数量也很惊人：







![](https://img-blog.csdn.net/20131028001430812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这里就不再花篇幅形容mpc-hc工程的巨大了。赶紧说说如何来研究分析它的代码。本文主要分析它的主程序即在Apps目录下的工程。先介绍一下我总结出来的一些规则：

1.以PPage开头的.cpp或.h文件通常是一些属性选项卡的对话框对应的类。随后会详细介绍一个“视频信息”选项卡的代码（在这里用到了开源库MediaInfo）

2.主框架所在的位置是Mainfrm.cpp

目前只有这两条规则，以后会随着研究的不断深入，进一步完善这些规则。




