# 本期最新 9 篇论文，每一篇都想推荐给你 | PaperDaily #14 - Paper weekly - CSDN博客





2017年11月15日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：1243












![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。

这是 PaperDaily 的第 **14** 篇文章
[ 自然语言处理 ]
![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/?wxfrom=5&wx_lazy=1)
**Word Translation Without Parallel Data**

**@Ttssxuan 推荐**

#Unsupervised Learning

**本文贡献：**

• 一种无需平行语料的无监督词翻译方案 

• 缓和 hubness problem 的方案 

• 无监督验证与选参方法 

• 证明本方法在词料较少的语言的适用性 

• 多种语言的 词翻译结果 

**主要价值：**

• 无监督和无平行语料的训练方案 

• 对抗学习 的应用 

**困难和思路：**

• 无监督训练：使用对抗学习使得 两种语言在目标空间分布重合 

• 无监督验证：计算翻译对之间的平均 cosin 距离，用于验证和调参 

• 生成词典：使用 CSLS 找出翻译结果

论文链接

http://www.paperweekly.site/papers/1097

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icQRlrP3aP0pQfsEGbyTZKF6UDWtfZmrMzn4nY13xQ7kA1icr0N5TcRVQ/?)
**Generalized End-to-End Loss for Speaker Verification**

**@Drin_Peng 推荐**

#Speaker Recognition

这篇文章是 Google 投 ICASSP2018 的，本文在原始端到端框架上重新设计了更加具有可分性的判别函数和度量距离（1. 损失函数和清华大学王东老师投 ICASSP2018 的 FULL-INFO TRAINING FOR DEEP SPEAKER FEATURE LEARNING 文章有异曲同工之妙）。此外还应用了 domain adaptation，借助大数据集训练了小数据集。这些方法减少了训练时间（60%）提高了准确率（10%）。

论文链接

http://www.paperweekly.site/papers/1121

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ichDlonfdvKXvzUPKNndGkVFic5wMs53ZjOygqDQouOASgne02AYV1yaA/?)

**End-to-End Adversarial Memory Network for Cross-domain Sentiment Classification**

**@robertdlut 推荐**

#Sentiment Classification

该论文是香港科技大学杨强老师团队的工作，发表在 IJCAI2017 上。该论文在跨领域情感分类任务上使用了一个端到端的对抗记忆网络进行迁移学习。

该网络有两部分组成，一个情感分类器（目的找到对情感分类重要的特征），一个领域分类器（目的在于找到所有领域都共享的特征）。在领域分类器中使用了对抗训练来学习共享特征。然后这两部分同时训练，来选出特征即能够有效进行情感分类，又是所有领域都存在的。在迁移学习中使用对抗训练，值得学习。

论文链接

http://www.paperweekly.site/papers/1085



[ 计算机视觉 ]
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/?)
**Unsupervised Adaptation for Deep Stereo**

**@lisabug 推荐**

#Person Re-identification

**文章有何贡献：**本文提出了一种新的 fine-tuning 的方法使在大量合成数据上训练的 DispNet 可以迁移到无 groudtruth 或者只有极少量的 groundtruth 的实际数据集上。 

**本文研究的问题有何价值：**双目深度估计的标签现实中很难获得，本文提出的 fine-tuning 方法可以在没 有groundtruth 的情况下将模型迁移过来。 

**所研究问题有何困难：**如何获得可靠的监督信息来 fine-tune。

**本文的解决思路是怎样的：**文章受在 Kitti 数据集上 fine-tune 的启发，发现利用稀疏的标签也可以很好地对模型进行训练。文章利用传统算法如 AD-CENSUS 或 SGM 生成 label 来作为 groundtruth， 同时利用 CCCN（一种 confidence
 measure 的方法）来选取可信度高的 label，只利用这部分置信度高的 sparse label 来 fine-tune。

论文链接

