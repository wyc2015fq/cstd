
# Nature论文解读：用于改善加权生物网络信噪比的网络增强方法 - Paper weekly - CSDN博客


2018年08月07日 12:14:03[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：216


![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgnC9iaic8hDbiadLafh7TtCZS6icEYddVmMqZBksDV7cQkKmAu95h53FxyibqmZOS1yQgHibJT0WYD2s1Zw/640)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640?)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**95**篇文章
本期推荐的论文笔记来自 PaperWeekly 社区用户**@xuehansheng**。本文是斯坦福大学 Bo Wang 的又一篇大作，**即将发表于 Nature Communications**。

**本文提出一种网络增强（Network Enhancement）方法，即一种用于改善无向加权网络的信噪比的方法**。NE 使用双随机矩阵算子来诱导稀疏性，并提供封闭形式的解决方案，增加输入网络的频谱本征。因此，NE 可消除弱边缘，增强实际连接，并带来更好的下游性能。
如果你对本文工作感兴趣，点击底部**阅读原文**即可查看原论文。

# 关于作者：薛寒生，澳大利亚国立大学博士生，研究方向为人工智能与计算生物学。
■ 论文 | Network Enhancement: a general method to denoise weighted biological networks
■ 链接 | https://www.paperweekly.site/papers/2152
■ 作者 |Bo Wang / Armin Pourshafeie / Marinka Zitnik / Junjie Zhu / Carlos D. Bustamante / Serafim Batzoglou / Jure Leskovec

# 论文动机

**这篇文章即将发表于《Nature Communications》****，本文主要针对由于测量技术的局限性和固有的自然变化所产生的网络噪声，提出了一种网络增强方法 NE (Network Enhancement)**。NE 使用双随机矩阵算子来诱导稀疏性，并提供封闭形式的解决方案，增加输入网络的频谱本征。因此，NE 可消除弱边缘，增强实际连接。

# 模型介绍-NE

**网络增强 Network Enhancement 是对加权生物网络进行去噪的通用方法**。 该方法改善了无向加权网络的信噪比，从而提高了下游网络分析的性能。 NE 是一种原则性方法，具有收敛性和性能的理论保证。

NE 将嘈杂的，无向的加权网络作为输入，并在同一组节点上输出网络，但具有一组新的边缘权重。NE 的主要关键是观察到通过网络中的强（高边缘权重）路径连接的节点更可能通过高权重边缘链接。NE 使用高阶网络结构来增强给定的加权生物网络。

NE 中的扩散过程基于任何给定的两个节点之间的交互流来修改网络中的边缘权重，如图所示。具体地说，对于任何两个节点，NE 通过对连接这些节点的长度为 3 或更小的所有路径建模来更新其边缘的权重。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl2woRsU6n7e3GG8s6Whic7ibZZeIQowm4yj618dJ9dakK7NGia37ZVUJickf335IbzEp89sGgKqKyF3g/640)

下图说明了 NE 的迭代扩散过程。 NE 中的扩散过程生成网络，其中具有强相似性/相互作用的节点与高权重边缘连接，而具有弱相似性/相互作用的节点与低权重边缘连接。

我们用于对加权生物网络进行去噪的算法框架是： a）将加权网络作为输入并形成其相关的邻接矩阵（在下面可视化为热图）。 b）使用 NE 扩散过程迭代地更新网络。NE 中的扩散过程保证收敛。我们为下图所示的会聚扩散过程提供封闭形式的解决方案。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl2woRsU6n7e3GG8s6Whic7ibdUqHyicicc4iaOYX4EySjNaGAFEk83t3jGvAuuotDcRib2oELknPBj7vbQ/640)

在收敛时，增强网络具有双随机矩阵属性。 在数学上，这意味着与输入网络相关联的特征向量被保留，而特征向量增加。

# 实验结果

**人体组织网络**

我们将 NE 应用于来自不同人体组织的 22 个基因相互作用网络。 该网络捕获特定于人组织和细胞谱系的基因相互作用，范围从 B 淋巴细胞到骨骼肌和整个脑。鉴于增强的组织网络，我们检查了相关组织特异性基因功能在网络中的连接程度。 期望功能相关基因倾向于在功能活跃的组织中比在其他非相关组织中更频繁地相互作用。

