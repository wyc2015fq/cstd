
# 再来一顿贺岁宴 | 从K-Means到Capsule - Paper weekly - CSDN博客


2018年02月12日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：305


作者丨苏剑林
单位丨广州火焰信息科技有限公司
研究方向丨NLP，神经网络
个人主页丨kexue.fm

在本文中，我们再次对 Capsule 进行一次分析。

整体上来看，Capsule 算法的细节不是很复杂，对照着它的流程把 Capsule 用框架实现它基本是没问题的。所以，困难的问题是理解 Capsule 究竟做了什么，以及为什么要这样做，尤其是 Dynamic Routing 那几步。

为什么我要反复对 Capsule 进行分析？这绝非单纯的“炒冷饭”，而是为了得到对 Capsule 原理的理解。

众所周知，Capsule 给人的感觉就是“似乎有太多人为约定的内容”，没有一种**“虽然我不懂，但我相信应该就是这样”**的直观感受。我希望尽可能将 Capsule 的来龙去脉思考清楚，使我们能觉得 Capsule 是一个自然、流畅的模型，甚至对它举一反三。

在[揭开迷雾，来一顿美味的「Capsule」盛宴](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487194&idx=1&sn=1be48dd9a45f7f3a35024fdce55f2369&chksm=96e9d15aa19e584c0b4815d6b5791a7ab859049116f8500a08c454c55457f9489cf2adf4ab93&scene=21#wechat_redirect)中，笔者先分析了动态路由的结果，然后指出输出是输入的某种聚类，这个“从结果到原因”的过程多多少少有些望文生义的猜测成分。

**这次则反过来，****直接确认输出是输入的聚类，然后反推动态路由应该是怎样的，其中含糊的成分大大减少**。两篇文章之间有一定的互补作用。

# Capsule框架

![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjtM6KodGrWHjibZr71FC8FlAgkXfI1fDVCcGnmWljTlClC0vDAzichFkQ/640?wxfrom=5&wx_lazy=1)
△图1：Capsule框架的简明示意图

与其说 Capsule 是一个具体的模型，倒不如说 Capsule 是一个建模的框架，而框架内每个步骤的内容，是可以自己灵活替换的，而 Hinton 所发表的论文，只是一个使用案例。

这是一个怎样的框架呢？

**特征表达**

Capsule 模型中，**每个特征都是用一个向量（即 Capsule，胶囊）来表示的**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjoxV6oyYuVVTZDZ7HiaCAZ4HgRJnc6s0mshN2OP0P64o0Gd4fTaDof6A/640)
△图2：Capsule的每个特征都是向量，并且通过聚类来递进

当然，对于关注新闻的读者来说，这已经不是什么新消息。可能读者会有疑问：用向量来表示特征有什么稀奇的，本来神经网络的特征输入不就是一个向量吗？

原来神经网络（MLP）的每一层输入是一个向量![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjWfHIqtGqjMRUWRA5vPTZGmSLjhC2rASria4Cg7dsml2abkFh01d2ZGg/640)，然后输出是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjo3pFMzqxTbERic1ykbfuGsb0nwfeTUiczib3OmP56EaF90SevNuSxhZZg/640)，我们就将*x*的每一个分量都看成一个特征，那么每个特征都是标量了。

而所谓的特征向量化后，那么每一层的输入变成了![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj4w6v3uUkbUYgQQ8ibMC3aFL4x5jhCIibMMicl0zW1C05TrWwRMBl1NVfw/640)，然后输出是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjJxhhNdibOXSNvxLmSKfbaVLsNKBVQAibGvjZ9vSbtpETyRdwTDmcRe3Q/640)，这时候的输入*x*也看成是 n 个特征，但每个特征都是一个 dx 维向量；输出*y*则看成是 k 个特征，每个特征是一个 dy 维向量。

换一个角度看，其实就是说 MLP 每一层的输入输出由单个的向量变成了向量的集合（矩阵）。

或者我们可以将它换一个名称，叫做“特征的分布式表示”。也许有读者看到了“分布式表示”，会想起 NLP 中的词向量。

