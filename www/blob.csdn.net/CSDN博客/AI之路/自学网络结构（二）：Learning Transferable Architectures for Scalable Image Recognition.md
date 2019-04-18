# 自学网络结构（二）：Learning Transferable Architectures for Scalable Image Recognition - AI之路 - CSDN博客





2017年11月13日 22:36:53[AI之路](https://me.csdn.net/u014380165)阅读数：2046标签：[深度学习与计算机视觉](https://so.csdn.net/so/search/s.do?q=深度学习与计算机视觉&t=blog)
个人分类：[深度学习](https://blog.csdn.net/u014380165/article/category/6829229)









论文：Learning Transferable Architectures for Scalable Image Recognition 

链接：[https://arxiv.org/abs/1707.07012](https://arxiv.org/abs/1707.07012)

**Google Brain的作品，关于用神经网络学习另一个神经网络的结构。**我们知道现在图像分类、检测算法的优劣很大一部分取决于网络结构的设计，最近今年比较优秀的分类网络结构比如VGG，ResNet，DenseNet等在网络结构的设计上下了不少功夫。Google的这支团队认为人为设计的结构不一定是最佳的，因此希望能通过一个神经网络来按照一定策略学习得到最佳的网络结构。在这里要先提到另一篇论文，也就是ICLR2017的Neural architecture search with reinforcement learning（暂时称为NAS），一作是同一个人，那篇文章通过用强化学习在一个search space中搜索最优的网络结构，可以简单看下面的Figure1，或者参看博客：  
[自学网络结构（一）：Neural Architecture Search With Reinforcement Learning](http://blog.csdn.net/u014380165/article/details/78525500)。那篇文章中实验都是在CIFAR-10上做的，所以那种搜索方式可以在能接受的时间范围内达到目的，但是如果想要在ImageNet数据集应用就不大行，因此就有了这篇文章，也就是**设计一个合适的search space，使得在CIFAR-10上得到的最好的网络结构可以方便地迁移到ImageNet这样的图像尺寸大且数量也多的数据集上**，因此这篇文章可以看做的ICLR2017那篇文章的升级版（搜索速度比之前快了7倍左右）。

**总的来看这篇文章不仅借鉴了NAS（Neural Architecture Search）的训练结构思路（Figure1），只不过在更新controller参数的时候采用 Proximal Policy Optimization (PPO)，而不是原先的policy geadient method；同时借鉴了目前优秀网络结构（ResNet，GoogleNet）的重复堆叠思想，后面会提到堆叠的基本单元是convolutional cell，这是和原来的NAS很不一样的地方。**

那么这个自动学习得到的模型效果怎么样呢？总结起来就是一句话：**The resulting architectures approach or exceed state-of-the-art performance in both CIFAR-10 and ImageNet datasets with less computational demand than humandesigned architectures. 同时普遍看来，通过自动学习得到的网络的训练时间比人工设计的网络的训练时间要短许多。**

Figure1是ICLR2017的NAS算法的示意图，解释的是如何通过一个controller（RNN结构）训练得到一个网络结构A并训练A得到准确率R最后利用R更新controller。ICLR2017的NAS设计的search space使得搜索过程极其耗费计算资源，因此这篇文章虽然借鉴了NAS的整体结构，但是在search space的设计及优化方式上有所改进。

![这里写图片描述](https://img-blog.csdn.net/20171113223215107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面提到过这篇文章借鉴了ResNet和GooleNet中网络结构堆叠的思想，这篇文章中堆叠的最小单位就是convolution cell，而convlolution cell主要包含两种：第一种是不改变输入feature map的大小的卷积，也就是Figure2中的Normal Cell；第二种是将输入feature map的长宽各减少为原来的一半的卷积，也就是Figure2中的Reduction Cell。因此当网络结构定义如Figure2，那么Figure1中的controller就用来预测Figure2中的Normal Cell和Reduction Cell。

![这里写图片描述](https://img-blog.csdn.net/20171113223232508?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么convolution cell到底是什么呢？首先Figure2中的cell结构中每个cell包含B个block，那么什么是block？**Figure3左边中的5个框组成的就是一个block，通过重复B个这样的block就得到了一个convolution cell**。在Figure3中每个block包含5个prediction steps。这5个steps分别是： 

Step 1. Select a hidden state from h or from the set of hidden states created in previous blocks. 

Step 2. Select a second hidden state from the same options as in Step 1. 

Step 3. Select an operation to apply to the hidden state selected in Step 1. 

Step 4. Select an operation to apply to the hidden state selected in Step 2. 

Step 5. Select a method to combine the outputs of Step 3 and 4 to create a new hidden state. 

因为每个convolution cell包含B个block，每个block包含5个softmax层，所以每个convolution cell包含5B个softmax层。 
**上面这5个step就是本文的核心了，也就是作者设计的search space**。这是和ICLR2017那篇文章最不一样的地方，在ICLR2017那篇文章中的search space包含了filter的size，stride等等，而这篇文章则通过构造convolution cell的方式大大降低了search的难度。 
**Figure3中左右两个图都是一个block的示意图，且是相互对应的，比如左图中的两个灰色框对应右图中的最底下两个虚线框，左图中的两个黄色框对应右图中的3*3 convhe 2*2 maxpool，左图中的绿色框对应右图中的add操作。**
![这里写图片描述](https://img-blog.csdn.net/20171113223322710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

顺便提一下，上面提到的step3和step4中的operation（Figure3中的两个黄色框）包含下面这些卷积神经网络中常规的操作。

![这里写图片描述](https://img-blog.csdn.net/20171113223347673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面说过Figure1中的controller是用来预测Figure2中的Normal Cell和Reduction Cell，结合前面介绍的step和block可以得到每个controller有2*5B个prediction，其中前5B个prediction是用于预测Normal Cell，后5B个prediction是用来预测Reduction Cell。

Figure4就是作者通过实验得到的表现最好的Normal Cell和Reduction Cell结构。**每个convolution cell（比如Fiugre4中的Normal Cell和Reduction Cell）都是B个block的结果，在Figure4中，B=5，所以可以看到不管在Normal Cell还是Reduction Cell中都有5个add操作**。注意Figure 4展示的是NASNet-A的Cell结构，**其他两种基于不同的search spaces得到的NASNet-B和NASNet-C的Cell结构可以看论文中最后的附录部分**。可以看出虽然作者没有设计类似ResNet那样的residual connection结构（或者叫skip connection），但是这些Cell在训练过程中自己学会了这种skip connection（Figure4中的虚线连接），这个学习的过程就是FIgure3中前面两个灰色矩形框的select过程（可以和ICLR2017的NAS中设计的skip connection结构对比，在那篇文章中采用的sigmoid结构表达层与层之间的连接关系）。而且作者发现当手动插入residual connection时并没能提高模型的效果。

![这里写图片描述](https://img-blog.csdn.net/20171113223421000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验结果：**

首先Table1是在CIFAR-10数据集上的结果。总共4个部分，前面两个部分都是常规的网络，第三部分是原来NAS网络的结果，第四部分就是这批文章的NASNet结果。可以看出NASNet的特点在于在参数数量较少的情况还还能保持较高的准确率。

![这里写图片描述](https://img-blog.csdn.net/20171113223440470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其实网络结构的学习都是在CIFAR-10数据集上训练得到的，最后才移植到ImageNet数据集。**这里要注意的是在移植过程中仅仅是将训练得到的网络结构移植，参数还是在ImageNet数据集上从头开始训练**。这是文章中的原文：We emphasize that we merely transfer the architectures from CIFAR-10 but train all ImageNet models weights from scratch.

Table2是在ImageNet数据集上的结果。可以看出在CIFAR-10数据集上学到的Cell结构可以很好地泛化到ImageNet数据集。

![这里写图片描述](https://img-blog.csdn.net/20171113223532879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Figure5也是在ImageNet数据集上的结果对比。左边是准确率和浮点计算的关系，右边是准确率和参数数量的关系，可以看出NASNet几乎全面占据优势。作者尤其强调在和MobileNet、ShuffleNet的对比中，在相同的计算资源限制下，NASNet的效果更好。

![这里写图片描述](https://img-blog.csdn.net/20171113223551879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验中除了验证该网络在分类任务中的效果，还验证了该网络用于检测算法的效果**。 

Table4就是在COCO数据集上的检测效果对比。这里将在ImageNet数据集上预训练的Table4中的model作为检测算法Faster-RCNN的主网络进行实验。

![这里写图片描述](https://img-blog.csdn.net/20171113223619389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后是关于网络搜索算法的效率。我们知道，文章中的网络搜索算法采用的是强化学习（reinforcement learning）的算法。Figure7是作者画的关于用reinforcement learning（RL）和random search（RS）的效率对比。

![这里写图片描述](https://img-blog.csdn.net/20171113223637639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




