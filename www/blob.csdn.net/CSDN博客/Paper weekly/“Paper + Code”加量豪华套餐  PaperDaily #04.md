# “Paper + Code”加量豪华套餐 | PaperDaily #04 - Paper weekly - CSDN博客





2017年10月13日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：324












![640.jpeg?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/640.jpeg?wxfrom=5&wx_lazy=1)




在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。





在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。




点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。

这是 PaperDaily 的第 **4** 篇文章[ 自然语言处理 ]
![640.png?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icO9FmLojPqkAkFLqO8OhZEARhJGDywtkJx945hvpibxdvtFduMhzpThQ/640.png?wxfrom=5&wx_lazy=1)
**ZhuSuan: A Library for Bayesian Deep Learning**

**@paperweekly 推荐**

#Bayesian Deep Learning

Reddit 热文，清华大学朱军老师组的工作，贝叶斯学习工具包 ZhuSuan。

论文链接

http://www.paperweekly.site/papers/742



代码链接

https://github.com/thu-ml/zhusuan




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GialBA3SGMBbvEWztUWZZnGKibn1SyAZMgNoXNIgJA7ZH4WfFz0UVCMNMA/0.png)
**OpenNMT: Open-Source Toolkit for Neural Machine Translation**

**@Ttssxuan 推荐**

#NMT

完全基于 sequence-to-sequence 实现，包括诸如：multi-layer RNN, attention, bidirec- tional encoder, word features, input feeding, residual connections, beam search, and several others 等扩展。

论文链接

http://www.paperweekly.site/papers/716



代码链接

https://github.com/opennmt/opennmt




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8ic2RRwm8aXCXL5HWq6ibib9MB9AogRAsiakDeMIv4UVZ28M1MjuYSU1sCiaA/0.png)
**A Compare-Aggregate Model for Matching Text Sequences**

**@xwzhong 推荐**

#QA

该 paper 更像是一篇实验性论文，在“general”框架下对其中某一块使用不同的方法比较、组合。

通过实验得到（subtraction+multiplication+nn）结果比（Euclidean distance or cosine similarity）效果更好。原因可能在于，前一种方式得到的是高维的 matrix，而后一种方式只是二维的向量，表现能力比较弱，高维包含了更细致的信息

论文链接

http://www.paperweekly.site/papers/564



代码链接

https://github.com/shuohangwang/SeqMatchSeq



[ 计算机视觉 ]
![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icJ6oGKRITiaenF00wDTL2VZF5zDm4mcv4S9N18QbCuxVtibhcltkXmb0g/0.png)
**Adversarial Representation Learning for Domain Adaptation**

**@corenel 推荐**

#Representation Learning

ARDA 将 classifier、encoder 以及 discriminator 三者共同训练，思路不错。

论文链接

http://www.paperweekly.site/papers/692



代码链接

https://github.com/corenel/pytorch-arda




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnzMG2RjDrT4O9Gn283L4GiaDc3GOpv6X8XguY9aNvsDjQ2pzJ3YPlibX0EjGZagDM5JMIoj55icXdGA/0.png)
**Unsupervised Image-to-Image Translation Networks**

**@gujiuxiang 推荐**

#GAN

The proposed framework can learn the translation function without any corresponding images in two domains.

论文链接

http://www.paperweekly.site/papers/647



代码链接

https://github.com/leehomyc/Img2Img-Translation-Networks




![0.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icaZzQUgYngzzhJxZoP5AIlmr1T0HArp3uKIFX8aibD4OlwVS3KuAJ79g/0.png)
**A Discriminatively Learned CNN Embedding for Person Re-identification**

**@xintong 推荐**

#image captioning

在行人识别领域有两类流型的模型（都是基于 CNN），一类是 verification model，一类是 identification model，由于损失函数不同，两类模型各有优缺点。

verification model 以图片对作为输入，经过一个非线性函数（CNN），得到特征后进行相似度判断，这样只利用到了弱的 Re-id 标签。identification model 训练时是以一张图片作为输入，经过一个非线性函数（CNN），得到特征，然后进行多类判断。

在测试阶段，把两张图片经过全连接的网络，得到特征，然后再做相似度判断。identification model 虽然利用了更多的标注信息，但是训练的目标并不直接是行人重识别。

本文的创新之处，是结合两类模型，学习一个更有区分度的行人识别判别器。实验效果表明，本文提出的融合模型，在 Market1501 和 CUHK 数据集都比基础的两类模型有效果提升，并且该模型还可以运用在图片检索领域。

论文链接

http://www.paperweekly.site/papers/824



代码链接

https://github.com/D-X-Y/caffe-reid




**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「阅读原文」即刻加入社区！**


**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![640?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmUEtia3RCJ5eZHIskGRIYc1Uen4885tjUqeiaS963f1BQ5PdHHibppPMuHbfbpVsbicYsU00j6RwOA2w/640?)

![0.gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/ePTzepwoNWPuSZ5SBgtleovKV97Gn4cIicAMa4kDTwWw586xyoZVfJn4gWZ7nv4krxKxVjZQ8wWmI1iba4HCia8bg/0.gif)




