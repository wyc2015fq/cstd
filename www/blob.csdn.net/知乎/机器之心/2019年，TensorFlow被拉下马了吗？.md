# 2019年，TensorFlow被拉下马了吗？ - 知乎
# 



> AI领域里技术的发展总是很快，深度学习框架这样的工具流行的趋势也时刻处于变化之中。最近一段时间里，TensorFlow放出了2.0版（Alpha），推出了诸多新功能，也让入门的门槛有所降低。不过这些改变并没有减慢PyTorch追赶的步伐。

**选自TowardsDataScience，作者：Jeff Hale，机器之心编译，参与：杜伟、李泽南。**

本文中，作者通过GitHub、Medium文章、arXiv论文和领英等维度评估了不同神经网络框架的最新发展趋势。

2018年9月，作者曾就需求、使用和受欢迎程度三方面比较了所有主要的深度学习框架，其中[TensorFlow](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759875%26idx%3D2%26sn%3De4eacb6126de840085f1165854937c59%26chksm%3D871aa6bdb06d2fababfcd031371db5903f3c440f97808fbb2f2db7d2efee02b441037fc16577%26token%3D1934456430%26lang%3Dzh_CN)是无可争议的重量级冠军，[PyTorch](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759875%26idx%3D2%26sn%3De4eacb6126de840085f1165854937c59%26chksm%3D871aa6bdb06d2fababfcd031371db5903f3c440f97808fbb2f2db7d2efee02b441037fc16577%26token%3D1934456430%26lang%3Dzh_CN)是赢得大量口碑的后起之秀。

