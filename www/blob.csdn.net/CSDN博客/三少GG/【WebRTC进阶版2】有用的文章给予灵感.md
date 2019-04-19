# 【WebRTC进阶版2】有用的文章给予灵感 - 三少GG - CSDN博客
2011年10月27日 10:55:01[三少GG](https://me.csdn.net/scut1135)阅读数：4916标签：[服务器																[防火墙																[http服务器																[binding																[traversal																[终端](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=traversal&t=blog)](https://so.csdn.net/so/search/s.do?q=binding&t=blog)](https://so.csdn.net/so/search/s.do?q=http服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=防火墙&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
个人分类：[Android 多媒体](https://blog.csdn.net/scut1135/article/category/790993)
## 1.评论文章
## [WebRTC and the future of web-based communications](http://blog.zingaya.com/2011/07/28/webrtc-and-the-future-of-web-based-communications/)
[Technology](http://blog.zingaya.com/category/technology/) 28 July 2011[0
 Comments](http://blog.zingaya.com/2011/07/28/webrtc-and-the-future-of-web-based-communications/#respond)
Few weeks ago Google released WebRTC, a new open project that provides Internet users with the ability tocommunicate in real-time via voice and video by simply using a Real-Time Communications **(RTC) compatible**browser.We think it’s great for further evolution of the Internetand web technologies and, of course, we will work hard to add WebRTC
 support for our platform. There are still a lot of questions regarding browsers that will have WebRTC support and how long it will take to add this capabilities, but we are ready to use innovative technologies to improve user experience. We also continue using
 Adobe Flash, because now it’s the only option for service like Zingaya and we will need to support fallback to Flash if browser doesn’t have new capabilities anyway.But the future
 of web-based communications seems to be really bright.
**2. 开发指南**
[https://bugzilla.mozilla.org/show_bug.cgi?id=688187](https://bugzilla.mozilla.org/show_bug.cgi?id=688187)
**3.问题解决**
**1）**
[http://egamesir.blog.163.com/blog/static/18809608820115138433037/](http://egamesir.blog.163.com/blog/static/18809608820115138433037/)
2011.6.22 今天看webrtc的代码，发现里面使用的Google的stun服务器ping不通，网上找了一个可用的 stun.xetn.com
 ，在此mark一下，回去测试。
2）
## [公开的免费STUN服务器](http://blog.sina.com.cn/s/blog_683d26990100oucy.html)
当SIP终端在使用私有IP地址时，可能需要配置stun服务器。
公开的免费STUN服务器有：
stunserver.org 测试是OK的
校内使用**stunserver.org.sixxs.org **测试ok
关于stunserver的文章：
1.   **[一个方便的STUN客户端](http://mysuperbaby.iteye.com/blog/901230)**
STUN协议简单的理解是位于NAT或者防火墙后面的STUN客户端首先访问具有公网IP地址的STUN服务器,这样STUN服务器就会知道STUN客户端被NAT之后的公网IP地址和端口,然后STUN服务器会把这个信息返回给STUN客户端. 
这个工具使用很方便直观,安装python后,然后运行stun_client_0.0.1.py,默认会连接public的STUN服务器 "stun.ekiga.net",输出如下: 
2. **[对于飞信文件p2p文件传输的疑惑](http://basiccoder.com.sixxs.org/tag/stun)**
昨天读了一下rfc，然后写程序测试了一下，后来苦于在国内找不到免费的STUN服务器程序只简单了完成了Binding Request的发送和Binding Response的最简单接收，各个属性的实现原理都无法通过程序来一一检测，后来就突然想到了飞信，飞信的p2p无疑也要进行上面的一系统操作来使两台主机之间建立起p2p连接，那飞信的STUN服务器？？？猜测始终不如亲眼证实，打开飞信的配置文件(就是那个xml)，果然在里面找到了stun-server-urls这一项，**它用stun-server用的是stunserver.org,这是一个国外的免费服务器，于是便解决了我一直以来对于飞信文件输过程的一个疑惑，当我在教育网私有网络内向公网发起文件传输请求时，不管我请求的方式是不是p2p，最终进行传输所用的始终都是中继的方式，HTTP中继是一种效率多低的传输方式啊，不考虑HTTP服务器的带宽和负载，单单是上行传输的带宽限制就可以把文件传办理的速率拉低。**所以我不明白的是为什么飞信不自己开发一个stun服务器，而是要用stunserver.org这个国外的免费服务器，这样对于教育网的用户来说是很无奈的。
3.[STUN解决的过程](http://blog.csdn.net/carry1314lele/article/details/2420494)
简单来说，STUN解决的过程如下：
1、   将STUN Client放在NAT后的终端中。
2、   该终端在发送OLC之前调用STUN  Client，去连接位于公网的任一台STUN Server.
3、   STUN Server与STUN Client进行STUN Messgae交互（STUN Server将对应的RTP/RTCP经NAT转换后的地址
告之STUN Client）。
4、   该终端在打开本地逻辑通道时，使用STUN Client已经发送过的地址作为RTP/RTCP地址。
。
。
。
++++++
**[关于SIP防火墙穿越的汇总](http://www.cstnet.net.cn/show_media.jsp?NewsID=128)**
**术语和基础知识**
防火墙
　　一个防火墙限制私人内网和公众因特网之间的通讯，典型地防火墙就是丢弃那些它认为未经许可的数据包。在数据包穿越一个防火墙时，它检查但是不修改包里的 IP地址和TCP/ UDP 端口信息。 
网络地址转换(NAT)
　　当数据包穿过NAT时，NAT不仅检查同时也修改数据的包头信息，并且允许更多的在NAT后的主机分享少数公网IP地址(通常只有1个)。
**Firewall的基本策略：**
    Firewall会判断所有的包是来自内部（Inside）还是外部(Outside)。 
    允许所有来自inside的包发出去。 
    允许来自Outside的包发进来，但这个连接必须是由Inside发起的。 
    禁止所有连接由Outside发起的包发进来。 
    firewall会允许几个信任的outside主机，他们可以发起建立连接，并发包进来。 
　　所有NAT和Firewall都是对于TCP/IP层以下进行处理和过滤的，而SIP应用的地址是在应用层。所以必须采用其他的途径来解决这一问题。 
**解决方案**
针对不同的NAT类型，可以有不同的解决方案。 
1．客户端解决方案
　　客户端解决方案主要包括：STUN(simpletraversalof UDP through NAT)、TURN（Traversal Using Relay NAT）、ICE（Interactive Connectivity Establishment）。
　　STUN是一个轻量级的协议，允许应用程序探测当前在它们与公网之间是否存在NAT、防火墙以及它们的类型，并且具备能够探测到NAT所分配的公网地址和端口的能力。STUN协议中定义了两个实体：STUNClient和STUNServer。STUNClient嵌入在终端系统的应用程序中，比如SIP UA，它向STUN Server发送请求；STUN Server接收请求并产生STUN响应，它是无状态的。SIP终端在建立呼叫之前，通过向处在公网上的STUN服务器发送STUN请求，得到信令和媒体流在NAT上的映射地址，并且将这些地址填写到SIP消息中的Via、Contact字段以及SDP中的媒体流传输地址，替代原有的私有地址。但是，STUN只能工作在全通NAT、地址限制NAT以及端口限制NAT的网络环境下，在对称性NAT的情况下，SIP
 UA通过STUN请求得到的映射地址是无效的。
　　TURN协议在语法和操作上均与STUN相似，其优点是提供了对对称性NAT的穿越。处在公网的TURN服务器为客户端提供本身的一个外部IP地址和端口，并且负责中转通信双方的媒体流。TURN协议虽然支持所有类型的NAT穿越，但是它需要中转通信双方的媒体流，使得媒体流在传输过程中增加了一跳，不可避免地增加了包的延迟和丢包的可能性，而且完全使用TURN方式需要大量的TURN服务器，在有大量用户时，TURN服务器会成为系统瓶颈，因此我们应该尽量避免使用这种方法。目前，TURN的使用越来越少。
　　ICE目前已经被推认为在非对称性NAT环境下首选的客户端解决方案。ICE本身是一种方法，它利用STUN,TURN等任何符合UNSAF的协议来提供一个通用的解决方案。
