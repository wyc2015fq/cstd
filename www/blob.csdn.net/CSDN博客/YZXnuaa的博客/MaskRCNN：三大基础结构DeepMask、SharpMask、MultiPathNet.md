# MaskRCNN：三大基础结构DeepMask、SharpMask、MultiPathNet - YZXnuaa的博客 - CSDN博客
2018年03月28日 10:37:23[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1753
                   MaskXRCnn俨然成为一个现阶段最成功的图像检测分割网络，关于MaskXRCnn的介绍，需要从MaskRCNN看起。
        当然一个煽情的介绍可见：[何恺明团队推出Mask^X R-CNN](https://zhuanlan.zhihu.com/p/31625007)，将实例分割扩展到3000类。
        MaskRCnn取得的精细结果有三个主要技术构架：DeepMask、SharpMask、MultiPathNet。MaskRCNN与普通FNN的典型不同之处，重要两点为添加了SharpMask、MultiPathNet。
        文章链接：[FaceBook的物体检测新框架](http://blog.csdn.net/u011718701/article/details/53762969)研究
**Index**
一、DeepMask【Learning to Segment Object Candidates.2015 NIPS】[DeepMask](http://blog.csdn.net/u011718701/article/details/53763352)笔记
二、SharpMask【Learning to Refine Object Segments.2016 ECCV】[SharpMask笔记](http://blog.csdn.net/u011718701/article/details/53768731)
三、MultipathNet【A MultiPath Network for Object Detection.2016 BMVC】
       FAIR开发了一项用于**发现和切割单张图像中每个物体**的新技术，这一技术的主要驱动算法是DeepMask——一个新的图像分割框架，以及SharpMask——一个图像分割refine模型，二者结合使得FAIR的机器视觉系统能够**感知并且精确的描绘一张图片中任何物体的轮廓。**这一识别管道中的最后一步，研究院使用了一个特殊的卷积网络，称为MultiPathNet，为图片中检测到的物体添加标签。也就是说Facebook研究院的物体检测系统遵循一个三阶段的过程：（1）DeepMask生成初始物体mask（2）SharpMask优化这些mask（3）MutiPathNet识别每个mask框定的物体。
- DeepMask的技巧是把**分割看成是一个海量的二进制分类问题**
- - 对一张图像中的每一个重叠的图像块：这个图像块包含一个物体吗？如果包含，那对于一个图像块中的每个像素：这个像素是图像块中心物体的一部分吗？用深度网络来回答每一个Yes/No的问题
- - 上层功能以相当低的空间分辨率计算，这为mask预测带来一个问题：mask能捕捉一个物体大致外形，但不能准确捕捉物体边界。
- SharpMask优化DeepMask的输出，产生具有更高保真度的能精确框定物体边界的mask
- - 在DeepMask预测前向通过网络时，SharpMask反转信息在深度网络的流向，并通过使用progressively earlier layers的特性来优化DeepMask做的预测。
- - 要捕捉物体外形，你必须高度理解你正在看的是什么（DeepMask）；但是要准确框出边界，你需要使用低层次的特性一直到像素级（SharpMask）
- DeepMask不知道具体对象类型，尽管可以框定但不能区分物体；以及没有选择性，会为不是很有趣的图像区域生成mask
- - 训练一个单独的深度网络来对每一个DeepMask产生的mask的物体类型进行分类（包括“无”），采用R-CNN
- - 改进是使用DeepMask作为R-CNN的第一阶段。
- - 对于RCNN的第二阶段，使用一个专门的网络架构来对每一个mask进行分类，也就是MultiPathNet，允许信息以多种路径通过网络，从而使其能够在多种图像尺寸和图像背景中挖掘信息。
- MultiPathNet目的是提高物体检测性能，包括定位的精确度和解决一些尺度、遮挡、集群的问题。网络的起点是Fast R-CNN。
- - 基本上，MultiPathNet就是把Fast R-CNN与DeepMask/SharpMask一起使用，但是做了一些特殊的改造，例如：skip connections、foveal regions和integral loss function。
- - 
     请继续参考原文...............
## MultiPathNet
**A MultiPath Network for Object Detection**
- intro: BMVC 2016. Facebook AI Research (FAIR)
- arxiv: [http://arxiv.org/abs/1604.02135](http://arxiv.org/abs/1604.02135)
- github: [https://github.com/facebookresearch/multipathnet](https://github.com/facebookresearch/multipathnet)
原文链接： **[******A MultiPath Network for Object Detection](http://blog.csdn.net/app_12062011/article/details/77941771)-分割网络**
        CNN一般完成对框的检测，而MaskRCNN则识别Mask。
       有了DeepMask输出的粗略分割mask，经过SharpMask refine边缘，接下来就要靠MultiPathNet来对mask中的物体进行识别分类了。MultiPathNet目的是提高物体检测性能，包括定位的精确度和解决一些尺度、遮挡、集群的问题。网络的起点是Fast R-CNN，基本上，MultiPathNet就是把Fast R-CNN与DeepMask/SharpMask一起使用，但是做了一些特殊的改造，例如：skip connections、foveal regions和integral loss function。
1.背景工作
          显然自从Fast R-CNN出现以来的object detector基本都是将它作为起点，进行一些改造，我们先来总结一下这些改造，以便理解本文的想法。Context核心思想就是利用物体周围的环境信息，比如有人在每个物体周围crop了10个contextual区域辅助检测。本文就是借鉴这种做法不过只用了4个contextual区域，涉及特殊的结构。
Skip connections
     Sermanet提出一个多阶段的分类器利用许多个卷积层的特征来实现行人检测，取得了很好的效果，这种‘skip’结构最近在语义分割方面也很火呐
       Classifers
大家都知道现在基本上是CNN结构的天下啦。。。本文用的是VGG-D，如果和何凯明的ResNet结合效果应该会更好哒。
2.网络结构
先上整个结构图：
![](https://img-blog.csdn.net/20161221202555123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTcxODcwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**Foveal regions**
        像Fast R-CNN一样，图像先经过VGG16的13个卷积层生成conv feature map，然后经过RoI pooling层提取特征。从结构图里可以看到，对每个object proposal，都产生4个不同的region crops，相当于从不同的尺度来看这个物体，这就是所谓的‘foveal region’。
**Skip connections**
       VGG16有四个pooling层，所以图像尺寸会/16，那我们设想一个32*32大小的物体，经过VGG16就剩2*2了。虽然RoI pooling层输出7*7的feature map，但是很显然我们损失了太多的空间信息 。
所以呢，要把conv3（/4*256）、conv4（/8*512）、conv5（/16*512）层的【RoI-pooled normalized 】feature级联起来一起送到foveal分类器，这种级联使得foveal分类器可以利用不同位置的特征，有种弥补空间信息损失的味道在里面呢~注意需要用1*1的卷积层把级联特征的维度降低到分类器的输入维度。
文章说，他们只把conv3连接到1×的分类器head，conv4连接到1×、1.5×和2×的head。
- **Integral Loss**
        在原来的Fast R-CNN中，如果一个proposal与一个ground-truth box的IoU大于50，那么就给它分配ground-truth的标签，否则就是0。然后在Fast R-CNN的分类loss里面（用的是log loss），对所有proposal的IoU大于50的一样对待，这其实有点不合理：我们希望IoU越大的对loss的贡献越大吧~所以本文就用Integral loss代替了原来Fast-RCNN的分类loss：
![](https://img-blog.csdn.net/20161222193713457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTcxODcwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       那当然积分就要用加和的方式近似啦，本文选择用n=6个不同的阈值，所以现在对每一个object rpoposal实际上有n个ground-truth label了，每个阈值一个~相应的每个阈值应该也得有一个分类输出的概率吧，所以我们看到结构图最后有6个分类器。在inference的时候，每个分类器的softmax概率输出求平均得到最终的class probabilities。
        至此，Mask识别完成。
