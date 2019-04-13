
# Fashion-MNIST数据集发布一周年，论文引用量超250篇 - Paper weekly - CSDN博客


2018年10月08日 13:51:46[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：347


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

**作者简介：**肖涵博士，**Fashion-MNIST 数据集作者**。现为腾讯 AI 部门高级科学家、德中人工智能协会主席。他曾于 2014-2018 年初就职于位于德国柏林的 Zalando 电商，先后在其推荐组、搜索组和 Research 组，担任高级数据科学家。

肖涵在德国慕尼黑工业大学计算机系取得了博士（2014）和硕士学位（2011），博士方向为对抗机器学习。这期间曾在国立台湾大学资工所做访问学者（2013）。赴德前，他曾在北京邮电大学取得了信息通信学士学位（2009）。肖涵所创办的德中人工智能协会如今拥有 400 余名会员，致力于德中两国 AI 领域的合作与交流。

**个人微信：**hxiao1987
**Blog：**https://hanxiao.github.io

# 引言

距离我 2017 年 8 月发布**Fashion-MNIST 数据集**已经有一年的时间了。正如我在其 README 中写到的那样：**Fashion-MNIST 的目标是要替代经典数字 MNIST 数据集，帮助研究者更好的评测和理解机器学习算法。**在过去的一年里，我看到 ML/AI 社区朝这个方向不断前进着。越来越多的研究人员、工程师、学生和爱好者和初学者都爱上了这个数据集。

**数据集项目地址：**
https://github.com/zalandoresearch/fashion-mnist

今天你可以在网上找到关于 Fashion-MNIST 数以千计的讨论、代码和教程。在**Github**[1]，Fashion-MNIST 已经获得了超过 4100 颗星星，被引用在 400 多个代码仓库中，1000 多条 commits 里和 7000 多个代码片段中。在**Google Scholar**[2]中，超过 250 篇学术论文使用或引用了 Fashion-MNIST 数据集。甚至连来自 AAAS 的「科学」（Science）杂志都引用了这个数据集。而在**Kaggle**[3]上，这个数据集在数千个数据集中是名列前茅，并附有 300 多个 Kernels。**目前，所有主流的深度学习库都内置了这个数据集：你只需要一行 `import`，就可以直接使用它。**

毫无疑问，Fashion-MNIST 是一个非常成功的项目。在这里，我会对它在过去一年里所取得的重要成就做一个总结。

# 我为什么会做Fashion-MNIST？

2017 年 8 月的一天，那时我还在 Zalando Research 部门工作。我的上司让我调研一把生成对抗网络（GAN）并实现其中一两个模型，借机熟悉一下。我很快就写完并在 MNIST 数据集上做了测试。但我发现在 MNIST 上的生成结果似乎总是不错，无论你用的网络复杂度如何，参数设置如何。

**或许因为 MNIST 过于简单？****于是，我决定增加一些难度，把公司数据库中的衣服图片扔到网络里去。**很显然，这就需要我重新写一个用于输入处理的 pipeline：把图片居中、调整白平衡、缩放等等，最后再载入到模型中。最终，我决定将图片做离线处理后，存储为 MNIST 的格式，省去了自己写 data loader。

**这就是 Fashion-MNIST 最初的第一个版本**，随后的几天里，我一直在玩这个新数据集，利用它做各种实验。同时，我不断改善它的图片质量，并在 Tensorflow，Keras 中为它提供更通用的 API 接口。最终，我把它发布为公司内部的一个开源项目（inner source project）。

在我的前同事 Kashif Rasul 和 Lauri Apple 的鼓励下，我们最终决定把这个数据集发布到 Github 和 arXiv 上。为了在国际上扩大影响力，我也将数据集的说明翻译成了中文和日文。

剩下的事情可能大家都已经知道了：这个数据集最先被 Reddit 下的 r/MachineLearning 板块讨论，随后迅速在 HackerNews，Github，Twitter 和 Facebook 上传播开来。连续三天内，Fashion-MNIST 成为 Github 上最热门的开源项目之一。**几天后，甚至 Yann LeCun 本人都在他的 Facebook 主页上发帖鼓励大家尝试这个新数据集。**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pnFSb3UeiaNQFT3RYVFJ6eHePTCsAz6Tib83OfkQl2CTESImygSbqukOg/640)

