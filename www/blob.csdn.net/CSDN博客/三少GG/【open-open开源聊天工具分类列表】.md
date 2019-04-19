# 【open-open开源聊天工具分类列表】 - 三少GG - CSDN博客
2011年10月02日 21:30:13[三少GG](https://me.csdn.net/scut1135)阅读数：2210标签：[聊天																[工具																[google																[javascript																[xmpp																[facebook](https://so.csdn.net/so/search/s.do?q=facebook&t=blog)](https://so.csdn.net/so/search/s.do?q=xmpp&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=聊天&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
[http://www.open-open.com/52.htm](http://www.open-open.com/52.htm)
【Jitsi官方网站】[http://jitsi.org.sixxs.org/](http://jitsi.org.sixxs.org/)[nightly
 builds](http://dict.youdao.com/search?q=nightly+builds&le=eng&keyfrom=dict.phrase.wordgroup) 每日构建 ; 当日最新版 ; 每夜构建 ; 每日编译版
 视频通话和聊天工具Jitsi 
Jitsi是一个开源的，安全（ZRTP加密），高质量的SIP/XMPP视频通话、会议、聊天、桌面共享、文件传传输。可以安装在你喜欢的操作系统中并且支持多种IM网络。Jitsi是当最功能最完整的高级通信工具。Jitsi让你可以在同一个软件中连到Facebook、GoogleTalk、XMPP、Windows Live、Yahoo!、AIM和ICQ进行通信。
![Jitsi.jpg](http://www.open-open.com/projectimage/Jitsi.jpg)
![Jitsi1.jpg](http://www.open-open.com/projectimage/Jitsi2.jpg)
收录时间：2011-05-13 08:43:55
[更多视频通话和聊天工具Jitsi信息](http://www.open-open.com/open324452.htm)
 企业即时通讯系统 - ExtremeTalk 
ExtremeTalk（XT）是一个用于企业或者组织内部进行在线通讯的，基于XMPP(Jabber)协议的即时通讯系统。您可以将XT部署在您的内部局域网内，并且通过XT的管理控制台建立企业组织架构、添加企业内部联系人来给企业内部人员分配IM帐号。分配到帐号的人员可以使用支持XMPP协议的客户端进行单人或多人的在线通讯，传送文件等。
![企业即时通讯系统 - ExtremeTalk](http://www.open-open.com/pimage/ExtremeTalk.jpg)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[探微·智在未来](http://howe.im.sixxs.org/) 
 个人推荐不错的博客： [http://howe.im.sixxs.org/2011/07](http://howe.im.sixxs.org/2011/07)[http://howe.im.sixxs.org/](http://howe.im.sixxs.org/)
## [Google+技术架构介绍](http://howe.im.sixxs.org/%E7%BD%91%E7%BB%9C/google%E6%8A%80%E6%9C%AF%E6%9E%B6%E6%9E%84%E4%BB%8B%E7%BB%8D.html)
谷歌的社交网络Google Plus主要基于Java和JavaScript语言，而其视频会议框架Hangouts则采用了C/S方式。
期待已久的Google社交网络Google Plus或者Google+终于公之于众了，它采用邀请的形式向部分用户开放。该项目开始于2010年初，在其底层技术鲜为人知的情形下，秘密完成了开发。Joseph
 Smarr，Google+技术主管、Plaxo的前CTO，在AnyAsk的访谈中非针对性地透露了一些网络的[技术细节](http://anyasq.com.sixxs.org/79-im-a-technical-lead-on-the-google+-team)。我们联系了Smarr试图获取有关Google+架构的更多细节，但被拒绝了，他承诺更多信息会在不久的将来公布的。
按照Smarr的说法，Google+的服务端使用了Java语言和Guice，而客户端基于开源的跨浏览器框架Closure，采用了大量的JavaScript。Closure是一组JavaScript工具，方便开发人员编写富Web应用程序。Google的Gmail、Maps和Docs均使用了该框架。有趣的是Google并没有采用Wave和AdWords中使用的GWT，GWT主要面向Java开发者而非JavaScript开发者的。为了“确保即使在AJAX应用中，URL也很美观（对老版本浏览器，回退Hash-Fragment）“，添加了HTML5的历史API，另外，Closure模板也常常在服务端呈现，从而做到“在JavaScript代码加载前显示页面，然后JavaScript找到正确的DOM节点并挂接事件处理器等，使其响应“。Smarr进一步解释到：
Closure模板最酷的事情是它既可以编译成Java也可以编译成JavaScript。所以，我们使用Java服务端把模板转换成HTML，但是也可以在客户端使用JavaScript动态地显示。例如，如果你直接输入详细资料页面的URL，我们将在服务端呈现它，但是如果你进入页面流中，然后导航到某人的详细资料页面，我们将使用同一模板在客户端使用AJAX技术来显示。
后端采用的是BigTable和Colossus，它是Google的[实时搜索](http://highscalability.com.sixxs.org/blog/2010/9/11/googles-colossus-makes-search-real-time-by-dumping-mapreduce.html)所使用的文件系统。
Smarr承认Google+团队借鉴了Google和其他公司的很多东西，他们是“站在巨人的肩膀上，充分学习了我们以前的和业界其他社交产品“，这里指的应该就是Facebook和Twitter，而且本产品在很多方面看上去和Facebook很像。
[Google+ Hangouts](http://juberti.blogspot.com.sixxs.org/2011/06/announcing-google-hangouts.html)是在线云视频会议平台，基于XMPP、Jingle、RTP、ICE、STUN、SRTP等，它与采用P2P网络的Skype方法完全不同。Hangouts是C/S及云技术，主要依赖于Google的基础设施。目前还没有数据表明Hangouts会消耗多少资源，但估计应该会很多。所有这些都是为了提供低迟延（小于100ms）的流畅的群组沟通。
Hangouts要求与Google Talk相同的插件，不过，工程师们正在努力迁移到WebRTC上。WebRTC是视频通信的JavaScript框架，由Google、Mozilla和Opera提供支持的开源产品。当WebRTC集成到Chrome后，在Firefox、Opera或Chrome中运行Hangouts时就无需下载插件了。到时，Hangouts将使用集成在WebRTC中的VP8编码，而不是现在使用的H.264。
[Christian Oestlien](https://plus.google.com.sixxs.org/105923173045049725307)，Google+的项目经理，透露Google为[企业用户提供了特别的特性](https://plus.google.com.sixxs.org/105923173045049725307/posts/E3mVj6nskaX)，如分析及和其他企业账户的集成等，但他没有透露详细信息。他让企业先不要创建Google+账户，暂且等待功能开放，否则也会被删除的。
**查看英文原文**：Google+ Technological Details

