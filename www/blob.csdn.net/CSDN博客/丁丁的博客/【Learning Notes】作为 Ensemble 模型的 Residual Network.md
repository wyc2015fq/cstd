
# 【Learning Notes】作为 Ensemble 模型的 Residual Network - 丁丁的博客 - CSDN博客


2016年11月20日 17:40:07[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：3408


**Andreas Veit et al.**[Residual Networks Behave Like Ensembles of Relatively Shallow Networks](https://arxiv.org/abs/1605.06431). NIPS’16.
Residual Network (ResNet) 在图像、语音任务上都取得了巨大的成功，是继 ReLU, Dropout, Batch Normalization 后，深度学习上又一个 milestone。一般认为，ResNet 通过引入 skip connect，解决了网络层数变深后的梯度反传的问题，从而使得训练非常深的网络成为可能。而深层网络比浅层网络有更好的性能则是一个 well-known 的结论。
Veit 等的文章却提出了对 ResNet 的一种非常不同的解释。作者认为：
1. ResNet的优良性能，并不来自于层数深，而是由于ResNet是指数多（相对于层数）的浅层（相对物理层数）网络的ensemble模型。
2. ResNet 也并没有解决深层网络情况下的梯度反传问题，而是回避的这一问题——ResNet 只是训练了一系列浅层模型，而 ensemble 中的深层模型并没并良好的训练，只不过是这部分模型数量少，并没有影响 ResNet 的整体性能。
为支持以上观点，文章中进行了一系列 lesion study（破坏网络结构）。
## 1. 相关工作
### Residual networks
简单而言，ResNet引入skip connect (Fig1(a))。因此，常规网络中输出
$$
f(y_{i-1})
$$
，在这一框架下变成了上一层的残差(residual)。

$$
y_i = y_{i-1} + y_{i-1}
$$
ResNet 最初用这图像任务上。由于 deep CNN 是语音识别领域一个热点，引入 ResNet，CNN 也可以更 deeper 了，性能提升也很显著（如[MSR 近期的工作](https://arxiv.org/pdf/1610.05256v1.pdf))。
![](https://img-blog.csdn.net/20161120174230002)
### Highway
Highway 是 LSTM 中 gate 思路在空间上的推广。ResNet 可以看作是 Highway 的一个特例，即 gate系数被手动设为了常数。 和 ResNet 一样，highway 也被用于设计窄而深（thinner and deeper）的网络，比如 InterSpeech’16 上有利用 highway 做 small footprint 语音识别的[工作](https://arxiv.org/pdf/1512.04280v3.pdf)。

$$
y_i = f_i(y_{i-1}) \cdot t_i(y_{i-1}) + y_{i-1} \cdot (1-t_i(y_{i-1}))
$$

### Investigating neural networks
对CNN的研究主要有几种方法：
1. 观察学习到的 filter
2. 通过对抗样本
3. 通过破坏（lesion）网络结构，观察性能变化
文章采用的是第三种方法。
### Ensemble
Ensemble 是机器学习的经典方法，比如 Adaboost, bagging, random forests、GBDT 等。一般初始条件或结构不同的若干基础模型，通过简单投票等都能获得一定的性能提升。Kaggle 上 winner 多为 ungodly ensemble 模型(reddit 网友语:-)。语音识别领域，一般会 enemble
多个模型的打分，因此更多的称为融合（fusion）或插值。
### Dropout
Hinton 老爷子的文章把 dropout 视作 implicitly ensemble 了指数多个模型（虽然很多人只把 dropout 视作是一种简单高效的正则化方法）。Dropout 的“ensemble”是通过特殊的训练策略（random drop）实现的，而 ResNet 则是通过网络结构直接构造了一种指数多的 ensemble。
### The unraveled view of residual networks
ResNet可以按信息流展开来看（Fig.1(b)）。这样，有 n 个 identity connect 的 ResNet 就包括了
$$
2^n
$$
条不同的路径，也即基础模型。
## 2. Lesion study
为证明 ResNet 是 expotiental ensemble 的论述，作者进行了一系统实验。基本方法是，在训练和的 ResNet 上进行某种破坏，观察受损网络的性能变化。具体网络结构可参见文献。
### 去掉某一层
如 Fig.3 所示，对普通的 VGG 网络，去掉任意一层，预测结果变成了随机的（~10%），完全不可用。这个结果非常符合预期。但是，对于 ResNet, 除了去年前面的 downsampling 会对性能造成较大的影响外，其他层的缺失对网络整体性能几乎没有影响。
这一现象符合 ensemble 的假设——一个由多个基础模型构成的 ensemble 模型，不会因为某个模型的缺失而受到重大性能损害。
![](https://img-blog.csdn.net/20161120174308847)
### 去掉某几层
对于ensemble模型，其性能会随着去掉基础模型的个数而平衡的下降。文章统计也去掉不层数的ResNet的性能变化。如 Fig.5(a)，结果符合 ensemble 模型的预期。
![](https://img-blog.csdn.net/20161120174346129)
### 随机重组某些层
这相当于随机删除了某些基础模型，同时随机加入了某些基础模型（没有经过训练）。Emsemble 模型应该随着重组程度的增加，性能平衡的下降。Fig.5(b) 展示的结果符合这一预期。
### 短路径的重要性
按照 ResNet 展开的思路，根据多项式分布，容易计算出 ResNet 中的路径大多数为短路径（Fig.6(a)）。对于 54 个 residual module 的 ResNet (110层)，95% 的路径长度在 19~35 层。文章统计了不同长度路径的梯度幅值（Fig.6(b)）及梯度更新的贡献与路径长度的关系（Fig.6(c)），论证了在 ResNet 中，长路径（相当于深层网络）的梯度反传同样非常困难，对模型更新的贡献不多。
![](https://img-blog.csdn.net/20161120174411053)
## 一些思考：
ensemble 的观点非常有意思，它能解释有关 ResNet 的一些重要的现象:
The deeper, the better——层数越深，聚合的模型越多
梯度反传问题——resNet 没有直接解决深层网络回传的问题，而是训练一系列较浅层的网络
Stochastic depth training——Ensemble 中，没有必要每个 minibatch 都训练所有基础模型。
已经有研究者 follow 文章的思路，提出了对[resNet 的改进](https://arxiv.org/abs/1609.05672)。论文中通过增加 multiplicity（增加指数的底数），用更浅的网络，达到与更深层 resnet 相同或更好的分类性能。如果这一结果可以在大数据集上得到验证（文章是在 cifar10 和 cifar100 上做的），能够极大提高网络的并行度，对加速 training和 predicting 都会有好处。
> 注：
> [Aggregated Residual Transformations for Deep Neural Networks](https://arxiv.org/abs/1611.05431)
> （coauthored by Kaiming He） 在 ImageNet 验证了上述方法的有效必（使用了cardinality而的概念非multiplicity）。
其他理论工作也在涌现。比如[这个工作](https://arxiv.org/abs/1611.04231)的一个 takeaway 是，使用远小于常规的初始化方法（Glorot, LeCun etc.），有利于 ResNet 的训练。
比较好奇，ResNet 的各层按照 Bengio 的[classifier prober](https://arxiv.org/abs/1610.01644)测一下会是怎样一条网线。
LSTM, GRU 等 gate based RNN 变种，改善了 RNN 的 gradient vanishing 问题。Highway 是 LSTM 在空间上的自然推广。各种 Attention model 出现后，感觉在数学上，这些模型非常类似，都是引入了乘法。这篇文章提出的关注 dataflow path 的思路，未尝不是理解这些网络结构的一种新的角度。

---

## Further Reading:
Huang et al.[Learning Deep ResNet Blocks Sequetially using Boosting Theory](https://arxiv.org/abs/1706.04964).
Abdi et al.[Multi-Residual Networks: Improving the Speed and Accuracy of Residual Networks](https://arxiv.org/abs/1609.05672).
Hardt et al.[Identity Matters in Deep Learning](https://arxiv.org/abs/1611.04231).
Xie et al.[Aggregated Residual Transformations for Deep Neural Networks](https://arxiv.org/abs/1611.05431).
Wu et al.[Wider or Deeper: Revisiting the ResNet Model for Visual Recognition](https://arxiv.org/abs/1611.10080).
Liao et al .[Bridging the Gaps Between Residual Learning, Recurrent Neural Networks and Visual Cortex](https://arxiv.org/abs/1604.03640).
Greff et al.[Highway and Residual Networks learn Unrolled Iterative Estimation](https://arxiv.org/abs/1612.07771).

