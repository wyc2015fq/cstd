# 关于国家标准AVS的相关学习资料 - 三少GG - CSDN博客
2011年12月29日 13:43:01[三少GG](https://me.csdn.net/scut1135)阅读数：2060
1。 
**[AVS（数字音视频编解码国家标准）开源社区今日开放](http://www.sarft.net/a/35495.aspx)**
　AVS开源社区首期对外开放的实时AVS编码、实时AVS转码和实时AVS解码三个项目，均可在常规计算平台上实时编解码，企业和用户可以直接用来开发AVS视听产品和部署AVS视听服务。
　　AVS 开源编码器 XAVS是基于GPL开源协议的AVS视频编码器，由原清华大学博士陈建文（现在美国加州大学洛杉矶校区）牵头、太原理工天成和长虹等企业的多名技术人员历时三年共同开发完成的。XAVS实现了AVS国家标准基准档次的编码功能，可以免费在任何电脑平台、任何组织机构和商业产品中使用，是目前AVS编码器所有相关产品中编码性能较高的一个。XAVS编码器支持指令级并行和多线程编码，具有较高的编码速度，能够支持AVS相关的实时视频应用。XAVS还支持多种的编码预置模式，可以满足各种应用对视频质量和编码速度的不同需求。XAVS编码器提供标准的API接口，可以便利地与其它AVS视频应用相结合。
　　AVS Directshow 解码器  (AVSDshow)是一个符合Windows Directshow框架的AVS解码解复用插件组。插件组包括AVS原始码流读取插件、TS流文件读取和解复用插件、AVS-P9文件格式封装的AVS文件解复用插件、AVS解码插件、RTSP接收插件。能支持对AVS原始码流、符合标准MPEG-2 TS封装的AVS流文件、符合AVS-P9标准封装的AVS文件的播放，以及支持RTSP协议下，符合AVS-P8标准封装的AVS RTP流的播放。任何基于或支持Directshow框架的播放器都能直接使用AVSDShow插件进行AVS码流的播放。开发人员也能在基于Directshow框架的应用程序中灵活使用或修改AVSDShow项目提供的插件，进行二次开发。AVSDShow由北京大学深圳研究生院王振宇牵头开发，在过去三年多时间内已经得到规模化应用验证。
　　AVS 开源转码器FFMPEG codec of AVS和AVSTranscoder由北京大学计算机研究所孙俊博士领衔开发完成。FFMPEG codec of AVS通过在业界广泛使用的FFMPEG开源项目基础上添加对AVS编解码的支持，支持将任意格式的视频格式转换到AVS视频，同时也支持将AVS视频转换为其它任意格式的视频。FFMPEG codec of AVS的解码器部分采用FFMPEG中的AVS解码器，编码器采用XAVS编码器，已经集成在高效成熟的FFMPEG的转码框架中，具有较高的转码性能和转码速度，可以将不同格式的海量视频转换为AVS的视频格式，具有较高的实用价值。AVSTranscoder是一套完整的AVS转码解决方案，文件的视频部分采用符合AVS标准的视频文件，音频部分采用mp3编码格式，并最终封装为AVS1-P9标准的asm格式媒体文件，可通过暴风影音直接播放，也可通过Darwin服务器实现流媒体的传输。
　　AVS编码、转码、解码三个开源项目对于推进AVS在互联网上的应用具有重要意义。
2.   AVS开放源代码社区（[http://opensource.avs.org.cn](http://opensource.avs.org.cn/)）
解码项目：
[http://sourceforge.net.sixxs.org/p/avsdshow/home/Home/](http://sourceforge.net.sixxs.org/p/avsdshow/home/Home/)
代码下载： git clone git://git.code.sf.net/p/avsdshow/code avsdshow-code
3.  联合信源公司  
[http://www.usc.com.cn/Product.aspx?id=105&sortid=13](http://www.usc.com.cn/Product.aspx?id=105&sortid=13)