在这次的经历中，我想特别感谢我的前同事 Lauri Apple 的支持。Lauri 是一位长期致力于开源事业的女性。在一开始，我的想法（采用 Fashion-MNIST 替换 MNIST）遭到其他人的嘲笑和不理解，他们认为圈子里根本不会在乎这个数据集。这个时候，Lauri 站了出来告诉我：**那些真正做出改变的人永远是那些相信改变的人**（*people who actually make change are the ones who believe that change is possible*）。

# Fashion-MNIST对学术圈的贡献

在我今天写这篇总结的时候，已经有**超过 260 篇学术论文**在实验环节引用或使用了 Fashion-MNIST 数据集（根据截止至 9 月 18 日的**Google Scholar结果**[4]）。考虑到数据集仅仅发布 1 年，这个数量还是非常可观的。经过一番筛选之后，我留下了其中 247 篇放在了一个**Google 表格里**[5]，大家可以自行查阅。

**那么这 247 篇论文出自谁之手，在哪发表，又研究了哪些问题？**在下面，我会向大家做一个全面的介绍。

**顶级AI研究机构青睐Fashion-MNIST**

下图给出了 247 篇论文中按照机构的统计图。如果一篇论文由多个机构合作完成，则在统计时每个机构都加 1。而如果一篇论文中多个作者来自同一机构，则这一机构只加 1。并且，我还把同组织的机构合并在一起，比如 Google Research，Google Brain，Deep Mind 合并为了 Google；马普（Max Planck Institute）旗下的信息所、智能所和量子光学所合并为 Max Planck Institute。为了更加清晰的展示，论文数量小于 3 机构没有在这里展示，但大家仍然可以在上述 Google 文档[5]中查看全部机构的统计列表。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pWN22tEQGGlYg2ICLbMfzswQgAxLbe9Aa7ziaAEEXzyic08XnX9hw3Qfw/640)

在这个图表中，我们不难发现来自北美、亚洲和欧洲地区顶尖的 AI 研究机构。在它们之中，**Google 以 9 篇使用 Fashion-MNIST 的论文位居榜首**。其次是英国剑桥大学，7 篇；IBM Research、加拿大蒙特利尔大学、北京大学和加州大学洛杉矶分校以 6 篇并列第三。中国方面，除北京大学外，中科院、南京大学、清华大学及京东也非常青睐 Fashion-MNIST 这个数据集。

而在工业界，除了已经列出的 Google，IBM Research 和 Microsoft，在完整列表中可以找到更多大家耳顺能详的公司 Facebook（2 篇）, Telefónica Research（2篇），Uber（1 篇），Apple（1 篇）, Samsung（1 篇）, 华为（1 篇）和 Twitter（1 篇）。同时，我还注意到有不少北美地区的机器视觉创业公司也在 Fashion-MNIST 数据集上做了非常有意思的研究，并发表了高质量的论文。

**美中两国在AI领域的竞争和合作**

下图展示了这些文章出自于哪些国家或地区。如果一个论文由多个国家或地区合作完成，则在统计时每个国家或地区时都加 1。而如果一篇论文中多个作者来自同一国家或地区，则这一国家或地区只加 1。为了更清晰的展示结果，论文数量小于 4 的国家或地区没有在这里展示，但大家仍然可以在这里[5]查看全部国家或地区的统计列表。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pk333TSnricqx04QMUmicnVI0MXjsavythVU7ICejLZtJVicoghUkIM5sg/640)

很明显，美国以 94 篇引用 Fashion-MNIST 的论文的绝对多数占据了榜首，其次是中国 44 篇。加拿大、英国德国的研究人员也显示了对这个数据集的浓厚兴趣，分列第 3、4、5 位。榜上前 5 名的国家贡献了大约 50% 的论文量。**总体来讲，共有来自 38 个国家的研究人员在他们的论文中使用了这个数据集。**

尽管中美之间贸易战的乌云笼罩，AI 领域竞争激烈，但我们仍然看到两国在 AI 研究领域中大量的合作。在过去的一年内，两国共有 10 篇合作的论文使用了 Fashion-MNIST 数据集，高于任何其他两国之间的合作。其次是英国和德国，共合作了 4 篇论文。完整的国家地区合作列表可以在这里[5]看到。

把政治偏见放在一旁，我非常愿意看到更多的国家与国家之间在 AI 领域的合作。这也是 Fashion-MNIST 的使命之一：增加整个社区的凝聚力和多元化。特别是今天，当 AI 技术已经越出象牙塔，国与国之间的交流合作变得极为重要。

**顶级会议青睐Fashion-MNIST**

