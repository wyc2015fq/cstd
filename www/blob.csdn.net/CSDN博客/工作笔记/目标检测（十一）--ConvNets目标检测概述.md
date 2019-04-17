# 目标检测（十一）--ConvNets目标检测概述 - 工作笔记 - CSDN博客





2017年09月11日 19:23:19[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7317
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿转自：[http://coolwyj.com/%E9%80%9A%E7%94%A8%E7%89%A9%E4%BD%93%E6%A3%80%E6%B5%8B%E6%96%B9%E6%B3%95%E5%B0%8F%E7%BB%93/](http://coolwyj.com/%E9%80%9A%E7%94%A8%E7%89%A9%E4%BD%93%E6%A3%80%E6%B5%8B%E6%96%B9%E6%B3%95%E5%B0%8F%E7%BB%93/)

略增删改

[overfeat]([http://arxiv.org/abs/1312.6229](http://arxiv.org/abs/1312.6229))

[RCNN]([http://arxiv.org/abs/1311.2524](http://arxiv.org/abs/1311.2524))

[SPP-Net]([http://arxiv.org/pdf/1406.4729.pdf](http://arxiv.org/pdf/1406.4729.pdf))

[Fast-RCNN]([http://arxiv.org/abs/1504.08083](http://arxiv.org/abs/1504.08083))

[Faster-RCNN]([http://arxiv.org/abs/1506.01497](http://arxiv.org/abs/1506.01497))

[YOLO]([http://arxiv.org/abs/1506.02640](http://arxiv.org/abs/1506.02640))

[SSD]([http://arxiv.org/pdf/1512.02325v1.pdf](http://arxiv.org/pdf/1512.02325v1.pdf))

[HyperNet]([https://www.arxiv.org/abs/1604.00600](https://www.arxiv.org/abs/1604.00600))

[MR-CNN]([http://arxiv.org/abs/1505.01749](http://arxiv.org/abs/1505.01749))

[Inside-Outside Net]([https://arxiv.org/abs/1512.04143](https://arxiv.org/abs/1512.04143))

[R-FCN]([http://arxiv.org/abs/1605.06409](http://arxiv.org/abs/1605.06409))

[GBD-Net]([https://arxiv.org/abs/1610.02579](https://arxiv.org/abs/1610.02579))

上述方法大致提供了两种物体检测框架，下面分别来介绍。

## **1、基础：Overfeat**

**核心点**

feature map上的一个点可以对应到原图中的一个区域；在feature map上做滑动窗口代替在原图做滑动窗口。

feature map上的一个点可以对应到原图中的一个区域（也是后面框架的基础）。因此可以通过在feature map上做sliding window代替在原图做，避免原图每一个窗口都要单独计算CNN。当然带来的问题是这样得到的框不一定有在原图做sliding window准确。

此框架将detection问题分解为classification问题和localization问题。

**classification**：一张图片经过fully convolutional层及pool5后得到mxn的pool5 feature map。使用固定大小的filter（3x3）在pool5 feature map上做滑动窗口。此处的滑动窗口与一般的不同，总共要做9次，从(0,0), (0,1), (0,2), (1,0), (1,1)...处分别开始滑动。得到的feature map分别经过最后三层FC层，得到特征，然后再拼接起来得到最终的特征，用来分类。

**localization**：1-5层作为特征提取层和分类问题完全一样，后面接两个全连接层，组成 regressor network。训练时，前面5层的参数由classification network给定，只需要训练后面的两个全连接层。这个regressor network的输出就是一个bounding box，也就是说，如果将一幅图像或者一个图像块送到这个regressor network中，那么，这个regressor network输出一个相对于这个图像或者图像块的区域，这个区域中包含感兴趣物体的框。这个regressor network的最后一层是class specific的，也就是说，对于每一个class，都需要训练单独最后一层。这样，假设类别数有1000，则这个regressor network输出1000个bounding box，每一个 bounding box对应一类。

**合并多余框的方法**

令Cs为top-k（例如5）标准中每个scale下找到的可能类别。

令Bs为Cs中这些类别的预测框。而B集合是Bs的并集。

找B集合里面两个框的中心距离和相交面积之和最小的一对，如果他们的和大于给定阈值t，则结束，否则从B中删去两个框，合并这一对用他们的均值构成新的一个框，加入B

**trick**

1、multi-scale：将图片放缩到多个分辨率，然后把最后的特征or概率结合起来。

## 2、RCNN框架

**基于region，通过分类region达到识别的目的，结合Bbox对region进行修正。**

### 2.1、RCNN

**核心点**

使用selective search产生图中候选region，使用CNN提取region的特征，用SVM分类。

基本流程如下图，即：通过SelectiveSearch在原图中得到所有候选区域（即可能存在物体的region），然后对这些region依次提取特征，将得到的特征用SVM进行分类。

![](https://img-blog.csdn.net/20170911192623085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**Bbox regression**

用pool5提取到的特征来回归出一个偏移量，使得原框附加此偏移后更加接近ground truth。

有N对训练pairs   , 其中   , 分别代表原始region框的中心点坐标以及宽高，P表示proposal。G代表ground truth框，同样由四元组构成。 

**trick**

1、在region放缩到CNN输入大小前，将region向周围扩大16个像素来获取context信息

2、非极大值抑制（NMS）

3、在ImageNet上预训练，在特定领域fine-tuning

4、分类的时候需要分N+1类，因为存在非物体类，即背景

5、选择负样本时，与ground truth的IOU小于0.3的为负样本。通过cross validation得出。

6、没有fine-tuning的情况下，pool5层提取到的特征与FC6，FC7的特征效果相当。CNN的大部分表示能力来自于conv层。

经过fine-tuning之后，从各个层提取的特征效果均有提升，FC6,FC7提升效果最大。说明pool5层的特征更加general，fine-tuning的提升大部分来自后面的FC层。

7、使用pool5得到的特征做bounding-box regression，closed form方法。

### 2.3、SPP-Net

**核心点**

设计了Spatial Pyramid Pooling层，支持任意大小图片作为CNN的输入而不影响网络的FC层。整幅图片只需要计算一次网络SPP前面的layers。

conv层并不需要输入的图片是固定大小的，之所以限制输入图片固定大小，是因为conv层之后接了FC层，而FC层需要保证前一层的元素数目不变。

针对CNN中图片必须放缩到固定大小输入，从而导致信息损失的问题，此文提出了Spatial Pyramid Pooling来解决这一问题。

不同大小的图片经过conv层后得到的feature map大小也不同，预先设定要从此feature map获取的bin的数量，由feature map大小确定pooling窗口滑动的步长。这样经过SPP得到的特征长度就一样了。

SPP还可以应用多尺度的方法，在conv后的特征图上按照输出的bin数量构成金字塔型multi-scale。然后把得到的特征拼接。如下图所示。

![](https://img-blog.csdn.net/20170911194522215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

受overfeat启发，一张图只做一次conv计算，在conv后的feature map上做SPP

**trick**

1、使用图片的不同分辨率来训练，对于准确率有提升。例如本文训练时使用了224与180，测试时可以是任意尺寸

2、多尺度SPP可以提升准确率

3、使用不同初始化训练同一个模型，detection之后，对两个模型的输出窗口的交集，将置信度高的一个作为系统最终的输出。

### 2.3、fast-RCNN

**核心点**

共享计算，减少冗余，统一框架。RCNN需要每一个region都跑一次整个网络，而现在只有FC层以后的部分需要每个region单独跑。用softmax代替了SVM，同时也将Bbox regression纳入了整个框架。

基本流程如下图：

![](https://img-blog.csdn.net/20170911194700142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

与RCNN类似，先用SelectiveSearch找出候选框，但是不把每一个候选框都过一次CNN。而是像overfeat和SPP-Net那样整张图过一次CNN，然后用ROI-Pooling（即单层SPP-Net），将对应候选框的部分做pooling，得到相同长度的特征，又经过两层FC之后得到最终的特征。接着产生两个分支，一个分支给此特征分类，另一个分支回归此特征的候选框偏移（即RCNN中Bbox regression的部分）。fast-rcnn将分类和Bbox-regression何在了一起。

为了同时训练好分类器和框回归，本文提出了一种多任务目标函数，即：



其中：

  表示当该ROI为前景图时为1，否则为0.

  是标准交叉熵损失函数。  

训练的时候一个batch从两张图片中提取128个samples。每张图片取64个samples。正样本为与ground truth的IOU大于0.5的，负样本为IOU在0.1到0.5之间的。

**trick**

1、FC层的计算可以使用SVD来加速。将一个FC层变成两个FC层，其中没有非线性变换，分别计算SVD的前半部分和后半部分。

2、训练的时候并不一定需要把整个网络全部fine-tune，因为较前的层表示的特征都比较通用，fine-tune与否关系不大。比如VGG16在conv3_1以后的层对结果的影响才比较大。此外全部用来fine-tune耗时会比较大。

3、single-scale的accuracy较multi-scale低一点，但是速度要快很多。

### 2.4、faster-RCNN

**核心点**

共享计算，减少冗余。基于fast-rcnn，设计了Region Proposal Network，将region的生成也纳入到CNN的计算中来。

faster-rcnn是对fast-rcnn的改进，速度得到了进一步提升。整个流程见下图。

![](https://img-blog.csdn.net/20170911195127816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

fast-rcnn中使用SelectiveSearch得到候选框，花费了很长时间。faster-rcnn中将找候选框的功能结合到了rcnn框架中，使用一个叫做region proposal network（RPN）的神经网络并行产生候选框。

与fast-rcnn类似，faster-rcnn先将整幅图forward 到最后一层conv层，在pool5层分出两支。一支后面接RPN，另一只接ROI Pooling，等待从RPN得到的框。

RPN在最后的conv层上做滑动窗口，每一个窗口产生256维的向量，根据这一个向量又分出两支，分别用于分类和回归框。如图所示。

![](https://img-blog.csdn.net/20170911195230905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

此外，针对每一个窗口，文中还提出了使用anchor，即大小、比例不同的原始框作为回归的起点，而非整个窗口大小。这样假设有k种anchor，那么RPN最终的分类有2k个输出，回归有4k个输出。

在最后的conv层的feature map上滑窗，feat_stride为conv1到conv5所有步长的乘积。ZF-Net为16.以feat_stride x feat_stride为base，进行scale和ratio变换，得到9个anchor。在conv 5的feature map的每一个点为中心的anchors，对应到原图上绝对坐标，由此可得anchor与ground truth的IOU，从而选取正负样本。

一个batch从所有anchor选取256个样本。正负样本1：1.

为了使RPN与fast-rcnn共享权值，减少计算，faster-rcnn使用了四阶段训练的方法:

1、训练RPN

2、使用1中产生的proposals训练faster-rcnn做detection

3、用fast-rcnn与RPN共享的部分初始化RPN，只训练RPN独有的层。

4、固定conv层，只训练fast-rcnn独有的层。

**trick**

1、训练的时候对于跨过边界的anchor，不加入训练样本。这样可能会导致不收敛.

### **2.5、MR-CNN**

**核心点**

结合region本身的特征，region周围采样的特征和图像分割的特征来提高识别率。

MR-CNN流程图如下：

![](https://img-blog.csdn.net/20170911195618119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

MR-CNN将detection问题分解为classification和localization两部分。

classification

分为两部分，即Multi-Region CNN Model和Semantic Segmentation-Aware CNN Model。

**Multi-Region CNN Model**

一幅图片经过NN的最后一层conv后得到feature map

图中候选框由SelectiveSearch得到。对于每一个region，取10个区域分别提取特征后拼接。这样可以强制网络捕捉物体的不同方面，同时可以增强网络对于定位不准确的敏感性。区域的选取如下图：

![](https://img-blog.csdn.net/20170911195641253?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

所谓adaptive max pooling即ROI max pooling

**Segmentation-Aware CNN Model**

经验上来说segmentation对于detection有帮助。

使用Fully Convolutional Network（FCN）来做Segmentation。不使用pixel-level的标签，仅需要使用image-level的标签即可，即此图是哪一类。

训练好FCN后，去掉最后的classification layer，取剩下的最后一层作为feature layer，与上述`Multi-Region CNN Model`产生的feature拼接作为最后的feature。

Localization

MR-CNN使用了3种方法来精确定位。

1、Bbox regression

在`Multi-Region CNN Model`中整幅图经过NN的最后一层conv后，接一个Bbox regression layer，与RPN不同，此处的regression layer是两层FC以及一层prediction layer。为了防止SelectiveSearch得到的框过于贴近物体而导致无法很好的框定物体，将候选框扩大为原来的1.3倍再做。

2、Iterative Localization

初始的框是SelectiveSearch得到的框，然后用已有的classification模型对框做出估值，低于给定阈值的框被筛掉，剩下的框用1中的regression network来调整大小。重复上述过程。文章重复了2次。

3、Bounding box voting

首先对经过2处理后的框应用NMS, IoU = 0.3，得到detection的结果   。 然后对于每一个框，用每一个和他同一类的而且IoU大于0.5的框（经过2筛选的），来加权坐标，得到最后的框。每一个框的权值是   ,加权公式为：



其中，   表示第i个框是c类的置信度。   表示属于c类的第i个框。

**trick**

1、MR-CNN的三种定位方法

2、多个region提供position sensitive的信息

### 2.6、HyperNet

**核心点**

结合多个layer提取的特征，即multi-level。

HyperNet结合了CNN中多个layer的特征，达到了一种multi-level的效果。而且从较前层获取的精细特征可以减少对于小物体检测的缺陷。本质上是对faster-rcnn的改进。

![](https://img-blog.csdn.net/20170911200617318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如上图，一幅图经过CNN后，只保留conv1到conv5的layer。

以conv3为基准，feature map大于它的用max pooling来降维，小于它的用deconv来扩增，使得到的所有特征维度相等，然后经过LRN正则化之后堆叠在一起，形成Hyper Feature maps。因为不同层的特征量纲不同，直接拼接会导致量纲不同的特征互相抑制。

得到Hyper feature之后，接一个region proposal generation module。包括一个ROI pooling层，一个conv层，一个FC层以及经典的sibling classification&regression layers。但是输入这个module之前如何产生30k个初始box论文并没有讲，可能是随机选取。然后对于每一个box，ROI pooling产生13X13的输出，后面的layer进行框的预测和回归，保留框置信度最高的N个送到最后的一部分。

最后一部分对每个框分别判别之前，多加了一个conv层来降维并增加网络深度。最后得出结果。

**trick**

1、multi-level feature， 不要从相邻层提取，因为相邻层强关联。

### 2.7、R-FCN

**核心点**

在faster-rcnn的基础上，解决了网络FC层每个region需要独立计算的问题。

解决了faster-rcnn由于ROI-Pooling层后面的结构需要对每一个region跑一次而耗时比较大的问题。整个框架变为了fully convolutional， 引入了position-sensitive score maps来解决对于图片分类的平移不变性和对于物体检测的平移变化性。

网络的整体结构与faster-rcnn类似，不同的是RPN得到候选框后传给了position-sensitive ROI pooling层，而非ROI pooling层。

最后一层conv层后面紧接着的就是position-sensitive ROI pooling层，对于每一类物体，它有k x k层，本文中k=3, 这9层分别代表一个九宫格的左上角，上边，右上角，……，右下角，一个格子的大小为  。那么position-sensitive ROI pooling 层共有   个channel。更具体来讲，每一个channel，例如代表左上角的channel，是从最后一层conv层做滑动窗口，每一个点处的窗口取其左上角做pooling。

在第(i,j)个格子里，position-sensitive ROI pooling只在该格子中做pool操作。



其中，   表示在网络参数为   的情况下，(i,j)这个bin的第c类物体的pool值。

  表示   个score map中的一个。

  表示当前ROI的左上角坐标。

n是feature map的当前bin中点的个数。

决定此ROI是哪一类的时候，只需要取各个bin的得分均值，然后用softmax分类。即   ,  

![](https://img-blog.csdn.net/20170911200810658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

整个算法流程如下图：

![](https://img-blog.csdn.net/20170911200921400?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**trick**

1、训练的时候采用OHEM（online hard example mining），即反向传导的时候仅选择loss最大的几项来计算。

2、   算法，即dilated convolution

简单来说， 即dilated convolution，是对于conv过程的一种改进。相关介绍的文章见multi-scale context aggregation by dilated convolutions。原理见下图。

![](https://img-blog.csdn.net/20170911201502397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170911201511741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

caffe中是通过im2col实现的

![](https://img-blog.csdn.net/20170911201600150?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 2.8 Inside-Outside Net

**核心点**

引入了不同layer的特征，即multi-levle or multi-scale，以及context的特征。

![](https://img-blog.csdn.net/20170911202348179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

大部分物体检测算法的缺陷之一在于提取的特征不够精细。这篇文章针对这一个问题进行了改进。利用物体的multi-scale信息（信息的精细度）和context信息（信息的广度），提高识别率。

基于VGG network，使用SelectiveSearch一类的proposal生成算法生成reigon，作为ROI。一次过一张图，保留conv1到conv5，其后跟两个4-direction IRNN，再接一个conv得到context特征。然后将conv3、conv4、conv5以及context特征拼接起来，正则化后分类并回归框。

4-direction IRNN的结构如下图所示。即将feature map分块，分别按照4个方向排列从而形成序列，经过IRNN处理后形成的feature map堆叠到一起传入下一个单元。最后使用deconv层还原图像，用semantic segmentation的loss来监督学习。

![](https://img-blog.csdn.net/20170911202529071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**trick**

1、对特征使用L2 norm：对于一个d维输入   公式:   ,其中，   .为了不让特征数值上而过小导致难以训练，需要给特征乘以一个放缩因子，可以固定，也可以学习一个参数。

2、提取全局context特征可以使用global average pooling、堆叠CNN、RNN等方法，区别如下图所示。

![](https://img-blog.csdn.net/20170911202500550?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

堆叠CNN可能获得更好的效果，但是堆叠RNN并不一定。

3、测试的时候可以将图片左右翻转测两次，然后取均值。

## 3、YOLO框架

**等效于将原图划分为n个区域，然后分别在各个区域中回归框同时分类框。**

### 3.1、YOLO

**核心点**

将原图分块，每一块分类和回归一起做

流程图如下：

![](https://img-blog.csdn.net/20170911202621583?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

YOLO将原图划分为SxS个区域。如果一个物体的中心在某个区域，那么该区域就负责此物体的定位和识别。每个区域会回归B个框(x,y,w,h)以及其对应的置信度(conf)。置信度用  表示。即给定某个预测时（特征以及预测框），第一项表示该区域存在物体的概率，后一项表示该预测的准确度，即预测框与ground truth的重叠面积。每个区域还会预测C个条件概率  ，C为类别数。

在test阶段，我们将条件概率和置信度相乘就得到了某个区域中存在某物体的概率以及其准确度。即：



![](https://img-blog.csdn.net/20170911202631791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

YOLO的缺陷：受限于每个区域对于每个类只预测两个框，导致某个区域附近临近的小物体或物体集群（例如鸟群）无法被检测到。

因为是在区域中直接回归框，因此物体的长宽比比较特殊而且训练中没有出现时，难以识别。

提取的特征过于粗糙，因为经过了很多次降采样，不够精细。

loss function对于同样大小的错误在大框和小框上的惩罚应该不同。

主要错误在于定位不准。

**trick**

1、做detection要求精细的特征，因此将输入图片的大小适当扩大会有比较好的效果。例如YOLO讲原GoogleNet的输入大小由224改为了448

2、使用平方和误差损失函数的度量方法，应该能反应：细小的误差对于大框的影响要远小于对于小框的影响。因此预测的是框w和h的平方根，而不是其本身。提高灵敏度。

3、通常来说，一幅图片中包含物体的区域远小于不含物体的区域。在训练时，这种情况会导致没有物体的区域产生的loss严重影响正常区域的loss（因为每个区域都要产生预测的框，但是没有物体存在的区域并没有ground truth，此时该区域仍然会产生loss）。因此，需要给不同区域的loss加权。全局loss公式如下：



其中   表示在区域i中预测的第j个框负责这个区域的预测。   表示区域i中是否存在物体。

4、数据扩增：在HSV颜色空间随机调整曝光率和饱和度；随机放缩和平移。

### 3.3、SSD

**核心点**

在YOLO的基础上，引入了anchor以及多层特征，即multi-scale，并结合了众多trick

针对YOLO选取的特征不精细，对小物体和集群物体无法很好识别的问题做了改进。

![](https://img-blog.csdn.net/20170911202744018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

与YOLO类似，也是相当于把图片划分为n个区域，然后直接在这些区域上判断类别并回归框。

区别在于：

1、使用了多个layer不同精细程度的feature拼接。

2、每一层提取特征的layer都会对feature map上的每个点得出每个类的置信度以及框的四个值（x,y,w,h）来拼接。

3、feature map上每一个点引入了类似faster-rcnn的anchor。在确定正负样本的时候，只要anchor与ground truth的IOU大于0.5即为正样本。即一个anchor可以对应多个物体，不必强迫其选择出一个。

4、loss function:  

其中N是匹配到的anchor的数量。   是Smooth L1 loss。c是多类的confidence，   设置为1.

缺点：对小物体识别效果差于大物体。部分原因是物体过于小，导致后面的layer基本没有信息。

**tricks**

1、hard negative mining：负样本：正样本=3：1

2、data augmentation：1>初始图片 2>选择patch使得与物体的IoU分别为0.1，0.3，0.5，0.7，0.9，patch的大小为初始图片的[0.1,1]，长宽比为原图的1/2，2倍。 3> 2中得到的patch resize到固定大小，然后以0.5的概率翻转。

3、使用更多的feature map可以提升准确率

4、使用较多的anchor可以提高准确率

5、   算法，即dilated convolution，见<a href="#atrous">2.7</a>

## 4、比较

下表中+++表示训练数据为VOC07+VOC12+MS COCO

其余方法的训练数据均为VOC07 for VOC07 test，VOC07+VOC12 for VOC12test， MS COCO for MS COCO test
|方法|检测耗时|VOC07|VOC12|MS COCO|
|----|----|----|----|----|
|overfeat|----|----|----|----|
|RCNN|13s|66.0|53.3|----|
|SPP-Net|0.29s|59.2|----|----|
|fast-rcnn|0.32s|70.0|68.0|19.7|
|faster-rcnn+++|140ms|85.6|83.8|21.9|
|HyperNet|1140ms|76.3|71.4|----|
|MR-CNN|30s|78.2|73.9|----|
|R-FCN+++|0.17s|83.6|82.0|29.9|
|ION|0.8s|79.2|76.4|33.1|
|实时算法|--|--|--|--|
|YOLO|45FPS|63.4|57.9|----|
|Fast YOLO|155FPS|52.7|----|----|
|SSD300|58FPS|72.1|70.3|20.8|
|SSD500|23FPS|75.1|73.1|24.4|

﻿﻿

补充：﻿﻿

﻿﻿

5、GBD-Net

**Gated Bi-directional CNN for Object Detection**
- intro: The Chinese University of Hong Kong & Sensetime Group Limited
- paper: [http://link.springer.com/chapter/10.1007/978-3-319-46478-7_22](http://link.springer.com/chapter/10.1007/978-3-319-46478-7_22)
- mirror: [https://pan.baidu.com/s/1dFohO7v](https://pan.baidu.com/s/1dFohO7v)

**Crafting GBD-Net for Object Detection**
- intro: winner of the ImageNet object detection challenge of 2016. CUImage and CUVideo
- intro: gated bi-directional CNN (GBD-Net)
- arxiv: [https://arxiv.org/abs/1610.02579](https://arxiv.org/abs/1610.02579)
- github: [https://github.com/craftGBD/craftGBD](https://github.com/craftGBD/craftGBD)

GBD-Net（Gated Bi-Directional CNN [1]）是CUImage团队的成果，也是今年DET任务中的一大亮点。该方法利用双向门控的CNN网络在不同尺度的上下文窗口中选择性地传递信息，以此对context建模。

GBD-Net的研究动机源于对context信息在候选窗口分类过程中起到的作用的仔细分析。首先，Context信息在对窗口分类时能起到关键的作用，如图2(a)(b)所示，图中的红框必须结合Context信息才能准确判断其类别（包括判断为背景）。所以很多时候，我们可以利用context信息作出如图1(c)所示的判断。但是如图1(d)所示，并不是所有的context信息都能给我们正确的指导，所以context信息需要选择性的利用。

![](https://img-blog.csdn.net/20170430215627695)

图2. GBD-Net的研究动机[1]

基于这一点，CUImage提出了GBD-Net。如图3所示，GBD-Net采集Context信息的方式与[2][3]一脉相承，直接在目标窗口基础上放大窗口以获得更多的context信息，或缩小窗口以保留更多的目标细节，以此得到多个support region，双向连接的网络让不同尺度和分辨率的信息在每个support region之间相互传递，从而综合学习到最优的特征。然而如研究动机中所说，并非所有的上下文信息都能给决策带来“正能量”，所以在双向互通的连接上都加了一个“门”，以此控制context信息的相互传播。GBD-Net在ImageNet DET数据集上，在ResNet-269为基础网络，带来了2.2%的mAP提升。

![](https://img-blog.csdn.net/20170430215647320)

图3. GBD-Net框架图[1]

6、StuffNet

**StuffNet: Using ‘Stuff’ to Improve Object Detection**
- arxiv: [https://arxiv.org/abs/1610.05861](https://arxiv.org/abs/1610.05861)

**Generalized Haar Filter based Deep Networks for Real-Time Object Detection in Traffic Scene**
- arxiv: [https://arxiv.org/abs/1610.09609](https://arxiv.org/abs/1610.09609)

**Hierarchical Object Detection with Deep Reinforcement Learning**
- intro: Deep Reinforcement Learning Workshop (NIPS 2016)
- project page: [https://imatge-upc.github.io/detection-2016-nipsws/](https://imatge-upc.github.io/detection-2016-nipsws/)
- arxiv: [https://arxiv.org/abs/1611.03718](https://arxiv.org/abs/1611.03718)
- slides: [http://www.slideshare.net/xavigiro/hierarchical-object-detection-with-deep-reinforcement-learning](http://www.slideshare.net/xavigiro/hierarchical-object-detection-with-deep-reinforcement-learning)
- github: [https://github.com/imatge-upc/detection-2016-nipsws](https://github.com/imatge-upc/detection-2016-nipsws)
- blog: [http://jorditorres.org/nips/](http://jorditorres.org/nips/)

**Learning to detect and localize many objects from few examples**
- arxiv: [https://arxiv.org/abs/1611.05664](https://arxiv.org/abs/1611.05664)

**Speed/accuracy trade-offs for modern convolutional object detectors**
- intro: Google Research
- arxiv: [https://arxiv.org/abs/1611.10012](https://arxiv.org/abs/1611.10012)

**SqueezeDet: Unified, Small, Low Power Fully Convolutional Neural Networks for Real-Time Object Detection for Autonomous Driving**
- arxiv: [https://arxiv.org/abs/1612.01051](https://arxiv.org/abs/1612.01051)
- github: [https://github.com/BichenWuUCB/squeezeDet](https://github.com/BichenWuUCB/squeezeDet)

![](https://img-blog.csdn.net/20161027193124810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从图中看，可以知道stuffnet并没有特别之处：

1 用vgg16作为feature extractor，并共享

2 不同的task用不同branch作为sub-network

3 不同task之间的features的融合，来服务于det-net（即fast-rcnn里的detection branch）。

seg-net采用DeepLab的[Semantic image segmentation with deep convolutional nets and fully connected crfs](https://arxiv.org/pdf/1412.7062v4.pdf)。具体的segmentation的内容请参考这篇论文。

cls-net和det-net沿用faster-rcnn的。

亮点:

1 跟一般的segmentation不同，论文的segmentation是针对stuff（没有固定形状的物体，如水，天空，草地等）的，而且stuff的segmentation的label容易获取。

2 非常直观而且直观的framework，不似[Contextual Priming and Feedback for Faster R-CNN](http://abhinav-shrivastava.info/context_priming_feedback.pdf)的那么复杂。

那么为什么要用stuff而不是object？（当然论文里最后还是用到了object，而且还是stuff和object的并集）

1 stuff通常会和target object有空间位置上的关联，如船boat往往在水上面，天空下面

2 stuff可以在这里理解为background，能在一定程度上区分出background和target objects，而且也能够区分不同的target objects，如car和boat。

3 stuff在不同的数据集上往往是相同的或者相似的，这样就可以在一个数据集上训良stuff的seg-net，然后可以在其他数据集上直接拿过来用。

### 本质上stuff充当了context。

那么怎么训练？

作者用pascal voc 2010的数据集来训练stuffnet，然后用训练好的stuffnet的seg-net来对新的数据集预测stuff的segmentation，并作为其ground-truth。

这样使得只有detection的label的数据集有了segmetation的ground-truth label。

新数据集的stuffnet的训练过程和pascal voc 2010的一样。

由于Deeplab的seg-net和faster-rcnn的超参数一样，为了简单，作者采用了faster-rcnn的默认参数：

一旦训练好stuff-net，便可以用来做segment和detect了。

当然如果不想要segmentation的结果，可以直接把conv5-3-seg后面的layers给扔了。

至于结果嘛，感觉不是很好，没有带来big margin的惊喜。

![](https://img-blog.csdn.net/20161027195733473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

从上面的结果看，StuffNet对小物体比较友好，笔者估计segmentation能够提高小物体的检测效果。

想提高小物体的detection的效果的童鞋，不妨从这里入手。

﻿﻿﻿﻿﻿﻿﻿﻿

﻿﻿



