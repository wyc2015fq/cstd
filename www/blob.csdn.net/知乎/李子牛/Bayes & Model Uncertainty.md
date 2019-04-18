# Bayes & Model Uncertainty - 知乎
# 

## Intuition

这篇文章主要整理最近在思考的贝叶斯定理与神经网络参数的不确定性。为了理解模型的不确定性，不妨看下面这张图：
![](https://pic4.zhimg.com/v2-d1d95e5326ed7b4a62e52b8e831e352b_b.jpg)图片来自Chua, Kurtland, et al. &amp;amp;amp;quot;Deep reinforcement learning in a handful of trials using probabilistic dynamics models.&amp;amp;amp;quot; Advances in Neural Information Processing Systems. 2018.
这是一个回归问题，真实目标是一个sine函数，训练样本用带颜色的圆点表示，但是采样比较有限。采用boostrap训练两个模型，分别用红色和蓝色表示。可以看到两个模型在训练数据集上输出比较一致，在非训练数据集上输出非常不一样。这正是我们想到的结果，假设模型是一个概率模型，经过学习后，应该在见过数据集上输出的方差比较小点，表示置信度比较高；在没见过的数据集上输出的方法比较大点，表示置信度比较低。

这就是下面这篇文章的工作，利用Bayes衡量模型参数的不确定度。

> [Blundell, Charles, et al. "Weight uncertainty in neural networks."arXiv preprint arXiv:1505.05424(2015).](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1505.05424)

## Introduction

不妨先从监督学习的框架说起。

监督学习一般有最大似然法(MLE)和最大后验法(MAP)。假设固定的数据集 ![D=(x_i, y_i)_{i=1}^{N}](https://www.zhihu.com/equation?tex=D%3D%28x_i%2C+y_i%29_%7Bi%3D1%7D%5E%7BN%7D) 对于MLE而言，求解如下问题:

![\begin{equation} w^{MLE} = \arg\max_w \log P(w|D) \\ \end{equation}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bequation%7D+w%5E%7BMLE%7D+%3D+%5Carg%5Cmax_w+%5Clog+P%28w%7CD%29+%5C%5C+%5Cend%7Bequation%7D)

比如常见的最小二乘问题:

![w^{MLE} = \arg \min_w \sum_{i=1}^{N} ||f_w(x_i)- y_i||^2_2\\](https://www.zhihu.com/equation?tex=w%5E%7BMLE%7D+%3D+%5Carg+%5Cmin_w+%5Csum_%7Bi%3D1%7D%5E%7BN%7D+%7C%7Cf_w%28x_i%29-+y_i%7C%7C%5E2_2%5C%5C)

这么一个框架第一眼看上去没什么问题。但仔细想一下:  如果模型是凸的，我们可以得到唯一的最优值；如果模型是非凸的，可能得到很多组性能相同的![w](https://www.zhihu.com/equation?tex=w) 。对于一些问题而言，没关系，我们只要从中random选取一个即可。但是，可能对于另外一些问题，我们对不同的 ![w](https://www.zhihu.com/equation?tex=w) 有不同的偏好，比如：我们可能更喜欢 ![w=(1, 1) ](https://www.zhihu.com/equation?tex=w%3D%281%2C+1%29+) 而不是 ![w = (100, 0.01)](https://www.zhihu.com/equation?tex=w+%3D+%28100%2C+0.01%29) ，因为前者比较均衡一点。那么如何表达这种偏好呢？我们可以根据经验指定一个关于 ![w](https://www.zhihu.com/equation?tex=w) 的分布 ![p(w)](https://www.zhihu.com/equation?tex=p%28w%29) ，为了把这种偏好融合进参数优化，我们可以使用贝叶斯定理：

![\begin{equation} P(w|D） = \frac{P(D|w)P(w)}{P(D)}  \\ \end{equation}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bequation%7D+P%28w%7CD%EF%BC%89+%3D+%5Cfrac%7BP%28D%7Cw%29P%28w%29%7D%7BP%28D%29%7D++%5C%5C+%5Cend%7Bequation%7D)

那么我们的问题就是求解后验分布中最优的参数，即最大后验法：

![\begin{equation} \begin{split} w^{MAP} &= \arg\max_w \log P(w|D) \\ &= \arg \max_w \log \frac{P(D|w)P(w)}{P(D)} \\ &= \arg \max_w \log P(D|w) + \log P(w) \end{split} \end{equation}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bequation%7D+%5Cbegin%7Bsplit%7D+w%5E%7BMAP%7D+%26%3D+%5Carg%5Cmax_w+%5Clog+P%28w%7CD%29+%5C%5C+%26%3D+%5Carg+%5Cmax_w+%5Clog+%5Cfrac%7BP%28D%7Cw%29P%28w%29%7D%7BP%28D%29%7D+%5C%5C+%26%3D+%5Carg+%5Cmax_w+%5Clog+P%28D%7Cw%29+%2B+%5Clog+P%28w%29+%5Cend%7Bsplit%7D+%5Cend%7Bequation%7D)

作为一个例子，对于一个回归问题，假如先验采取一个后期分布， ![\log P(w) \propto ||w||^2](https://www.zhihu.com/equation?tex=%5Clog+P%28w%29+%5Cpropto+%7C%7Cw%7C%7C%5E2) ，即常见的L2正则。也就是说，先验起到了对不同解的选择作用。粗略地说，MAP=MLE + Regularization.

顺便提一下，假如我们可以对局部最优解进行先验建模，那么MAP就有可能跳出局部最优解，而得到全局最优解。

## Model Uncertainty

正如前面看到，对于一个模型输入 ![x](https://www.zhihu.com/equation?tex=x) ，得到 ![y](https://www.zhihu.com/equation?tex=y) 的不确定度，可以由两个方法: 1) ![P(y|x)](https://www.zhihu.com/equation?tex=P%28y%7Cx%29) 的随机性；2) ![w](https://www.zhihu.com/equation?tex=w) 的不确定度。前者一般由建模决定，这里讨论如何获得 ![w](https://www.zhihu.com/equation?tex=w) 的不确定度。不妨再次讨论先验 ![P(w)](https://www.zhihu.com/equation?tex=P%28w%29) ，假如这是一个高斯分布，那么pdf曲线越窄，表示 ![w](https://www.zhihu.com/equation?tex=w) 的可能取值范围越小，可以认为我们对 ![w](https://www.zhihu.com/equation?tex=w) 越确信；反之，如果pdf越宽，表示 ![w](https://www.zhihu.com/equation?tex=w) 的可能取值范围越大，可以认为我们对 ![w](https://www.zhihu.com/equation?tex=w) 越不确定。先验一般是不准的，我们自然想确定后验，即 ![P(w|D)](https://www.zhihu.com/equation?tex=P%28w%7CD%29) 。

这是一个棘手的问题，从贝叶斯定理中可以看到，要求 ![P(w|D)](https://www.zhihu.com/equation?tex=P%28w%7CD%29) ，我们需要知道 ![P(D)](https://www.zhihu.com/equation?tex=P%28D%29) (对于MAP则不存在这个问题，因为这个在MAP求解是一个无关最优解的常量)，而 ![P(D) = \sum_{w} P(D|w) P(w)](https://www.zhihu.com/equation?tex=P%28D%29+%3D+%5Csum_%7Bw%7D+P%28D%7Cw%29+P%28w%29) , 这意味着我们要对所有可能的 ![w](https://www.zhihu.com/equation?tex=w) 求和，很多情况下是intractable的。

那么不妨换种思路，用一个参数化的分布 ![Q_\theta(w)](https://www.zhihu.com/equation?tex=Q_%5Ctheta%28w%29) 去逼近 ![P(w|D)](https://www.zhihu.com/equation?tex=P%28w%7CD%29) 。如何衡量两个分布的"距离"呢？KL散度。

![KL(q||p) = \sum_x q(x) \log \frac{q(x)}{p(x)} \\](https://www.zhihu.com/equation?tex=KL%28q%7C%7Cp%29+%3D+%5Csum_x+q%28x%29+%5Clog+%5Cfrac%7Bq%28x%29%7D%7Bp%28x%29%7D+%5C%5C)

也就是说我们的问题变成了：

![\begin{equation} \begin{split}  KL(Q_\theta (w) || P(w|D)) &= \sum_{w} Q_\theta(w) \log \frac{Q_\theta(w)}{P(w|D)} \\ &= \sum_{w} Q_\theta(w) \log \frac{Q_\theta(w)P(D)} {P(D|w)P(w)} \\ &= \sum_{w} Q_\theta(w) \log \frac{Q_\theta(w)} {P(D|w)P(w)} + \sum_{w}Q_\theta(w)\log P(D) \\ &= \sum_{w} Q_\theta(w)\log\frac{Q_\theta(w)}{P(w)} - \sum_{w} Q_\theta(w)\log\ P(D|w) + \sum_{w}Q_\theta(w)\log P(D) \\ &= KL(Q_\theta(w)||P(w)) - \mathbb E_{Q_\theta(w) }[\log P(D|w)] + \log P(D)  \end{split} \end{equation}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bequation%7D+%5Cbegin%7Bsplit%7D++KL%28Q_%5Ctheta+%28w%29+%7C%7C+P%28w%7CD%29%29+%26%3D+%5Csum_%7Bw%7D+Q_%5Ctheta%28w%29+%5Clog+%5Cfrac%7BQ_%5Ctheta%28w%29%7D%7BP%28w%7CD%29%7D+%5C%5C+%26%3D+%5Csum_%7Bw%7D+Q_%5Ctheta%28w%29+%5Clog+%5Cfrac%7BQ_%5Ctheta%28w%29P%28D%29%7D+%7BP%28D%7Cw%29P%28w%29%7D+%5C%5C+%26%3D+%5Csum_%7Bw%7D+Q_%5Ctheta%28w%29+%5Clog+%5Cfrac%7BQ_%5Ctheta%28w%29%7D+%7BP%28D%7Cw%29P%28w%29%7D+%2B+%5Csum_%7Bw%7DQ_%5Ctheta%28w%29%5Clog+P%28D%29+%5C%5C+%26%3D+%5Csum_%7Bw%7D+Q_%5Ctheta%28w%29%5Clog%5Cfrac%7BQ_%5Ctheta%28w%29%7D%7BP%28w%29%7D+-+%5Csum_%7Bw%7D+Q_%5Ctheta%28w%29%5Clog%5C+P%28D%7Cw%29+%2B+%5Csum_%7Bw%7DQ_%5Ctheta%28w%29%5Clog+P%28D%29+%5C%5C+%26%3D+KL%28Q_%5Ctheta%28w%29%7C%7CP%28w%29%29+-+%5Cmathbb+E_%7BQ_%5Ctheta%28w%29+%7D%5B%5Clog+P%28D%7Cw%29%5D+%2B+%5Clog+P%28D%29++%5Cend%7Bsplit%7D+%5Cend%7Bequation%7D)

相应的优化问题为：

![\theta^* = \arg\min_\theta KL(Q_\theta(w)||P(w)) - \mathbb E_{Q_\theta(w) }[\log P(D|w)] \\](https://www.zhihu.com/equation?tex=%5Ctheta%5E%2A+%3D+%5Carg%5Cmin_%5Ctheta+KL%28Q_%5Ctheta%28w%29%7C%7CP%28w%29%29+-+%5Cmathbb+E_%7BQ_%5Ctheta%28w%29+%7D%5B%5Clog+P%28D%7Cw%29%5D+%5C%5C)

即 ![Q_\theta(w) \approx P(w|D)](https://www.zhihu.com/equation?tex=Q_%5Ctheta%28w%29+%5Capprox+P%28w%7CD%29) 要同时最小化与先验的"距离"，同时保证采样出来的 ![w](https://www.zhihu.com/equation?tex=w) 可以最大化似然。

## Comments
-  原文作者在MNIST上做了实验，给出了网络参数的信噪比结果，可以看到weight的随机性比较大。
![](https://pic1.zhimg.com/v2-ad0491ecd7389b52ac724d805d58ee1c_b.jpg)
在近似相同的精度下，而我复现出来的结果的weight的权重的随机性则比较小。
![](https://pic4.zhimg.com/v2-65c860d8fa38da7ac48c60314a0d191b_b.jpg)
2. ![P(w)](https://www.zhihu.com/equation?tex=P%28w%29) 可以认为是无限个模型的Ensemble。但是文章里采用的先验是一个单峰的高斯，个人感觉这种情况下不会出现无限个模型Ensemble的效果，合理的先验应该是个多峰的分布。

