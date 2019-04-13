
# 感觉灵感被掏空？你需要这 9 篇论文来补一补 | PaperDaily \#05 - Paper weekly - CSDN博客


2017年10月18日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：293



![640.jpeg?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640.jpeg?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**5**篇文章
[ 自然语言处理 ]![640.png?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640.png?wxfrom=5&wx_lazy=1)
**What does Attention in Neural Machine Translation Pay Attention to?**
**@paperweekly 推荐**
\#Attention Model
本文对 attention 进行了详细研究，对比了 NMT 中的 alignment，给出了很多有意思的结论。
论文链接
http://www.paperweekly.site/papers/857
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/0.png)
**Flexible End-to-End Dialogue System for Knowledge Grounded Conversation**
**@lukepan 推荐**
\#Chatbot
该论⽂所引⼊的知识是以 tuple 的形式储存在 KB 中的。 knowledge 的引⼊⽅式如下：Candidate Facts Retriever 从 input 中提取 entity，然后在 KB 中进⾏ query，将通过 relation 寻找到的 objects 和 subjects 作为 Candidate Facts 存储为⼀个集合。
在 Reply Decoding 流程中，为了决定该使⽤哪些 facts，提出了 dynamic knowledge enquirer，⽤以根 据 local contexts at various positions within a response 来 rank 并选择集合中哪些 facts 应该被使⽤。
论文链接
http://www.paperweekly.site/papers/891
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/0.png)
**Program Induction by Rationale Generation : Learning to Solve and Explain Algebraic Word Problems**
**@RamonYeung 推荐**
\#Question Answering
DeepMind 和牛津大学共同打造的代数问题数据集 AQuA（AQuA = Algebra Question Answering）。
论文链接
http://www.paperweekly.site/papers/913

[ 计算机视觉 ]![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/0.png)
**Detect to Track and Track to Detect**
**@corenel 推荐**
\#Object Detection
新出的一篇视频内目标检测的文章，在逐帧的 RoI Pooling 上增加了帧间的 RoI Tracking（regresses box transformations (translation, scale, aspect ratio) across frames），结构简单，精度不错，通过调间隔时间可以调整处理速度。
论文链接
http://www.paperweekly.site/papers/912

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/0.png)
**GP-GAN: Gender Preserving GAN for Synthesizing Faces from Landmarks**
**@starif 推荐**
\#Face Synthesis
此文尝试用 facial landmarks 通过 GAN 来合成人脸。除了 Adversarial loss，文章使用了 perceptual loss、L1 reconstruction error，特别还设计了 gender preserving loss。
性别具有确定性、二元性，是人脸的一个很特别的属性，也是对机器学习来说一个易用的信息，如何利用这一属性来提高人脸的合成效果，文章或许可以给你一个很好的启发。
论文链接
http://www.paperweekly.site/papers/860

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/0.png)
**Face Super-Resolution Through Wasserstein GANs**
**@993917172 推荐**
\#Face Reenactment
2017 年，生成对抗网络 GAN 广泛应用在超分辨率复原 SR 等领域，并取得巨大发展。但是，GAN 的变体 WGAN，CGAN 等方法仍然较少的应用于 SR 上。
本文把超分辨率复原思想和 WGAN 结合起来，思路和上面的 SRGAN 有点类似。与之不同，它是用于解决人脸图像分辨率不足的问题。该方法不仅采用基础 GAN（2014）作为基本网络框架，还将 WGAN 作为提升 GAN 网络学习能力的重要工具。
总的来说，该方法使用卷积神经网络进行图像复原，简单高效，结合 GAN 网络，只要较少的训练样本就可以让生成图像非常逼真，广大学者可以尝试在更多图像对监控场景下应用人脸图像的思路，发挥生成对抗网络的威力。
论文链接
http://www.paperweekly.site/papers/927

[ 机器学习 ]
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/0.png)
**Mixed Precision Training**
**@机器之心 推荐**
\#CTC
减少模型半数内存用量：百度 & 英伟达提出混合精度训练法。
论文链接
http://www.paperweekly.site/papers/858

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/0.png)
**A Brief Survey of Deep Reinforcement Learning**
**@Nevertiree 推荐**
\#Deep Reinforcement Learning
深度强化学习的最新综述，发表于 2017 下半年。本文是 Reddit 推荐的热文。
论文链接
http://www.paperweekly.site/papers/922

![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmdTOOt6xMGO8q70RUDWEzldUCgrhG69LwL4uQCmiaK8A19jufpMJdHTxzZ5KGRrJNpEgnHJRfNk1A/0.png)
**MoleculeNet: A Benchmark for Molecular Machine Learning**
**@paperweekly 推荐**
\#Molecular Machine Learning
一个分子机器学习 benchmark，最喜欢看到这种将机器学习应用到传统学科领域了。
论文链接
http://www.paperweekly.site/papers/862

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![0.gif?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/0.gif?)

▽ 点击 |阅读原文|
 查看更多优质论文


