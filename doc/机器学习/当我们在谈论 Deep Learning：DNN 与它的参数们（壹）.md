# 当我们在谈论 Deep Learning：DNN 与它的参数们（壹）

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

29 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## 引言

在上一篇文章中我们介绍了 DNN 的基本结构，以及使用 BP（Backpropagation）求解![w, b](https://www.zhihu.com/equation?tex=w%2C+b)的说明，并简单介绍了在 DNN 中可以使用 Mini-batch GD（Stochastic Gradient Descent）来获得更好的结果。这几个方面算是 DNN 的基础，也即较为固定的。同时，还有其他一些方面，随着对 Deep Learning 认识、试错的增加，有过一些改变或发展，而且以后可能还会继续演进。接下来会从几个方面进行具体介绍：Activation Function、Cost Function、Optimization、Dropout 等。

## Activation Function

**Sigmoid**

最开始接触 ANN 的时候，大家听说的 Activation Function 应该还都是 Sigmoid 函数。它的定义如下：



其图形如下

![img](https://pic3.zhimg.com/80/v2-037a1bc549898a03986a777395e355a6_hd.png)

Sigmoid 函数优点很多：

1. 作为 Activation Function，它是单调递增的，能够很好地描述被激活的程度
2. Sigmoid 能将![(-\infty,+\infty)](https://www.zhihu.com/equation?tex=%28-%5Cinfty%2C%2B%5Cinfty%29)转换为![(0,1)](https://www.zhihu.com/equation?tex=%280%2C1%29)，避免数据在传递过程中太过发散，同时输出还能被理解成某种概率
3. Sigmoid 在定义域内处处可导，而且导数很好算。![f^{\prime}(x)=f(x)(1-f(x))](https://www.zhihu.com/equation?tex=f%5E%7B%5Cprime%7D%28x%29%3Df%28x%29%281-f%28x%29%29)，图形如下，可以看出![f^{\prime}(x)<1](https://www.zhihu.com/equation?tex=f%5E%7B%5Cprime%7D%28x%29%3C1)

![img](https://pic2.zhimg.com/80/v2-c1fa7c708b86f5c8ec74467fc3a6de21_hd.png)

但是，Sigmoid 的导数也带来了一些问题。在上一部分我们介绍过如何通过 BP 来计算![\frac{\partial C^r}{\partial w^l_{ij}}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+w%5El_%7Bij%7D%7D)，如下



假设![\sigma(z)](https://www.zhihu.com/equation?tex=%5Csigma%28z%29)为 Sigmoid 函数，有![\sigma^{\prime}(z)<1](https://www.zhihu.com/equation?tex=%5Csigma%5E%7B%5Cprime%7D%28z%29%3C1)。因此随着![l](https://www.zhihu.com/equation?tex=l)的减小，![\delta^l](https://www.zhihu.com/equation?tex=%5Cdelta%5El)会越来越小，从而![\frac{\partial C^r}{\partial w^l_{ij}}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+w%5El_%7Bij%7D%7D)也会越来越小。当 DNN 比较深的时，较前层的参数求出的梯度会非常小，几乎不会再更新，这种现象被称为 Gradient Vanish。

**ReLU**

为了缓解 Gradient Vanish 现象，现在大家都会使用 ReLU（Rectified Linear Unit），其定义如下



对应的图形如下

![img](https://pic3.zhimg.com/80/v2-8a82504cbf3df8b26f26aa46fbcc500e_hd.png)

ReLU 除了具有 Sigmoid 函数大部分的优点外，还有

1. 对某个神经元，当![x>0](https://www.zhihu.com/equation?tex=x%3E0)时，其导数为1，因而缓解了 Gradient Vanish 现象。因此，ReLU 也是最近几年非常受欢迎的激活函数
2. 对某个神经元，当![x<0](https://www.zhihu.com/equation?tex=x%3C0)时，其输出也是0，也就是对后续的网络不起作用，可以看作从网络中被移除了。因此在整个训练过程结束后，整个网络会呈现出一种稀疏的状态，也就是会有很多的神经元由于在网络中不起作用，可以当成不存在。这种稀疏也表明 ReLU 对避免网络过拟合有一定作用。

同时，ReLU 也有自己的缺陷：

1. 可以看出当![x<0](https://www.zhihu.com/equation?tex=x%3C0)时，不仅输出为0，ReLU 的导数也为0。即对应的参数不再更新。因此这个神经元再也没有被激活的机会了，这种现象被称为 dying ReLU
2. 第二个现象叫 Bias shift。在将数据输入 DNN 时我们一般会进行高斯归一，但是由于 ReLU 的输出恒大于0，会导致后续层输出的数据分布发生偏移。对于很深的网络，这可能会导致无法收敛。

**LReLU、PReLU**

为了解决 dying ReLU 的问题，有学者提出了 LReLU（Leaky Rectified Linear Unit）、PReLU（Prametric Rectified Linear Unit） 。它们被定义为



对应的图形如下

![img](https://pic3.zhimg.com/80/v2-5cb649b8b01a14e935bc34b4c3230932_hd.png)

LReLU 可以避免 dying ReLU，使神经元在任何输入下都能持续更新参数。对于 LReLU，![a_i](https://www.zhihu.com/equation?tex=a_i)是需要我们提前设定好的较小的数，比如0.01。但是，提前设定![a_i](https://www.zhihu.com/equation?tex=a_i)也带来了调参的难度。为了解决难以选择合适参数的问题，出现了 PReLU。

PReLU 公式跟 LReLU 是一样的，不同的是PReLU的![a_i](https://www.zhihu.com/equation?tex=a_i)是通过训练样本自动学习的。学习的方式依然是 BP 算法。但是，PReLU 在小数据下容易过拟合。具体的步骤可以参考"He, Kaiming, Zhang, Xiangyu, Ren, Shaoqing, and Sun, Jian. Delving deep into rectiers: Surpassing human-level performance on imagenet classication.2015."

**RReLU**

RReLU（Randomized Rectified Linear Unit）是 LReLU 的随机版本。它最早出现在[Kaggle NDSB](http://link.zhihu.com/?target=https%3A//www.kaggle.com/c/datasciencebowl)比赛中，定义如下



其中![a_{ji} \sim U(l,u),l<u\;\;and\;\;l,u\in [0,1)](https://www.zhihu.com/equation?tex=a_%7Bji%7D+%5Csim+U%28l%2Cu%29%2Cl%3Cu%5C%3B%5C%3Band%5C%3B%5C%3Bl%2Cu%5Cin+%5B0%2C1%29)

![img](https://pic4.zhimg.com/80/v2-23a511cece63571054896fac70dbb26f_hd.png)

在训练时，![a_{ji}](https://www.zhihu.com/equation?tex=a_%7Bji%7D)是一个保持在![(l,u)](https://www.zhihu.com/equation?tex=%28l%2Cu%29)内均匀分布的随机变量，而在测试中被固定为![\frac{l+u}2](https://www.zhihu.com/equation?tex=%5Cfrac%7Bl%2Bu%7D2)。关于 RReLU 并没有太多的文章，想进一步了解的话可以参考“Xu B, Wang N, Chen T, et al. Empirical Evaluation of Rectified Activations in Convolutional Network. 2015.”

**Others**

Activation Function 是一个比较发散的课题，在不同的任务中有不同的选择，暂时先不做更多的介绍。其它的 Activation Function 比如 Maxou、ELU 等，有兴趣的同学可以自己查找相关资料。

## Cost Function

**Softmax + Cross Entropy**

在 DNN 中进行多类分类时，使用的最多的 Cost Function 就是 Cross Entropy Loss，这里我们也主要介绍它。

首先，对于两个分布![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)与![q(x)](https://www.zhihu.com/equation?tex=q%28x%29)，交叉熵的定义为：



交叉熵用于衡量两个分布的相似性。当![p(x)](https://www.zhihu.com/equation?tex=p%28x%29)与![q(x)](https://www.zhihu.com/equation?tex=q%28x%29)的分布完全一致时，![H(p,q)](https://www.zhihu.com/equation?tex=H%28p%2Cq%29)最小。

对于分类问题，假设进行![K](https://www.zhihu.com/equation?tex=K)类分类，我们将某个样本![x](https://www.zhihu.com/equation?tex=x)的真实的类别标记![\hat y](https://www.zhihu.com/equation?tex=%5Chat+y)用向量![[\hat y_1,...,\hat y_k,...,\hat y_K]](https://www.zhihu.com/equation?tex=%5B%5Chat+y_1%2C...%2C%5Chat+y_k%2C...%2C%5Chat+y_K%5D)描述，其中



于是，![\hat y_k](https://www.zhihu.com/equation?tex=%5Chat+y_k)可以被理解为![x](https://www.zhihu.com/equation?tex=x)为第![k](https://www.zhihu.com/equation?tex=k)类的概率。因此，DNN 的输出![y](https://www.zhihu.com/equation?tex=y)应该也是一个向量![[y_1,...,y_k,...y_K]](https://www.zhihu.com/equation?tex=%5By_1%2C...%2Cy_k%2C...y_K%5D)，且需要与![[\hat y_1,...,\hat y_k,...,\hat y_K]](https://www.zhihu.com/equation?tex=%5B%5Chat+y_1%2C...%2C%5Chat+y_k%2C...%2C%5Chat+y_K%5D)尽可能的接近。为了描述这两个分布的相似性，于是就引入了 Cross Entropy Loss，其定义如下：



对于两类分类，![\hat y\in\{0,1\}](https://www.zhihu.com/equation?tex=%5Chat+y%5Cin%5C%7B0%2C1%5C%7D)，![C(y, \hat y)](https://www.zhihu.com/equation?tex=C%28y%2C+%5Chat+y%29)可以简化为：



还有一点需要注意，在上一章介绍DNN时，我们假设了 Output Layer 中从![z](https://www.zhihu.com/equation?tex=z)到![y](https://www.zhihu.com/equation?tex=y)的变换是普通的 Activation Function，即![\sigma](https://www.zhihu.com/equation?tex=%5Csigma)函数，如图

![img](https://pic3.zhimg.com/80/v2-834a63e61f7696b9eab74533109aec12_hd.png)

但是在多类分类时，![y_k](https://www.zhihu.com/equation?tex=y_k)表示的样本属于第![k](https://www.zhihu.com/equation?tex=k)类的概率。此时将![z_k](https://www.zhihu.com/equation?tex=z_k)转换为![y_k](https://www.zhihu.com/equation?tex=y_k)的操作即 Sotfmax，其公式如下



用图像来表述，如下

![img](https://pic3.zhimg.com/80/v2-60bbc400a35d3b76a08d9643bd9c1242_hd.png)

其实使用 Cross Entropy Loss 作为损失函数不是 DNN 的专属，在“[当我们在谈论GBDT：Gradient Boosting 用于分类与回归](https://zhuanlan.zhihu.com/p/25257856?refer=data-miner)”中介绍过 GBDT 在进行多类分类的时候就是使用的 Softmax + Cross Entropy，只是当时它被称为对数损失函数（Log-Likehood Loss）。有兴趣的同学可以回去看看那个部分。

最后需要说明的是，当我们使用 MSE（均方误差）作为 Cost Function ，会导致![w,b](https://www.zhihu.com/equation?tex=w%2Cb)的更新速度依赖于 Activation Function 的导数，在 Activation Function 为 Sigmoid 函数时很容易更新缓慢；而使用 Softmax + Cross Entropy Loss，能够有效克服这个问题。这也是 Cross Entropy Loss 在分类问题中使用较多的原因之一。这里先不展开介绍了，以后有机会再补充。

## Regularization 与 Weight Decay

上面讲完 Cost Function，按照套路，这个时候就要开始预防过拟合了，这里我们再讲一下 Regularization 相关。

**Weight Decay**

假设我们使用的是 L2 Regularization，则对应的公式如下：



其中，![\theta=\{W^1,W^2... \}](https://www.zhihu.com/equation?tex=%5Ctheta%3D%5C%7BW%5E1%2CW%5E2...+%5C%7D)，即所有![W](https://www.zhihu.com/equation?tex=W)的集合，注意这里只考虑了![W](https://www.zhihu.com/equation?tex=W)没有考虑![b](https://www.zhihu.com/equation?tex=b)（原因似乎是因为实验表明![b](https://www.zhihu.com/equation?tex=b)的约束对结果并没有太大的提升，不过暂时还没找到出处，以后看到再补充）；![||\theta||^2=\sum_l (w^l_{ij})^2](https://www.zhihu.com/equation?tex=%7C%7C%5Ctheta%7C%7C%5E2%3D%5Csum_l+%28w%5El_%7Bij%7D%29%5E2)，即所有![w](https://www.zhihu.com/equation?tex=w)的平方和；![\lambda](https://www.zhihu.com/equation?tex=%5Clambda)用于控制 Regularization 的程度，也叫 Weight Decay，越大则模型越倾向于简单。

在用 BP 更新参数时，有



于是在更新![w](https://www.zhihu.com/equation?tex=w)时，更新式为



可以看出，更新式比没有 Weight Decay 多了![(1-\eta\lambda)](https://www.zhihu.com/equation?tex=%281-%5Ceta%5Clambda%29)这一项；且由于它小于1，![w](https://www.zhihu.com/equation?tex=w)会在进行梯度下降的同时会被持续减小，这也是 Weight Decay 这个名称的由来。

**Regularization 的理解**

既然谈到了 Regularization，顺带谈一个经常被提及的问题——Regularization 的解释，即它到底是为什么会带来正则化的效果的。

第一种，是从 [PRML](http://link.zhihu.com/?target=https%3A//book.douban.com/subject/2061116/) 中看到的

对于 L1、L2 Regularization，他们的 Cost Function 分别可以写作



于是，我们可以写出它们的等价形式，如下



可以看出，我们其实是通过 L1 或 L2 约束将![w](https://www.zhihu.com/equation?tex=w)限制在一个空间中，在此基础上求出使 Cost Function 最小的![w](https://www.zhihu.com/equation?tex=w)。

假设现在考虑![w](https://www.zhihu.com/equation?tex=w)有两维，如下图。其中黄色区域分别是 L2 和 L1 约束，蓝色是 Cost Function 的等高线。蓝色与红色的切点即求出的![w](https://www.zhihu.com/equation?tex=w)，可以看出对于 L1 约束，![w](https://www.zhihu.com/equation?tex=w)更容易出现在坐标轴上，即只有一个维度上有非0值；而对于 L2 约束，则可能出现在象限的任何位置。这也是 L1 正则会带来稀疏解的解释之一。

![img](https://pic3.zhimg.com/80/v2-f68fd1337d98835004686cfacbf287f6_hd.png)

第二种，是从概率的角度

假设给定观察数据为![D](https://www.zhihu.com/equation?tex=D)，贝叶斯方法通过最大化后验概率估计参数![w](https://www.zhihu.com/equation?tex=w)，即



其中，![p(D|w)](https://www.zhihu.com/equation?tex=p%28D%7Cw%29)是似然函数，![p(w)](https://www.zhihu.com/equation?tex=p%28w%29)是参数的先验。

当![w](https://www.zhihu.com/equation?tex=w)服从![M](https://www.zhihu.com/equation?tex=M)维0均值高斯分布，即



代入上式，有



其中，第一项就是我们平时所定义的 Cost Function；第二项就是 L2 正则项；第三项当给定了![\alpha](https://www.zhihu.com/equation?tex=%5Calpha)那么就是常数了。所以我们可以看出， L2 正则本质其实是给模型参数![w](https://www.zhihu.com/equation?tex=w)添加了一个协方差为![\alpha^{-1}I](https://www.zhihu.com/equation?tex=%5Calpha%5E%7B-1%7DI)的零均值高斯分布先验。而![\alpha](https://www.zhihu.com/equation?tex=%5Calpha)越小，则协方差![\alpha^{-1}I](https://www.zhihu.com/equation?tex=%5Calpha%5E%7B-1%7DI)越大，表明先验对![w](https://www.zhihu.com/equation?tex=w)的约束越弱，模型的 variance 越大；反之，而![\alpha](https://www.zhihu.com/equation?tex=%5Calpha)越大，则协方差![\alpha^{-1}I](https://www.zhihu.com/equation?tex=%5Calpha%5E%7B-1%7DI)越小，表明先验对![w](https://www.zhihu.com/equation?tex=w)的约束越强，模型越稳定。这与我们平时的理解是吻合的。

当![w](https://www.zhihu.com/equation?tex=w)服从![M](https://www.zhihu.com/equation?tex=M)维0均值同分布 Laplace 分布时，即



代入上式，有



同样，第一项就是我们平时所定义的 Cost Function；第二项就是 L1 正则项；第三项当给定了![b](https://www.zhihu.com/equation?tex=b)那么就是常数了。一维 Laplace 分布如下：



其概率密度如下图所示，可以看出它取值的特点是很大概率落在一个小范围内。当![\mu=0](https://www.zhihu.com/equation?tex=%5Cmu%3D0)时，它会以很大概率取值在0的附近，这也就是 L1 约束会出现稀疏解的原因。

![img](https://pic2.zhimg.com/80/v2-0cc35ef2397fc06fd4c0d269284b31a5_hd.png)

## 尾巴

在梳理 DNN 相关知识时，感觉现阶段 DNN 相关的信息有一些特点：首先是涉及到的知识很广泛，却都比较零碎；其次，DNN 中对于参数的解释更多地需要意会，理论上能解释的特别好的并不太多。这种特点某种程度上也体现在了这篇文章中，可能也会体现在整个 DNN 系列中。同时，由于事情较多，每日写 BLOG 时间很有限，如果文章有什么错误或者建议，也欢迎指出。

## **本系列其他文章：**

Supervised Learning：

当我们在谈论 Deep Learning：DNN 与 Backpropagation

当我们在谈论 Deep Learning：DNN 与它的参数们（壹）

当我们在谈论 Deep Learning：DNN 与它的参数们（贰）

当我们在谈论 Deep Learning：DNN 与它的参数们（叁）

当我们在谈论 Deep Learning：CNN 其常见架构（上）

Unsupervised Learning：