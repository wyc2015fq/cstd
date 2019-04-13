
# 自然语言处理中的自注意力机制（Self-Attention Mechanism） - Paper weekly - CSDN博客


2018年03月28日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1284


作者丨罗凌
学校丨大连理工大学信息检索研究室
研究方向丨深度学习，文本分类，实体识别

近年来，注意力（Attention）机制被广泛应用到基于深度学习的自然语言处理各个任务中，之前我对早期注意力机制进行过一些学习总结[1]。

随着注意力机制的深入研究，各式各样的 Attention 被研究者们提出。在 2017年 6 月 Google 机器翻译团队在 arXiv 上放出的**Attention is All You Need**[2]论文受到了大家广泛关注，自注意力（self-attention）机制开始成为神经网络 Attention 的研究热点，在各个任务上也取得了不错的效果。

**本人就这篇论文中的 Self-Attention 以及一些相关工作进行了学习总结**，其中也参考借鉴了张俊林博士的博客**深度学习中的注意力机制（2017版）**[3]"和苏剑林的[一文读懂「Attention is All You Need」| 附代码实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486960&idx=1&sn=1b4b9d7ec7a9f40fa8a9df6b6f53bbfb&chksm=96e9d270a19e5b668875392da1d1aaa28ffd0af17d44f7ee81c2754c78cc35edf2e35be2c6a1&scene=21#wechat_redirect)，和大家一起分享。

# 背景知识

Attention 机制最早是在视觉图像领域提出来的，应该是在九几年思想就提出来了，但是真正火起来应该算是 2014 年 Google Mind 团队的这篇论文**Recurrent Models of Visual Attention**[4]，他们在 RNN 模型上使用了 Attention机制来进行图像分类。

随后，Bahdanau 等人在论文**Neural Machine Translation by Jointly Learning to Align and Translate**[5]中，使用类似 Attention 的机制在机器翻译任务上将翻译和对齐同时进行，他们的工作算是第一个将 Attention 机制应用到 NLP 领域中。

接着 Attention 机制被广泛应用在基于 RNN/CNN 等神经网络模型的各种 NLP 任务中。2017 年，Google 机器翻译团队发表的**Attention is All You Need**中大量使用了自注意力（self-attention）机制来学习文本表示。自注意力机制也成为了大家近期的研究热点，并在各种 NLP 任务上进行探索。

**下图为 Attention 研究进展的大概趋势：**

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXyVfuIibmicR24S8aEB9FGdRtj51d8WHWJXTg8VM2y6elDricSo0SetaYg/640)

**Attention 机制的本质来自于人类视觉注意力机制**。人们视觉在感知东西的时候一般不会是一个场景从到头看到尾每次全部都看，而往往是根据需求观察注意特定的一部分。而且当人们发现一个场景经常在某部分出现自己想观察的东西时，人们会进行学习在将来再出现类似场景时把注意力放到该部分上。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX0EJ0HYhZVU6KmcZCsFycwfjp20iaRxfXhobSGibZoE38U988Au6eWeUw/640)

下面我先介绍一下在 NLP 中常用 Attention 的计算方法，里面借鉴了张俊林博士**深度学习中的注意力机制（2017版）**里的一些图。

**Attention 函数的本质可以被描述为一个查询（query）到一系列（键key-值value）对的映射**，如下图：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXGwv7npVicQuJXa4Qol7efibvGKcdDfr3s3Uf0hGV3lmawmqPNGAoriaDg/640)

在计算 Attention 时主要分为三步，第一步是将 query 和每个 key 进行相似度计算得到权重，常用的相似度函数有点积，拼接，感知机等；然后第二步一般是使用一个 softmax 函数对这些权重进行归一化；最后将权重和相应的键值 value 进行加权求和得到最后的 Attention。

目前在 NLP 研究中，key 和 value 常常都是同一个，即 key=value。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXFDH7WUpCibzeazFgMPExKSmhDWzibOyx0SxczjRWVwJYV2qmicfyWNDQQ/640)
# Attention is All You Need

接下来我将介绍**Attention is All You Need**这篇论文。这篇论文是 Google 机器翻译团队在 2017 年 6 月放在 arXiv 上，最后发表在 2017 年 NIPS 上，到目前为止 Google 学术显示引用量为 119，可见也是受到了大家广泛关注和应用。

**这篇论文主要亮点在于：**

1. 不同于以往主流机器翻译使用基于 RNN 的 Seq2Seq 模型框架，**该论文用 Attention 机制代替了 RNN 搭建了整个模型框架**。

