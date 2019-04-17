# YOLOv2论文理解 - zj360202的专栏 - CSDN博客





2018年02月11日 16:15:20[zj360202](https://me.csdn.net/zj360202)阅读数：350








# YOLO9000:Better, Faster, Stronger

论文YOLO9000: Better, Faster, Stronger的主要内容有三点： 
1、作者提出了YOLOv2。 
YOLOv2在YOLOv1的基础上，使用新的网络结构（darknet19）和技巧（Batch Normalization、High Resolution Classifier、Convolutional With Anchor Boxes等），提高了检测速度和检测精度。 
2、作者提出了一种联合训练方法，可以同时使用检测数据集和分类数据集来训练检测模型，用分层的观点对物体分类，用检测数据集学习准确预测物体的位置，用分类数据集来增加可识别的类别量，提升鲁棒性。 
3、作者基于YOLOv2提出了YOLO9000，可以实时检测九千多种物体。

我参考论文和代码进行了实验，YOLOv2相对于YOLOv1有很明显的提升，但是YOLO9000使用感受并不好。YOLO9000的主要问题在于很难调整合适的分层阈值，同时很多物体的检测精度过低。

由于工作和学习中对YOLOv2的研究比较多，对YOLO9000的研究并不深入，所以本文仅记录对YOLOv2的理解。

## Better

YOLO相较于其他的state-of-the-art的检测系统有一些缺陷，主要表现在两点： 
1、和Fast R-CNN相比，YOLO会产生较多的bounding boxes的定位错误。 
2、和基于region proposal的检测系统相比，YOLO的Recall较低。

机器视觉的发展有着神经网络越来越大、越来越深的趋势。现在的检测系统，更好的检测性能往往伴随着更大的神经网络或者是多个检测模型的集成。YOLO的目标是高精度实时检测，所以期望在不增大网络、精度不下降的前提下来对定位错误和低Recall进行改善，为此作者尝试了一系列方法（如图所示）。 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_1_2017101709581251)

### Batch Normalization

BN在消除对其他正则项依赖的同时，在帮助模型收敛方面有显著作用。通过对每个卷积层添加BN，YOLO的mAP有了2%的提升。BN对模型的泛化很有帮助，加入BN后，去掉了dropout模型依旧没有过拟合。

### High Resolution Classifier

所有state-of-the-art的检测方法都在ImageNet上对分类器进行了预训练。从AlexNet开始，多数分类器都把输入图像resize到256 * 256以下，这会容易丢失一些小物体的信息。 
YOLOv1先使用224 * 224的分辨率来训练分类网络，在训练检测网络的时候再切换到448 * 448的分辨率，这意味着YOLOv1的卷积层要重新适应新的分辨率同时YOLOv1的网络还要学习检测网络。 
现在，YOLOv2直接使用448 * 448的分辨率来fine tune分类网络，好让网络可以调整filters来适应高分辨率。然后再用这个结果来fine tune检测网络。 
使用高分辨率的分类网络提升了将近4%的mAP。

### Convolutional With Anchor Boxes

YOLOv1使用卷积层之后的全连接层来直接预测bounding boxes的坐标。Faster R-CNN的做法和YOLO不同，使用精心挑选的anchor boxes来预测bounding boxes的坐标。Faster R-CNN的region proposal network（RPN）使用全卷积网络来预测相对anchor boxes的offsets和confidences。因为预测层是个卷积层，RPN在一个特征图上预测所有bounding boxes的offsets。和直接预测坐标相比，预测offsets简化了问题，而且网络更容易学习。 
YOLOv2去掉了全连接层，使用anchor boxes预测bounding boxes。 
YOLOv2一个池化层，使得卷积层的输出有更高的分辨率。 
YOLOv2将输入图像的尺寸从448 * 448缩减到416 * 416，这样特征图的输出就是一个奇数，有一个中心栅格。作者观察到，有很多物体，尤其是较大的物体往往会位于图像的中心。有一个中心栅格的话可以用中心栅格专门去负责预测这些中心落在图像中心附近的物体，而不需要图像中心附近的4个栅格去预测这些物体。YOLOv2对图像进行了32倍的降采样，最终输出的特征图尺寸是13 * 13。

