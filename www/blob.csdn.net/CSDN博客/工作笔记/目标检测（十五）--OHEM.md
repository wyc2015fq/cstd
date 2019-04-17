# 目标检测（十五）--OHEM - 工作笔记 - CSDN博客





2017年09月12日 15:23:25[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7730
个人分类：[目标检测](https://blog.csdn.net/App_12062011/article/category/6269522)

所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

**Training Region-based Object Detectors with Online Hard Example Mining**
- intro: CVPR 2016 Oral. Online hard example mining (OHEM)
- arxiv: [http://arxiv.org/abs/1604.03540](http://arxiv.org/abs/1604.03540)
- paper: [http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Shrivastava_Training_Region-Based_Object_CVPR_2016_paper.pdf](http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Shrivastava_Training_Region-Based_Object_CVPR_2016_paper.pdf)
- github（Official）: [https://github.com/abhi2610/ohem](https://github.com/abhi2610/ohem)
- author page: [http://abhinav-shrivastava.info/](http://abhinav-shrivastava.info/)

**Track and Transfer: Watching Videos to Simulate Strong Human Supervision for Weakly-Supervised Object Detection**
- intro: CVPR 2016
- arxiv: [http://arxiv.org/abs/1604.05766](http://arxiv.org/abs/1604.05766)

**Exploit All the Layers: Fast and Accurate CNN Object Detector with Scale Dependent Pooling and Cascaded Rejection Classifiers**
- intro: scale-dependent pooling (SDP), cascaded rejection clas-sifiers (CRC)
- paper: [http://www-personal.umich.edu/~wgchoi/SDP-CRC_camready.pdf](http://www-personal.umich.edu/~wgchoi/SDP-CRC_camready.pdf)



转自：[http://blog.csdn.net/u012905422/article/details/52760669](http://blog.csdn.net/u012905422/article/details/52760669)

文章提出了一种通过online hard example mining（OHEM）算法训练基于区域的卷积检测算子的高效目标检测算法，能够对简单样本和一些小数量样本进行抑制，使得训练过程更加高效。该方法利用显著的bootstrapping技术（SVM中被普遍利用），对SGD算法进行一定的修改，使得原有的region-based ConvNets的启发式学习和多参数可以被移除，并得到较准确稳定的检测结果。在PASCAL VOC2007和2012中的mAP分别为：78.9%，76.3%。

# Hard example mining：

主要有2种参见Hard example mining算法，优化SVM时候的算法和非SVM时的利用。

在优化SVM中使用Hard example mining时，训练算法主要维持训练SVM和在工作集上收敛的平衡迭代过程，同时在更新过程中去除一些工作集中样本并添加其他特殊的标准。这里的标准即去掉一些很容易区分的样本类，并添加一些用现有的模型不能判断的样本类，进行新的训练。工作集为整个训练集中的一小部分数据。

非SVM中使用时，该Hard example mining算法开始于正样本数据集和随机的负样本数据集，机器学习模型在这些数据集中进行训练使其达到该数据集上收敛，并将其应用到其他未训练的负样本集中，将判断错误的负样本数据（false positives）加入训练集，重新对模型进行训练。这种过程通常只迭代一次，并不获得大量的再训练收敛过程。

# 网络结构框架：

OHEM算法基于Fast R-CNN算法进行改进，作者认为Fast R-CNN算法中创造mini-batch用来进行SGD算法，并不具有高效和最优的状态，而OHEM可以取得lower training loss，和higher mAP。对比下图两种算法Fast R-CNN和OHEM结构：

![](https://img-blog.csdn.net/20161008222116459)



![](https://img-blog.csdn.net/20161008222120444)

对比可以发现，文章提出的OHEM算法里，对于给定图像，经过selective search RoIs，同样计算出卷积特征图。但是在绿色部分的（a）中，一个只读的RoI网络对特征图和所有RoI进行前向传播，然后Hard RoI module利用这些RoI的loss选择B个样本。在红色部分（b）中，这些选择出的样本（hard examples）进入RoI网络，进一步进行前向和后向传播。

# 实验结果：

实验得到了很好的实验结果，这里仅附mAP结果：

![](https://img-blog.csdn.net/20161008222124242)



