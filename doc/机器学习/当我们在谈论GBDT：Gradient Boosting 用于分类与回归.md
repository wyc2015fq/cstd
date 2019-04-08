# 当我们在谈论GBDT：Gradient Boosting 用于分类与回归

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

75 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## 引言

在上一部分，我们介绍了GB（Gradient Boosting ）的概念。而GBDT（Gradient Boosting Decision Tree），其实就是将GB里的弱算法选择为决策树，比如CART（Classification And Regression Tree）。

但是GB毕竟只是一种求解 Boosting 的思想，到底是怎么用在分类与回归中的，将在这一部分介绍。**这一部分不会介绍决策树相关概念，一个是它相对简单，网上也有大量教程，但如果有机会以后出一个系列来进行更深入的介绍。**

首先我们回顾一下Gradient Boosting 的本质，就是训练出$F^*$，使损失函数$\psi(y,F(x))$最小，即
$$
F^*=\underset{F(x)}{\arg\min}E_{y,x}\psi(y,F(x))
$$
其求解步骤如下：



![img](https://pic2.zhimg.com/80/v2-9fbb970e03e82b6d6ef4040c1c7c0fad_hd.png)

所以，我们首先得定义出损失函数$\psi(y,F(x))$，才能谈求解的事情。接下来我们针对不同场景，介绍相应的损失函数。



## 回归

对于回归问题，定义好损失函数$\psi(y,F(x))$后，Gradient Boosting 不需要作出什么修改，计算出来的结果就是预测值。

**平方损失**

在实际回归中，最常用的$\psi(y,F(x))$之一，就是平方损失，即
$$
\psi(y,F)=(y-F)^2/2
$$
将它画出来，形状大致如下



![img](https://pic3.zhimg.com/80/v2-50ad07d8ef3b5e3c9334568c32cde0de_hd.png)

能看出来，它含义就是对较大的偏差有着很强的惩罚，并相对忽略较小的偏差。容易得到，$\psi(y,F(x))$的梯度为

![img](https://pic2.zhimg.com/80/v2-c2280be09605acbb85f189f3aa3d4da1_hd.png)

因此代入Algorithm 1，就有了将平方损失应用于Gradient Boosting 的 LS_Boost 方法，步骤如下



![img](https://pic2.zhimg.com/80/v2-bc77e54eb57d14c1fae8181397a62d09_hd.png)

**绝对值损失**

绝对值损失的定义，以及其梯度如下：

![img](https://pic3.zhimg.com/80/v2-a0c52cbfee1771f1cf4fa3c27dbcaf72_hd.png)

![img](https://pic1.zhimg.com/80/v2-2a0351588fc2e339c6d0ff1793c57d50_hd.png)

绝对值损失函数画出来图，形状如下

![img](https://pic2.zhimg.com/80/v2-d45dbd673fbd6d574185750aa99ad3e5_hd.png)

有了以上两项（损失函数与梯度），本质上就能够解Algorithm 1了。同时，**如果将弱学习算法设为决策树**，还能进一步推导出形式更简洁的算法形式。这部分公式推导，此处不再赘述，有兴趣的同学可以参看[原文](http://link.zhihu.com/?target=http%3A//xueshu.baidu.com/s%3Fwd%3Dpaperuri%3A%28405f49151571c7ea03b5332c1031ed6c%29%26filter%3Dsc_long_sign%26sc_ks_para%3Dq%253DGreedy%2Bfunction%2Bapproximation%253A%2BA%2Bgradient%2Bboosting%2Bmachine.%26tn%3DSE_baiduxueshu_c1gjeupa%26ie%3Dutf-8%26sc_us%3D13783016239361402484)。绝对值损失的 GBDT 算法流程如下

![img](https://pic1.zhimg.com/80/v2-da9d91c9ad86d7a2b144adcac47234d0_hd.png)

需要了解的是，使用绝对值损失，一般比平方损失更加稳健。

**Huber 损失**

还有一种叫Huber 损失，其定义为

![img](https://pic1.zhimg.com/80/v2-ab94045cd4221436d3dcd1ef87480d04_hd.png)

分段函数看起来不直观，它的图画出来大致是

![img](https://pic1.zhimg.com/80/v2-eff9f2d877ef93f70549db2398417564_hd.png)

从图和公式可以看出，它融合了平方损失和绝对值损失。当偏差较小时，采用平方差损失；当偏差较大时，采用绝对值损失；而参数![\delta](https://www.zhihu.com/equation?tex=%5Cdelta)就是用于控制偏差的临界值的。

按照作者的说法，对于正态分布的数据，Huber 损失的效果近似于平方差损失；而对于长尾数据，Huber 损失的效果近似于绝对值损失；而对于中等程度拖尾的数据，Huber 损失的效果要优于以上两者。

与平方差损失一样，**如果将弱学习算法设为决策树**，还能进一步推导出更具体的算法形式，即，Huber 损失的 GBDT 算法流程如下

![img](https://pic3.zhimg.com/80/v2-906ce552fdb4e28ef23ea432bffcdcb2_hd.png)

**三种损失函数与对应的梯度表**

![img](https://pic2.zhimg.com/80/v2-8cb6283bbf2e1077dfb02daf722c69d9_hd.png)

| Setting    | Loss Function | -    |
| ---------- | ------------- | ---- |
| Regression |               |      |
| Regression |               |      |
| Regression |               |      |



## 分类

在说明分类之前，我们先介绍一种损失函数。与常见的直接求预测与真实值的偏差不同，这种损失函数的目的是最大化预测值为真实值的概率。这种损失函数叫做对数损失函数（Log-Likehood Loss），定义如下



对于二项分布，![y^*\in\{0,1\}](https://www.zhihu.com/equation?tex=y%5E%2A%5Cin%5C%7B0%2C1%5C%7D)，我们定义预测概率为![p(x)=P(y^*=1)](https://www.zhihu.com/equation?tex=p%28x%29%3DP%28y%5E%2A%3D1%29)，即二项分布的概率，可得



即，可以合并写成



对于$p(x)​$与![F(x)](https://www.zhihu.com/equation?tex=F%28x%29)的关系，我们定义为



即，$F(x)=\frac{1}{2}log(\frac{p(x)}{1-p(x)})$ 当![p(x)=1](https://www.zhihu.com/equation?tex=p%28x%29%3D1)，![F(x)\to\infty](https://www.zhihu.com/equation?tex=F%28x%29%5Cto%5Cinfty)；当![p(x)=0](https://www.zhihu.com/equation?tex=p%28x%29%3D0)，![F(x)\to-\infty](https://www.zhihu.com/equation?tex=F%28x%29%5Cto-%5Cinfty)

**两类分类**

对于两类分类，![y\in\{-1,1\}](https://www.zhihu.com/equation?tex=y%5Cin%5C%7B-1%2C1%5C%7D)，我们先将它转成二项分布![y^*\in\{0,1\}](https://www.zhihu.com/equation?tex=y%5E%2A%5Cin%5C%7B0%2C1%5C%7D)，即令![y^*=(y+1)/2](https://www.zhihu.com/equation?tex=y%5E%2A%3D%28y%2B1%29%2F2)。

于是根据上面得到的，损失函数期望为



其中，![F(x)](https://www.zhihu.com/equation?tex=F%28x%29)定义为

![img](https://pic1.zhimg.com/80/v2-418c42039a65659200e478af1acf574c_hd.png)

接下来求出梯度

![img](https://pic4.zhimg.com/80/v2-07914e6e32532d9625ed8af612710a4f_hd.png)

这样，Gradient Boosting 需要的条件就准备齐了。

但是，**如果我们将弱算法设置为决策树**，并在求解步长的时候利用牛顿法，原算法能够得到如下更简洁的形式，即两类分类的 GBDT 算法流程如下

![img](https://pic1.zhimg.com/80/v2-975e17afba3f820b69d1e022feaac3a8_hd.png)

最后依据计算出来的![F_m(x)](https://www.zhihu.com/equation?tex=F_m%28x%29)分类即可。即，通过![F_m(x)](https://www.zhihu.com/equation?tex=F_m%28x%29)估算预测的概率

![img](https://pic1.zhimg.com/80/v2-8f9d583850c9197fc1eb7495a91ac050_hd.png)

然后根据以下准则预测标签，其中![c(\hat y,y)](https://www.zhihu.com/equation?tex=c%28%5Chat+y%2Cy%29)是代价函数，表示当真实类别为![y](https://www.zhihu.com/equation?tex=y)，预测类别为![\hat y](https://www.zhihu.com/equation?tex=%5Chat+y)时的代价

![img](https://pic2.zhimg.com/80/v2-1571e87355b09c6988fb6272a5282d69_hd.png)

**多类分类**

模仿上面两类分类的损失函数，我们能够将![K](https://www.zhihu.com/equation?tex=K)类分类的损失函数定义为

![img](https://pic4.zhimg.com/80/v2-74eee9e955abbfde9bfd35c207ac5217_hd.png)

其中，![p_k(x)=P(y_k=1|x_k)](https://www.zhihu.com/equation?tex=p_k%28x%29%3DP%28y_k%3D1%7Cx_k%29)，且将![p_k(x)](https://www.zhihu.com/equation?tex=p_k%28x%29)与![F_k(x)](https://www.zhihu.com/equation?tex=F_k%28x%29)关系定义为

![img](https://pic2.zhimg.com/80/v2-6e7fc52c1e26a2893282939f6056d58d_hd.png)

或者，换一种表达方式



![img](https://pic1.zhimg.com/80/v2-bb163a7c1fe96c481af9d88c9c9d59dc_hd.png)

接下来求出梯度



![img](https://pic3.zhimg.com/80/v2-a72017b87e36115c93ca4d7cefe126c6_hd.png)

于是可以看出，这里在每一次迭代，都要求![K](https://www.zhihu.com/equation?tex=K)个参数，和对应的![F_k(x)](https://www.zhihu.com/equation?tex=F_k%28x%29)。而求出的![F_k(x)](https://www.zhihu.com/equation?tex=F_k%28x%29)则可以理解为![x](https://www.zhihu.com/equation?tex=x)属于第![k](https://www.zhihu.com/equation?tex=k)类而不是其他类的概率。本质上就是OneVsRest的思想。

同上，**如果我们对弱算法选择决策树**，则有![K](https://www.zhihu.com/equation?tex=K)类分类的 GBDT 算法流程为

![img](https://pic1.zhimg.com/80/v2-0572e445509e2530cfd433b3964b033c_hd.png)

然后，根据上面的公式，将最终得到的![\{F_{kM}(x)\}_1^K](https://www.zhihu.com/equation?tex=%5C%7BF_%7BkM%7D%28x%29%5C%7D_1%5EK)转换为对应的类别概率![\{p_{kM}(x)\}_1^K](https://www.zhihu.com/equation?tex=%5C%7Bp_%7BkM%7D%28x%29%5C%7D_1%5EK)，并用于分类即可。分类准则如下

![img](https://pic1.zhimg.com/80/v2-95b03106cb7cbe2589c5c8f631b7fd58_hd.png)

其中，![c(k,k^\prime)](https://www.zhihu.com/equation?tex=c%28k%2Ck%5E%5Cprime%29)是代价函数，表示当真实类别为![k^\prime](https://www.zhihu.com/equation?tex=k%5E%5Cprime)，预测类别为![k](https://www.zhihu.com/equation?tex=k)时的代价

## 正则化

采取以上算法去训练测试样本集，能很好地拟合测试数据，相对不可避免地会产生过拟合。为了减少过拟合，可以从两个方面入手，即弱算法的个数![M](https://www.zhihu.com/equation?tex=M)，以及收缩率![v](https://www.zhihu.com/equation?tex=v)。

**弱算法的个数**

在推导 AdaBoost 的时候，我们就介绍过，我们希望训练出的![F(x)](https://www.zhihu.com/equation?tex=F%28x%29)是若干个弱算法的线性组合，即

![img](https://pic2.zhimg.com/80/v2-226a5a0d807a1d67a0cd9d9bf95cffbd_hd.png)

因此，这个![M](https://www.zhihu.com/equation?tex=M)的大小就影响着算法的复杂度。

一般来说，在训练阶段，我们通过交叉验证的方式，选择使损失最小的![M](https://www.zhihu.com/equation?tex=M)，并用于测试。

**收缩率**

前面介绍过，在第![m](https://www.zhihu.com/equation?tex=m)次迭代时，我们用如下公式更新![F_m(x)](https://www.zhihu.com/equation?tex=F_m%28x%29)

![img](https://pic1.zhimg.com/80/v2-61a2410900583598a4437e46a88c4468_hd.png)

而增加收缩率![v](https://www.zhihu.com/equation?tex=v)后，则更新公式变为

![img](https://pic1.zhimg.com/80/v2-cfa8a2d0f5f25e2f9387d7324c4785b0_hd.png)

即越往后训练出的弱算法，其在总算法中占得权重相对越低，于是真正有效的弱算法也就前面有限个，因而影响了算法的复杂度。

同样，在训练阶段，我们通过交叉验证的方式，选择使损失最小的![v](https://www.zhihu.com/equation?tex=v)，并用于测试。

不过，![M](https://www.zhihu.com/equation?tex=M)和![v](https://www.zhihu.com/equation?tex=v)是会相互影响的，一般减小![v](https://www.zhihu.com/equation?tex=v)，则对应的最优的![M](https://www.zhihu.com/equation?tex=M)会增加。因此，在选择参数时，应该综合考虑这两个参数的效果。

## 尾巴

在这一部分，我们看到不论在分类还是回归的Gradient Boosting，如果弱算法选择决策树，都能够一定程度简化求解思路；同时，决策树是个简单容易实现的弱算法，在实际中 GBDT 的表现也很好，相反太强太稳定的算法反而容易过拟合。希望这两篇文章不仅能帮助大家了解 GBDT 这类算法，更多的是能了解它整个演化的过程，对它有个更深的理解。

## 本系列其他文章：

[当我们在谈论GBDT：从 AdaBoost 到 Gradient Boosting](https://zhuanlan.zhihu.com/p/25096501?refer=data-miner)

[当我们在谈论GBDT：Gradient Boosting 用于分类与回归](https://zhuanlan.zhihu.com/p/25257856?refer=data-miner)

[当我们在谈论GBDT：其他 Ensemble Learning 算法](https://zhuanlan.zhihu.com/p/25443980)

当我们在谈论GBDT：在 Kaggle 等竞赛中实践