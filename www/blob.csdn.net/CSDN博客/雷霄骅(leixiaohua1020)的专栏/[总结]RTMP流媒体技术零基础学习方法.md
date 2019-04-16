# [总结]RTMP流媒体技术零基础学习方法 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月18日 00:10:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：120











本文主要总结一些我在学习RTMP流媒体技术过程中积累的经验。也为后来学习RTMP流媒体技术的人们一个参考。本文力图从简到难，循序渐进的介绍RTMP流媒体技术的方方面面，先从应用说起，逐步深化剖析相关工程的源代码。看完本文的话应该会对RTMP流媒体技术有比较完整的了解。

![](https://img-blog.csdn.net/20140616105029843)

# **0.背景知识**

本章主要介绍一下RTMP流媒体技术都用在了哪里（在这里仅列几个我所知的，其实远比这个多）。说白了就是为了说明：RTMP是非常重要的。

使用RTMP技术的流媒体系统有一个非常明显的特点：使用 Flash Player 作为播放器客户端，而Flash Player 现在已经安装在了全世界将近99%的PC上，因此一般情况下收看RTMP流媒体系统的视音频是不需要安装插件的。用户只需要打开网页，就可以直接收看流媒体，十分方便。

目前使用RTMP技术的电视台有如下几个：

> CNTV部分点播内容

中国教育电视台1直播（rtmp://pub1.guoshi.com/live/newcetv1）

中国教育电视台3直播（rtmp://pub1.guoshi.com/live/newcetv3）

香港电视台直播（rtmp://live.hkstv.hk.lxdns.com/live/hks）

北广传媒移动电视直播（rtmp://www.bj-mobiletv.com:8000/live/live1）

中石化网络电视台直播（rtmp://wowza.sinopectv.cn:1935/live/sinopec）



使用RTMP技术社交直播平台：

> 六间房
9158
新浪秀场



可以这样概括一下：凡是想实现“无插件直播”的流媒体系统，都会使用RTMP。

# **1.RTMP在Flash流媒体系统中的使用**

RTMP被Adobe开发出来，就是用于在其Flash平台的流媒体系统之间传递数据的。因此想要了解RTMP，需要首先了解Flash流媒体系统。

最简单的Flash流媒体系统由两部分组成：

客户端：**Flash Player**（一般只有这一种）



> 
注：支持RTMP的基于Flash的播放器除了可以自己开发之外，还有现成的，比如说[jwplayer](http://www.jwplayer.com/)，[flowplayer](http://flowplayer.org/)等等。




服务器端：

> 
**1.Flash Media Server（FMS）：**Adobe官方的流媒体服务器。毕竟是商业服务器，其性能应该是最强的，配置是最简单的，但是价格不菲，推荐作为入门学习使用。个人下载使用是不用花钱的。服务器端编程使用ActionScript语言。

官网：[http://www.adobe.com/cn/products/adobe-media-server-family.html](http://www.adobe.com/cn/products/adobe-media-server-family.html)

可参考教程：

> 
1.[FMS开发者向导](http://download.csdn.net/detail/leixiaohua1020/6563043)。这个教程讲的是FMS开发技术，是网友翻译的中文版，学习起来比较方便。这里有英文原版，内容更全：[Flash Media Server Delevoper's Guide](http://help.adobe.com/en_US/flashmediaserver/devguide/flashmediaserver_4.5_dev_guide.pdf)


> 
注：FMS开发技术方面使用到了ActionScript，可以先找相关教程做一个初步的了解。主要使用Flash CSx进行开发。


> 
2.[FMS技术指南](http://download.csdn.net/detail/leixiaohua1020/6563051)。这个教程主要是讲述FMS配置的，第2部分涉及到集群，负载均衡等内容，挺高端的。


**2.Red5：**开源流媒体服务器，基于Java的。曾经使用过，感觉也还算比较方便，如果商业应用，又买不起昂贵的FMS的话，使用也不错。服务器端编程使用Java语言。

官网：[http://www.red5.org/](http://www.red5.org/)

**3.还有其他服务器，但目前我还没接触过**






# **2.RTMPdump（libRTMP）的使用**

这一章就要开始学习RTMP协议的处理了。

可以使用RTMPdump（libRTMP）进行学习。这是一个C++的开源工程。主要作用是下载RTMP流媒体。首先可以了解一下其使用方法：[RTMPdump 使用说明](http://blog.csdn.net/leixiaohua1020/article/details/14229047)

官网：[http://rtmpdump.mplayerhq.hu/](http://rtmpdump.mplayerhq.hu/)



# **3.RTMP**dump（libRTMP）源代码分析****

这一章开始学习RTMPdump（libRTMP）的源代码。学习它之前需要有Socket编程的基础。

首先学习《[RTMP协议规范](http://download.csdn.net/detail/leixiaohua1020/6563059)》

中文翻译版：[http://download.csdn.net/detail/leixiaohua1020/6563059](http://download.csdn.net/detail/leixiaohua1020/6563059)

英文原版：[http://www.adobe.com/cn/devnet/rtmp.html](http://www.adobe.com/cn/devnet/rtmp.html)

我对其部分内容做的简单分析：[RTMP规范简单分析](http://blog.csdn.net/leixiaohua1020/article/details/11694129)

其次从协议规范中，总结出RTMP协议建立连接的流程：[RTMP流媒体播放过程](http://blog.csdn.net/leixiaohua1020/article/details/11704355)

然后，下载一个windows下可以编译通过的RTMPdump工程（这个工程是很难得！）：[rtmpdump vs2005版本](http://download.csdn.net/detail/leixiaohua1020/6563163)

（当然，linux下会方便很多，因为RTMPdump就是在linux下开发完成的）

最后，通过调试工程，就可以基本掌握RTMP协议了，我写了一些分析RTMPdump（libRTMP）源代码的文章：

[RTMPdump 源代码分析 1： main()函数](http://blog.csdn.net/leixiaohua1020/article/details/12952977)
[RTMPDump（libRTMP）源代码分析 2：解析RTMP地址——RTMP_ParseURL()](http://blog.csdn.net/leixiaohua1020/article/details/12953833)
[RTMPdump（libRTMP） 源代码分析 3： AMF编码](http://blog.csdn.net/leixiaohua1020/article/details/12954145)
[RTMPdump（libRTMP）源代码分析 4： 连接第一步——握手（Hand Shake）](http://blog.csdn.net/leixiaohua1020/article/details/12954329)
[RTMPdump（libRTMP） 源代码分析 5： 建立一个流媒体连接 （NetConnection部分）](http://blog.csdn.net/leixiaohua1020/article/details/12957291)
[RTMPdump（libRTMP） 源代码分析 6： 建立一个流媒体连接 （NetStream部分 1）](http://blog.csdn.net/leixiaohua1020/article/details/12957877)
[RTMPdump（libRTMP） 源代码分析 7： 建立一个流媒体连接 （NetStream部分 2）](http://blog.csdn.net/leixiaohua1020/article/details/12958617)
[RTMPdump（libRTMP） 源代码分析 8： 发送消息（Message）](http://blog.csdn.net/leixiaohua1020/article/details/12958747)
[RTMPdump（libRTMP） 源代码分析 9： 接收消息（Message）（接收视音频数据）](http://blog.csdn.net/leixiaohua1020/article/details/12971635)
[RTMPdump（libRTMP） 源代码分析 10： 处理各种消息（Message）](http://blog.csdn.net/leixiaohua1020/article/details/12972399)

# **4.相关应用**

libRTMP作用还是很大的，除了可以保存RTMP流媒体之外，还可以发送RTMP流媒体等等。它可以整合到FFMPEG中，下面几篇文章简述了它的应用：

[libRTMP使用说明](http://blog.csdn.net/leixiaohua1020/article/details/14229543)
[在FFMPEG中使用libRTMP的经验](http://blog.csdn.net/leixiaohua1020/article/details/14228879)

（以后更新其他方面......）







****