没错，词向量一开始确实叫做“分布式表示”（Distributed Representation），而笔者看到 Capsule 的这一特点，第一反应也就是词向量。

我们可以用词向量代替 one hot 来表示一个词，这样表达的信息就更为丰富了，而且所有的词都位于同一向量空间，方便后续处理。

此外，事实上图像中早也有这样的例子，众所周知彩色图像一般有 RGB 三个通道，每个通道 256 个选择，所以一共可以表达 256 的三次方，即 16777216 种颜色（约 1700 万）。

**为什么不直接用 1700 万个数字来分别表示这 1700 种颜色，而要分开 3 组，每组 256 个数字呢？**

**这其实也是一种分布式表示**，这样可以更好地表达色彩的多样性。比如红色的相近颜色是什么色？也许有人说橙色，也有人说紫色，也有可能是粉红，单一一个数字难以表达多种的相似性，而分组后则可以。

更进一步说，**我们在对图像不断进行卷积操作时，所得结果的通道维度，其实就是图像特征的一种分布式表示了**。

**特征组合**

Capsule 的第二个特点，是**通过聚类来组合特征**。

**组合与表达**

通过将底层特征组合为上层的特征，是跟我们的认知规律是相符的。在NLP中，我们有“字-->词-->句-->段”的层层组合；在图像中，我们也有“点-->线-->面-->体”的层层组合。

面对新事物（上层特征），我们总会将它分解为我们熟悉的一些事物（底层特征），然后脑海里将这些事物映射到这个新事物（特征组合）。

**对于我们来说，这个分解和组合的过程，不一定有什么目的，而只是为了用我们自己的方式去理解这个新事物（在大脑中形成良好的特征表达）**。

这也就能理解 Hinton 诟病深度学习、发展 Capsule 的原因之一了，因为他觉得现在深度学习的模型针对性太强，比如 MNIST 分类模型就只能做单个数字的识别，多个数字的识别就要重新构建数据集、重新设计并训练模型。

而事实上，我们的根本目的并不是单纯地做任务，而是通过任务形成良好的、普适的特征表达，这样才有可能形成真正的人工智能。

**特征间聚类**

那么，怎么完成这个组合的过程呢？试想一下，两个字为什么能成为一个词，是因为这两个字经常“扎堆”出现，而且这个“堆”只有它们俩。这就告诉我们，**特征的聚合是因为它们有聚类倾向，所以 Capsule 把聚类算法融入到模型中**。

要注意，我们以前所说的聚类，都是指**样本间的聚类**，比如将 MNIST 的图片自动聚类成 10 个类别，或者将 Word2Vec 训练而来的词向量聚类成若干类，聚类的对象就是一个样本（输入）。

而 Capsule 则设想将输入本身表示为若干个特征向量，然后对这些向量进行聚类（**特征间的聚类**），得到若干中心向量，接着再对这些中心向量聚类，层层递进，从而完成层层抽象的过程。这是一种特征间的聚类。

现在问题就来了。既然是聚类，是按照什么方法来聚类的呢？然后又是怎么根据这个聚类方法来导出那个神奇的 Dynamic Routing 的呢？后面我们会从K-Means出发来寻根问底，现在让我们先把主要思路讲完。

**特征显著性**

通过特征的组合可以得到上层特征，那如何对比特征的强弱程度呢？

**Capsule 的答案是：模长**。**这就好比在茫茫向量如何找出“突出”的那个？只需要看看谁更高就行了**。因此通过特征向量的模长来衡量它自己的“突出程度”，显然也是比较自然的选择。

此外，一个有界的度量也是我们希望的，因此我们对特征向量做一个压缩：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjETLMJg44Q6K5Bvicat6LeNx4gtVaZQ8PUUT81FiaJ8ibMaHu96XzC0zdQ/640)

压缩的方案并不唯一，这里就不展开了。**不过我在实验过程中，发现将 1 替换为 0.5 能提升性能**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjQLGjlAb01KzOPAibicMWfr7dcoQTIP63p1KwcELliaiabhPluic3bIMHCfw/640)
△图3：Capsule通过特征向量的聚类，来刻画特征的组合特性

