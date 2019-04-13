
# GoogLeNet - 郭云飞的专栏 - CSDN博客


2017年10月30日 17:50:50[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：674


*GoogLeNet*最早出现在2014年的*《Going deeper with convolutions》*，之所以叫*GoogLeNet*，而不是*GoogleNet*，文章说是为了向早期的*LeNet*致敬。*GoogLeNet*是谷歌团队为了参加*ILSVRC 2014（ImageNet Large Scale Visual Recognition Challenge）*比赛而精心准备的。*GoogLeNet*,在*2014*年*ILSVRC*挑战赛上获得冠军，将*Top5*的错误率降低到6.67%。总体上是一个**22**层的深度网络。
*GoogLeNet*凭借其优秀的表现，得到了很多研究人员的学习和使用，因此*Google*团队又对其进行了进一步发掘改进，产生了升级版本的*GoogLeNet*。文章为：*[《Rethinking
 the Inception Architecture for Computer Vision》](https://arxiv.org/abs/1512.00567)*。
**一、GoogLeNet V1（第一个版本）**
一般来说，提升网络性能最直接的办法就是增加网络深度和宽度，这也就意味着更巨量的参数。但是，巨量参数**容易产生过拟合**，也会**大大增加计算量**。文章认为解决上述两个缺点的根本方法是将全连接甚至一般的卷积都转化为**稀疏连接**。参考文献**《Provable bounds for learning some deep representations. CoRR, abs/1310.6343, 2013.》**认为对于大规模稀疏的神经网络，可以通过分析激活值的统计特性和对高度相关的输出进行聚类来逐层构建出一个最优网络。所以，为了**既能保持网络结构的稀疏性，又能利用密集矩阵的高计算性能**，论文提出了inception的结构！
**1、inception结构**
Inception 结构的主要思路是用密集成分来近似最优的局部稀疏结构。作者首先提出下图这样的基本结构：
![](https://img-blog.csdn.net/20171030182205010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于上图的解释为：
采用不同大小的卷积核意味着不同大小的感受野，最后拼接意味着不同尺度特征的融合；
之所以卷积核大小采用1、3和5，主要是为了方便对齐。设定卷积步长stride=1之后，只要分别设定pad=0、1、2，那么卷积之后便可以得到相同维度的特征，然后这些特征就可以直接拼接在一起了；
文章说很多地方都表明pooling挺有效，所以Inception里面也嵌入了。
网络越到后面，特征越抽象，而且每个特征所涉及的感受野也更大了，因此随着层数的增加，3x3和5x5卷积的比例也要增加。
但是，使用5x5卷积核仍然会带来巨大的计算量。因此论文借鉴了**NIN《Min Lin, Qiang Chen, and Shuicheng Yan. Network in network. CoRR, abs/1312.4400, 2013》**，采用**1x1卷积核来进行降维**。
关于1x1卷积核来进行降维，举个例子：
假如上一层的输出为100x100x128，经过具有256个输出的5x5卷积层之后(stride=1，pad=2)，输出数据为100x100x256。其中，卷积层的参数为128x5x5x256。此时如果上一层输出先经过具有32个输出的1x1卷积层，再经过具有256个输出的5x5卷积层，那么最终的输出数据仍为为100x100x256，但卷积参数量已经减少为128x1x1x32 + 32x5x5x256，大约减少了4倍。
改进后（降维）的Inception Module如下图：
![](https://img-blog.csdn.net/20171030183506942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2、GoogLeNet网络结构**
![](https://img-blog.csdn.net/20171030184100215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于上图的GoogLeNet网络结构，做如下几点说明：
显然GoogLeNet采用了模块化的结构，方便增添和修改。
网络在FC之前采用了average pooling层，想法来自NIN,事实证明可以将TOP1 accuracy提高0.6%。
可以看出，GoogLeNet网络其实也是非常深的，如果梯度从最后一层传递到第一层，梯度基本已经没有了。为了避免梯度消失，网络额外增加了2个辅助的softmax用于向前传导梯度。文章中说这两个辅助的分类器的loss应该加一个衰减系数，但看caffe中的model也没有加任何衰减。此外，实际预测时，这两个额外的softmax会被去掉。其实这种训练方式可以看作将几个不同深度的子网络合并到一块进行训练，由于网络的卷积核共享，因此计算的梯度可以累加，这样最终的梯度便不会很小。关于这一点做的最彻底的应该就是ResNet。

另外，为了达到最佳的性能，除了使用上述的网络结构外，还做了大量的辅助工作：包括训练多个model求平均、裁剪不同尺度的图像做多次验证等等。详细的这些可以参看文章的实验部分。本文的主要想法其实是想通过构建密集的块结构来近似最优的稀疏结构，从而达到提高性能而又不大量增加计算量的目的。GoogleNet的caffemodel大小约50M，但性能却很优异。
**二、GoogLeNet V2（改进版本）**
GoogLeNet V1出现的同期，性能与之接近的大概只有VGGNet了，并且二者在图像分类之外的很多领域都得到了成功的应用。但是相比之下，GoogLeNet的计算效率明显高于VGGNet，大约只有500万参数。只相当于VGGNet的1/12（GoogLeNet的caffemodel大约50M，VGGNet的caffemodel则要超过600M）。
GoogLeNet的表现很好，但是如果想要通过简单地放大Inception结构来构建更大的网络，则会立即提高计算消耗。此外，在V1版本中，文章也没给出有关构建Inception结构注意事项的清晰描述。因此，在V2中作者首先给出了一些已经被证明有效的用于放大网络的通用准则和优化方法。这些准则和方法适用但不局限于Inception结构。

避免表达瓶颈，特别是在网络靠前的地方。 信息流前向传播过程中显然不能经过高度压缩的层，即表达瓶颈。从input到output，feature map的宽和高基本都会逐渐变小，但是不能一下子就变得很小。比如你上来就来个kernel = 7, stride = 5 ,这样显然不合适。
对于输出的维度channel，一般来说会逐渐增多(每层的num_output)，否则网络会很难训练。（特征维度并不代表信息的多少，只是作为一种估计的手段）。
高维特征更易处理。 高维特征更易区分，会加快训练。
可以在低维嵌入上进行空间汇聚而无需担心丢失很多信息。 比如在进行3x3卷积之前，可以对输入先进行降维而不会产生严重的后果。假设信息可以被简单压缩，那么训练就会加快。
平衡网络的宽度与深度。
上述的这些并不能直接用来提高网络质量，而仅用来在大环境下作指导。
**1、把大的卷积核用若干个小的串联起来（Factorizing Convolutions with Large Filter Size）**
大尺寸的卷积核可以带来更大的感受野，但也意味着更多的参数，比如5x5卷积核参数是3x3卷积核的25/9=2.78倍。为此，作者提出可以用2个连续的3x3卷积层(stride=1)组成的小网络来代替单个的5x5卷积层，(保持感受野范围的同时又减少了参数量)如下图：
![](https://img-blog.csdn.net/20171030190205222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这种替代会造成表达能力的下降吗？大量的试验表明：这种转变不会造成表达的缺失。
3x3卷积之后还要再加激活吗？作者也做了对比试验，表明添加非线性激活会提高性能。
从上面来看，大卷积核完全可以由一系列的3x3卷积核来替代。那3x3卷积核能不能分解的更小呢，文章考虑了nx1 卷积核：
![](https://img-blog.csdn.net/20171030191228575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
任意nxn的卷积核都可以通过1xn卷积后接nx1卷积来替代。实际上，作者发现在网络的前期使用这种分解效果并不好，还有在中度大小的feature map上使用效果才会更好。（对于nxn大小的feature map,建议n在12到20之间）。
网络结构对比如下：
![](https://img-blog.csdn.net/20171030191529110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上左图4是GoogLeNet V1中使用的Inception结构；
上中图5是用3x3卷积序列来代替大卷积核；
上右图6是用nx1卷积来代替大卷积核，这里设定n=7来应对17x17大小的feature map。该结构被正式用在GoogLeNet V2中。
**三、Opencv加载Caffe训练好的GoogLeNet网络用于预测**