过去的六个月，领先的深度学习框架又有什么变化呢？
![](https://pic2.zhimg.com/v2-c8ec96d9c3a79f588620dbd05f0ca451_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='506'></svg>)
为回答这一问题，作者不仅查看了Indeed、Monster、LinkedIn和SimplyHired上的职位列表数量，而且评估了谷歌搜索量、GitHub动态、Medium和ArXiv文章以及Quora主题关注者的变化。总的说来，这些资源全面描述了需求、使用和兴趣三方面的增长。

**集成和更新**

从工具本身来说，近来TensorFlow和PyTorch框架都出现了几项重大发展。

2018年10月，PyTorch v1.0预发布，同时fastai v1.0发布。二者的发布是标志框架成熟的重大里程碑。

2019年3月4日，[TensorFlow 2.0 alpha版发布](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758196%26idx%3D1%26sn%3D55499592929483fa69b1104164cd54b1%26chksm%3D871a984ab06d115cc488d075b43230feed6e3a4d07029074620b1d0bafa79c5aee7e2669d4d6%26scene%3D21%23wechat_redirect)，增加了一些新功能，改善了用户体验。TensorFlow 2.0 alpha版更紧密地集成了Keras，作为其高阶API。

**方法论**

Keras和fastai与TensorFlow和PyTorch紧密集成在一起，因而本文在比较时将二者包含在内。Keras和fastai还为评估TensorFlow和PyTorch提供了可以参考的评估范围。
![](https://pic2.zhimg.com/v2-0005fbf4cf8ccf1142fd8c1539679335_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='714' height='556'></svg>)
作者在本文中不会探究其他深度学习框架。作者希望收到反馈，证明Caffe、Theano、[MXNet](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650759875%26idx%3D2%26sn%3De4eacb6126de840085f1165854937c59%26chksm%3D871aa6bdb06d2fababfcd031371db5903f3c440f97808fbb2f2db7d2efee02b441037fc16577%26token%3D1934456430%26lang%3Dzh_CN)、CNTK、PaddlePaddle、DeepLearning4J或Chainer值得讨论。虽然这些深度学习框架各有其优点，但就其增长轨迹而言，似乎不太可能接近TensorFlow或PyTorch。这些框架也未能与TensorFlow或PyTorch紧密耦合。

搜索日期为2019年3月20日-21日。源数据在谷歌表格中。

让我们看一下各类深度学习框架的结果吧！

**在线职位列表变化**

为了确认哪种深度学习库在当今求职市场中受欢迎，作者搜索了Indeed、LinkedIn、Monster和SimplyHired中的职位列表。

作者首先搜索术语「机器学习」，其后为框架/库名。所以，作者以「机器学习 TensorFlow」评估TensorFlow。基于历史比较的原因，作者采用了这种评估方法。不包含「机器学习」的搜索没有产生明显不同的结果。搜索区域是美国。

作者从2019年3月的职位列表数量中减去六个月前的数量，结果如下：
![](https://pic4.zhimg.com/v2-f0968d1ddd39d4047520181aa3388203_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
TensorFlow的职位列表增长略高于PyTorch。Keras的职位列表也出现了增长——约是TensorFlow增长的一半。FastAI依然没有出现在任何职位列表中。

需要注意的是，除了LinkedIn，PyTorch在所有求职网站上的增加职位列表数量都超过了TensorFlow。还需要注意的是，就绝对项而言，TensorFlow出现在职位列表中的数量几乎是PyTorch或Keras三倍。

**平均谷歌搜索活动的变化**

最大搜索引擎中的网页搜索是衡量受欢迎程度的一个标准。作者查看了过去一年里Google Trends的搜索历史。作者还搜索了全世界对「机器学习」和「人工智能」分类的兴趣。谷歌没有给出绝对搜索数量，但提供了相对数据。

作者将过去六个月与在之前六个月的搜索兴趣平均分进行比较。
![](https://pic2.zhimg.com/v2-22b080547ff26609229484e2d19943b9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
在过去六个月，TensorFlow的相对搜索数量减少，而PyTorch的相对搜索数量增加。

下图来自谷歌，直接显示了过去一年的搜索兴趣。
![](https://pic1.zhimg.com/v2-9f0e90d9ec40f4f2af58bb8f3653b3b0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='244'></svg>)
TensorFlow显示为蓝色，Keras黄色，PyTorch红色，fastAI绿色。

**新Medium文章**

Medium是数据科学文章和教程的流行阵地。作者希望读者可以喜欢！😃

过去六个月里，作者使用[http://Medium.com](https://link.zhihu.com/?target=http%3A//Medium.com)的谷歌站点搜索，结果发现TensorFlow和Keras发表的文章数量相当，而PyTorch相对较少。
![](https://pic4.zhimg.com/v2-9322c70ef306b660bc466765f0e3855b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
作为高阶API，Keras和fastAI非常受深度学习新从业人员的欢迎。Medium中有很多教程介绍了这些框架的使用方式。

**最新arXiv文章**

作为最流行的预印版论文发布平台，大多数学术类深度学习文章在arXiv上发表。作者根据过去六个月的谷歌站点搜索结果来搜索提及每种框架的最新文章。
![](https://pic2.zhimg.com/v2-9a572cee46f1a26fa5baf9d7806a2149_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
TensorFlow有接近3000篇文章，在这方面占有很大优势，大多数最新AI论文使用的是TensorFlow框架（说好的学界爱用PyTorch呢？）。

**最新GitHub动态**

GitHub上的最新动态是框架受欢迎程度的另一指标。作者在下图列出了star、fork、watcher和contributor的数量。
![](https://pic4.zhimg.com/v2-9bdd6ba35268887315ff9458b5d27c87_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
在每种类别中，TensorFlow上的GitHub动态最多。但是，PyTorch在watcher和contributor的增长方面与TensorFlow相当接近。此外，Fastai上也有大量的新增contributor。

毫无疑问，Keras的一些贡献者会在TensorFlow库中工作。值得注意的是，TensorFlow和Keras都是由谷歌人牵头的开源产品。

**Quora新关注者**

作者将新的Quora主题关注者添加进参考标准中——这是一个以前没有数据的新分类。
![](https://pic1.zhimg.com/v2-422e69f74219614a286277710aa59550_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
过去六月里，TensorFlow增加了最多的新话题关注者。PyTorch和Keras相差甚多。

作者在添加所有数据后将其合并成一个指标。

**增长分数过程**

作者创建了增长评分机制：

1. 所有特征的数值范围为0-1。

2. 汇总了「线上职位列表」和「GitHub动态」子分类。

3.基于下列百分比的加权分类。
![](https://pic1.zhimg.com/v2-d58dd30bc470932b15aea714230d8460_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
4.可理解性乘以加权分数100。

5.将每个框架的类别分数汇总为单个增长分数。

工作列表占总分数的三分之一多，这是比例最高的一部分：钱说了算。这种多权重方法平衡了不同类别。相比2018年的统计，这次统计未包含KDNuggets的使用调查（因为还未出现新数据），以及书籍的出版（过去六个月相应新书数量不多）。

**结果**

这里是表格形式的变化。
![](https://pic1.zhimg.com/v2-43f8eaeb40662f106a4a6cd1fde25ab4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='148'></svg>)
相应的Google表格：[https://docs.google.com/spreadsheets/d/1Q9rQkfi8ubKM8aX33In0Ki6ldUCfJhGqiH9ir6boexw/edit?usp=sharing](https://link.zhihu.com/?target=https%3A//docs.google.com/spreadsheets/d/1Q9rQkfi8ubKM8aX33In0Ki6ldUCfJhGqiH9ir6boexw/edit%3Fusp%3Dsharing)

这里是类别和最终分数。
![](https://pic4.zhimg.com/v2-8f6c8b7156ff96897ff61eee98491903_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='188'></svg>)
这是最终的增长分数。
![](https://pic3.zhimg.com/v2-35b884e9bebbb6b4f6e405934e057796_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='450'></svg>)
**TensorFlow是需求量最大，也是增长最快的框架，这个趋势近期不会有任何变化。**PyTorch的增长速度也很快，在职业需求列表中的快速增长证明了这种需求。在过去的六个月里，Keras也有了很大发展。最后，fastai的起点最低，不过因为它最年轻，所以仍然值得期待。

TensorFlow和PyTorch都是值得学习的框架。

**学习建议**

如果你希望学习TensorFlow，我们建议你可以从Keras学起。相应的课程则是Chollet的Deep Learning with Python和Dan Becker的DataCamp course on Keras。TensorFlow 2.0使用tf.keras，将Keras作为其高阶API。Chollet有一篇TensorFlow 2.0的快速入门。

如果你想要学习PyTorch，我们建议你从fast.ai的MOOC课程《Practical Deep Learning for Coders v3》入手。在这里你将学习到深度学习基础、fastai和PyTorch的基础。

TensorFlow和PyTorch的未来方向会是什么？

**未来的方向**

我们一直能够听到“PyTorch比TensorFlow更好用”的说法。的确，PyTorch更加Python一些，也有更为统一的API，它拥有原生的ONNX模型导出机制，可用于加速推理。而且，PyTorch和Numpy有着很多共同之处，这让人们的学习成本降低不少。

但是，TensorFlow 在2.0版中大力改进了用户体验，正如谷歌首席智能决策工程师Cassie Kozyrkov所说的。TensorFlow现在已拥有更加直接的API、完全集成的Keras和Eager Execution选项了。这些变化，以及TensorFlow的用户基础可以让这个框架在未来继续流行下去。

TensorFlow最近宣布了另一个激动人心的计划：Swift版TensorFlow的开发。Swift是一种原先由苹果推动的编程语言。在执行和开发速度方面，Swift比Python有很多优势。Fast.ai将在MOOC进阶课程中使用Swift for TensorFlow。这种语言可能不会在未来一两年内迎来黄金时间，但这种发展对于目前的深度学习框架非常有益。

语言和框架之间的融合正在发生。

另一个即将影响深度学习框架的是量子计算。实用型量子计算机有机会在几年后出现，而谷歌、IBM、微软和其他公司正在探索量子计算与深度学习的融合。框架需要适应这些新技术。

**结语**

在统计之后，我们可以看到TensorFlow和PyTorch都是增长强劲的神经网络框架。它们都有高阶API——tf.keras和fastai——它们降低了进入深度学习的门槛。除此之外，我们还看到了未来的一些发展方向。

若希望获取文章中的数据，以及Jupyter Notebook，请移步Kaggle Kernel：[https://www.kaggle.com/discdiver/2019-deep-learning-framework-growth-scores](https://link.zhihu.com/?target=https%3A//www.kaggle.com/discdiver/2019-deep-learning-framework-growth-scores)
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
*原文链接：*
[https://towardsdatascience.com/which-deep-learning-framework-is-growing-fastest-3f77f14aa318](https://link.zhihu.com/?target=https%3A//towardsdatascience.com/which-deep-learning-framework-is-growing-fastest-3f77f14aa318)