当然，论文不是靠数量而是靠质量取胜的。那么这些论文都是在哪里发表的呢？他们的质量又如何呢？下图给出了按照会议和期刊的统计表。我只统计了过去一年中已经确定被正式录取的论文，而那些正在审核状态的论文并没有在这个图表中展示。完整的列表可以在这里[5]查看。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pnicW5pOZaoqqibNHnkgdNJSzQYGXjoATBTUd9MwW1IWMHFC0ckxYFVcA/640)

大部分的论文都是在 2018 年涌现出来的，尤其是在诸如 NIPS，ICLR，ICML 等顶会。**在 NIPS 2018 中，一共有 17 篇录用的论文使用了 Fashion-MNIST数 据集。**而 2017 年这个数字是：零。不过这显而易见，因为在 NIPS 2017 年的 5 月份的投稿截止时，Fashion-MNIST 还没有诞生。而在稍后一点的 11 月戒指的 NIPS 2017 Workshop 中，我们已经可以看到研究者在论文中使用了这个数据集。

除了在会议上发表，**也有很多毕业论文和顶级期刊中引用到了 Fashion-MNIST**，其中知名的期刊包括：**Journal of Machine Learning Research**（2 篇），*Neurocomputing*（2 篇），**Nature Communications**（1 篇）和*Science*（1 篇）。

**Fashion-MNIST 在「科学」杂志中被引用**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pRx7tG4twf1FB6IXvhIYFtdd4KaBc3wRJzyLVicnzNHBrIUtdeSCNppA/640)

这其中最让我骄傲的一篇论文引用，发表在 Science 科学杂志上。 在这篇题为*All-optical machine learning using diffractive deep neural networks*[6]的 Science 论文中，一个来自加州大学洛杉矶分校的团队建造了一个全光学的深度“神经”网络，并且用 3D 打印机打印了出来。这个光学深度神经网络功能上没什么不同，它能对 MNIST/Fashion-MNIST 的图像进行分类，**只不过它的分类速度是——光速！**

下图展示了 MNIST 和 Fashion-MNIST 模型上 5 个不同的物理层。这些层可以被 3D 打印出来，而层上面的每个点都有着不同的功能：或是允许光子通过，或是直接把光子反射回去，这就好比神经元之间的连接。在下图右侧，就是一个打印出的 3D 打印的全光学神经网络。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6p9KhrPR80Er9fDBB64Epxu59sQQWicO12niaGRiaIp2KBZ2NxK8iahzJCAg/640)

在实际分类时，这个 3D 打印的网络被放在如下的仪器中。通过判断在最后一层上光学信号最强的区域来获得分类结果。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pZDF4BbrOqkTPxgHGIUgcFLpibyAxMLn3mJ5jEJAeSe9G0udcvu8pQKw/640)

**GAN研究者青睐Fashion-MNIST**

生成对抗网络（GAN）近几年一直是深度学习领域的研究重点，尤其是在图像生成、图像处理等应用领域，GAN 经常能取得惊人的效果。也不难理解为什么 GAN 的研究员喜欢 Fashion-MNIST 这个数据集：**它比较轻量；不需要重新写 data-loader；却比 MNIST 包含更复杂的和多元的局部模式信息。**

对于图像 GAN 的研究者来说，在打造一个新的算法时，第一个测试就是在 MNIST 和 Fashion-MNIST 上进行测试。**下图展示了 247 篇论文按照关键词分类结果。**很多论文作者没有提供关键词，对于这些论文，我在读过 Abstract 和 Related Work 后，为这些论文手工总结了一些关键词。为了更清晰的展示结果，论文数量小于 5 的关键词没有在这里展示，但大家仍然可以在这里[5]查看全部关键词的统计列表。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6psnEicmicYYia6yXvyQuse1kqpXs9BI1ic3rFG578L87ib6W3Tj1RC7DaQicg/640)

**新的机器学习算法也喜欢使用 Fashion-MNIST 来做评测，例如胶囊网络（Capsule Networks）。**胶囊网络自从发表以来就收到很多的讨论：有不少人质疑胶囊网络从设计上是否为 MNIST 做了特别的优化，而这种结构是否在其他数据集上仍然有效。不说大的 ImageNet 数据集，胶囊网络是否在 Fashion-MNIST 上仍然有效呢？在过去的一年里，有 9 篇新的胶囊网络在论文实验中使用了 Fashion-MNIST 数据集。他们设计了更稳定的路由算法以确保空间特征能够保留住。

# Fashion-MNIST对社区的贡献