使用anchor boxes预测坐标的同时，YOLOv2还对对conditional class probability的预测机制和空间位置（栅格）做了解耦。 
在YOLOv1将输入图像划分为S*S的栅格，每一个栅格预测B个bounding boxes，以及这些bounding boxes的confidence scores。 
每一个栅格还要预测C个 conditional class probability（条件类别概率）：Pr(Classi|Object)。即在一个栅格包含一个Object的前提下，它属于某个类的概率。且每个栅格预测一组（C个）类概率，而不考虑框B的数量。 
YOLOv2不再由栅格去预测条件类别概率，而由Bounding boxes去预测。在YOLOv1中每个栅格只有1组条件类别概率，而在YOLOv2中，因为每个栅格有B个bounding boxes，所以有B组条件类别概率。 
在YOLOv1中输出的维度为S * S * （B * 5 + C ），而YOLOv2为S * S * （B * （5 + C））。 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_2_20171017095812332)

使用anchor boxes, 模型的的精度有一点点下降，但是Recall有大幅上升。没有anchor box，我们的中间模型的mAP为69.5，Recall为81%。使用anchor boxes 模型的mAP为69.2，Recall为88%。尽管mAP有轻微的下降，但是Recall的增加意味着模型有更多的改进空间。

### Dimension Clusters

尝试在YOLO中使用anchor boxes的过程中遇到了两个问题，第一个问题是如何选择anchor boxes。Faster R-CNN的anchor boxes是手工精心挑选的，但未必是最好的。虽然神经网络在训练过程中会逐渐学着调整预测的bounding boxes，让预测更合理，但有好的anchor boxes帮助可以神经网络让预测更简单。 
机器学习的本质是学习数据中的概率分布，手工挑选的anchor boxes未必很好的符合训练集ROI的概率分布，使用K-means从训练集中聚类得到的anchor boxes可能更好。 
YOLOv2中的anchor boxes是通过k-means在训练集中学得的。值得注意的是，因为使用欧氏距离会让大的bounding boxes比小的bounding boxes产生更多的error，而我们希望能通过anchor boxes获得好的IOU scores，并且IOU scores是与box的尺寸无关的。 
为此作者定义了新的距离公式： 


d(box,centroid)=1−IOU(box,centroid)

作者使用了一系列k的值运行k-means，并画出了平均IOU，如图所示： 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_3_20171017095812613)
和使用手工挑选的anchor boxes相比，使用K-means得到的anchor boxes表现更好。使用5个k-means得到的anchor boxes的性能（IOU 61.0）和使用9个手工挑选的anchor boxes的性能（IOU 60.9）相当。这意味着使用k-means获取anchor boxes来预测bounding boxes让模型更容易学习如何预测bounding boxes。 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_4_20171017095812941)


### Direct location prediction

尝试在YOLO中使用anchor boxes的过程中遇到的第二个问题就是模型变得不稳定，尤其是在训练的早期迭代。不稳定的主要因素来自对bounding box中心坐标x，y的预测。在RPN中，网络预测出偏移量tx 和 ty,计算对bounding box中心坐标x，y的公式为：



x=(tx∗wa)−xa


y=(ty∗ha)−ya

论文中的该公式可能有误，参考Faster R-CNN的论文，公式应为： 

x=(tx∗wa)+xa


y=(ty∗ha)+ya