2.**提出了多头注意力（Multi-headed Attention）机制方法**，在编码器和解码器中大量的使用了多头自注意力机制（Multi-headed self-attention）。

3.**在 WMT2014 语料中的英德和英法任务上取得了先进结果**，并且训练速度比主流模型更快。

该论文模型的整体结构如下图，还是**由编码器和解码器组成，在编码器的一个网络块中，由一个多头 Attention 子层和一个前馈神经网络子层组成，整个编码器栈式搭建了 N 个块**。类似于编码器，只是解码器的一个网络块中多了一个多头 Attention 层。

为了更好的优化深度网络，整个网络使用了残差连接和对层进行了规范化（Add & Norm）。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX6vic317YNtIsibNONe9NoXDsK30VD7Xgwl6Sj4FFKwzxbPblk2REtPOQ/640)

下面我们重点关注一下这篇论文中的 Attention。在介绍多头 Attention 之前，我们先看一下论文中提到的放缩点积 Attention (Scaled Dot-Product attention)。

对比我在前面背景知识里提到的 Attention 的一般形式，其实**Scaled Dot-Product Attention 就是我们常用的使用点积进行相似度计算的 Attention**，只是多除了一个（为 K 的维度）起到调节作用，使得内积不至于太大。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXibGBdAich2Wp0yYmqadkmdn3guBicy1hNClc3Jozyo5tqefTIgeCtHgGA/640)

多头 Attention（Multi-head Attention）结构如下图，Query，Key，Value 首先进过一个线性变换，然后输入到放缩点积 Attention，注意这里要做 h 次，其实也就是所谓的多头，每一次算一个头。而且每次 Q，K，V 进行线性变换的参数 W 是不一样的。然后将 h 次的放缩点积 Attention 结果进行拼接，再进行一次线性变换得到的值作为多头 Attention 的结果。

可以看到，Google 提出来的多头 Attention 的不同之处在于进行了 h 次计算而不仅仅算一次，论文中说到这样的好处是**可以允许模型在不同的表示子空间里学习到相关的信息**，后面还会根据 Attention 可视化来验证。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX2dLJHxn2jqG0WQUicm3VSYEmDLWtu2bXkgCj6XNJyU5m0EF0CG0XS8A/640)

**那么在整个模型中，是如何使用 Attention 的呢？**如下图，首先在编码器到解码器的地方使用了多头 Attention 进行连接，K，V，Q 分别是编码器的层输出（这里 K=V）和解码器中都头 Attention 的输入。

其实就和主流的机器翻译模型中的 Attention 一样，利用解码器和编码器 Attention 来进行翻译对齐。然后在编码器和解码器中都使用了多头自注意力 Self-Attention 来学习文本的表示。

Self-Attention 即 K=V=Q，例如输入一个句子，那么里面的每个词都要和该句子中的所有词进行 Attention 计算。目的是学习句子内部的词依赖关系，捕获句子的内部结构。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXsSNZPXCm6GuITooI6OuRiaaolv6FqxwpVdPw89mE8hicowygmCRQkDZQ/640)

对于使用自注意力机制的原因，论文中提到主要从三个方面考虑（每一层的复杂度，是否可以并行，长距离依赖学习），并给出了和 RNN，CNN 计算复杂度的比较。

可以看到，如果输入序列 n 小于表示维度 d 的话，每一层的时间复杂度 Self-Attention 是比较有优势的。

当 n 比较大时，作者也给出了一种解决方案 Self-Attention（restricted）即每个词不是和所有词计算 Attention，而是只与限制的 r 个词去计算 Attention。

在并行方面，多头 Attention 和 CNN 一样不依赖于前一时刻的计算，可以很好的并行，优于 RNN。

在长距离依赖上，由于 Self-Attention 是每个词和所有词都要计算 Attention，所以不管他们中间有多长距离，最大的路径长度也都只是 1。可以捕获长距离依赖关系。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXGicksf5UvUGSe7I8Q35SicqLg0yYySEQICOf0VicysbnLBbh5aL6h8Mjw/640)

最后我们看一下实验结果，在 WMT2014 的英德和英法机器翻译任务上，都取得了先进的结果，且训练速度优于其他模型。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX1icwP8ae7wyrfutgE6KTQQQfbDiazk9ajNKYQYBa5ia3w4LPVaj9Z89uQ/640)

在模型的超参实验中可以看到，多头 Attention 的超参 h 太小也不好，太大也会下降。整体更大的模型比小模型要好，使用 dropout 可以帮助过拟合。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXo5L5ka5ELx3BjlTlzmp4zh6VuSlRo5pd2qxeDONUTQqmcnyQLMd0eg/640)

