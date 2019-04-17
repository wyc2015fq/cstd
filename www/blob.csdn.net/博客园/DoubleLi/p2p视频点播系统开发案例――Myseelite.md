# p2p视频点播系统开发案例――Myseelite - DoubleLi - 博客园






项目地址：[http://sourceforge.net/projects/myseelite/](http://sourceforge.net/projects/myseelite/)



1. MySee公司倡导的开源项目，目前国内功能最强大的一个开源系统。

2. 一个直播（也支持轮播、点播）系统

3. 直播源支持从摄像头直接获取，也可以是mms流。

4. 文档齐全

5. 《P2P网络技术原理与C++开发案例》等书籍有提到该项目



MySee公司简介：

1. MySee依托全国强大的媒体合作伙伴以及自主GDBS的网络传输技术，具备传播影视节目资源的合法资质

2. MySee网络电视拥有上海文广，优度宽频，凤凰网，等主流媒体的优秀节目资源

3. MySee自主研发的GDBS播放系统，利用先进的P2P技术，完全超越了传统带宽+服务器的传播方式，在视频流播放速度上处于世界领先水平。

4. 领导团队全部具有清华大学，北京大学的强大技术背景；





Myseelite子系统定义：

1、Capture  其主要功能是获取直播流，在Myseelite中，capture可以从windows media encoder（WME)和windows media Server(WMS） Helix中获取节目流

2、Super Peer 其主要功能是接收capture传送的流，作为整个P2P系统的源节点和缓冲节点，为Client提供直播流

3、Tracker 其主要功能是为Client提供下载信息，接收Client的信息，返回Supper Peer地址和拥有此流的Client节点

4、Client 主要功能是从Supper和其他Client获取直播流，并进行播放。

系统结构图：

![](http://img.blog.csdn.net/20150525101444674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2xsenc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



工作流程：

Capture获取直播节目流，经打包后传到Supper Peer;

Supper Peer将此节目流发布到Web Server，并且向Tracker注册此节目流；

Client登录Web Server,获取myseelite协议，从myseelite协议串中获得tracker的地址及节目信息。

Client登录Tracker，Tracker根据节目信息，返回Client要求节目的Supper Peer地址的Client列表。

Client依次连接Supper Peer和拥有此节目流的Client，获得节目流。