在我发布 Fashion-MNIST 几周后，我受邀到亚马逊柏林办公室做了一个演讲[7]。在问答环节，我被一个研究员问道，**是否担心 Fashion-MNIST 成为那些懒惰的研究员逃避现实问题的一个借口。**毕竟，他们现在可以说自己的算法有了份“双保险”（在两个 MNIST 上验证过了）。

这个社区从来没有让我失望，这些高质量的论文已经很好的证明了自己。而除了学术圈的论文，**这个社区还找到了 Fashion-MNIST 另一个重要用途：教学。**在网上你可以找到数以千计的讨论、代码和教程，尤其是在机器学习入门的第一讲。

Fashion-MNIST 增加了整个社区的多元化：它吸引了很多年轻的 AI（特别是女性）爱好者、学生甚至是艺术家和设计师。他们在 Twitter 上表示，第一眼看到这个数据集都会觉得非常可爱，因此很想动手实践。

在 2018 年 9 月，Google 在中国举办的开发者大会上，来自 Google 的演讲者使用 Fashion-MNIST 作为例子向在场的数百名参会者普及 Keras 深度学习库和机器学习技术。而就在前几天，2018 年 10 月，Google Colab 发布的**TPU 示例**[8]中，也使用了 Fashion-MNIST 作为演示的例子来展现 TPU 的超强算力。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnY0DaibdXT2IIriceMhENK6pGJrrIx5wXBaTT4YXadtHb8qfgPQKcoGgofUNdjC2nnSmECz5fGxzqQ/640)

# 总结

人工智能的进步需要来自整个社区的共同努力。我很欣慰的看到在过去的一年里，Fashion-MNIST 为这个社区做出了自己的贡献：**它不仅吸引了多元化背景的爱好者，也促进研究人员设计更严密更具说服力的实验。**所以无论你是研究员、学生、教授还是爱好者，无论你是想把 Fashion-MNIST 用在讲座、论文、研讨会、竞赛还是培训中，只要你喜欢，都可以去做。最重要的是享受技术带来的乐趣，让更多的人体验到机器学习和人工智能的魅力。

# 相关链接

[1]. https://github.com/search?q=fashion-mnist
[2]. https://scholar.google.com/scholar?hl=en&as_sdt=0%2C5&q=fashion-mnist&btnG=&oq=fas
[3]. https://www.kaggle.com/zalando-research/fashionmnist
[4]. https://scholar.google.com/scholar?hl=en&as_sdt=0%2C5&q=fashion-mnist&btnG=&oq=fas
[5]. https://docs.google.com/spreadsheets/d/1cGX7Juedn_KVUgjDk298v5uUjc_wPk930tKyEoZhTQM/edit?usp=sharing
[6]. http://innovate.ee.ucla.edu/wp-content/uploads/2018/07/2018-optical-ml-neural-network.pdf
[7]. https://www.slideshare.net/HanXiao4/fashionmnist-a-novel-image-dataset-for-benchmarking-machine-learning-algorithms
[8]. https://colab.research.google.com/github/tensorflow/tpu/blob/master/tools/colab/fashion_mnist.ipynb

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多论文解读：**

[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)

[ECCV 2018 | 从单帧RGB图像生成三维网格模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491465&idx=1&sn=a6871b72d460debf90d2daa0bed719c8&chksm=96e9c009a19e491f8c247f36a53bad0a6812c462f3fb5b2d9ae74b38be673b946b82a4b44330&scene=21#wechat_redirect)
[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)
[ECCV 2018 | 腾讯AI Lab提出视频再定位任务](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491347&idx=1&sn=e1f2cc16c9fcfcc5d2935118f09ee094&chksm=96e9c093a19e49855931cf621ec7f715c1d2dd5041e3343bc311aea2e5069ae03aaa1367e8f9&scene=21#wechat_redirect)
[KDD 18 | 斯坦福大学提出全新网络嵌入方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491364&idx=1&sn=afa577a1bebfd148ac20cde05a927e24&chksm=96e9c0a4a19e49b2458f451c6e201277c509905201a1f751b30fbfa1e2ba31bc2711b2ca1b7a&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。

📝**来稿标准：**
• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向）
• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接
• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志

**📬 投稿邮箱：**
• 投稿邮箱：hr@paperweekly.site
• 所有文章配图，请单独在附件中发送
• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通



🔍

现在，在**「知乎」**也能找到我们了
进入知乎首页搜索**「PaperWeekly」**
点击**「关注」**订阅我们的专栏吧


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 获取最新论文推荐


