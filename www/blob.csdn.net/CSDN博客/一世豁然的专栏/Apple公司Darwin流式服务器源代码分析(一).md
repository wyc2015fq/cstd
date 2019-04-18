# Apple公司Darwin流式服务器源代码分析(一) - 一世豁然的专栏 - CSDN博客





2016年10月25日 19:28:29[一世豁然](https://me.csdn.net/Explorer_day)阅读数：543








本博客转载于：[http://www.cublog.cn/u/11445/showart.php?id=225276](http://www.cublog.cn/u/11445/showart.php?id=225276)







当前，伴随着Internet的飞速发展，计算机网络已经进入到每一个普通人的家庭。在这个过程中，一个值得我们关注的现象是：Internet中存储和传输内容的构成已经发生了本质的改变，从传统的基于文本或少量图像的主页变为大容量、富信息量的流式媒体信息。一份早在1998年提交的研究报告就曾指出，流式媒体统治Internet的潮流是不可抗拒的，该报告估计到2003年，存储在网络服务器上的内容超过50%的将是流式媒体信息。但今天看来，这个估计还是有些保守了。
所谓的流式媒体简单的讲就是指人们通过网络实时的收看多媒体信息：如音频流、视频流等。与流式媒体对应的传统工作方式是下载+播放模式，即用户首先下载多媒体文件，然后再在本地播放，这种方法的一个主要缺点是启动延迟较大，例如一个30分钟长的MPEG-I文件（相当于VCD质量），即使使用1.5Mbps的速率下载，也需要半个小时才能完成，这样一个漫长的等待时间实在是无法忍受。在窄带网络环境中，几乎所有基于Internet的流式媒体产品都有着类似的工作原理：首先需要开发高效的压缩编码技术，并通过一套完整有效的传输体系将其发布到用户的桌面上。目前在流式媒体领域，有三种占有主导地位的产品，它们分别是Apple公司的Quick
 Time、Microsoft公司的Media Server以及Real公司的Real System。本文将介绍QuickTime技术及其开放源代码的Darwin流化服务器。

1     QuickTime技术介绍

Apple公司近日发布了QuickTime 5及QuickTime Streaming Server 3（简称QTSS）。作为客户端的QuickTime
 5是用于在Internet上对高质量音频和视频内容进行创建、播放及提供数字流的软件，目前QuickTime在全世界的使用量已经超过1亿5千万份。QuickTime
 Streaming Server 3是Apple基于标准的、开放式源代码的流式服务器软件的新版本，它包括以下新功能：跳读保护（Skip Protection），一项获得专利的特性组合，它可以保证Internet上数字流的质量，防止中断；全新的易于使用、基于Web的界面，用户可以在本地或远程进行管理，实现服务器配置。作为Internet流媒体联盟(ISMA)的创建者之一，Apple不断致力于开⒎?弦到绫曜嫉牟?泛图际酰?ü?岣呋ゲ僮餍岳从呕?没У氖褂锰逖椋?壳癚uickTime已被国际标准组织(ISO)选为MPEG-4的基本文件格式，可预见Apple将有更多MPEG-4
产品和技术的推出。
QuickTime正迅速成为世界领先的跨平台多媒体技术，而且是迄今为止唯一的开放式源代码、基于标准的数字流解决方案。ZDNet在2000年9月对于三种流式媒体服务器的特征比较说明了QTSS不仅仅被技术开发者关注，而且可以通过简单的定制成为成熟强大的产品，评测结果可见表1。
表1  ZDNet对三类产品的评测结果
服务器模块    QTSS 2.01     Media Server 7       RealServer Basic 7
操作系统支持       Windows NT, 2000； FreeBSD； Linux； Mac OS；
 Solaris       Windows NT, 2000       Windows NT, 2000
并发流个数    2,000      2,000      25 free/3000 pro
现场直播和广播    Yes  Yes  Yes
在线广告支持      Yes  Yes  Yes

PPV/流加密   No / No   Yes / Yes Yes / Yes
分配流能力    No   Yes  Yes

SMIL标准支持     Yes  No   Yes

RTSP标准支持     Yes        No   Yes
多播支持       Yes  Yes  Yes
状态报告       Yes  Yes  Yes
服务器日志    Yes  Yes  Yes
防火墙和代理支持       Yes  Yes  Yes
远程监控       Yes  Yes  Yes
客户可以使用QuickTime Player或其他支持QuickTime的应用程序在Windows或Macintosh平台上接收视频流，而且QuickTime
 Player可以从苹果公司的网站上下载免费使用。如果安装了QuickTime的插件，客户还可以直接通过浏览器收看。
客户希望点播一个节目时，QuickTime Player或插件将向QTSS发送请求，指明要点播的节目名。如果该节目存在，QTSS将向客户发送相应的视频流。当客户希望收看现场直播（或实时广播）时，它首先从QTSS获得关于当前频道的编码格式、地址等相关信息，然后再接受该频道的媒体流。
对于那些希望在Internet上实时流化视频或音频信息的用户，QTSS服务器将是一个很好的选择，通过它可实现多项任务，例如：
创建一个24小时在线的Internet广播电台；
现场实况转播：如公司会议、体育节目等；
创建远程学习站点：如能够点播视频和演讲；
图1是一个利用QTSS服务器建立的现场直播场景。

2     Darwin流化服务器介绍

Darwin Streaming Server（简称DSS）是QuickTime Streaming Server开放式源代码的版本，同时支持FreeBSD、Linux、Solaris、Windows
 NT和Windows 2000等多个操作系统，是当前所有同类产品中支持平台最多的一个。DSS的源代码和相关文档可从以下站点获得：http://www.apple.com

DSS源代码完全采用标准C++语言写成，编程风格非常优秀，每个C++类都对应着一对和类同名的.h/.cpp文件。但是由于大量采用了面向对象的概念，如继承、多态等等；而且源文件和类相当多，所以不太容易讲清楚。因此，读者最好事先把代码完整的过滤一两遍，再配合本文，就能看得更清楚点。
整个服务器包括多个子系统，分别存放在独立的工程内，如图2所示。

其中，最为重要的是基础功能类库（CommonUtilitiesLib）和流化服务器（StreamingServer）两个工程，前者是整个系统的通用代码工具箱，包括了线程管理、数据结构、网络和文本分析等多个功能模块。DSS和其他相关的工具使用基础功能类库工程中定义的功能类实现以下三个目标：
（1）抽象出系统中相同或类似的功能，用于降低代码的冗余度；
（2）封装基本功能，简化高层编码的复杂度；
（3）隔离开操作系统平台相关的代码。
而流化服务器工程中包含了DSS对多个国际标准的实现，是整个服务器的主工程。在本文中，我们将重点分析这两个工程中的核心代码和模块。另外，我们还将简单介绍利用DSS提供的开发接口（Module）扩展和定制服务器的方法。
DSS实现了四种IETF制定的国际标准，分别是：实时流传输协议RTSP（Real-time
 Streaming Protocol, RFC 2326）、实时传输协议（RTP Real-time Transfer Protocol，RFC 1889）、实时传输控制协议RTCP（Real-time
 Transport Control Protocol，RFC 1889）、会话描述协议SDP（Session Description Protocol，RFC
 2327）。这四个标准是开发所有流式媒体产品都必须掌握的，因此在对相关代码进行分析和二次开发之前，希望读者了解上述四种协议的基本思想，上述协议样本可从以下网站获得：[http://www.ietf.org](http://www.ietf.org/)




