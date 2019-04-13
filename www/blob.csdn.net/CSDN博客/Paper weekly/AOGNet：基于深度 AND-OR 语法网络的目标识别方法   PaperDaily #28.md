
# AOGNet：基于深度 AND-OR 语法网络的目标识别方法 | PaperDaily \#28 - Paper weekly - CSDN博客


2017年12月19日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：468



![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgl7VHx00TkzicBMAfz1dFT8icD4HwmJZpt0Jiccw6ns7c3co7MpZslIia8VAuZicUTSuoPaq6hE4KbxWPg/?wxfrom=5&wx_lazy=1)

在碎片化阅读充斥眼球的时代，越来越少的人会去关注每篇论文背后的探索和思考。

在这个栏目里，你会快速 get 每篇精选论文的亮点和痛点，时刻紧跟 AI 前沿成果。

点击本文底部的「**阅读原文**」即刻加入社区，查看更多最新论文推荐。
这是 PaperDaily 的第**28**篇文章
本期推荐的论文笔记来自 PaperWeekly 社区用户**@duinodu**。**本文研究的问题是深度学习中的网络工程问题。**如何设计更好的网络结构，是目前的一个研究热点。这样的网络结构一旦被设计出来，可以马上用于很多其他任务。
**本文贡献主要有两点：**
1. 把语法模型和深度神经网络模型结合起来，设计的模型同时兼顾特征的 exploration and exploitation（探索和利用），并在网络的深度和宽度上保持平衡；
2. 设计的网络结构，在分类任务和目标检测任务上，都比基于残差结构的方法要好。
如果你对本文工作感兴趣，点击底部的**阅读原文**即可查看原论文。
# 关于作者：杜敏，华中科技大学硕士生，研究方向为模式识别与智能系统。

■ 论文 | AOGNets: Deep AND-OR Grammar Networks for Visual Recognition
■ 链接 | https://www.paperweekly.site/papers/1315
■ 作者 | duinodu

# 论文亮点

网络工程问题，属于深度学习中比较基础的问题。网络工程的最大难点在于，由于缺乏对深度神经网络的理论理解，无法根据某种理论来设计网络结构，实际中更多的不断地尝试，根据实验反馈来确定某种结构是不是好。

而使用像增强学习、进化算法等自动学习网络结构的方法，搜索空间巨大。有两个子空间，一个是结构连接子空间，一个是运算符子空间，搜索空间是这两个空间的乘积，自动学习网络结构的算法需要昂贵的计算资源。

本文的解决办法是，**把语法模型（grammer model）放到神经网络的设计中来，****在分类和目标检测任务中，均取得比基于残差结构的模型更好的效果**。

# 模型介绍

**整个模型概览图如下：**

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBed6On9XlBiaq0ooiaI7mCkRoPL8m3BFgwm3Nqdz1iaUkLHThpicVibluIkYg/?)
中间有 4 个 AOG 构建块，每个 AOG 块的结构如下图所示：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBeY5QgYRf4RdIcUsEJ7a3pB4CoZCxgkicKjbOmZUtRqgQ4GaE5WgXF5Yw/?)
AOG 的全称叫 AND-OR graph，是一种语法模型（grammer model）。在人工智能的发展历程中，大体有两种解决办法：一种是自底向上，即目前非常流形的深度神经网络方法，另一种方法是自顶向下，语法模型可以认为是一种自顶向下的方法。
所谓的语法模型，即规定 3-4 条规则，构建 graph，graph 可以认为是一种特征表达的范式，它没有具体规定特征表达是什么形式，但是如果遵循这种规则构建特征表达，却能达到比较好的效果。本文使用如下三条语法规则：
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBeicvGIwEYJCapTrVqHzCppMQYqsFmSf9ns4ZxmNDZXsSGD4WAoUakvkw/?)
**AOGNet 的构建流程如下：**
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBeerITJpDFVUH6PwicnNNcUvsJNk8qpNxp8YVs6uoHEHTSfxnaNhHHt9A/?)
可以配合视频[1]，看这个流程图。
网络工程分为两个部分：设计连接结构和设计运算操作符（structure space 和 operator space）。
整个的 CNN 发展历程，都可以归结到这两点。论文的相关工作部分，很好地对 CNN 网络结构的演变，梳理了一番，有如下网络结构：
LeNet-5（20 年前）
AlexNet（8 层，在 operator space 提出两种新的操作符：ReLU 和 Dropout)）
VGG Net（19 层，多个连续的重复的小卷积核，且卷积的 stride 很小）
network-in-network（用 1x1 的卷积，在层层之间，增大或者较少特征的维数）
GoogleNet（inception, bottleneck）
Highway network（skip connection）
ResidualNetwork（Residual connection）
Fractal Net（another short path without residual）
DenseNet（concatenation scheme）
Dual Path Network SE-Net（channel-wise encoding）
Hourglass（subsampling & upsampling）
使用三条语法规则构建了网络连接方式，网络中每个节点的运算操作符采用 Bottleneck + Conv_BN_ReLU 的方式。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBeoyibuNzoCey5lWkqSKPJbmBGZkrjOBOOhPhzOJK0Av4Kyt1tJYFS4ibA/?)
其实可以不同的节点，设计不同的运算符，作者这里都统一成一样的。处理上面统一的运算符，各个不同的节点还要分别做下面的运算。
![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBeV7qVLKaribEM44KsROWDegZD3ulMYWspBEqw1cxBCblVGjau5MrQnTA/?)

