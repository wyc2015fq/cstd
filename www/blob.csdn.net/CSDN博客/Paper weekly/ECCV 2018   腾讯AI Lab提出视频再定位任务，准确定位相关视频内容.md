
# ECCV 2018 | 腾讯AI Lab提出视频再定位任务，准确定位相关视频内容 - Paper weekly - CSDN博客


2018年09月05日 12:26:29[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：227


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)

**腾讯 AI Lab 主导提出一种新的视频再定位方法，能在多个备选视频中快速找到希望搜索的片段。**该研究论文被顶级会议 ECCV 2018 收录，以下是技术详细解读。

ECCV（European Conference on Computer Vision，计算机视觉欧洲大会）将于 9 月 8 日-14 日在德国慕尼黑举办，该会议与 CVPR、ICCV 共称为计算机视觉领域三大顶级学术会议，每年录用论文约 300 篇。AI Lab 是第二次参与该会议，录取文章数高达 19 篇，位居国内前列。在刚结束的计算机视觉领域另外两大会议 CVPR，ICCV 中也收获颇丰，分别录取 21 篇和 7 篇论文。

■ 论文 | Video Re-localization
■ 链接 | https://www.paperweekly.site/papers/2272
■ 作者 |Yang Feng / Lin Ma / Wei Liu / Tong Zhang / Jiebo Luo

该研究由腾讯 AI Lab 主导，与美国罗切斯特大学（University of Rochester）合作完成，研究目的是在给定一个欲搜索的视频后，在某个备选视频中，快速找到与搜索视频语义相关的片段，这在视频处理研究领域仍属空白。

因此**本文定义了一个新任务——视频再定位（Video Re-localization）****，重组 ActivityNet 数据集视频，生成了一个符合研究需求的新数据集，并提出一种交叉过滤的双线性匹配模型**，实验已证明了其有效性。

目前应用最广泛的视频检索方法是基于关键字的视频检索，这种检索方法依赖人工标记，而人工标记不能涵盖视频的所有内容。基于内容的视频检索（CBVR）可以克服上述不足，但是 CBVR 方法一般返回完整的视频，并不会返回具体的相关内容的位置。行为定位（Action Localization）方法可以定位到具体行为在视频当中发生的位置，但是这类方法一般只能定位某些具体的行为类别。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqiakc2rBJZXvMYyQlJUQ1134tZfRibPDMWpunfjhxfUia0ldR407eOQC9g/640)
▲图1：一段查询视频（上）和两段备选视频（中、下）。与查询视频相关的片段已经用绿色方框标出。

图 1 当中有三段视频，当给定图 1 中的查询视频之后，**如何在两个备选视频当中找到与查询视频语义相关的片段？**

已有的视频处理方法并不能很好的解决这个问题。比如，视频拷贝检测（Video Copy Detection）方法只能检测同一段视频出现的不同位置，拷贝检测不能处理图 1 当中的场景变化和动作角色变化的情况。另外，也很难用行为定位方法来解决这个问题，因为训练行为定位的模型需要大量的行为样本数据，在图 1 的例子当中，我们只有一个数据样本。

# 思路

为了解决这类问题，我们定义了一项新的任务，任务的名字是视频再定位。**在给定一段查询视频和一段备选视频之后，视频再定位的目标是快速的在备选视频当中定位到与查询视频语义相关的视频片段。**

要解决视频再定位问题，面临的第一个困难是**缺少训练数据**。虽然目前有很多视频数据集，但是它们都不适合视频再定位研究。训练视频再定位模型，需要成对的查询视频和备选视频，并且备选视频当中需要有和查询视频语义相关的片段，相关片段的起始点和终止点也需要标注出来。

收集这样的训练数据费时费力。为了得到训练数据，我们决定**重新组织现有的数据集，来生成一个适合视频再定位研究的新数据集**。经过调研，我们决定使用 ActivityNet 数据集当中的视频，来构建新数据集。

