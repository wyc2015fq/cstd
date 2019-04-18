# Libjingle介绍 - 深之JohnChen的专栏 - CSDN博客

2016年10月11日 11:31:02[byxdaz](https://me.csdn.net/byxdaz)阅读数：6823


转载:http://www.cppblog.com/peakflys/archive/2013/01/30/197629.html

国内现在很多语音聊天工具都是基于TURN方式实现的，包括YY、AK等等，这种方式对于服务器的性能要求很高，而且在用户量增大的时候，服务器压力也会越来越大，用户的语音质量也会受到很大影响。而基于P2P方式实现的语聊服务器，就可以极大的避免这种情况的发生，而且用户的语音体验也会非常好。

   通过上文([P2P的原理和常见的实现方式(为libjingle开路)](http://www.cppblog.com/peakflys/archive/2013/01/25/197562.html))我们知道，因为NAT设备没有固定标准的原因，导致并不能100%的实现P2P，但是根据现在通用的ICE&STUN的方式，P2P的成功率可以达到90%多。前段时间在找使用这种方法实现的成熟库，最后猛然发现libjingle就在那里。

   通过一个多星期的研究，在此记录一下libjingle库的大致情况，如有不妥，希望朋友们可以留言或者邮件(peakflys@gmail.com)指正。

# Libjingle综述      Libjingle是一个方便实现P2P传输的开源库，由google公司开发，并与2005年12月15日发布第一个版本，可以粗略的看成是Jingle协议的C++实现库(peakflys注：只是和Jingle协议非常相似，并不完全兼容，区别以后介绍)，Google Talk即是基于这个库开发的。通过libjingle我们可以建立一个直通的网络连接(无视中间的NAT、防火墙、中继服务器和代理等)，无需特别关心Session建立的细节(加解密、格式等)，直接进行数据的交换。它也实现了一些辅助的功能，例如XML的解析和网络代理的处理。我们通过它可以实现如下的应用：

·一个多用户的语音聊天应用

·一个多用户的视频会议应用

·一个多用户的现场音乐、流媒体应用

·一个点到点的文件传输和共享的应用

目前库的版本是0.7.1(2012年10月2日发布)，支持Windows和UNIX/Linux，开源许可用的是Berkeley-style，这也就意味着，可以任意的修改和扩展它来更好的满足自己产品的需要。

库的SVN路径：http://libjingle.googlecode.com/svn/trunk/talk

值得注意的是：

①    Libjingle库本身的实现依赖于一些第三方库，例如:语音聊天依赖于Linphone或者GIPS，这取决于使用者的平台

②    Libjingle只是一个客户端的实现， relay Server协议和STUN协议（如果需要），还需要自己实现
实现relay Server后的网络结构(STUN使用网络上现有的，例如Google等公司提供的公有STUN服务器)：
![](http://www.cppblog.com/images/cppblog_com/peakflys/libjingle%E5%AE%9E%E7%8E%B0%E7%AE%80%E5%9B%BE.gif)

实现relay Server和STUN Server后的网络结构：
![](http://www.cppblog.com/images/cppblog_com/peakflys/libjingle%E5%AE%9E%E7%8E%B0%E5%85%A8%E5%9B%BE.gif)

STUN协议的实现不难，网上也有很多开源的代码实现，关键是需要一些资源的部署(需要两个公网IP)

P2P的关键实现在客户端，通过使用libjingle我们可以快速的构造一个健壮高效的P2P Client，如果仅仅是实现类似于Skype或者QQ那样的两人或者几人聊天，服务器方面实现就非常容易。但是如果要实现大用户参与的语音室，那么我们就得专注于服务器广播包的优化了。

Libjingle和各协议的关系

Jingle协议的发起方是Google，而libjingle库也是Google公司实现，ICE协议又基本包含在Jingle协议里，所以只需要知道libjingle和Jingle协议的区别即可。
历史：Libjingle大概和jingle XMPP扩展在同一时间被建立。Libjingle的团队建立了他们自己的协议去处理回话协商，后来和使用标准化的jingle（基于XMPP的标准）一起工作。尽管，jingle和libjingle是非常相似的，但是它们是不一样的，而且不能共同使用。现在libjingle的源码版本依然使用原始的网络协议，跟以前的稍微有些不同，而且无法兼容jingle的规范。不过它还是足够的接近jingle，所以学习jingle的说明书是值得的。类似的“接近但不是一样”，libjingle的视频内容描述（早期的jingle的视频内容描述格式XEP-0167），ICE的传输描述（早期的jingle的ICE传输XEP-0176），以及流的UDP描述（早期的jingle流UDP的传输描述XEP-0177）
·XMPP协议（核心协议）：
      全称：The Extensible Messaging and Presence Protocol，即可扩展通讯和表示协议。说白了，就是规定基于XML流传输指定节点数据的协议。这么做的好处就是统一(peakflys注：大家都按照这个定义，做的东西就可以相互通讯、交流，这个应该很有发展前景！)。它是一个开放并且可扩展的协议，包括Jingle协议
 都是XMPP协议的扩展。(peakflys注：使用Wireshark抓包时，早期的版本可能找不到这个协议，这时候可以选择Jabber，它是XMPP协议的前身)。现在很多的IM都是基于XMPP协议开发的，包括gtalk等。

·Jingle协议（重要的协议）：

Jingle协议是XMPP协议上的扩展协议，它着手解决在XMPP协议框架下的点对点的连接问题，也即P2P连接。在Jingle框架下，即使用户在防火墙或是NAT网络保护之下，也能够建立连接，从而提供文件传送、视频、音频服务等。纲领性文件是**XEP-0166**

·TURN协议：

全称：Traversal Using Relays around NAT，顾名思义，就是通过中继服务器来传输数据的协议。

·STUN协议：

全称：Simple Traversal of UDP over NATs，即NAT的UDP简单穿越，它允许位于NAT（或多重NAT）后的客户端找出自己的公网地址，查出自己位于哪种类型的NAT之后以及NAT为某一个本地端口所绑定的Internet端端口。知道NAT类型并且有了公网IP和port，P2P就方便多了。

·ICE协议：
全称：Interactive Connectivity Establishment，即交互式连接建立，说白了，它就是利用STUN和TURN等协议找到最适合的连接。
相关文档：

      RFC3921（下载： [RFC3921](http://www.cppblog.com/Files/peakflys/rfc3921.pdf)）                          XMPP协议的核心文档

      RFC3489(STUN)（下载：[RFC3489](http://www.cppblog.com/Files/peakflys/RFC3489.pdf)）             STUN协议的草案

      rfc5245(ICE)（下载：[RFC5245](http://www.cppblog.com/Files/peakflys/rfc5245.pdf)）                    ICE协议的草案

      xep-0166(Jingle)（下载：[XEP-0166](http://www.cppblog.com/Files/peakflys/xep0166.pdf)）            Jingle协议的官方主体文档

      xep-0176(Jingle ICE-UDP)（下载：[XEP-0176](http://www.cppblog.com/Files/peakflys/xep0176.pdf)）定义Jingle和ICE结合的官方文档（主要就是用XMPP作为ICE信道来重新描述ICE协议）

### 在Windows下编译最新版本的Libjingle

http://mysuperbaby.iteye.com/blog/908866