为了突出模长的这一含义，也需要在设计模型的时候有所配合。如图，尽管*v1*所代表的类所包含的特征向量*u1,u2,u4,u8*的模长均比较小，但因为成员多（“**小弟多**”），因此*v1*的模长也能占优（“**势力大**”）。

这说明，**一个类要突出，跟类内向量的数目、每个类内向量本身的模长都有关联**。后面我们也会看到 Capsule 是如何体现这一点的。
# K-Means新探

既然本文不断强调 Capsule 是通过聚类来抽象特征的，那么就有必要来细谈一下聚类算法了。Capsule 所使用的聚类算法，其实是 K-Means 的变种。

聚类算法有很多，理论上每种聚类算法都是可能的，然而要将聚类算法嵌入到 Capsule 中，还需要费上一点周折。

**聚类目标**

**K-Means 聚类本质上是一种“中心聚类方法”——聚类就是找类别中心**。为了定义中心，我们就需要一个相近程度的度量，常用的是欧氏距离，但这并不是唯一的选择。

所以这里我们干脆**在一个****更加一般的框架下介绍 K-Means**：K-Means 希望把已有的数据*u1,u2,…,un*无监督地划分为*k*类，聚类的方法是找出*k*个聚类中心*v1,v2,…,vk*，使得类内间隔最小：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjuJoENjhLBd9SzKPzdEt3DjugpOorUMdCsh2DsyDA6FicMwguXNh28iag/640)

这里*d*代表了相近程度的度量，所以这个式子的意思很简单，就是说每个*ui*只属于跟它最相近的那一类，然后将所有类内距离加起来，最小化这个类内距离：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj1GNEeHYnKxiaOCiaagTFSqbItGzibtxsjYWZS6PK1XndicyzXGDrftEyCA/640)

显然，聚类的结果依赖于*d*的具体形式，这其实就告诉我们：无监督学习和有监督学习的差别，在于我们跟模型“交流”的方法不同。

有监督学习中，我们通过标注数据向模型传达我们的意愿；在无监督学习中，我们则通过设计适当的度量*d*来完成这个过程。

**求解过程**

怎么去最小化*L*来求出各个中心呢？如果读者不希望细细了解推导过程，可以跳过这一节，直接看下一节。

因为*L*中有 min 这个操作，所以直接求它的梯度会有困难（不是不能求，而是在临界点附近不好处理），事实上有很多类似的问题没能得到很好的解决，都是因为它们的 loss 中有 min。

然而，这里我们可以“软化”这个*L*，使得它可以求导。因为我们有一个很漂亮的公式：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjuAugD8vj5lweniauqND7V8ajppzXH6iav5GGWBWzSGscA0od424zOzgw/640)

如果取*K*=1，显然括号里边就是 softmax 的分母，这也就是 softmax 的由来了——它是“soft”加“max”——“软的最大值”。

而我们有：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj4aTB6ZGibujpR0s77Mtjicm89kRO3EyhfG1edqUic68ufFFc5bpYLSUIw/640)

因此我们就得到：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjdzg5e6MB3yibvdaq499X5CSuz9Htl5zLbLS1oRd5icERt0LBYArobhgw/640)

现在这个近似的 loss 在全局都光滑可导了，因此我们可以尝试求它的梯度。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjh01KC5qDQVN7wuibC7cia6MCgPy6iaeNuguKibSZKoHsN1hMX1JmAr5EQA/640)

这里：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjYtnGwhxRv708Bia1vDzicZczfVpVbFuHWXwBqIgDlRC09gVmyHMQYBOQ/640)

我们已经指明了是对*j*所在的维度来归一化。为了求出一个极小值，我们希望让*∂L/∂vj*=0，但得到的方程并不是简单可解的。因此，可以引入一个迭代过程，假设![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjRKbw55erQKECzPHibDbQtzw8Yk1ePvNCFibWctupTCBibg1sW34kgYo2w/640)是*vj*的第*r*次迭代的结果，那么我们可以让：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjRicVqzlZERg65BWKCUPCJS8o4ZnUVA3WHUeZNcSbf4vQkvibQM4JcbuQ/640)