ActivityNet 数据集当中包含 200 类行为，我们认为同一个类别下的两个行为片段是互相语义相关的。在 ActivityNet 数据集当中，每类行为的样本被划分到训练集，验证集和测试集。因为视频再定位并不局限在一些特定的类别，这种划分并不适合视频再定位任务。

因此，我们决定根据行为的类别，来划分训练集，验证集和测试集。我随机选取了 160 类行为作为训练用，其余的 20 类行为做验证用，再剩余的 20 类行为做测试用。经过一系列的视频预处理和筛选，我们得到了近 10000 个可用的视频。在训练的过程当中，我们随机的选择同一行为类别的两个视频，分别作为查询视频和备选视频。测试的时候，我们固定了查询视频和备选视频的组合。图 2 展示了本文构建的数据集中每部分视频样本的数量。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqLNQY9e0vIUOgfj0LvbKzCwdGPNw9HSSYaHF3xszib9EJXyLiaiasdKIPw/640)
▲图2：本文构建的数据集当中，每类行为当中的视频样本个数。绿色，蓝色和红色分别表示训练，验证和测试用的视频。

# 模型

为了解决视频再定位问题，我们提出了一种**交叉过滤的双线性匹配模型**。对于给定一段查询视频以及一段备选视频，我们首先分别对查询视频和备选视频进行特征提取，然后将查询视频使用注意力机制合并成一个特征向量用于与备选视频匹配。匹配的时候，我们过滤掉不相关的信息，提取相关的信息，然后用双向 LSTM 来生成匹配结果。最后，我们把匹配结果整合，输出预测的起始点和终止点的概率。

接下来，我们着重介绍模型中具有创新性的**交叉过滤机制**，**双向性匹配机制**，以及**定位层**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqvCISicDnb5Er2SfnST0bfPgnO6QO1HlUV3O26OG6Rho35wKFqu8UZWA/640)
▲图3：模型的框架图

**交叉过滤 （Cross Gating）**

因为在备选视频当中有很多我们不关心的内容，所以在匹配的过程当中，我们需要一种过滤机制来去除不相关的内容。我们根据当前的查询视频的特征，来生成一个过滤门，用来过滤备选视频。相应的，我们根据备选视频的特征，来生成另外一个过滤门，来过滤查询视频。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqP9mHybJkicvVfF0P1mZ6ibticq2ia3Q1r8b0iandHJTEFqodTSRLwsyUXyQ/640)

这里的 σ表示 sigmoid 函数，⊙ 表示对对应位相乘，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqZAyHOU54jspVhD4XrPksknlsm1rshom2TLhvNRXVHKQnMc35yiaTqQg/640)、![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqsib5bU5C5sRiavXdWKE6ORciclmqiaBr5EGMmVpRLDELM6zN1YlMsEaLlw/640)、![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMq4OuRryvYDvZvEjpTn4aAia6ib8ic1PVQYG6n8771KH5yAicj4T1FLzAlMg/640)、![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqwhJTILAQGO07B9DqGA8LeSBon1hrf9iarXnqoWPCodESSia7rxMHhzibw/640)是模型的参数。![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqAMOVAxz9RHyia8a5AMlSLMQCY9oHt4U2sypl1kgsGdDSvLibJgHxWAGw/640)和![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqXuKvrQian2D8wErJO7iciaTaxiaRnibZic0BgujEccBKutDuQZ1aaGdyfMQA/640)分别是备选视频和查询视频的特征表示。

**双线性匹配 （Bilinear Matching）**

在得到查询视频和备选视频的特征表示之后，传统的方法将他们拼接到一起，然后输入到神经网络来计算匹配结果。直接的拼接的方法，并不能很好的得到视频中相关的内容，所以我们采用双线性匹配的方法来代替拼接，来更加全面的获取视频的相关内容。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqzP3dU4mzWicvrLCmQKhgrHyTZYryGgluVxz3gLP3hicMhr8iar5PARe4A/640)

