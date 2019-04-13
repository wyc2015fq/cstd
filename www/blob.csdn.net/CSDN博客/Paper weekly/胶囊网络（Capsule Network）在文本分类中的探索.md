
# 胶囊网络（Capsule Network）在文本分类中的探索 - Paper weekly - CSDN博客


2018年04月05日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：4246


作者丨杨敏
单位丨中国科学院深圳先进技术研究院助理研究员
研究方向丨自然语言处理

文本建模方法大致可以分为两类：（1）**忽略词序、对文本进行浅层语义建模**（代表模型包括 LDA，EarthMover’s distance等）; （2）**考虑词序、对文本进行深层语义建模**（深度学习算法，代表模型包括 LSTM，CNN 等）。

在深度学习模型中，空间模式（spatial patterns）汇总在较低层，有助于表示更高层的概念（concepts）。例如，CNN 建立卷积特征检测器提取来自局部序列窗口的模式，并使用 max-pooling 来选择最明显的特征。然后，CNN 分层地提取不同层次的特征模式。

然而，**CNN 在对空间信息进行建模时，需要对特征检测器进行复制，降低了模型的效率**。正如（Sabouret al, 2017）所论证的那样，这类方法所需复制的特征检测器的个数或所需的有标签训练数据的数量随数据维度呈指数增长。

另一方面，**空间不敏感的方法不可避免地受限于丰富的文本结构**（比如保存单词的位置信息、语义信息、语法结构等），**难以有效地进行编码且缺乏文本表达能力**。

最近，Hinton 老师等提出了胶囊网络（Capsule Network）, 用神经元向量代替传统神经网络的单个神经元节点，以 Dynamic Routing 的方式去训练这种全新的神经网络，有效地改善了上述两类方法的缺点。

正如在人类的视觉系统的推理过程中，可以智能地对局部和整体（part-whole）的关系进行建模，自动地将学到的知识推广到不同的新场景中。

到目前为止，并没用工作将 Capsule Network 应用于自然语言处理中（e.g., 文本分类）。

**我们针对****Capsule Network****在文本分类任务上的应用做了深入研究**。对于传统的分类问题，Capsule Network取得了较好性能，我们在 6 个 benchmarks 上进行了实验，Capsule Network在其中 4 个中取得了最好结果。

更重要的是，**在多标签迁移的任务上（fromsingle-label to multi-label text classification），****Capsule Network****的性能远远地超过了 CNN 和 LSTM**。

我们的工作已经发表在 arXiv上，论文名为**Investigating Capsule Networks with Dynamic Routing for Text Classificationm**，更多细节可以点击**阅读原文**进行参考。我们会在论文发表后公开源码。

# 论文模型

文本主要研究胶囊网络在文本分类任务上的应用，模型的结构图如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8JjibUmqqeM5QUalLpZsSe3l8Q8iaD0tWaVJ7k7mHFn6ibCLEz8wAu9UHA/640)

其中，**连续两个卷积层采用动态路由替换池化操作**。动态路由的具体细节如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8GNaANDJwLaib2LH9Y1CueAz2d9RPeN2LI6ck04tWriaQU4n6nz0eqAKw/640)

在路由过程中，许多胶囊属于背景胶囊，它们和最终的类别胶囊没有关系，比如文本里的停用词、类别无关词等等。因此，我们提出**三种策略有减少背景或者噪音胶囊对网络的影响**。

**1. Orphan 类别**：在胶囊网络的最后一层，我们引入 Orphan 类别，它可以捕捉一些背景知识，比如停用词。在视觉任务加入 Orphan 类别效果比较有限，因为图片的背景在训练和测试集里往往是多变的。然而，在文本任务，停用词比较一致，比如谓词和代词等。

**2. Leaky-Softmax**：除了在最后一层引入 Orphan 类别，中间的连续卷积层也需要引入去噪机制。对比 Orphan 类别，Leaky-Softmax 是一种轻量的去噪方法，它不需要额外的参数和计算量。

**3. 路由参数修正**：传统的路由参数，通常用均与分布进行初始化，忽略了下层胶囊的概率。相反，我们把下层胶囊的概率当成路由参数的先验，改进路由过程。 在 ablation test 中，我们对改进的路由和原始路由方法进行对比，如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8dRyQgBiciaJzt17HWp6icjj5DO1pf6FCLRX9lsdcrAoRfpO4H7tnty2Nw/640)

