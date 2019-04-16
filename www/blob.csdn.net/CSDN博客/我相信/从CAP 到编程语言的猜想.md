# 从CAP 到编程语言的猜想 - 我相信...... - CSDN博客





2017年03月13日 08:32:18[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：10967








TIBOE 有如期的发布了最新的编程语言的排行榜，变化总是有的，这是今年3月的榜单：

![这里写图片描述](https://img-blog.csdn.net/20170313082346815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

关于编程语言的争议总是有的，不论是各个论坛，微信群，QQ群，乃至知乎，Quara 等知名网站，总会有对各种编程语言的吐槽。一句“PHP是世界上最好的语言”可能会引起一场又一场的激战，这就话仿佛成了一个“梗”，我自己也忍不住写了一篇《PHP是最好的语言吗》，但编程语言的鄙视链令很多人无休止的追问，这不应该只是一种程序员间的乐此不疲吧？！

在微服务中，数据库的集成是一件不会令人愉悦的事，因为分布式系统CAP理论在起作用吧。 
![这里写图片描述](https://img-blog.csdn.net/20170313082631237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- Consistency(一致性),数据一致更新，所有数据变动都是同步的
- Availability(可用性) 好的响应性能
- Partition tolerance(分区容错性) 可靠性

CAP 的核心思想是任何分布式系统只可同时满足二点，没法三者兼顾。

由此，类比到编程语言会是怎样的情景呢？

是否可以对编程语言进行抽象呢？ 而且抽象到3个维度呢？

根据自己所用过的编程语言的经验（35岁前曾经每年学一门编程语言），仿佛可以把编程语言归纳到3个约束SAP：

### Simple 简洁明快
- 编程语言的读与写都容易理解，学习曲线平滑而且短
- 结构紧凑，容易协作与维护
- 拥有丰富的类库，以及完善的参考手册
- 依赖少，部署同样简单
- ……

### Adaptive 适用性强
- 适用于多个应用领域，支持各种层次的抽象
- 伸缩性好，增减依赖都很方便
- 支持多种编程范式
- 可编译执行，也可解释执行
- 甚至可以定制派生的语言
- ……

### Performace 性能高效
- 不论使用IDE还是文本编辑器，代码编写都很快
- 执行速度快，充分利用CPU甚至GPU的能力
- 编译的速度同样快，支持JIT
- 故障定位的速度快，debug快捷精准
- ……

如果分成这样3个约束，会发现它们之间存在着内在的矛盾，就像CAP那样：

![这里写图片描述](https://img-blog.csdn.net/20170313082802473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

SA 是Python，Java，还是……？

AP 是C，C++, 还是 ……?

SP 难道是 汇编么，还是 ……?

CAP 给我们的启示是 架构师不要将精力浪费在如何设计能满足三者的完美分布式系统，而是应该进行取舍。

如果SAP 的猜想是成立的，那么给我们带来怎样的启示呢？ 会不会是混合编程是一种必然呢？ 没有仔细阅读CAP的数学证明，也没有努力想用数学的方法去证明SAP的成立，现在只是老码农的猜想，可能是一种遗憾吧。

回顾一下编程语言的历史，献上一张来自veracode.com的旧图：

![这里写图片描述](https://img-blog.csdn.net/20170313082902334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是一张不太完整的编年史。老码农总尝试溯源，视图从其他的角度看编程语言。
那为什么会将编程语言划分为S，A，P三类而不是别的什么呢？

因为老码农对编程的理解是软件的时空与人的关系。

软件时空与人的关系是形而上而务虚么？

老码农还没有那个本领，如果想知道老码农如何思考软件时空与人，以及全栈与全栈团队，请关注：

![这里写图片描述](https://img-blog.csdn.net/20170313083006694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

或者在嘉年华之后，见字如面，只是没有了现场的那个朗读者。