公式中xa，ya为anchor的坐标，wa,ha为anchor的长宽。如果tx大于0，bounding box会向右移动anchor宽的tx倍；如果tx小于0，bounding box会向左移动anchor宽的tx倍。由于tx和ty没有限制，预测出的bounding box的中心可以在图像上的任意一点，即便这个点落在别的栅格中，应当由别的栅格来预测。 
由于模型是随机初始化的，要花很长时间去训练，参数才能调整到能够很敏感的预测offset。因此YOLOv2不预测offset，而是延续YOLOv1预测bounding boxes相对栅格左上角的坐标（相对栅格边长的比例）。为了确保bounding boxes的中心落在栅格中（即坐标落在0到1之间），作者使用了logistic激活函数来约束预测值。 
YOLOv2网络为每个栅格预测5个bounding boxes（对应5个anchor boxes），每个bounding box预测5个坐标tx,ty,tw,th和to。如果栅格左上角的坐标为（cx,xy），anchor box的宽高为(pw,ph)，则bounding box的中心点坐标(bx,by)，宽高bw,bh和 
置信分的计算公式为： 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_5_2017101709581366)
σ(tx)是bounding box的中心相对栅格左上角的横坐标，σ(ty)是纵坐标，σ(to)是bounding box的confidence score。 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_6_20171017095813223)
约束了位置预测的范围后，网络参数变得更容易学习，网络变得稳定。和使用手选的anchor boxes预测offset相比，使用k-means得到的anchor boxes直接预测相对栅格的坐标，mAP提升了将近5%。


### Fine-Grained Features

YOLOv2在卷积层输出的13 * 13特征图上进行检测，这对于大的物体是足够的，但是更细粒度的特征可以帮助模型定位较小的目标。Faster R-CNN和SSD在一系列尺寸不同的特征图上运行RPN来获取不同分别率的信息。YOLOv2采取不同的方法，通过添加一个passthrough layer，将前一个卷积块26 * 26分辨率的特征图的信息融合起来。 
![image](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_7_20171017095813535)
类似于ResNet的Identity Mapping，passthrough layer层通过将相邻特征堆叠到不同的通道，将较高分辨率特征与低分辨率特征的信息融合。这使得26×26×512的特征图转换为与原始特征相连接的13×13×2048的特征图。YOLOv2的检测器在这个扩展的特征图的顶部运行，以便利用细粒度的特征。这提高了将近1%的性能。

### Multi-Scale Training

YOLOv1网络使用448 * 448的图像作为输入，YOLOv2加入anchor boxes后，输入图像的尺寸变成了416 * 416。YOLOv2网络只用到了卷积层和池化层，因此可以进行动态调整输入图像的尺寸。作者希望YOLOv2对于不同尺寸图像的检测都有较好的鲁棒性，因此做了针对性训练。 
和YOLOv1训练时网络输入的图像尺寸固定不变不同，YOLOv2（在cfg文件中random=1时）每隔几次迭代后就会微调网络的输入尺寸。训练时每迭代10次，就会随机选择新的输入图像尺寸。因为YOLOv2的网络使用的downsamples倍率为32，所以使用32的倍数调整输入图像尺寸{320,352，…，608}。训练使用的最小的图像尺寸为320 * 320，最大的图像尺寸为608 * 608。 
这种策略让YOLOv2网络不得不学着对不同尺寸的图像输入都要预测得很好，这意味着同一个网络可以胜任不同分辨率的检测任务，在网络训练好之后，在使用时只需要根据需求，修改cfg文件中的网络输入图像尺寸（width和height的值）即可。 
YOLOv2在检测速度和精度上达到了很好的平衡。 
YOLOv2在检测低分辨率的图像时，精度较低但速度非常快。在输入尺寸为228 * 228的时候，检测帧率达到90FPS，而mAP几乎和Faster R-CNN的水准相同。使意味着YOLOv2在低性能GPU、高帧率视频、多媒体视频流场景中更加适用。 
在检测大尺寸图像检测中，YOLOv2不仅依然保持着实时检测，精度也很高，如在VOC2007 上mAP为78.6%。和其他检测系统的性能对比见下面几张图：

VOC2007 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_8_20171017095813770)
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_9_20171017095813973)

VOC2012 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_10_20171017095814176)

COCO2015 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_11_20171017095814473)

## Faster

YOLO设计的初衷是兼得速度和精度，达到实时检测。YOLOv2不仅提高了精度，通过设计新的网络结构，检测速度也有提升。

