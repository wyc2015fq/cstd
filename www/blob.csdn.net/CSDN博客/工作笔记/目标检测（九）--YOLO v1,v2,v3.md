# 目标检测（九）--YOLO v1,v2,v3 - 工作笔记 - CSDN博客





2017年08月25日 09:44:50[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：17809
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









**YOLO v1**

这是继RCNN，fast-RCNN和faster-RCNN之后，rbg（RossGirshick）针对DL目标检测速度问题提出的另外一种框架。YOLO V1其增强版本GPU中能跑45fps，简化版本155fps。

论文下载：[http://arxiv.org/abs/1506.02640](http://arxiv.org/abs/1506.02640)

代码下载：[https://github.com/pjreddie/darknet](https://github.com/pjreddie/darknet)

1. **YOLO的核心思想**
- 
YOLO的核心思想就是利用整张图作为网络的输入，直接在输出层回归bounding box的位置和bounding box所属的类别。

- 
faster RCNN中也直接用整张图作为输入，但是faster-RCNN整体还是采用了RCNN那种 proposal+classifier的思想，只不过是将提取proposal的步骤放在CNN中实现了,而YOLO则采用直接回归的思路。


2.**YOLO的实现方法**
- 将一幅图像分成SxS个网格(grid cell)，如果某个object的中心 落在这个网格中，则这个网格就负责预测这个object。
![这里写图片描述](https://img-blog.csdn.net/20160317163739691)
- 
每个网格要预测B个bounding box，每个bounding box除了要回归自身的位置之外，还要附带预测一个confidence值。

	这个confidence代表了所预测的box中含有object的置信度和这个box预测的有多准两重信息，其值是这样计算的：
![](https://img-blog.csdn.net/20160317154752063)

	其中如果有object落在一个grid cell里，第一项取1，否则取0。 第二项是预测的bounding box和实际的groundtruth之间的IoU值。

- 
每个bounding box要预测(x, y, w, h)和confidence共5个值，每个网格还要预测一个类别信息，记为C类。则SxS个网格，每个网格要预测B个bounding box还要预测C个categories。输出就是S x S x (5*B+C)的一个tensor。
**注意：class信息是针对每个网格的，confidence信息是针对每个bounding box的。**

- 
**举例说明:** 在PASCAL VOC中，图像输入为448x448，取S=7，B=2，一共有20个类别(C=20)。则输出就是7x7x30的一个tensor。

	整个网络结构如下图所示：
![](https://img-blog.csdn.net/20160317155955917)

- 
在test的时候，每个网格预测的class信息和bounding box预测的confidence信息相乘，就得到每个bounding box的class-specific confidence score:
![这里写图片描述](https://img-blog.csdn.net/20160317161412042)

	等式左边第一项就是每个网格预测的类别信息，第二三项就是每个bounding box预测的confidence。这个乘积即encode了预测的box属于某一类的概率，也有该box准确度的信息。

- 
得到每个box的class-specific confidence score以后，设置阈值，滤掉得分低的boxes，对保留的boxes进行NMS处理，就得到最终的检测结果。

- 
*注：*

**由于输出层为全连接层，因此在检测时，YOLO训练模型只支持与训练图像相同的输入分辨率。*

**虽然每个格子可以预测B个bounding box，但是最终只选择只选择IOU最高的bounding box作为物体检测输出，即每个格子最多只预测出一个物体。当物体占画面比例较小，如图像中包含畜群或鸟群时，每个格子包含多个物体，但却只能检测出其中一个。这是YOLO方法的一个缺陷。*


3.**YOLO的实现细节**
- 
每个grid有30维，这30维中，8维是回归box的坐标，2维是box的confidence，还有20维是类别。

	其中坐标的x,y用对应网格的offset归一化到0-1之间，w,h用图像的width和height归一化到0-1之间。

- 
在实现中，最主要的就是怎么设计损失函数，让这个三个方面得到很好的平衡。作者简单粗暴的全部采用了**sum-squared error loss**来做这件事。
**这种做法存在以下几个问题：**

	第一，8维的localization error和20维的classification error同等重要显然是不合理的；

	第二，如果一个网格中没有object（一幅图中这种网格很多），那么就会将这些网格中的box的confidence push到0，相比于较少的有object的网格，这种做法是overpowering的，这会导致网络不稳定甚至发散。
**解决办法：**
- 更重视8维的坐标预测，给这些损失前面赋予更大的loss weight, 记为![](https://img-blog.csdn.net/20160317162205978)在pascal VOC训练中取5。
- 对没有object的box的confidence loss，赋予小的loss weight，记为![](https://img-blog.csdn.net/20160317162453951)在pascal VOC训练中取0.5。
- 有object的box的confidence loss和类别的loss的loss weight正常取1。

- 
对不同大小的box预测中，相比于大box预测偏一点，小box预测偏一点肯定更不能被忍受的。而sum-square error loss中对同样的偏移loss是一样。

	为了缓和这个问题，作者用了一个比较取巧的办法，就是将box的width和height取平方根代替原本的height和width。这个参考下面的图很容易理解，小box的横轴值较小，发生偏移时，反应到y轴上相比大box要大。（也是个近似逼近方式）
![](https://img-blog.csdn.net/20160317163247283)

- 
一个网格预测多个box，希望的是每个box predictor专门负责预测某个object。具体做法就是看当前预测的box与ground truth box中哪个IoU大，就负责哪个。这种做法称作box predictor的specialization。

- 最后整个的损失函数如下所示：
![这里写图片描述](https://img-blog.csdn.net/20160317163417800)

	这个损失函数中：
	- 只有当某个网格中有object的时候才对classification error进行惩罚。
- 只有当某个box predictor对某个ground truth box负责的时候，才会对box的coordinate error进行惩罚，而对哪个ground truth box负责就看其预测值和ground truth box的IoU是不是在那个cell的所有box中最大。

- 其他细节，例如使用激活函数使用leak RELU，模型用ImageNet预训练等等，在这里就不一一赘述了。
- 
*注：*

**YOLO方法模型训练依赖于物体识别标注数据，因此，对于非常规的物体形状或比例，YOLO的检测效果并不理想。*

**YOLO采用了多个下采样层，网络学到的物体特征并不精细，因此也会影响检测效果。*

** YOLO loss函数中，大物体IOU误差和小物体IOU误差对网络训练中loss贡献值接近（虽然采用求平方根方式，但没有根本解决问题）。因此，对于小物体，小的IOU误差也会对网络优化过程造成很大的影响，从而降低了物体检测的定位准确性。*


4.**YOLO的缺点**
- 
YOLO对相互靠的很近的物体，还有很小的群体 检测效果不好，这是因为一个网格中只预测了两个框，并且只属于一类。

- 
同一类物体出现的新的不常见的长宽比和其他情况时，泛化能力偏弱。

- 
由于损失函数的问题，定位误差是影响检测效果的主要原因。尤其是大小物体的处理上，还有待加强。


## YOLO9000 YOLOv2

原文下载：[https://arxiv.org/pdf/1612.08242v1.pdf](https://arxiv.org/pdf/1612.08242v1.pdf)

工程代码：[http://pjreddie.com/darknet/yolo/](http://pjreddie.com/darknet/yolo/)

## **摘要**
- 提出YOLOv2：代表着目前业界最先进物体检测的水平，它的速度要快过其他检测系统（FasterR-CNN，ResNet，SSD），使用者可以在它的速度与精确度之间进行权衡。
- 提出YOLO9000：这一网络结构可以实时地检测超过9000种物体分类，这归功于它使用了WordTree，通过WordTree来混合检测数据集与识别数据集之中的数据。
- 提出了一种新的联合训练算法（ Joint Training Algorithm ），使用这种联合训练技术同时在ImageNet和COCO数据集上进行训练。YOLO9000进一步缩小了监测数据集与识别数据集之间的代沟。

## **简介**

目前的检测数据集（DetectionDatasets）有很多限制，分类标签的信息太少，图片的数量小于分类数据集（Classiﬁcation Datasets），而且检测数据集的成本太高，使其无法当作分类数据集进行使用。而现在的分类数据集却有着大量的图片和十分丰富分类信息。

文章提出了一种新的训练方法–联合训练算法，这种算法可以把这两种的数据集混合到一起。使用一种分层的观点对物体进行分类，用巨量的分类数据集数据来扩充检测数据集，从而把两种不同的数据集混合起来。

联合训练算法的基本思路就是：同时在检测数据集和分类数据集上训练物体检测器（Object Detectors ），用监测数据集的数据学习物体的准确位置，用分类数据集的数据来增加分类的类别量、提升健壮性。

YOLO9000就是使用联合训练算法训练出来的，他拥有9000类的分类信息，这些分类信息学习自ImageNet分类数据集，而物体位置检测则学习自COCO检测数据集。

*All of our code and pre-trained models are available online at*[http://pjreddie.com/yolo9000/](http://pjreddie.com/yolo9000/)

## **BETTER**

YOLO一代有很多缺点，作者希望改进的方向是:改善recall，提升定位的准确度，同时保持分类的准确度。

目前计算机视觉的趋势是更大更深的网络，更好的性能表现通常依赖于训练更大的网络或者把多种model综合到一起。但是YOLO v2则着力于简化网络。具体的改进见下表：
![这里写图片描述](https://img-blog.csdn.net/20161228160531638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
**Batch Normalization**

使用Batch Normalization对网络进行优化，让网络提高了收敛性，同时还消除了对其他形式的正则化（regularization）的依赖。通过对YOLO的每一个卷积层增加Batch Normalization，最终使得mAP提高了2%，同时还使model正则化。使用Batch Normalization可以从model中去掉Dropout，而不会产生过拟合。

- 
**High resolution classifier**

目前业界标准的检测方法，都要先把分类器（classiﬁer）放在ImageNet上进行预训练。从Alexnet开始，大多数的分类器都运行在小于256*256的图片上。而现在YOLO从224*224增加到了448*448，这就意味着网络需要适应新的输入分辨率。

	为了适应新的分辨率，YOLO v2的分类网络以448*448的分辨率先在ImageNet上进行Fine Tune，Fine Tune10个epochs，让网络有时间调整他的滤波器（filters），好让其能更好的运行在新分辨率上，还需要调优用于检测的Resulting Network。最终通过使用高分辨率，mAP提升了4%。

- 
**Convolution with anchor boxes**

YOLO一代包含有全连接层，从而能直接预测Bounding Boxes的坐标值。 Faster R-CNN的方法只用卷积层与Region Proposal Network来预测Anchor Box的偏移值与置信度，而不是直接预测坐标值。作者发现通过预测偏移量而不是坐标值能够简化问题，让神经网络学习起来更容易。

	所以最终YOLO去掉了全连接层，使用Anchor Boxes来预测 Bounding Boxes。作者去掉了网络中一个Pooling层，这让卷积层的输出能有更高的分辨率。收缩网络让其运行在416*416而不是448*448。由于图片中的物体都倾向于出现在图片的中心位置，特别是那种比较大的物体，所以有一个单独位于物体中心的位置用于预测这些物体。YOLO的卷积层采用32这个值来下采样图片，所以通过选择416*416用作输入尺寸最终能输出一个13*13的Feature Map。 使用Anchor Box会让精确度稍微下降，但用了它能让YOLO能预测出大于一千个框，同时recall达到88%，mAP达到69.2%。

- 
**Dimension clusters**

之前Anchor Box的尺寸是手动选择的，所以尺寸还有优化的余地。 为了优化，在训练集（training set）Bounding Boxes上跑了一下k-means聚类，来找到一个比较好的值。

	如果我们用标准的欧式距离的k-means，尺寸大的框比小框产生更多的错误。因为我们的目的是提高IOU分数，这依赖于Box的大小，所以距离度量的使用：
![这里写图片描述](https://img-blog.csdn.net/20161228165353892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过分析实验结果（Figure 2），左图：在model复杂性与high recall之间权衡之后，选择聚类分类数K=5。右图：是聚类的中心，大多数是高瘦的Box。

	Table1是说明用K-means选择Anchor Boxes时，当Cluster IOU选择值为5时，AVG IOU的值是61，这个值要比不用聚类的方法的60.9要高。选择值为9的时候，AVG IOU更有显著提高。总之就是说明用聚类的方法是有效果的。


![这里写图片描述](https://img-blog.csdn.net/20161228203435148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
**Direct location prediction**

用Anchor Box的方法，会让model变得不稳定，尤其是在最开始的几次迭代的时候。大多数不稳定因素产生自预测Box的（x,y）位置的时候。按照之前YOLO的方法，网络不会预测偏移量，而是根据YOLO中的网格单元的位置来预测坐标，这就让Ground Truth的值介于0到1之间。而为了让网络的结果能落在这一范围内，网络使用一个 Logistic Activation来对于网络预测结果进行限制，让结果介于0到1之间。 网络在每一个网格单元中预测出5个Bounding Boxes，每个Bounding Boxes有五个坐标值tx，ty，tw，th，t0，他们的关系见下图（Figure3）。假设一个网格单元对于图片左上角的偏移量是cx，cy，Bounding Boxes Prior的宽度和高度是pw，ph，那么预测的结果见下图右面的公式：
![这里写图片描述](https://img-blog.csdn.net/20161228203420288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为使用了限制让数值变得参数化，也让网络更容易学习、更稳定。

	Dimension clusters和Direct location prediction，improves YOLO by almost 5% over the version with anchor boxes.

- 
**Fine-Grained Features**

YOLO修改后的Feature Map大小为13*13，这个尺寸对检测图片中尺寸大物体来说足够了，同时使用这种细粒度的特征对定位小物体的位置可能也有好处。Faster R-CNN、SSD都使用不同尺寸的Feature Map来取得不同范围的分辨率，而YOLO采取了不同的方法，YOLO加上了一个Passthrough Layer来取得之前的某个26*26分辨率的层的特征。这个Passthrough layer能够把高分辨率特征与低分辨率特征联系在一起，联系起来的方法是把相邻的特征堆积在不同的Channel之中，这一方法类似与Resnet的Identity Mapping，从而把26*26*512变成13*13*2048。YOLO中的检测器位于扩展后（expanded ）的Feature Map的上方，所以他能取得细粒度的特征信息，这提升了YOLO 1%的性能。

- 
**Multi-ScaleTraining**

作者希望YOLO v2能健壮的运行于不同尺寸的图片之上，所以把这一想法用于训练model中。

	区别于之前的补全图片的尺寸的方法，YOLO v2每迭代几次都会改变网络参数。每10个Batch，网络会随机地选择一个新的图片尺寸，由于使用了下采样参数是32，所以不同的尺寸大小也选择为32的倍数{320，352…..608}，最小320*320，最大608*608，网络会自动改变尺寸，并继续训练的过程。

	这一政策让网络在不同的输入尺寸上都能达到一个很好的预测效果，同一网络能在不同分辨率上进行检测。当输入图片尺寸比较小的时候跑的比较快，输入图片尺寸比较大的时候精度高，所以你可以在YOLO v2的速度和精度上进行权衡。

	Figure4，Table 3：在voc2007上的速度与精度

![这里写图片描述](https://img-blog.csdn.net/20161228203349647?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- 
**Further Experiments**

![这里写图片描述](https://img-blog.csdn.net/20161228202959505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20161228203009328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## **Faster**

YOLO使用的是GoogleLeNet，比VGG-16快，YOLO完成一次前向过程只用8.52 billion 运算，而VGG-16要30.69billion，但是YOLO精度稍低于VGG-16。
- 
**Draknet19**

YOLO v2基于一个新的分类model，有点类似与VGG。YOLO v2使用3*3filter，每次Pooling之后都增加一倍Channels的数量。YOLO v2使用全局平均Pooling，使用Batch Normilazation来让训练更稳定，加速收敛，使model规范化。

	最终的model–Darknet19，有19个卷积层和5个maxpooling层，处理一张图片只需要5.58 billion次运算，在ImageNet上达到72.9%top-1精确度，91.2%top-5精确度。

- 
**Training for classiﬁcation**

网络训练在 ImageNet 1000类分类数据集，训练了160epochs，使用随机梯度下降，初始学习率为0.1， polynomial

	rate decay with a power of 4, weight decay of 0.0005 and momentum of 0.9 。训练期间使用标准的数据扩大方法：随机裁剪、旋转、变换颜色（hue）、变换饱和度（saturation）， 变换曝光度（exposure shifts）。

	在训练时，把整个网络在更大的448*448分辨率上Fine Turnning 10个 epoches，初始学习率设置为0.001，这种网络达到达到76.5%top-1精确度，93.3%top-5精确度。

- 
**Training for detection**

网络去掉了最后一个卷积层，而加上了三个3*3卷积层，每个卷积层有1024个Filters，每个卷积层紧接着一个1*1卷积层， with

	the number of outputs we need for detection。

	对于VOC数据，网络预测出每个网格单元预测五个Bounding Boxes，每个Bounding Boxes预测5个坐标和20类，所以一共125个Filters，增加了Passthough层来获取前面层的细粒度信息，网络训练了160epoches，初始学习率0.001，dividing it by 10 at 60 and 90 epochs，a weight decay of 0.0005 and momentum of 0.9，数据扩大方法相同，对COCO与VOC数据集的训练对策相同。


## **Stronger**

在训练的过程中，当网络遇到一个来自检测数据集的图片与标记信息，那么就把这些数据用完整的YOLO v2 loss功能反向传播这个图片。当网络遇到一个来自分类数据集的图片和分类标记信息，只用整个结构中分类部分的loss功能反向传播这个图片。

但是检测数据集只有粗粒度的标记信息，像“猫“、“ 狗”之类，而分类数据集的标签信息则更细粒度，更丰富。比如狗这一类就包括”哈士奇“”牛头梗“”金毛狗“等等。所以如果想同时在监测数据集与分类数据集上进行训练，那么就要用一种一致性的方法融合这些标签信息。

再者，用于分类的方法，大多是用softmax layer方法，softmax意味着分类的类别之间要互相独立的。而盲目地混合数据集训练，就会出现比如：检测数据集的分类信息中”狗“这一分类，在分类数据集合中，就会有的不同种类的狗”哈士奇“”牛头梗“”金毛“这些分类，这两种数据集之间的分类信息不相互独立。所以使用一种多标签的model来混合数据集，假设一个图片可以有多个分类信息，并假定分类信息必须是相互独立的规则可以被忽略。
- 
**Hierarchical classification**

WordNet的结构是一个直接图表（directed graph），而不是树型结构。因为语言是复杂的，狗这个词既属于‘犬科’又属于‘家畜’两类，而‘犬科’和‘家畜’两类在wordnet中则是同义词，所以不能用树形结构。

	作者希望根据ImageNet中包含的概念来建立一个分层树，为了建立这个分层树，首先检查ImagenNet中出现的名词，再在WordNet中找到这些名词，再找到这些名词到达他们根节点的路径（在这里设为所有的根节点为实体对象（physical object））。在WordNet中，大多数同义词只有一个路径，所以首先把这条路径中的词全部都加到分层树中。接着迭代地检查剩下的名词，并尽可能少的把他们添加到分层树上，添加的原则是取最短路径加入到树中。

	为了计算某一结点的绝对概率，只需要对这一结点到根节点的整条路径的所有概率进行相乘。所以比如你想知道一个图片是否是Norfolk terrier的概率，则进行如下计算：
![这里写图片描述](https://img-blog.csdn.net/20161228234902143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了验证这一个方法，在WordTree上训练Darknet19的model，使用1000类的ImageNet进行训练，为了建立WordtTree 1K，把所有中间词汇加入到WordTree上，把标签空间从1000扩大到了1369。在训练过程中，如果有一个图片的标签是”Norfolk terrier“，那么这个图片还会获得”狗“（dog）以及“哺乳动物”（mammal）等标签。总之现在一张图片是多标记的，标记之间不需要相互独立。

	如Figure5所示，之前的ImageNet分类是使用一个大softmax进行分类。而现在，WordTree只需要对同一概念下的同义词进行softmax分类。

	使用相同的训练参数，这种分层结构的Darknet19达到71.9%top-1精度和90.4%top-5精确度，精度只有微小的下降。

	这种方法的好处：在对未知或者新的物体进行分类时，性能降低的很优雅（gracefully）。比如看到一个狗的照片，但不知道是哪种种类的狗，那么就高置信度（confidence）预测是”狗“，而其他狗的种类的同义词如”哈士奇“”牛头梗“”金毛“等这些则低置信度。
![这里写图片描述](https://img-blog.csdn.net/20161229000453197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

- 
**Datasets combination with wordtree**

用WordTree 把数据集合中的类别映射到分层树中的同义词上，例如上图Figure 6，WordTree混合ImageNet与COCO。

- 
**Joint classification and detection**

作者的目的是：训练一个Extremely Large Scale检测器。所以训练的时候使用WordTree混合了COCO检测数据集与ImageNet中的Top9000类，混合后的数据集对应的WordTree有9418个类。另一方面，由于ImageNet数据集太大了，作者为了平衡一下两个数据集之间的数据量，通过过采样（oversampling）COCO数据集中的数据，使COCO数据集与ImageNet数据集之间的数据量比例达到1：4。

	YOLO9000的训练基于YOLO v2的构架，但是使用3priors而不是5来限制输出的大小。当网络遇到检测数据集中的图片时则正常地反方向传播，当遇到分类数据集图片的时候，只使用分类的loss功能进行反向传播。同时作者假设IOU最少为 .3。最后根据这些假设进行反向传播。

使用联合训练法，YOLO9000使用COCO检测数据集学习检测图片中的物体的位置，使用ImageNet分类数据集学习如何从大量的类别中进行分类。

	为了评估这一方法，使用ImageNet Detection Task对训练结果进行评估。

	评估结果：

	YOLO9000取得19.7mAP。

	在未学习过的156个分类数据上进行测试，mAP达到16.0。

	YOLO9000的mAP比DPM高，而且YOLO有更多先进的特征，YOLO9000是用部分监督的方式在不同训练集上进行训练，同时还能检测9000个物体类别，并保证实时运行。


虽然YOLO9000对动物的识别性能很好，但是对类别为”sungalsses“或者”swimming trunks“这些衣服或者装备的类别，它的识别性能不是很好，见table 7。这跟数据集的数据组成有很大关系。
![这里写图片描述](https://img-blog.csdn.net/20161229104057846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHlzdGVyaWMzMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **总结**

YOLO v2 代表着目前最先进物体检测的水平，在多种监测数据集中都要快过其他检测系统，并可以在速度与精确度上进行权衡。

YOLO 9000 的网络结构允许实时地检测超过9000种物体分类，这归功于它能同时优化检测与分类功能。使用WordTree来混合来自不同的资源的训练数据，并使用联合优化技术同时在ImageNet和COCO数据集上进行训练，YOLO9000进一步缩小了监测数据集与识别数据集之间的大小代沟。

文章还提出了WordTree，数据集混合训练，多尺寸训练等全新的训练方法。

## YOLO v3

YOLOv3在Pascal Titan X上处理608x608图像速度达到20FPS，在 COCO test-dev 上 mAP@0.5 达到 57.9%，与RetinaNet（FocalLoss论文所提出的单阶段网络）的结果相近，并且速度快4倍.

YOLO v3的模型比之前的模型复杂了不少，可以通过改变模型结构的大小来权衡速度与精度。

速度对比如下：

![](https://img-blog.csdn.net/20180408092754890)‘

YOLOv3 在实现相同准确度下要显著地比其它检测方法快。时间都是在采用 M40 或 Titan X 等相同 GPU 下测量的。

简而言之，YOLOv3 的先验检测（Prior detection）系统将分类器或定位器重新用于执行检测任务。他们将模型应用于图像的多个位置和尺度。而那些评分较高的区域就可以视为检测结果。此外，相对于其它目标检测方法，我们使用了完全不同的方法。我们将一个单神经网络应用于整张图像，该网络将图像划分为不同的区域，因而预测每一块区域的边界框和概率，这些边界框会通过预测的概率加权。我们的模型相比于基于分类器的系统有一些优势。它在测试时会查看整个图像，所以它的预测利用了图像中的全局信息。与需要数千张单一目标图像的 R-CNN 不同，它通过单一网络评估进行预测。这令 YOLOv3 非常快，一般它比 R-CNN 快 1000 倍、比 Fast R-CNN 快 100 倍。

**改进之处：**
-     1.多尺度预测 （类FPN）
-     2.更好的基础分类网络（类ResNet）和分类器 darknet-53,见下图。

    3.分类器-类别预测：

        YOLOv3不使用Softmax对每个框进行分类，主要考虑因素有两个：
-             a.Softmax使得每个框分配一个类别（score最大的一个），而对于`Open Images`这种数据集，目标可能有重叠的类别标签，因此Softmax不适用于多标签分类。
-             b.Softmax可被独立的多个logistic分类器替代，且准确率不会下降。

	        分类损失采用binary cross-entropy loss.

**多尺度预测**

    每种尺度预测3个box, anchor的设计方式仍然使用聚类,得到9个聚类中心,将其按照大小均分给3中尺度.
-         尺度1: 在基础网络之后添加一些卷积层再输出box信息.
-         尺度2: 从尺度1中的倒数第二层的卷积层上采样(x2)再与最后一个16x16大小的特征图相加,再次通过多个卷积后输出box信息.相比尺度1变大两倍.
-         尺度3: 与尺度2类似,使用了32x32大小的特征图.

参见网络结构定义文件[yolov3.cfg](https://github.com/pjreddie/darknet/blob/master/cfg/yolov3.cfg)

基础网络 Darknet-53



![](https://img-blog.csdn.net/20180408093509828)

darknet-53

仿ResNet, 与ResNet-101或ResNet-152准确率接近,但速度更快.对比如下:

![](https://img-blog.csdn.net/20180407150541880)

主干架构的性能对比



检测结构如下：

![](https://img-blog.csdn.net/20180407150550783)

![](https://img-blog.csdn.net/20180408093654127)

YOLOv3在mAP@0.5及小目标APs上具有不错的结果,但随着IOU的增大,性能下降,说明YOLOv3不能很好地与ground truth切合.

边框预测

![](https://img-blog.csdn.net/20180408093359734)

图 2：带有维度先验和定位预测的边界框。我们边界框的宽和高以作为离聚类中心的位移，并使用 Sigmoid 函数预测边界框相对于滤波器应用位置的中心坐标。

仍采用之前的logis，其中cx,cy是网格的坐标偏移量,pw,ph是预设的anchor box的边长.最终得到的边框坐标值是b*,而网络学习目标是t*，用sigmod函数、指数转换。

优点
- 
    快速,pipline简单.

- 
    背景误检率低。

- 
    通用性强。YOLO对于艺术类作品中的物体检测同样适用。它对非自然图像物体的检测率远远高于DPM和RCNN系列检测方法。


但相比RCNN系列物体检测方法，YOLO具有以下缺点：
- 
    识别物体位置精准性差。

- 
    召回率低。在每个网格中预测两个box这种约束方式减少了对同一目标的多次检测(R-CNN使用的region proposal方式重叠较多),相比R-CNN使用Selective Search产生2000个proposal（RCNN测试时每张超过40秒）,yolo仅使用7x7x2个.


### YOLO v.s Faster R-CNN
- 统一网络:

	YOLO没有显示求取region proposal的过程。Faster R-CNN中尽管RPN与fast rcnn共享卷积层，但是在模型训练过程中，需要反复训练RPN网络和fast rcnn网络.

	相对于R-CNN系列的"看两眼"(候选框提取与分类，图示如下),YOLO只需要Look Once.
![R-CNN pipline](https://images2018.cnblogs.com/blog/606386/201803/606386-20180324181429804-1383715883.png)
- YOLO统一为一个回归问题，而R-CNN将检测结果分为两部分求解：物体类别（分类问题），物体位置即bounding box（回归问题）。

参考
- YOLO主页 [https://pjreddie.com/darknet/yolo/](https://pjreddie.com/darknet/yolo/)
- [YOLOv3: An Incremental Improvement](https://pjreddie.com/media/files/papers/YOLOv3.pdf)
- [YOLO9000: Better, Faster, Stronger](https://arxiv.org/abs/1612.08242)
- [You Only Look Once: Unified, Real-Time Object Detection](http://arxiv.org/abs/1506.02640)





