# 押宝WebRTC：技术概要和对网络架构的冲击 - weixin_33985507的博客 - CSDN博客
2016年10月10日 15:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
# **WebRTC****技术概要**
WebRTC是一套标准的而技术，可以使用Web或者移动应用程序通过浏览器实现强大的实时语音、视频、数据和视频会议等服务（如图1所示）。
如同一个基于浏览器的多媒体电话，或软件电话，它不但可以与其他的浏览器共同使用，也可与其他通信系统使用，如PSTN和VoIP。以前，如果试图建立的实时解决方案，必须依赖于昂贵的专用硬件和定制软件，需要投入把巨大的基础设施。但由于WebRTC的出现，在浏览器平台定义必要的接口后，就可以提供各种实时通信解决方案。这都归功于速度更快，高速计算硬件的出现。
![2890026-e6190ddc494d1f84.png](https://upload-images.jianshu.io/upload_images/2890026-e6190ddc494d1f84.png)
在传统的客户端-服务器浏览器模型中，用户浏览器从Web服务器下载内容（HTML / JavaScript、CSS），然后执行并显示在浏览器中。WebRTC则在浏览器中的添加一个基于标准的实时通信功能接口（如图2所示），这个功能是一个应用程序，通过Web服务器提供实时通信服务，它可以用简单的JavaScript AP和html5对下载的内容进行控制。
![2890026-cc07662a77294232.png](https://upload-images.jianshu.io/upload_images/2890026-cc07662a77294232.png)
# **WebRTC****的商机**
通过浏览器访问互联网已经成为个人和商业生活中的一项基本活动。从平板电脑、智能手机到PC，超过10亿的设备已经支持浏览器，据最新统计，到2016年将有40亿台设备支持浏览器。这意味着WebRTC客户端设备已经广泛应用，而且经济实惠。这是移动运营商最大的商机，移动运营商可以利用这个机会，通过扩展现有的IMS服务发展更多新用户，为老用户提升现有服务和创新的服务。举个例子，一个VoLTE用户与另一个非VoLTE用户，通过运行WebRTC兼容的浏览器在PC上建立多媒体通信，对移动运营商来说，这将显著扩大客户群（如图3所示）。
![2890026-569059ecd387b009.png](https://upload-images.jianshu.io/upload_images/2890026-569059ecd387b009.png)
WebRTC的好处并不仅限于此，对于移动运营商来说，还包括以下机会：
1.通过IMS基础设施，使PLMN和PSTN语音用户成为WebRTC用户，不断融合传统网络和网络电话；
2.促进网络功能虚拟化基础设施（NFVI），打造自己或第三方的WebRTC增值服务的垂直领域，如教育和医疗；
3.增加企业的服务产品，如浏览器到IP-PBX、统一通信、联络中心、语音和视频会议；
4.拓展基于消费者的互通服务，如WebRTC用户间的短信和彩信服务。
5.提供自己的WebRTC服务或与第三方WebRTC互联网玩家合作。简而言之，WebRTC为移动网络运营商提供了诸多机会，同时利用他们现有的实力和专业资质、安全、政策、计费方式、漫游和切换可以创造很多商机。这项技术可以帮助企业的研发、丰富产品功能和抢占市场
# **WebRTC****的架构和协议栈**
为了便于理解，我们来看一个最基本的三角形WebRTC架构（图4）。在这个架构中，移动电话用“浏览器M”表示，笔记本电脑用“浏览器L”表示，通过Web服务器将它们连接起来。要建立一个实时媒体通讯，两台设备需要了解彼此的媒体功能，通过交换呼叫信令控制协议实现。诸如这样的信令协议在WebRTC标准中并非事先规定，而是由开发者自行制定。在浏览器RTC会话的步骤如下：
1.首先，两个浏览器都从Web服务器下载了WebRTC程序（HTML5/JavaScript）；
2.其次，两个浏览器通过Web服务器交换控制信令信息（使用嵌入式信令服务器），建立媒体功能功能互通。
3.最后，两个浏览器直接建立RTC媒体的音频、视频和数据通道。
![2890026-f13b3607cda7c543.png](https://upload-images.jianshu.io/upload_images/2890026-f13b3607cda7c543.png)
WebRTC使用P2P媒体流，音频、视频和数据的连接直接通过浏览器实现。但是，浏览器却隐藏在NAT（网络地址翻译）和防火墙的后面，这增加了建立P2P媒体会话的难度。这些流程和协议，如ICE或Trickle ICE，STUN和TURN，在建立P2P媒体流都是必不可少的。
![2890026-66bfbb650bd5964d.png](https://upload-images.jianshu.io/upload_images/2890026-66bfbb650bd5964d.png)
如何使用STUN协议建立一个P2P
RTC媒体（如图5所示），简化版的ICE流程如下：
1.两个浏览器通过自己的公网IP地址，使用STUN协议信息和STUN服务器建立联系；
2.两个浏览器通过SDP提供/应答机制，使用呼叫控制信令消息交换它们已发现的公共IP地址（ICE候选）；
3.两个浏览器执行连接检查（ICE冲孔），确保P2P可以连接；
4.建立连接后，RTC媒体会话和媒体交换就可以实现了。
5.但是，假如在一个高度限制的NAT或防火墙，这种直接的路径将无法建立，只能到达TURN服务器。结果是媒体通过TURN服务器分程传递（如图6所示）。
![2890026-eda79c80cbb2bbbd.png](https://upload-images.jianshu.io/upload_images/2890026-eda79c80cbb2bbbd.png)
由互联网工程任务组（IETF）基于标准的可互操作的通信模型和协议栈详细地定义了WebRTC技术（参见图7），如下：
![2890026-39455194e9077d0f.png](https://upload-images.jianshu.io/upload_images/2890026-39455194e9077d0f.png)
如前所述的信令栈，并非由WebRTC实现规定，而是由开发者自行决定。在这个例子中，我们将使用SIP-over-WebSocket（SIPoWS）作为信令栈。HTTP协议用于浏览器下载HTML5/JavaScript程序内容；
**NAT栈**解决P2P连接问题；
**媒体栈**用于发送和接收RTC的音频和视频。LETF标准规定G.711和Opus作为音频/视频解码器。视频解码器尚未授权，但是H.248和VP8已经获得授权。媒体栈也用于交换RTC数据。本例中，实时信息采用**消息会话中继协议（MSRP），**实时会议采用**二层控制协议（BFCP），**实时文本服务采用**T.140。**
# **WebRTC****对移动运营商网络架构的冲击**
如我们所见，WebRTC的三角形架构，使用WebRTC协议和栈是实现浏览器到浏览器的因特网通讯的最佳选择。相反，IMS架构使用不同的协议和栈。如果WebRTC用户要使用IMS，则需要做架构性的改变-增加一个WebRTC网关支持两个架构之间的交互（如图8所示）新的架构必须在WebRTC和IMS用户间搭建统一的认证框架，以及策略和计费控制功能（PCC），从而WebRTC用户可以通过LTE/EPC的QoS功能访问网络。
![2890026-8aa630e86c74e134.png](https://upload-images.jianshu.io/upload_images/2890026-8aa630e86c74e134.png)
WebRTC网关必须支持WebRTC-IMS交互的信令和媒体功能（如图9所示）。WebRTC网关必须支持的信令交互功能有：
1.信令协议翻译，如SIPoWS对SIP的翻译；
2.与IMS SDP扩展对应的WebRTC
SDP扩展规范，如RTP复用/解复用；
3.与ICE对象交换的WebRTC
UE;如通过STUN服务器返回公网IP地址；
4.根据协议好的功能配置媒体功能。
![2890026-25d0f688bf50d345.png](https://upload-images.jianshu.io/upload_images/2890026-25d0f688bf50d345.png)
支持媒体互通主要功能的WebRTC网关必须有以下特征：
1.ICE冲孔和针对WebRTC UE的消息持续连接，如STUN连接检查和持续连接；
2.媒体转码和复用/解复用功能，如G.711对应的Opus；
3.终止DTLS-SRTP信息及RTP参数在IMS中的应用。网络设备制造商（NEMs）现在要么在现有设备中加入WebRTC-IMS交互功能，或者生产新的WebRTC网关。现有的包含WebRTC-IMS交互功能的产品有会话边界控制器（SBC），媒体网关控制器（MGC）和媒体网关（MGW）。
# **WebRTC****端到端的流程**
截止目前，我们已经了解了WebRTC技术、移动运营商（MNO）的商机、架构和组成WebRTC网关的部件。当我们把这些片段连接起来就可以对WebRTC端到端流程有整体的把握，接下来的例子中，有几个关键点需要考虑：
1.WebRTC用户通过IMS呼叫VoLTE用户；
2.WebRTC用户同时与Web服务器和STUN服务器相连；
3.WebRTC网关支持WebRTC-IMS交互功能。
注：为简洁说明这个案例，不考虑LET/EPC接入网络连接、PCC流、NAT在IMS的漫游和统一认证流程。
图10展示端到端流的6大简易步骤。它们是：
![2890026-0a3f89f3218913bb.png](https://upload-images.jianshu.io/upload_images/2890026-0a3f89f3218913bb.png)
第一步，WebRTC用户从Web服务器下载Web应用程序内容到他/她自己的浏览器；
第二步，WebRTC用户通过WebRTC网关升级HTTP端口（80或443）到WebSoket。如果没有WebSockt中转，SIP信息将会被HTTP防火墙阻挡。
第三步，WebRTC用户和VoLTE用户都注册到了IMS网络。
第四步，WebRTC用户生成对VoLTE用户的请求，从STUN服务器获取公网IP地址后，通过SDP提供/应答实现交换并认同媒体功能。ICE候选（私网和公网IP地址）同时也进行交换。WebRTC网关执行必要的信令交互功能。
第五步，WebRTC用户发起与WebRTC网关中已交换的每一个ICE候选的链接检查（ICE冲孔）。当连接检查通过后，WebRTC用户和WebRTC网关间的DTLS-SRTP媒体会话就建立好了。在IMS网络中，在VoLTE用户和WebRTC网关会建立SDES-SRTP或者RTP会话。当WebRTC网关执行必须的媒体互通功能后，WebRTC和VoLTE用户间的媒体就产生了交换。
第六步，WebRTC用户断开与VoLTE用户的呼叫，或者反之亦然。
# **结论**
虽然WebRTC为移动运营商（MNO）的现有IMS网络投资提供了巨大的机会，但是运营商并没有给予WebRTC足够的重视，尤其是当使用WebRTC去扩展IMS服务到浏览器用户时，因为这需要通过使用WebRTC网关或类似的设备来改变网络架构，从而使服务互通。如果在网络架构时考虑WebRTC技术（必须关注WebRTC对网络架构的冲击和改变），移动运营商（MNO）在市场竞争和回收投资方面都有利可图。为实现市场对实时服务需求的不断增长和满足未来用户的新需求，初期的辛劳和投资都是值得的。
> 
更多[WebRTC优秀资源](https://link.jianshu.com?t=http://befo.io/424.html)可登陆[编风网](https://link.jianshu.com?t=http://befo.io/)http://befo.io/
微信公众号：WebRTC中文网，微信ID：webrtcorgcn
