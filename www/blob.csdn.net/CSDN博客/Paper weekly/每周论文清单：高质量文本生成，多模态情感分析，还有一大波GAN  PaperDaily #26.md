# 每周论文清单：高质量文本生成，多模态情感分析，还有一大波GAN | PaperDaily #26 - Paper weekly - CSDN博客





2017年12月14日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1420












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第 **27** 篇文章
[ 自然语言处理 ]
![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)
**Multi-channel Encoder for Neural Machine Translation**

**@Synced 推荐**

#Neural Machine Translation

文章提出了一种多信道编码器（MCE）模型，可改进传统神经机器翻译（NMT）模型单一信道编码的缺点。该模型在中英翻译上的 BLEU 值提升了超过 6 个百分点，在英法翻译上用单层模型取得了可与深层模型媲美的效果。

论文链接

https://www.paperweekly.site/papers/1342

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/?)
**DisSent: Sentence Representation Learning from Explicit Discourse Relations**

**@Ttssxuan 推荐**

#Sentence Embedding

借助文档中一些特殊的词训练句子 embedding。使用文档中 but、because、although 等词，以及其前后或关联的句子构成语义模型。也就是，使用这些词和句子的关系，约束了句子向量的生成空间（使用句子向量，预测关联词），从而达到训练句子向量目的。

 文章只对英文语料进行了测试，实际中文这样的结构也很多，如：因为、所以、虽然、但是，可以参考。

论文链接

https://www.paperweekly.site/papers/1324

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/?)

**End-to-end Learning for Short Text Expansion**

**@tangxianfeng 推荐**

#Short Text Expansion

本文第一次用了 end to end 模型来做 short text expansion 这个 task，方法上用了 memory network 来提升性能，在多个数据集上证明了方法的效果；Short text expansion 对很多问题都有帮助，所以这篇 paper 解决的问题是有意义的。

通过在多个数据集上的实验证明了 model 的可靠性，设计的方法非常直观，很 intuitive。

论文链接

https://www.paperweekly.site/papers/1313




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/?)
**Benchmarking Multimodal Sentiment Analysis**

**@chenbjin 推荐**

#Multimodal Sentiment Analysis

多模态情感分析目前还有很多难点，该文提出了一个基于 CNN 的多模态融合框架，融合表情，语音，文本等信息做情感分析，情绪识别。

论文链接

https://www.paperweekly.site/papers/1306




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/?)
**Adversarial Ranking for Language Generation**

**@zhangjun 推荐**

#Generative Adversarial Networks

本文提出了一种 RankGAN 模型，来解决如何生成高质量文本的问题。

论文链接

https://www.paperweekly.site/papers/1290




[ 计算机视觉 ]




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/?)
**PacGAN: The Power of Two Samples in Generative Adversarial Networks**

**@Gapeng 推荐**

#Generative Adversarial Networks

本文旨在研究 mode collapse 问题，提出了 Pac Discriminator 的方法用于解决mode collapse。所谓 pac，就是把 Discriminator 的输入改成多个样本，这么做的好处在于判别器能够同时看到多个样本，可以一定程度上防止 mode collapse 的发生。

我要推荐的并不是他对 mode collapse 的解决方案，而是他分析 mode collapse 的方法，详情参见第 4、5 章。

论文链接

https://www.paperweekly.site/papers/1337




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/?)
**Show-and-Fool: Crafting Adversarial Examples for Neural Image Captioning**

**@Synced 推荐**

#Adversarial Training

针对深度学习系统的对抗性样本攻击问题，来自麻省理工学院，加州大学戴维斯分校，IBM Research 和腾讯 AI Lab 的学者在 arXiv 上发表论文提出对于神经网络图像标注系统（neural image captioning system）的对抗样本生成方法。实验结果显示图像标注系统能够很容易地被欺骗。

论文链接

https://www.paperweekly.site/papers/1321




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/?)
**Convolutional Image Captioning**

**@jamiechoi 推荐**

#Image Captioning

用 CNN 做 image caption，与 LSTM 对比的话，CNN 的训练速度更快，并且从生成的 caption 效果来看，个人感觉 CNN 的效果也更好。

论文链接

https://www.paperweekly.site/papers/1335




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmdTOOt6xMGO8q70RUDWEzldUCgrhG69LwL4uQCmiaK8A19jufpMJdHTxzZ5KGRrJNpEgnHJRfNk1A/?)
**Dual-Path Convolutional Image-Text Embedding**

**@Layumi 推荐**

#Convolutional Neural Network

1. ImageNet 预训练的网络是否是最好的？黑狗白狗都训练成一类 dog，这会损失颜色信息（黑色还是白色）/位置信息（在追球还是树枝）/数量信息（有多少只狗）。同样 word2vec 也未必适合需要处理的数据集（blue 和 red 就很近）所以要进一步 finetune；

2. 图文互搜要求对图像和文本都有细致的理解。detailed information 很重要； 

3. Ranking Loss 在同时调整两个 CNN 的时候有难度，所以引入了 instance loss；

4. 在 MSCOCO/Flickr30k 一些 general 数据集上达到 state of the art，在自然语言搜人问题上提升了 18 个点。

论文链接

https://www.paperweekly.site/papers/1311




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgm3OQD8fU9b5nyluFPpOop0o6c9fGMicicnQyd57ibficujqN6StPyHBIib7ktB5sX5IHNWSyXE77dry4w/?)
**Learning Feature Pyramids for Human Pose Estimation**

**@chenhong 推荐**

#Pose Estimation

论文是香港中文科技大学王晓刚教授团队之作，目前在 MPII 官网在 Single Person 领域，PCKh @ 0.5 evaluation measure，取得 state-of-the-art 水平。 

计算机视觉处理人体姿态估计的挑战在于，随着计摄像头视角变化，人体不同部位的图像会发生显著性尺度变化（离摄像头越近的人体部位，看上去越大，图片中占用像素越多）。

单纯低层次或高层次特征是无意义的，需要用特征金字塔网络来融合多层特征，改进 CNN 特征提取，这也是为什么特征金字塔在目标识别和人体姿态识别领域得到广泛使用。

论文在 Stacked Hourglass 基础上，提出 PRMs 模型和研究多分支网络的权重初始化方法。

论文链接

https://www.paperweekly.site/papers/1325




** 本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**



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










******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/?)




