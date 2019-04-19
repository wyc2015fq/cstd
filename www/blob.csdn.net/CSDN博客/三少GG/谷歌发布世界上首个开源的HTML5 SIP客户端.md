# 谷歌发布世界上首个开源的HTML5 SIP客户端 - 三少GG - CSDN博客
2012年09月10日 18:54:06[三少GG](https://me.csdn.net/scut1135)阅读数：3114
# 谷歌发布世界上首个开源的HTML5 SIP客户端
发表于05-21 14:06| 28074次阅读| 来源CSDN| 25 条评论|
 作者夏梦竹
[浏览器](http://www.csdn.net/article/tag/%E6%B5%8F%E8%A7%88%E5%99%A8)[电子商务](http://www.csdn.net/article/tag/%E7%94%B5%E5%AD%90%E5%95%86%E5%8A%A1)[html5](http://www.csdn.net/article/tag/html5)[谷歌](http://www.csdn.net/article/tag/%E8%B0%B7%E6%AD%8C)[sip](http://www.csdn.net/article/tag/sip)
**摘要：**HTML5 SIP客户端是一款开源的，完全利用JavaScript编写的集社交(FaceBook，Twitter，Google+)，在线游，电子商务……。无扩展，无插件或是必备的网关。
HTML5 SIP客户端是一款开源的，完全利用JavaScript编写的集社交(FaceBook，Twitter，Google+)，在线游戏，电子商务等应用于一体。无扩展，无插件或是必备的网关，视频堆栈技术依赖于WebRTC。如同主页里的Demo视频演示，你可以轻松实现Chrome和IOS/Android移动设备之间的实时视频/音频通话。
![](http://articles.csdn.net/uploads/allimg/120521/141502F07-0.png)
该客户端是一项在浏览器中可被用来连接任意SIP或者IMS网络进行拨打和接收音频/视频通话及即时信息技术。该协议解析器(SIP，SDP...)通过使用Ragel查找表进行了高度优化，很适合硬件（内存和运算能力）受限的嵌入式系统使用。
SDP是会话描述协议的缩写，是描述流媒体初始化参数的格式，由IETF作为RFC 4566颁布。流媒体是指在传输过程中看到或听到的内容。
SIP是一项类似于HTTP的基于文本的协议。比较有意义的是，SIP可以减少应用特别是高级应用的开发时间。而且，由于HTTP和SIP之间存在相似之处，已有许多人准备采用SIP来生成应用，包括Web设计师。
**新特性包括：**
- 支持Audio / Video通话功能；
- 支持即时信息；
- Presence；
- 呼叫保持/恢复；
- 显示呼叫转移；
- 支持多个账号；
- 双音多频信号（DTMF）使用SIIP INFO
**入门指南：**
HTML5 SIP可运行在任意一款Web浏览器中，支持WebRTC，但我们强烈建议使用[Google chrome Canary 20.0.1127.0](https://tools.google.com/dlpage/chromesxs)或者最新版本进行测试。
- [Enabling WebRTC on Chrome](http://code.google.com/p/sipml5/wiki/Enable_WebRTC_On_Chrome)
- [Live demo](http://www.sipml5.org/)
- [Calling SIP rich clients running on iOS, Android, OS X or Windows](http://code.google.com/p/sipml5/wiki/Calling_SIP_clients)
- [Frequently asked questions](http://code.google.com/p/sipml5/wiki/FAQ)
- Non-exhaustive list of [Public SIP Servers](http://code.google.com/p/sipml5/wiki/Public_SIP_Servers) known to work with sipML5
**本文为CSDN编译整理，未经允许不得转载。如需转载请联系market@csdn.net。**
英文出自：[code.google.com](http://code.google.com/p/sipml5/)
