# 当我们在谈论 Deep Learning：DNN 与它的参数们（贰）

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

8 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## **Gradient Descent Optimization**

**Mini-batch SGD**

在[当我们在谈论 Deep Learning：DNN 与它的参数们（壹）](https://zhuanlan.zhihu.com/p/26122560)中我们介绍过 Mini-batch SGD，它是 GD 与 SGD 的一个折中选择，有着较好的收敛效果与速度。在此，我们回顾一下此算法的步骤

![img](https://pic2.zhimg.com/80/v2-a6fc115264faabfce8540844f711b189_hd.png)

对于![\theta=\theta-\epsilon \hat g](https://www.zhihu.com/equation?tex=%5Ctheta%3D%5Ctheta-%5Cepsilon+%5Chat+g)，我们可以这样理解：某次迭代的参数初始位置为![\theta](https://www.zhihu.com/equation?tex=%5Ctheta)，该参数以匀速![-\epsilon \hat g](https://www.zhihu.com/equation?tex=-%5Cepsilon+%5Chat+g)发生位移，时长为单位时间，得到的新的位置，即新的![\theta](https://www.zhihu.com/equation?tex=%5Ctheta)。由于![\hat g](https://www.zhihu.com/equation?tex=%5Chat+g)是根据训练样本的一个 Batch 求得的，所以变化可能会比较剧烈，也容易收噪声干扰，从而减慢收敛速度。

**Momentum**

为了提高 Mini-batch SGD 的收敛速度，利用动量（Momentum）来加速收敛的方法被提了出来，其步骤如下

![img](https://pic4.zhimg.com/80/v2-c1d9f3e513fda9cd94b757b4e013962f_hd.png)

可以看出，与 Mini-batch SGD 的区别，其中一个就是![\theta=\theta+v](https://www.zhihu.com/equation?tex=%5Ctheta%3D%5Ctheta%2Bv)这个步骤。即，参数改变的速度变成了![v](https://www.zhihu.com/equation?tex=v)，而不再是![-\epsilon \hat g](https://www.zhihu.com/equation?tex=-%5Cepsilon+%5Chat+g)。同时，![v=\alpha v-\epsilon g](https://www.zhihu.com/equation?tex=v%3D%5Calpha+v-%5Cepsilon+g)，即此时![-\epsilon g](https://www.zhihu.com/equation?tex=-%5Cepsilon+g)可以看作是速度![v](https://www.zhihu.com/equation?tex=v)的加速度，且![v](https://www.zhihu.com/equation?tex=v)以![\alpha](https://www.zhihu.com/equation?tex=%5Calpha)发生衰减。

于是有：

1. 参数以速度![v](https://www.zhihu.com/equation?tex=v)发生改变，![v](https://www.zhihu.com/equation?tex=v)的加速度是![-\epsilon g](https://www.zhihu.com/equation?tex=-%5Cepsilon+g)。此时，每一个 Batch 的![g](https://www.zhihu.com/equation?tex=g)的改变会以加速度的方式影响![v](https://www.zhihu.com/equation?tex=v)，而![v](https://www.zhihu.com/equation?tex=v)不会再突变（比如![v](https://www.zhihu.com/equation?tex=v)方向的剧烈改变），这就带来了一定的抗噪声能力
2. 当速度![v](https://www.zhihu.com/equation?tex=v)与加速度![-\epsilon g](https://www.zhihu.com/equation?tex=-%5Cepsilon+g)长期方向一致时，即使![-\epsilon g](https://www.zhihu.com/equation?tex=-%5Cepsilon+g)比较小，速度![v](https://www.zhihu.com/equation?tex=v)也会逐渐加快，从而加快收敛速度
3. 同时，可以通过设置![\alpha](https://www.zhihu.com/equation?tex=%5Calpha)来改变前期梯度![g](https://www.zhihu.com/equation?tex=g)对当前![v](https://www.zhihu.com/equation?tex=v)的影响。![\alpha](https://www.zhihu.com/equation?tex=%5Calpha)越小，则前期的梯度![g](https://www.zhihu.com/equation?tex=g)对现在的速度![v](https://www.zhihu.com/equation?tex=v)的影响越小。![\alpha](https://www.zhihu.com/equation?tex=%5Calpha)的含义可以类别成运动时的摩擦力，使参数不会无止境的改变下去。

**Nesterov Momentum**

Nesterov Momentum （**Sutskever I, Martens J, Dahl G, et al. On the importance of initialization and momentum in deep learning. 2013.**）是对 Momentum 方法的一种修改，又称为 NAG（Nesterov Accelerated Gradient），其步骤如下

![img](https://pic3.zhimg.com/80/v2-b843053e5d3e5f3c2696f058a4a3a4c6_hd.png)

可以看出，Nesterov Momentum 是先对新的参数进行了预估，![\theta=\theta+\alpha v](https://www.zhihu.com/equation?tex=%5Ctheta%3D%5Ctheta%2B%5Calpha+v)，再进行梯度的计算。虽然只是进行了细微的改变，Nesterov Momentum 在效果上比 Momentum 却有着显著的提升。关于对这种提升原因的分析，可以参考[比Momentum更快：揭开Nesterov Accelerated Gradient的真面目](https://zhuanlan.zhihu.com/p/22810533)，这里就不再赘述了。

**AdaGrad**

SGD 在参数更新的过程中，经常需要对![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)进行衰减，原因也是很直观的：前期参数距离最优解比较远，需要较大的更新速度；而当到达了最优解附近，则需要较小的速度以精细搜索最优解。

而以上介绍的方法，它们的学习率![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)都需要提前设定。如果想需要对![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)进行衰减，衰减的策略也需要自己去设置。对于多维数据，不同维度的数据还需要考虑是否需要不同的![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)衰减策略。于是，有学者在 Mini-Batch SGD 的基础上提出了自适应衰减![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)的方法（**Duchi J, Hazan E, Singer Y. Adaptive Subgradient Methods for Online Learning and Stochastic Optimization. 2011.**），其步骤如下

![img](https://pic2.zhimg.com/80/v2-438d3e6b55cc32b0e2a122c1b2b9120d_hd.png)

可以看出，它与 Mini-Batch SGD 的区别在于：![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)在更新的过程中一直在自适应的衰减。学习率![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)衰减的幅度与该维度上梯度![g](https://www.zhihu.com/equation?tex=g)的累计平方和成反比，即：如果某个维度上![g](https://www.zhihu.com/equation?tex=g)一直较大，则该维度上的学习率![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)衰减的较快；![g](https://www.zhihu.com/equation?tex=g)较小，则该维度上的学习率![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)衰减的较慢。

但按照《Deep Learning》的说法，在 DNN 的训练中，AdaGrad 在某些模型上效果不错，但并非全部。

**RMSProp**

RMSProp（**Tieleman, T. and Hinton, G. Lecture 6.5 - rmsprop, COURSERA: Neural Networks for Machine Learning.2012**）是 AdaGrad 的一种修改形式，用于改善 AdaGrad 在非凸场景效果不好的情况。RMSProp 的步骤如下

![img](https://pic4.zhimg.com/80/v2-60411b070556e9780324793a7962a90b_hd.png)

可以看出，RMSProp 与 AdaGrad 最大的区别就是在计算累计梯度时，多了一个衰减参数![\rho](https://www.zhihu.com/equation?tex=%5Crho)。

AdaGrad 在计算累计梯度时，使用的是过去所有梯度的平方和。于是，在更新参数的过程中，如果某些迭代计算出的![g](https://www.zhihu.com/equation?tex=g)使累计梯度太大，后续参数更新速度就会剧烈减少从而过早停止更新。

RMSProp 通过衰减参数![\rho](https://www.zhihu.com/equation?tex=%5Crho)，降低较为久远的![g](https://www.zhihu.com/equation?tex=g)的影响，从而改善了上述情况。由于其实践中较好的效果，RMSProp 也是目前 Deep Learning 中最常用的优化方法之一。

**RMSProp + Nesterov Momentum**

当我们将 RMSProp 和 Nesterov Momentum 融合，也即同时考虑了自适应更新学习率，以及 Momentum 的思想，就有如下算法

![img](https://pic3.zhimg.com/80/v2-3a22a7434e5d96b5b83d6302a0d1cf66_hd.png)

**Adam**

Adam 的全称是 Adaptive Moment（**Kingma, D. P., & Ba, J. L. Adam: a Method for Stochastic Optimization. 2015.**），同样也是一种融合了自适应学习率，以及 Momentum 思想的方法。其步骤如下

![img](https://pic2.zhimg.com/80/v2-5ce20ef5f4aeb64ade9e79fbf1af35d1_hd.png)

可以看出，Adam 就是在 RMSProp 的基础上，又考虑了 Momentum。

由于![\rho_1, \rho_2](https://www.zhihu.com/equation?tex=%5Crho_1%2C+%5Crho_2)的设置近似1，Adam 中的![s, r](https://www.zhihu.com/equation?tex=s%2C+r)其实分别是梯度![g](https://www.zhihu.com/equation?tex=g)的一阶矩、二阶矩的估计。因此，![\hat s/ \hat r](https://www.zhihu.com/equation?tex=%5Chat+s%2F+%5Chat+r)属于![[0,1]](https://www.zhihu.com/equation?tex=%5B0%2C1%5D)，于是![\Delta \theta](https://www.zhihu.com/equation?tex=%5CDelta+%5Ctheta)也有了约束。按照原文的说法，这个性质可以帮助我们更好地调节![\epsilon](https://www.zhihu.com/equation?tex=%5Cepsilon)；同时，《Deep Learning》中也提到，这使 Adam 对参数更加的鲁棒。

**Others**

不得不说，Gradient Descent Optimization 这个课题在 Deep Learning 火热起来后也越发热闹，各种方法络绎不绝。其它的方法如 Adamax、Nadam 等等，有兴趣的同学可以去进行更多的学习。而到底哪个方法最好，还是要根据实际场景选择，Mini-batch SGD 可能效果已经满足需求，而 RMSprop 是现阶段比较常用的方法之一。

## **Dropout**

Dropout 是 Hinton 在2014年提出的一种优化 DNN 的一种通用方法（**Srivastava N, Hinton G, Krizhevsky A, et al. Dropout: a simple way to prevent neural networks from overfitting. 2014**）。虽然作者在文章中并没有对 Dropout 的原理进行太多的理论分析，但是他用大量的对比实验证明此方法的有效性与效果的稳定性。而随后，Dropout 也确实在实践中被证明是非常简单，且有效提高 DNN 效果的一种优化方法。（恩，这很 Deep Learning。。。所以很多老 ML 专家不喜欢 Deep Learning 论文其实是可以被理解的。）

接下来简单介绍一下 Dropout 方法。

假设对于一般的 DNN，其结构如下。我们使用 Mini-batch SGD 的方式进行参数更新

![img](https://pic3.zhimg.com/80/v2-3f83d411ac52c66e3fece8937794cc86_hd.png)

此时，DNN 对应的 Feed Forward 的公式为

![img](https://pic1.zhimg.com/80/v2-c2efb01f6fb69609c37e81ea95648598_hd.png)

在训练时，对于一次 Mini-batch，我们设定一个概率![p](https://www.zhihu.com/equation?tex=p)，表示 DNN 中每个节点（包括输入节点，以及 Hidden Layer 的节点）被保留的概率。也就是 DNN 中的每个节点，都会以![1-p](https://www.zhihu.com/equation?tex=1-p)概率被移除。如下

![img](https://pic2.zhimg.com/80/v2-ef8e26888b635090d67bd7f316f39e75_hd.png)

与被移除的节相连的那些线在这次 Mini-batch 训练中也会被移除，于是 DNN 可以被看作如下结构。对这个结构中的参数应用 Mini-batch SGD 进行更新，被删除的结构对应的参数本次不更新。

![img](https://pic3.zhimg.com/80/v2-a4fe50622713d4c95a300fe7e7f391c6_hd.png)

将这种思想写成公式，即 Feed Forward 公式变成如下。其中，![r^{(l)}_j](https://www.zhihu.com/equation?tex=r%5E%7B%28l%29%7D_j)服从 Bernoulli 分布，即以![p](https://www.zhihu.com/equation?tex=p)的概率为1，以![1-p](https://www.zhihu.com/equation?tex=1-p)的概率为0。

![img](https://pic3.zhimg.com/80/v2-84ae5771b8581bce4667d561a02e6aea_hd.png)

在测试时，会使用初始的 DNN 结构，即

![img](https://pic3.zhimg.com/80/v2-ace8b8774413b4a21a245c9e5c2d84f6_hd.png)

但是，每个节点对应的参数![w](https://www.zhihu.com/equation?tex=w)需要乘以![p](https://www.zhihu.com/equation?tex=p)，即![W^{(l)}_{test}=pW^{(l)}](https://www.zhihu.com/equation?tex=W%5E%7B%28l%29%7D_%7Btest%7D%3DpW%5E%7B%28l%29%7D)。

在文章中，作者给出![p](https://www.zhihu.com/equation?tex=p)的建议值是：对于输入层，![p=0.8](https://www.zhihu.com/equation?tex=p%3D0.8)；对于 Hidden Layer，![p=0.5](https://www.zhihu.com/equation?tex=p%3D0.5)。

Dropout 可以看作是一种 Ensemble Learning 方法。Ensemble Learning 本质就是用同样的数据去训练不同的模型，然后进行融合；或者用不同的数据去训练相同的模型，然后再进行融合。在 DNN 的训练过程中，对于不同的 Epoch，相同的 Mini-batch，如果 Dropout 后的结构不同，那么就对应第一种情况；整个训练过程中，如果某两次 Dropout 后的结构相同，而 Mini-batch 不同，那么就对应第二种情况。所以，Dropout 也拥有 Ensemble Learning 的优点，即可以缓解 overfitting。

除此之外，Dropout 还有一些仿生学之类的解释。不过我一般都觉得这只是学者们在卖萌，有兴趣的同学可以参考原文。

## **尾巴**

迄今为止，我们已经简单介绍了几种 DNN 超参数：Activation Function、Cost Function、Regularization 、Gradient Descent Optimization、Dropout。但是这些参数每个都非常不好选择，是否有办法能做到减少需要调节的超参数，使 DNN 更智能呢？下一篇，我们会介绍被誉为“one of the most exciting recent innovations in optimizing DNNs”的方法—— Batch Normalization 以及相关概念。

## **Reference**

1. 文中 PPT 截图出自[李宏毅](http://link.zhihu.com/?target=http%3A//speech.ee.ntu.edu.tw/%7Etlkagk/index.html)老师的课件

## **本系列其他文章：**

Supervised Learning：

当我们在谈论 Deep Learning：DNN 与 Backpropagation

当我们在谈论 Deep Learning：DNN 与它的参数们（壹）

当我们在谈论 Deep Learning：DNN 与它的参数们（贰）

当我们在谈论 Deep Learning：DNN 与它的参数们（叁）

当我们在谈论 Deep Learning：CNN 其常见架构（上）

Unsupervised Learning：