如果可以从上述方程解出![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj9ubUDDNXl4qkUL7d3em1hOAnJyicK20nPp3YAOQwFN2x8fpuXbVrshg/640)，那么就可以从中得到一个迭代格式。

**欧氏距离**

现在就可以把我们选择的度量代入（8）式进行计算了。我们可以看一个最基本的例子：![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj46uy1sPQzKvflA6jxm8Biay5990YUL7g0icMhKLmNpAYxkzclOwicLib5g/640)，这时候就有：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjeXhItXJtrCIqG9ziaPDfkVviaBV37vUcoVsXwrdRqBJyNGGzNojrwLRA/640)

从而我们可以解出：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjbiabCytO429WnF96g4Nc1VoyC5EibmtgmFmicCbZW7uo1ZiccBgVUvVib7w/640)

如果取*K*→+∞，那么![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj8TlV8iabdroiaOfrUVqvITOibQxHMchyzUL0JYKT4nPVdn0A4zVHmS5yA/640)非 0 即 1，所以上式就是说（读者可以自己证明一下）![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj1RRibRKHEZyf1ibKib9kF40LWM8exGt4ZuvIjVnuEOPmGQZ7Wrq4aT9qA/640)是距离![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjRKbw55erQKECzPHibDbQtzw8Yk1ePvNCFibWctupTCBibg1sW34kgYo2w/640)最近的那些*ui*的平均值。

这就得到了我们平时说的 K-Means 聚类算法。

**内积相似度**

**欧氏距离并不适合用在 Capsule 中**，这是因为欧氏距离得到的中心向量是类内的向量的平均，这样类内向量越多，也不会导致中心向量的模越长，**这不满足我们前面说的“小弟越多，势力越大”的设计**。

什么距离比较适合呢？在论文**Dynamic Routing Between Capsules**中有一段话：

> The initial coupling coefficients are then iteratively refined by measuring the agreement between the current output vjvj of each capsule, jj, in the layer above and the prediction û j|i made by capsule ii.


> The agreement is simply the scalar product aij=vj⋅u^j|i ...

对应到本文，大概的意思是用内积⟨ui,vj⟩作为相似度的度量，也就是说，d(ui,vj)=−⟨ui,vj⟩。但仔细思考就会发现问题，因为这样的*d*是无下界的。

无下界的函数我们不能用来做 loss，所以我一直被这里困惑着。直到有一天，我觉得可以将*vj*先归一化，然后再算内积，这样一来实际上是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjRYqvgRtYqGL1Ljaj2te1c7R2WovA97Xzr29iajJy7jdCODr6M0CiaLmw/640)

现在对于固定的*ui*，不管*vj*怎么变，*d(ui,vj)*就有下界了。所以这样的*d*是可以用来作为 loss，代入（8）式算，最终得到的结果是：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjLoufvCJEaaLz49qWCSh0wP37O8ic0B7N2OjSYrZXhcvUACia4RhPib4Gw/640)

注意这结果只能说明![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj1RRibRKHEZyf1ibKib9kF40LWM8exGt4ZuvIjVnuEOPmGQZ7Wrq4aT9qA/640)和![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj2IanLaticeXqM4oiab0P7wXcwcQH3sXaBcUvuXu1XKapaICgM7u18kYw/640)的方向是一样的，但不能说明它们两个是相等的。然而，我们确实可以简单地取：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjxRZa46yZJj7t5uR8KDqqjqUPgnlAOjXSMQjCwVJGoGdpahZqPJEtDw/640)

如果取*K*→+∞ 的极限，那么就是说![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj1RRibRKHEZyf1ibKib9kF40LWM8exGt4ZuvIjVnuEOPmGQZ7Wrq4aT9qA/640)是距离![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjRKbw55erQKECzPHibDbQtzw8Yk1ePvNCFibWctupTCBibg1sW34kgYo2w/640)最近的那些*ui*的和。

