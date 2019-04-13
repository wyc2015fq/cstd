
# 开源代码“All in One”：6 份最新「Paper + Code」等你复现 | PaperDaily \#12 - Paper weekly - CSDN博客


2017年11月10日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：241



![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**12**篇文章[ 自然语言处理 ]![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)
**Earth Mover’s Distance Minimization for Unsupervised Bilingual Lexicon Induction**
**@paperhwd 推荐**
\#Bilingual Word Embeddings
文章想解决的问题是如何使用无监督的方法构建双语词典，传统方法需要利用有监督的种子翻译词对来获得两个语言空间的映射。文章的主要思路是借助 GAN 这个工具。当然我们知道 GAN 存在训练困难的问题，所以文章其实真正实现的是 WGAN。
关于文章的应用和意义，一方面为许多跨语言处理任务提供了重要资源，尤其是为完全缺乏双语资源的小语种和专门领域开辟了与其他语言连接的可能。此外，只使用单语语料就能构建双语词典意味着语言在词汇层面的某种同态性，佐证了人类语言在概念表示上可能存在的一致性。
论文链接
http://www.paperweekly.site/papers/1087
代码链接
http://nlp.csai.tsinghua.edu.cn/~zm/UBiLexEMD/

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GialBA3SGMBbvEWztUWZZnGKibn1SyAZMgNoXNIgJA7ZH4WfFz0UVCMNMA/?)
**Ngram2vec: Learning Improved Word Representations from Ngram Co-occurrence Statistics**
**@****paperhwd****推荐**
\#Word Embeddings
本文主要还是想生成质量更好的词向量，基于经典的 word2vec 的思想，在其之上加入了 ngram 的共现信息，取得了更好的结果。我更加在意的其实是他们提供了不错的工具包，同时在建立共现矩阵的时候采取了优化策略来减少存储开销，加快了计算速度。
论文链接
http://www.paperweekly.site/papers/1086
代码链接
https://github.com/zhezhaoa/ngram2vec/

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ic2RRwm8aXCXL5HWq6ibib9MB9AogRAsiakDeMIv4UVZ28M1MjuYSU1sCiaA/?)
**Improved Neural Machine Translation with a Syntax-Aware Encoder and Decoder**
**@Andy 推荐**
\#Neural Machine Translation
文章提出了 bidirectional tree encoder，可以同时学会译出语言的序列表达和树状表达特征。之后，解码器利用这些信息进行解码。 其次，提出了 tree-coverage model，使得注意力机制更有效地利用了译出语言的句法结构。
论文链接
http://www.paperweekly.site/papers/1082
代码链接
https://github.com/howardchenhd/Syntax-awared-NMT/

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/?)
**A Simple Approach to Learn Polysemous Word Embeddings**
**@haimizhao 推荐**
\#Word Embeddings
文章在原有的向量表示的基础上，直接对上下文的向量表示作加权和，作为目标词在该上下文的表示，方法极其简单，可以低成本尝试，一系列评估方法也自成一体。
论文链接
http://www.paperweekly.site/papers/1052
代码链接
https://github.com/dingwc/multisense

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaDc3GOpv6X8XguY9aNvsDjQ2pzJ3YPlibX0EjGZagDM5JMIoj55icXdGA/?)
**Neural Paraphrase Generation with Stacked Residual LSTM Networks**
**@paperweekly 推荐**
\#Paraphrase Generation
本文是第一篇用 deep learning 来做 paraphrase 生成任务的工作，已开源。
论文链接
http://www.paperweekly.site/papers/1034
代码链接
https://github.com/iamaaditya/neural-paraphrase-generation

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/?)
**Attention-Based Recurrent Neural Network Models for Joint Intent Detection and Slot Filling**
**@applenob 推荐**
\#Recurrent Neural Networks
基于 attention 的意图识别 + 槽位填充 RNN 模型。
论文链接
http://www.paperweekly.site/papers/937
代码链接
https://github.com/DSKSD/RNN-for-Joint-NLU
https://github.com/applenob/RNN-for-Joint-NLU

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击****「****阅读原文****」即刻加入社区！**
我是彩蛋
**解锁新姿势：用微信刷论文！**

PaperWeekly小程序上线啦

**今日arXiv√猜你喜欢√热门资源****√**

随时紧跟最新最热论文
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)


