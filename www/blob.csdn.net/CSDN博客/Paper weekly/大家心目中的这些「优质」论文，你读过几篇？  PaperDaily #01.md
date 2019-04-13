
# 大家心目中的这些「优质」论文，你读过几篇？| PaperDaily \#01 - Paper weekly - CSDN博客


2017年09月29日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：571



![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**1**篇文章
[ 自然语言处理 ]![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)

**Sequence Level Training with Recurrent Neural Networks**
**@cosmmb 推荐**
\#recurrent neural networks
现在的 neural-based 的机器翻译在 decoder 端，training 和 testing 在生成机制上存在一定的 disagreement。
decoder 说 training time 的时候后是 local 的，比如说无论之前生成的字是什么 mode l都会用 ground truth 作为前一个字。而在 testing time 的时候是没有 ground truth 的，因此无法给出前应该正确生成的字，这就导致了 training 和 testing 的时候对前一个字的准确度的依赖程度不一样。
如果 testing time 前一个字生产错误，这就导致了 training 时学习的 distribution 和此时不一致而导致未知的后果。这篇文章就是为了统一 training 和 testing 的生成模式。
论文链接
http://www.paperweekly.site/papers/785
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/0?wx_fmt=png)

**Learning to Rank Short Text Pairs with Convolutional Deep Neural Networks**
**@geledek 推荐**
\#question answering
这是一篇相当经典的 Siamese 网络模型，类似架构可以通过改变左右两边的网络结构而适用于各种 task。
如果把右侧网络输入换成 Label，则可以用于实现http://yann.lecun.com/exdb/publis/pdf/lecun-06.pdf.
论文链接
http://www.paperweekly.site/papers/775
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/0?wx_fmt=png)

**Generating Sentences by Editing Prototypes**
**@paperweekly 推荐**
\#natural language generation
本文提出了一种新的文本生成模型，即先从大量的语料中学习生成出一个原型句子，然后再次编辑原型句子而形成最后的句子。相对传统的 left-to-right beam search，本文提出的 prototype-then-edit 生成的句子质量更高。
论文链接
http://www.paperweekly.site/papers/786

[ 计算机视觉 ]![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/0?wx_fmt=png)

**Realtime Multi-Person 2D Pose Estimation using Part Affinity Fields**
**@ailingzeng 推荐**
\#motion estimation
CVPR 2017：多人姿态实时估计，这里主要亮点还是多人实时+效果.本文算法主要流程如下：输入一幅图像，经过卷积网络提取特征，得到一组特征图，然后分成两个岔路，分别使用 CNN 网络提取 Part Confidence Maps 和 Part Affinity Fields ，得到这两个信息后，我们使用图论中的 Bipartite Matching 将同一个人的关节点连接起来得到最终的结果。
论文链接
http://www.paperweekly.site/papers/784

![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/0?wx_fmt=png)

**In Defense of the Triplet Loss for Person Re-Identification**
**@Molly 推荐**
\#question answering
对 triplet loss 的一个讨论，里面的 batch hardmining 非常好用。
论文链接
http://www.paperweekly.site/papers/787

[ 机器学习 ]![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/0?wx_fmt=png)

**Graph Regularized Non-negative Matrix Factorization for Data Representation**
**@jindongwang 推荐**
\#matrix factorization
数据表示领域的经典文章，讨论用图正则化和矩阵分解的方式来表示数据。可以应用到很多方向的研究中。 第一作者是大牛、浙江大学 Deng Cai 老师，Jiawei Han 是共同作者。
论文链接
http://www.paperweekly.site/papers/781

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。
![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmUEtia3RCJ5eZHIskGRIYc1Uen4885tjUqeiaS963f1BQ5PdHHibppPMuHbfbpVsbicYsU00j6RwOA2w/640?)
![0?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/0?wx_fmt=gif)


