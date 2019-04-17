# 系统学习深度学习（十九）--GoogLeNetV1,V2,V3 - 工作笔记 - CSDN博客





2017年03月15日 10:46:59[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：20337








转自：http://blog.csdn.net/shuzfan/article/details/50738394

参考：http://www.jianshu.com/p/33197e469414

同时做了一些补充。

本文介绍的是著名的网络结构GoogLeNet及其延伸版本，目的是试图领会其中的**思想而不是单纯关注结构**。

# **GoogLeNet Incepetion V1**

这是GoogLeNet的最早版本，出现在2014年的[《Going deeper with convolutions》](http://arxiv.org/abs/1409.4842)。之所以名为“GoogLeNet”而非“GoogleNet”,文章说是为了向早期的LeNet致敬。

## **Motivation**

[深度学习](http://lib.csdn.net/base/deeplearning)以及神经网络快速发展，人们不再只关注更给力的硬件、更大的数据集、更大的模型，而是更在意新的idea、新的[算法](http://lib.csdn.net/base/datastructure)以及模型的改进。

一般来说，提升网络性能最直接的办法就是增加网络深度和宽度，这也就意味着巨量的参数。但是，巨量参数容易产生**过拟合**也会大大增加**计算量**。

文章认为解决上述两个缺点的根本方法是将全连接甚至一般的卷积都转化为稀疏连接。一方面现实生物神经系统的连接也是稀疏的，另一方面有文献[1](http://blog.csdn.net/shuzfan/article/details/50738394#fn:1x)表明：对于大规模稀疏的神经网络，可以通过分析激活值的统计特性和对高度相关的输出进行聚类来逐层构建出一个最优网络。**这点表明臃肿的稀疏网络可能被不失性能地简化。** 虽然数学证明有着严格的条件限制，但Hebbian准则有力地支持了这一点：fire together,wire together。

早些的时候，为了打破网络对称性和提高学习能力，传统的网络都使用了随机稀疏连接。但是，计算机软硬件对非均匀稀疏数据的计算效率很差，所以在AlexNet中又重新启用了全连接层，目的是为了更好地优化并行运算。

所以，现在的问题是有没有一种方法，**既能保持网络结构的稀疏性，又能利用密集矩阵的高计算性能**。大量的文献表明可以将稀疏矩阵聚类为较为密集的子矩阵来提高计算性能，据此论文提出了名为Inception 的结构来实现此目的。

## **Architectural Details**

**Inception** 结构的主要思路是怎样用密集成分来近似最优的局部稀疏结构。

作者首先提出下图这样的基本结构：
![这里写图片描述](https://img-blog.csdn.net/20160225155336279)

对上图做以下说明：
**1 .** 采用不同大小的卷积核意味着不同大小的感受野，最后拼接意味着不同尺度特征的融合；
**2 .** 之所以卷积核大小采用1、3和5，主要是为了方便对齐。设定卷积步长stride=1之后，只要分别设定pad=0、1、2，那么卷积之后便可以得到相同维度的特征，然后这些特征就可以直接拼接在一起了；
**3 .** 文章说很多地方都表明pooling挺有效，所以Inception里面也嵌入了。
**4 .** 网络越到后面，特征越抽象，而且每个特征所涉及的感受野也更大了，因此随着层数的增加，3x3和5x5卷积的比例也要增加。

**但是，使用5x5的卷积核仍然会带来巨大的计算量。** 为此，文章借鉴NIN[2](http://blog.csdn.net/shuzfan/article/details/50738394#fn:2x)，采用1x1卷积核来进行**降维**。

例如：上一层的输出为100x100x128，经过具有256个输出的5x5卷积层之后(stride=1，pad=2)，输出数据为100x100x256。其中，卷积层的参数为128x5x5x256。假如上一层输出先经过具有32个输出的1x1卷积层，再经过具有256个输出的5x5卷积层，那么最终的输出数据仍为为100x100x256，但卷积参数量已经减少为128x1x1x32 + 32x5x5x256，大约减少了4倍。

具体改进后的Inception Module如下图：
![这里写图片描述](https://img-blog.csdn.net/20160225155351172)

## **GoogLeNet**

GoogLeNet的整体结构如下图：

![这里写图片描述](https://img-blog.csdn.net/20160225155403967)

对上图做如下说明：
**1 .** 显然GoogLeNet采用了模块化的结构，方便增添和修改；
**2 .** 网络最后采用了average pooling来代替全连接层，想法来自NIN,事实证明可以将TOP1 accuracy提高0.6%。但是，实际在最后还是加了一个全连接层，主要是为了方便以后大家finetune；
**3 .** 虽然移除了全连接，但是网络中依然使用了Dropout ;
**4 .** 为了避免梯度消失，网络额外增加了2个辅助的softmax用于向前传导梯度。文章中说这两个辅助的分类器的loss应该加一个衰减系数，但看caffe中的model也没有加任何衰减。此外，实际[测试](http://lib.csdn.net/base/softwaretest)的时候，这两个额外的softmax会被去掉。

下图是一个比较清晰的结构图：

![这里写图片描述](https://img-blog.csdn.net/20160225155414702)

## **Conclusion**

GoogLeNet是谷歌团队为了参加ILSVRC 2014比赛而精心准备的，为了达到最佳的性能，除了使用上述的网络结构外，还做了大量的辅助工作：包括训练多个model求平均、裁剪不同尺度的图像做多次验证等等。详细的这些可以参看文章的实验部分。

本文的主要想法其实是想通过构建密集的块结构来近似最优的稀疏结构，从而达到提高性能而又不大量增加计算量的目的。GoogleNet的caffemodel大小约50M，但性能却很优异。



# **GoogLeNet Inception V2**

V2提出了BN，http://blog.csdn.net/app_12062011/article/details/57083447有介绍，另外， BN的反向传导：http://www.jianshu.com/p/4270f5acc066.softmax 梯度计算:http://blog.csdn.net/u014313009/article/details/51045303



# **GoogLeNet Inception V3**

GoogLeNet凭借其优秀的表现，得到了很多研究人员的学习和使用，因此Google团队又对其进行了进一步发掘改进，产生了升级版本的GoogLeNet。这一节介绍的版本记为V3，文章为：[《Rethinking the Inception Architecture for Computer Vision》](http://arxiv.org/abs/1512.00567)。

## **Introduction**

14年以来，构建更深的网络逐渐成为主流，但是模型的变大也使计算效率越来越低。这里，文章试图找到一种方法在**扩大网络的同时又尽可能地发挥计算性能**。

首先，GoogLeNet V1出现的同期，性能与之接近的大概只有VGGNet了，并且二者在图像分类之外的很多领域都得到了成功的应用。但是相比之下，GoogLeNet的计算效率明显高于VGGNet，大约只有500万参数，只相当于Alexnet的1/12(GoogLeNet的caffemodel大约50M，VGGNet的caffemodel则要超过600M)。

GoogLeNet的表现很好，但是，如果想要通过简单地放大Inception结构来构建更大的网络，则会立即提高计算消耗。此外，在V1版本中，文章也没给出有关构建Inception结构注意事项的清晰描述。因此，在文章中作者**首先给出了一些已经被证明有效的用于放大网络的通用准则和优化方法**。这些准则和方法适用但不局限于Inception结构。

## **General Design Principles**

下面的准则来源于大量的实验，因此包含一定的推测，但实际证明基本都是有效的。

**1 . 避免表达瓶颈，特别是在网络靠前的地方。** 信息流前向传播过程中显然不能经过高度压缩的层，即表达瓶颈。从input到output，feature map的宽和高基本都会逐渐变小，但是不能一下子就变得很小。比如你上来就来个kernel = 7, stride = 5 ,这样显然不合适。

另外输出的维度channel，一般来说会逐渐增多(每层的num_output)，否则网络会很难训练。（特征维度并不代表信息的多少，只是作为一种估计的手段）

这种情况一般发生在pooling层，字面意思是，pooling后特征图变小了，但有用信息不能丢，不能因为网络的漏斗形结构而产生表达瓶颈，解决办法是作者提出了一种特征图缩小方法，更复杂的池化。

**2 . 高维特征更易处理。** 高维特征更易区分，会加快训练。

**3. 可以在低维嵌入上进行空间汇聚而无需担心丢失很多信息。** 比如在进行3x3卷积之前，可以对输入先进行降维而不会产生严重的后果。假设信息可以被简单压缩，那么训练就会加快。

**4 . 平衡网络的宽度与深度。**

上述的这些并不能直接用来提高网络质量，而仅用来在大环境下作指导。

## **Factorizing Convolutions with Large Filter Size**

大尺寸的卷积核可以带来更大的感受野，但也意味着更多的参数，比如5x5卷积核参数是3x3卷积核的25/9=2.78倍。为此，作者提出可以用2个连续的3x3卷积层(stride=1)组成的小网络来代替单个的5x5卷积层，(保持感受野范围的同时又减少了参数量)如下图：（这个其实在VGG里面提出过了）
![这里写图片描述](https://img-blog.csdn.net/20160228155204525)

然后就会有2个疑问：

**1 . 这种替代会造成表达能力的下降吗？**

后面有大量实验可以表明不会造成表达缺失；

**2 . 3x3卷积之后还要再加激活吗？**

作者也做了对比试验，表明**添加非线性激活会提高性能**。

从上面来看，大卷积核完全可以由一系列的3x3卷积核来替代，那能不能分解的更小一点呢。文章考虑了 **nx1 卷积核**。

如下图所示的取代3x3卷积：
![这里写图片描述](https://img-blog.csdn.net/20160228155219087)

于是，任意nxn的卷积都可以通过1xn卷积后接nx1卷积来替代。实际上，作者发现**在网络的前期使用这种分解效果并不好，还有在中度大小的feature map上使用效果才会更好**。（对于mxm大小的feature map,建议m在12到20之间）。

总结如下图：

![这里写图片描述](https://img-blog.csdn.net/20160228155230994)

**(1)** 图4是GoogLeNet V1中使用的Inception结构；

**(2)** 图5是用3x3卷积序列来代替大卷积核；

**(3)** 图6是用nx1卷积来代替大卷积核，这里设定n=7来应对17x17大小的feature map。该结构被正式用在GoogLeNet V2中。即非对称个卷积核，其实类似于卷积运算中，二维分解为1维计算，提高了计算速度。



## **优化辅助分类器**



作者发现，V1中的辅助分类器有点问题：auxiliary classifiers在训练初期的时候并不能加速收敛，只有当训练快结束的时候它才会略微提高网络精度。

![](http://upload-images.jianshu.io/upload_images/2228224-b473d26009856550.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)

Auxiliary Classifiers

然后Szegedy就把第一个auxiliary classifiers去掉了！还说，auxiliary classifiers能够起到regularizer的作用。原文如下：

> 
Instead, we argue that the auxiliary classifiers act as regularizer. This is supported by the fact that the main classifier of the network performs better if the side branch is batch-normalized or has a dropout layer. This also gives a weak supporting evidence for the conjecture that batch normalization acts as a regularizer.






## **优化池化**



按照传统的做法，在pooling之前，为了防止信息丢失，应当加入了expand层，如下图右半部分。

![](http://upload-images.jianshu.io/upload_images/2228224-126bf44f3c657546.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)

Unefficient grid size reduction

这么做有个问题，会增加运算量，于是Szegedy就想出了下面这种pooling层。

![](http://upload-images.jianshu.io/upload_images/2228224-0241d70fee2df17b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/600)

Efficient grid size reduction

上图可以这么理解，Szegedy利用了两个并行的结构完成grid size reduction，分别是conv和pool，就是上图的右半部分。左半部分是右半部分的内部结构。

为什么这么做？我是说这种结构是怎么设计出来的？Szegedy只字不提，或许这就是深度学习的魅力所在吧。

## **优化标签**

Szegedy用了将近一页的篇幅叙述label smooth，可见这个方法有多么难理解。

深度学习用的labels一般都是one hot向量，用来指示classifier的唯一结果，这样的labels有点类似信号与系统里的脉冲函数，或者叫“Dirac delta”，即只在某一位置取1，其它位置都是0。

Labels的脉冲性质会引发两个不良后果：一是over-fitting，另外一个是降低了网络的适应性。这段话我实在没看懂，附上原文：

> 
First, it may result in over-fitting: if the model learns to assign full probability to the groundtruth label for each training example, it is not guaranteed to generalize. Second, it encourages the differences between the largest logit and all others to become large, and this, combined with the bounded gradient, reduces the ability of the model to adapt.


Szegedy在阐述完上面两个缺点后，补充了一句话，说不良后果的产生就是因为网络对它预测的东西太自信了。

> 
Intuitively, this happens because the model becomes too confident about its predictions.


好了，先不管这么多，这个label smooth具体是怎么实现的？就是下式：

![](http://upload-images.jianshu.io/upload_images/2228224-d5d5982ba2186072.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/280)

label smooth

为了方便理解，转换成python代码，就是这样的：

```python
`new_labels = (1.0 - label_smoothing) * one_hot_labels + label_smoothing / num_classes`
```

Szegedy在网络实现的时候，令 label_smoothing = 0.1，num_classes = 1000。Label smooth提高了网络精度0.2%。

我对Label smooth理解是这样的，它把原来很突兀的one_hot_labels稍微的平滑了一点，枪打了出头鸟，削了立于鸡群那只鹤的脑袋，分了点身高给鸡们，避免了网络过度学习labels而产生的弊端。
- Sanjeev Arora, Aditya Bhaskara, Rong Ge, and Tengyu Ma. Provable bounds for learning some deep representations. CoRR, abs/1310.6343, 2013.[↩](http://blog.csdn.net/shuzfan/article/details/50738394#fnref:1x)
- Min Lin, Qiang Chen, and Shuicheng Yan. Network in network. CoRR, abs/1312.4400, 2013.[↩](http://blog.csdn.net/shuzfan/article/details/50738394#fnref:2x)

V4在后面先学习完ResNet后，再补充，因为作者是结合了ResNet后，提出的V4



