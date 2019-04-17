# IoU-Net 直观深入理解 - Snoopy_Dream - CSDN博客





2019年03月16日 13:18:43[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：79








具体的研究背景可以参看这两篇，我这里只说一下我的直观理解。

这两篇文章前因后果讲的可以，但是总觉得到底实现的过程是怎么样的，以及怎么给人一个直观的流程的感受，没有说到。

推荐大家先看这两篇，然后再看看我的这篇，欢迎大家讨论。

[https://blog.csdn.net/u014380165/article/details/82025507](https://blog.csdn.net/u014380165/article/details/82025507)

[https://www.jiqizhixin.com/articles/2018-08-01-4](https://www.jiqizhixin.com/articles/2018-08-01-4)

![](https://img-blog.csdnimg.cn/20190316124851248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

1.提出了定位置信度，精修回归框。Algorithm2。

2.用定位置信度替换分类置信度进行nms的方法，IoU-Guide NMS。Algorithm1



检测过程

作者文章能体现的改进的NMS是第二次边框回归的时候，这个时候所有的detected boxes已经有了IOU得分，和Classification得分，用这个来进行具体的IoU-Guide NMS。

那第一次边框回归也就是RPN的时候，怎么做的呢？作者文章里面没有解释。下面是我的YY，欢迎大家积极讨论。

先简述一下第一次回归的过程，以一般faster rcnn 为例，这个过程源码在prosal_layer.py中实现，训练是从20000rpn_anchor中选出2000个rpn_rois，检测的时候选2000rpn_anchor然后nms成300rpn_rois。选出来rpn_rois之后对这些进行RPN阶段的边框回归。

我觉得可以RPN的时候加一个全卷积的层，对应输出IoU得分，然后在RPN的时候也能用IoU guide NMS了。





### 定位置信度

这里定位置信度是由上面的模型框架得到的。至于这个standalone IoU-Net，怎么个操作，反向传播，见下图Algorithm2。

简单的说就是添加一条IOU的通道，用来输出每个detected bbox对应IoU，并以此作为定位置信度，参与NMS。

![](https://img-blog.csdnimg.cn/20190316125003207.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

### IoU-Guide NMS

简单的说，就是用定位置信度IOU的分数（下面一段讲解），替代原先的softmax得到的分类分数，用于detected bbox的筛选，以前是找出分类分数最高的，与其他做IOU超过的就去掉，现在是用定位置信度IOU得分最高的，与其他的box求IOU，在这个过程中，如果有遇到分类得分比它自己高的，拿过来，替换（聚类的思想）。

![](https://img-blog.csdnimg.cn/20190316125235988.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)





