# HTTP 与 HTTPS 的区别 - 算法与数学之美 - CSDN博客
2018年09月29日 22:03:42[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：23
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJQF9fQQnEOaEwibzPd1EHTYEqiazELlvMBFlFQDfDiaiceaM9Z6Bn7sF3uw/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJiaJibxyb1cPaf1YJfq6rE9meiaOON48qkDFO25NO4sSytorHQSTficYdNg/640?wx_fmt=png)
基本概念
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJL38kcpv4aaDov8jiakaEMosF8zicSJKTgqZ4zq5I3AFYqokJF2ib1mWwA/640?wx_fmt=png)
HTTP（HyperText Transfer Protocol：超文本传输协议）是一种用于分布式、协作式和超媒体信息系统的应用层协议。 简单来说就是一种发布和接收 HTML 页面的方法，被用于在 Web 浏览器和网站服务器之间传递信息。
HTTP 默认工作在 TCP 协议 80 端口，用户访问网站 http:// 打头的都是标准 HTTP 服务。
HTTP 协议以明文方式发送内容，不提供任何方式的数据加密，如果攻击者截取了Web浏览器和网站服务器之间的传输报文，就可以直接读懂其中的信息，因此，HTTP协议不适合传输一些敏感信息，比如：信用卡号、密码等支付信息。
HTTPS（Hypertext Transfer Protocol Secure：超文本传输安全协议）是一种透过计算机网络进行安全通信的传输协议。HTTPS 经由 HTTP 进行通信，但利用 SSL/TLS 来加密数据包。HTTPS 开发的主要目的，是提供对网站服务器的身份认证，保护交换数据的隐私与完整性。
HTTPS 默认工作在 TCP 协议443端口，它的工作流程一般如以下方式：
- 
1、TCP 三次同步握手
- 
2、客户端验证服务器数字证书
- 
3、DH 算法协商对称加密算法的密钥、hash 算法的密钥
- 
4、SSL 安全加密隧道协商完成
- 
5、网页以加密的方式传输，用协商的对称加密算法和密钥加密，保证数据机密性；用协商的hash算法进行数据完整性保护，保证数据不被篡改。
截至 2018 年 6 月，Alexa 排名前 100 万的网站中有 34.6% 使用 HTTPS 作为默认值，互联网 141387 个最受欢迎网站的 43.1% 具有安全实施的 HTTPS，以及 45% 的页面加载（透过Firefox纪录）使用HTTPS。2017 年3 月，中国注册域名总数的 0.11％使用 HTTPS。
根据 Mozilla 统计，自 2017 年 1 月以来，超过一半的网站流量被加密。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJiaJibxyb1cPaf1YJfq6rE9meiaOON48qkDFO25NO4sSytorHQSTficYdNg/640?wx_fmt=png)
HTTP 与 HTTPS 区别
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJL38kcpv4aaDov8jiakaEMosF8zicSJKTgqZ4zq5I3AFYqokJF2ib1mWwA/640?wx_fmt=png)- 
HTTP 明文传输，数据都是未加密的，安全性较差，HTTPS（SSL+HTTP） 数据传输过程是加密的，安全性较好。
- 
使用 HTTPS 协议需要到 CA（Certificate Authority，数字证书认证机构） 申请证书，一般免费证书较少，因而需要一定费用。证书颁发机构如：Symantec、Comodo、GoDaddy 和 GlobalSign 等。
- 
HTTP 页面响应速度比 HTTPS 快，主要是因为 HTTP 使用 TCP 三次握手建立连接，客户端和服务器需要交换 3 个包，而 HTTPS除了 TCP 的三个包，还要加上 ssl 握手需要的 9 个包，所以一共是 12 个包。
- 
http 和 https 使用的是完全不同的连接方式，用的端口也不一样，前者是 80，后者是 443。
- 
HTTPS 其实就是建构在 SSL/TLS 之上的 HTTP 协议，所以，要比较 HTTPS 比 HTTP 要更耗费服务器资源。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJiaJibxyb1cPaf1YJfq6rE9meiaOON48qkDFO25NO4sSytorHQSTficYdNg/640?wx_fmt=png)
TCP 三次握手
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJL38kcpv4aaDov8jiakaEMosF8zicSJKTgqZ4zq5I3AFYqokJF2ib1mWwA/640?wx_fmt=png)
在TCP/IP协议中，TCP协议通过三次握手建立一个可靠的连接。▼
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJumHvt6yaHb1oABlpcxuG2OKekjMrs0wG3eDRJekfNeibRL4H2ia8CTqQ/640?wx_fmt=jpeg)
- 
第一次握手：客户端尝试连接服务器，向服务器发送 syn 包（同步序列编号Synchronize Sequence Numbers），syn=j，客户端进入 SYN_SEND 状态等待服务器确认
- 
第二次握手：服务器接收客户端syn包并确认（ack=j+1），同时向客户端发送一个 SYN包（syn=k），即 SYN+ACK 包，此时服务器进入 SYN_RECV 状态
- 
第三次握手：第三次握手：客户端收到服务器的SYN+ACK包，向服务器发送确认包ACK(ack=k+1），此包发送完毕，客户端和服务器进入ESTABLISHED状态，完成三次握手
简化：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJXzATUYU9iahzU2NePpsxvicLRKnXXE9Tl0evS2AicLgKWV0tedABaA08g/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJiaJibxyb1cPaf1YJfq6rE9meiaOON48qkDFO25NO4sSytorHQSTficYdNg/640?wx_fmt=png)
HTTPS 的工作原理
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJL38kcpv4aaDov8jiakaEMosF8zicSJKTgqZ4zq5I3AFYqokJF2ib1mWwA/640?wx_fmt=png)
我们都知道 HTTPS 能够加密信息，以免敏感信息被第三方获取，所以很多银行网站或电子邮箱等等安全级别较高的服务都会采用 HTTPS 协议。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkywZlKfZriaSlicLbaqwKU6HJgkCGwkankb3GR4Fia5z4cicDM4LqMB2t5mgZQNnvZkTsPgn4Yyxg0mDQ/640?wx_fmt=jpeg)
01
客户端发起 HTTPS 请求
这个没什么好说的，就是用户在浏览器里输入一个 https 网址，然后连接到 server 的 443 端口。
02
服务端的配置
采用 HTTPS 协议的服务器必须要有一套数字证书，可以自己制作，也可以向组织申请，区别就是自己颁发的证书需要客户端验证通过，才可以继续访问，而使用受信任的公司申请的证书则不会弹出提示页面(startssl 就是个不错的选择，有 1 年的免费服务)。
这套证书其实就是一对公钥和私钥，如果对公钥和私钥不太理解，可以想象成一把钥匙和一个锁头，只是全世界只有你一个人有这把钥匙，你可以把锁头给别人，别人可以用这个锁把重要的东西锁起来，然后发给你，因为只有你一个人有这把钥匙，所以只有你才能看到被这把锁锁起来的东西。
03
传送证书
这个证书其实就是公钥，只是包含了很多信息，如证书的颁发机构，过期时间等等。
04
客户端解析证书
这部分工作是有客户端的TLS来完成的，首先会验证公钥是否有效，比如颁发机构，过期时间等等，如果发现异常，则会弹出一个警告框，提示证书存在问题。
如果证书没有问题，那么就生成一个随机值，然后用证书对该随机值进行加密，就好像上面说的，把随机值用锁头锁起来，这样除非有钥匙，不然看不到被锁住的内容。
05
传送加密信息
这部分传送的是用证书加密后的随机值，目的就是让服务端得到这个随机值，以后客户端和服务端的通信就可以通过这个随机值来进行加密解密了。
06
服务端解密信息
服务端用私钥解密后，得到了客户端传过来的随机值(私钥)，然后把内容通过该值进行对称加密，所谓对称加密就是，将信息和私钥通过某种算法混合在一起，这样除非知道私钥，不然无法获取内容，而正好客户端和服务端都知道这个私钥，所以只要加密算法够彪悍，私钥够复杂，数据就够安全。
07
传输加密后的信息
这部分信息是服务段用私钥加密后的信息，可以在客户端被还原。
08
客户端解密信息
客户端用之前生成的私钥解密服务段传过来的信息，于是获取了解密后的内容，整个过程第三方即使监听到了数据，也束手无策。
END
∑编辑 | Gemini
来源| 国际教育
**更多精彩：**
☞  [哈尔莫斯：怎样做数学研究](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=1&sn=35143b89b06fe4f5273f210b2d6a7c91&chksm=8b7e3290bc09bb86f7bb3f158d993df3f019a7e9ce3bc8897e164e35a2ebe5a4e0bdcc111089&scene=21#wechat_redirect)
☞  [扎克伯格2017年哈佛大学毕业演讲](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=2&sn=c75293463823e4d6769638e54b64f3ec&chksm=8b7e3290bc09bb86dc1e3f8e78d0b6de8811d75f3dcb092766fcb8ba0bab1cd9ba1ddfcef3b9&scene=21#wechat_redirect)
☞  [线性代数在组合数学中的应用](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554141&idx=1&sn=74a74c4e4d08eba0dd734528aa0b08e7&chksm=8b7e32c6bc09bbd073b34c22004ac6e4d99c8a0caa64c7d3dbaa8fd55e6ef1fc87ed545b8b7e&scene=21#wechat_redirect)
☞  [你见过真的菲利普曲线吗？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554105&idx=1&sn=224ab0d38fb57facea70081385360d58&chksm=8b7e3222bc09bb34d3b6df665087e64b233778ed427598d08e809f96261e898c1c0de6188bbc&scene=21#wechat_redirect)
☞  [支持向量机(SVM)的故事是这样子的](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554096&idx=1&sn=46783e6ace661a3ccbd8a6e00fb17bf9&chksm=8b7e322bbc09bb3d73dc240f2280bddf2ef8b7824a459a24bd7f6eeadd60edb96e690d467f6e&scene=21#wechat_redirect)
☞  [深度神经网络中的数学，对你来说会不会太难？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554077&idx=2&sn=3ecd63f2205fd59df8c360c97c943ef6&chksm=8b7e3206bc09bb10a36b09547efe0c54f41423b180622c1fdc7f14747ccc8f8fecee3a12e2cd&scene=21#wechat_redirect)
☞  [编程需要知道多少数学知识？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554062&idx=1&sn=17f0a88d5e15d1adfc29c690a0b1b89b&chksm=8b7e3215bc09bb038c6caa59d0f49cedd929f9be1104beea3411186cf4c81de69efc71a17883&scene=21#wechat_redirect)
☞  [陈省身——什么是几何学](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553994&idx=2&sn=74f67a1a3ac5c705f51f2ba619b717f6&chksm=8b7e3251bc09bb47dce73319948780081efe0333ffae99ea04a9eeabbcfcb38a29b4b73fb7c1&scene=21#wechat_redirect)
☞  [简述多种降维算法](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652562048&idx=2&sn=10cadb9dca53ccc3d86151e1dd44cf66&chksm=8b7e13dbbc099acd46b916ed2b564cbfeb8a3a5b1e6fef5f29e276d305bd37b7734f1c8817ac&scene=21#wechat_redirect)
☞  [曲面论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553747&idx=1&sn=e25f866d510cf2338b6d9e1b32bafb62&chksm=8b7e3348bc09ba5ea1caaf2a7bfcd80a7e7559b1983e473eda2206e56df7f38ef3cecf2f77c7&scene=21#wechat_redirect)
☞  [自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553811&idx=1&sn=000305074471c3d4c681c9cfd4e4bc93&chksm=8b7e3308bc09ba1e3043f5568a3a75a045285a1de97e4da36918bac68e7c6d579ad5d8cc25ab&scene=21#wechat_redirect)
☞  [如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553670&idx=1&sn=ea75a448c016f7229e4cb298f6017614&chksm=8b7e309dbc09b98bc622acdf1223c7c2f743609d0a577dd43c9e9d98ab4da4314be7c1002bd5&scene=21#wechat_redirect)
☞  [华裔天才数学家陶哲轩自述](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553646&idx=2&sn=bbf8f1be1ca1c66ad3f3270babea6885&chksm=8b7e30f5bc09b9e3e1a4fa735412e2fcb20df9e78f2f346bf578018ceab77de6326095d1bf71&scene=21#wechat_redirect)
☞  [代数，分析，几何与拓扑，现代数学的三大方法论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553596&idx=1&sn=bc5064e871831f862db6d19c3de6327e&chksm=8b7e3027bc09b93194fa09b25e2df400421c062927bb9120912875f8aaf0bb25553fc8f51e3b&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
