# PJNATH介绍 -- 开源的用于NAT穿透的ICE, STUN和TURN - DoubleLi - 博客园






原文地址：[http://blog.pjsip.org/2007/04/06/introducing-pjnath-open-source-ice-stun-and-turn/](http://blog.pjsip.org/2007/04/06/introducing-pjnath-open-source-ice-stun-and-turn/)

**ICE是什么？**

对于那些不熟悉ICE的人而言，ICE可能是一种最全面的用于穿透NAT和多媒体通信的方法。它提供了一种找到两个终端之间最好路径的方法，它还解决了NAT所带来的各种各样的问题，比如两个终端处在同一个NAT后面（and no hairpin is available），两个终端都处在对称NAT后面（在这种情况下，将使用一个中继器）。详情请参看[draft-ietf-mmusic-ice-15.txt](http://www.ietf.org/internet-drafts/draft-ietf-mmusic-ice-15.txt)。

这是个了不起的协议！首先，它不靠自身来运行，ICE使用STUN（Session Traversal Utlities for NAT, 其最新的草案是[draft-ietf-behave-rfc3489bis-06](http://www.ietf.org/internet-drafts/draft-ietf-behave-rfc3489bis-06.txt)）协议来做连通性检测。对于转播，它使用TURN（Obtaining Relay Addresses from Simple Traversal Underneath NAT，其最新的草案是[draft-ietf-behave-turn-03](http://www.ietf.org/internet-drafts/draft-ietf-behave-turn-03.txt)）来做同样的检测。这三者加起来，总共有103+61+44=208页协议文档。我想这就为什么是"Simple"这个词被从STUN这个首字母缩略语中移除的原因。

总之，我相信现在是支持这个协议的好时机。ICE一周前通过了WGLC-ed (Work Group Last Call)，而且STUN草案也日趋完善（TURN稍微落后一点点）。实际上，我觉得时机是完美的，因为在它们通过RFC-ed (this is not to say that we’re expert in any kind!)之前，我们可以帮助发现这些文档中的漏洞。我们早点把这些协议实现出来并发现其中的问题，协议也可以及时做相应的改动。

**PJNATH – NAT Traversal Helper Library**

[PJNATH – Open Source NAT Traversal Helper supporting STUN, TURN, and ICE](http://www.pjsip.org/pjnath/docs/html/index.htm)（点击该链接可以转到相应的文档）

PJNATH是PJ项目中一个新的库，它与PJLIB，PJSIP，PJMEDIA这些库处于同一层次。它由如下部分组成：
- 一个通用的STUN库，该库支持[draft-ietf-behave-rfc3489bis-06](http://www.ietf.org/internet-drafts/draft-ietf-behave-rfc3489bis-06.txt),[draft-ietf-behave-turn-03](http://www.ietf.org/internet-drafts/draft-ietf-behave-turn-03.txt), and[draft-ietf-mmusic-ice-15.txt](http://www.ietf.org/internet-drafts/draft-ietf-mmusic-ice-15.txt)。
- 一个ICE实现，包括两部分：传输独立的ICE会话和ICE流传输（同一些套接字联系在一起的ICE）。

将来，我们可能把诸如UPnP或SOCKS这样的NAT穿透协议也加进来。

为了支持ICE，PJMEDIA和PJSUA这两个库需要升级：
- 在PJMEDIA中，我们添加了一个新的媒体传输，叫做pjmedia_ice_trans。
- 在PJSUA库中，STUN设置已经被从传输设置移动全局设置，并且在媒体设置中添加了使能ICE的设置选项。

**它可以工作了吗？**

我想，是的。

我试着在同一个NAT之后运行两个pjsua，本地地址对被使用。我试着在不同的NAT之后运行两个pjsua，公共地址对被使用。所以看起来它是可以工作了。

此外，从上周开始，我一直在做[测试](http://lib.csdn.net/base/softwaretest)，发现并解决了一些bugs。而且在测试过程中，我发现协商很快就完成了，对于不同的ADSL线上的两个终端，这个时间大约是100ms，即便是SDP应答被代理延迟（ICE在SDP应答被调用者收到之前就可以开始检查）。

但是这些天测试ICE最主要的困难之一是没有其它可以免费获取的ICE实现，所以虽然PJNATH中的实现看起来可以工作，并且它严格遵守ICE-15，但在它与其它的实现进行对话之前我们还是不能确定它的兼容性到底怎样。