http://www.paperweekly.site/papers/1084




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icEknJzstkpn6Gab1EeXF5tmGG8rGM2FibNFG9O31YIc5eib0lrZ6MloxQ/?)
**ShuffleNet: An Extremely Efficient Convolutional Neural Network for Mobile Devices**

**@chenhong 推荐**

#Object Classification

分类网络是计算机视觉的基础，目标追踪定位识别都可以分割到分类实现。分类网络在嵌入式设备比如手机等智能终端中应用，是商业化的必然趋势。Face++ 团队提出的 ShuffleNet，基于 mobilenet 的改进，具有实现更高效的优势。

论文链接

http://www.paperweekly.site/papers/1113




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/?)
**Representation Learning by Rotating Your Faces**

**@993917172 推荐**

#Face Capture

**贡献：**与现有的方法 SRGAN 等不同，该论文建立了一个带有编码-解码结构的生成器，由编码器输入任意姿态的人脸图像，由解码器输出摆正姿态的人脸图像。此外，该论文的判别器不仅用来分辨真实和人造图像，还用来预测人脸身份信息。 

**研究价值：**现实中，受到多种因素影响，用户所获得的监控视频中人脸图像往往质量较低，且人脸图像姿态较多，这些因素都对人脸识别等应用造成影响。 

**研究困难：**在图像质量评价中，生成对抗网络的实验结果图具有更好的主观结果图，但是它的客观评价未达到最优结果。 

**未来可能研究方向：**未来，可以将人脸识别与生成对抗网络更好的结合，生成器与判别器的密集连接将成为一种可能。

论文链接

http://www.paperweekly.site/papers/1106




[ 机器学习 ]

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaPFKKcQtakJkrJqiay67wic4UibzIoJEFesEXzBdgvbKrU3pQbdFib3fEibQ/?)
**NeST: A Neural Network Synthesis Tool Based on a Grow-and-Prune Paradigm**

**@paperhwd 推荐**

#Neural Networks

文章提出了一种在训练过程中连接、生长、移除神经元的神经网络。这种神经网络使用梯度和神经元来生长和修剪，从而实现权重和结构的同时训练。此算法可以同时实现神经网络结构的自动选择和超高效压缩。

论文链接

http://www.paperweekly.site/papers/1116




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkFFwOYfF41kUMpx9QrhiacsUsaDJKp7afm9SFyWQgUwpPItQo9UH0zH3GFjzTw4LJezwoExXFCzpA/?)
**Neural Discrete Representation Learning**

**@Synced 推荐**

#Neural Network Toolkit

DeepMind 最近提出的 VQ-VAE 是一种简单而强大的生成模型，结合向量量化和变分自编码器学习离散表示，可以实现在图像识别、语音和对话等任务上的无监督学习。

论文链接

http://www.paperweekly.site/papers/1100






![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmdTOOt6xMGO8q70RUDWEzldUCgrhG69LwL4uQCmiaK8A19jufpMJdHTxzZ5KGRrJNpEgnHJRfNk1A/?)
**An Empirical Analysis of Multiple-Turn Reasoning Strategies in Reading Comprehension Tasks**

**@zhangjun 推荐**

#Probabilistic Reasoning

本文通过实验得出结论，在机器阅读理解任务中多轮推理将会得到更好的结果，这里用增强学习对轮数进行控制。

论文链接

http://www.paperweekly.site/papers/1099




**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**



我是彩蛋




**解锁新姿势：用微信刷论文！**




PaperWeekly小程序上线啦




**今日arXiv√猜你喜欢√热门资源**√****




随时紧跟最新最热论文



** 解锁方式 **

1. 识别下方二维码打开小程序

2. 用PaperWeekly社区账号进行登陆

3. 登陆后即可解锁所有功能




**长按识别二维码，使用小程序**

*点击阅读原文即可注册




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)










******关于PaperWeekly******




PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)




▽ 点击 | 阅读原文|
 查看更多优质论文




