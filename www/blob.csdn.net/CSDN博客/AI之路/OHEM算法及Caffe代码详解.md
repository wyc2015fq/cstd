# OHEM算法及Caffe代码详解 - AI之路 - CSDN博客





2017年06月12日 23:25:27[AI之路](https://me.csdn.net/u014380165)阅读数：8129








**这是CVPR2016的一篇论文，用于目标检测，本篇博文先介绍这个算法，然后介绍其Caffe代码。**

论文：Training Region-based Object Detectors with Online Hard Example Mining 

论文链接：[https://arxiv.org/abs/1604.03540](https://arxiv.org/abs/1604.03540)

## **算法概述：**

OHEM（online hard example miniing）算法的核心思想是根据输入样本的损失进行筛选，筛选出hard example，表示对分类和检测影响较大的样本，然后将筛选得到的这些样本应用在随机梯度下降中训练。**在实际操作中是将原来的一个ROI Network扩充为两个ROI Network，这两个ROI Network共享参数。其中前面一个ROI Network只有前向操作，主要用于计算损失；后面一个ROI Network包括前向和后向操作，以hard example作为输入，计算损失并回传梯度。**作者将该算法应用在Fast RCNN中，网络结构还是采用VGG16和VGG_CNN_M_1024，数据集主要采用VOC2007，VOC2012和COCO数据集。 
**算法优点**：1、对于数据的类别不平衡问题不需要采用设置正负样本比例的方式来解决，这种在线选择方式针对性更强。2、随着数据集的增大，算法的提升更加明显（作者是通过在COCO数据集上做实验和VOC数据集做对比，因为前者的数据集更大，而且提升更明显，所以有这个结论）。

算法的测试结果：在pascal VOC2007上的mAP为78.9%，在pascal VOC2012上的mAP为76.3%。注意，这些结果的得到包含了一些小tricks，比如multi-scale test（测试时候采用多尺度输入），bounding box的不断迭代回归。

代码的github地址：[https://github.com/abhi2610/ohem](https://github.com/abhi2610/ohem)

## **算法详解：**

如前所述，OHEM算法的核心是选择一些hard example作为训练的样本，那么什么样的样本是hard example呢？答案是：有多样性和高损失的样本。

在论文中作者主要是将OHEM算法用在Fast RCNN结构中。因此可以先回顾下Fast RCNN的结构。 

Fast RCNN的结构图如下，主要包含两个部分：1、卷积网络。主要由多个卷积层和max pooling层组成；2、ROI pooling网络。主要包括ROI pooling层，一些全连接层和两个损失层。

![这里写图片描述](https://img-blog.csdn.net/20170612231445862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

hard example是根据每个ROI的损失来选择的，选择损失最大的一些ROI。**但是这里有一个问题：重合率比较大的ROI之间的损失也比较相似。因此这里作者采用NMS（non-maximum suppresison）去除重合率较大的ROI，这里作者给的阈值是当IOU大于0.7就认为重合率较高，需去除。**
**注意，这里作者没有采用设定背景和目标样本数的比例方式处理数据的类别不平衡问题。因为如果哪个类别不平衡，那么这个类别的损失就会比较大，这样被采样的可能性也比较大。**

那么具体要怎么把OHEM放在Fast RCNN网络结构中呢？可能首先会想到的做法是修改损失层，将没选择的ROI的loss设置为0。但是这种做法并不高效，因为即便很多ROI的loss都是0，也就是不需要更新梯度，但是这样仍需要给每个ROI都分配存储空间，并且每个ROI都需要后向传播。

因此就有了作者将OHEM应用在Fast RCNN的网络结构，如下图。这里包含两个ROI network，上面一个ROI network是只读的，为所有的ROI在前向传递的时候分配空间。下面一个ROI network则同时为前向和后向分配空间。 

首先，ROI经过ROI plooling层生成feature map，然后进入只读的ROI network得到所有ROI的loss；然后是hard ROI sampler结构根据损失排序选出hard example，并把这些hard example作为下面那个ROI network的输入。

![这里写图片描述](https://img-blog.csdn.net/20170612231556549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实际训练的时候，每个mini-batch包含N个图像，共|R|个ROI，也就是每张图像包含|R|/N个ROI**。经过hard ROI sampler筛选后得到B个hard example。作者在文中采用N=2，|R|=4000，B=128。 

另外关于正负样本的选择：当一个ROI和一个ground truth的IOU大于0.5，则为正样本；当一个ROI和所有ground truth的IOU的最大值小于0.5时为负样本。

对上面Fig2中的图例截图放大：

![这里写图片描述](https://img-blog.csdn.net/20170612231629019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **实验结果：**

数据集采用pascal VOC2007，结构包括VGG_CNN_M_1024（VGGM）和VGG16。这里作者的对比了许多因素，比如1,2和3,4是对比是否有hard mining的差异，也就是一个是数据不平衡，一个是采用hard mining控制正负样本的比例；还有减小每个batch中的图像数量的影响等。其中7,8,9,10是采用所有ROI训练的标准Fast RCNN，因为增加了ROI数量的同时也要增大学习率，实验结果也还可以。**综上，实验证明这些的影响都不大，最后作者加上OHEM明显提高了mAP。**

![这里写图片描述](https://img-blog.csdn.net/20170612231655128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后是训练损失之间的对比，采用每个ROI的平均损失作为纵坐标，迭代次数为横坐标。可以看出如果bg_lo为0，也就是正负样本是不均衡的，那么损失是最大的。同时采用OHEM的损失是最小的。右边的图表和左边的图表的主要差别在于N=1，也就是min-batch的图像数量为1，损失上升了不少。

![这里写图片描述](https://img-blog.csdn.net/20170612231722941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在计算时间和存储上是比Fast RCNN要差一些的。

![这里写图片描述](https://img-blog.csdn.net/20170612231739004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后是在VOC 2007数据集上各算法的mAP对比：除了OHEM本身的效果以外，也可以看出增加训练数据的效果以及在训练和测试时采用multi-scale，对bounding box采用multi-stage的回归等的效果。

![这里写图片描述](https://img-blog.csdn.net/20170612231802801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

以及在VOC2012上的对比：

![这里写图片描述](https://img-blog.csdn.net/20170612231824927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

一个更详细的关于multi-scale和iterative bbox regression的对比：可以看出multi-scale不管应用在训练还是测试阶段都有不错的提升效果，这个和很多采用特征融合的方式做目标检测的算法其实本质是一样的，比如SSD算法，FPN算法等等。还有就是Iterative bounding box regression也是非常有效，具体怎么实现还得看代码。

![这里写图片描述](https://img-blog.csdn.net/20170612231859990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在COCO数据集上的对比可以参看论文，这里不再列出。

## 总结：

**总的来讲，OHEM算法通过选择hard example的方式来训练，不仅解决了正负样本类别不均衡问题，同时提高了算法准确率，算是一个不错的trick。可以联系另一篇博文：A-Fast-RCNN算法，也是类似的关于利用hard example来提高mAP，只不过那篇算法是生成hard example，而这篇算法是选择hard example。**

## **Caffe代码：**

## 1、数据层

![这里写图片描述](https://img-blog.csdn.net/20170612231944960?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **2、VGG16的卷积层**

最后一层为conv5_3。

## **3、只读 ROI Network**

包含一个ROI pooling层，两个常规全连接层（fc6，fc7），一个分类得分的全连接层，一个回归的全连接层，一个算分类概率的softmax层。

![这里写图片描述](https://img-blog.csdn.net/20170612232043489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

分类得分的全连接层：**注意这里有个propagate_down参数设为false，也就是不回传梯度，后面的回归同理。**

![这里写图片描述](https://img-blog.csdn.net/20170612232114130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

回归的全连接层：

![这里写图片描述](https://img-blog.csdn.net/20170612232136333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

算分类概率的softmax层：

![这里写图片描述](https://img-blog.csdn.net/20170612232205744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **4、hard ROI mining层**

输入包括：分类概率的输出，回归的输出，最开始数据层的输出（rois，labels，targets和两个weights）

![这里写图片描述](https://img-blog.csdn.net/20170612232228818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **5、另一个ROI Network**

注意这里的ROI pooling层和前面那个只读ROI Network的ROI pooling层的主要区别在于输入变成了rois_hard，即挑选后的hard example。然后依然是两个常规的全连接层（fc6，fc7），一个分类全连接层和一个回归全连接层。

![这里写图片描述](https://img-blog.csdn.net/20170612232305115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

看看ROI Network的分类全连接层：可以和前面只读ROI Network的分类全连接层做对比

![这里写图片描述](https://img-blog.csdn.net/20170612232402053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **6、分类损失层和回归损失层**

（前者用SoftmaxWithLoss，后者用SmoothL1Loss）

![这里写图片描述](https://img-blog.csdn.net/20170612232422513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