上式中，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMq8asWdbvEpgfcKTp7PFt1umLqQ4Dgiaibcpr4ibYKab4hXib4Bfic6AVCs6A/640)、![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqKVsPWdPUnf48Mw4PGkSAxBv8icEDm5HmPOGhlS2YHBKSACZcdjt51icg/640)是模型的参数。

**定位层（Localization）**

根据视频匹配结果，我们来预测备选视频当中每个时间点是开始点和结束点的概率。除此之外，我们还预测了一个时间点是在相关视频片段之内或者不在相关视频片段之内的概率。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqJw5OGNOPuy6icAXugwNFv1YDDicMGEWHLwl7ZvalPKD7GaFZYZIiakeKw/640)

其中，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqSSHhzOVwSHq00TH8xFt3bWGTozHpHiaGwP6qrslIVlxmYVbfgkFBiaow/640)是 LSTM 的隐含状态，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMq5uD8PQvpjomMMn6ZWHFlsBpEIn6cYq98c8jxOUkS35nstmiaF44RUVg/640)、![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqlNBnP5Kvwxjex8kjxN04KL1NH8D3QibnlqkGsI06ThRcM0XACtib9UAQ/640)是模型的参数，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqB10gclRXJ7Jib5MF72n6HIm4nuC9nAdY4nqtX9GZVNia3EUzaf2l2gtg/640)是上一层匹配得到的结果。在预测的时候，我们选择联合概率最大的视频片段。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqae0ibVXjBPG1akc7c2Y2ohiaHYoydN4RlkDmic6KcrxuoCtLo16BPqzfA/640)

其中，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqL3OibRwBIvyJbjL4tW5Ys6AMaTYr3ro5aiaf1buu2XKwc7oSpcZDpIvg/640)是第 s 个时间点是视频片段的起始点的概率，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqc5K8icnOPlK3zcrMpbibmGNubibNuQL07cCrRtBtnWRkcPP6Byw8vJPibg/640)是第 e 个时间点是视频片段的终止点的概率，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqU5M0oXmMa1DKk5nu5ENbbUcUk7H4A0smAROEXLOPCXVkic4Z8JpJApQ/640)是第 i 个时间点是视频片段中的一个时间点的概率。

# 实验

在实验当中，我们设计了三种基线方法。第一个基线方法根据视频帧之间的相似度，计算两个视频片段的相关程度。第二个基线方法把每个视频编码成一个特征向量，然后用特征向量的距离表示两段视频的相关程度。第三个基线方法没有使用查询视频，仅根据备选视频选择最有可能包含行为的视频片段。

在新构建的数据集上定位的定量结果如表 1 所示。另外，一些定性的结果如图 4 所示。可以看到我们提出的方法取得的较优的定位结果。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMqHmsUbaKUqdfkkHSEaDr0psAcz4y3nnrdKjkkljMEgcXbo4iaKtwE73Q/640)
▲表1. 不同方法的定位结果

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmVT3JhZhvoCB9WwjTBzYMq4xicUclFs8WwGMQTNdBpR00Fmibtb758wA0AnT8fibicwatscuWQmiaFjCw/640)
▲图4. 定性结果

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)


**点击以下标题查看更多论文解读：**

网络表示学习综述：一文理解Network Embedding
神经网络架构搜索（NAS）综述
从傅里叶分析角度解读深度学习的泛化能力

哈佛NLP组论文解读：基于隐变量的注意力模型
[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)
[COLING 2018最佳论文：序列标注经典模型复现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490099&idx=1&sn=2d2497999186b979dd557fe3133b7606&chksm=96e9c5b3a19e4ca550a7ae55705af84e941b1aba14cb21f3f2ffc366df837d387575f8529cf2&scene=21#wechat_redirect)
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)



![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****投 稿 通 道****\#**
**让你的论文被更多人看到**

如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？**答案就是：你不认识的人。**

总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。

PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。


