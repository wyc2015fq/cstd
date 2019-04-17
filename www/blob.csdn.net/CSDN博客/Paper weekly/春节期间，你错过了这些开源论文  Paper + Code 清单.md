# 春节期间，你错过了这些开源论文 | Paper + Code 清单 - Paper weekly - CSDN博客





2018年03月01日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：385












![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **46** 篇文章
[ 自然语言处理 ]



![640?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640?wxfrom=5&wx_lazy=1)
**Nested LSTMs**

**@Synced 推荐**

#RNN

近日，CMU 和蒙特利尔大学联合提出一种新型的多级记忆的 RNN 架构——嵌套 LSTM。在访问内部记忆时，嵌套 LSTM 相比传统的堆栈 LSTM 有更高的自由度，从而能处理更长时间规模的内部记忆。

实验也表明，NLSTM 在多种任务上都超越了堆栈 LSTM。作者认为嵌套 LSTM 有潜力直接取代堆栈 LSTM。


论文链接

https://www.paperweekly.site/papers/1627

代码链接

https://github.com/hannw/nlstm




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/640?)
**A Consolidated Open Knowledge Representation for Multiple Texts**

**@hussar 推荐**

#Knowledge Representation

本文提出了一种统一的开放知识表示方法 OKR，可以解决多文档的摘要问题。通过各种指代链接技术对多文档的信息进行压缩。

 论文链接

https://www.paperweekly.site/papers/1668

代码链接

https://github.com/vered1986/OKR




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/640?)

**Deconvolutional Paragraph Representation Learning**

**@icaruss 推荐**

#Encoder-Decoder Framework

这篇 2017 年发表在 NIPS 的文章提供了一个 deconvolutional text autoencoder 架构，在长文本重构上从原先的 20 多的 bleu score 提升到 90 多，并在文本分类等下游任务上提升显著。另外，本文在速度上也提升了三到五倍。

论文链接

https://www.paperweekly.site/papers/1536

代码链接

https://github.com/dreasysnail/textCNN_public







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/640?)
**KBGAN: Adversarial Learning for Knowledge Graph Embeddings**

**@paperweekly 推荐**

#Knowledge Graph

知识图谱和 NLP 数据里通常只有正样本，所以训练传统嵌入模型的时候，大家喜欢用随机采样方法生成负例。但是这样生成的负例往往质量很差，对模型训练没什么帮助。作者提出了一种基于强化对抗学习的新方法，可以自动生成高质量的负样本。

论文链接

https://www.paperweekly.site/papers/1712

代码链接

https://github.com/cai-lw/KBGAN







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/640?)
**MobiRNN: Efficient Recurrent Neural Network Execution on Mobile GPU**

**@kaierlong 推荐**

#Recurrent Neural Network

本文提出的 MobiRNN 框架，可以显著降低 RNN 模型在手机设备上的运行时间。

论文链接

https://www.paperweekly.site/papers/1674

代码链接

https://github.com/csarron/MobiRnn







[ 计算机视觉 ]




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/640?)
**Spectral Normalization for Generative Adversarial Networks**

**@paperweekly 推荐**

#GAN

训练不稳定至今仍是 GAN 的一个重大挑战。本文提出了一种新型权值归一化（weight normalization）技术 SN-GANs，能让判定器训练更加稳定。

这是一种轻量化、易实现的方法，作者用 CIFAR10、STL-10 和 ILSVRC2012 的数据集对它进行了测试，实验证明，SN-GANs 能比以往提出的稳定方法生成质量更高，或者至少是质量相当的图像。

论文链接

https://www.paperweekly.site/papers/1670

代码链接

https://github.com/pfnet-research/sngan_projection







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/640?)
**Parallel Tracking and Verifying**

**@chenhong 推荐**

#Object Tracking

object tracking 是视频分析的基本任务，长期以来一直有两大阵营，CF 相关滤波和 CNN，当然也可以 CF+CNN 融合。

本文将 tracking 过程分解为两个并行但是相互协作的部分：一个用于快速的跟踪（fast tracking），另一个用于准确的验证（accurate verification）。 

论文链接

https://www.paperweekly.site/papers/1613

代码链接

http://www.dabi.temple.edu/~hbling/code/PTAV/ptav.htm







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/640?)
**AmbientGAN: Generative models from lossy measurements**

**@paperweekly 推荐**

#Generative Adversarial Networks

本文为 ICLR 2018 录用论文，主要研究如何从嘈杂、扭曲、局部的观察中学习 GAN。

论文链接

https://www.paperweekly.site/papers/1620

代码链接

https://github.com/shinseung428/ambientGAN_TF







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmdTOOt6xMGO8q70RUDWEzldUCgrhG69LwL4uQCmiaK8A19jufpMJdHTxzZ5KGRrJNpEgnHJRfNk1A/640?)
**Depth CNNs for RGB-D scene recognition: learning from scratch better than transferring from RGB-CNNs**

**@paperweekly 推荐**

#Scene Recognition

基于卷积神经网络的 RGB-D 场景识别，本文为 AAAI2017 录用文章。

论文链接

https://www.paperweekly.site/papers/1610

代码链接

https://github.com/songxinhang/D-CNN







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm3OQD8fU9b5nyluFPpOop0o6c9fGMicicnQyd57ibficujqN6StPyHBIib7ktB5sX5IHNWSyXE77dry4w/640?)
**TernausNet: U-Net with VGG11 Encoder Pre-Trained on ImageNet for Image Segmentation**

**@Synced 推荐**

#Image Segmentation

本文展示了 U-Net 类型的架构如何利用预训练的编码器提升性能。代码和相关的预训练权重已开源。文中比较了三种权重初始化方案：LeCun uniform、取自 VGG11 权重的编码器和在 Carvana 数据集上训练的完整网络。

该网络架构是 Kaggle 竞赛（Carvana Image Masking Challenge）中获胜解决方案（在 735 名参赛者中排名第一）的一部分。

论文链接

https://www.paperweekly.site/papers/1573

代码链接

https://github.com/ternaus/TernausNet




** 本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl5LficnyYLGMnLFlQHP1jZ1JSdBD0p6ibDrQO4cFeh25aT63UERMTS0OrjrskUQGaYXawebJaSNg7A/640)




**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****榜 单 公 布 #**



**[2017年度最值得读的AI论文 | NLP篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487348&idx=1&sn=8ee8bf57418342a419fe73829cb14e75&chksm=96e9d0f4a19e59e288dcb105bd90b1e13f419ee7268ac69eba7cd6dac12e2e64aa84c56e5c07&scene=21#wechat_redirect)**

**[2017年度最值得读的AI论文 | CV篇 · 评选结果公布](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487362&idx=1&sn=980153481f88ba5d6ba2929fd08240f3&chksm=96e9d002a19e5914f845973111b7056d24a28a8f7932479178c9cfcdb10c05c3284d5c0eb602&scene=21#wechat_redirect)**



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







![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/640?)










******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/640?)




