# 如约而至：微信自用的移动端IM网络层跨平台组件库Mars已正式开源 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月29日 10:44:01[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1317
# 1、前言
关于微信内部正在使用的网络层封装库Mars开源的消息，1个多月前就已满天飞（参见《[微信Mars：微信内部正在使用的网络层封装库，即将开源](http://www.52im.net/thread-620-1-1.html)》），不过微信团队没有失约，微信Mars 于2016年12月28日正式公开源码（源码地址：[https://github.com/Tencent/mars](https://github.com/52im/mars)，也可从本文文末的附件下载之，Android版演示程序可以从文末的附件中下载）。
之前无论是微信团队还是手机QQ团队，都以腾讯公司的名义在Github开源了数个工程，但这些工程所受的关注度远不及Mars。之所以Mars广受关注的原因，其实搞移动端IM或推送技术的开发者同行都明白，因为移动网络实在太不可靠、太复杂，以至于写出一个能用于大规模用户环境的稳定、省流量、省电、数据传输流畅、弱网络健壮、后台自动保活等技术指标的IM或推送是相当困难的。
更为重要的原因是毕竟微信Mars经过微信团队多年积累并经过海量用户的测试和使用，是经受的住各种复杂移动端网络环境、各种乱七八糟型号智能手机的真实考验的。若Mars开源，必将为IM及相关技术应用领域的同行带来很多有价值的实践成果，毕竟微信的体量和应用规模决定了技术的高度，确实是值得同行学习和关注。
之前的文章，比如《[微信移动端应对弱网络情况的探索和实践PPT](http://www.52im.net/thread-619-1-1.html)》、《[微信Mars：微信内部正在使用的网络层封装库，即将开源](http://www.52im.net/thread-620-1-1.html)》，也都或多或少对Mars进行了初步介绍，但微信Mars到底是个啥玩意，它能解决啥问题？
**我们简要的概括一下，微信Mars解决了如下问题：**
> 
[1] 提供长连、短连两种网络通道；
[2] 常规的网络能力，例如 DNS 防劫持、动态 IP 下发、就近接入、容灾恢复等；
[3] 贴合移动互联网的网络层解决方案；
[4] 贴合移动终端的平台特性：前后台、活跃态、休眠、省电、省流量等。
以上特点，还不尽于概括微信Mars的技术特征，建议对C++熟悉的IM或推送技术同行可以直接去看看[Mars源码](https://github.com/52im/mars)。
那么微信Mars到底有什么用呢？毫无疑问，微信Mars存在的前提就是为了更好的服务微信这个超级IM而存在，最适合干的活就是开发移动端IM了，当然由于提炼的很好，相信移动端推送技术等都是可以使用微信Mars作为网络层lib来使用，从而以微信的成果为起点开发出拥有更加优秀网络体验的移动端富网络应用。
好了，言归正传，本文正文内容引用了微信开发团队的资料，请继续往下阅读。（本文同步发布于：[http://www.52im.net/thread-684-1-1.html](http://www.52im.net/thread-684-1-1.html)）
# 2、关联文章
《[移动端IM实践：实现Android版微信的智能心跳机制](http://www.52im.net/thread-120-1-1.html)》
《[微信Mars：微信内部正在使用的网络层封装库，即将开源](http://www.52im.net/thread-620-1-1.html)》
《[微信移动端应对弱网络情况的探索和实践PPT [附件下载]](http://www.52im.net/thread-619-1-1.html)》
《[微信异步化改造实践：8亿月活、单机千万连接背后的后台解决方案](http://www.52im.net/thread-624-1-1.html)》
# 3、微信Mars起源
2012 年中，微信支持包括 Android、iOS、Symbian 三个平台。但在各个平台上，微信客户端没有任何统一的基础模块。2012 年的微信正处于高速发展时期，各平台的迭代速度不一、使用的编程语言各异，后台架构也处在不断探索的过程中。多种因素使得各个平台基础模块的实现出现了差异，导致出现多次需要服务器做兼容的善后工作。网络作为微信的基础，重要性不言而喻。任何网络实现的 bug 都可能导致重大事故。例如微信的容灾实现，如果因为版本的实现差异，导致某些版本上无法进行容灾恢复，将会严重的影响用户体验，甚至造成用户的流失。我们亟需一套统一的网络基础库，为微信的高速发展保驾护航。
恰好，这个时候塞班渐入日暮，微信对塞班的支持也逐渐减弱。老大从塞班组抽调人力，组成一个三人小 team 的初始团队，开始着手做通用的基础组件。这个基础组件最初就定位为：跨平台、跨业务的基础组件。现在看，这个组件除了解决了已有问题，还给微信的高速发展带来了很多优势，例如：
> 
基础组件方便了开展专项的网络基础研究与优化。
基础组件为多平台的快速实现提供了有力的支持。
经过四年多的发展，跨平台的基础组件已经包含了网络组件、日志组件在内的多个组件。回头看，这是一条开荒路。
# 4、微信Mars设计原则
在基础模块的开发中，设计尤为重要。在设计上，微信基础组件以跨平台、跨业务为前提，遵从高可用，高性能，负载均衡的设计原则。
可用是一个即时通讯类 App 的立身之本。高可用又体现在多个层面上：网络的可用性、 App 的可用性、系统的可用性等。
**网络的可用性：**
移动互联网有着丢包率高、带宽受限、延迟波动、第三方影响等特点，使得网络的可用性，尤其是弱网络下的可用性变得尤为关键。Mars 的 STN 组件作为基于 socket 层的网络解决方案，在很多细节设计上会充分考虑弱网络下的可用性。
**App 的可用性：**
App 的可用性包含稳定性、运行性能等多个方面。文章高性能日志模块 xlog 描述了 xlog 在不影响 App 运行性能的前提下进行的大量设计思考。
**系统的可用性：**
除了考虑正常的使用场景，APP的设计还需要从整个系统的角度进行设计思考。例如在容灾设计上，Mars 不仅使用了服务器容灾方案，也设计了客户端的本地容灾。当部分服务器出灾时，目前微信可以做到，15min 内把95%以上的用户转移到可用服务器上。
保障高可用并不代表可以牺牲性能，对于一个用户使用最频繁的应用，反而更要对使用的资源精打细算。例如在 Mars 信令传输超时设计中，多级超时的设计充分的考虑了可用性与高性能之间的平衡取舍。
如果说高可用高性能只是客户端本身的考虑的话，负载均衡就需要结合服务器端来考虑了，做一个客户端网络永远不能只把眼光放在客户端上。任何有关网络访问的决策都要考虑给服务器所带来的额外压力是多大。为了选用质量较好的 IP，曾经写了完整的客户端测速代码，后来删掉，其中一个原因是因为不想给服务器带来额外的负担。Mars 的代码中，选择 IP 时用了大量的随机函数也是为了规避大量的用户同时访问同一台服务器而做的。
在这四年，我学到最多的就是简单和平衡。 把方案做的尽可能简单，这样才不容易出错。设计方案时大多数时候都不可能满足所有想达到的条件，这个时候就需要去平衡各个因素。在组件中一个很好的例子就是长连接的连接频率(具体实现见[longlink_connect_monitor.cc](https://github.com/52im/mars/blob/master/mars/stn/src/longlink_connect_monitor.cc))，这个连接频率就是综合耗电量，流量，网络高可用，用户行为等因素进行综合考虑的。
# 5、Mars 的发展历程
#### 5.1 阶段1：让微信跑起来
跨平台基础组件的需求起源于微信，首要目标当然是先承载起微信业务。为了不局限于微信，满足跨平台、跨业务的设计目标，在设计上，网络组件定位为客户端与服务端之间的无状态网络信令通道，即交互方式主要包含一来一回、主动push两种方式。这使得基础组件无需考虑请求间的关联性、时序性，核心接口得到了极大的简化。同时，简洁的交互也使得业务逻辑的耦合极少。目前基础组件与业务的交互只包括：编解码、auth状态查询两部分（具体见[stn_logic.h](https://github.com/52im/mars/blob/master/mars/stn/stn_logic.h)）。
> 
voidStartTask(...);
intOnTaskEnd(...);
voidOnPush(...);
boolReq2Buf(...);
intBuf2Resp(...);
boolMakeSureAuthed();
在线程模型的选择上，最早使用的是多线程模型。当需要异步做一个工作，就起一个线程。多线程势必少不了锁。但当灰度几次之后发现，想要规避死锁的四个必要条件并没有想象中的那么容易。用户使用场景复杂，客户端的时序、状态的影响因素多，例如网络切换事件、前后台事件、定时器事件、网络事件、任务事件等，导致了不少的死锁现象和对象析构时序错乱导致的内存非法访问问题。
这时，我们开始思考，多线程确实有它的优点：可以并发甚至并行提高运行速度。但是对于网络模块来说，性能瓶颈主要是在网络耗时上，并不在于本地程序执行速度上。那为何不把大部分程序执行改成串行的，这样就不会存在多线程临界区的问题，无锁自然就不会死锁。
因此，我们目前使用了消息队列的方案(具体实现见[comm/messagequeue](https://github.com/52im/mars/blob/master/mars/comm/messagequeue/)目录)，把绝大多数非阻塞操作放到消息队列里执行。并且规定，基础组件与调用方之间的交互必须1. 尽快完成，不进行任何阻塞操作；2. 单向调用，避免形成环状的复杂时序。消息队列的引入很好的改善了死锁问题，但消息队列的线程模型中，我们还是不能避免存在需要阻塞的调用，例如网络操作。在未来的尝试中，我们计划引入协程的方式，将线程模型尽可能的简化。
在其它技术选型上，有时甚至需要细节到API 的使用，比如考虑平台兼容性问题，舍弃了一些函数的线程安全版本，使用了 asctime、localtime、rand 等非线程安全的版本。
#### 5.2 阶段2：修炼内功
在多次的灰度验证、数据比对下，微信各平台的网络逻辑顺利的过渡到了统一基础组件。为了有效的验证组件的效果，我们开发了 smc 的统计监控组件，开始关注网络的各项指标，进行网络基础研究与优化，尤其是关注移动网络的特征。
基础网络优化：常规的网络能力，例如 DNS 防劫持、动态 IP 下发、就近接入、容灾恢复等，在这一阶段得到逐步的建设与完善。除此之外，Mars 的网络模块是基于 socket 层的网络解决方案，在缺失大而全的 HTTP 能力的同时，却可以将优化做到更细致，细致到连接策略、连接超时、多级读写超时、收发策略等每个网络过程中。例如，当遇到弱网络下连通率较低，或者某些连通率不好的的服务器影响使用时，我们使用了复合连接(代码见[complexconnect.cc](https://github.com/52im/mars/blob/master/mars/comm/socket/complexconnect.cc))和
 IP 排序(代码见[simple_ipport_sort.cc](https://github.com/52im/mars/blob/master/mars/stn/src/simple_ipport_sort.cc))的方案很好的应对这两个问题。
平台特性优化：虽然 Mars 是跨平台的基础组件，但在很多设计上是需要结合各平台的特性的。例如为了尽量减少频繁的唤醒手机，引入了智能心跳（详见《[移动端IM实践：实现Android版微信的智能心跳机制](http://www.52im.net/thread-120-1-1.html)》），并且在智能心跳中考虑了 Android 的 alarm 对齐特性(具体实现见[smart_heartbeat.cc](https://github.com/52im/mars/blob/master/mars/stn/src/smart_heartbeat.cc))。再如在网络切换时，为了平滑切换的过程，使用了
 iOS 中网络的特性，在 iOS 中做了延迟处理等。
移动特性优化：微信的使用场景大部分是在手机端进行使用，在组件的设计过程中，我们也会研究移动设备的特性，并进行结合优化。例如，结合移动设备的无线电资源控制器（RRC）的状态切换，对一些性能要求特别特别敏感的请求，进行提前激活的优化处理等。
#### 5.3 阶段3：“抓妖记”
基础组件全量上线微信后，以微信的用户量，当然也会遇到各种各样的“妖”。例如，写网络程序躲不开运营商。印象比较深刻的某地的用户反馈连接 WiFi 时，微信不可用，后来 tcpdump 发现，当包的大小超过一定大小后就发不出去。解决方案：在 WiFi 网络下强制把 MSS 改为1400(代码见[unix_socket.cc](https://github.com/52im/mars/blob/master/mars/comm/socket/unix_socket.cc))。
**做移动客户端更避不开手机厂商。一次遇到了一个百思不得其解的 crash，堆栈如下：**
> 
#00pc0x43e50/system/lib/libc.so (???)
#01pc0x3143/system/vendor/lib/libvendorconn.so (handleDpmIpcReq+154)
#02pc0x2f6d/system/vendor/lib/libvendorconn.so (send_ipc_req+276)
#03pc0x30ff/system/vendor/lib/libcneconn.so (connect+438)
看堆栈结合程序 xlog 分析，非阻塞 socket 卡在了 connect 函数里超过了6 min, 被我们自带的 anr 检测(代码见anr.cc)发现然后自杀。最后实在束手无策，联系厂商一起排查，最终查明原因：为了省电，当手机锁屏时连的不是 WiFi 且又没有下行网络数据时，芯片 gate 会关闭，block 住所有网络请求，直到有下行数据或者超过 20min 才会放开。当手机有网络即使是手机网络的情况下，很难没有下行数据，所以基本不会触发组件自带的 anr 检测，但当手机没连接任何网络时，就很容易触发。解决方案：厂商修改代码逻辑，当没有任何网络时不
 block 网络请求。
运营商和手机厂商对我们来说已经是一个黑盒，但其实也遇到过更黑的黑盒。当手机长时间不重启，有极小概率不能继续使用微信，重启手机会恢复。但因为一直找不到一个愿意配合我们又满足条件的用户，导致这个问题很长一段时间内都没有任何进展，最终偶然一个机会，在一台测试机器上重现了该问题，tcpdump 发现在三步握手阶段，服务器带回的客户端带过去的 tsval 字段被篡改，导致三步握手直接失败，而且这个篡改发生在离开服务器之后到达客户端之前。
这个问题是微信网络模块中排查时间最长也是花费精力最多的一个问题，不仅因为很长一段时间内无案例可分析，也因为在重现后，联系了大量的同事和外部有关人的帮忙，想排查出罪魁祸首。但因为中间涉及的环节和运营商相关部门过多，无法继续排查下去，最终也没找到根本原因。 解决办法：服务器更改 net.ipv4.tcp_timestamps = 0。
这段时间是痛并快乐着，见识到了各种极差的网络，才切肤感受到移动网络环境的恶劣程度，但看着我们的网络性能数据在稳步提升又有种满足感。截止到今天，已经很少有真正的网络问题需要跟进了。这也是我们能有时间开始把这些代码开源出去的很大的一个原因。
# 6、为了更好地开源Mars，微信团队做了这些前期工作
大概一年前（大约2015年10月），我们开始有想法把基础组件开源出去，当时大家都在纠结叫什么名字好呢？此时恰逢《火星救援》正在热映，一位同事说干脆叫 Mars 吧，于是就定下来叫了 Mars。看了看代码，发现想要开源出去可能还是需要做一些其他工作的。
#### 6.1 代码重构
首先，代码风格方面，因为最初我们使用文件名、函数名、变量名的规则是内部定义的规则，为了能让其他人读起来更舒心，我们决定把代码风格改为谷歌风格，比如：变量名一律小写, 单词之间用下划线连接；左大括号不换行等等。但是为了更好的区分访问空间，我们又在谷歌代码风格进行了一些变通，比如：私有函数全部是”__”开头；函数参数全部以”_”开头 等等。
其次，虽然最初的设计一直是秉承着业务性无关的设计，但在实际开发过程中仍然难免带上了微信的业务性相关代码，比较典型的就是 newdns 。为了 Mars 以后的维护以及保证开源出去代码的同源，在开源出去之前必须把这些业务性有关的代码抽离出来，抽离后的结构如下：
> 
mars-open 也就是要开源出去的代码，独立 git repo。
mars-private 是可能开源出去的代码，依赖 mars-open。
mars-wechat 是微信业务性相关的代码，依赖 mars-open 和 mars-private。
最后，为了接口更易用，对调用接口以及回调接口的参数也进行了反复思考与修改。
#### 6.2 编译优化
在 Mars之前，是直接给 Android 提供动态库(.so)，因为代码逻辑都已经固定，不需要有可定制的部分。给 Apple 系平台提供静态库(.a)，因为对外暴露的函数几乎不会改变，直接把相应的头文件放到相应的项目里就行。但对外开源就完全不一样了：日志的加密算法可能别人需要自己实现；长连或者短连的包头有人需要自己定制；对外接口的头文件我们可能会修改……
**为了让使用者可定制代码，对于编译 Android 平台我们提供了两种选择：**
> 
1. 动态库。有些可能需要定制的代码都提供了默认实现。
2. 先编译静态库，再编译动态库。
编译出来静态库后，实现自己需要定制的代码后，执行 ndk-build 后即可编译出来动态库。 对于 Apple 系平台，把头文件全部收拢为 Mars 维护，直接编译出 Framework。
为了能让开发者快速的入门，我们提供了 Android、iOS、OS X 平台的 demo（[微信开源Mars的Demo源码点此进入](https://github.com/52im/mars/tree/master/samples)），其他平台的编译和 demo 会在不久就加上支持。
![](http://upload-images.jianshu.io/upload_images/1500839-56dd9cd7e6242638.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
▲ 成型的 Mars 结构图如上图所示
#### 6.3 同类技术横向对比
我们做的一直都不是满足所有需求的组件，只是做了一个更适合我们使用的组件，这里也列了下和同类型的开源代码的对比。
![](http://upload-images.jianshu.io/upload_images/1500839-1117bd4131a90912.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
**可以看出：**
> 
Mars 中包括一个完整的高性能的日志组件 xlog；
Mars 中 STN 是一个跨平台的 socket 层解决方案，并不支持完整的 HTTP 协议；
Mars 中 STN 模块是更加贴合“移动互联网”、“移动平台”特性的网络解决方案，尤其针对弱网络、平台特性等有很多的相关优化策略。
总的来说，Mars 是一个结合移动 App 所设计的基于 socket 层的解决方案，在网络调优方面有更好的可控性，对于 HTTP 完整协议的支持，已经考虑后续版本会加入。
#### 6.4 Mars开源源码结构
![](http://upload-images.jianshu.io/upload_images/1500839-0206aedc6ff595e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 7、获取Mars的演示Demo（Sample）
#### 7.1 下载和安装
试用 Android Demo 请从文末的附件下载之，iOS sample 请通过[Mars的Github仓库](https://github.com/52im/mars/tree/master/samples)编译获得（或文末的附件下载源码）。
#### 7.2 Mars Android版演示程序截图
![](http://upload-images.jianshu.io/upload_images/1500839-8943f95f60967721.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1500839-e46d4f8e8884b1a5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 7.3 Mars iOS版演示程序源码截图
![](http://upload-images.jianshu.io/upload_images/1500839-2ec30121e86b8d92.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 8、本文小结
经常有朋友和我说：发现网络信号差的时候或者其他应用不能用的时候，微信仍然能发出去消息。不知不觉我们好像什么都没做，回头看，原来我们已经做了这么多。
我想，并不是任何一行代码都可以经历日活跃5亿用户的考验，感谢微信给我们提供了这么一个平台。现在我们想把这些代码和你们分享，运营方式上 Mars 所开源出去的代码会和微信所用的代码保持同源，所有开源出去的代码也首先会在微信上验证通过后再公开。
开源并不是结束，只是开始。我们后续仍然会继续探索在移动互联网下的网络优化。Talk is cheap，show you our code。
（本文同步发布于：[http://www.52im.net/thread-684-1-1.html](http://www.52im.net/thread-684-1-1.html)，本文引用了微信团队原创文章：[原始文字点此进入](http://mp.weixin.qq.com/s?__biz=MzAwNDY1ODY2OQ==&mid=2649286451&idx=1&sn=9711761792fe800094efde219fda3cde&chksm=8334c3b1b4434aa7aefc6b77b47b64de70cd7ed77799e716b8881eaa4649a959a90e9d24cc25&scene=0#rd)）
# 9、更多IM技术资料
> 
**[1] 网络编程基础资料：**
《[TCP/IP详解](http://www.52im.net/topic-tcpipvol1.html)-[第11章·UDP：用户数据报协议](http://docs.52im.net/extend/docs/book/tcpip/vol1/11/)》
《[TCP/IP详解](http://www.52im.net/topic-tcpipvol1.html)-[第17章·TCP：传输控制协议](http://docs.52im.net/extend/docs/book/tcpip/vol1/17/)》
《[TCP/IP详解](http://www.52im.net/topic-tcpipvol1.html)-[第18章·TCP连接的建立与终止](http://docs.52im.net/extend/docs/book/tcpip/vol1/18/)》
《[TCP/IP详解](http://www.52im.net/topic-tcpipvol1.html)-[第21章·TCP的超时与重传](http://docs.52im.net/extend/docs/book/tcpip/vol1/21/)》
《[技术往事：改变世界的TCP/IP协议（珍贵多图、手机慎点）](http://www.52im.net/thread-520-1-1.html)》
《[通俗易懂-深入理解TCP协议（上）：理论基础](http://www.52im.net/thread-513-1-1.html)》
《[通俗易懂-深入理解TCP协议（下）：RTT、滑动窗口、拥塞处理](http://www.52im.net/thread-515-1-1.html)》
《[理论经典：TCP协议的3次握手与4次挥手过程详解](http://www.52im.net/thread-258-1-1.html)》
《[理论联系实际：Wireshark抓包分析TCP 3次握手、4次挥手过程](http://www.52im.net/thread-275-1-1.html)》
《[计算机网络通讯协议关系图（中文珍藏版）](http://www.52im.net/thread-180-1-1.html)》
《[UDP中一个包的大小最大能多大？](http://www.52im.net/thread-29-1-1.html)》
《[Java新一代网络编程模型AIO原理及Linux系统AIO介绍](http://www.52im.net/thread-306-1-1.html)》
《[NIO框架入门(一)：服务端基于Netty4的UDP双向通信Demo演示](http://www.52im.net/thread-367-1-1.html)》
《[NIO框架入门(二)：服务端基于MINA2的UDP双向通信Demo演示](http://www.52im.net/thread-373-1-1.html)》
《[NIO框架入门(三)：iOS与MINA2、Netty4的跨平台UDP双向通信实战](http://www.52im.net/thread-378-1-1.html)》
《[NIO框架入门(四)：Android与MINA2、Netty4的跨平台UDP双向通信实战](http://www.52im.net/thread-388-1-1.html)》
《[P2P技术详解(一)：NAT详解——详细原理、P2P简介](http://www.52im.net/thread-50-1-1.html)》
《[P2P技术详解(二)：P2P中的NAT穿越(打洞)方案详解](http://www.52im.net/thread-542-1-1.html)》
《[P2P技术详解(三)：P2P技术之STUN、TURN、ICE详解](http://www.52im.net/thread-557-1-1.html)》
《[高性能网络编程(一)：单台服务器并发TCP连接数到底可以有多少](http://www.52im.net/thread-561-1-1.html)》
《[高性能网络编程(二)：上一个10年，著名的C10K并发连接问题](http://www.52im.net/thread-566-1-1.html)》
《[高性能网络编程(三)：下一个10年，是时候考虑C10M并发问题了](http://www.52im.net/thread-568-1-1.html)》
《[高性能网络编程(四)：从C10K到C10M高性能网络应用的理论探索](http://www.52im.net/thread-578-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=9)
**[2] 有关IM/推送的通信格式、协议的选择：**
《[为什么QQ用的是UDP协议而不是TCP协议？](http://www.52im.net/thread-279-1-1.html)》
《[移动端即时通讯协议选择：UDP还是TCP？](http://www.52im.net/thread-33-1-1.html)》
《[如何选择即时通讯应用的数据传输格式](http://www.52im.net/thread-276-1-1.html)》
《[强列建议将Protobuf作为你的即时通讯应用数据传输格式](http://www.52im.net/thread-277-1-1.html)》
《[移动端IM开发需要面对的技术问题（含通信协议选择）](http://www.52im.net/thread-133-1-1.html)》
《[简述移动端IM开发的那些坑：架构设计、通信协议和客户端](http://www.52im.net/thread-289-1-1.html)》
《[理论联系实际：一套典型的IM通信协议设计详解](http://www.52im.net/thread-283-1-1.html)》
《[58到家实时消息系统的协议设计等技术实践分享](http://www.52im.net/thread-298-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=18)
**[3] 有关IM/推送的心跳保活处理：**
《[Android进程保活详解：一篇文章解决你的所有疑问](http://www.52im.net/thread-438-1-1.html)》
《[Android端消息推送总结：实现原理、心跳保活、遇到的问题等](http://www.52im.net/thread-341-1-1.html)》
《[为何基于TCP协议的移动端IM仍然需要心跳保活机制？](http://www.52im.net/thread-281-1-1.html)》
《[微信团队原创分享：Android版微信后台保活实战分享(进程保活篇)](http://www.52im.net/thread-210-1-1.html)》
《[微信团队原创分享：Android版微信后台保活实战分享(网络保活篇)](http://www.52im.net/thread-209-1-1.html)》
《[移动端IM实践：实现Android版微信的智能心跳机制](http://www.52im.net/thread-120-1-1.html)》
《[移动端IM实践：WhatsApp、Line、微信的心跳策略分析](http://www.52im.net/thread-121-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=17)
**[4] 有关WEB端即时通讯开发：**
《[新手入门贴：史上最全Web端即时通讯技术原理详解](http://www.52im.net/thread-338-1-1.html)》
《[Web端即时通讯技术盘点：短轮询、Comet、Websocket、SSE](http://www.52im.net/thread-336-1-1.html)》
《[SSE技术详解：一种全新的HTML5服务器推送事件技术](http://www.52im.net/thread-335-1-1.html)》
《[Comet技术详解：基于HTTP长连接的Web端实时通信技术](http://www.52im.net/thread-334-1-1.html)》
《[WebSocket详解（一）：初步认识WebSocket技术](http://www.52im.net/thread-331-1-1.html)》
《[socket.io实现消息推送的一点实践及思路](http://www.52im.net/thread-188-1-1.html)》
《[LinkedIn的Web端即时通讯实践：实现单机几十万条长连接](http://www.52im.net/thread-659-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=15)
**[5] 有关IM架构设计：**
《[浅谈IM系统的架构设计](http://www.52im.net/thread-307-1-1.html)》
《[简述移动端IM开发的那些坑：架构设计、通信协议和客户端](http://www.52im.net/thread-289-1-1.html)》
《[一套原创分布式即时通讯(IM)系统理论架构方案](http://www.52im.net/thread-151-1-1.html)》
《[从零到卓越：京东客服即时通讯系统的技术架构演进历程](http://www.52im.net/thread-152-1-1.html)》
《[蘑菇街即时通讯/IM服务器开发之架构选择](http://www.52im.net/thread-31-1-1.html)》
《[腾讯QQ1.4亿在线用户的技术挑战和架构演进之路PPT](http://www.52im.net/thread-158-1-1.html)》
《[微信技术总监谈架构：微信之道——大道至简(演讲全文)](http://www.52im.net/thread-200-1-1.html)》
《[如何解读《微信技术总监谈架构：微信之道——大道至简》](http://www.52im.net/thread-201-1-1.html)》
《[快速裂变：见证微信强大后台架构从0到1的演进历程（一）](http://www.52im.net/thread-168-1-1.html)》
《[17年的实践：腾讯海量产品的技术方法论](http://www.52im.net/thread-159-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=7)
**[6] 有关IM安全的文章：**
《[即时通讯安全篇（一）：正确地理解和使用Android端加密算法](http://www.52im.net/thread-216-1-1.html)》
《[即时通讯安全篇（二）：探讨组合加密算法在IM中的应用](http://www.52im.net/thread-217-1-1.html)》
《[即时通讯安全篇（三）：常用加解密算法与通讯安全讲解](http://www.52im.net/thread-219-1-1.html)》
《[即时通讯安全篇（四）：实例分析Android中密钥硬编码的风险](http://www.52im.net/thread-312-1-1.html)》
《[即时通讯安全篇（五）：对称加密技术在Android平台上的应用实践](http://www.52im.net/thread-642-1-1.html)》
《[即时通讯安全篇（六）：非对称加密技术的原理与应用实践](http://www.52im.net/thread-653-1-1.html)》
《[传输层安全协议SSL/TLS的Java平台实现简介和Demo演示](http://www.52im.net/thread-327-1-1.html)》
《[理论联系实际：一套典型的IM通信协议设计详解（含安全层设计）](http://www.52im.net/thread-283-1-1.html)》
《[微信新一代通信安全解决方案：基于TLS1.3的MMTLS详解](http://www.52im.net/thread-310-1-1.html)》
《[来自阿里OpenIM：打造安全可靠即时通讯服务的技术实践分享](http://www.52im.net/thread-215-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=6)
**[7] 有关实时音视频开发：**
《[即时通讯音视频开发（一）：视频编解码之理论概述](http://www.52im.net/thread-228-1-1.html)》
《[即时通讯音视频开发（二）：视频编解码之数字视频介绍](http://www.52im.net/thread-229-1-1.html)》
《[即时通讯音视频开发（三）：视频编解码之编码基础](http://www.52im.net/thread-232-1-1.html)》
《[即时通讯音视频开发（四）：视频编解码之预测技术介绍](http://www.52im.net/thread-235-1-1.html)》
《[即时通讯音视频开发（五）：认识主流视频编码技术H.264](http://www.52im.net/thread-237-1-1.html)》
《[即时通讯音视频开发（六）：如何开始音频编解码技术的学习](http://www.52im.net/thread-241-1-1.html)》
《[即时通讯音视频开发（七）：音频基础及编码原理入门](http://www.52im.net/thread-242-1-1.html)》
《[即时通讯音视频开发（八）：常见的实时语音通讯编码标准](http://www.52im.net/thread-243-1-1.html)》
《[即时通讯音视频开发（九）：实时语音通讯的回音及回音消除概述](http://www.52im.net/thread-247-1-1.html)》
《[即时通讯音视频开发（十）：实时语音通讯的回音消除技术详解](http://www.52im.net/thread-250-1-1.html)》
《[即时通讯音视频开发（十一）：实时语音通讯丢包补偿技术详解](http://www.52im.net/thread-251-1-1.html)》
《[即时通讯音视频开发（十二）：多人实时音视频聊天架构探讨](http://www.52im.net/thread-253-1-1.html)》
《[即时通讯音视频开发（十三）：实时视频编码H.264的特点与优势](http://www.52im.net/thread-266-1-1.html)》
《[即时通讯音视频开发（十四）：实时音视频数据传输协议介绍](http://www.52im.net/thread-267-1-1.html)》
《[即时通讯音视频开发（十五）：聊聊P2P与实时音视频的应用情况](http://www.52im.net/thread-269-1-1.html)》
《[即时通讯音视频开发（十六）：移动端实时音视频开发的几个建议](http://www.52im.net/thread-270-1-1.html)》
《[即时通讯音视频开发（十七）：视频编码H.264、V8的前世今生](http://www.52im.net/thread-274-1-1.html)》
《[网易视频云技术分享：音频处理与压缩技术快速入门](http://www.52im.net/thread-678-1-1.html)》
《[学习RFC3550：RTP/RTCP实时传输协议基础知识](http://www.52im.net/thread-590-1-1.html)》
《[简述开源实时音视频技术WebRTC的优缺点](http://www.52im.net/thread-225-1-1.html)》
《[良心分享：WebRTC 零基础开发者教程（中文）](http://www.52im.net/thread-265-1-1.html)》
《[开源实时音视频技术WebRTC中RTP/RTCP数据传输协议的应用](http://www.52im.net/thread-589-1-1.html)》
《[基于RTMP数据传输协议的实时流媒体技术研究（论文全文）](http://www.52im.net/thread-273-1-1.html)》
《[声网架构师谈实时音视频云的实现难点(视频采访)](http://www.52im.net/thread-399-1-1.html)》
《[浅谈开发实时视频直播平台的技术要点](http://www.52im.net/thread-475-1-1.html)》
《[还在靠“喂喂喂”测试实时语音通话质量？本文教你科学的评测方法！](http://www.52im.net/thread-507-1-1.html)》
《[实现延迟低于500毫秒的1080P实时音视频直播的实践分享](http://www.52im.net/thread-528-1-1.html)》
《[移动端实时视频直播技术实践：如何做到实时秒开、流畅不卡](http://www.52im.net/thread-530-1-1.html)》
《[如何用最简单的方法测试你的实时音视频方案](http://www.52im.net/thread-535-1-1.html)》
《[技术揭秘：支持百万级粉丝互动的Facebook实时视频直播](http://www.52im.net/thread-541-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=4)
**[8] IM开发综合文章：**
《[移动端IM开发需要面对的技术问题](http://www.52im.net/thread-133-1-1.html)》
《[开发IM是自己设计协议用字节流好还是字符流好？](http://www.52im.net/thread-150-1-1.html)》
《[请问有人知道语音留言聊天的主流实现方式吗？](http://www.52im.net/thread-175-1-1.html)》
《[IM消息送达保证机制实现(一)：保证在线实时消息的可靠投递](http://www.52im.net/thread-294-1-1.html)》
《[IM消息送达保证机制实现(二)：保证离线消息的可靠投递](http://www.52im.net/thread-594-1-1.html)》
《[谈谈移动端 IM 开发中登录请求的优化](http://www.52im.net/thread-282-1-1.html)》
《[完全自已开发的IM该如何设计“失败重试”机制？](http://www.52im.net/thread-280-1-1.html)》
《[微信对网络影响的技术试验及分析（论文全文）](http://www.52im.net/thread-195-1-1.html)》
《[即时通讯系统的原理、技术和应用（技术论文）](http://www.52im.net/thread-218-1-1.html)》
《[开源IM工程“蘑菇街TeamTalk”的现状：一场有始无终的开源秀](http://www.52im.net/thread-447-1-1.html)》
《[腾讯原创分享(一)：如何大幅提升移动网络下手机QQ的图片传输速度和成功率](http://www.52im.net/thread-675-1-1.html)》
《[如约而至：微信自用的移动端IM网络层跨平台组件库Mars已正式开源](http://www.52im.net/thread-684-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=10)
**[9] 开源移动端IM技术框架资料：**
《[开源移动端IM技术框架MobileIMSDK：快速入门](http://www.52im.net/thread-52-1-1.html)》
《[开源移动端IM技术框架MobileIMSDK：常见问题解答](http://www.52im.net/thread-60-1-1.html)》
《[开源移动端IM技术框架MobileIMSDK：压力测试报告](http://www.52im.net/thread-57-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=1)
**[10] 有关推送技术的文章：**
《[iOS的推送服务APNs详解：设计思路、技术原理及缺陷等](http://www.52im.net/thread-345-1-1.html)》
《[Android端消息推送总结：实现原理、心跳保活、遇到的问题等](http://www.52im.net/thread-341-1-1.html)》
《[扫盲贴：认识MQTT通信协议](http://www.52im.net/thread-318-1-1.html)》
《[一个基于MQTT通信协议的完整Android推送Demo](http://www.52im.net/thread-315-1-1.html)》
《[IBM技术经理访谈：MQTT协议的制定历程、发展现状等](http://www.52im.net/thread-525-1-1.html)》
《[求教android消息推送：GCM、XMPP、MQTT三种方案的优劣](http://www.52im.net/thread-314-1-1.html)》
《[移动端实时消息推送技术浅析](http://www.52im.net/thread-288-1-1.html)》
《[扫盲贴：浅谈iOS和Android后台实时消息推送的原理和区别](http://www.52im.net/thread-286-1-1.html)》
《[绝对干货：基于Netty实现海量接入的推送服务技术要点](http://www.52im.net/thread-166-1-1.html)》
《[移动端IM实践：谷歌消息推送服务(GCM)研究（来自微信）](http://www.52im.net/thread-122-1-1.html)》
《[为何微信、QQ这样的IM工具不使用GCM服务推送消息？](http://www.52im.net/thread-117-1-1.html)》
《[极光推送系统大规模高并发架构的技术实践分享](http://www.52im.net/thread-602-1-1.html)》
《[从HTTP到MQTT：一个基于位置服务的APP数据通信实践概述](http://www.52im.net/thread-605-1-1.html)》
>>[更多同类文章 ……](http://www.52im.net/forum.php?mod=collection&action=view&ctid=11)
**[11] 更多即时通讯技术好文分类：**
[http://www.52im.net/forum.php?mod=collection&op=all](http://www.52im.net/forum.php?mod=collection&op=all)
> 
**作者：**[Jack Jiang](https://github.com/JackJiang2011)(点击作者姓名进入Github)
**出处：**[http://www.52im.net/space-uid-1.html](http://www.52im.net/space-uid-1.html)
**交流：**欢迎加入即时通讯开发交流群[215891622](http://shang.qq.com/wpa/qunwpa?idkey=4cc788473d261129ab3ded26fbb22168d0fa52c799d28f92a8f193dc36865bcb)
**讨论：**[http://www.52im.net/](http://www.52im.net/)
**Jack Jiang同时是**[【开源原创Java Swing外观工程BeautyEye】](http://git.oschina.net/jackjiang/beautyeye/)和[【开源轻量级移动端即时通讯框架MobileIMSDK】](http://git.oschina.net/jackjiang/beautyeye/)的作者，可前往下载交流。
- [](http://www.52im.net/thread-594-1-1.html)
###### IM消息送达保证机制实现(二)：保证离线消息的可靠投递-IM开发/专项技术区 - 即时通讯开发者社区!
— IM消息送达保证机制实现(二)：保证离线消息的可靠投递
[jb2011](/users/a240b0ea61be)・
[www.52im.net →](http://www.52im.net/thread-594-1-1.html)
- [](http://www.52im.net/thread-520-1-1.html)
###### 技术往事：改变世界的TCP/IP协议（珍贵多图、手机慎点）-自由讨论/综合互动区 - 即时通讯开发者社区!
— 技术往事：改变世界的TCP/IP协议（珍贵多图、手机慎点）
[jb2011](/users/a240b0ea61be)・
[www.52im.net →](http://www.52im.net/thread-520-1-1.html)
- [](http://www.52im.net/thread-464-1-1.html)
###### 新手入门一篇就够：从零开发移动端IM-IM开发/专项技术区 - 即时通讯开发者社区!
— 新手入门一篇就够：从零开发移动端IM
[jb2011](/users/a240b0ea61be)・
[www.52im.net →](http://www.52im.net/thread-464-1-1.html)
- [](http://www.52im.net/thread-447-1-1.html)
###### 开源IM工程“蘑菇街TeamTalk”的现状：一场有始无终的开源秀-自由讨论/综合互动区 - 即时通讯开发者社区!
— 开源IM工程“蘑菇街TeamTalk”的现状：一场有始无终的开源秀
[jb2011](/users/a240b0ea61be)・
[www.52im.net →](http://www.52im.net/thread-447-1-1.html)
- [](http://www.52im.net/thread-338-1-1.html)
###### 新手入门贴：史上最全Web端即时通讯技术原理详解-IM开发/专项技术区 - 即时通讯开发者社区!
— 新手入门贴：史上最全Web端即时通讯技术原理详解
[jb2011](/users/a240b0ea61be)・
[www.52im.net →](http://www.52im.net/thread-338-1-1.html)
- [](http://www.52im.net/forum.php?mod=viewthread&tid=506)
###### 独家发布《TCP/IP详解 卷1：协议》CHM版 [附件下载]-资源共享/综合互动区 - 即时通讯开发者社区!
— 独家发布《TCP/IP详解 卷1：协议》CHM版 [附件下载]
[jb2011](/users/a240b0ea61be)・
[www.52im.net →](http://www.52im.net/forum.php?mod=viewthread&tid=506)
- [](http://www.52im.net/thread-33-1-1.html)
###### 移动端IM/推送系统的协议选型：UDP还是TCP？-IM开发/专项技术区 - 即时通讯开发者社区!
— 移动端IM/推送系统的协议选型：UDP还是TCP？
[jb2011](/users/a240b0ea61be)・
[www.52im.net →](http://www.52im.net/thread-33-1-1.html)
- [](http://www.52im.net/thread-675-1-1.html)
###### 腾讯原创分享(一)：如何大幅提升移动网络下手机QQ的图片传输速度和成功率-网络编程/专项技术区 - 即时通讯开发者社区!
— 腾讯原创分享(一)：如何大幅提升移动网络下手机QQ的图片传输速度和成功率
文／jb2011（简书作者）
原文链接：http://www.jianshu.com/p/ef98be666b27
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
