# 深入理解目标检测与YOLO（从v1到v3） - Machine Learning with Peppa - CSDN博客





2018年06月14日 16:20:54[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1323










## Introduction



YOLO之前的物体检测方法主要是通过region proposal产生大量的可能包含待检测物体的 potential bounding box，再用分类器去判断每个 bounding box里是否包含有物体，以及物体所属类别的 probability或者 confidence，如R-CNN,Fast-R-CNN,Faster-R-CNN等。一直以来，在计算机视觉领域有一个问题待解决，那就是如何检测两个距离很近的同类的目标或不同类目标？

大多数的算法都会对输入的图像数据进行尺度变化，缩放到较小的分辨率情况下，但是一般在这总情况下只会给出一个Bounding Box，主要由于特征提取过程中将这总情况人为是一个目标。（本来就很近，一放缩之间的近距离越发明显了），但是实际这是两个相同或不同的目标。这个难题就是目标检测领域内的一个挑战。对小目标检测，有很多新的算法，但是YOLO V3版本却做到了，它对这种距离很近的目标或者小目标有很好的鲁棒性，虽然不能百分百检测，但是这个难题得到了很大程度的解决。


YOLO的V1和V2都不如SSD的算法，主要原因是V1的448尺寸和V2版本的416尺寸都不如SSD的300，以上结论都是实验测试的，V3版本的416应该比SSD512好，可见其性能。YOLO不同于这些物体检测方法，它将物体检测任务当做一个regression问题来处理，使用一个神经网络，直接从一整张图像来预测出bounding box 的坐标、box中包含物体的置信度和物体的probabilities。因为YOLO的物体检测流程是在一个神经网络里完成的，所以可以end to end来优化物体检测性能。

YOLO检测物体的速度很快，标准版本的YOLO在Titan X 的 GPU 上能达到45 FPS。网络较小的版本Fast YOLO在保持mAP是之前的其他实时物体检测器的两倍的同时，检测速度可以达到155 FPS。

相较于其他的state-of-the-art 物体检测系统，YOLO在物体定位时更容易出错，但是在背景上预测出不存在的物体（false positives）的情况会少一些。而且，YOLO比DPM、R-CNN等物体检测系统能够学到更加抽象的物体的特征，这使得YOLO可以从真实图像领域迁移到其他领域，如艺术。

## What is Yolo?

YOLO之前的物体检测系统使用分类器来完成物体检测任务。为了检测一个物体，这些物体检测系统要在一张测试图的不同位置和不同尺寸的bounding box上使用该物体的分类器去评估是否有该物体。如DPM系统，要使用一个滑窗（sliding window）在整张图像上均匀滑动，用分类器评估是否有物体。

在DPM之后提出的其他方法，如R-CNN方法使用region proposal来生成整张图像中可能包含待检测物体的potential bounding boxes，然后用分类器来评估这些boxes，接着通过post-processing来改善bounding boxes，消除重复的检测目标，并基于整个场景中的其他物体重新对boxes进行打分。整个流程执行下来很慢，而且因为这些环节都是分开训练的，检测性能很难进行优化。

作者设计了YOLO（you only look once），将物体检测任务当做回归问题（regression problem）来处理，直接通过整张图片的所有像素得到bounding box的坐标、box中包含物体的置信度和class probabilities。通过YOLO，每张图像只需要看一眼就能得出图像中都有哪些物体和这些物体的位置。

![这里写图片描述](https://img-blog.csdn.net/20170420213534638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如图所示，使用YOLO来检测物体，其流程是非常简单明了的： 
1、将图像resize到448 * 448作为神经网络的输入 
2、运行神经网络，得到一些bounding box坐标、box中包含物体的置信度和class probabilities 
3、进行非极大值抑制，筛选Boxes

下图是各物体检测系统的检测流程对比：

![这里写图片描述](https://img-blog.csdn.net/20170420213609657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

YOLO模型相对于之前的物体检测方法有多个优点：

1、YOLO检测物体非常快。 
因为没有复杂的检测流程，只需要将图像输入到神经网络就可以得到检测结果，YOLO可以非常快的完成物体检测任务。标准版本的YOLO在Titan X 的 GPU 上能达到45 FPS。更快的Fast YOLO检测速度可以达到155 FPS。而且，YOLO的mAP是之前其他实时物体检测系统的两倍以上。

2、YOLO可以很好的避免背景错误，产生false positives。 
不像其他物体检测系统使用了滑窗或region proposal，分类器只能得到图像的局部信息。YOLO在训练和测试时都能够看到一整张图像的信息，因此YOLO在检测物体时能很好的利用上下文信息，从而不容易在背景上预测出错误的物体信息。和Fast-R-CNN相比，YOLO的背景错误不到Fast-R-CNN的一半。

3、YOLO可以学到物体的泛化特征。 
当YOLO在自然图像上做训练，在艺术作品上做测试时，YOLO表现的性能比DPM、R-CNN等之前的物体检测系统要好很多。因为YOLO可以学习到高度泛化的特征，从而迁移到其他领域。

尽管YOLO有这些优点，它也有一些缺点：

1、YOLO的物体检测精度低于其他state-of-the-art的物体检测系统。 
2、YOLO容易产生物体的定位错误。 
3、YOLO对小物体的检测效果不好（尤其是密集的小物体，因为一个栅格只能预测2个物体）。

下图是各物体检测系统的检测性能对比： 


![这里写图片描述](https://img-blog.csdn.net/20170420213820985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Unified Detection

YOLO将输入图像划分为S*S的栅格，每个栅格负责检测中心落在该栅格中的物体,如下图所示： 


![这里写图片描述](https://img-blog.csdn.net/20170420213841466?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

每一个栅格预测B个bounding boxes，以及这些bounding boxes的confidence scores。 
这个 confidence scores反映了模型对于这个栅格的预测：该栅格是否含有物体，以及这个box的坐标预测的有多准。 
公式定义如下： 


![这里写图片描述](https://img-blog.csdn.net/20170420213914966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果这个栅格中不存在一个 object，则confidence score应该为0；否则的话，confidence score则为 predicted bounding box与 ground truth box之间的 IOU（intersection over union）。
YOLO对每个bounding box有5个predictions：x, y, w, h, 
and confidence。 
坐标x,y代表了预测的bounding box的中心与栅格边界的相对值。 
坐标w,h代表了预测的bounding box的width、height相对于整幅图像width,height的比例。 
confidence就是预测的bounding box和ground truth box的IOU值。 


![这里写图片描述](https://img-blog.csdn.net/20170420213936232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

每一个栅格还要预测C个 conditional class probability（条件类别概率）：Pr(Classi|Object)。即在一个栅格包含一个Object的前提下，它属于某个类的概率。 
我们只为每个栅格预测一组（C个）类概率，而不考虑框B的数量。



![这里写图片描述](https://img-blog.csdn.net/20170420214004907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**注意： **
conditional class probability信息是针对每个网格的。 confidence信息是针对每个bounding box的。在测试阶段，将每个栅格的conditional class probabilities与每个 bounding box的 confidence相乘：

![这里写图片描述](https://img-blog.csdn.net/20170420214816938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样既可得到每个bounding box的具体类别的confidence score。 这乘积既包含了bounding box中预测的class的 probability信息，也反映了bounding box是否含有Object和bounding box坐标的准确度。

![这里写图片描述](https://img-blog.csdn.net/20170420214036891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

将YOLO用于PASCAL VOC数据集时： 
论文使用的 S=7，即将一张图像分为7×7=49个栅格每一个栅格预测B=2个boxes（每个box有 x,y,w,h,confidence，5个预测值），同时C=20（PASCAL数据集中有20个类别）。 
因此，最后的prediction是7×7×30 { 即S * S * ( B * 5 + C) }的Tensor。



![这里写图片描述](https://img-blog.csdn.net/20170420214103391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170420214125579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170420214145482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### Network Design

YOLO检测网络包括24个卷积层和2个全连接层,如图所示： 


![这里写图片描述](https://img-blog.csdn.net/20170420214206923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170420214223326?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，卷积层用来提取图像特征，全连接层用来预测图像位置和类别概率值。
YOLO网络借鉴了GoogLeNet分类网络结构。不同的是，YOLO未使用inception module，而是使用1x1卷积层（此处1x1卷积层的存在是为了跨通道信息整合）+3x3卷积层简单替代。

## Training

首先利用ImageNet 1000-class的分类任务数据集Pretrain卷积层。使用上述网络中的前20 个卷积层，加上一个 average-pooling layer，最后加一个全连接层，作为 Pretrain 的网络。训练大约一周的时间，使得在ImageNet 2012的验证数据集Top-5的精度达到 88%，这个结果跟 GoogleNet 的效果相当。

将Pretrain的结果的前20层卷积层应用到Detection中，并加入剩下的4个卷积层及2个全连接。 
同时为了获取更精细化的结果，将输入图像的分辨率由 224* 224 提升到 448* 448。 
将所有的预测结果都归一化到 0~1, 使用 Leaky RELU 作为激活函数。 
为了防止过拟合，在第一个全连接层后面接了一个 ratio=0.5 的 Dropout 层。 
为了提高精度，对原始图像做数据提升。

## 损失函数

损失函数的设计目标就是让坐标（x,y,w,h），confidence，classification 这个三个方面达到很好的平衡。 
简单的全部采用了sum-squared error loss来做这件事会有以下不足： 
a) 8维的localization error和20维的classification error同等重要显然是不合理的。 
b) 如果一些栅格中没有object（一幅图中这种栅格很多），那么就会将这些栅格中的bounding box的confidence 置为0，相比于较少的有object的栅格，这些不包含物体的栅格对梯度更新的贡献会远大于包含物体的栅格对梯度更新的贡献，这会导致网络不稳定甚至发散。

![这里写图片描述](https://img-blog.csdn.net/20170420214248108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**解决方案如下： **

更重视8维的坐标预测，给这些损失前面赋予更大的loss weight, 记为 λcoord ,在pascal VOC训练中取5。（上图蓝色框） 
对没有object的bbox的confidence loss，赋予小的loss weight，记为 λnoobj ，在pascal VOC训练中取0.5。（上图橙色框） 
有object的bbox的confidence loss (上图红色框) 和类别的loss （上图紫色框）的loss weight正常取1。

对不同大小的bbox预测中，相比于大bbox预测偏一点，小box预测偏相同的尺寸对IOU的影响更大。而sum-square error loss中对同样的偏移loss是一样。 为了缓和这个问题，作者用了一个巧妙的办法，就是将box的width和height取平方根代替原本的height和width。 如下图：small bbox的横轴值较小，发生偏移时，反应到y轴上的loss（下图绿色）比big box(下图红色)要大。

![这里写图片描述](https://img-blog.csdn.net/20170420214304420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在 YOLO中，每个栅格预测多个bounding box，但在网络模型的训练中，希望每一个物体最后由一个bounding box predictor来负责预测。 因此，当前哪一个predictor预测的bounding box与ground truth box的IOU最大，这个 predictor就负责 predict object。 这会使得每个predictor可以专门的负责特定的物体检测。随着训练的进行，每一个 predictor对特定的物体尺寸、长宽比的物体的类别的预测会越来越好。

## 神经网络输出后的检测流程

![这里写图片描述](https://img-blog.csdn.net/20170420214324204?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 非极大值抑制

![这里写图片描述](https://img-blog.csdn.net/20170420214347813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 获取Object Detect 结果

![这里写图片描述](https://img-blog.csdn.net/20170420214406796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaHJzc3R1ZHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 关于YOLO v3

根据其他博主的测试，有如下对官方YOLO的实验数据。实验中，采用同一个视频、同一张显卡，在阈值为0.3的前提下，对比了V3和V2的测试效果之后，有了下面两个疑问： 
- 
V3和V2的测试性能可以有较大的提升，但速度却没有降低？ 

- 
V3性能上为啥有这么大的改进？对小目标检测变得这么好？   


如果看了V3论文的，应该很清楚结果，如下：

Loss不同：将YOLO V3替换了V2中的Softmax loss变成Logistic loss，而且每个GT只匹配一个先验框； 

Anchor bbox prior不同：V2用了5个anchor，V3用了9个anchor，提高了IOU；

Detection的策略不同：V2只有一个detection，V3设置有3个，分别是一个下采样的，Feature map为13*13，还有2个上采样的eltwise sum，Feature map分别为26*26和52*52，也就是说，V3的416版本已经用到了52的Feature map，而V2把多尺度考虑到训练的data采样上，最后也只是用到了13的Feature map，这应该是对小目标影响最大的地方；

backbone不同：V2的Darknet-19变成了V3的Darknet-53，这与上一个有关。

另外V3还是用了一连串的3*3、1*1卷积，其中，3*3的卷积增加channel，而1*1的卷积在于压缩3*3卷积后的特征表示，这波操作很具有实用性。 

V2日志信息：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8ETClL4XW2wMVZOWadJInVYzWmkO6CaUhxDn1zcgKhibu13265WdB5lwA/640?wx_fmt=png)

V3的日志信息：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8EgFSicaZiavMOMWotJRgiaqiaI6NLPHcHgRianEr1Eumdw0ib75ArdXJAc5xg/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8EJdlL84Gia5JoibpKtyicEPF0QukaROHq8uB3HFiafzpuvhZ3AZSYe9F4aA/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8EwSGh9yOBl7CCZqMjzP4hbia5zxkHORC4PKfqAztSA2SNA4PQ9Y9989A/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8E0fzoQOHHHbJP1XibVNgsWdJwThRHicZ1NLtFibodhNaibPibxLpwwClG3BQ/640?wx_fmt=png)

简单分析：

YOLO V2是一个纵向自上而下的网络架构，随着通道数目的不断增加，FLOPS是不断增加的，而V3网络架构是横纵交叉的，看着卷积层多，其实很多通道的卷积层没有继承性，另外，虽然V3增加了anchor centroid，但是对GT的估计变得更加简单，每个GT只匹配一个先验框，而且每个尺度只预测3个框，V2预测5个框，这样的话也降低了复杂度。

YOLO V3

YOLO V3在Pascal Titan X上处理608x608图像速度达到20FPS，在 COCO test-dev 上 mAP@0.5 达到 57.9%，与RetinaNet的结果相近，并且速度快了4倍。  YOLO V3的模型比之前的模型复杂了不少，可以通过改变模型结构的大小来权衡速度与精度。  速度对比如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8EicfnibpCOhyHrlJicnbQfj3Y3URJxZtSyJ7oqE9RhLCxKhNXwhvy29Hhg/640?wx_fmt=png)

改进之处： 
- 
多尺度预测；

- 
更好的基础分类网络和分类器。


多尺度预测

每种尺度预测3个box, anchor的设计方式仍然使用聚类,得到9个聚类中心,将其按照大小均分给3中尺度。
- 
尺度1：在基础网络之后添加一些卷积层再输出box信息；

- 
尺度2：从尺度1中的倒数第二层的卷积层上采样(x2)再与最后一个16x16大小的特征图相加,再次通过多个卷积后输出box信息，相比尺度1变大两倍；

- 
尺度3：与尺度2类似,使用了32x32大小的特征图。


分类器-类别预测：

YOLO V3不使用Softmax对每个框进行分类，主要考虑因素有两个：  
- 
Softmax使得每个框分配一个类别（score最大的一个），而对于Open Images这种数据集，目标可能有重叠的类别标签，因此Softmax不适用于多标签分类；

- 
Softmax可被独立的多个logistic分类器替代，且准确率不会下降。 分类损失采用binary cross-entropy loss。


基础网络 Darknet-53

仿ResNet, 与ResNet-101或ResNet-152准确率接近,但速度更快.对比如下:

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8E6rhITE2td37iblNOzH7hOhCSFFo9b5A9lTAibUbzMZic4m25FUQfKbDvg/640?wx_fmt=png)

YOLO V3网络结构如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8EfA7aDVvq0yIaibSbxrlpj7HeJ1I7cMh9ia2XFE2AvAXqmZ6dBujzRWibg/640?wx_fmt=png)

边框预测

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8ETBcSPspTCqYIWRDgLpItJSDhFGKRqf1zh2uIFGMmtKfDbPkHd4F0zQ/640?wx_fmt=png)


优缺点分析：

优点：
- 
快速，pipline简单，背景误检率低，通用性强。


YOLO V3对非自然图像物体的检测率远远高于DPM和RCNN系列检测方法。 但相比RCNN系列物体检测方法，YOLO V3具有以下缺点：  

缺点：
- 
识别物体位置精准性差，召回率低。


在每个网格中预测两个bbox这种约束方式减少了对同一目标的多次检测(R-CNN使用的region proposal方式重叠较多)，相比R-CNN使用Selective Search产生2000个proposal（RCNN测试时每张超过40秒），YOLO仅使用7x7x2个。

实验结果：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8EI5R6ovEfrzE26kCj1YHAq3GMxpfLSw5rWJMpd1icovMxYWteP97ZpGQ/640?wx_fmt=png)

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8Em6WQ94RibZic1JaSU6hCia5Y6Jc9dykxwQWnicdpm2TPObhoia1NXKO1V3A/640?wx_fmt=png)

自己也在数据集上做了一些实验：

用的数据集也是最近比较火的数据集——王者荣耀游戏数据。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/1MtnAxmWSwNiaUQzxGD1GUGj3XqEZbb8EXIG9hzJicia2FDaw3gGsmF9eCUYpFezPtmBBGqCXe6ZKp5qlkjzb9t3A/640?wx_fmt=jpeg)




## 参考文档

[https://zhuanlan.zhihu.com/p/25236464](https://zhuanlan.zhihu.com/p/25236464)
[http://blog.csdn.net/surgewong/article/details/51864859](http://blog.csdn.net/surgewong/article/details/51864859)
[https://deepsystems.io/en/reviews](https://deepsystems.io/en/reviews)
[https://zhuanlan.zhihu.com/p/24916786?utm_source=qq&utm_medium=social](https://zhuanlan.zhihu.com/p/24916786?utm_source=qq&utm_medium=social)
[https://arxiv.org/abs/1506.02640](https://arxiv.org/abs/1506.02640)



