# 系统学习深度学习（十一）--dropout,dropconect - 工作笔记 - CSDN博客





2017年02月25日 11:40:31[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7665








原文：http://blog.csdn.net/u012702874/article/details/45030991





dropout layer的目的是为了防止CNN 过拟合。那么为什么可以有效的防止过拟合呢？

首先，想象我们现在只训练一个特定的网络，当迭代次数增多的时候，可能出现网络对训练集拟合的很好（在训练集上loss很小），但是对验证集的拟合程度很差的情况。所以，我们有了这样的想法：可不可以让每次跌代随机的去更新网络参数（weights），引入这样的随机性就可以增加网络generalize 的能力。所以就有了dropout 。

在训练的时候，我们只需要按一定的概率（retaining probability）p 来对weight layer 的参数进行随机采样，将这个子网络作为此次更新的目标网络。可以想象，如果整个网络有n个参数，那么我们可用的子网络个数为 2^n 。 并且，当n很大时，每次迭代更新 使用的子网络基本上不会重复，从而避免了某一个网络被过分的拟合到训练集上。

那么[测试](http://lib.csdn.net/base/softwaretest)的时候怎么办呢？ 一种最naive的方法是，我们把 2^n 个子网络都用来做测试，然后以某种 voting 机制将所有结果结合一下（比如说平均一下下），然后得到最终的结果。但是，由于n实在是太大了，这种方法实际中完全不可行！


所以有人提出，那我做一个大致的估计不就得了，我从2^n个网络中随机选取 m 个网络做测试，最后在用某种voting 机制得到最终的预测结果。这种想法当然可行，当m很大时但又远小于2^n时，能够很好的逼近原2^n个网络结合起来的预测结果。但是，有没有更好的办法呢？ of course！那就是dropout 自带的功能，能够通过一次测试得到逼近于原2^n个网络组合起来的预测能力！


虽然训练的时候我们使用了dropout， 但是在测试时，我们不使用dropout （不对网络的参数做任何丢弃，这时dropout layer相当于进来什么就输出什么）。然后，把测试时dropout layer的输出乘以训练时使用的retaining probability  p （这时dropout layer相当于把进来的东东乘以p）。仔细想想这里面的意义在哪里呢？？？ 事实上，由于我们在测试时不做任何的参数丢弃，如上面所说，dropout layer 把进来的东西原样输出，导致在统计意义下，测试时
 每层 dropout layer的输出比训练时的输出多加了【（1 - p）*100】%  units 的输出。 即 【p*100】% 个units 的和  是同训练时随机采样得到的子网络的输出一致，另【（1 - p）*100】%  的units的和  是本来应该扔掉但是又在测试阶段被保留下来的。所以，为了使得dropout layer 下一层的输入和训练时具有相同的“意义”和“数量级”，我们要对测试时的伪dropout layer的输出（即下层的输入）做 rescale： 乘以一个p，表示最后的sum中只有这么大的概率，或者这么多的部分被保留。这样以来，只要一次测试，将原2^n个子网络的参数全部考虑进来了，并且最后的
 rescale 保证了后面一层的输入仍然符合相应的物理意义和数量级。

假设x是dropout layer的输入，y是dropout layer的输出，W是上一层的所有weight parameters，![](http://latex.codecogs.com/gif.latex?W%7C_%7Bp%7D) 是以retaining probability 为p 采样得到的weight parameter子集。把上面的东西用公式表示（忽略bias）：

train：![](http://latex.codecogs.com/gif.latex?y%3DW%7C_%7Bp%7D*x)

test:![](http://latex.codecogs.com/gif.latex?y%3DW*px)

但是一般写程序的时候，我们想直接在test时用  ![](http://latex.codecogs.com/gif.latex?y%3DW%27*x) ， 这种表达式。（where  ![](http://latex.codecogs.com/gif.latex?W%27%3DW%5Ccdot%20p)） 

因此我们就在训练的时候就直接训练 ![](http://latex.codecogs.com/gif.latex?W%27) 。 所以训练时，第一个公式修正为  ![](http://latex.codecogs.com/gif.latex?y%3D%5Cfrac%7BW%27%7C_%7Bp%7D%7D%7Bp%7D*x%3DW%27%7C_%7Bp%7D*%5Cfrac%7Bx%7D%7Bp%7D)  。 即把dropout的输入除以p 再进行训练，这样得到的训练得到的weight
 参数就是 ![](http://latex.codecogs.com/gif.latex?W%27) ，测试的时候除了不使用dropout外，不需要再做任何rescale。Caffe 和Lasagne 里面的代码就是这样写的。

原文作者说，是dropout的输入乘以p,但我看公式，应该是除以p


参考文献： Improving Neural Networks with Dropout， Hinton学生的一片thesis

       Dropout: A Simple Way to Prevent Neural Networks from Overfitting

转自：http://blog.csdn.net/shuzfan/article/details/50580915


文主要介绍Dropout及延伸下来的一些方法，以便更深入的理解。

**想要提高CNN的表达或分类能力，最直接的方法就是采用更深的网络和更多的神经元，即deeper and wider。但是，复杂的网络也意味着更加容易过拟合。于是就有了Dropout，大部分实验表明其具有一定的防止过拟合的能力。**

### 1- Dropout

最早的Dropout可以看Hinton的这篇文章 
[《Improving neural networks by preventing co-adaptation of feature Detectors》](http://arxiv.org/pdf/1207.0580.pdf)

从文章的名字我们就可以先对Dropout的工作原理有个猜测：过拟合意味着网络记住了训练样本，而打破网络固定的工作方式，就有可能打破这种不好的记忆。

Ok，我们直接来看什么是**Dropout**：

![这里写图片描述](https://img-blog.csdn.net/20160124223731521)

**需要注意的是**：论文中Dropout被使用在全连接层之后，而目前的caffe框架中，其可以使用在各种层之后。

**如上图左**，为没有Dropout的普通2层全连接结构，记为 r=a(Wv)，其中a为激活函数。

**如上图右**，为在第2层全连接后添加Dropout层的示意图。即在 **模 型 训 练 时** 随机让网络的某些节点不工作（输出置0），其它过程不变。

对于Dropout这样的操作为何可以防止训练过拟合，原作者也没有给出数学证明，只是有一些直观的理解或者说猜想。下面说几个我认为比较靠谱的解释：

**(1)** 由于随机的让一些节点不工作了，因此可以避免某些特征只在固定组合下才生效，有意识地让网络去学习一些普遍的共性（而不是某些训练样本的一些特性）

**(2)** Bagging方法通过对训练数据有放回的采样来训练多个模型。而Dropout的随机意味着每次训练时只训练了一部分，而且其中大部分参数还是共享的，因此和Bagging有点相似。因此，Dropout可以看做训练了多个模型，实际使用时采用了模型平均作为输出

（具体可以看一下论文，论文讲的不是很明了，我理解的也够呛）

**训练的时候**，我们通常设定一个dropout ratio = p,即每一个输出节点以概率 **p** 置0(不工作)。假设每一个输出都是相互独立的，每个输出都服从二项伯努利分布B(1-p)，则大约认为训练时**只使用了** (1-p)比例的输出。

**[测试](http://lib.csdn.net/base/softwaretest)的时候**，最直接的方法就是保留Dropout层的同时，将一张图片重复测试M次，取M次结果的平均作为最终结果。假如有N个节点，则可能的情况为R=2^N,如果M远小于R，则显然平均效果不好；如果M≈N，那么计算量就太大了。因此作者做了一个近似：可以直接去掉Dropout层，将所有输出**都使用**
 起来，为此需要将尺度对齐，即比例缩小输出 r=r*(1-p)。 

即如下公式： 
![这里写图片描述](https://img-blog.csdn.net/20160222164038496)
**特别的**， 为了使用方便，我们不在测试时再缩小输出，而在训练时直接将输出放大1/(1-p)倍。

**结论：** Dropout得到了广泛的使用，但具体用到哪里、训练一开始就用还是后面才用、dropout_ratio取多大，还要自己多多尝试。有时添加Dropout反而会降低性能。

### 2- DropConnect

DropConnect来源于[《Regularization of Neural Networks using DropConnect》](http://yann.lecun.com/exdb/publis/pdf/wan-icml-13.pdf)这篇文章。

[更详细的实验对比以及代码，可以点击http://cs.nyu.edu/~wanli/dropc/](http://cs.nyu.edu/~wanli/dropc/)

该方法改进于第一节介绍的Dropout,具体可看下图作对比 
![这里写图片描述](https://img-blog.csdn.net/20160222164525715)

由图可知，二者的区别很明显：Dropout是将输出随机置0，而DropConnect是将权重随机置0。 

文章说之所以这么干是因为原来的Dropout进行的不够充分，随机采样不够合理。这可以从下图进行理解： 
![这里写图片描述](https://img-blog.csdn.net/20160222165525283)

如上图所示，a表示不加任何Drop时的一层网络模型。添加Drop相当于给权重再乘以一个随机掩膜矩阵M。

![](https://img-blog.csdn.net/20160222170346989)

不同的是，DropConnect由于直接对权重随机置0，因此其掩膜显得更加具有随机性，如b所示。而Dropout仅对输出进行随机置0,因此其掩膜相当于是对随机的行和列进行置0，如c所示。

**训练的时候**，训练过程与Dropout基本相同。

**测试的时候**，我们同样需要一种近似的方法。如下图公式所示： 
![这里写图片描述](https://img-blog.csdn.net/20160222193632780)
**注意：** 掩膜矩阵M的每一个元素都满足二项伯努利分布。假如M的维度为m*n，则可能的掩膜有2^(m*n)种，之前提到过我们可以粗暴的遍历所有的掩膜然后计算结果最后求平均。**中心极限定理：和分布渐进于正态分布。** 于是，我们可以不去遍历，而是通过计算每一维的均值与方差，确定每一维的正态分布，最后**在此正态分布上做多次采样后求平均即可获得最终的近似结果**。由此看出计算量相比dropout增加了不少。

具体测试时的[算法](http://lib.csdn.net/base/datastructure)流程如下：
![这里写图片描述](https://img-blog.csdn.net/20160222193646890)

其中，Z是在正态分布上的采样次数，一般来说越大越好，但会使得计算变慢。

**实验：** 作者当然要做很多对比试验，但其实发现效果并不比Dropout优秀太多，反而计算量要大很多，因此到目前DropConnect并没有得到广泛的应用。具体的对比，可以参看原文，这里我只贴一张图来说明对于**Drop ratio**的看法：
![这里写图片描述](https://img-blog.csdn.net/20160222193657140)
由此可以看出，drop ratio并不是越大越好，具体需要大家多做实验多体会。

补充：





http://blog.csdn.net/wantong1017/article/details/14602129

实际上，我自己的实验结论和上面这位博主的结果一样dropout在卷基层，性能反而下降，全层drop也会下降，只在全连接层性能会提升.dropconect没有试过，好像我用的Torch不支持。




