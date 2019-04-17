# SSD（single shot multibox detector）算法及Caffe代码详解 - AI之路 - CSDN博客





2017年05月31日 22:55:49[AI之路](https://me.csdn.net/u014380165)阅读数：71520








**这篇博客主要介绍SSD算法，该算法是最近一年比较优秀的object detection算法，主要特点在于采用了特征融合。**

论文：SSD single shot multibox detector 

论文链接：[https://arxiv.org/abs/1512.02325](https://arxiv.org/abs/1512.02325)

## **算法概述：**

**本文提出的SSD算法是一种直接预测bounding box的坐标和类别的object detection算法，没有生成proposal的过程**。针对不同大小的物体检测，传统的做法是将图像转换成不同的大小，然后分别处理，最后将结果综合起来，而**本文的ssd利用不同卷积层的feature map进行综合也能达到同样的效果**。算法的主网络结构是VGG16，将两个全连接层改成卷积层再增加4个卷积层构造网络结构。对其中5个不同的卷积层的输出分别用两个3*3的卷积核进行卷积，**一个输出分类用的confidence，每个default box生成21个confidence（这是针对VOC数据集包含20个object类别而言的）；一个输出回归用的localization，每个default box生成4个坐标值（x，y，w，h）**。另外这5个卷积层还经过priorBox层生成default box（生成的是坐标）。上面所述的5个卷积层中每一层的default box的数量是给定的。最后将前面三个计算结果分别合并然后传递给loss层。 
**算法的结果：对于300*300的输入，SSD可以在VOC2007 test上有74.3%的mAP，速度是59 FPS(Nvidia Titan X)，对于512*512的输入, SSD可以有76.9%的mAP**。相比之下Faster RCNN是73.2%的mAP和7FPS，YOLO是63.4%的mAP和45FPS。即便对于分辨率较低的输入也能取得较高的准确率。可见作者并非像传统的做法一样以牺牲准确率的方式来提高检测速度。**作者认为自己的算法之所以在速度上有明显的提升，得益于去掉了bounding box proposal以及后续的pixel或feature的resampling步骤。**

code地址：[https://github.com/weiliu89/caffe/tree/ssd](https://github.com/weiliu89/caffe/tree/ssd)

## **算法详解：**

SSD算法在训练的时候只需要一张输入图像及其每个object的ground truth boxes。 

基本的网络结构是基于VGG16，在ImageNet数据集上预训练完以后用两个新的卷积层代替fc6和fc7，另外对pool5也做了一点小改动，还增加了4个卷积层构成本文的网络。VGG的结构如下图所示：

![这里写图片描述](https://img-blog.csdn.net/20170531223500273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**文章的一个核心是作者同时采用lower和upper的feature maps做检测。**如下图Fig1，有8*8和4*4两种大小的feature maps，而feature map cell就是其中的每一个小格。另外有一个概念：**default box，是指在feature map的每个小格(cell)上都有一系列固定大小的box**，如下图有4个（下图中的虚线框，仔细看格子的中间有比格子还小的一个box）。假设每个feature map cell有k个default box，那么对于每个default box都需要预测c个类别score和4个offset，那么如果一个feature map的大小是m*n，也就是有m*n个feature map cell，那么这个feature map就一共有k*m*n个default box，每个default box需要预测4个坐标相关的值和c+1个类别概率（**实际code是分别用不同数量的3*3卷积核对该层feature map进行卷积，比如卷积核数量为（c+1）*k对应confidence输出，表示每个default box的confidence，就是类别；卷积核数量为4*k对应localization输出，表示每个default box的坐标**）。作者的实验也表明default box的shape数量越多，效果越好。 

所以这里用到的default box和Faster RCNN中的anchor很像，在Faster RCNN中anchor只用在最后一个卷积层，但是在本文中，default box是应用在多个不同层的feature map上。

**下图还有一个重要的信息是：在训练阶段，算法在一开始会先将这些default box和ground truth box进行匹配，比如蓝色的两个虚线框和猫的ground truth box匹配上了，一个红色的虚线框和狗的ground truth box匹配上了。所以一个ground truth可能对应多个default box。在预测阶段，直接预测每个default box的偏移以及对每个类别相应的得分，最后通过NMS得到最终的结果。**

Fig1（c）说明对于每个default box，同时预测它的坐标offset和所有类的confidence。

![这里写图片描述](https://img-blog.csdn.net/20170531223622025?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**关于ground truth和default box的匹配细则可以参考下图：**

![这里写图片描述](https://img-blog.csdn.net/20170531223726598?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么default box的scale（大小）和aspect ratio（横纵比）要怎么定呢？假设我们用m个feature maps做预测，那么对于每个featuer map而言其default box的scale是按以下公式计算的： 
![这里写图片描述](https://img-blog.csdn.net/20170531223831240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里smin是0.2，表示最底层的scale是0.2,；smax是0.9，表示最高层的scale是0.9。 

至于aspect ratio，用ar表示为下式：注意这里一共有5种aspect ratio 
![这里写图片描述](https://img-blog.csdn.net/20170531223908725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此每个default box的宽的计算公式为： 
![这里写图片描述](https://img-blog.csdn.net/20170531223919225?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

高的计算公式为：（很容易理解宽和高的乘积是scale的平方） 
![这里写图片描述](https://img-blog.csdn.net/20170531223928678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

另外当aspect ratio为1时，作者还增加一种scale的default box： 
![这里写图片描述](https://img-blog.csdn.net/20170531223938350?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因此，对于每个feature map cell而言，一共有6种default box。 
**可以看出这种default box在不同的feature层有不同的scale，在同一个feature层又有不同的aspect ratio，因此基本上可以覆盖输入图像中的各种形状和大小的object！**

显然，当default box和grount truth匹配上了，那么这个default box就是positive example（正样本），如果匹配不上，就是negative example（负样本），显然这样产生的负样本的数量要远远多于正样本。于是作者将负样本按照confidence loss进行排序，然后选择排名靠前的一些负样本作为训练，使得最后负样本和正样本的比例在3:1左右。

下图是**SSD算法和YOLO算法的结构图对比**。YOLO算法的输入是448*448*3，输出是7*7*30，这7*7个grid cell一共预测98个bounding box。SSD算法是在原来VGG16的后面添加了几个卷积层来预测offset和confidence（相比之下YOLO算法是采用全连接层），算法的输入是300*300*3，采用conv4_3，conv7，conv8_2，conv9_2，conv10_2和conv11_2的输出来预测location和confidence。

![这里写图片描述](https://img-blog.csdn.net/20170531224057923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**详细讲一下SSD的结构，可以参看Caffe代码。SSD的结构为conv1_1，conv1_2，conv2_1，conv2_2，conv3_1，conv3_2，conv3_3，conv4_1，conv4_2，conv4_3，conv5_1，conv5_2，conv5_3（512），fc6：3*3*1024的卷积（原来VGG16中的fc6是全连接层，这里变成卷积层，下面的fc7层同理），fc7:1*1*1024的卷积，conv6_1，conv6_2（对应上图的conv8_2），……，conv9_1，conv9_2，loss。然后针对conv4_3（4），fc7（6），conv6_2（6），conv7_2（6），conv8_2（4），conv9_2（4）的每一个再分别采用两个3*3大小的卷积核进行卷积，这两个卷积核是并列的（括号里的数字代表default box的数量，可以参考Caffe代码，所以上图中SSD结构的倒数第二列的数字8732表示的是所有default box的数量，是这么来的38*38*4+19*19*6+10*10*6+5*5*6+3*3*4+1*1*4=8732），这两个3*3的卷积核一个是用来做localization的（回归用，如果default box是6个，那么就有6*4=24个这样的卷积核，卷积后map的大小和卷积前一样，因为pad=1，下同），另一个是用来做confidence的（分类用，如果default box是6个，VOC的object类别有20个，那么就有6*（20+1）=126个这样的卷积核）。如下图是conv6_2的localizaiton的3*3卷积核操作，卷积核个数是24（6*4=24，由于pad=1，所以卷积结果的map大小不变，下同）：这里的permute层就是交换的作用，比如你卷积后的维度是32*24*19*19，那么经过交换层后就变成32*19*19*24，顺序变了而已。而flatten层的作用就是将32*19*19*24变成32*8664，32是batchsize的大小。**

![这里写图片描述](https://img-blog.csdn.net/20170531224148539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

confidence的3*3卷积核操作如下，注意卷积核个数是126（6*21=126）：

![这里写图片描述](https://img-blog.csdn.net/20170531224218555?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**然后是一个生成default box的操作，根据最小尺寸，最大尺寸以及横纵比来生成，step表示该层的一个像素点相当于最开始输入图像的1/32，简单讲就是感受野，源码里面是通过将原始的input image的大小除以该层feature map的大小来得到的。variance目测是一个尺度变换，本文的四个坐标采用的是中心坐标加上长宽，计算loss的时候可能需要对中心坐标的loss和长宽的loss做一个权衡，所以有了这个variance。如果采用的是box的四大顶点坐标这种方式，默认variance都是0.1，即相互之间没有权重差异。经过上述3个操作后，对这一层feature的处理就结束了。**

![](https://img-blog.csdn.net/20170531224310243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

稍微看下几个层的输出维度，注意看priorbox的维度，以conv8_2_mbox_priorbox为例，是（1,2,144），这个144表示生成的default box的所有坐标，所以和前面回归的坐标个数是一样的：3*3*4*4。2是和variance相关的。

![这里写图片描述](https://img-blog.csdn.net/20170531224336681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对前面所列的5个卷积层输出都执行上述的操作后，就将得到的结果合并：采用Concat，类似googleNet的Inception操作，是通道合并而不是数值相加。

![这里写图片描述](https://img-blog.csdn.net/20170531224453271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是几个通道合并后的维度：

![这里写图片描述](https://img-blog.csdn.net/20170531224618991?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后就是作者自定义的损失函数层，这里的overlap_threshold表示default box和ground truth的重合度超过这个阈值则为正样本：

![这里写图片描述](https://img-blog.csdn.net/20170531224634526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**损失函数方面**：和Faster RCNN的基本一样，由分类和回归两部分组成，可以参考Faster RCNN，这里不细讲。总之，回归部分的loss是希望预测的box和default box的差距尽可能跟ground truth和default box的差距接近，这样预测的box就能尽量和ground truth一样。

![这里写图片描述](https://img-blog.csdn.net/20170531224728103?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里稍微列了下几种object detection算法的default boxes数量以及为什么要有这么多的box：

![这里写图片描述](https://img-blog.csdn.net/20170531224757808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **实验结果：**

**数据集增加对于mAP的提升确实相当明显！**

![这里写图片描述](https://img-blog.csdn.net/20170531224823652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过对比各种设计方法说明增加数据集对mAP的增加是最明显的。

![这里写图片描述](https://img-blog.csdn.net/20170531224841964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在Fast RCNN和Faster RCNN中，数据集增加的办法主要是采用原有的数据及其水平翻转的结果作为训练集。

**这个图说明使用多层特征的有效性。融合不同层的特征是一种重要的方法，在这里主要解决了大小不同的object的检测问题。**

![这里写图片描述](https://img-blog.csdn.net/20170531224918090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过实验对比YOLO和Faster RCNN，说明SSD速度快且准确率更高。

![这里写图片描述](https://img-blog.csdn.net/20170531225045575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **总结：**

**这种算法对于不同横纵比的object的检测都有效，这是因为算法对于每个feature map cell都使用多种横纵比的default boxes，这也是本文算法的核心。另外本文的default box做法是很类似Faster RCNN中的anchor的做法的。最后本文也强调了增加数据集的作用，包括随机裁剪，旋转，对比度调整等等。**
**文中作者提到该算法对于小的object的detection比大的object要差。作者认为原因在于这些小的object在网络的顶层所占的信息量太少，所以增加输入图像的尺寸对于小的object的检测有帮助。另外增加数据集对于小的object的检测也有帮助，原因在于随机裁剪后的图像相当于“放大”原图像，所以这样的裁剪操作不仅增加了图像数量，也放大了图像。**

参考资料： 

1、[https://docs.google.com/presentation/d/1rtfeV_VmdGdZD5ObVVpPDPIODSDxKnFSU0bsN_rgZXc/pub?start=false&loop=false&delayms=3000&slide=id.g179f601b72_0_51](https://docs.google.com/presentation/d/1rtfeV_VmdGdZD5ObVVpPDPIODSDxKnFSU0bsN_rgZXc/pub?start=false&loop=false&delayms=3000&slide=id.g179f601b72_0_51)

2、[http://www.cs.unc.edu/~wliu/papers/ssd_eccv2016_slide.pdf](http://www.cs.unc.edu/~wliu/papers/ssd_eccv2016_slide.pdf)




