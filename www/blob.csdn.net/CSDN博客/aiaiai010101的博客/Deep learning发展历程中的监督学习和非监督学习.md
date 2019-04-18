# Deep learning发展历程中的监督学习和非监督学习 - aiaiai010101的博客 - CSDN博客

2017年03月28日 17:51:37[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：2178



# [Deep learning系列（十五）有监督和无监督训练](/u012526120/article/details/49591213)

标签： [深度学习](http://www.csdn.net/tag/%E6%B7%B1%E5%BA%A6%E5%AD%A6%E4%B9%A0)[有监督训练](http://www.csdn.net/tag/%E6%9C%89%E7%9B%91%E7%9D%A3%E8%AE%AD%E7%BB%83)[无监督训练](http://www.csdn.net/tag/%E6%97%A0%E7%9B%91%E7%9D%A3%E8%AE%AD%E7%BB%83)[自编码器](http://www.csdn.net/tag/%E8%87%AA%E7%BC%96%E7%A0%81%E5%99%A8)

2015-11-02 19:59
3906人阅读[评论](#comments)(0)[收藏]()[举报](#report)

本文章已收录于：

![](http://static.blog.csdn.net/images/category_icon.jpg)分类：

deep-learning*（14）*![](http://static.blog.csdn.net/images/arrow_triangle%20_down.jpg)![](http://static.blog.csdn.net/images/arrow_triangle_up.jpg)

[作者同类文章](http://blog.csdn.net/u012526120/article/category/5851589)X

版权声明：本文为博主原创文章，未经博主允许不得转载。

目录[(?)](#)[[+]](#)
- [前言](#t0)
- [萌芽期](#t1)
- [迅速发展期](#t2)
- [爆发期](#t3)
- [总结](#t4)


### **1. 前言**

在学习深度学习的过程中，主要参考了四份资料：
- 台湾大学的机器学习技法公开课；
- [Andrew NG的深度学习教程](http://ufldl.stanford.edu/wiki/index.php/UFLDL_Tutorial)；
- [Li feifei的CNN教程](http://cs231n.stanford.edu/syllabus.html)；
- [caffe官网的教程](http://caffe.berkeleyvision.org/)；

对比过这几份资料，突然间产生一个困惑：台大和Andrew的教程中用了很大的篇幅介绍了无监督的自编码神经网络，但在Li feifei的教程和caffe的实现中几乎没有涉及。当时一直搞不清这种现象的原因，直到翻阅了深度学习的发展史之后，才稍微有了些眉目。

深度学习的发展大致分为这么几个时期：
- 萌芽期。从BP算法的发明（1970s-1980s）到2006年期间。
- 迅速发展期。从2006年栈式自编码器+BP微调提出之后。
- 爆发期。2012年Hilton团队的Alexnet模型在imagenet竞赛取得惊人成绩之后。

### **2. 萌芽期**

在Yann LeCun、Yoshua Bengio和Geoffrey Hinton三巨头nature深度学习综述[deep learning](http://www.nature.com/nature/journal/v521/n7553/full/nature14539.html)文章中提到，这段期间神经网络模型被主流的计算机视觉和学术界所抛弃。

这期间，学者们试图用**有监督**学习的方式训练深度神经网络，然而方法不是十分奏效，陷入了困境，在Andrew的教程中可以找到大概这几点原因：
- **数据获取问题**。有监督训练需要依赖于有标签的数据才能进行训练。然而有标签的数据通常是稀缺的，因此对于许多问题，很难获得足够多的样本来拟合一个复杂模型的参数。例如，考虑到深度网络具有强大的表达能力，在不充足的数据上进行训练将会导致过拟合。
- **局部极值问题**。使用监督学习方法来对浅层网络（只有一个隐藏层）进行训练通常能够使参数收敛到合理的范围内。但是当用这种方法来训练深度网络的时候，并不能取得很好的效果。特别的，使用监督学习方法训练神经网络时，通常会涉及到求解一个高度非凸的优化问题。对深度网络而言，这种非凸优化问题的搜索区域中充斥着大量“坏”的局部极值，因而使用梯度下降法（或者像共轭梯度下降法，L-BFGS等方法）效果并不好。
- **梯度弥散问题**。梯度下降法在使用随机初始化权重的深度网络上效果不好的技术原因是：梯度会变得非常小。具体而言，当使用反向传播方法计算导数的时候，随着网络的深度的增加，反向传播的梯度（从输出层到网络的最初几层）的幅度值会急剧地减小。结果就造成了整体的损失函数相对于最初几层的权重的导数非常小。这样，当使用梯度下降法的时候，最初几层的权重变化非常缓慢，以至于它们不能够从样本中进行有效的学习。这种问题通常被称为“梯度的弥散”。

因为一直没找到有效解决这些问题的方法，这期间，深度神经网络的发展一直不温不火。或者说在2001年Hochreiter的[Gradient flow in recurrent nets: the difficulty of learning long-term
 dependencies](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.24.7321&rep=rep1&type=pdf)（好像是这篇）提出了神经网络存在的几个问题后，在接下来的几年内神经网络的发展陷入了低谷。而那几年流行的[机器学习](http://lib.csdn.net/base/2)[算法](http://lib.csdn.net/base/31)是SVM和集成模型（随机森林、adaboost等），如下图所示。

![图片名称](https://img-blog.csdn.net/20151102205122969)

### **3. 迅速发展期**

06年Hilton在nature上发表了一篇文章[Reducing the dimensionality of data with neural networks](http://www.lsv.uni-saarland.de/Seminar/ML_for_NLP_SS12/HinSal06.pdf)，针对上面提到的三个深度学习问题，提出了栈式自编码器+BP微调的解决方案。在一定程度上解决了上面的三个问题：
- 栈式自编码神经网络是**无监督学习算法**。因而不需要规模很大的有标签样本。
- 经过自编码神经网络训练后的参数已经落在一个较优的位置上，从这个位置开始BP微调，不用担心局部极值问题。
- 自编码神经网络训练已经使深度网络的前几层参数具有表达能力了，比如可以提取出图片的边，局部组建等，即使有梯度弥散问题，前几层参数不再更新，也不会影响最终深度网络的表达能力。

因为上面的原因，在经历01年神经网络的低谷后，深度学习开启的崭新的浪潮，走上了发展的快车道，从上图的红线可以明显的看出。

### **4. 爆发期**

在12年的ILSVRC竞赛中，Hilton团队的Alexnet模型[Imagenet classification with deep convolutional neural networks](http://arche.depotoi.re/autoblogs/wwwinternetactunet_8a3fe3331e0ad7327e18d9fe6ec3f0ad04dcea58/media/4635014e.imagenet.pdf)将1000类分类的top-5误差率降低到了15.3%，碾压了第二名使用SVM算法的26.2%，开启了深度学习的革命，从此之后，深度学习走上了指数式的发展道路。在15年CVPR的文章中，我关注的两个方向场景语义标注和显著物体检测，有相当大比例的文章中涉及CNN或者deep的字眼，估计明年CVPR文章中深度学习的比例会更高。工业界的火爆就不用多提了，从Yann
 LeCun、Yoshua Bengio和Geoffrey Hinton三巨头到颜水成、li feifei这样的视觉方向大牛都被挖到互联网公司就可见一斑。

回到Hilton团队的Alexnet模型上，仅仅使用了**有监督的训练**，貌似没涉及无监督的预训练。不是在之前说有监督的深度学习训练存在很多问题吗，大概是因为这几条原因，导致了有监督训练的可行：
- 大规模标注数据的出现。在ILSVRC使用的数据集包括120万的训练图片，5万张验证图片和15万张测试图片。这些图片都是有标注的（属于1000类），而在imagenet出现之前，这样规模的标注数据是不存在的。
- 对于局部极值的问题，nature综述中，三个大牛作者的说法是：对于深度网络来说，局部极值从来都不是一个问题，从任意的初始参数值开始训练网络，最后都能达到相似的分类效果。这也是被**最近**的理论和实践所证明的。
- 对于梯度弥散导致的收敛速度慢问题。Alexnet模型的两大利器：ReLU激活函数和GPU并行加速。前者使SGD有6倍的加速，后者使用两块GTX580GPU也极大的加快了SGD的收敛速度，两者效果相乘，使得无监督预训练几乎是多余的了，梯度弥散问题也不再是一个很大的问题。

### **5. 总结**

从上面介绍可以看出，[Andrew NG的教程](http://ufldl.stanford.edu/wiki/index.php/UFLDL_Tutorial)是06年到12年之间的产物，当时**无监督训练**是主流，[Li feifei的CNN教程](http://cs231n.stanford.edu/syllabus.html)和[caffe官网的教程](http://caffe.berkeleyvision.org/)是产生于12年之后，这时[数据库](http://lib.csdn.net/base/14)足够大（上千万级别），模型足够先进（ReLU激活函数，dropout等等），同时计算速度足够快（GPU加速），使得无监督预训练（自编码神经网络）在很多应用场景中失去了存在的价值，有监督训练已足够完成任务。

一句话总结，06年的无监督预训练开启了深度学习的纪元，在之后深度学习快速发展的过程中，[大数据](http://lib.csdn.net/base/20)的获得、计算机硬件的发展以及深度模型的升级使得有监督训练重新走上舞台，无监督预训练也算是完成了历史使命。

那预训练还有用吗？答案是肯定的，比如我们有一个分类任务，数据库很小，这时还是需要通过预训练来避免深度模型的过拟合问题的，只不过预训练是通过在一个大的数据库上（比如imagenet），通过**有监督的训练**来完成的。这种有监督预训练加小的数据库上微调的模式称为[Transfer learning](http://blog.csdn.net/u012526120/article/details/49496617)，在[Li
 feifei的CNN教程](http://cs231n.stanford.edu/syllabus.html)和[caffe官网的教程](http://caffe.berkeleyvision.org/)中都有详细的介绍。

除此之外，[Andrew NG的教程](http://ufldl.stanford.edu/wiki/index.php/UFLDL_Tutorial)也有其它几点常用于12年之前但现在很少用的细节，比如这个教程中介绍的激活函数是sigmoid，现在很少见了，几乎被[ReLU激活](http://blog.csdn.net/u012526120/article/details/49149317)函数取代，优化算法用的是L-BFGS，现在的优化算法主流是[SGD+momentum](http://blog.csdn.net/u012526120/article/details/49183279)。这些教程之间的不同点在当时学习之初是很困惑的，直到明白了深度学习的发展历程，才渐渐了解这些不同的来源。

**个人阅读理解：**

**解释了我这段时间以来的“为什么caffe中没有预训练这一环节”的困惑。**

#### 本文转载自：http://blog.csdn.net/u012526120/article/details/49591213#t4

