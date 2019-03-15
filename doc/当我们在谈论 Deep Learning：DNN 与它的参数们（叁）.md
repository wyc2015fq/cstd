# 当我们在谈论 Deep Learning：DNN 与它的参数们（叁）

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

23 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## **引言**

在前两篇中，我们介绍了各种 DNN 中需要调节的参数，已经能窥见 DNN 参数是多么难以调节。而且，这还只是九牛一毛而已。在本篇中，我们会先简介一些“数据预处理”的方法，然后介绍由此演进出的 Batch Normalization 结构。Batch Normalization 最大的优点，就在于能缓解 DNN 在调参时的困难，是一种简单但是很实用的结构。

## **数据预处理**

众所周知，“数据预处理”是 ML 中的一个重要步骤，对 Feature Engineering 和结果有着巨大的影响。同样，在 DNN 中，一般数据也需要进行预处理，才会送入网络中进行训练或测试。这时，“数据预处理”是希望缓解以下问题：

- 范围、量纲统一：对每一维的特征，进行范围、量纲的统一，防止数值过大的维度影响分类效果
- 加速 Gradient Descent：特征不同维度的尺度、相关性等可能会使 Loss Function 的等高线变得复杂，进而减慢 Gradient Descent
- Covariate Shift：训练、测试样本的分布不一致时，模型无法很好地泛化。这个问题一般的处理办法是 Domain Adaption

接下来，我们介绍一些 DNN 中常见的“数据预处理”方法

**Mean Subtraction 与 Z-Score**

