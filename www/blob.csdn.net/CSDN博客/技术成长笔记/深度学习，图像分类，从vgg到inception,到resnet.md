# 深度学习，图像分类，从vgg到inception,到resnet - 技术成长笔记 - CSDN博客





2017年07月30日 20:51:08[zhixuhao](https://me.csdn.net/u012931582)阅读数：27888








最近工作在做一件事情，就是把游戏图像进行场景分类，相比于自然图像来说，游戏图像种类较少，因此分类任务比较简单，但是由于追求工程上的高精度和高效率，所以阅读了vgg，inception，resnet等相关论文，并且都试了一下效果，算是对深度学习图像分类有了一个系统的了解，在这里总结一下。

# VGG

![vgg](https://img-blog.csdn.net/20170730201146203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

vgg的最主要的思想就是增加**网络深度**，**减小卷积核尺寸**（3*3）。减少卷积核的好处就是可以减少参数和计算量，比如，论文中举了一个例子，把一个7*7的卷积层替换成3个3*3的卷积层，参数上可以减少81%,因为3*3*3 = 27, 7*7 = 49. 

文中还提到了1*1的卷积层，比如说下图的configC的conv1层，这样做的目的主要是增加卷积层的非线性表达，同时影响卷积层的感受野，这个思想在google的inception系列网络中有了很好的应用，具体可以参考Network in Network这篇论文。

![这里写图片描述](https://img-blog.csdn.net/20170730201208460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

vgg的参数还是很多的，可以看出vgg-16有138million个参数。

# inception系列

参考论文： 

[v1] Going Deeper with Convolutions, 6.67% test error  
[http://arxiv.org/abs/1409.4842](http://arxiv.org/abs/1409.4842)

[v2] Batch Normalization: Accelerating Deep Network Training by Reducing Internal Covariate Shift, 4.8% test error  
[http://arxiv.org/abs/1502.03167](http://arxiv.org/abs/1502.03167)

[v3] Rethinking the Inception Architecture for Computer Vision, 3.5% test error  
[http://arxiv.org/abs/1512.00567](http://arxiv.org/abs/1512.00567)

[v4] Inception-v4, Inception-ResNet and the Impact of Residual Connections on Learning, 3.08% test error  
[http://arxiv.org/abs/1602.07261](http://arxiv.org/abs/1602.07261)
# inception-v1

这个就是GoogLeNet了，读谷歌的论文，一股工程的气息扑面而来，作者摘要中就说，本文的主要工作，就是研究怎么设计一种网络，更加有效地利用计算资源。

## inception

一般来说，提升网络性能最直接的办法就是增加网络深度和宽度，这也就意味着巨量的参数。但是，巨量参数容易产生过拟合也会大大增加计算量。

文章认为解决上述两个缺点的根本方法是将全连接甚至一般的卷积都转化为稀疏连接。一方面现实生物神经系统的连接也是稀疏的，另一方面有文献表明：对于大规模稀疏的神经网络，可以通过分析激活值的统计特性和对高度相关的输出进行聚类来逐层构建出一个最优网络。这点表明臃肿的稀疏网络可能被不失性能地简化。 虽然数学证明有着严格的条件限制，但Hebbian准则有力地支持了这一点：fire together,wire together。

早些的时候，为了打破网络对称性和提高学习能力，传统的网络都使用了随机稀疏连接。但是，计算机软硬件对非均匀稀疏数据的计算效率很差，所以在AlexNet中又重新启用了全连接层，目的是为了更好地优化并行运算。

所以，现在的问题是有没有一种方法，既能保持网络结构的稀疏性，又能利用密集矩阵的高计算性能。大量的文献表明可以将稀疏矩阵聚类为较为密集的子矩阵来提高计算性能，据此论文提出了名为Inception 的结构来实现此目的。

**inception module** 主要就是把稀疏结构近似成几个密集的子矩阵，从而在减少参数的同时，更加有效地利用计算资源。

![这里写图片描述](https://img-blog.csdn.net/20170730202347871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 采用不同大小的卷积核意味着不同大小的感受野，最后拼接意味着不同尺度特征的融合； 
- 之所以卷积核大小采用1、3和5，主要是为了方便对齐。设定卷积步长stride=1之后，只要分别设定pad=0、1、2，那么卷积之后便可以得到相同维度的特征，然后这些特征就可以直接拼接在一起了；
- 文章说很多地方都表明pooling挺有效，所以Inception里面也嵌入了。 
- 网络越到后面，特征越抽象，而且每个特征所涉及的感受野也更大了，因此随着层数的增加，3x3和5x5卷积的比例也要增加。

但是，这种naive的inception 并不实用，因为不同的feature map拼到一起之后传递到下一层inception 的5*5卷积层，计算量是非常巨大的，所以需要有一个降维的操作，这个降维就是依靠1*1的卷积层来实现，下面是改进之后的inception。

![这里写图片描述](https://img-blog.csdn.net/20170730202701125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Network in Network

GoogLeNet提出之时，说到其实idea是来自NIN，NIN就是Network in Network了。

NIN有两个特性，是它对CNN的贡献：
- MLP代替GLM
- Global Average Pooling

### mlpconv

![这里写图片描述](https://img-blog.csdn.net/20170730203449721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

普通的卷积可以看做输入feature map 和kernel 矩阵相乘的过程，是一个线性变换。MLP是指多层感知机，由于是多层的，f(x) = w*g(x)+b, g(x) = w*x +b，输出f和输入x自然是非线性关系，mlpconv的意思就是用mlp代替普通卷积，可以增加网络的抽象表达能力。

所以，基于如上，可以认为，在抽特征的时候直接做了非线性变换，可以有效的对图像特征进行更好的抽象。

从而，Linear convolution layer就变成了Mlpconv layer。

值得一提的是，**Mlpconv相当于在激活函数后面，再添加一个1×1的卷积层。**

当然，1*1卷积层在本文中的最重要的作用还是进行降维。

### Global Average Pooling

Global Average Pooling的做法是将全连接层去掉。

全连接层的存在有两个缺点：

全连接层是传统的神经网络形式，使用了全连接层以为着卷积层只是作为特征提取器来提取图像的特征，而全连接层是不可解释的，从而CNN也不可解释了 

全连接层中的参数往往占据CNN整个网络参数的一大部分，从而使用全连接层容易导致过拟合。 

而Global Average Pooling则是在最后一层，将卷积层设为与类别数目一致，然后全局pooling，从而输出类别个数个结果。
但是本文中，在average pooling 后面还跟了一个linear layer,加这个层主要是为了方便起见，能够更容易扩展到其他的数据集上。

## 结构

最后的结构如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20170730202737444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

网络在训练的时候还加了两个额外的分类器，目的是为了防止梯度消失，最后测试的时候可以把这两个分类器去掉。 

最后的参数相比于alexnet少了12倍，相比于vgg-16来说少了3倍。

# inception-v2

最主要的贡献就是提出了batch normalization，目的主要在于**加快训练速度**。

关于BN层，这一篇博客讲得很好，[http://blog.csdn.net/app_12062011/article/details/57083447](http://blog.csdn.net/app_12062011/article/details/57083447)

我在这里转一下。 

作者认为：网络训练过程中参数不断改变导致后续每一层输入的分布也发生变化，而学习的过程又要使每一层适应输入的分布，因此我们不得不降低学习率、小心地初始化。作者将分布发生变化称之为internal covariate shift。 

大家应该都知道，我们一般在训练网络的时会将输入减去均值，还有些人甚至会对输入做白化等操作，目的是为了加快训练。为什么减均值、白化可以加快训练呢，这里做一个简单地说明： 

首先，图像数据是高度相关的，假设其分布如下图a所示(简化为2维)。由于初始化的时候，我们的参数一般都是0均值的，因此开始的拟合y=Wx+b，基本过原点附近，如图b红色虚线。因此，网络需要经过多次学习才能逐步达到如紫色实线的拟合，即收敛的比较慢。如果我们对输入数据先作减均值操作，如图c，显然可以加快学习。更进一步的，我们对数据再进行去相关操作，使得数据更加容易区分，这样又会加快训练，如图d。
![这里写图片描述](https://img-blog.csdn.net/20170730205714820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

白化的方式有好几种，常用的有PCA白化：即对数据进行PCA操作之后，在进行方差归一化。这样数据基本满足0均值、单位方差、弱相关性。作者首先考虑，对每一层数据都使用白化操作，但分析认为这是不可取的。因为白化需要计算协方差矩阵、求逆等操作，计算量很大，此外，反向传播时，白化操作不一定可导。于是，作者采用下面的Normalization方法。

数据归一化方法很简单，就是要让数据具有**0均值和单位方差**
![这里写图片描述](https://img-blog.csdn.net/20170730205806435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是作者又说如果简单的这么干，会降低层的表达能力。比如下图，在使用sigmoid激活函数的时候，如果把数据限制到0均值单位方差，那么相当于只使用了激活函数中近似线性的部分，这显然会降低模型表达能力。

![这里写图片描述](https://img-blog.csdn.net/20170730205837384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为此，作者又为BN增加了2个参数，用来保持模型的表达能力：

![这里写图片描述](https://img-blog.csdn.net/20170730205910998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

整个BN的算法如下：

![这里写图片描述](https://img-blog.csdn.net/20170730210035393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在**测试**的时候：

我们依然会应用下面的式子：

![这里写图片描述](https://img-blog.csdn.net/20170730205806435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**特别注意：** 这里的均值和方差已经不是针对某一个Batch了，而是针对整个数据集而言。因此，在训练过程中除了正常的前向传播和反向求导之外，我们还要记录每一个Batch的均值和方差，以便训练完成之后按照下式计算整体的均值和方差：

![这里写图片描述](https://img-blog.csdn.net/20170730210236356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在测试的时候，计算公式为：

![这里写图片描述](https://img-blog.csdn.net/20170730210520830?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

作者在文章中说应该把BN放在激活函数之前，这是因为Wx+b具有更加一致和非稀疏的分布。

# inception-v3

本文的主要目的是研究如何在增加网络规模的同时，能够保证计算的高效率。

Rethinking这篇论文中提出了一些CNN调参的经验型规则，暂列如下：
- 避免特征表征的瓶颈。特征表征就是指图像在CNN某层的激活值，特征表征的大小在CNN中应该是缓慢的减小的。
- 高维的特征更容易处理，在高维特征上训练更快，更容易收敛
- 低维嵌入空间上进行空间汇聚，损失并不是很大。这个的解释是相邻的神经单元之间具有很强的相关性，信息具有冗余。
- 平衡的网络的深度和宽度。宽度和深度适宜的话可以让网络应用到分布式上时具有比较平衡的computational budget。

## 更小的卷积核

![这里写图片描述](https://img-blog.csdn.net/20170730211926756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

简而言之，就是将尺寸比较大的卷积，变成一系列3×3的卷积的叠加，这样既具有相同的视野，还具有更少的参数。 

实验表明，这样做不会导致性能的损失。

## Grid Size Reduction

Grid就是图像在某一层的激活值，即feature_map，一般情况下，如果想让图像缩小，可以有如下两种方式：

![这里写图片描述](https://img-blog.csdn.net/20170730212124925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

右图是正常的缩小，但计算量很大。左图先pooling会导致特征表征遇到瓶颈，违反上面所说的第一个规则，为了同时达到不违反规则且降低计算量的作用，将网络改为下图：

![这里写图片描述](https://img-blog.csdn.net/20170730212524002?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

结构如左图表示，左边两个代表的是右图中的左边分支。

## label smoothing

除了上述的模型结构的改进以外，Rethinking那篇论文还改进了目标函数。

原来的目标函数，在单类情况下，如果某一类概率接近1，其他的概率接近0，那么会导致交叉熵取log后变得很大很大。从而导致两个问题：

过拟合 

导致样本属于某个类别的概率非常的大，模型太过于自信自己的判断。 

所以，使用了一种平滑方法，可以使得类别概率之间的差别没有那么大.
# resnet

转自[http://blog.csdn.net/wspba/article/details/56019373](http://blog.csdn.net/wspba/article/details/56019373)

resnet要解决的主要问题，就是深度网络中的**退化**问题。

![这里写图片描述](https://img-blog.csdn.net/20170730213411066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

深度学习网络的深度对最后的分类和识别的效果有着很大的影响，所以正常想法就是能把网络设计的越深越好，但是事实上却不是这样，常规的网络的堆叠（plain network）在网络很深的时候，效果却越来越差了。 

这里其中的原因之一即是网络越深，梯度消失的现象就越来越明显，网络的训练效果也不会很好。  

但是现在浅层的网络（shallower network）又无法明显提升网络的识别效果了，所以现在要解决的问题就是怎样在加深网络的情况下又解决梯度消失的问题。

## 残差网络

作者提出了一个Residual的结构：

![这里写图片描述](https://img-blog.csdn.net/20170730214049140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

即增加一个identity mapping（恒等映射），将原始所需要学的函数H(x)转换成F(x)+x，而作者认为这两种表达的效果相同，但是优化的难度却并不相同，作者假设F(x)的优化 会比H(x)简单的多。这一想法也是源于图像处理中的残差向量编码，通过一个reformulation，将一个问题分解成多个尺度直接的残差问题，能够很好的起到优化训练的效果。 

这个Residual block通过shortcut connection实现，通过shortcut将这个block的输入和输出进行一个element-wise的加叠，这个简单的加法并不会给网络增加额外的参数和计算量，同时却可以大大增加模型的训练速度、提高训练效果，并且当模型的层数加深时，这个简单的结构能够很好的解决退化问题。

接下来，作者就设计实验来证明自己的观点。 

首先构建了一个18层和一个34层的plain网络，即将所有层进行简单的铺叠，然后构建了一个18层和一个34层的residual网络，仅仅是在plain上插入了shortcut，而且这两个网络的参数量、计算量相同，并且和之前有很好效果的VGG-19相比，计算量要小很多。（36亿FLOPs VS 196亿FLOPs，FLOPs即每秒浮点运算次数。）这也是作者反复强调的地方，也是这个模型最大的优势所在。

![这里写图片描述](https://img-blog.csdn.net/20170730214218056?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

模型构建好后进行实验，在plain上观测到明显的退化现象，而且ResNet上不仅没有退化，34层网络的效果反而比18层的更好，而且不仅如此，ResNet的收敛速度比plain的要快得多。

对于shortcut的方式，作者提出了三个选项： 

A. 使用恒等映射，如果residual block的输入输出维度不一致，对增加的维度用0来填充； 

B. 在block输入输出维度一致时使用恒等映射，不一致时使用线性投影以保证维度一致； 

C. 对于所有的block均使用线性投影。 

对这三个选项都进行了实验，发现虽然C的效果好于B的效果好于A的效果，但是差距很小，因此线性投影并不是必需的，而使用0填充时，可以保证模型的复杂度最低，这对于更深的网络是更加有利的。另外，shortcut过程中，stride是2可以保证feature map的大小一致。 

进一步实验，作者又提出了deeper的residual block：
![这里写图片描述](https://img-blog.csdn.net/20170730214315586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这相当于对于相同数量的层又减少了参数量，因此可以拓展成更深的模型。于是作者提出了50、101、152层的ResNet，而且不仅没有出现退化问题，错误率也大大降低，同时计算复杂度也保持在很低的程度。 

这个时候ResNet的错误率已经把其他网络落下几条街了，但是似乎还并不满足，于是又搭建了更加变态的1202层的网络，对于这么深的网络，优化依然并不困难，但是出现了过拟合的问题，这是很正常的，作者也说了以后会对这个1202层的模型进行进一步的改进。

# Inception-ResNet-v2

这篇论文里，作者借鉴了resnet的残差网络的思想，将其应用到了inception-v3当中，结构如下：

![这里写图片描述](https://img-blog.csdn.net/20170730214635297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# inception-v4

inception-v4 是通过提出了更多新的inception module扩展网络结构，能够达到和inception-resnet-v2相媲美的效果，这个有时间再研究一下。









