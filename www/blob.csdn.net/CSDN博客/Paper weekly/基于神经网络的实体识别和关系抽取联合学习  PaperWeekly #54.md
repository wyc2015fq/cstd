# 基于神经网络的实体识别和关系抽取联合学习 | PaperWeekly #54 - Paper weekly - CSDN博客





2017年12月04日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：930









作者丨罗凌

学校丨大连理工大学博士生

研究方向丨深度学习，文本分类，实体识别

联合学习（Joint learning）一词并不是一个最近才出现的术语，在自然语言处理领域，很早就有研究者使用基于传统机器学习的联合模型（Joint model）来对一些有些密切联系的自然语言处理任务进行联合学习。例如实体识别和实体标准化联合学习，分词和词性标注联合学习等等。

最近，研究者们在**基于神经网络方法上进行实体识别和关系抽取联合学习**，我阅读了一些相关工作，在此和大家一起分享学习（本文中引用了一些论文作者 Suncong Zheng 的 PPT 报告）。




# 引言




本文关注的任务是**从无结构的文本中抽取实体以及实体之间的关系**（实体
 1-关系-实体 2，三元组），这里的关系是我们预定义好的关系类型，例如下图：




![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBw6biagViaDeGtCzDhqmp76Eo5OxZtvhJqOJzGQRRVicbUIROFKiaJMWUoQ/?wxfrom=5&wx_lazy=1)

目前有两大类方法，一种是使用**流水线的方法（Pipelined Method）**进行抽取：输入一个句子，首先进行命名实体识别，然后对识别出来的实体进行两两组合，再进行关系分类，最后把存在实体关系的三元组作为输入。

**流水线的方法存在的缺点有：**

1. 错误传播，实体识别模块的错误会影响到下面的关系分类性能；

2. 忽视了两个子任务之间存在的关系，例如图中的例子，如果存在 Country-President 关系，那么我们可以知道前一个实体必然属于 Location 类型，后一个实体属于 Person 类型，流水线的方法没法利用这样的信息；

3. 产生了没必要的冗余信息，由于对识别出来的实体进行两两配对，然后再进行关系分类，那些没有关系的实体对就会带来多余信息，提升错误率。


![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBSjFYJFzQV6LVFUvsmbicb8SvGO5icYUEia2ZHz8ExibetD36ElCGPTibQibw/?)

**理想的联合学习**应该如下图：输入一个句子，通过实体识别和关系抽取联合模型，直接得到有关系的实体三元组。这种可以克服上面流水线方法的缺点，但是可能会有更复杂的结构。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBVPxlVZs1em1eugSDS7XPialKm1WsgKL8HibHDhnllmFzEibqHFeMxA3yQ/?)




# 联合学习




这里我主要关注的**基于神经网络方法的联合学习**，我把目前的工作主要分为两大类：1. 参数共享（Parameter Sharing）和 2. 标注策略（Tagging Scheme）。主要涉及到下面一些相关工作。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBmZDnd3XLqZXN8pFfP0NESib2knKQkibmicnLJDSPadCibzlnHDxRWO8OPw/?)




# 参数共享




论文**《Joint Entity and Relation Extraction Based on A Hybrid Neural Network》**，Zheng
 等人利用共享神经网络底层表达来进行联合学习。

具体的，对于输入句子通过共用的 word embedding 层，然后接双向的 LSTM 层来对输入进行编码。然后分别使用一个 LSTM 来进行命名实体识别（NER）和一个 CNN 来进行关系分类（RC）。

相比现在主流的 NER 模型 BiLSTM-CRF 模型，这里将前一个预测标签进行了 embedding 再传入到当前解码中来代替 CRF 层解决 NER 中的标签依赖问题。

在进行关系分类的时候，需要先根据 NER 预测的结果对实体进行配对，然后将实体之间的文本使用一个 CNN 进行关系分类。所以该模型主要是通过底层的模型参数共享，在训练时两个任务都会通过后向传播算法来更新共享参数来实现两个子任务之间的依赖。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBSHWia0tDSxO1IH0FFARmgzNM4UFCcibQLMhm0H0lUibGiauyXGuuSFdnDw/?)

论文**《End-to-End Relation Extraction using LSTMs on Sequences and Tree Structures》**也是类似的思想，通过参数共享来联合学习。只是他们在
 NER 和 RC 的解码模型上有所区别。

这篇论文 Miwa 等人同样是通过参数共享，NER 使用的是一个 NN 进行解码，在 RC 上加入了依存信息，根据依存树最短路径使用一个 BiLSTM 来进行关系分类。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBO8RcraPJvmE1tZmIW6P1rbWKLKf1MHnLCL2NWzFShH5nmT5S3icqibvg/?)




根据这两篇论文的实验，使用参数共享来进行联合学习比流水线的方法获得了更好的结果在他们的任务上 F 值约提升了 1%，是一种简单通用的方法。论文**《A Neural Joint Model for Entity and Relation Extraction from Biomedical Text》**将同样的思想用到了生物医学文本中的实体关系抽取任务上。




