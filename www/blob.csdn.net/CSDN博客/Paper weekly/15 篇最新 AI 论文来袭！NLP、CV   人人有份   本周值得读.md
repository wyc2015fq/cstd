
# 15 篇最新 AI 论文来袭！NLP、CV...人人有份 | 本周值得读 - Paper weekly - CSDN博客


2018年05月16日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1244




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**72**篇文章

**Accelerating Neural Transformer via an Average Attention Network**
**@bzhang 推荐**
\#Neural Machine Translation
**本文主要研究机器翻译领域最先进的 Transformer 系统**（Attention is all you need）。针对该系统解码效率底下的问题，本文在模型设计层面提出平均注意网络，在不损失翻译质量的情况下，本文所提模型有效提升解码速率 4~7 倍。
本文在 WMT 六个语言对 12 个翻译方向上进行了实验论证，结果一致地表明本文所提模型可以有效地提升解码速率，并生成高质量译文。


论文链接
https://www.paperweekly.site/papers/1929
代码链接
https://github.com/bzhangXMU/transformer-aan
**Cross Domain Regularization for Neural Ranking Models Using Adversarial Learning**
**@Ttssxuan 推荐**
\#Adversarial Learning
本文来自 SIGIR ’18。深度表征学习网络可以自动地学习数据集中数据的表示，但是这也存在局限性，其被局限到被采样的数据中，而对未见过的数据域泛化能力有限。**本文借助对抗网络对表征学习网络进行正则化**，其分类器向表征网络提供负反馈，使其不会陷入特定数据域的表征学习，从而提升网络对的泛化能力。

论文链接
https://www.paperweekly.site/papers/1923

**Hierarchical Neural Story Generation**
**@llamazing 推荐**
\#Text Generation
本文来自 Facebook AI Research，**论文使用层次话结构做故事生成，解决长依赖性问题**。少信息->多信息，decoder self-attention + model fusion，decoder 时 word 从 word prob top10 中随机选取，可减少生成重复文本。

论文链接
https://www.paperweekly.site/papers/1932


**DOTA: A Large-scale Dataset for Object Detection in Aerial Images**
**@paperweekly 推荐**
\#Object Detection
**本文提出了一个数据集**，包含 2806 张遥感图像（大小约 4000*4000），188,282 个 instances，分为 15 个类别。

论文链接
http://www.paperweekly.site/papers/1907

代码链接
https://github.com/jessemelpolio/Faster_RCNN_for_DOTA

数据集链接
https://captain-whu.github.io/DOTA/dataset.html


**Spiking Deep Residual Network**
**@chlr1995 推荐**
\#Spiking Neural Network
脉冲神经网络（SNN）在生物理论中备受关注。理论上脉冲神经网络应该与人工神经网络的性能是相同的，但是训练深层的 SNN 是非常困难的。**本文提出了一种脉冲版本的 ResNet**，并且在 MNIST、CIFAR 等数据集上实验得到了 state of the art的结果。

论文链接
https://www.paperweekly.site/papers/1916


**Deep Active Learning for Named Entity Recognition**
**@cmdjeu 推荐**
\#Named Entity Recognition
本文是亚马逊和 UT Austin 发表于ICLR 2018 的工作，**论文在命名实体识别的方法上引入主动学习，在少量数据集即可达到较优结果**，感觉也可以扩展到其他自然语言方向。

论文链接
https://www.paperweekly.site/papers/1919


**An Universal Image Attractiveness Ranking Framework**
**@Ttssxuan 推荐**
\#Image Ranking
本文来自微软，**本文结合 deep convolutional neural network 和 rank net，设计对成对的图片的 Attractiveness 排序模型**。模型首先使用深度卷积得到网络图片的 attractiveness score 的均值和方差，然后使用设计好的标准，对两个图片之间的关系进行预测。
论文把模型排序结果和搜索引擎排序结果比较，质量得到较明显提升。

论文链接
http://www.paperweekly.site/papers/1908


**Network Embedding as Matrix Factorization: Unifying DeepWalk, LINE, PTE, and node2vec**
**@xavierzw 推荐**
\#Network Embedding
本文来自清华和微软。**论文创造性地将 DeepWalk，LINE，Node2Vec 等 network embedding 的方法，通过 Matrix Factorization 框架来统一表示**。
进一步地基于 Matrix Factorization 的思路，作者提出 NetMF 方法，实验证明优于 DeepWalk，LINE 的算法。此外作者也给出了相关 Upper Bound 的严格数学证明。

论文链接
https://www.paperweekly.site/papers/1924

代码链接
https://github.com/xptree/NetMF


