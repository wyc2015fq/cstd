# 当我们在谈论GBDT：其他 Ensemble Learning 算法

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

9 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## 引言

在上一部分中，我们介绍了 Boosting 类算法，这是 Ensemble Learning 中常用的一类算法，每个迭代学习一个弱算法来降低损失，并将这些算法线性组合成强算法。还有另外一种 Ensemble Learning 算法 Bagging，通过学习出若干个不相关的弱算法，并将它们组合成强算法。这一部分我们主要介绍 Bagging，以及它的扩展形式 Random Forest。

## Bagging

Bagging 是 Bootstrap Aggregating 的缩写，如它的名字所示，它也包含两个步骤，即 Bootstrap Sampling 和 Aggregating。

Bootstrap Sampling 本质是一种有放回的采样方式，即，对于有![M](https://www.zhihu.com/equation?tex=M)个训练样本的原始样本集合，我们从中有放回的抽取![M](https://www.zhihu.com/equation?tex=M)个样本，作为一个 Bootstrap Sampling 样本集合。于是，在一个 Bootstrap Sampling 样本集合中，可能某个样本出现超过1次，也可能某个训练样本并没有出现过。重复进行![T](https://www.zhihu.com/equation?tex=T)次 Bootstrap Sampling，则能得到![T](https://www.zhihu.com/equation?tex=T)个Bootstrap Sampling 样本集合。对每个集合都能够单独训练一个弱算法，可以训练出![T](https://www.zhihu.com/equation?tex=T)个弱算法。

对于Bagging，它采用的 Aggregating 方法也很简单，即投票法。如对于1个样本的分类问题，先得到T个弱算法的结果，然后对结果进行投票，票数较多的类型即最终的类型。

Bagging 的算法步骤如下



![img](https://pic3.zhimg.com/80/v2-94b25937fbeb8ab2eaff9f35e71d18c6_hd.png)

Bagging 性质



假设，对有![n](https://www.zhihu.com/equation?tex=n)个训练样本的原始样本集合，我们采用 Bootstrap Sampling 的方式进行抽样![n](https://www.zhihu.com/equation?tex=n)次，则每个样本被抽到的次数满足二项分布，如下，显然其中![p=\frac1n](https://www.zhihu.com/equation?tex=p%3D%5Cfrac1n)



当![n\to\infty](https://www.zhihu.com/equation?tex=n%5Cto%5Cinfty)，且![p\to0](https://www.zhihu.com/equation?tex=p%5Cto0)时，令![\lambda=np](https://www.zhihu.com/equation?tex=%5Clambda%3Dnp)，此时每个样本被抽到的次数满足泊松分布，如下。显然，对于 Bootstrap Sampling 这个场景，![\lambda=np=1](https://www.zhihu.com/equation?tex=%5Clambda%3Dnp%3D1)



证明流程如下

![img](https://pic3.zhimg.com/80/v2-26b92a74cf21cb334ce12f99185fd952_hd.png)

因此，某个样本在整个抽取过程中，一次也没被抽到的概率![P(k=0)=\frac1e](https://www.zhihu.com/equation?tex=P%28k%3D0%29%3D%5Cfrac1e)。换句话说，在这次采样过程中，![\frac1e](https://www.zhihu.com/equation?tex=%5Cfrac1e)的训练样本未被抽中。

这是 Bagging 一个很好的性质：天然的交叉验证集。即，我们可以利用被抽中的样本，学习出一个弱算法。然后，再利用那些未被抽中的样本来估算这个弱算法的性能。为了更好的使用这些没有抽中的样本，它们还专门被命名为 out-of-bag 样本。

因此，对于利用 Bootstrap Sampling 学习出的![T](https://www.zhihu.com/equation?tex=T)个弱算法，我们将它们组合成强算法的方式如下。即，对某个样本的预测，仅使用那些 out-of-bag 样本包含此样本的弱算法

![img](https://pic3.zhimg.com/80/v2-8bfc87f13e42825673819ba3b3338c9a_hd.png)

对应的，其错误率被定义为

![img](https://pic2.zhimg.com/80/v2-a3cc8190e0b6837113ce9d2e8cd9fe21_hd.png)

这里还有一点需要说明，由于 Bagging 本质是利用 Bootstrap Sampling 来学习出多个弱算法，并将它们组合得到强算法。当用于训练的弱算法对训练样本不太敏感时（如![k](https://www.zhihu.com/equation?tex=k)近邻），即很稳定，则每个学习出的弱算法依然很相似，组合起来的效果也难以提高。因此，应当尽量选择那种对训练样本敏感的算法作为弱算法（决策树，非线性算法）。

## Rradom Forest

Rradom Forest 可以认为是 Bagging 算法的扩展形式：Bagging 通过 Bootstrap Sampling 的方式获得训练样本，训练每个弱分类器，减少弱分类器之间的相关性；而 Rradom Forest 则在此基础上，弱算法采用 CART 决策树算法，并同时在构建一课树时引入特征的随机性，进一步减少弱分类器之间的相关性。

在构建一棵树的时候，其步骤如下。需要注意的是，在构建树后并不进行剪枝操作

![img](https://pic2.zhimg.com/80/v2-2af64172564e0a162178a26e66d648a5_hd.png)

可以看出，这里的步骤跟原始的 CART 基本没有区别，最大的区别就在于第5步中：原始的 CART 从所有特征中选择最优的特征；Rradom Forest 则从所有特征中无放回的抽取![K](https://www.zhihu.com/equation?tex=K)个特征，再从中选择最优的。对于![K](https://www.zhihu.com/equation?tex=K)的选择，作者推荐可以使用![log_2(N)](https://www.zhihu.com/equation?tex=log_2%28N%29)，其中![N](https://www.zhihu.com/equation?tex=N)为总特征数。

最后，Rradom Forest 会将构造的树的结果合并：对于分类问题，可以使用投票法，即选择预测类别较多的一类作为结果；对于回归，可以将所有的结果求均值等。同时，由于 Rradom Forest 也使用了 Bootstrap Sampling，因此跟上面的 Bagging 一样可以利用 out-of-bag 样本更好的预测错误率。

更多的细节可以查看[原始文献](http://link.zhihu.com/?target=http%3A//xueshu.baidu.com/s%3Fwd%3Dpaperuri%3A%28c0070a6c001fcce334f260deb1951b2a%29%26filter%3Dsc_long_sign%26sc_ks_para%3Dq%253DRandom%2BForests%26tn%3DSE_baiduxueshu_c1gjeupa%26ie%3Dutf-8%26sc_us%3D4440798240814384271)。

## 尾巴

这一部分比较简短，一方面是最近比较忙碌，另一方面也是并没有太多可以讲。很多其他的 Ensemble Learning 算法也只是这几种基础算法上进行一些改动，扩展，或者用于不同的场景而已。以后有时间也可以适当进行一些补充。

## 本系列其他文章：

[当我们在谈论GBDT：从 AdaBoost 到 Gradient Boosting](https://zhuanlan.zhihu.com/p/25096501?refer=data-miner)

[当我们在谈论GBDT：Gradient Boosting 用于分类与回归](https://zhuanlan.zhihu.com/p/25257856?refer=data-miner)

[当我们在谈论GBDT：其他 Ensemble Learning 算法](https://zhuanlan.zhihu.com/p/25443980)

当我们在谈论GBDT：在 Kaggle 等竞赛中实践