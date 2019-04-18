# DNS协议 - 算法与数学之美 - CSDN博客
2017年10月25日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：170
> 
**9527 (DNS协议)**
**作者：Vamei **
**出处：http://www.cnblogs.com/vamei**
在周星驰的电影《唐伯虎点秋香》中，周星驰饰演的主角一进入华府，就被强制增加了一个代号9527。从此，华府的人开始称呼主角为9527，而不是他的姓名。
![640?wx_fmt=png](https://mmbiz.qlogo.cn/mmbiz_png/951TjTgiabkxphyybsY12BwOI0j5JsWs3dcFrUyqzNmdy5KT2ZgGWBhuCLIUTQS8Av1gKibhAM79SNibvp8bmRnUg/640?wx_fmt=png)
域名(domain name)是IP地址的代号。域名通常是由字符构成的。对于人类来说，字符构成的域名，比如www.yahoo.com，要比纯粹数字构成的IP地址(106.10.170.118)容易记忆。域名解析系统(DNS, domain name system)就负责将域名翻译为对应的IP地址。在DNS的帮助下，我们可以在浏览器的地址栏输入域名，而不是IP地址。这大大减轻了互联网用户的记忆负担。另一方面，处于维护和运营的原因，一些网站可能会变更IP地址。这些网站可以更改DNS中的对应关系，从而保持域名不变，而IP地址更新。由于大部分用户记录的都是域名，这样就可以降低IP变更带来的影响。
从机器和技术的角度上来说，域名并不是必须的。但Internet是由机器和用户共同构成的。鉴于DNS对用户的巨大帮助，DNS已经被当作TCP/IP套装不可或缺的一个组成部分。
### DNS服务器
域名和IP地址的对应关系存储在DNS服务器(DNS server)中。所谓的DNS服务器，是指在网络中进行域名解析的一些服务器(计算机)。这些服务器都有自己的IP地址，并使用DNS协议(DNS protocol)进行通信。DNS协议主要基于UDP，是应用层协议(这也是我们见到的第一个应用层协议)。
![640?wx_fmt=png](https://mmbiz.qlogo.cn/mmbiz_png/951TjTgiabkxphyybsY12BwOI0j5JsWs3V2YynWPLL7ic4vvufDoqBibplXQyibR5HsL8zc877sfAUs2P3whPdYcEA/640?wx_fmt=png)
DNS服务器构成一个分级(hierarchical)的树状体系。上图中，每个节点(node)为一个DNS服务器，每个节点都有自己的IP地址。树的顶端为用户电脑出口处的DNS服务器。在Linux下，可以使用cat
 /etc/resolv.conf，在Windows下，可以使用`ipconfig /all，来`查询出口DNS服务器。树的末端是真正的域名/IP对应关系记录。一次DNS查询就是从树的顶端节点出发，最终找到相应末端记录的过程。
中间节点根据域名的构成，将DNS查询引导向下一级的服务器。比如说一个域名cs.berkeley.edu，DNS解析会将域名分割为cs, berkeley, edu，然后按照相反的顺序查询(edu, berkeley, cs)。出口DNS首先根据edu，将查询指向下一层的edu节点。然后edu节点根据berkeley，将查询指向下一层的berkeley节点。这台berkeley服务器上存储有cs.berkeley.edu的IP地址。所以，中间节点不断重新定向，并将我们引导到正确的记录。
在整个DNS查询过程中，无论是重新定向还是最终取得对应关系，都是用户计算机和DNS服务器使用DNS协议通信。用户计算机根据DNS服务器的反馈，依次与下一层的DNS服务器建立通信。用户计算机经过递归查询，最终和末端节点通信，并获得IP地址。
![640?wx_fmt=png](https://mmbiz.qlogo.cn/mmbiz_png/951TjTgiabkxphyybsY12BwOI0j5JsWs3MpYobjnHUFibva8DribGd5UO4RROuSZDLQfJaUbp2dibTzGMLFMQG5jwA/640?wx_fmt=png)
来自wikipedia
### 缓存
用户计算机的操作系统中的域名解析模块(DNS Resolver)负责域名解析的相关工作。任何一个应用程序(邮件，浏览器)都可以通过调用该模块来进行域名解析。
并不是每次域名解析都要完整的经历解析过程。DNS Resolver通常有DNS缓存(cache)，用来记录最近使用和查询的域名/IP关系。在进行DNS查询之前，计算机会先查询cache中是否有相关记录。这样，重复使用的域名就不用总要经过整个递归查询过程。
![640?wx_fmt=png](https://mmbiz.qlogo.cn/mmbiz_png/951TjTgiabkxphyybsY12BwOI0j5JsWs33zgGwIXOjBwRPqiaGtHPQIgUrYSebl0d2Ao199YF8SEMV1OeCrrrpiaA/640?wx_fmt=png)
来自wikipedia
### 反向DNS
上面的DNS查询均为正向DNS查询：已经知道域名，想要查询对应IP。而反向DNS(reverse DNS)是已经知道IP的前提下，想要查询域名。反向DNS也是采用分层查询方式，对于一个IP地址(比如106.10.170.118)，依次查询in-addr.arpa节点(如果是IPv6，则为ip6.arpa节点)，106节点，10节点，170节点，并在该节点获得106.10.170.118对应的域名。
### 实例
下面一个视频来自youtube，我觉得它很生动的解释了DNS的工作过程。
请访问：
http://v.youku.com/v_show/id_XNTAwNDY2NjYw.html
很棒的视频，静静享受吧。
未完待续！
☞  [哈尔莫斯：怎样做数学研究](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=1&sn=35143b89b06fe4f5273f210b2d6a7c91&chksm=8b7e3290bc09bb86f7bb3f158d993df3f019a7e9ce3bc8897e164e35a2ebe5a4e0bdcc111089&scene=21#wechat_redirect)
☞  [扎克伯格2017年哈佛大学毕业演讲](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=2&sn=c75293463823e4d6769638e54b64f3ec&chksm=8b7e3290bc09bb86dc1e3f8e78d0b6de8811d75f3dcb092766fcb8ba0bab1cd9ba1ddfcef3b9&scene=21#wechat_redirect)
☞  [线性代数在组合数学中的应用](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554141&idx=1&sn=74a74c4e4d08eba0dd734528aa0b08e7&chksm=8b7e32c6bc09bbd073b34c22004ac6e4d99c8a0caa64c7d3dbaa8fd55e6ef1fc87ed545b8b7e&scene=21#wechat_redirect)
☞  [你见过真的菲利普曲线吗？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554105&idx=1&sn=224ab0d38fb57facea70081385360d58&chksm=8b7e3222bc09bb34d3b6df665087e64b233778ed427598d08e809f96261e898c1c0de6188bbc&scene=21#wechat_redirect)
☞  [支持向量机(SVM)的故事是这样子的](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554096&idx=1&sn=46783e6ace661a3ccbd8a6e00fb17bf9&chksm=8b7e322bbc09bb3d73dc240f2280bddf2ef8b7824a459a24bd7f6eeadd60edb96e690d467f6e&scene=21#wechat_redirect)
☞  [深度神经网络中的数学，对你来说会不会太难？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554077&idx=2&sn=3ecd63f2205fd59df8c360c97c943ef6&chksm=8b7e3206bc09bb10a36b09547efe0c54f41423b180622c1fdc7f14747ccc8f8fecee3a12e2cd&scene=21#wechat_redirect)
☞  [编程需要知道多少数学知识？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554062&idx=1&sn=17f0a88d5e15d1adfc29c690a0b1b89b&chksm=8b7e3215bc09bb038c6caa59d0f49cedd929f9be1104beea3411186cf4c81de69efc71a17883&scene=21#wechat_redirect)
☞  [陈省身——什么是几何学](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553994&idx=2&sn=74f67a1a3ac5c705f51f2ba619b717f6&chksm=8b7e3251bc09bb47dce73319948780081efe0333ffae99ea04a9eeabbcfcb38a29b4b73fb7c1&scene=21#wechat_redirect)
☞  [模式识别研究的回顾与展望](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553949&idx=2&sn=d171680964df774397efd9db81c00347&chksm=8b7e3386bc09ba90bf0f6e1cabf82ba86ff94630cb5ee2e0f14ff9455db52be32ddbc289d237&scene=21#wechat_redirect)
☞  [曲面论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553747&idx=1&sn=e25f866d510cf2338b6d9e1b32bafb62&chksm=8b7e3348bc09ba5ea1caaf2a7bfcd80a7e7559b1983e473eda2206e56df7f38ef3cecf2f77c7&scene=21#wechat_redirect)
☞  [自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553811&idx=1&sn=000305074471c3d4c681c9cfd4e4bc93&chksm=8b7e3308bc09ba1e3043f5568a3a75a045285a1de97e4da36918bac68e7c6d579ad5d8cc25ab&scene=21#wechat_redirect)
☞  [如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553670&idx=1&sn=ea75a448c016f7229e4cb298f6017614&chksm=8b7e309dbc09b98bc622acdf1223c7c2f743609d0a577dd43c9e9d98ab4da4314be7c1002bd5&scene=21#wechat_redirect)
☞  [华裔天才数学家陶哲轩自述](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553646&idx=2&sn=bbf8f1be1ca1c66ad3f3270babea6885&chksm=8b7e30f5bc09b9e3e1a4fa735412e2fcb20df9e78f2f346bf578018ceab77de6326095d1bf71&scene=21#wechat_redirect)
☞  [代数，分析，几何与拓扑，现代数学的三大方法论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553596&idx=1&sn=bc5064e871831f862db6d19c3de6327e&chksm=8b7e3027bc09b93194fa09b25e2df400421c062927bb9120912875f8aaf0bb25553fc8f51e3b&scene=21#wechat_redirect)
![0?wx_fmt=gif](https://mmbiz.qlogo.cn/mmbiz_gif/951TjTgiabkwJ4BpvBcQhGAbtWZZvV69s7GickZGibsKgYkTQkiaZfLYOmGS9iaaoibadibGJhT18OVZkfeJmCSUSD0zw/0?wx_fmt=gif)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域。
稿件一经采用，我们将奉上稿酬。
投稿邮箱：math_alg@163.com
![0?wx_fmt=gif](https://mmbiz.qlogo.cn/mmbiz_gif/951TjTgiabkxN5SJPzhu6icTXrIpMZqSdFzG0y6ib1c9enWGK3GxfHTRIN7ich2kzqepNvMHfktp4Ir88ibolsDBuhQ/0?wx_fmt=gif)
