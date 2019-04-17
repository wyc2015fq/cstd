# Deep Residual Networks（ResNet） 简介 - 快来学习鸭～～～ - CSDN博客





置顶2017年02月10日 20:07:42[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：13252








**author:DivinerShi**

原文地址：[http://blog.csdn.net/sxf1061926959/article/details/54973588](http://blog.csdn.net/sxf1061926959/article/details/54973588)

Kaiming He 的《Deep Residual Learning for Image Recognition》获得了CVPR最佳论文。他提出的深度残差网络在2015年可以说是洗刷了图像方面的各大比赛，以绝对优势取得了多个比赛的冠军。而且它在保证网络精度的前提下，将网络的深度达到了152层，后来又进一步加到1000的深度，可想而知，残差网络的实力，正如他在ICML2016上tutorial首页写的那样: 

    Deep Residual Networks ——Deep Learning Gets Way Deeper。

## 简介

深度卷积神经网络在图像分类方面获得了突破性的成绩。而最近的证据显示网络的深度至关重要，在imagenet领先的团队都利用了很深的模型，深度至少是十六以上，十六到三十左右。而且很多特殊的视觉识别任务也从深度模型中大大受益。

那么是否叠加层数就可以获得一个更好的网络呢？答案显然是不是的。其中对主要的原因BP反传带来的梯度弥散问题。在经过梯度反传后，经过几层后反传的梯度会彻底消失，导致当网络层数增加后，梯度无法传到的层就好比没有经过训练一样。当然由于RELU、中心归一化层的出现使得网络的深度变大，但是也还是没有从根本上解决梯度弥散的问题。单纯的把layer叠起来深层网络的效果反而不如合适层数的较浅的网络效果好，这种情况称为网络退化。

但是当网络深度继续增加的时候，错误率又高了，为啥呢？作者经过实验发现，错误率的提高不是因为网络变深产生了过拟合，而是网络自身结构的误差下限提高了。如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170210201029750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图显示了20层和56层“平原”网络时CIFAR-10的训练误差（左）和测试误差（右），深度网络有着更高的训练误差，并且造成测试误差。表明了网络变深后网络变得不容易优化。所以作者提出来一种残差学习方法，可以使得网络能进一步扩大深度，并且能更容易优化，提高精度。

## 深度残差学习

作者指出，如果继续添加层可以被构造为身份映射，那么一个更深的模型的训练误差是不应该比其相应的更浅的模型训练误差大的。但是就像我们前面说的，我们在继续增加层数后，训练误差反而变大，所以作者提出了用残差来重构网络的映射，说白了就是把输入x再次引入到结果，这样堆叠层的权重会趋向于零，学习起来会简单，能更加方便逼近身份映射。说的通俗一点就是说将x经过网络映射为F(x)+x，那么网络的映射F(x)自然就趋向于F(x)=0。

残差网络的单个构建模块并不复杂。假设单个构建模块（可以先理解为几层平原层）中，我们最终需要的结果为H(x),因为理论上一个由几个堆叠层匹配的（不一定是整个网络）基础映射可以拟合任意函数，那么自然也可以拟合H(x)-x，而且可能去拟合H(x)-x更方便更准确。所以作者使用堆叠层去拟合H(x)-x,然后再在最后将x重新加上去，就得到了H(x)。这里我们把H(x)-x定义为F(x)。我们用图表示这个模块就如下图： 
![这里写图片描述](https://img-blog.csdn.net/20170210201118767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这就是残差网络的一个模块，文章中使用公式进行定义，公式如下： 
![这里写图片描述](https://img-blog.csdn.net/20170210201148845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中x和y分别为图2模块的输入和输出。F（x，{Wi}）表示为我们需要去训练的残差映射函数。而且通过前面这个图我们能发现相同的堆叠层和残差模块，只是多加了一个x，文中把这步叫做快捷连接（shortcut connection），不但方便实现，而且方便比较相同层的堆叠层和残差层之间的优劣。

**还有一点**，因为F（x，{Wi}）和x都是向量，向量加减需要相同长度，所以这里必须保证它们两个大小相同，或者文章也说了，使用一个线性映射来使得两者相同，如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/20170210201208315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而且模块内部的结构不是固定的，上图2就是使用了一个两层的全链接的网络，当只有一层的时候，那么网络就是一个简单的线性函数y=W1x+x。

残差网络就是根据这样的构建模块，进行叠加，文章给出了使用在imagenet上的resnet，并给出了平原网络和残差网络两种网络结构的比较。

## 比较

**平原网络**： 

即不使用残差，是从VGG启发获得的网络结构。卷积层主要有卷积层主要有3X3的过滤器并遵循两个简单的设计规则： 

    （i）对于相同的输出特征映射大小，图层有相同数量的滤波器； 

    （ii）如果特征映射的大小被减半，过滤器的数量增加一倍，以保持每层的时间复杂度。我们通过有一个跨度为2的卷积层，直接进行下采样。网络以一个全局平均池层和有着最温和的1000路全连接层结尾。图3（中间）中加权层的总数是34。值得注意的是，文章的模型比VGG网（图3，左）有着更少的过滤器和更低的复杂度。我们的34层基线有3.6亿个触发器（乘加），这只有vgg-19的18%（19.6亿触发器）。
**这个应该很好理解，CNN网络叠加都需要这样。**

**残差网络**： 

基于上面的平原网络，作者插入快捷连接（图3，右），将网络转变成其对应的残差版本。其实也就是将x直接连接到了模块输出。当输入，输出大小相同的时候（实心线的捷径在图3），快捷连接（公式（1））可以直接使用。当尺寸增加时（虚线快捷键在图3）中，我们考虑了两个选项： 
**1、**快捷方式仍然执行身份映射，不足的用零填充整个向量，直到和输入大小相同。该选项不会引入额外的参数； 
**2、**公式（2）中的投影捷径，用于匹配尺寸（完成1×1卷积）。并使用和feature maps相同的跨度大小。
**图3如下图所示：**

![这里写图片描述](https://img-blog.csdn.net/20170210201454913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果还有什么地方不明白的，可以看看何凯明大神在ICML2016上的tutorial演讲，他深入浅出的阐述了残差网络的含义。这篇文章对其进行了整理[http://www.leiphone.com/news/201608/vhqwt5eWmUsLBcnv.html](http://www.leiphone.com/news/201608/vhqwt5eWmUsLBcnv.html)

我这也附上作者的原文，感兴趣的或者想要了解具体的实验过程以及参数的最好看看原文，顺便膜拜下大神： 
[https://arxiv.org/pdf/1512.03385v1.pdf](https://arxiv.org/pdf/1512.03385v1.pdf)

下图是实验效果比较图： 
![这里写图片描述](https://img-blog.csdn.net/20170210203221698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
照顾下英文不好的同学，细心的小伙伴将原文翻译成了中文： 
[http://www.leiphone.com/news/201606/BhcC5LV32tdot6DD.html](http://www.leiphone.com/news/201606/BhcC5LV32tdot6DD.html)

## 参考文献

[1] [https://arxiv.org/pdf/1512.03385v1.pdf](https://arxiv.org/pdf/1512.03385v1.pdf)

[2][http://www.leiphone.com/news/201608/vhqwt5eWmUsLBcnv.html](http://www.leiphone.com/news/201608/vhqwt5eWmUsLBcnv.html)

[3] [http://www.leiphone.com/news/201606/BhcC5LV32tdot6DD.html](http://www.leiphone.com/news/201606/BhcC5LV32tdot6DD.html)










