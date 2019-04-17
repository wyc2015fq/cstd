# 不需要预训练模型的检测算法—DSOD - AI之路 - CSDN博客





2017年08月12日 10:35:08[AI之路](https://me.csdn.net/u014380165)阅读数：9596








论文：DSOD： Learning Deeply Supervised Object Detectors from Scratch 

论文链接：[https://arxiv.org/abs/1708.01241](https://arxiv.org/abs/1708.01241)

caffe代码：[https://github.com/szq0214/DSOD](https://github.com/szq0214/DSOD).
这是一篇**ICCV2017**的文章，我觉得非常有意思，**因为DSOD（Deeply Supervised Object Detector）算法并不是在mAP上和其他检测算法做比较，看谁的算法更有效或者速度更快，而是从另一个角度切入说明fine-tune和直接训练检测模型的差异其实是可以减小的，也就是说训练一个检测模型可以不需要大量的数据和预训练好的模型。**这篇文章的核心内容如果用一句话概括那就是：**DSOD可以从0开始训练数据，不需要预训练模型，而且效果可以和fine-tune的模型媲美。**

所以如果看到这里你也和我一样被这个算法所吸引，那么就继续往下看吧。

DSOD是在SSD算法的基础上进行改进的，可以简单理解为**SSD+DenseNet=DSOD**（作者文中也曾尝试从0开始训练region proposal based的检测算法比如Faster RCNN，R-FCN等，发现模型很难收敛；而proposal-free的检测算法比如SSD却可以收敛，虽然效果一般，因此最后采用proposal-free的检测模型SSD。可以看原文的这句话：**Our DSOD300 achieves 77.7% mAP, which is much better than the SSD300S that is trained from scratch using VGG16 (69.6%) without deep supervision. It is also much better than the fine-tuned results by SSD300 (75.8%)**。在实验中作者是在VOC2007 trainval和2012 trainval数据集上训练模型，然后在VOC2007 testset上测试模型的效果。

**为什么要从0开始训练一个检测模型，而不是fine-tune一个预训练的模型呢？**这是本文的出发点，而作者也在文中介绍了三个主要原因：**1、是预训练的模型一般是在分类图像数据集比如Imagenet上训练的，不一定可以迁移到检测模型的数据上（比如医疗图像）；2、预训练的模型，其结构都是固定的，所以如果你要再修改的话比较麻烦；3、预训练的分类网络的训练目标一般和检测目标不一致，因此预训练的模型对于检测算法而言不一定是最优的选择。**第2点我在实际中有体会过，自己修改一些检测模型，新添的层只能随机初始化，这些层和预训练好的层合并在一起的效果并不一定比不添加的好。

**基于上面3个原因，作者就想能不能从0开始训练一个检测模型呢？如果可以的话，要怎么设计网络结构使得检测的效果可以跟在预训练模型上训练检测模型的效果媲美呢？于是就有了本文的算法DSOD。**

Figure1左边的**plain connection**表示SSD算法中的特征融合操作，这里对于300*300的输入图像而言，一共融合了6种不同scale的特征。在每个虚线矩形框内都有一个1*1的卷积和一个3*3的卷积操作，这其实就是一个**bottleneck**，也就是1*1的卷积主要起到降低channel个数从而降低3*3卷积计算量的作用。 

Figure1右边的**dense connection**表示本文引入densenet思想的特征融合操作。dense connection部分左边的虚线矩形框部分和plain connection的右边虚线矩形框部分很像，差别在于channel个数（**dense connection中3*3的channel个数是对应plain connection中3*3的channel个数的一半**），主要是因为在plain connection中，每个bottleneck的输入直接是前一个bottleneck的输出，**但是在dense connection中，每个bottleneck的输入是前面所有bottleneck的输出的concate**。dense connection部分右边的矩形框是down sampling block，包含2*2的max pooling（降采样作用）和一个1*1的卷积（降低channel个数的作用），作者也提到先进行降采样再进行1*1卷积主要可以减少计算量。因此可以看出DSOD是SSD+DenseNet的结果。

![这里写图片描述](https://img-blog.csdn.net/20170812103157273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

DSOD的网络结构图如Table1所示。Dense Block就是DenseNet网络的子模块。**这里的stem block部分由3*3卷积和2*2的max pool层组成，后面作者的实验有证明这个stem block的存在可以提高mAP。**

![这里写图片描述](https://img-blog.csdn.net/20170812103228150?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验结果：**

Table3是在VOC2007 testset上不同参数的实验结果。**第3，4行的对比可以看出bottlenect的channel个数越多，mAP相对越高。第5、6行的对比可以看出growth rate从16变成48，可以提高4.8%的mAP。第6，9行的对比可以看出stem block的存在可以提高2.8%的mAP。**

![这里写图片描述](https://img-blog.csdn.net/20170812103259744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Table4的信息很丰富。前面两个block是在预训练模型上继续训练检测模型，后面3个block是没有预训练模型的，也就是直接开始训练检测模型。**粗略地看第1,2,5三个block的对比，DSOD可以在没有预训练模型的情况下取得和其他检测算法在预训练模型上训练的效果**。第4个block可以看做baseline，因为作者也对比都没有预训练模型的情况下检测算法的优劣，可以看出第5个block比第4个block的提升非常明显。第5个block的前两个DSOD300的对比主要是预测的结构是plain还是dense的，或者简单讲是否包含down-sampling block和concate操作，可以看出dense结构比plain结构在mAP上有提升，但是速度慢了，因为前者包含down-sampling等操作。

![这里写图片描述](https://img-blog.csdn.net/20170812103327007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

更多实验结果可以参看论文。

**总结：**
**DSOD是在SSD算法的基础上引入了Densenet的思想，也就是改造了一些层的输入。文章的核心内容可以用原文的一句话来概括：A better model structure might enable similar or better performance compared with complex models trained from large data.另外通过结合densenet，也使得DSOD的参数数量大大减少。如果专注于特殊图像的检测或者苦于没有预训练模型，可以参考这篇文章的思想，而且已经开源了代码。**