此外，**为了提升文本性能，我们引入了两种网络结构**，具体如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8f2VfYsAwWc0evF9Q7lnt64ZM4EVlJBJBe2z81qpq2DbuYjJd1z1WsQ/640)

# 实验

数据集：为了验证模型的有效性，我们在 6 个文本数据集上做测试，细节如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM89OIPTdPkfZA3MAic1aIx9W8XUJOds3Wsu1d4nxxFKicc9DH1DAQmK6Fg/640)

在实验中，我们和一些效果较好的文本分类算法进行了对比。由于本文的重点是研究 Capsule Network 相对已有分类算法（e.g.,LSTM, CNN）是否有提升，我们并没有与网络结构太过复杂的模型进行对比。实验结果如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8WoTI76iaP0EW19myxfSF5At9EneP99dt3weTWsWYSxZuVFS5etw2YnA/640)

此外，**我们重点进行了多标签迁移实验**。我们将 Rueter-21578 数据集中的单标签样本作为训练数据，分别在只包含多标签样本的测试数据集和标准测试数据集上进行测试。详细的数据统计以及实验结果如下图所示。

从表中我们可以看出，**当我们用单标签数据对模型进行训练，并在多标签数据上进行测试时，Capsule Network 的性能远远高于 LSTM、CNN 等**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8bXId7JaLm07b30yD8hCSptzic3hia5dQs92B2TGH3YqcFcoticaaU349w/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8q8o350ibvrDBL6ics3Ag9VxC0OO8BSab9G62YR36KWLibG90BOG2cBQIA/640)

此外，我们还做了 case study 分析，发现路由参数可以表示胶囊的重要性，并对胶囊进行可视化（此处我们主要可视化 3-gram 的结果）。

具体来说，我们删除卷积胶囊层，将 Primary Capsule Layer 直接链接到 Fully-connected Capsule Layer，其中 Primary Capsule 代表了 N-gram 短语在 Capsule 里的形式，Capsule 之间的链接强度代表了每个 Primary Capsule 在本文类别中的重要性（比较类似并行注意力机制）。

由图我们可以看出，对于 Interest Rate 类别，months-interbank-rate 等 3-grams 起着重要作用。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl3miacsukQl2KD81mEGzzM8nqmjbjqq1ibrqP8SJpsHzibAiazElo3EcoVXZV3S8EJRqsDGicUopqKqLg/640)

# 致谢

感谢 jhui 和苏剑林，他们的文章启发了我们的工作。感谢 naturomics 和 gyang274 的开源代码，让我们的开发过程变得高效。

[1] https://jhui.github.io/2017/11/14/Matrix-Capsules-with-EM-routing-Capsule-Network/
[2] https://spaces.ac.cn/archives/4819
[3] https://github.com/bojone/Capsule
[4] https://github.com/naturomics/CapsNet-Tensorflow
# 主要参考文献

[1] Dynamic Routing Between Capsules
[2] Matrix Capsules with Em Routing


![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击以下标题查看相关文章：**

揭开迷雾，来一顿美味的「Capsule」盛宴

[再来一顿贺岁宴 | 从K-Means到Capsule](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487603&idx=1&sn=f2066b47d1ff6a323bffe043d1731750&chksm=96e9cff3a19e46e56b02cc6175d16dc4bc6572684081a1ab5756983f1ea638c0136dc0f5ef8d&scene=21#wechat_redirect)
[三味Capsule：矩阵Capsule与EM路由](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487682&idx=1&sn=97419d2cfb3a62ce8881358b897b2956&chksm=96e9cf42a19e4654933891edb47de676ec87f46e9fbaf02c99fc32150b00595549c8bc20f218&scene=21#wechat_redirect)


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl3miacsukQl2KD81mEGzzM8Cgx5yOXlnpQG8scpa6QGAllvA20iaTZy7ibRuhSXlA8Ts5yFTX9FCf5g/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**

**[投稿须知 | 让你的文字被很多很多很多人看到](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488131&idx=2&sn=a13f40787d3cb8893f141ac2b7318de7&chksm=96e9cd03a19e441566238e10e29eb684e5724d4c6e908792cc5658905e7f6d5d416faace8600&scene=21#wechat_redirect)**

我是彩蛋
**解锁新功能：热门职位推荐！**

PaperWeekly小程序升级啦

**今日arXiv√猜你喜欢√****热门职位****√**

找全职找实习都不是问题
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**职位发布**
请添加小助手微信（**pwbot02**）进行咨询

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)



**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 查看论文


