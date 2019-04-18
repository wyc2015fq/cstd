# YOLO—神经网络原理 - wsp_1138886114的博客 - CSDN博客





2018年08月25日 10:19:47[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1640
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)









### 一、背景介绍

YOLO（You Only Look Once: Unified, Real-Time Object Detection），是Joseph Redmon和Ali Farhadi等人于2015年提出的基于单个神经网络的目标检测系统。在2017年CVPR上，Joseph Redmon和Ali Farhadi又发表的YOLO 2，进一步提高了检测的精度和速度。  

论文下载地址:[https://pjreddie.com/media/files/papers/yolo.pdf](https://pjreddie.com/media/files/papers/yolo.pdf)

##### 1.1 目标检测发展过程
- 
早期的目标检测方法:  

通过提取图像的一些 robust 的特征（如 Haar、SIFT、HOG 等），使用 DPM （Deformable Parts Model）模型，用滑动窗口（silding window）的方式来预测具有较高 score 的 bounding box。这种方式非常耗时，而且精度又不怎么高。

- 
后来出现了object proposal方法（其中selective search为这类方法的典型代表）: 

相比于sliding window这中穷举的方式，减少了大量的计算，同时在性能上也有很大的提高。利用 **selective search**的结果，结合卷积神经网络的R-CNN出现后，Object detection 的性能有了一个质的飞越。基于 R-CNN 发展出来的 SPPnet、Fast R-CNN、Faster R-CNN 等方法，证明了 “Proposal + Classification” 的方法在 Objection Detection 上的有效性。 

关于[R-CNN；fast-R-CNN；faster-R-CNN](blog.csdn.net/wsp_1138886114/article/details/81906661)详情☛- 
相比于 R-CNN 系列的方法，**YOLO提供了另外一种思路**： 

将 Object Detection(目标检测) 的问题转化成一个 Regression 问题。给定输入图像，直接在图像的多个位置上回归出目标的bounding box以及其分类类别。
- **YOLO**是一个可以**一次性预测多个Box位置**和**类别**的卷积神经网络能够**实现端到端的目标检测和识别**，**其最大的优势就是速度快**。 
- 事实上，目标检测的本质就是回归，因此一个实现回归功能的CNN并不需要复杂的设计过程。YOLO没有选择滑动窗口（silding window）或提取proposal的方式训练网络，而是直接选用整图训练模型。这样做的好处在于可以更好的区分目标和背景区域，相比之下，采用proposal训练方式的Fast-R-CNN常常把背景区域误检为特定目标。  



![这里写图片描述](https://img-blog.csdn.net/20180825100230457?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


### 二、YOLO 神经网络

##### 2.1 YOLO的总体框架示意图

![这里写图片描述](https://img-blog.csdn.net/20180825095457364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

一体化的设计方案：

YOLO的设计理念遵循端到端训练和实时检测。YOLO将输入图像划分为S*S个网格，如果一个物体的中心落在某网格(cell)内，则相应网格负责检测该物体。

在训练和测试时，每个网络预测B个bounding boxes，每个bounding box对应5个预测参数：

bounding box的中心点坐标(x,y)，宽高（w,h） 

和置信度评分（confidence） 

这个置信度评分：
综合反映了：

当前bounding box中含有object的置信度Pr(Object) 

当前bounding box预测目标位置的准确性IOU(pred|truth) 

如果bouding box内不存在物体，则Pr(Object)=0。如果存在物体，则根据预测的bounding box和真实的bounding box计算IOU，同时会预测存在物体的情况下该物体属于某一类的后验概率Pr(Class_i|Object)。
假定一共有C类物体，那么每一个网格只预测一次C类物体的条件类概率Pr(Class_i|Object), i=1,2,…,C;每一个网格预测B个bounding box的位置。即这B个bounding box共享一套条件类概率Pr(Class_i|Object), i=1,2,…,C。基于计算得到的Pr(Class_i|Object)，在测试时可以计算某个bounding box类相关置信度：Pr(Class_i|Object)*Pr(Object)*IOU(pred|truth)=Pr(Class_i)*IOU(pred|truth)。如果将输入图像划分为7*7网格（S=7），每个网格预测2个bounding box (B=2)，有20类待检测的目标（C=20），则相当于最终预测一个长度为S*S*(B*5+C)=7*7*30的向量，从而完成检测+识别任务，整个流程可以通过下图理解。

##### 2.2 缺点：

虽然每个格子可以预测B个bounding box，但是最终只选择只选择IOU最高的bounding box作为物体检测输出，即每个格子最多只预测出一个物体。当物体占画面比例较小，如图像中包含畜群或鸟群时，每个格子包含多个物体，但却只能检测出其中一个。这是YOLO方法的一个缺陷。

### 三、Loss函数

YOLO使用均方和误差作为loss函数来优化模型参数，即网络输出的S*S*(B*5 + C)维向量与真实图像的对应S*S*(B*5 + C)维向量的均方和误差。如下式所示 

这里写图片描述 

其中，coordError、iouError和classError分别代表预测数据与标定数据之间的坐标误差、IOU误差和分类误差。具体的公式如下： 

这里写图片描述 

其中，x,y,w,C,p为网络预测值，x,y,w,C,p帽 为标注值。ii(obj)(i)表示物体落入格子i中，ii(obj)(ij) 表示物体落入格子i的第j个bounding box内。ii(noobj)(ij) 表示物体未落入格子i的第j个bounding box内。YOLO方法模型训练依赖于物体识别标注数据，因此，对于非常规的物体形状或比例，YOLO的检测效果并不理想。？？？不是都依赖于标注数据吗？

可以看到，式中有几项是带系数的，这是因为：  

[1] 位置相关误差（坐标、IOU）与分类误差对网络loss的贡献值是不同的。

[2] 在计算IOU误差时，落入物体的格子与无落入物体的格子，二者的IOU误差对网络loss的贡献值是不同的。若采用相同的权值，那么无落入物体的格子的confidence值近似为0，反过来，变相放大了落入物体的格子的confidence误差在计算网络参数梯度时的影响。

[3]对于相等的误差值，大物体误差对检测的影响应小于小物体误差对检测的影响。这是因为，相同的位置偏差占大物体的比例远小于同等偏差占小物体的比例。YOLO将物体大小的信息项（w和h）进行求平方根来改进这个问题。（注：这个方法并不能完全解决这个问题）。那有没有其他什么好方法解决这一问题？

### 四、训练

YOLO模型训练分为两步：

1）预训练。使用ImageNet  

1000类数据训练YOLO网络的前20个卷积层+1个average池化层+1个全连接层。训练图像分辨率resize到224x224。

2）用步骤1）得到的前20个卷积层网络参数来初始化YOLO模型前20个卷积层的网络参数，然后用VOC 20类标注数据进行YOLO模型训练。为提高图像精度，在训练检测模型时，将输入图像分辨率resize到448x448。那这时候用到的就是24个卷积层加2个全连接层？

### 五、效果

下表给出了YOLO与其他物体检测方法，在检测速度和准确性方面的比较结果（使用VOC 2007数据集）。 
![这里写图片描述](https://img-blog.csdn.net/20180825101813390?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 5.1 YOLO具有如下优点：

1.快。YOLO将物体检测作为回归问题进行求解，整个检测网络pipeline简单。在titan x GPU上，在保证检测准确率的前提下（63.4% mAP，VOC 2007 test set），可以达到45fps的检测速度。  

2.背景误检率低。YOLO在训练和推理过程中关注整张图像的整体信息，而基于region proposal的物体检测方法（如rcnn/fast rcnn），在检测过程中，只关注候选框内的局部图像信息。因此，对于后者，若当图像背景中的部分数据被包含在候选框中送入检测网络进行检测时，容易被误检测成物体。测试证明，YOLO对于背景图像的误检率低于fast rcnn误检率的一半。  

3.通用性强。YOLO对于艺术类作品中的物体检测同样适用。它对非自然图像物体的检测率远远高于DPM和RCNN系列检测方法。
##### 5.2 相比RCNN系列物体检测方法，YOLO具有以下缺点：

1.识别物体位置精准性差。  

2.召回率低。召回率？

为提高物体定位精准性和召回率，YOLO作者提出了YOLO9000，提高训练图像的分辨率，引入了faster rcnn中anchor box的思想，对各网络结构及各层的设计进行了改进，输出层使用卷积层替代YOLO的全连接层，联合使用coco物体检测标注数据和imagenet物体分类标注数据训练物体检测模型。相比YOLO，YOLO9000在识别种类、精度、速度、和定位准确性等方面都有大大提升。 

鸣谢： 
[https://blog.csdn.net/qq_28123095/article/details/80052308](https://blog.csdn.net/qq_28123095/article/details/80052308)
[https://blog.csdn.net/guoyunfei20/article/details/78744753](https://blog.csdn.net/guoyunfei20/article/details/78744753)