在 NE 增强的血浆网络中，具有最高边缘密度的功能是血液凝固，纤维蛋白凝块形成和极低密度脂蛋白颗粒重塑的负调节，所有这些功能都特异于血浆（下图，左）。 NE 增强脑网络中连接功能最强的是脑形态发生和前脑区域化，它们都是大脑特有的（下图，右图）。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl2woRsU6n7e3GG8s6Whic7ibicW1w2JZszhA9dmehiamE3qd3TE8DDgJOI2RIsF8Ml15nZerIxQJQVcQ/640)

我们评估了原始网络（RAW）和使用 MU，ND，DSD 和 NE 去噪的网络对组织特异性基因功能预测进行评价。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl2woRsU6n7e3GG8s6Whic7ibgfdsISKquHbqxge12oTVNVCn2mqG3GZ5z6eudwdpm84SDv8zh6oZ6A/640)

**Hi-C交互网络**

我们将 NE 应用于 Hi-C 交互网络。Hi-C 是一种基于 3C 的技术，可以测量细胞群内成对染色质的相互作用频率。Hi-C 读数据可以被认为是基因组区域是节点的网络，并且映射到两个区间的标准化读取计数是加权边缘。

使用 NE 对 Hi-C 网络之前和之后的 Hi-C 接触矩阵进行目标检查，揭示了每个社区内边缘的增强以及社区之间的边界更加清晰（如下图所示）。这种改进对于 5kb 分辨率数据尤为明显，在这些数据中，在使用 NE 进行去噪后，原始数据中视觉上无法检测到的社区变得清晰。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl2woRsU6n7e3GG8s6Whic7ib5Vl4Hic42x66voF1qYRlvK7OzDXLR5mtejL7qyiaAOxJ5gZRxLt0icOCw/640)

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl2woRsU6n7e3GG8s6Whic7ib5hiaOfdPl99mIURDRM8diaXNCmF9gHjEEs7K5evBxKuChyzcBRVfYwyA/640)

**蝴蝶种类相似性网络**

我们将 NE 应用于利兹蝴蝶细粒物种图像数据集。 细粒度图像检索旨在区分具有细微差别的类别（例如，君主蝴蝶与孔雀蝴蝶）。 我们分析了表示蝴蝶物种图像之间成对亲和力的加权相似性网络。

目视检查表明，NE 能够极大地改善细粒度识别的整体相似性网络。 在 NE 之前，所有图像都拼凑在一起而没有清晰的聚类（下图，左图）。 应用 NE 后得到的相似性网络清楚地显示了代表不同蝴蝶种类的簇（下图，右图）。

![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl2woRsU6n7e3GG8s6Whic7ibREiaRIouKibeQbj2S1icHuHesNHH3PISdHFVrS5pJzDbZGMQ4s8WSG7ow/640)

# 总结

本文主要通过观测到生物网络中强（高边缘权重）路径连接的节点更可能通过高权重边缘链接。NE 使用高阶网络结构来增强给定的加权生物网络并在扩散过程中根据任何给定的两个节点之间的交互流来修改网络中的边缘权重。并应用在人类组织网络等三个网络中取到了意想不到的结果。方法看似简单却很新颖有效，值得进一步研究。
**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)

**点击标题查看更多论文解读：**

[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)
[基于词向量的简单模型 | ACL 2018论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490031&idx=1&sn=e307230ffbffb648b213b1a775372d06&chksm=96e9c66fa19e4f7996bb13ed2d944d5e49bd538174bd192e41abaf4d2a8863d29135b034cf9c&scene=21#wechat_redirect)
COLING 2018最佳论文：序列标注经典模型复现
[一文解析OpenAI最新流生成模型「Glow」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490358&idx=1&sn=b4b5d6014bdd365456d500537ba5bcad&chksm=96e9c4b6a19e4da08710a55935dc2e15b00838d5395fdf2a424c50dedff9af7fa11441741b9d&scene=21#wechat_redirect)
CVPR 2018 最佳论文解读：探秘任务迁移学习
[哈佛NLP组论文解读：基于隐变量的注意力模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490536&idx=1&sn=0998c5dd4e20841f3542ee328de1f1b4&chksm=96e9c468a19e4d7e24a38f3d9c4b1b4ea48d729c7db124f2b92a2309f0eee982bb9f9bc8356e&scene=21#wechat_redirect)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/640?)
▽ 点击 |阅读原文| 下载论文


