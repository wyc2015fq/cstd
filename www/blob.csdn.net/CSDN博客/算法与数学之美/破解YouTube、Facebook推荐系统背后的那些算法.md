# 破解YouTube、Facebook推荐系统背后的那些算法 - 算法与数学之美 - CSDN博客
2018年09月20日 22:45:56[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：339
最初看到这篇文章是@fengyoung 在Facebook上分享的，觉得题目很有意思就看了一遍，看完后感觉很有启发，遂决定翻译一下让更多人看到。
这篇文章内所指的算法包含多个YouTube增长类算法（为你推荐（Recommended），建议观看（Suggest），相关视频（Related），搜索（Search），原始评分（MetaScore），等等）。这些不同的算法产品，各有侧重，但有一个共同点，那就是它们的优化目标相同，都是观看时长（Watch Time）。
** 这篇文章给我的启发有三方面：**
1. 从YouTube平台的算法设计人员角度，设计繁多的推荐算法，是为了提高频道的观看时长，而提高频道的观看时长又是为了让用户能够经常访问平台。这是一种双赢的思维，说白了：**谁能帮平台留住用户，平台就重点扶持他。**
2. 文章得出结论，要做垂直内容才能在YouTube上活下去。平台上内容越多样，平台越健康，这是毋庸置疑的，尽管我赞同这个结论，但是我没有在本文中看到作者是如何得到这个结论的。**这一点就是YouTube和国内视频平台最大的差别，国内的视频平台严重趋同，花高价购买独家版权似乎是国内视频平台的唯一出路，也是一个妖魔化的出路**。反观YouTube，他们利用算法驱使了各个频道专耕某一个垂直内容，然后把最适合的用户给你匹配上，这才是更宏大的一盘内容棋。
3. 本文作者给我们了一个启示，**算法并不是黑盒子，是可以hack的**，尽管这个也只能hack到冰山一角，但是也比我们盲目地运营要明亮很多了。作者的研究方式，首先是明确了一个平台的算法目标是什么，YouTube是watch time，那么就去观察这个目标和哪些指标有关，进一步看到每个指标又能怎么提高。
感兴趣的朋友可以搜索着看，我这里就不过多重复了。
关于Facebook的算法，我在《推荐系统36式》专栏里都有提到，你也可以看下图，总结了推荐系统背后的那些算法，都是需要你花时间去学习和实践的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzow2ORRzgpfHIGAKIAWlXmelOw69WRdDX8pqPMKoj2zEa18HfBVb4yOQAicasBHOsoVsvViaNwy3Pw/640?wx_fmt=jpeg)
**为什么会有《推荐系统36式》这个专栏？**
最近十年尤其最近五年，借助推荐系统的技术和名头，异军突起的互联网产品越来越多，Youtube、淘宝、京东、Netflix、今日头条、Amazon等等这些产品都已经从个性化推荐中尝到了商业的甜头。甚至有人说在未来，推荐系统会成为所有数据型产品的标配。
然而推荐系统前方技术蓬勃发展，后方却落地困难。
审视推荐系统的技术应用现状，大厂们一骑绝尘，但太多中小厂的工程师们还不知道一个推荐系统如何从 0 到 1 诞生，需要去了解哪些知识。比如有人问我这些问题：
**1. 我们产品这个阶段需要上线推荐系统吗？推荐系统前期投入大吗？**
**2. 推荐系统这事容易整吗？里面那些算法到底是怎么回事？**
**3. 搭建一个推荐系统，这里面有哪些坑？**
**4. 推荐架构、搜索引擎和广告系统之间应该如何协同？**
**5. 推荐系统相关的开源软件都有哪些？如何选型？**
所以我就顺势写了一个推荐系统相关的专栏，希望能帮助推荐系统学习者架构起整体的知识脉络，并在此基础上补充实践案例与经验，力图解决你系统起步阶段 80% 的问题。
下面是这个专栏完结目录的部分，目前只在极客时间上发布，你需要付费阅读（在本文最后有相关的二维码）。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzow2ORRzgpfHIGAKIAWlXmXUD15CiabpJUrY5yTUicCicS31A1WpHyqouPE66TKqcmxY4PyAekdFabw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzow2ORRzgpfHIGAKIAWlXmACUUqvu2ibE2CAoQqVATmF9WSGClWM7sJtQXTJibVhz4nsFElxLYAcibg/640?wx_fmt=jpeg)
这个专栏已经完结，作为完结礼物，我特别花了很长的时间给大家整理了一些业界公开的相关读物，一共有52篇，供想深入阅读的人继续学习。有论文形式，网络文章，演示文稿，还有一些经典书籍，我都都整理在专栏尾篇，记得要看。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzow2ORRzgpfHIGAKIAWlXmbqUN580nQF59rcSSulUbGXED82I0xLIT5M5o4mF7p74AGycxpd5d2w/640?wx_fmt=jpeg)
我的专栏现在正在做拼图活动，**原价68元，拼团价58元，限时拼团3天！**
凡购买用户，还可得到我整理的【52篇推荐系统相关文章】+【推荐系统知识框架图】。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkzow2ORRzgpfHIGAKIAWlXmr1Dg1zRTo6ACMxftVUL0yibz3uVfT5UNh0WUlA83xzlfNFhvf2iaunVg/640?wx_fmt=jpeg)
**刑无刀是谁？**
我是刑无刀，本名陈开江，现在是链家网资深算法专家，从事算法类产品的研发。在这之前我曾任新浪微博资深算法工程师，考拉FM算法主管。从业8年时间，我的工作和研究范围始终没有超出推荐系统。
这些年，我曾服务过创业公司、传统大公司和大型互联网公司，这些经历也让我见证了大大小小、形状各异的推荐系统的构建过程。**又因为我基本都从0到1参与了这些公司的推荐系统，所以我也清楚这中间都有哪些坑。**
**扫码立即订阅**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzow2ORRzgpfHIGAKIAWlXmibDIx6EPJn0M8cnWN5KOoDSduNsU9Xhr4op3GjeKS99pGVrKsKlibFPQ/640?wx_fmt=png)
