# 【MLA首日报告摘要】周志华、马毅等教授分享机器学习最新进展 - 人工智能学家 - CSDN博客
2017年11月05日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：786
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWHyzQ6IiadjEAewVBL2RGZW2SaMhF0ibG0Hste49GbaaOWPR8E3KlMaPpm5Da5vaHcJY9MQJYXKKMw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：专知*
*概要：第15届中国机器学习及其应用研讨会今天11月4日在北京交通大学举行，海内外从事机器学习及相关领域研究的10余位专家与会进行学术交流。*
第15届中国机器学习及其应用研讨会今天11月4日在北京交通大学举行，海内外从事机器学习及相关领域研究的10余位专家与会进行学术交流，包括特邀报告、顶会论文交流、以及Top Conference Review等部分。
**1. 深度森林初探**
这是由机器学习西瓜书作者、南京大学周志华老师讲述的关于他最新集成学习研究成果-深度森林，一种对深度神经网络可替代性方法。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0ezNECkxaV9qCkZjb21cDEoOGM7Yt6RubT8elmRZdpV0erEOnBsOHCDK1NMuYPqmnfnDricgUNBhQ/640?wx_fmt=png)
*图示：级联森林结构的图示。级联的每个级别包括两个随机森林（蓝色字体标出）和两个完全随机树木森林（黑色）。假设有三个类要预测; 因此，每个森林将输出三维类向量，然后将其连接以重新表示原始输入。*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0ezNECkxaV9qCkZjb21cDEjPdxFBAhfMZescKpib6I6oNmLYXY7ibf6YVt4k4D9MCHgdBweyuYLeDg/640?wx_fmt=png)
*gcForest的整体架构*
gcForest在图像分类、人脸识别、音乐分类、情感分类等特定数据集上都取得了非常不错的分类效果，是非深度神经网络表现最好的方法。gcForest只是深度森林一个开始。有很多可探索的可能性和应用场景。
**2. Latent tree analysis**
香港科技大学张连文教授的报告。
Latent tree analysis seeks to model the correlations among a set of random variables using a tree of latent variables. It was proposed as an improvement to latent class analysis — a
 method widely used in social sciences and medicine to identify homogeneous subgroups in a population. It provides new and fruitful perspectives on a number of machine learning areas, including cluster analysis, topic detection, and deep probabilistic modeling.
 In this talk, I will give an overview of the research on latent tree analysis and various ways it is used in practice.
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0ezNECkxaV9qCkZjb21cDEWQmGPRv13LnB0ibmBPTndgpJB37uDxP6Tzuyr2Cf0xlxDHwVRBWA9Qw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw0ezNECkxaV9qCkZjb21cDEyXnBJDoib8eON5BoHxtS3NB1RHmatkehVeniaRFVEmy4AvME5ofkEhGg/640?wx_fmt=png)
**3****. Graph Refinement**
浙江大学张振跃教授的报告。
数据聚类方法的有效性非常受制于差异性或相似性图矩阵内涵的类属性特点。由于受多种因素的影响，图矩阵或高维数据本身的类属性通常比较模糊，即便是由局部邻域点构成的图矩阵也通常如此。在多源异尺度数据聚类中，图矩阵的类属性模糊性或矛盾性更为明显。在本报告中，我们将从三个角度考虑如何修正给定的图矩阵，提升图矩阵的类属性：（1）从多源数据的视角扭曲及图矩阵形模拟，恢复固有的一致性图矩阵；（2）从多源数据的稀疏邻域表达，构建一致化稀疏图矩阵；（3）从单源图矩阵的稀疏低秩逼近，修正图矩阵
 。我们将从理论基础、模型建立、算法设计和数值检验等方面说明上述图修正方法的合理及其有效性。
**4. Low-dimensional Structures and Deep Models for High-dimensional (Visual) Data**
加州大学伯克利分校马毅教授的报告。
We  discuss a class of models and techniques that can effectively model and extract rich low-dimensional structures in high-dimensional data such as images and videos, despite nonlinear
 transformation, gross corruption, or severely compressed measurements. This work leverages recent advancements in convex optimization from Compressive Sensing for recovering low-rank or sparse signals that provide both strong theoretical guarantees and efficient
 and scalable algorithms for solving such high-dimensional combinatorial problems. We illustrate how these new mathematical models and tools could bring disruptive changes to solutions to many challenging tasks in computer vision, image processing, and pattern
 recognition. We will also illustrate some emerging applications of these tools to other data types such as 3D range data, web documents, image tags, bioinformatics data, audio/music analysis, etc. Throughout the talk, we will discuss strong connections of
 algorithms from Compressive Sensing with other popular data-driven models such as Deep Neural Networks, providing some new perspectives to understand Deep Learning. 
**5. 回复神经网络学习**
四川大学张蕾教授的报告。
随着大数据时代的到来及深度神经网络的兴起，神经网络在图像理解、语音识别、自然语言处理等领域取得了令人瞩目的成功。回复神经网络作为神经网络的一种主要用于处理时序数据，广泛用于机器翻译、图像理解、情感分析、语音翻译等时序任务中。这一讲座将系统地对回复神经网络进行回顾，并针对其两个学习算法Back Propagation Through Time (BPTT) 和Real
 Time Recurrent Learning (RTRL) 进行介绍，并基于此对回复神经网络训练中存在的问题进行了“进一步的思考”。具体包括：（1）生物神经网络与人工神经网络；（2）回复神经网络的学习算法BPTT和RTRL；（3）回复神经网络训练过程中存在的“梯度消失”问题及相应的解决方法，基于此简要地介绍新的回复神经网络模型，如：Long Short-Term Memory (LSTM), Gated Recurrent Unit (GRU) 及 Recurrent Highway Network (RHN)等。
**6. Towards Understanding Deep Learning: Two Theories of Stochastic Gradient Langevin Dynamics**
北京大学王立威教授的报告。
Deep learning has achieved great success in many applications. However, deep learning is a mystery from a learning theory point of view. In all typical deep learning tasks, the number
 of free parameters of the networks is at least an order of magnitude larger than the number of training data. This rules out the possibility of using any model complexity-based learning theory (VC dimension, Rademacher complexity etc.) to explain the good
 generalization ability of deep learning. Indeed, the best paper of ICLR 2017 “Understanding Deep Learning Requires Rethinking Generalization” conducted a series of carefully designed experiments and concluded that all previously well-known learning theories
 fail to explain the phenomenon of deep learning.
**7.  大规模分类任务的结构化学习策略**
胡清华 教授 天津大学
随着数据规模的不断扩大，分类学习算法面临的任务也越来越复杂，分类学习的类别数从几个增长到几百个，甚至几万个。此时，不同的类别标签之间可能会形成复杂的结构关系。充分利用这种结构信息可显著提升分类性能和决策的可靠性。本报告将讨论结构化学习任务的特点、评价指标、特征评价和分类模型构造算法。
**8. Active Learning: Query Less for More**
黄圣君 副教授 南京航空航天大学
 In supervised learning, a large training set of labeled examples is usually required to train an effective model. However, in many real applications, there are plentiful unlabeled data but limited labeled data, and the acquisition of labels is costly. Active
 learning reduces the labeling cost by iteratively selecting the most valuable data to query their labels from the annotator. This talk will summarize some important issues in active learning, including the designing of selection criterion and query type, querying
 from imperfect annotators and fast selection from large scale unlabeled data. Our recent efforts towards solving these issues will be reported.
