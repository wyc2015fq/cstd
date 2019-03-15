# 当我们在谈论GBDT：从 AdaBoost 到 Gradient Boosting

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

144 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## 引言

GBDT 全称是 Gradient Boosting Decision Tree，是一种常用的 Ensemble Learning 算法。由于其出色的表现以及 XGBoost 的出现，近些年在 Kaggle 等竞赛中大放异彩。本系列旨在梳理 GBDT 相关的概念，并在 Kaggle 等竞赛中举例实践。

GBDT 作为一种 Ensemble Learning 算法，本质也就是融合多个弱分类器进行决策。从 GBDT 的名字可以看出，它包含两个概念：GB（Gradient Boosting） 和 DT（Decision Tree）。因此，GBDT 使用的弱分类器就是 Decision Tree，而融合的方法叫做 Gradient Boosting。但是在介绍 Gradient Boosting 前，我们先回顾一下相关的 Boosting 算法。

## 从 AdaBoost 到 Gradient Boosting

Boosting 是 Ensemble Learning 算法的一个类别。Boost 的意思为“提升”，这类算法的思想是“给定仅比随机猜测略好的弱学习算法 ,将其提升为强学习算法”。在2000年左右，Friedman 的几篇论文提出的几个 Boosting 算法，奠定了 Boosting 从一种思想走向实用的基础：

- 1997年，Freund 在 **A Decision-Theoretic Generalization of On-Line Learning and an Application to Boosting** 中，提出了第一个实用的 Boosting 算法并扩展——AdaBoost（两类分类），AdaBoost.M1（多类分类），AdaBoost.M2（多类分类），AdaBoost.R（回归）
- 2000年，Freund 在 **Additive logistic regression: a statistical view of boosting** 中，从加性模型的角度对 AdaBoost 的数学性质进行了研究，并扩展提出了Real AdaBoost、LogitBoost、Gentle AdaBoost 等两类分类 Boosting 算法
- 2001年，Freund 在 **Greedy function approximation: A gradient boosting machine**中，将梯度下降的思想引入 Boosting 算法，提出了Gradient Boosting 思想。Gradient Boosting 是一个 Boosting 框架，能处理不同的损失函数，较为实用

本文主要对其中部分算法进行理论上的介绍。

## AdaBoost