### Darknet-19

多数的检测系统使用VGG-16作为基础特征提取器。VGG-16是一个高精度的有效的分类网络，但是有些过于复杂。VGG-16的卷积层对一张224 * 224 的图像做一次前传计算要做306.9亿次浮点数运算。 
YOLOv1使用的网络是基于GoogleNet的。该网络比VGG-16快，对一张224 * 224 的图像做一次前传计算要做85.2亿次浮点数运算，但是精度比VGG-16略低。VGG-16在ImageNet上的Top-5精度为90.0%，YOLOv1的定制网络精度为88.0%。 
YOLOv2的分类网络叫做Darknet-19，Darknet-19对一张224 * 224 的图像做一次前传计算要做55.8亿次浮点数运算，在ImageNet上的Top-5精度为91.2%，比YOLOv1更快，比VGG-16更高。

Darknet-19的网络结构如图： 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_12_20171017095814754)
类似VGG网络，Darknet-19中使用了较多的3 * 3卷积核，在每一次池化操作后把通道数翻倍。 
Darknet-19也借鉴了Network in Network的思想，使用了全局平均池化（global average pooling），还把1 * 1的卷积核置于3 * 3的卷积核之间，用来压缩特征。 
同时，Darknet-19还用了batch normalization稳定模型训练，加速收敛并泛化模型。

### Training for classification

作者使用Darknet-19在标准的ImageNet1000类分类数据集上训练了160个迭代，用的随机梯度下降算法，初始学习率为0.1，polynomial rate decay 为4，weight decay为0.0005 ，momentum 为0.9。训练时用了很多常见的数据扩充方法（data augmentation），包括random crops, rotations, and hue, saturation, and exposure shifts。 
初始训练时网络的输入是224 * 224，160个迭代后输入的分辨率切换到448 * 448进行fine tune，fine tune时学习率调整为0.001，训练10个迭代。最终分类网络在ImageNet上top-1准确率76.5%，top-5准确率93.3%。

### Training for detection

训练检测网络时去掉了分类网络的网络最后一个卷积层，在后面增加了三个卷积核尺寸为3 * 3，卷积核数量为1024的卷积层，并在这三个卷积层的最后一层后面跟一个卷积核尺寸为1 * 1的卷积层，卷积核数量是（B * （5 + C））。 
对于VOC数据集，卷积层输入图像尺寸为416 * 416时最终输出是13 * 13个栅格，每个栅格预测5种boxes大小，每个box包含5个坐标值和20个条件类别概率，所以输出维度是13 * 13 * 5 * （5+20）= 13 * 13 * 125。

检测网络加入了passthrough layer，从最后一个输出为26 * 26 * 512的卷积层连接到新加入的三个卷积核尺寸为3 * 3的卷积层的第二层，使模型有了细粒度特征。 
从yolo-voc.cfg文件可以看到，第25层为route层，逆向9层拿到第16层26 * 26 * 512的输出，并由第26层的reorg层把26 * 26 * 512 变形为13 * 13 * 2048，再有第27层的route层连接24层和26层的输出，堆叠为13 * 13 * 3072，由最后一个卷积核为3 * 3的卷积层进行跨通道的信息融合并把通道降维为1024。 
如图所示： 
![这里写图片描述](http://image109.360doc.com/DownloadImg/2017/10/1709/113608199_13_20171017095815238)

作者训练检测网络时以0.001的初始学习率训练了160个迭代，在60次和90次迭代的时候，学习率减为原来的十分之一。weight decay为0.0005，momentum为0.9，使用了类似于Faster-RCNN和SSD的数据扩充（data augmentation）方法。

# 参考

[https://zhuanlan.zhihu.com/p/25167153](https://zhuanlan.zhihu.com/p/25167153)
[http://blog.csdn.net/jesse_mx/article/details/53925356](http://blog.csdn.net/jesse_mx/article/details/53925356)
[https://arxiv.org/abs/1612.08242](https://arxiv.org/abs/1612.08242)



