# 最大似然估计 (Maximum Likelihood Estimation), 交叉熵 (Cross Entropy) 与深度神经网络 - LeftNotEasy - 博客园







# [最大似然估计 (Maximum Likelihood Estimation), 交叉熵 (Cross Entropy) 与深度神经网络](https://www.cnblogs.com/LeftNotEasy/p/mle-cross-entropy-and-deep-learning.html)





最近在看深度学习的"花书" （也就是Ian Goodfellow那本了），第五章机器学习基础部分的解释很精华，对比PRML少了很多复杂的推理，比较适合闲暇的时候翻开看看。今天准备写一写很多童鞋们w未必完全理解的最大似然估计的部分。

单纯从原理上来说，最大似然估计并不是一个非常难以理解的东西。最大似然估计不过就是评估模型好坏的方式，它是很多种不同评估方式中的一种。未来准备写一写最大似然估计与它的好朋友们，比如说贝叶斯估计 (Beyasian Estimation), 最大后验估计(Maximum Posterior Estimation)的关系。

(版权所有，转载请注明出处)

# 最大似然估计

如果写出最大似然估计的公式的话是这样的：

\[\theta_\text{ML}={arg\,max}_\theta P(Y|X;\theta)\]

假设一个模型是一个图片分类任务，从一个\(256 \times 256\)的图片来识别出模型到底是一个猫，一个狗，还是其他什么东西。那么输入就是\(256 \times 256\)的矩阵（图片），输出就是一个3维的向量，比如说\(\{0.1, 0,2,0.7\}\)代表从图片到对应三个分类的概率。

对于这个任务而言，我们需要学习的是一个给定分类函数的参数\(\theta\), 这个函数可以是任何函数，只要这个函数最终能输出三分类对应的概率向量就可以了。对于\(\theta\)而言，我们的目标是，对于给定的输入，输出值越接近真实值越理想，这个就是所谓的最大似然。

如果用模型训练的角度上来说，我们可以把公式展开成如下的形式：

\[{arg\,max}_\theta \sum_{i=1}^{n} log P(y^\text{(i)}|x^\text{(i)};\theta) \]

在这里，\(x^\text{(i)}\)和\(y^\text{(i)}\)分别代表一张训练数据的图片和训练数据图片代表的分类结果，在公式里面求和是希望考虑所有的样本情况。由于用于训练的图片是事先标注好的属于某一个类别，所以最终的概率向量的每一个分量不是\(0\)就是\(1\)，比如说\(\{0, 1,0\}\), \(\{0,0,1\}\)之类的。

那么问题来了，我们如何来判断目前\(\theta\)的输出概率更接近于真实的概率呢？在这里我们能用到一个现成的工具： KL Divergence (KL散度), KL 散度是用来判断两个概率分布的距离，其公式为（假设我们要判断从概率分布P到概率分布Q的距离）:

\[D_\text{KL}(P||Q)=-\sum_{i} P(i) log \frac{Q(i)}{P(i)}\]

所以说在我们这里就成为:

\[D_\text{KL}(Y||\hat{Y})=-\sum_{i} y^\text{(i)} log \frac {\hat{y}^\text{(i)}}{y^\text{(i)}}\]

在这里\(y^\text{(i)}\)指的是标注好的输出，而\(\hat{y}^\text{(i)}\)指的是算法的输出。

那么我们的优化目标就变成了使得\(D_\text{KL}\)最小。

# 交叉熵

这里不想叙述太多关于熵和交叉熵的基本知识，简单来说交叉熵是用来计算两个函数或者概率之间的距离，计算的方式也是使用的KL Divergence，在机器学习的世界里面大概可以认为交叉熵和最大似然估计是一回事，如果看到这两个术语应该把他们联系在一起。

具体可以参考下面的几篇文章：

1) [Andrew Moore关于信息论的Tutorial](http://www.cs.cmu.edu/%7E./awm/tutorials/infogain11.pdf)。

2) [A Friendly Introduction to Cross-Entropy Loss](https://rdipietro.github.io/friendly-intro-to-cross-entropy-loss/)

# 与深度学习的关系

那么最大似然估计（交叉熵）与深度神经网络有什么关系呢？如果你写过TensorFlow的程序的话，你会注意到程序的最后往往会加上一个Softmax函数。Softmax函数一个重要的性质就是把输出归一化转换到每一个对应分类的概率。一旦转换为概率之后，我们就可以用到最大似然估计（交叉熵）的方式来求得最大似然或者最小交叉熵。

在这里推荐Tobe同学的一篇[总结Cross Entropy在Tensorflow中应用](https://weibo.com/ttarticle/p/show?id=2309404047468714166594)的文章：

# 其他参考资料

1） [https://www.autonlab.org/_media/tutorials/mle13.pdf](https://www.autonlab.org/_media/tutorials/mle13.pdf)

2） [http://www.mi.fu-berlin.de/wiki/pub/ABI/Genomics12/MLvsMAP.pdf](http://www.mi.fu-berlin.de/wiki/pub/ABI/Genomics12/MLvsMAP.pdf)














