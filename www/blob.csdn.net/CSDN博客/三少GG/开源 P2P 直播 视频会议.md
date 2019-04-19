# 开源 P2P 直播 视频会议 - 三少GG - CSDN博客
2012年11月25日 19:03:24[三少GG](https://me.csdn.net/scut1135)阅读数：8087
## [一个P2P点播直播开源项目：P2PCenter](http://blog.sina.com.cn/s/blog_4696f46b0100cn7j.html)
最近跟着公司的项目走，我也研究了不少东西，尤其是在P2P方面，广泛涉猎各种开源项目，尤其是国外的开源项目，意外的发现了一个国内的项目，做的还不错，推荐一下。
---------------------
使用平台：
客户端:Windows 平台
服务器:Windows & Linux
框架图例：
![一个P2P点播直播开源项目：P2PCenter](http://lh6.ggpht.com/_OQb1ey5AMX4/ScHpuhTWuwI/AAAAAAAACsE/RPt_B6z2gFQ/s800/frame.gif)
项目简介：
 P2P点播直播开源项目是一个由Kevin发起，多人参与开发并不断更新，符合GPL开源协议的开源项目。
项目成员： 系统架构及初期开发： 符文科(Kevin),马军(Mark), 张威, 高成, 王亮
  LINUX相关开发:  李桂生(QQ:674859634)
  JSP网站系统:  伍林(QQ:86409354)
  ASP网站系统:  徐小均(QQ:275992019)
  Bittorrent模块:  Lism(QQ:32437503[Johnny]),肖双全(3707878[Color_X])
目的：缩短企业同类型项目开发周期，集成多种P2P协议，如BT,EMULE等。
主要功能：
 Peer间传输数据采用UDP，能有效的穿透防火墙，在进一步的版本中将实现TCP及UPNP穿透
 能够实现流媒体的点播及直播，还能共享其他文件，文件类型不限。
 流媒体文件支持广泛，如rmvb,wmv,flv,mp4等
源代码下载论坛：[http://opensource.kan8888.com](http://opensource.kan8888.com/)
SVN下载地址：
 工程地址:svn://p2p.kan8888.com/P2PCenter
 用户名：mc_public
 密码：kan8888.com
sourceforge.net 下载地址：
website:http://p2pmedia.svn.sourceforge.net
SVN 地址：[https://p2pmedia.svn.sourceforge.net/svnroot/p2pmedia](https://p2pmedia.svn.sourceforge.net/svnroot/p2pmedia)
演示网站：[http://www.skyvcd.com](http://www.skyvcd.com/)
最新源代码打包下载：[http://opensource.kan8888.com/soft/P2PCenter_last.rar](http://opensource.kan8888.com/soft/P2PCenter_last.rar)
P2P点播直播开源项目 非常不错
[http://opensource.kan8888.com](http://opensource.kan8888.com/)
1. 视频会议系统 OpenMeetings
OpenMeetings是一个多语言可定制的视频会议和协作系统。它支持音频、视频，能让你查看每个与会者的桌面。OpenMeetings还包含一个白板，通过白板可以导入各种格式的图片和涂鸦。
在线演示：[http://www.openmeetings.de/](http://www.openmeetings.de/)
2. 网络视频会议软件 VMukti
Vmukti是一个网络视频会议软件.它是第一个开源的PBX和会议软件,它支持在家里或办公室进行声/视频交流，桌面共享等。它使用了VoIP技术。
3. 视频会议系统 Ekiga
Ekiga是一个兼容SIP和H.323的[视频会议](http://www.polomeeting.cn/)程序，兼容VoIP，IP电话，通过Ekiga可以与使用任何SIP和H.323软硬件的远程用户进行视频和音频对话。Ekiga的前身是GnomeMeeting.
4. 在线会议系统 DimDim
DimDim 是一个开源项目，基于 Web 的进行在线会议的系统。用户可以自由下载相关代码，用来搭建自己的[在线会议系统](http://www.polomeeting.cn/)平台。目前仅支持 Windows 的 IE 浏览器下的通讯，不过开发团队表示已经着手开发 Firefox 适用版本，平台上也会支持 Linux 和 OSX 用户。目前就测试情况来看相当理想，无论是速度还是语音质量，应用方面实际上也合适自己建站的
 blogger 们搭建属于自己的通讯平台，要求并不高。
5.开源会议软件 Kablink Conferencing
Kablink Conferencing 是一个开源的会议软件，团队可以通过它进行实时的通讯，与会者可以看到其他在线的人员并启动及时会议，会议可使用音频、聊天、白板、桌面共享和演示等功能。
    现在有很多人架设视频网站，要缓解视频网站服务器带宽的压力，最好的办法是使用p2p的方式。从头写一个p2p系统，要花费太多功夫，不如直接用开源的。本文介绍3款开源的p2p点播/直播系统。只是简单介绍，并给出一些有用信息，并不做源码分析。
    1.peercast
    这是比较古老的一个直播系统，性能上有不少缺陷，现在应该只有学习价值，没有应用价值了。下载地址：[http://download.csdn.net/down/614681/vah101](http://download.csdn.net/down/614681/vah101)
    peercast有个王浩聪注释版，他给源码添加了些注释，并写了些分析文档，对于学习应该挺有帮助。下载地址：[http://download.csdn.net/hicsdn/bbisonic](http://download.csdn.net/hicsdn/bbisonic)
    2.myseelite
    是一个直播（也支持轮播、点播）系统。是国内一家公司（mysee）倡导的开源项目，由于有公司背景，所以也是功能最强大的一个系统。直播源支持从摄像头直接获取，也可以是mms流。文档非常全，还有不少书籍介绍它（《P2P网络技术原理与C++开发案例》专门有一章讲它），学习起来很方便。我个人也非常推崇它。myseelite有myseelite1和myseelite2两个版本，2比1升级许多功能。最新的版本应该是myseelite_2007_06_28,下载地址：[http://sourceforge.net/projects/myseelite](http://sourceforge.net/projects/myseelite)
    3.p2pcenter
    是国内几个人写的一个开源项目，自称是点播直播系统，但我经过编译部署后，认为它没有直播功能。文档少得可怜，仅有的几个文档中，写得极其简单，甚至有错误（可能那些文档是为很早的版本写的，系统更新后，文档就没跟着更新），这样的文档只能起到误导的作用。该项目有个相应的论坛，里面也是只有人发问，没有人解答。系统发布点播频道的方式很怪异。我个人不推荐这个系统，不给出下载地址了，可以自己在baidu搜索。虽然存在非常多的缺陷，但它的开源精神还是值得称赞。
### [5个基于Web的开源即时聊天应用推荐](http://www.iteye.com/news/22712)
### 1.  Cryptocat – An Open Source Encrypted & Private Chat Application
![](http://dl.iteye.com/upload/attachment/549817/998f6809-934f-32fe-aa08-95e6dff13086.jpg)
**Cryptocat** 是一个开源基于Web的即时消息聊天应用程序。与其它即时聊天工具不同之处在于，它主要关注确保通信是进行加密过和私人化的。
**Cryptocat** 基于PHP开发，除了静态文件(CSS, JS和图片)外，只有单独的一个PHP文件。其界面简单，而且漂亮。消息使用[crypto.js library](http://code.google.com/p/crypto-js/)进行AES-256加密。
**项目地址: [https://crypto.cat/](https://crypto.cat/)**
### 2. Candy: A JavaScript Powered Multiuser Chat Client 
![](http://dl.iteye.com/upload/attachment/549819/82cc317b-e8e5-3852-8bba-5d362a9e1fcd.jpg)
**Candy**是一个开源的聊天客户端，基于JavaScript开发。支持任意Jabber服务器，包括自己搭建的服务器。但需要服务器开启HTTP-binding支持。
这个应用程序支持多个聊天室，与多个用户进行私聊，主持人甚至可以踢出和禁止用户。
它的界面支持多国语方，并能够通过插件进行扩展。
**Candy**提供良好的文档和完整的API，能够便捷地与其它应用程序或网站相集成。
**项目地址: [http://amiadogroup.github.com/candy/](http://amiadogroup.github.com/candy/)下载: [https://github.com/amiadogroup/candy](https://github.com/amiadogroup/candy)**
## 3. JsChat : An Open Source Chat System
![](http://dl.iteye.com/upload/attachment/549821/3517a1bc-62c5-3077-9bef-da2c4205bdf1.jpg)
Js Chat是一个开源的聊天应用程序，使用了一个基于JSON的简单协议。Js Chat采用Ruby (Sinatra)开发。 它拥有一个控制台客户端，外观类似于IRC客户端。它能够展示图片和YouTube视频。用户可以使用Twitter帐号登录并使用mangodb记录日志。利用Js Chat用户可以创建多个聊天室。它的协议设计成接近可执行的代码，所以利用它可以很方便地创建客户端。
**项目地址: [http://jschat.org/](http://jschat.org/)下载: [https://github.com/alexyoung/jschat](https://github.com/alexyoung/jschat)**
### 4. Big Blue Button: An Open Source Video Conferencing Chat App
![](http://dl.iteye.com/upload/attachment/549823/37be8f61-b063-32bd-863e-6fd6a2b53141.jpg)
BigBlueButton是一套开源的视频会议系统，特别适用于远程教育，也可以用于标准的会议。该系统可以让多个用户登录，共享他们的摄像头并同时能够通过VOIP进行交流。可以在线演示PDF和Office文档，用户还可以将他们的桌面共享给其他用户。用户可以举手发言，支持群聊和私聊。
**项目地址: [http://demo.bigbluebutton.org](http://demo.bigbluebutton.org/)**
### 5. iJab : Open Source Chat Ajax Im Application
### ![](http://dl.iteye.com/upload/attachment/549825/590575e7-ca59-3116-adbb-62a7bb4a1ecc.jpg)
### iJab是基于Ajax技术开发的、支持XMPP/Jabber协议的Web即时通讯客户端（XMPP Web IM/Web Chat）。iJab是开源软件，任何人都可以基于iJab建立支持XMPP协议的即时通讯系统。iJab使用Javascript和HTML开发，基于浏览器运行，客户端无需下载其他程序。
### iJab同时还开发了类似于Facebook聊天风格的Web即时通讯客户端--iJabBar。功能包括：
嵌入浏览器运行，无弹出窗口声音多人聊天用户搜索 Roster管理 HTTP-Binding还支持MSN、AIM等即时通讯协议
**项目地址:[http://opensource.ijab.im/](http://opensource.ijab.im/)**
