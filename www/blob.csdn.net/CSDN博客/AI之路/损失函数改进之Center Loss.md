# 损失函数改进之Center Loss - AI之路 - CSDN博客





2017年08月09日 08:00:37[AI之路](https://me.csdn.net/u014380165)阅读数：25768








最近几年网络效果的提升除了改变网络结构外，还有一群人在研究损失层的改进，这篇博文要介绍的就是较为新颖的center loss。center loss来自ECCV2016的一篇论文：A Discriminative Feature Learning Approach for Deep Face Recognition。 

论文链接：[http://ydwen.github.io/papers/WenECCV16.pdf](http://ydwen.github.io/papers/WenECCV16.pdf)

代码链接：[https://github.com/pangyupo/mxnet_center_loss](https://github.com/pangyupo/mxnet_center_loss)
对于常见的图像分类问题，我们常常用softmax loss来求损失，关于softmax loss你可以参考这篇博文：[softmax，softmax-loss，BP的解释](http://blog.csdn.net/u014380165/article/details/71181256)。如果你的损失采用softmax loss，那么最后各个类别学出来的特征分布大概如下图Fig2。这个图是以MNISTt数据集做的实验，一共10个类别，用不同的颜色表示。从Fig2可以看出不管是训练数据集还是测试数据集，都能看出比较清晰的类别界限。

![这里写图片描述](https://img-blog.csdn.net/20170809075622675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果你是采用softmax loss加上本文提出的center loss的损失，那么最后各个类别的特征分布大概如下图Fig3。和Fig2相比，类间距离变大了，类内距离减少了（**主要变化在于类内距离：intra-class**），这就是直观的结果。

![这里写图片描述](https://img-blog.csdn.net/20170809075656563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

接下来详细介绍center loss。如果你还是不熟悉传统的softmax loss，那么先来看看传统的softmax loss。首先区分softmax和softmax loss的区别，可以看博客：[softmax，softmax-loss，BP的解释](http://blog.csdn.net/u014380165/article/details/71181256)。下面公式1中log函数的输入就是softmax的结果（是概率），而Ls表示的是softmax loss的结果（是损失）。**wx+b是全连接层的输出，因此log的输入就表示xi属于类别yi的概率。**

![这里写图片描述](https://img-blog.csdn.net/20170809075802673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么center loss到底是什么呢？先看看center loss的公式LC。cyi表示第yi个类别的特征中心，xi表示全连接层之前的特征。后面会讲到实际使用的时候，m表示mini-batch的大小。因此这个公式就是希望一个batch中的每个样本的feature离feature 的中心的距离的平方和要越小越好，也就是类内距离要越小越好。这就是center loss。

![这里写图片描述](https://img-blog.csdn.net/20170809075835685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

关于LC的梯度和cyi的更新公式如下：

![这里写图片描述](https://img-blog.csdn.net/20170809075857779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这个公式里面有个条件表达式如下式，这里当condition满足的时候，下面这个式子等于1，当不满足的时候，下面这个式子等于0.

![这里写图片描述](https://img-blog.csdn.net/20170809075916109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此上面关于cyi的更新的公式中，当yi（表示yi类别）和cj的类别j不一样的时候，cj是不需要更新的，只有当yi和j一样才需要更新。

作者文中用的损失L的包含softmax loss和center loss，用参数南木达（打不出这个特殊字符）控制二者的比重，如下式所示。这里的m表示mini-batch的包含的样本数量，n表示类别数。

![这里写图片描述](https://img-blog.csdn.net/20170809075933518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

具体的算法描述可以看下面的Algorithm1：

![这里写图片描述](https://img-blog.csdn.net/20170809075950159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