**Global Encoding for Abstractive Summarization**
**@llamazing 推荐**
\#Abstractive Summarization
本文是北京大学发表于 ACL 2018 的工作，**论文提出用 Global Encoding 解决句内重复和输入输出语义无关问题**，Convolutional Gated Unit + Self Attention。

论文链接
https://www.paperweekly.site/papers/1930


**Look Closer to See Better: Recurrent Attention Convolutional Neural Network for Fine-grained Image Recognition**
**@RTM 推荐**
\#Image Recognition
本文是 CVPR 2017 的一篇 Oral 文章，主要工作集中在细粒度图片识别。**文中提出了一种级联的网络结构，通过 anattention proposal sub-network 实现粗粒度图片到细粒度图片的获取和识别**，文中充分利用了卷积神经网络的注意力机制，在原始图片的基础上裁剪、放大识别图片中目标。

论文链接
https://www.paperweekly.site/papers/1904


**An Attention Mechanism for Answer Selection Using a Combined Global and Local View**
**@IndexFziQ 推荐**
\#Answer Selection
本文来自 Digitalgenius，**提出用 attention 根据不同的输入粒度计算相似度，将答案的特定部分中的局部信息与整个问题的全局表示相结合**。Answer selection 的关键就是文本相似度的计算，文章有可以学习的地方。
最后在 InsuranceQA 上评估系统，实验目的是看注意力机制关注的哪些部分文本，并探究其在不同参数设置下的表现，结果比 IBM（Improved Representation Learning for Question Answer Matching）提出的 Attention LSTM 稍微提高了一些。


论文链接
https://www.paperweekly.site/papers/1918


**Deep & Cross Network for Ad Click Predictions**
**@c0de 推荐**
\#Ad Click Predictions
本文来自斯坦福大学和 Google，**论文利用深度学习自动高效得学习高阶交叉特征**，免去特征工程。

论文链接
https://www.paperweekly.site/papers/1898


**Cardiologist-Level Arrhythmia Detection with Convolutional Neural Networks**
**@liria 推荐**
\#Convolutional Neural Network
本文来自斯坦福吴恩达组，**该论文主要做的事情建立了从单导联的心电信号到 14 种心脏疾病的模型**，模型是一个 34 层的 CNN 网络。文章定义了 12 种心脏异常状态和窦性心率及噪声，共 14 种。模型主要是 34 层的残差 CNN 将 ECG 序列映射到 label 序列。
**本文声称自己的模型超过了心电科的医生**，不同于传统的提取各种统计指标再训练模型，是一种直接从 sequnce 训练的模型，确实能够减少很多工作量。

论文链接
https://www.paperweekly.site/papers/1921


**Efficient Natural Language Response Suggestion for Smart Reply**
**@mev 推荐**
\#Natural Language Understanding
**本文介绍了 Gmail Smart Reply 的一个检索式实现**，这个结果应该是实际产品化了的，有一定的参考价值。文章中使用了大量的方式来降低模型的 latency，并且使最终结果保持在较高精度。
比较有意思的是文中有一个实验，使用句子的ngramembedding sum 来表示句子，然后通过一个 RNN 重新生成原句，在几十万词的数据集下得到了 ppl 为 1.2 的结果，证明了仅仅使用ngram就可以捕捉到足够的句子序列信息了。

论文链接
https://www.paperweekly.site/papers/1935


**DLTSR: A Deep Learning Framework for Recommendation of Long-tail Web Services**
**@somtian 推荐**
\#Recommender System
**作者使用深层自编码器解决推荐中的一个新颖问题：长尾推荐问题**。探索了深度学习在推荐系统中越来越多的领域。

论文链接
https://www.paperweekly.site/papers/1936

代码链接
https://github.com/baib/DLTSR


**\#****推 荐 有 礼****\#**

本期所有入选论文的推荐人
均将获得**PaperWeekly纪念周边一份**


**▲**机器学习主题行李牌/卡套

**▲**深度学习主题防水贴纸
**想要赢取以上周边好礼？**
**点击阅读原文**即刻加入社区吧！


**点击以下标题查看往期推荐：**

来不及想标题了，我要去打包收藏了

快醒醒，一大波最新 AI 论文加开源代码来袭！

[选对论文，效率提升50% | 本周值得读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487741&idx=1&sn=b61741b47e602626a236f5984a0b1cb4&chksm=96e9cf7da19e466b953b4f0fb4e0003b868045fd1a4eb1b38a2b6cfe5316c60bcd368f4ee985&scene=21#wechat_redirect)
入坑AI却不知道该读什么？这15篇最新论文给你答案



**▲**戳我查看比赛详情


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。


▽ 点击 |阅读原文| 加入社区刷论文