AdaBoost（Adaptive Boosting）针对的是两类分类问题。其弱学习算法![L](https://www.zhihu.com/equation?tex=L)也是一个两类分类器，可以根据需要选择。AdaBoost 的伪代码如下：



![img](https://pic2.zhimg.com/80/v2-b1db524dfb651d258928c0e175571555_hd.png)

可以看出：AdaBoost 本质就是，每次迭代更新样本分布，然后对新的分布下的样本学习一个弱分类器，和它对应的权重。更新样本分布的规则是：减小之前弱分类器分类效果较好的数据的概率，增大之前弱分类器分类效果较差的数据的概率。最终的分类器是弱分类器线性组合。



以下，我们以**指数损失函数**为例，对 AdaBoost 的原理进行说明。对理论不感兴趣的同学可以跳过。

**需要注意的是，按照作者的说法，“最小化指数分布损失函数”这并不是 AdaBoost 被设计的初衷。但是这提供了一个深入理解 AdaBoost，以及扩展 AdaBoost 的机会。**

**首先，**

我们有训练样本![(x_1, y_1),...,(x_m,y_m) ](https://www.zhihu.com/equation?tex=%28x_1%2C+y_1%29%2C...%2C%28x_m%2Cy_m%29+)，其中![y\in\{-1,1\}](https://www.zhihu.com/equation?tex=y%5Cin%5C%7B-1%2C1%5C%7D)。目标是训练出一个两类分类器![H(x)](https://www.zhihu.com/equation?tex=H%28x%29)，使损失函数最小，损失函数表达如下。（感谢

[@haijin jiang](https://www.zhihu.com/people/91e2911ae6be0c3032eb1ed3b8e29aa9)

 

提醒，这里需要说明下，下式中的



表示的是样本



对应的真实类别；同时，



应该理解为



，会与下文更连贯）：



![img](https://pic3.zhimg.com/80/v2-ad6872b443103e3488715501febb9cfe_hd.png)

同时，![H(x)](https://www.zhihu.com/equation?tex=H%28x%29)由若干个弱分类器线性组合而成

![img](https://pic4.zhimg.com/80/v2-7cb22f05ad084afaab6d78de136917eb_hd.png)

**随后**，我们需要证明如果求出![H](https://www.zhihu.com/equation?tex=H)，它是否是有效的分类器

为了求出令损失函数![\iota_{exp}](https://www.zhihu.com/equation?tex=%5Ciota_%7Bexp%7D)最小的![H](https://www.zhihu.com/equation?tex=H)，我们对其求导

![img](https://pic4.zhimg.com/80/v2-4ba4e744b0132c2d8d0410fc20a07e53_hd.png)

可以求得



![img](https://pic4.zhimg.com/80/v2-d91a64285cbce8c8b41cc2cd70cdf98f_hd.png)

而将



二值化，则能得到最终的二类分类器



![img](https://pic1.zhimg.com/80/v2-ea20c128c101475d9d9781083839f304_hd.png)

从上式能够看出，通过最小化损失函数![\iota_{exp}](https://www.zhihu.com/equation?tex=%5Ciota_%7Bexp%7D)，我们求出的![H(x)](https://www.zhihu.com/equation?tex=H%28x%29)能够最小化分类误差，所以![H(x)](https://www.zhihu.com/equation?tex=H%28x%29)是有效的

**随后**，由于弱分类器![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)是可以自己选择的，训练过程无需描述；但是需要求它对应的权值![\alpha_t](https://www.zhihu.com/equation?tex=%5Calpha_t)

![H(x)](https://www.zhihu.com/equation?tex=H%28x%29)是弱分类器![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)的线性组合，当第![t](https://www.zhihu.com/equation?tex=t)次迭代中，![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)已经训练完毕（步骤3），接下来就要求![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)对应的系数![\alpha_t](https://www.zhihu.com/equation?tex=%5Calpha_t)，使![\alpha_th_t(x)](https://www.zhihu.com/equation?tex=%5Calpha_th_t%28x%29)能最小化本轮的损失函数![\iota_{exp}](https://www.zhihu.com/equation?tex=%5Ciota_%7Bexp%7D)



其中，![\epsilon_t](https://www.zhihu.com/equation?tex=%5Cepsilon_t)为



为了求![\alpha_t](https://www.zhihu.com/equation?tex=%5Calpha_t)，对![\iota_{exp}](https://www.zhihu.com/equation?tex=%5Ciota_%7Bexp%7D)求导



可以求得![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)的权值![\alpha_t](https://www.zhihu.com/equation?tex=%5Calpha_t)

![img](https://pic3.zhimg.com/80/v2-1b5d7da4264eeeda7e962941294f9652_hd.png)

**之后**，需要明确如何通过前几轮的学习效果，确定下一个弱分类器

假设在第![t-1](https://www.zhihu.com/equation?tex=t-1)轮迭代后，我们得到了![t-1](https://www.zhihu.com/equation?tex=t-1)个弱分类器，此时将它们组合成强分类器![H_{t-1}](https://www.zhihu.com/equation?tex=H_%7Bt-1%7D)。接下来，我们希望下一个弱分类器![h_t](https://www.zhihu.com/equation?tex=h_t)能够纠正![H_{t-1}](https://www.zhihu.com/equation?tex=H_%7Bt-1%7D)中的所有错误。即，由于![h_t](https://www.zhihu.com/equation?tex=h_t)纠正了所有的错误，此时的损失函数![\iota_{exp}](https://www.zhihu.com/equation?tex=%5Ciota_%7Bexp%7D)应该最小

![img](https://pic4.zhimg.com/80/v2-0a51142711968a86d6925d842a13425f_hd.png)

对![e^{-f(x)h_t(x)}](https://www.zhihu.com/equation?tex=e%5E%7B-f%28x%29h_t%28x%29%7D)泰勒展开，且由于![f(x)^2=1](https://www.zhihu.com/equation?tex=f%28x%29%5E2%3D1)，![h_t(x)^2=1](https://www.zhihu.com/equation?tex=h_t%28x%29%5E2%3D1)

![img](https://pic1.zhimg.com/80/v2-0c69a50e811304e6c1c6f3b17d5af174_hd.png)

因此，理想的![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)应该能够有如下性质

![img](https://pic2.zhimg.com/80/v2-3ff0f74a30bdad7a47fe6c9101b15035_hd.png)

需要注意的是，上式最后两行，只是单纯除以了一个常数而已。有了这个常数，我们可以定义一个分布![D_t](https://www.zhihu.com/equation?tex=D_t)

![img](https://pic1.zhimg.com/80/v2-926260ea8ba07f1ecb12560088506100_hd.png)

于是，![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)可以被写成

![img](https://pic2.zhimg.com/80/v2-521b6825170cec218018b656a3b380e9_hd.png)

由于又有下式（其中![I(x)=1](https://www.zhihu.com/equation?tex=I%28x%29%3D1)当![x](https://www.zhihu.com/equation?tex=x)为真；![I(x)=0](https://www.zhihu.com/equation?tex=I%28x%29%3D0)当![x](https://www.zhihu.com/equation?tex=x)为假）

![img](https://pic4.zhimg.com/80/v2-84c0e919bddd3b9bf8d8c69d80c2a9e7_hd.png)

![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)最终可以写成

![img](https://pic1.zhimg.com/80/v2-aa6cd56d96c37c2466f33f115059fa1c_hd.png)

此时，![h_t(x)](https://www.zhihu.com/equation?tex=h_t%28x%29)只与定义出的分布![D_t](https://www.zhihu.com/equation?tex=D_t)相关，因此我们只需要求出![D_t](https://www.zhihu.com/equation?tex=D_t)的表达即可



上式最后一项是个常数，可以理解为将![D_t](https://www.zhihu.com/equation?tex=D_t)归一化为分布的系数，即得到了步骤7中的公式

## AdaBoost.SAMME（AdaBoost.M1改进版）

Freund 在1997年提出的 AdaBoost.M1 算法，本质上就是将 AdaBoost 的弱分类器从两类分类器，换成了多类分类器。此时 AdaBoost.M1 有个重要限制，就是每次迭代的弱分类器错误率小于0.5。这个限制对于二类分类是合适的，但是对于多类分类器是比较严格的。

在 **Hastie, Trevor, et al. "Multi-class AdaBoost." 2006** 中，Hastie 提出了AdaBoost.SAMME 。该算法通过最小化多类指数损失函数求得，并且作者也证明了此结果满足多类贝叶斯分类准则。即



AdaBoost.SAMME 的步骤与 AdaBoost 差不多，只是将步骤6中的系数公式变为了



AdaBoost.SAMME 详细步骤如下，其中，![K](https://www.zhihu.com/equation?tex=K)表示类别个数





在 AdaBoost.SAMME 中，为了使![\alpha^{(m)}>0](https://www.zhihu.com/equation?tex=%5Calpha%5E%7B%28m%29%7D%3E0)，需要![1-err^{(m)}>1/K](https://www.zhihu.com/equation?tex=1-err%5E%7B%28m%29%7D%3E1%2FK)，即弱分类器的效果大于随机猜测即可。这比 AdaBoost.M1 的约束宽松很多。

## Real AdaBoost

AdaBoost 中，弱分类器输出是类别，属于集合![\{-1,1\}](https://www.zhihu.com/equation?tex=%5C%7B-1%2C1%5C%7D)。在 Real AdaBoost 中，弱分类器输出的是属于每一类的概率。



## AdaBoost.MH

还有一种处理多类分类的方法，是将多类分类转换为两类分类问题，有 one-versus-rest 和 one-versus-one 两种方式：

一对多（one-versus-rest）即将![K](https://www.zhihu.com/equation?tex=K)类分类问题，转换为![K](https://www.zhihu.com/equation?tex=K)个两类分类问题，每个问题是“样本是否属于第![K](https://www.zhihu.com/equation?tex=K)类”

一对一（one-versus-one）即将![K](https://www.zhihu.com/equation?tex=K)类分类问题，转换为![K(K-1)/2](https://www.zhihu.com/equation?tex=K%28K-1%29%2F2)个两类分类问题，每个问题是“样本属于第![i](https://www.zhihu.com/equation?tex=i)类还是第![j](https://www.zhihu.com/equation?tex=j)类”

AdaBoost.MH 就是一对多的思想，伪代码如下



## Gradient Boosting 

Gradient Boosting 是一种 Boosting 的思想，它本质是，每一次建立模型是在之前建立模型损失函数的梯度下降方向。其步骤如下



接下来，我们从理论上推演上面的步骤

**首先**，

我们有样本![\{x,y\}_1^N](https://www.zhihu.com/equation?tex=%5C%7Bx%2Cy%5C%7D_1%5EN)，其中![x=\{x_1,...,x_n\}](https://www.zhihu.com/equation?tex=x%3D%5C%7Bx_1%2C...%2Cx_n%5C%7D)。目的是训练出![F^*](https://www.zhihu.com/equation?tex=F%5E%2A)，使损失函数![\psi(y,F(x))](https://www.zhihu.com/equation?tex=%5Cpsi%28y%2CF%28x%29%29)最小，即



**然后**，我们先推导如何在参数空间求最优解

假设这里的![F(x)](https://www.zhihu.com/equation?tex=F%28x%29)是由参数能确定的函数，即它其实是![F(x;P)](https://www.zhihu.com/equation?tex=F%28x%3BP%29)，这里![P](https://www.zhihu.com/equation?tex=P)表示![F(x;P)](https://www.zhihu.com/equation?tex=F%28x%3BP%29)中的参数序列。并且，我们只考虑加性模型，即



其中



因此，![F(x;P)](https://www.zhihu.com/equation?tex=F%28x%3BP%29)的求解从寻找最佳的函数![F^*](https://www.zhihu.com/equation?tex=F%5E%2A)，变成了寻找最佳的参数![P^*](https://www.zhihu.com/equation?tex=P%5E%2A)



为了能迭代求出![P^*](https://www.zhihu.com/equation?tex=P%5E%2A)，我们将它写成

![img](https://pic2.zhimg.com/80/v2-ed138c9aa7dc5423982707c2c28645a9_hd.png)

其中，![P_0](https://www.zhihu.com/equation?tex=P_0)表示初始设置的参数，![P_m](https://www.zhihu.com/equation?tex=P_m)表示后续第![m](https://www.zhihu.com/equation?tex=m)次迭代，求出的参数。

接下来求解![P^*](https://www.zhihu.com/equation?tex=P%5E%2A)使损失函数![\psi(y,F(x))](https://www.zhihu.com/equation?tex=%5Cpsi%28y%2CF%28x%29%29)最小，就是一个典型的梯度下降过程了。先求![P^*](https://www.zhihu.com/equation?tex=P%5E%2A)的梯度

![img](https://pic2.zhimg.com/80/v2-7254d96b9f84c814372d4491ef0ab1a1_hd.png)

其中

![img](https://pic3.zhimg.com/80/v2-7a1d1fc0a29ad47322433aa1b9f4acd2_hd.png)



随后，求出步长

![img](https://pic3.zhimg.com/80/v2-a95c1fa1ab0beb1ab3a3cac3d2e0aa9a_hd.png)

其中

![img](https://pic4.zhimg.com/80/v2-a4c4a5e85e01c3c3c9dfe5cac08abaff_hd.png)

迭代以上两步，直到收敛，既可求出![P^*](https://www.zhihu.com/equation?tex=P%5E%2A)

**随后**，我们将上述推导推广到函数空间

损失函数![\Phi(F(x))](https://www.zhihu.com/equation?tex=%5CPhi%28F%28x%29%29)为

![img](https://pic3.zhimg.com/80/v2-014ae347c85b75f98820391f364cf2e2_hd.png)

令![F^*](https://www.zhihu.com/equation?tex=F%5E%2A)为多个![f(x)](https://www.zhihu.com/equation?tex=f%28x%29)的线性组合

![img](https://pic2.zhimg.com/80/v2-9b9b4f87d22d2d2453147363214c054d_hd.png)

其中，![f_0(x)](https://www.zhihu.com/equation?tex=f_0%28x%29)为初始函数，![f_m(x)](https://www.zhihu.com/equation?tex=f_m%28x%29)为第![m](https://www.zhihu.com/equation?tex=m)次迭代求出的函数。使用梯度下降法求解![F^*](https://www.zhihu.com/equation?tex=F%5E%2A)，使损失函数最小

![img](https://pic3.zhimg.com/80/v2-c4791ceec68f96ee9baef1e550a81112_hd.png)

其中

![img](https://pic3.zhimg.com/80/v2-69f08ffdf46ef41b02f59a989d873df6_hd.png)

![img](https://pic3.zhimg.com/80/v2-04599773c7f85716e7da2d449aea6696_hd.png)

随后，求出步长





其中，



如下（交换了期望与求导）





但是，上述推导需要知道![(x,y)](https://www.zhihu.com/equation?tex=%28x%2Cy%29)的联合分布，而用训练样本估计出来的分布肯定是不准确的

**最后**，我们参考上面求解最优“参数/函数空间”的思想，得到在“有限样本集”的情况下的 Gradient Descent 模型

理论上，我们只需要在第![m](https://www.zhihu.com/equation?tex=m)次迭代的时候，求出最优的![(\beta_m,\alpha_m)](https://www.zhihu.com/equation?tex=%28%5Cbeta_m%2C%5Calpha_m%29)

![img](https://pic2.zhimg.com/80/v2-45701015003b163b35d65e72b5161dfd_hd.png)

并代入递推式即可，如下

![img](https://pic2.zhimg.com/80/v2-16b33d342769a00cd6a8cf4bb58bca81_hd.png)

但是，并不是所有的![\Psi(y,F)](https://www.zhihu.com/equation?tex=%5CPsi%28y%2CF%29)都能够直接求解出上式。同时，我们可以看到在加性模型下，![\beta h(x_i;\alpha)](https://www.zhihu.com/equation?tex=%5Cbeta+h%28x_i%3B%5Calpha%29)本身就可以看成是参数前进的方向。又根据梯度下降法，使参数前进方向为负梯度方向![-g_m(x_i)](https://www.zhihu.com/equation?tex=-g_m%28x_i%29)。于是，可以通过最小二乘法求![\alpha_m](https://www.zhihu.com/equation?tex=%5Calpha_m)，如下

![img](https://pic3.zhimg.com/80/v2-a187bc216f2369a97c6bc4607fef9ac6_hd.png)

接下来，将![\alpha_m](https://www.zhihu.com/equation?tex=%5Calpha_m)代入，并将![h(x_i;\alpha)](https://www.zhihu.com/equation?tex=h%28x_i%3B%5Calpha%29)作为梯度下降的方向，求解![\rho_m](https://www.zhihu.com/equation?tex=%5Crho_m)

![img](https://pic3.zhimg.com/80/v2-0bdcc27510d295665907f3d8a0048f62_hd.png)

最终的迭代式为

![img](https://pic1.zhimg.com/80/v2-cd51a4c852da9b1d4bc3d46d7f29e858_hd.png)

完整的 Gradient Descent 模型流程于是为

![img](https://pic1.zhimg.com/80/v2-eb95b8452e50ec207fd0260cd2b9a3dc_hd.png)



## 本系列其他文章：

[当我们在谈论GBDT：从 AdaBoost 到 Gradient Boosting](https://zhuanlan.zhihu.com/p/25096501?refer=data-miner)

[当我们在谈论GBDT：Gradient Boosting 用于分类与回归](https://zhuanlan.zhihu.com/p/25257856?refer=data-miner)

[当我们在谈论GBDT：其他 Ensemble Learning 算法](https://zhuanlan.zhihu.com/p/25443980)

当我们在谈论GBDT：在 Kaggle 等竞赛中实践