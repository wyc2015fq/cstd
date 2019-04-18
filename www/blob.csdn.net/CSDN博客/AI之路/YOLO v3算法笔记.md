# YOLO v3算法笔记 - AI之路 - CSDN博客





2018年05月05日 08:22:51[AI之路](https://me.csdn.net/u014380165)阅读数：39693
个人分类：[计算机视觉																[深度学习																[目标检测-object detection](https://blog.csdn.net/u014380165/article/category/6967389)




论文：YOLOv3: An Incremental Improvement 

论文地址：[https://pjreddie.com/media/files/papers/YOLOv3.pdf](https://pjreddie.com/media/files/papers/YOLOv3.pdf)

YOLO系列的目标检测算法真的非常赞！这篇博客就来介绍YOLO v3算法的内容，因为涉及到v1和v2的一些思想，所以可以先熟悉下：[YOLO v1算法详解](https://blog.csdn.net/u014380165/article/details/72616238)，[YOLO v2算法详解](https://blog.csdn.net/u014380165/article/details/77961414)。

**YOLO算法的基本思想是**：首先通过特征提取网络对输入图像提取特征，得到一定size的feature map，比如13*13，然后将输入图像分成13*13个grid cell，接着如果ground truth中某个object的中心坐标落在哪个grid cell中，那么就由该grid cell来预测该object，因为每个grid cell都会预测固定数量的bounding box（YOLO v1中是2个，YOLO v2中是5个，YOLO v3中是3个，这几个bounding box的初始size是不一样的），那么这几个bounding box中最终是由哪一个来预测该object？答案是：**这几个bounding box中只有和ground truth的IOU最大的bounding box才是用来预测该object的**。可以看出预测得到的输出feature map有两个维度是提取到的特征的维度，比如13*13，还有一个维度（深度）是B*（5+C），注：YOLO v1中是（B*5+C），其中B表示每个grid cell预测的bounding box的数量，比如YOLO v1中是2个，YOLO v2中是5个，YOLO v3中是3个，C表示bounding box的类别数（没有背景类，所以对于VOC数据集是20），5表示4个坐标信息和一个置信度（objectness score）。

算法在速度和精度上的提升可以看Figure1。 
![这里写图片描述](https://img-blog.csdn.net/20180505081955433?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**bounding box的坐标预测方式还是延续了YOLO v2的做法**，简单讲就是下面这个截图的公式，tx、ty、tw、th就是模型的预测输出。cx和cy表示grid cell的坐标，比如某层的feature map大小是13*13，那么grid cell就有13*13个，第0行第1列的grid cell的坐标cx就是0，cy就是1。pw和ph表示预测前bounding box的size。bx、by。bw和bh就是预测得到的bounding box的中心的坐标和size。坐标的损失采用的是平方误差损失。 
![这里写图片描述](https://img-blog.csdn.net/20180505082015852?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**类别预测方面主要是将原来的单标签分类改进为多标签分类**，因此网络结构上就将原来用于单标签多分类的softmax层换成用于多标签多分类的逻辑回归层。首先说明一下为什么要做这样的修改，原来分类网络中的softmax层都是假设一张图像或一个object只属于一个类别，但是在一些复杂场景下，一个object可能属于多个类，比如你的类别中有woman和person这两个类，那么如果一张图像中有一个woman，那么你检测的结果中类别标签就要同时有woman和person两个类，这就是多标签分类，需要用逻辑回归层来对每个类别做二分类。逻辑回归层主要用到sigmoid函数，该函数可以将输入约束在0到1的范围内，因此当一张图像经过特征提取后的某一类输出经过sigmoid函数约束后如果大于0.5，就表示属于该类。

**YOLO v3采用多个scale融合的方式做预测**。原来的YOLO v2有一个层叫：passthrough layer，假设最后提取的feature map的size是13*13，那么这个层的作用就是将前面一层的26*26的feature map和本层的13*13的feature map进行连接，有点像ResNet。当时这么操作也是为了加强YOLO算法对小目标检测的精确度。这个思想在YOLO v3中得到了进一步加强，在YOLO v3中采用类似FPN的upsample和融合做法（最后融合了3个scale，其他两个scale的大小分别是26*26和52*52），在多个scale的feature map上做检测，**对于小目标的检测效果提升还是比较明显的。前面提到过在YOLO v3中每个grid cell预测3个bounding box，看起来比YOLO v2中每个grid cell预测5个bounding box要少，其实不是！因为YOLO v3采用了多个scale的特征融合，所以boundign box的数量要比之前多很多，以输入图像为416*416为例：（13*13+26*26+52*52）*3和13*13*5相比哪个更多应该很清晰了。**

**关于bounding box的初始尺寸还是采用YOLO v2中的k-means聚类的方式来做**，这种先验知识对于bounding box的初始化帮助还是很大的，毕竟过多的bounding box虽然对于效果来说有保障，但是对于算法速度影响还是比较大的。作者在COCO数据集上得到的9种聚类结果：(10*13); (16*30); (33*23); (30*61); (62*45); (59*119); (116*90); (156*198); (373*326)，这应该是按照输入图像的尺寸是416*416计算得到的。

**网络结构（Darknet-53）**一方面基本采用**全卷积**（YOLO v2中采用pooling层做feature map的sample，这里都换成卷积层来做了），另一方面**引入了residual结构**（YOLO v2中还是类似VGG那样直筒型的网络结构，层数太多训起来会有梯度问题，所以Darknet-19也就19层，因此得益于ResNet的residual结构，训深层网络难度大大减小，因此这里可以将网络做到53层，精度提升比较明显）。**Darknet-53只是特征提取层，源码中只使用了pooling层前面的卷积层来提取特征，因此multi-scale的特征融合和预测支路并没有在该网络结构中体现**，具体信息可以看源码：[https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg](https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg)。预测支路采用的也是全卷积的结构，其中最后一个卷积层的卷积核个数是255，是针对COCO数据集的80类：3*(80+4+1)=255，3表示一个grid cell包含3个bounding box，4表示框的4个坐标信息，1表示objectness score。模型训练方面还是采用原来YOLO v2中的multi-scale training。 
![这里写图片描述](https://img-blog.csdn.net/20180505082150441?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是几个网络在ImageNet数据集上的性能和精度对比。可以看出Darknet-53的性能还是非常不错的。 
![这里写图片描述](https://img-blog.csdn.net/20180505082203355?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
YOLO v3的实验结果对比可以看Table3。原来YOLO v2对于小目标的检测效果是比较差的，通过引入多尺度特征融合的方式，可以看出YOLO v3的APS要比YOLO v2的APS高出不少。 
![这里写图片描述](https://img-blog.csdn.net/20180505082220742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后这张图非常有意思，直接用All the other slow ones来代表其他算法，实实在在展现了本篇文章随性的风格。 
![这里写图片描述](https://img-blog.csdn.net/20180505082234378?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后贴一份参考资料：[https://blog.paperspace.com/tag/series-yolo/](https://blog.paperspace.com/tag/series-yolo/)，其中part1是介绍YOLO算法相关的基础知识，part2到part5是介绍如何用PyTorch实现YOLO v3算法，非常推荐。](https://blog.csdn.net/u014380165/article/category/6829229)](https://blog.csdn.net/u014380165/article/category/6829230)





