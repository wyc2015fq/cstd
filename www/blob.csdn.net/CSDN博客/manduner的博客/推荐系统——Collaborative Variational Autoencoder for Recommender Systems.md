
# 推荐系统——Collaborative Variational Autoencoder for Recommender Systems - manduner的博客 - CSDN博客


2018年07月18日 08:51:08[Manduner_TJU](https://me.csdn.net/manduner)阅读数：745


# 论文描述：
*由于深度学习强大的表示学习能力，深度学习与传统推荐算法的结合的模型越来越多。最近，协同深度学习（CDL）和collaborative recurrent autoencoder已经被提出联合学习堆叠降噪自编码器和协同过滤，这两种模型通过一些降噪标准从内容中学习表示。首先通过掩盖某些部分或用通配符替换一些内容来破坏输入，然后使用神经网络重建原始输入。然而降噪自编码(DAE)实际上没有贝叶斯性质，并且DAEs的降噪方案实际上不是从概率的角度来看，而是从频率论的角度来看。这些模型要么很难应用上贝叶斯推理，要么需要很高的计算成本。
*为了解决上述问题，我们提出了一种称为协同变分自编码器（CVAE）的贝叶斯深度生成模型，在协同过滤中联合建模商品内容和评分信息。该模型以无监督方式学习商品内容信息的潜在表示。与降噪自编码器不同，CVAE模型不会破坏输入，而是寻求内在的概率潜在变量模型。它通过推理网络推断潜在变量的随机分布而不是点估计，这样可以产生更好的推荐性能。

# 参考文献：
Li X, She J. Collaborative Variational Autoencoder for Recommender Systems[C]// The, ACM SIGKDD International Conference. ACM, 2017:305-314.

