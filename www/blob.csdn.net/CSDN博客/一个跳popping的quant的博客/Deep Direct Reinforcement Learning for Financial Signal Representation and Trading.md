# Deep Direct Reinforcement Learning for Financial Signal Representation and Trading - 一个跳popping的quant的博客 - CSDN博客





2018年03月21日 18:14:13[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：2825
所属专栏：[一个用人工智能做量化的小白的进阶之路](https://blog.csdn.net/column/details/20644.html)









这篇论文对我个人的意义挺大的，毕竟是入坑智能交易看的第一篇论文，这篇论文前前后后看了也不下十多遍，抛去其技术性的方面，整篇论文的排版、写作方式以及实验的对比都有很大的借鉴意义。原文在百度学术和google学术都可以找到。


题目：


《Deep Direct Reinforcement Learning for Financial Signal Representation and Trading》

发表于《IEEE TRANSACTIONS ON NEURAL NETWORKS AND LEARNING SYSTEMS 》期刊的一篇SCI一区的论文。

时间：

2016/1





### 简介：

随着人工智能技术的发展，用训练好的计算机已经可以打败世界第一的围棋大师了，那能不能通过训练好的模型来打败有经验的交易员呢，这也是该parper想要做的目的。当时DL深度学习刚刚兴起，该paper就抓住了这个机会，通过构建这样的模型DL+RL->DRL来实现交易系统的构建，因而在论文中有一句：“ to the best of our knowledge, this is the first paper to implement DL in designing a real trading system for financial signal representation and self-taught reinforcement trading”。




### 应用技术：

这篇paper并没有提出什么新颖的算法和什么独特的技术，通篇也并没有什么数学推导和大堆的公式，但是应用的技术特别的多，无论是机器学习、强化学习还是深度学习、模糊学习都有涉及，是一篇应用性的paper。

从模型的训练方向上有这几种方法


参数初始化：

AutoEncoder和Kmeans

特征学习和去噪：

Fuzzy Learning


Deep Learning


决策执行：

Deep RNN





### 论文结构：

1、Introduction（综述论文，主要从决策执行RL和特征学习DL进行阐述）

2、Related works（介绍RL和DL相关知识）


3、Direct Deep Reinforcement Learning（正题部分，模型的构建顺序）

4、DRNN Learning（模型的初始化，训练方式）

5、Experimental Verifications（多种方法对比）




### 模型结构：

![](https://img-blog.csdn.net/20180321164137519)


论文中介绍的模型是一步步循序渐进的方式构建的，最终的模型如上。

就是通过输入的价格转换为向量，然后根据输入通过tanh函数进行求动作｛-1， 0， 1｝，这里的动作也只是个近似的计算tanh在(-1,  1)的范围中：

![](https://img-blog.csdn.net/20180321170009821)


然后根据价格差和动作计算t时间的收益：

![](https://img-blog.csdn.net/20180321170242267)


最终目的就是让收益最大：

![](https://img-blog.csdn.net/20180321170250633)





### 训练的过程是：

![](https://img-blog.csdn.net/20180321164528313)

概括就是：

1、先将输入的价格数据输入模型，用来初始化Fuzzy层和Deep层的参数。

2、通过BPTT方法展开网络，再通过BP算法进行对展开的网络进行梯度下降来更新RNN和DNN中的参数。

![](https://img-blog.csdn.net/20180321165416345)




### 实验对比：

训练和测试数据用的是商品期货（SU、AG）和股指期货（沪深300指数），红色是训练数据，后面的是测试数据。

![](https://img-blog.csdn.net/20180321170525160)


实验对比了很多常用的方法，包括一些DNNs和一些RLs方法，最后的衡量时用了SR和TP指标。

![](https://img-blog.csdn.net/20180321170821217)


![](https://img-blog.csdn.net/2018032116581893)


最后对手续费进行了探讨，结果是本文中的方法可以在手续费较高的情况下任然可以获利。

![](https://img-blog.csdn.net/20180321170929586)




### 总结：

1、创新之处来自与RL与DL的结合，文中提到是首篇paper。

2、实验充分，从多个角度（模型主体、交易费用等）进行了对比实验。

3、应用的技术多，将很多其他领域（如Fuzzy Learning）的技术应用于模型中。




个人拙见，仅供参考。ヾ(o◕∀◕)ﾉヾ