作者还将这个模型应用到了句法分析任务上也取得了不错的结果。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXnNPmWHibfDmGkQm4atzwA6TwWiaRaqPovY3dWAdxnxA2ZwIUSgYsPOhw/640)

最后我们看一下 Attention 可视化的效果（这里不同颜色代表 Attention 不同头的结果，颜色越深 Attention 值越大）。可以看到 Self-Attention 在这里可以学习到句子内部长距离依赖"making…….more difficult"这个短语。

在两个头和单头的比较中，可以看到单头"its"这个词只能学习到"law"的依赖关系，而两个头"its"不仅学习到了"law"还学习到了"application"依赖关系。多头能够从不同的表示子空间里学习相关信息。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXG9DdGKakTcGzcklmYmDtXOmW5OkvuPbic1BkRTvGe1iaVNJicqtX6Qb7w/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX7JXxMSeDsibn25CkPsiamcONhc1Z8fkeHa0t4vBgmXGT92bMastWWn9A/640)

# Self-Attention in NLP

■ 论文 | Deep Semantic Role Labeling with Self-Attention
■ 链接 | https://www.paperweekly.site/papers/1786
■ 源码 | https://github.com/XMUNLP/Tagger

这篇论文来自 AAAI2018，厦门大学 Tan 等人的工作。**他们将 Self-Attention 应用到了语义角色标注任务（SRL）上，并取得了先进的结果**。

这篇论文中，作者将 SRL 作为一个序列标注问题，使用 BIO 标签进行标注。然后提出使用深度注意力网络（Deep Attentional Neural Network）进行标注，网络结构如下：

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXrGIjRYic6xIsd4rZFTcPXnmuCo15uuJlBHZVYibZjloiapsh80SbXKQOw/640)

在每一个网络块中，有一个 RNN/CNN/FNN 子层和一个 Self-Attention 子层组成。最后直接利用 softmax 当成标签分类进行序列标注。

该模型在 CoNLL-2005 和 CoNll-2012 的 SRL 数据集上都取得了先进结果。我们知道序列标注问题中，标签之间是有依赖关系的，比如标签 I，应该是出现在标签 B 之后，而不应该出现在 O 之后。

目前主流的序列标注模型是 BiLSTM-CRF 模型，利用 CRF 进行全局标签优化。在对比实验中，He et al 和 Zhou and Xu 的模型分别使用了 CRF 和 constrained decoding 来处理这个问题。

可以看到**本论文仅使用 Self-Attention，作者认为在模型的顶层的 Attention 层能够学习到标签潜在的依赖信息**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXAm5It5ewcZ2P9fHvgNnwU5icibtKPibpANHkhh4KV86BaNfU6CKVjsaSw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX87n69bFH97Imq0ajN49gwMqMTt3icIiadibpUbDBXKlXCzh7jcADryyfQ/640)

■ 论文 | Simultaneously Self-Attending to All Mentions for Full-Abstract Biological Relation Extraction
■ 链接 | https://www.paperweekly.site/papers/1787
■ 作者 | Patrick Verga / Emma Strubell / Andrew McCallum

这篇论文是 Andrew McCallum 团队应用 Self-Attention 在生物医学关系抽取任务上的一个工作，应该是已经被 NAACL 2018 接收。**这篇论文作者提出了一个文档级别的生物关系抽取模型**，里面做了不少工作，感兴趣的读者可以更深入阅读原文。

我们这里只简单提一下他们 Self-Attention 的应用部分。论文模型的整体结构如下图，他们也是使用 Google 提出包含 Self-Attention 的 transformer 来对输入文本进行表示学习，**和原始的 transformer 略有不同在于他们使用了窗口大小为 5 的 CNN 代替了原始 FNN**。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYX4ycKrD0MkibPmH09suq9QibmYnWTbm10vmPdZ3sZCCMQRyygytUcCxgA/640)

我们关注一下 Attention 这部分的实验结果。他们在生物医学药物致病数据集上（Chemical Disease Relations，CDR）取得了先进结果。去掉 Self-Attention 这层以后可以看到结果大幅度下降，而且使用窗口大小为 5 的 CNN 比原始的 FNN 在这个数据集上有更突出的表现。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXib7QgpexAmo36hyChMgSW3O3Spa7wY0Pp53NInM0GtvkznRx84pdklg/640)

# 总结