# 实验结果

在 CIFAR 和 ImageNet-1k 上做了分类的实验，在 VOC0712 做了目标检测的实验（使用 fasterRCNN 框架）。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBeSOpZqTXQ8H3B6hlwYKDl6KcuS703bRZFAdTITkoRbFfS3uohZCkLzw/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBe52C77yRVwAyYTl2GCmdJv64ODg44tZGHdS2CpGRQEDO0O5lbr3odhg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgndcSuyibnZj5Q5y3ZoG4iaBe7fWQangoNMEma7X2l6fwvp7l42d56DvmQyOt6KjU3ibbRNcJFLPQZ9w/?)

# 文章评价

本文作者团队是朱松纯教授[2]组的，他们组一直在做语法模型。之前读过他写的《人工智能的现状、任务、架构和统一》[3]，看到这篇文章以及了解了语法模型，才算是了解朱松纯组到底要做什么样的事情。

语法模型和深度神经网络，是可以相互融合的方法，前者基于规则，后者基于大量数据拟合。本文只是语法模型的冰山一角，另一篇论文*Interpretable R-CNN*[4]，也用到了语法模型，和 RFCN 方法结合，获得一种目标的 part configuration。

这种语法模型目前还在探索之中，尤其是和深度神经网络结合的工作，以及和 GAN、RL 等不同范式的深度学习方法结合，应该会有有趣的工作。

# 相关链接

**[1] AOGNet 构建流程视频**
http://v.youku.com/v_show/id_XMzIxNDU4MTQ4NA
**[2] 朱松纯教授主页**
http://www.stat.ucla.edu/~sczhu/
**[3] 人工智能的现状、任务、架构和统一**
http://www.stat.ucla.edu/~sczhu/Blog_articles/浅谈人工智能.pdf
**[4] Interpretable R-CNN**
https://www.paperweekly.site/papers/1215

**本文由 AI 学术社区 PaperWeekly 精选推荐，社区目前已覆盖自然语言处理、计算机视觉、人工智能、机器学习、数据挖掘和信息检索等研究方向，点击「****阅读原文****」即刻加入社区！**

我是彩蛋

**解锁新功能：热门职位推荐！**

PaperWeekly小程序升级啦

**今日arXiv√猜你喜欢√****热门职位****√**

找全职找实习都不是问题
**解锁方式**
1. 识别下方二维码打开小程序
2. 用PaperWeekly社区账号进行登陆
3. 登陆后即可解锁所有功能

**职位发布**
请添加小助手微信（**pwbot01**）进行咨询

**长按识别二维码，使用小程序**
*点击阅读原文即可注册

![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)


**关于PaperWeekly**

PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgl9qrwuXS7D8F2ZLyZNmqfWibCVlSbGBVCrd80blia0iaiaKuVk5p1tWP8tCaIiaYxiaQwiacIOlu9yOw6Mg/?)
▽ 点击 |阅读原文| 查看原论文


