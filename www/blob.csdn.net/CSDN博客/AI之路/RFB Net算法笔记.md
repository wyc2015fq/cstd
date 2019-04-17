# RFB Net算法笔记 - AI之路 - CSDN博客





2018年08月10日 08:33:35[AI之路](https://me.csdn.net/u014380165)阅读数：6960
个人分类：[深度学习																[计算机视觉																[目标检测-object detection](https://blog.csdn.net/u014380165/article/category/6967389)




论文：Receptive Field Block Net for Accurate and Fast Object Detection 

论文链接：[https://arxiv.org/abs/1711.07767](https://arxiv.org/abs/1711.07767)

代码链接：[https://github.com/ruinmessi/RFBNet](https://github.com/ruinmessi/RFBNet)
**这篇是ECCV2018关于目标检测的文章，提出了RFB Net网络用于目标检测，可以在兼顾速度的同时达到良好的效果。该网络主要在SSD网络中引入Receptive Field Block (RFB) ，引入RFB的出发点通过模拟人类视觉的感受野加强网络的特征提取能力，在结构上RFB借鉴了Inception的思想，主要是在Inception的基础上加入了dilated卷积层（dilated convolution），从而有效增大了感受野（receptive field）。整体上因为是基于SSD网络进行改进，所以检测速度还是比较快，同时精度也有一定的保证。**

**这篇文章最主要的内容是引入了RFB结构，RFB的效果示意图如Figure2所示，其中中间虚线框部分就是RFB结构**。RFB结构主要有两个特点：1、不同尺寸卷积核的卷积层构成的多分枝结构，这部分可以参考Inception结构。在Figure2的RFB结构中也用不同大小的圆形表示不同尺寸卷积核的卷积层。2、引入了dilated卷积层，dilated卷积层之前应用在分割算法Deeplab中，主要作用也是增加感受野，和deformable卷积有异曲同工之处。在Figure2的RFB结构中用不同rate表示dilated卷积层的参数。在RFB结构中最后会将不同尺寸和rate的卷积层输出进行concat，达到融合不同特征的目的。在Figure2的RFB结构中用3种不同大小和颜色的输出叠加来展示。在Figure2的最后一列中将融合后的特征与人类视觉感受野做对比，从图可以看出是非常接近的，这也是这篇文章的出发点，换句话说就是模拟人类视觉的感受野进行RFB结构的设计。 
![这里写图片描述](https://img-blog.csdn.net/20180810083139839?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure4是两种RFB结构示意图。（a）是RFB，整体结构上借鉴了Inception的思想，主要不同点在于引入3个dilated卷积层（比如3*3conv, rate=1），这也是这篇文章增大感受野的主要方式之一。（b）是RFB-s。RFB-s和RFB相比主要有两个改进，一方面用3*3卷积层代替5*5卷积层，另一方面用1*3和3*1卷积层代替3*3卷积层，主要目的应该是为了减少计算量，类似Inception后期版本对Inception结构的改进。 
![这里写图片描述](https://img-blog.csdn.net/20180810083154785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure5是RFB-Net300的整体结构示意图，基本上和SSD类似，和SSD不同的是：1、主干网上用两个RFB结构替换原来新增的两层。2、conv4_3和conv7_fc在接预测层之前分别接RFB-s和RFB结构，这两个结构的示意图如前面Figure4所示。 
![这里写图片描述](https://img-blog.csdn.net/20180810083210775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table1是在PASCAL VOC2007 test-set数据集上的测试结果，训练集基于2007和2012的trainval。RFB Net300在mAP和FPS方面效果都不错。 
![这里写图片描述](https://img-blog.csdn.net/20180810083225601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table4是在COCO test-dev 2015数据集上的测试结果。最后一个RFB Net512-E主要做了2点改变：1、对conv7_fc的输出特征做了上up-sample，然后和conv4_3的输出特征做融合，基于融合后的特征做预测。这种做法其实是借鉴了FPN算法的思想。2、在RFB结构中增加了7*7大小的卷积分支。这两点改进对效果的提升有一定帮助，而且带来的计算量也少。 
![这里写图片描述](https://img-blog.csdn.net/20180810083243433?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Figure6是关于几个算法在COCO test-dev数据集上关于效果和速度的直观对比。 
![这里写图片描述](https://img-blog.csdn.net/20180810083256632?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)](https://blog.csdn.net/u014380165/article/category/6829230)](https://blog.csdn.net/u014380165/article/category/6829229)