由于现在是求和，就可以体现出“小弟越多，势力越大”的特点了。**注意，这里和欧氏距离那都出现了“最近”，两个最近的含义并不一样，因为所选用的****d****不一样。**

# 动态路由

经过漫长的准备，Dynamic Routing 算法已经呼之欲出了。

按照第一部分，我们说 Capsule 中每一层是通过特征间聚类来完成特征的组合与抽象，**聚类需要反复迭代，是一个隐式的过程。我们需要为每一层找到光滑的、显式的表达式：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjZP3nagd5LZR0O0CzicK17CDKKw2kvFUZ9SStbrgwfZnMlIVL1kZHlIQ/640)

**才能完成模型的训练。动态路由就是通过迭代来写出这个（近似的）显式表达式的过程。**

**基本步骤**

假设 Capsule 的输入特征分别为*u1,u2,…,un*，然后下一层的特征向量就是*v1,v2,…,vk*，它就是前一层*n*个向量聚为*k*类的聚类中心，聚类的度量是前面的归一化内积，于是我们就可以写出迭代过程：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjUXnliav1gnBlKF63R7p72AHsj6TcxnRDjqsD9aWvwY2ZuvhWbTtceaA/640)

这个版本是容易理解，但由于存在 arg⁡max 这个操作，我们用不了梯度下降，而梯度下降是目前求模型其他参数的唯一方法。为了解决这个问题，我们只好不取*K*→+∞ 的极限，取一个常数*K*>0，然后将算法变为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjpv7b1OSOf708pKh9pJA4e41pNsnNNOF5O7MNicUgYWHmGnFldNgcPOA/640)

然而这样又新引入了一个参数*K*，咋看上去*K*太大了就梯度消失，*K*太小了就不够准确，很难确定。不过后面我们将会看到，直接让*K*=1 即可，因为*K*=1 的解空间已经包含了任意*K*的解。最终我们可以得到：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjCI3V13ovEcW6pETtsnb3WMVRPq8Uicic5ZWgZQKkM90AOu9h7OxVuAMA/640)

有意思的是，最后导出的结果，不仅跟 Hinton 的原始论文**Dynamic Routing Between Capsules**有所出入，跟我前一篇介绍也有出入。

**其中，最明显的差别是在迭代过程中用 vj/‖vj‖ 替换了****squash****(vj)，仅在最后输出时才进行 squash**。**实验表明这有助于提升特征的表达能力**，它在我的前一文的数字实验（单数字训练，双数字预测）中，能达到 95% 以上的准确率（原来是 91%）。

**三种症状**

这样就完了？远远还没有。我们还要解决好几个问题。

**1. 如何做好类别初始化？**因为聚类结果跟初始化有关，而且好的初始化往往是聚类成功的一大半。现在我们要将聚类这个过程嵌入到模型中，作为模型的一部分，那么各个![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj0hJaKjJMI9rFHIRECUILdQUK6VtUVZqQe5YEMprmqQFgRfWPBIldqA/640)应该怎么选取呢？

如果同一初始化，那么无法完成聚类过程；如果随机初始化，那又不能得到确定的聚类结果，就算类中心向量不变，但是类的顺序也可能变化。

**2. 如何识别特征顺序？**我们知道，聚类的结果跟样本的顺序是无关的，也就是说，如果将输入向量的顺序打乱，聚类的结果还是一样的。

对于样本间的聚类，这是一个优点；然而如果是特征间的聚类，那么就有可能不妥了，因为不同顺序的特征组合可能代表不同的含义（就好比词序不同，句子含义也会不同），如果都给出一样的结果，那么就丧失了特征的序信息了；

**3. 如何保证特征表达能力？**动态路由将上层 Capsule 作为底层 Capsule 的聚类结果，每个类可能包含多个特征向量，但如果仅仅用类中心向量整个类的整体特征（上层特征），会不会降低了上层 Capsule 的特征表达能力？

**一个对策**

**有意思的是，以上三个问题都可以由同一个方法解决：加变换矩阵**。

