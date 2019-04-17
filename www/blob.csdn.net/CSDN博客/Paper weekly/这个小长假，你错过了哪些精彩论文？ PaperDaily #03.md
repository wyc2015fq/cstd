# 这个小长假，你错过了哪些精彩论文？| PaperDaily #03 - Paper weekly - CSDN博客





2017年10月10日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：190












![640.jpeg?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640.jpeg?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。

这是 PaperDaily 的第 **3** 篇文章
[ 自然语言处理 ]
![640.png?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640.png?wxfrom=5&wx_lazy=1)



**Improving Word Representations via Global Context and Multiple Word Prototypes**

**@paperhwd 推荐**

#Word Embeddings

文章试图通过在《Natural Language Processing (Almost) from Scratch》的基础上进行改进，使得词向量富含更丰富的语义信息。第一个创新是使用全文信息辅助已有的局部信息，第二个创新是使用多个词向量来表示多义词。

论文链接

http://www.paperweekly.site/papers/805

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/0.png)



**Neural Turing Machines**

**@Ttssxuan 推荐**

#Neural Network Architectures

通过使用注意力机制以及外部存储，扩展神经网络的能力，提出类似图灵机（Turing Machine）或冯诺尹曼机（Von Neumann）的系统 Neural Turing Machines (NTM)，NTM 与图灵机和冯诺尹曼机的主要区别在于，NTM 是端到端可微的，这样可能使用梯度下降有效的对 NTM 进行训练。 

本文主要从读（Reading）、写（Writing）、寻址（Addressing Mechanisms）、主控网络（Controller Network），四个方面对 NTM 进行阐述。 

**读：**每步产生个权重向量，向量决定内存每一行权重，向量和内存结合得到读取结果。

**写：**写由两部分组成，即：擦除（erase）和增强（add）。

**寻址：**本文提出基于内容寻址（content-based addressing）和基于位置寻址（location-based addressing）两种方式结合，形成 NTM 寻址方式。

**主控网络：**主要使用了 LSTM 和 Feedforward 两种方式 论文使用 LSTM、NTM（LSTM 控制器）、NTM（Feedforward 控制器），进行了拷贝（Copy）、重复拷贝（Repeat Copy）、Associative Recall、Dynamic N-Grams、Priority
 Sort 等对比实验。


论文链接

http://www.paperweekly.site/papers/810

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/0.png)




**Effective Approaches to Attention-based Neural Machine Translation**

**@zh794390558 推荐**

# Neural Machine Translation

Luong Attention，乘法 Attention，Bahdanau attention（加法）的变种。论文对 Attention 机制讲述清晰，比 Bhdanau 实现简单，对比了多种 score 的方法。语法简单，很值得看的一篇论文。

论文链接

http://www.paperweekly.site/papers/806



[ 计算机视觉 ]
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/0.png)



**Speed/accuracy trade-offs for modern convolutional object detectors**

**@Molly 推荐**

#Object Detection

对各种主流的目标检测网络进行比较（速度、正确率、大小）。 此前各模型的 base feature extractor、 图像像素、 平台都不同，不太好比较。文章给出了一个非常详尽的 Faster R-CNN， R-FCN， SSD 系统的性能比较。

论文链接

http://www.paperweekly.site/papers/840




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/0.png)



**Show, Attend and Tell: Neural Image Caption Generation with Visual Attention**

**@zh794390558 推荐**

#Image Captioning

Attention 在图像中的应用，15 年发表至今被引用上千次。

论文链接

http://www.paperweekly.site/papers/812




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/0.png)



**PixelCNN++: Improving the PixelCNN with Discretized Logistic Mixture Likelihood and Other Modifications**

**@sangiovese17 推荐**

#Image Generation

改进了 Pixel CNN 算法，假设像素点服从一种确定形式的混合分布，通过 CNN 做回归直接预测分布的参数，从而直接的到分布。生成时从预测的分布抽样。

论文链接

http://www.paperweekly.site/papers/838




[ 机器学习 ]

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/0.png)



**node2vec: Scalable Feature Learning for Networks**

**@YFLu 推荐**

#Network Embedding

node2vec 是 16 年 KDD 的一个工作，论文提出了一种学习网络节点的特征表示的算法框架。论文定义了一个网络节点邻居的灵活概念，并设计了一个带偏置的随机游走过程来获得不同邻居节点。

论文的亮点主要在：采用深度优先搜索（DFS）和宽度优先搜索（BFS）两种策略，随机游走生成节点的邻居节点集合。

论文链接

http://www.paperweekly.site/papers/823




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/0.png)



**Bilingual Word Embeddings from Parallel and Non-parallel Corpora for Cross-Language Text Classification**

**@xk57238890 推荐**

#Word Embeddings

该文章介绍了一种建模多语言的段落向量（BRAVE）的模型，该模型通过句子对齐或者标签对齐可以学习词语的多语言分布式表示，从而可以支持多语言的文本分类。

论文链接

http://www.paperweekly.site/papers/821

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**

** 相关活动**




**  不得不读的GAN  **




GAN 是现在最热门的研究方向之一，那么到底有哪些 paper 是值得一读的？




为此，我们将在 PaperWeekly 社区发起一次 **GAN专题论文推荐**，欢迎大家一起参与推荐和 GAN 相关的论文。针对活动中出现的优质论文，我们将组织发起**论文共读**，由一名学术大咖为活动参与者进行在线论文解读。如果你推荐的论文被大家认可，获得了全场最高点赞数，我们将为你送出一份 **PaperWeekly 神秘大礼包**。







** 活动时间**







**10 月 9 日-10 月 20 日**







****参与方式 ****







** 1 ** 点击本文底部的**阅读原文**进入活动页面


** 2 ** 点击**我要参加**，页面右上角将会收到活动消息通知（需注册登录）

** 3 ** 点击网站右上方的**推荐论文**进行推荐

**** 4 ****或从**今日arXiv**右侧下载arXiv
 Paper推荐**插件**，即可进行一键推荐

**** 5 ****为了更好的使用体验，建议**通过PC端访问网站**




*尚未注册PaperWeekly社区的用户请先申请注册。网站目前采用审核制注册，请如实填写个人信息，我们将在**12小时内**完成审核。




**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmUEtia3RCJ5eZHIskGRIYc1Uen4885tjUqeiaS963f1BQ5PdHHibppPMuHbfbpVsbicYsU00j6RwOA2w/640?)

▽ 点击 | 阅读原文|
 加入社区