Mean Subtraction 其实就是将每一维的特征的向量![x](https://www.zhihu.com/equation?tex=x)中心搬到原点，使均值为0。其公式如下，其中![\mu](https://www.zhihu.com/equation?tex=%5Cmu)是向量![x](https://www.zhihu.com/equation?tex=x)的均值



Z-Score 是在 Mean Subtraction 的基础上，使![x](https://www.zhihu.com/equation?tex=x)的均值为0，标准差为1。其公式如下，其中![\mu](https://www.zhihu.com/equation?tex=%5Cmu)是向量![x](https://www.zhihu.com/equation?tex=x)的均值，![\sigma](https://www.zhihu.com/equation?tex=%5Csigma)为标准差



如果用二维数据来展示，则原始数据、Mean Subtraction、Z-Score 后的图像分别为



![img](https://pic1.zhimg.com/80/v2-586b7a1f490fddefcc6e3e0d1d88e27c_hd.png)

Mean Subtraction 和 Z-Score 虽然简单，却是统一范围、量纲的最常用方法，且能显著提高 Gradient Descent 的速度。



**PCA 与 Whitening**

鉴于 PCA（Principle Components Analysis）是比较通识的技术了，这里只简单回顾一下其步骤。对于训练样本集![\hat X=[\hat x^{(1)},...,\hat x^{(i)},...,\hat x^{(N)}]](https://www.zhihu.com/equation?tex=%5Chat+X%3D%5B%5Chat+x%5E%7B%281%29%7D%2C...%2C%5Chat+x%5E%7B%28i%29%7D%2C...%2C%5Chat+x%5E%7B%28N%29%7D%5D)，其 PCA 部分步骤如下



需要注意的是，![\Sigma](https://www.zhihu.com/equation?tex=%5CSigma)是样本不同维度之间的协方差矩阵。一般我们为了求![\Sigma](https://www.zhihu.com/equation?tex=%5CSigma)的特征值，都是直接对![X](https://www.zhihu.com/equation?tex=X)做 SVD，即![USV^T=X](https://www.zhihu.com/equation?tex=USV%5ET%3DX)。这里写作![USV^T=\Sigma](https://www.zhihu.com/equation?tex=USV%5ET%3D%5CSigma)，是为了顺便说明，对![\Sigma](https://www.zhihu.com/equation?tex=%5CSigma)求特征值、对![\Sigma](https://www.zhihu.com/equation?tex=%5CSigma)做 SVD、对![X](https://www.zhihu.com/equation?tex=X)做 SVD，其实都是等价的。具体的说明，可以参考[从PCA和SVD的关系拾遗](http://link.zhihu.com/?target=http%3A//blog.csdn.net/Dark_Scope/article/details/53150883)。

在 SVD 后，![U](https://www.zhihu.com/equation?tex=U)是![X](https://www.zhihu.com/equation?tex=X)一组相互正交的基向量，且其每一列为![\Sigma](https://www.zhihu.com/equation?tex=%5CSigma)的特征值；![S](https://www.zhihu.com/equation?tex=S)的对角线上的值为![X](https://www.zhihu.com/equation?tex=X)的奇异值，且是![\Sigma](https://www.zhihu.com/equation?tex=%5CSigma)的非零特征值的平方。

此时，我们可以将![X](https://www.zhihu.com/equation?tex=X)投影到新的基![U](https://www.zhihu.com/equation?tex=U)上，得到![X_{proj}=U^TX](https://www.zhihu.com/equation?tex=X_%7Bproj%7D%3DU%5ETX)。有![X_{proj}](https://www.zhihu.com/equation?tex=X_%7Bproj%7D)不同维度的协方差矩阵![\Sigma_{proj}](https://www.zhihu.com/equation?tex=%5CSigma_%7Bproj%7D)为对角阵，推导如下



即，![X_{proj}](https://www.zhihu.com/equation?tex=X_%7Bproj%7D)的每个维度之间不相关。当然，还可以利用![U](https://www.zhihu.com/equation?tex=U)和![S](https://www.zhihu.com/equation?tex=S)进行特征降维，不是这里的重点，不再赘述。

接下来，我们介绍 Whitening。

利用 PCA 的一些步骤，我们对![\hat X](https://www.zhihu.com/equation?tex=%5Chat+X)每一维去相关，得到了![X_{proj}](https://www.zhihu.com/equation?tex=X_%7Bproj%7D)，且其每一维都是零中心。Whitening 就是在此基础上更进一步，使![X_{proj}](https://www.zhihu.com/equation?tex=X_%7Bproj%7D)每一维有相同的方差。上面已经介绍过，![X_{proj}](https://www.zhihu.com/equation?tex=X_%7Bproj%7D)的协方差矩阵为对角阵![S](https://www.zhihu.com/equation?tex=S)，于是我们对![X_{proj}](https://www.zhihu.com/equation?tex=X_%7Bproj%7D)每一维分别除以对应的标准差，即![S](https://www.zhihu.com/equation?tex=S)对角线对应元素的开方。

用二维数据来展示，则原始数据、去相关、Whitening 后的图像分别为



![img](https://pic3.zhimg.com/80/v2-a21024d2ec6b76127e5c05458ba2a4d2_hd.png)

Whitening 后能提高 Gradient Descent 的速度，且数据性质很好（去相关，零均值等方差）。但是，由于 Whitening 需要进行 SVD， 计算量会比较大。



## **Batch Normalization**

**WHY Batch Normalization ?**

前面介绍的都是数据预处理的方法，但是在 DNN 中，除了面对量纲、GD 速度慢、Covariate Shift 这些 ML 共有的问题，还有一个独特的问题，就是 Internal Covariate Shift。

\1. Internal Covariate Shift 是作者在 [Batch Normalization 论文](http://link.zhihu.com/?target=http%3A//xueshu.baidu.com/s%3Fwd%3Dpaperuri%3A%284634f864791a3f3a0817edabeacf4c49%29%26filter%3Dsc_long_sign%26sc_ks_para%3Dq%253DBatch%2BNormalization%253A%2BAccelerating%2BDeep%2BNetwork%2BTraining%2Bby%2BReducing%2BInternal%2BCovariate%2BShift%26tn%3DSE_baiduxueshu_c1gjeupa%26ie%3Dutf-8%26sc_us%3D12337223955103602384)中，仿照 Covariate Shift 提出的概念。具体来说，对 DNN 某一层，随着 GD 训练的过程中参数的改变，该层的输出数据的分布可能会改变；此时，对于下一层，相当于输入数据的分布改变了，这就类似于 Covariate Shift。这种输入数据分布的改变，可能会使 DNN 的难以学习到好的参数，从而影响 DNN 的效果

\2. 同时，DNN 为了补偿输入数据分布改变带来的损失，需要更多的时间来调整参数，这可能会使 GD 的速度下降

\3. 同时，DNN 还有 Gradient Vanish 的问题（参考[当我们在谈论 Deep Learning：DNN 与它的参数们（壹）](https://zhuanlan.zhihu.com/p/26122560)）。当输入分布改变，可能使本层的 Activation Function 饱和，进而导致 Gradient Vanish。而随着深度的增加，较深的层会受到前面层参数变化的影响叠加，它输入分布的改变可能会更明显

*这里有一点需要明确，对于上面的 Internal Covariate Shift，我是根据原始论文写出的自己的理解。我还见过一种说法，是这样理解 Internal Covariate Shift：对 DNN 每一层，经过了该层的变换，其输出数据与输入数据的分布一般是不会一样的，这种分布的差异随着深度的增加会越发明显，但是它们描述的样本与标记仍然是一样的。个人认为与原文中想表达的有所偏差，但是也列在这里，以免因为自己的原因误导大家。有兴趣的同学可以参考原文，也欢迎讨论与指导，毕竟真理越辩越明嘛*

**BN 步骤**

增加了 Batch Normalization 的 DNN，其训练步骤如下：

1. 增加 BN 结构：对 DNN 中每一个 Activation，在它们前面放置一个 BN Layer（Batch Normalization Layer）。相当于以前的将![Wu+b](https://www.zhihu.com/equation?tex=Wu%2Bb)输入 Activation Function，现在将![BN(Wu+b)](https://www.zhihu.com/equation?tex=BN%28Wu%2Bb%29)输入 Activation Function。至于为什么是在 Activation Function 前放置，而非整个 Hidden Layer 前放置，我们下面会解释
2. 求解参数：利用 BP 求解 DNN 中的参数

所以，DNN 与 BN+DNN 的结构分别如下(下图中的文中 Hidden Layer 请脑补成 Activation)

![img](https://pic4.zhimg.com/80/v2-d81243d26a2258535d993ecf3d0d1ad7_hd.png)

**BN 结构**

Batch Normalization 中的 Batch，跟 miniBatch 是一样的，说明它也是针对一个 Batch 进行的操作。

对于一个样本，假设有这么一个标量![x](https://www.zhihu.com/equation?tex=x)，![x](https://www.zhihu.com/equation?tex=x)可以理解成这个样本的某一维特征的值，或者是这个样本输入到 DNN 后某个 Activation Function 的输入值。那么对于一个 Batch，假设对应的![x](https://www.zhihu.com/equation?tex=x)的集合为![ B=\{x_1,...x_i,...x_m\}](https://www.zhihu.com/equation?tex=+B%3D%5C%7Bx_1%2C...x_i%2C...x_m%5C%7D)，于是其对应的 BN Layer 的输出集合![\{y_i\}](https://www.zhihu.com/equation?tex=%5C%7By_i%5C%7D)就通过以下方式计算

![img](https://pic2.zhimg.com/80/v2-aaaf57d7537b7131318dbc733eb1cacd_hd.png)

其步骤如下：

1. 对一个 Batch 中的样本![x_i](https://www.zhihu.com/equation?tex=x_i)，进行 Z-Score 归一化得到![\hat x_i](https://www.zhihu.com/equation?tex=%5Chat+x_i)
2. 由于 Z-Score 归一化后对![\hat x_i](https://www.zhihu.com/equation?tex=%5Chat+x_i)的范围增加了很强的约束，为了让 DNN 学习出合适的输入，利用 scale ![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma)和 shift ![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)对![\hat x_i](https://www.zhihu.com/equation?tex=%5Chat+x_i)进行变换。不过，这里![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma)和![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)都是网络自己学习出来的

有几个问题需要在这里进行说明：

1. 为什么是对 Activation Function 的输入进行 BN（即![BN(Wu+b)](https://www.zhihu.com/equation?tex=BN%28Wu%2Bb%29)），而非对 Hidden Layer 的每一个输入进行 BN（![W*BN(u)+b](https://www.zhihu.com/equation?tex=W%2ABN%28u%29%2Bb)）。按照作者的解释，由于 Hidden Layer 的输入![u](https://www.zhihu.com/equation?tex=u)是上一层非线性 Activation Function 的输出，在训练初期其分布还在剧烈改变，此时约束其一阶矩和二阶矩无法很好地缓解 Covariate Shift；而![BN(Wu+b)](https://www.zhihu.com/equation?tex=BN%28Wu%2Bb%29)的分布更接近 Gaussian Distribution，限制其一阶矩和二阶矩能使输入到 Activation Function 的值分布更加稳定
2. 还有![y_i=\gamma\hat x_i+\beta](https://www.zhihu.com/equation?tex=y_i%3D%5Cgamma%5Chat+x_i%2B%5Cbeta)这一步的意义，即为什么在对数据进行归一化后还要再做变换。我觉得这个跟 AutoEncoder 的意义是类似的，即 AutoEncoder 是一种智能的 PCA，BN 则是一种智能的 Z-Score。不过这些都只是感性的理解罢了，并没什么严谨的证明就是了。

**BN + BP**

在 DNN 中增加了 BN Layer 后，即增加了参数![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma)和![\beta](https://www.zhihu.com/equation?tex=%5Cbeta)，BP 也相应地发生了改变。这里，我参考 [Understanding the backward pass through Batch Normalization Layer](http://link.zhihu.com/?target=https%3A//kratzert.github.io/2016/02/12/understanding-the-gradient-flow-through-the-batch-normalization-layer.html)，以 Computational Graph 的方式来简单解释 BN 的 BP。

这里，我们定义![x=[x_1,...x_i,...x_m]^T](https://www.zhihu.com/equation?tex=x%3D%5Bx_1%2C...x_i%2C...x_m%5D%5ET)，其中![m](https://www.zhihu.com/equation?tex=m)表示一个 Batch 中样本的数量，![x_i](https://www.zhihu.com/equation?tex=x_i)是一个样本对应的某个标量，比如理解成 DNN 中某个节点的 Activation Function 的输入。于是，![x](https://www.zhihu.com/equation?tex=x)是一个![m](https://www.zhihu.com/equation?tex=m)维的列向量，这里跟 [Understanding the backward pass through Batch Normalization Layer](http://link.zhihu.com/?target=https%3A//kratzert.github.io/2016/02/12/understanding-the-gradient-flow-through-the-batch-normalization-layer.html) 定义不同，主要是为了让后续的公式更好理解。

根据 BN 的公式，我们能得到其 FP（Forward Propagation） 的 Computational Graph，如下

![img](https://pic2.zhimg.com/80/v2-59a2b3439c2bd0129ae5bff52788ede5_hd.png)

为了简介用 Computational Graph 计算 BP，我们定义一个函数![z=f(x,y)](https://www.zhihu.com/equation?tex=z%3Df%28x%2Cy%29)，其中![x,y](https://www.zhihu.com/equation?tex=x%2Cy)都为标量时，根据 Chain Rule，其 FP 和 BP 分别如下

![img](https://pic1.zhimg.com/80/v2-ea256273b6efb8f2ea81f2e5f89d8f18_hd.png)

可以看出，若![\frac{dL}{dz}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bdz%7D)已知，则求![\frac{dL}{dx}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bdx%7D)和![\frac{dL}{dy}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bdy%7D)分别只需要知道![\frac{dz}{dx}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bdz%7D%7Bdx%7D)和![\frac{dz}{dy}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bdz%7D%7Bdy%7D)即可。其实就是 BP 的 Computational Graph 描述。

于是，我们可以利用 Computational Graph 和 BP 来计算增加了 BN Layer 后的![\frac{dL}{dx}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bdx%7D)、![\frac{dL}{d\gamma}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bd%5Cgamma%7D)、![\frac{dL}{d\beta}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bd%5Cbeta%7D)

通过下图，我们可以计算出![\frac{dL}{d\beta}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bd%5Cbeta%7D)

![img](https://pic2.zhimg.com/80/v2-3d55af81b2b7ce6bbc97124483beb361_hd.png)

通过下图，我们可以计算出![\frac{dL}{d\gamma}](https://www.zhihu.com/equation?tex=%5Cfrac%7BdL%7D%7Bd%5Cgamma%7D)

![img](https://pic1.zhimg.com/80/v2-89594d6db10cc26e277986752bf6d5b4_hd.png)

因为步骤好长，这里暂时就不贴出所有细节了，详细可以参考 [Understanding the backward pass through Batch Normalization Layer](http://link.zhihu.com/?target=https%3A//kratzert.github.io/2016/02/12/understanding-the-gradient-flow-through-the-batch-normalization-layer.html)。当然，以后有时间再补全亦可。

最终，我们可以得到 BN 的 BP 公式，如下。然后就可以利用 miniBatch GD 来更新参数了。

![img](https://pic1.zhimg.com/80/v2-85ef5640e120bb212538e1a171460ba0_hd.png)

**BN 测试**

这里需要注意的是，上面 Batch 的概念只可能出现在训练过程中。在测试时，DNN 的参数需要固定。因此，在训练的最后一个 epoch，需要记录下其每个 Batch 的![\mu_i](https://www.zhihu.com/equation?tex=%5Cmu_i)和![\sigma_i](https://www.zhihu.com/equation?tex=%5Csigma_i)，并使用其无偏估计作为测试时 DNN 的![\mu](https://www.zhihu.com/equation?tex=%5Cmu)和![\sigma](https://www.zhihu.com/equation?tex=%5Csigma)，即

![img](https://pic2.zhimg.com/80/v2-804c34eded156a913a5fe3a6814ed051_hd.png)

**BN 的作用**

BN 主要的好处其实是减少 DNN 在训练时调参的难度，这里列举几个比较主要的：

1. 在一般 DNN 中，如果 Learning Rate 设置的太小，则 DNN 会收敛的很慢；而 Learning Rate 设置的太大，则容易 Gradient Explode 或者 Gradient Vanish。按照“WHY Batch Normalization ?”中的分析，增加的 BN Layer 能缓解这种情况，从而能够使用较大的 Learning Rate，加速 GD
2. 作者通过实验说明，在有 BN Layer 的情况下，可以不使用 Dropout，而不带来性能的降低。不过感觉其说明不是很有说服力，所以实际中适当减少 Dropout 的使用即可，毕竟 Dropout 本身被证明是简单而有效缓解 Overfitting 的一种方法
3. DNN 在控制 Overfitting 时一般都是使用 L2 Weight Decay。同样是用实验，作者表示有了 BN，可以适当减少 L2 Weight Decay 的大小，即 BN 本身也有一定 Regularization 的作用。这一点我觉得可以这么理解，BN Layer 虽然没有直接约束![W](https://www.zhihu.com/equation?tex=W)，但是通过![BN(Wu+b)](https://www.zhihu.com/equation?tex=BN%28Wu%2Bb%29)其实约束了下一层的![u](https://www.zhihu.com/equation?tex=u)，从而间接约束了![W](https://www.zhihu.com/equation?tex=W)。当然，这同样只是感性上的理解而已。

## **尾巴**

经历三个篇章，终于把比较常见的 DNN 需要调节的超参数简单介绍了一部分。其他比如初始化、Early Stopping 等都相对简单，暂时不再继续介绍。**鉴于时间和认识有限，文章中可能存在错误，或者表达不清等问题，欢迎指出。**从下一篇开始，终于进入 CNN 和 LSTM 等等更加有意思了内容了。不过接下来我要出国一周，所以可能会晚更一周左右。

## **本系列其他文章：**

Supervised Learning：

当我们在谈论 Deep Learning：DNN 与 Backpropagation

当我们在谈论 Deep Learning：DNN 与它的参数们（壹）

当我们在谈论 Deep Learning：DNN 与它的参数们（贰）

当我们在谈论 Deep Learning：DNN 与它的参数们（叁）

当我们在谈论 Deep Learning：CNN 其常见架构（上）

Unsupervised Learning：