最后进行一下总结，**Self-Attention 可以是一般 Attention 的一种特殊情况**，在 Self-Attention 中，Q=K=V 每个序列中的单元和该序列中所有单元进行 Attention 计算。

Google 提出的多头 Attention 通过计算多次来捕获不同子空间上的相关信息。Self-Attention 的特点在于无视词之间的距离直接计算依赖关系，能够学习一个句子的内部结构，实现也较为简单并行可以并行计算。

从一些论文中看到，Self-Attention 可以当成一个层和 RNN，CNN，FNN 等配合使用，成功应用于其他 NLP 任务。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXJrbo7iaaibVVaUtia1arG0GV4GUPIp14RXAwghshw7SYQ6tKVb6Mn1HVg/640)

除了 Google 提出的自注意力机制，目前也有不少其他相关工作，感兴趣的读者可以继续阅读。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkSPITic57SxrCrCo5ibiayoYXHXu56l22ibKyrILAEanL4Ae6Y6ibZIXa8DnMtMSmj7kUdZ9dXFYZGTIg/640)

# 相关链接

**[1] 注意力机制在自然语言处理中的应用**
http://www.cnblogs.com/robert-dlut/p/5952032.html
**[2] Attention is All You Need**
https://www.paperweekly.site/papers/224
**[3] 深度学习中的注意力机制（2017版）**
https://blog.csdn.net/malefactor/article/details/78767781
**[4] Recurrent Models of Visual Attention**
https://www.paperweekly.site/papers/1788
**[5] Neural Machine Translation by Jointly Learning to Align and Translate**
https://www.paperweekly.site/papers/434

# 参考文献

[1] Romain Paulus, Caiming Xiong, and Richard Socher.**A deep reinforced model for abstractive summarization**. arXiv preprint arXiv:1705.04304, 2017.
[2] Zhouhan Lin, Minwei Feng, Cicero Nogueira dos Santos, Mo Yu, Bing Xiang, Bowen Zhou, and Yoshua Bengio.**A structured self-attentive sentence embedding**. arXiv preprint arXiv:1703.03130, 2017.
[3] Jianpeng Cheng, Li Dong, and Mirella Lapata.**Long short-term memory-networks for machine reading**. arXiv preprint arXiv:1601.06733, 2016.
[4] Shen, T.; Zhou, T.; Long, G.; Jiang, J.; Pan, S.; and Zhang, C.**Disan:****Directional****self-attention network for****rnn****/****cnn****-free language understanding**. arXiv preprint arXiv:1709.04696, 2017.
[5] Im, Jinbae, and Sungzoon Cho.**Distance-based Self-Attention Network for Natural Language Inference**. arXiv preprint arXiv:1712.02047, 2017.
[6] Shaw, Peter, Jakob Uszkoreit, and Ashish Vaswani.**Self-Attention with Relative Position Representations**. arXiv preprint arXiv:1803.02155 ,2018.

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击以下标题查看相关内容：**

[一文读懂Attention is All You Need | 附代码实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486960&idx=1&sn=1b4b9d7ec7a9f40fa8a9df6b6f53bbfb&chksm=96e9d270a19e5b668875392da1d1aaa28ffd0af17d44f7ee81c2754c78cc35edf2e35be2c6a1&scene=21#wechat_redirect)
[从2017年顶会论文看Attention Model](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247485853&idx=1&sn=7e1dae6b690d718d17bebd8c37d2689b&chksm=96e9d61da19e5f0bafe59a848feccd340ce4fe1ff98cc5dfe24ea08ade644e176b45b8da64a7&scene=21#wechat_redirect)
[Attention is All You Need | 每周一起读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247484985&idx=1&sn=f8cb392ffbeb26c954d7ee3059364be1&chksm=96e9d9b9a19e50af671441ac18b6ec9a3ec7a30722ff6f71120f8b2c3afd063c5c9f96c0998a&scene=21#wechat_redirect)


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgmsvubgibQtWV5t7M3ETKt3bbXiaAothCErMicibic9QCUBpxkuibuht62MGcCTcLyAxqGrsUXbv254InDA/640)

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)**\#****作 者 招 募****\#**

**[让你的文字被很多很多人看到，喜欢我们不如加入我们](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487954&idx=1&sn=d247e5b99ecb2c37e85d962d7f93d7d7&chksm=96e9ce52a19e474457e04affae41dc6b6fe521154f95ae7122260b46ec91f55ae7c8fb472c3c&scene=21#wechat_redirect)**

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
▽ 点击 |阅读原文| 加入社区一起刷论文