# 标注策略




但是我们可以看到，参数共享的方法其实还是有两个子任务，只是这两个子任务之间通过参数共享有了交互。而且在训练的时候还是需要先进行 NER，再根据 NER 的预测信息进行两两匹配来进行关系分类。仍然会产生没有关系的实体对这种冗余信息。

出于这样的动机，Zheng 等人在论文**《Joint Extraction of Entities and Relations Based on a Novel Tagging Scheme》**中提出了一种新的标注策略来进行关系抽取，该论文发表在
 2017 ACL 上，并入选了 Outstanding Paper。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBfjictj0bwG3gqSTxkOgibOQSqBoCj7Am5j0Tqzh1yn2d7nGpDxPCvplQ/?)

他们通过提出了一种新的标注策略把原来涉及到序列标注任务和分类任务的关系抽取完全变成了一个序列标注问题。然后通过一个端对端的神经网络模型直接得到关系实体三元组。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfB2vaf6KzE7e24ghU1fSPcY5268YSY5iaicdZGx4MHHXJrFO4UhfrXDH7g/?)

他们提出的这种新的标注策略主要由下图中三部分组成：1）实体中词的位置信息{B（实体开始），I（实体内部），E（实体结尾），S（单个实体）}；2）关系类型信息{根据预先定义的关系类型进行编码}；3）实体角色信息{1（实体1），2（实体2）}。注意，这里只要不是实体关系三元组内的词全部标签都为“O”。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBIMmzQL3lkMFu1gyib9hs64c6iaahtNXrtMA44QENw6ic8MOIc4sYcia3jw/?)

根据标签序列，将同样关系类型的实体合并成一个三元组作为最后的结果，如果一个句子包含一个以上同一类型的关系，那么就采用就近原则来进行配对。目前这套标签并不支持实体关系重叠的情况。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBst0OJgM2SRAyvAYTibjPLu7LUp2o6fxyzLDZLzW07tGBvoeUcsSpJQw/?)

然后该任务就变成了一个序列标注问题，整体模型如下图。首先使用了一个 BiLSTM 来进行编码，然后使用了在参数共享中提到的 LSTM 来进行解码。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBKL9pOPRm1bNcR0YWD9iahYMRemwFay3ibiadM4Km5Vr8tMrJclSiakauRw/?)

和经典模型不同的地方在于他们使用了一个带偏置的目标函数。当标签为“O”时，就是正常的目标函数，当标签不是“O”时，即涉及到了关系实体标签，则通过 α 来增大标签的影响。实验结果表明，这个带偏置的目标函数能够更准确的预测实体关系对。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBaFPvjkd0c57aoBicLaduy2DdhRXZKeS2QziaepNoE6nu7eJibjWJQ9tYg/?)




# 总结




基于神经网络的实体识别和关系抽取联合学习主要由两类方法。其中参数共享的方法简单易实现，在多任务学习中有着广泛的应用。

Zheng 等人提出的新的标注策略，虽然目前还存在一些问题（例如无法识别重叠实体关系），但是给出了一种新的思路，真正的做到了两个子任务合并成了一个序列标注问题，在这套标注策略上也可以进行更多的改进和发展来进一步完善端到端的关系抽取任务。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkjZwt9qQxA5ictEzIzVFnfBFAHvxJWkVIJGtetfeLxgl22ybX2Nv4eM8GVBrrUP6ur6ECCicgnbtTQ/?)




# 参考文献




[1] S. Zheng, Y. Hao, D. Lu, H. Bao, J. Xu, H. Hao, et al., Joint Entity and Relation Extraction Based on A Hybrid Neural Network, Neurocomputing. (2017)
 1–8. 

[2] M. Miwa, M. Bansal, End-to-End Relation Extraction using LSTMs on Sequences and Tree Structures, ACL, (2016). 

[3] F. Li, M. Zhang, G. Fu, D. Ji, A Neural Joint Model for Entity and Relation Extraction from Biomedical Text, BMC Bioinformatics. 18 (2017). 

[4] S. Zheng, F. Wang, H. Bao, Y. Hao, P. Zhou, B. Xu, Joint Extraction of Entities and Relations Based on a Novel Tagging Scheme, Acl. (2017).




# 相关链接




[1] 论文作者 Suncong Zheng 报告：http://t.cn/RXmunzm

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**

我是彩蛋

** 解锁新功能：热门职位推荐！**




PaperWeekly小程序升级啦




**今日arXiv√猜你喜欢√**热门职位****√****




找全职找实习都不是问题



** 解锁方式 **

1. 识别下方二维码打开小程序

2. 用PaperWeekly社区账号进行登陆

3. 登陆后即可解锁所有功能




** 职位发布 **

请添加小助手微信（**pwbot01**）进行咨询




**长按识别二维码，使用小程序**

*点击阅读原文即可注册




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)










**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)