首先，为了模型的简洁性，我们将所有*ui*的和平均分配到每个类中作为![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj0hJaKjJMI9rFHIRECUILdQUK6VtUVZqQe5YEMprmqQFgRfWPBIldqA/640)。那怎么分辨出各个不同的类呢？我们在输出到每个类之前，**给每个类都配一个变换矩阵****Wj**，用来分辨不同的类，这时候动态路由变成了：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjgz9QSVqvOnGHnDdcuA1LjefSELqKpn7AP7Wia6oP855pCKQkNncuIug/640)

这就是我前一篇介绍中所说的**共享权重版的 Capsule**。细细斟酌就会发现，引入训练矩阵*Wj*是个非常妙的招数，它不仅让聚类算法在同一初始化时仍能分辨出不同的类，而且通过*Wj*可以改变*ui*的维度，从而也就改变了聚类后的中心向量的维度，这样也就能保证中心向量的特征表达能力。

此外还有一个好处，那就是⟨*Wjui*,*Kvj*⟩=⟨(*KWj*)*ui*,*vj*⟩，也就是说它相当于把前面的参数 K 也包含了，从而我们可以放心设*K*=1 而不用担心准确性不够——如果有必要，模型会自己去调整*Wj*达到调整*K*的效果。

现在只剩下最后一个问题了：识别输入特征的顺序。跟识别每一个类一样，我们也可以**给每个输入都配一个变换矩阵****W̃i**，用来分辨不同位置的输入，这样一来动态路由变为：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjLiaGy28UyPYUWUckdpc8ERcPjWLQ07XMJOEdsiapPZJRaEica6vNlX68Q/640)

如果觉得这样太累赘，那么可以把*WjW̃i*替换成一个整体矩阵*Wji*，也就是对每对指标 (i,j) 都配上一个变换矩阵，这样的好处是整体更简单明了了，缺点是矩阵数目从*n+k*个变成了*nk*个：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qj4N9G1xCiavA1tU9Tuazbl4xpXzg5mA2Raic95U3ScNkJoL8VnVE2ibdag/640)

**这便是全连接版的动态路由**。然而并不是每次我们都要分辨不同位置的输入，对于变长的输入，我们就很难给每个位置的输入都分配一个变换矩阵，这时候共享版的动态路由就能派上用场了。总的来说，全连接版和共享版动态路由都有其用武之地。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk3vFpelLercnlBUDG0ia2qjOU2fEG3lsWOIFAPbYZ87GpRkY8rCd8ibY2SG5icl5GElG8hwyp7VYG8g/640)
△图4：Capsule的变换矩阵可能的所在之处

# 结语

笔者通过这两篇“浩浩荡荡”（哆里哆嗦）的文章，来试图解读 Hinton 大力发展的 Capsule 模型，然而作者水平有限，其中不当之处，还请读者海涵。

**个人认为，Capsule 的确是新颖的、有前景的的研究内容。也许它不一定（但也是有可能的）是未来的发展方向**，但细细品味它，仍足以让我们获益良多。

Hinton 大胆地将聚类的迭代过程融入到神经网络中，因此诞生了 Capsule，**那是不是说，可以考虑将其他比较直观的算法也融入到里边，从而造就其他有意思的玩意？**让我们拭目以待。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkvSxCRgqK914dI363k7VWmXJBVCNtexC0iaXJXUohTRgBpX4wGrDNYNpPB6SrZHiccz24zYcN2th2Q/640?)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****榜 单 公 布****\#**

**[2017年度最值得读的AI论文 | NLP篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487348&idx=1&sn=8ee8bf57418342a419fe73829cb14e75&chksm=96e9d0f4a19e59e288dcb105bd90b1e13f419ee7268ac69eba7cd6dac12e2e64aa84c56e5c07&scene=21#wechat_redirect)**
**[2017年度最值得读的AI论文 | CV篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487362&idx=1&sn=980153481f88ba5d6ba2929fd08240f3&chksm=96e9d002a19e5914f845973111b7056d24a28a8f7932479178c9cfcdb10c05c3284d5c0eb602&scene=21#wechat_redirect)**

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
请添加小助手微信（**pwbot01**）进行咨询

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)



**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)
▽ 点击 |阅读原文| 进入作者博客


