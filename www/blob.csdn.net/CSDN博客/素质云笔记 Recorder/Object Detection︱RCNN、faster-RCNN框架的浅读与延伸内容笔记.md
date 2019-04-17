# Object Detection︱RCNN、faster-RCNN框架的浅读与延伸内容笔记 - 素质云笔记/Recorder... - CSDN博客





2017年01月22日 21:19:38[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：6622








# 一、RCNN，fast-RCNN、faster-RCNN进化史

本节由CDA深度学习课堂，唐宇迪老师教课，非常感谢唐老师课程中的论文解读，很有帮助。 

.

## 1、Selective search

![这里写图片描述](https://img-blog.csdn.net/20170122203137677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如何寻找有效的候选框，最开始的就是这个方法。 

寻找方法就是一开始把一幅图像，分割成无数个候选框构造而成的（convert regions to boxes） 

然后根据一些色彩特征、把候选框进行融合，框数量变小了，框变大；效果就是逐渐、慢慢找到最好的框 

.
## 2、R-CNN（CVPR 2014）

图像中的候选框，每个boxes跑一次卷积神经网络（一张图大概2000个框）。conv之后接SVM/reg，进行判断，论文中用SVM来进行分类（比softmax好一些） 
![这里写图片描述](https://img-blog.csdn.net/20170122203351050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

但是，每个都要计算太浪费时间了，最开始，需要把数据固化到硬盘很占容量，同时分类用SVM也非常耗时，效率不高。
在RCNN中CNN阶段的流程大致如下： 
![这里写图片描述](https://img-blog.csdn.net/20170326114157791?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

、红色框是selective search 输出的可能包含物体的候选框（ROI）。 

一张图图片会有~2k个候选框，每一个都要单独输入CNN做卷积等操作很费时。 

.
## 3、Faster R-CNN（ICCV 2015）

官方是caffe版本：towards real-time object detection with region proposal networks 

借用了RPN(region proposal network)选择候选框。 

**RPN可以很有效地共享卷积成果。**RCNN是基于像素点来做的，先提取框，后卷积，有几个框就要多少次卷积；RPN是卷积之上的特征图来的，先卷积，共享卷积成果（效率更高的切入点），只需要一次卷积。 
![这里写图片描述](https://img-blog.csdn.net/20170122204607161?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- （1）RPN候选框生成阶段。

在VGG中的最后一层卷积层的特征图中，找到某一个点，通过感受野，还原到原始图像，变成了9张大小不同的框anchor boxes，像素（128,256,521）*比率（1:1，1:2，1:3）1个点，模拟出9个候选框。其中弱候选框有超出边界的话，则去掉。
- （2）分类阶段。

选出候选框之后，接上分类对候选框进行判断。
- （3）微调阶段。

选定一些框之后，需要对框框进行最后的微调box regression、box回归。计算差异（长宽比），然后作了微调，box regression 
![这里写图片描述](https://img-blog.csdn.net/20170122205927238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- （4）ROI pooling层阶段

spatial pyramid pooling（SPP layer）为最后输出做准备。做了微调之后，候选框大小不一样，通过调整pooling的滑块，调整到相同的结果。
- 问题一：真的要画这9个候选框吗？

答：不是。卷积层每个点演化成9个框，而9个框则是虚构出来的，而没有真正画在图像上。
- 问题二：感受野

计算感受野的时候，在计算未来的候选框大小，VGG网络只需要根据pooling层来计算即可，因为卷积层不会矿大、缩小感受野。
- 问题三：分类任务实质上并不是二分类

之后再接分类softmax层判断候选框是否有物体。但是根据源码来看，并不是二分类（是否有物体），而是18分类。通过定义loss function来实现成9个框，9个框计算正负的概率，那就是18个结果。
- 问题四：如何判断候选框是有物体的？

根据给定的训练集的输入数据的四个坐标（x,y,m,h）的groud truth，看重合部分，大于0.7，认为是一个物体（正例，前景），小于0.3认为不是一个物体（负例，背景） 

.

## 4、YOLO

来源[知乎机智的大群主](https://www.zhihu.com/question/48979623/answer/113815747)，FACEBOOK提出来的YOLO网络，也是进行物体检测，最快达到每秒钟155帧，达到了完全实时。它让一整张图像进入到神经网络，让神经网络自己判断这物体可能在哪里，可能是什么。但它缩减了可能图像块的个数，从原来Faster R-CNN的2000多个缩减缩减到了98个。 
![这里写图片描述](https://img-blog.csdn.net/20170224171623563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同时取消了Faster R-CNN里面的RPN结构，代替Selective Search结构。YOLO里面没有RPN这一步，而是直接预测物体的种类和位置。
YOLO的代价就是精度下降，在155帧的速度下精度只有52.7，45帧每秒时的精度是63.4。 

github：[https://github.com/xingwangsfu/caffe-yolo](https://github.com/xingwangsfu/caffe-yolo)

**流程：**（来源：[图解YOLO](https://zhuanlan.zhihu.com/p/24916786?refer=xiaoleimlnote)）
- Resize成448*448，图片分割得到7*7网格(cell)
- CNN提取特征和预测：卷积不忿负责提特征。全链接部分负责预测：a) 7*7*2=98个bounding box(bbox) 

的坐标x_{center},y_{center},w,h 和是否有物体的conﬁdence 。 b) 

7*7=49个cell所属20个物体的概率。- 过滤bbox（通过nms）

![这里写图片描述](https://pic3.zhimg.com/v2-a6ac1595dc4d61a1cb0b850b7f7b6f0e_b.png)

**缺陷：**
- YOLO对相互靠的很近的物体（挨在一起且中点都落在同一个格子上的情况），还有很小的群体 

检测效果不好，这是因为一个网格中只预测了两个框，并且只属于一类。
- 测试图像中，当同一类物体出现的不常见的长宽比和其他情况时泛化能力偏弱。
- 由于损失函数的问题，定位误差是影响检测效果的主要原因，尤其是大小物体的处理上，还有待加强。

.

## 5、YOLO2

参考：[YOLO2](https://zhuanlan.zhihu.com/p/25167153?refer=xiaoleimlnote)、[YOLOv2 论文笔记](http://blog.csdn.net/jesse_mx/article/details/53925356)

**YOLO2主要有两个大方面的改进：**
- 使用一系列的方法对YOLO进行了改进，在保持原有速度的同时提升精度得到YOLOv2。
- 提出了一种目标分类与检测的联合训练方法，同时在COCO和ImageNet数据集中进行训练得到YOLO9000，实现9000多种物体的实时检测。

YOLOv2首先修改预训练分类网络的分辨率为448*448，在ImageNet数据集上训练10轮（10 epochs）。这个过程让网络有足够的时间调整filter去适应高分辨率的输入。然后fine tune为检测网络。mAP获得了4%的提升。 

YOLO(v1)使用全连接层数据进行bounding box预测（要把1470*1的全链接层reshape为7*7*30的最终特征），这会丢失较多的空间信息定位不准。

**特征：**
- Dimension 

Clusters（维度聚类）：设想能否一开始就选择了更好的、更有代表性的先验boxes维度，那么网络就应该更容易学到准确的预测位置。解决办法就是统计学习中的K-means聚类方法，通过对数据集中的ground 

true box做聚类，找到ground true box的统计规律。以聚类个数k为anchor 

boxs个数，以k个聚类中心box的宽高维度为anchor box的维度。- Direct location prediction：使用anchor 

boxes的另一个问题是模型不稳定，尤其是在早期迭代的时候。大部分的不稳定现象出现在预测box的（x,y）坐标时。
- Fine-Grained Features（细粒度特征）：修改后的网络最终在13 * 

13的特征图上进行预测，虽然这足以胜任大尺度物体的检测，如果用上细粒度特征的话可能对小尺度的物体检测有帮助。Faser 

R-CNN和SSD都在不同层次的特征图上产生区域建议以获得多尺度的适应性。YOLOv2使用了一种不同的方法，简单添加一个 

passthrough layer，把浅层特征图（分辨率为26 * 26）连接到深层特征图。- Multi-ScaleTraining：原始YOLO网络使用固定的448 * 448的图片作为输入，加入anchor 

boxes后输入变成416 * 

416，由于网络只用到了卷积层和池化层，就可以进行动态调整（检测任意大小图片）。为了让YOLOv2对不同尺寸图片的具有鲁棒性，在训练的时候也考虑了这一点。- 作者提出了一种在分类数据集和检测数据集上联合训练的机制。使用检测数据集的图片去学习检测相关的信息，例如bounding box 

坐标预测，是否包含物体以及属于各个物体的概率。使用仅有类别标签的分类数据集图片去扩展可以检测的种类。 

.
## 6、SSD

来源[知乎机智的大群主](https://www.zhihu.com/question/48979623/answer/113815747)Single Shot MultiBox Detector，即SSD。 
![](https://img-blog.csdn.net/20170224171718026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

它是YOLO的超级改进版，吸取了YOLO的精度下降的教训，同时保留速度快的特点。它能达到58帧每秒，精度有72.1。速度超过Faster R-CNN 有8倍，但达到类似的精度。 

github：[https://github.com/zhreshold/mxnet-ssd](https://github.com/zhreshold/mxnet-ssd)
## 7、DSSD模型

来源：[DSSD: Deconvolutional Single Shot Detector 论文笔记](http://blog.csdn.net/jesse_mx/article/details/55212179?utm_source=itdadao&utm_medium=referral)以及[这里写链接内容](https://mp.weixin.qq.com/s?__biz=MjM5ODU3OTIyOA==&mid=2650667107&idx=2&sn=e1885627643257993250412878e0d8b8&chksm=bec1c51089b64c067ff626d71faf26ff4298f3905657c01f5b93d349d33c0f1ffdb02eb22116&mpshare=1&scene=1&srcid=1017pV4bFSWTjshRqGKFRtPB#rd)

论文地址： [https://arxiv.org/abs/1708.01241](https://arxiv.org/abs/1708.01241)

代码：[https://github.com/szq0214/DSOD](https://github.com/szq0214/DSOD)

模型可视化示例：[http://ethereon.github.io/netscope/#/gist/b17d01f3131e2a60f9057b5d3eb9e04d](http://ethereon.github.io/netscope/#/gist/b17d01f3131e2a60f9057b5d3eb9e04d)

#### DSSD的基础是SSD检测网络

作者对SSD的第一项改进就是换网络，把VGG换成Resnet-101 
![这里写图片描述](https://img-blog.csdn.net/20170215170947594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

方法就是把红色层做反卷积操作，使其和上一级蓝色层尺度相同，再把二者融合在一起，得到的新的红色层用来做预测。如此反复，仍然形成多尺度检测框架。在图中越往后的红色层分辨率越高，而且包含的上下文信息越丰富，综合在一起，使得检测精度得以提升。 

预测模块 

SSD的直接从数个卷积层中分别引出预测函数，预测量多达7000多，梯度计算量也很大。MS-CNN方法指出，改进每个任务的子网可以提高准确性。根据这一思想，作者在每一个预测层后增加残差模块，并且对于多种方案进行了对比，如下图所示。结果表明，增加残差预测模块后，高分辨率图片的检测精度比原始SSD提升明显。
#### 反卷积模块

为了整合浅层特征图和反卷积层的信息，作者引入了如figure 3所示的反卷积模块，该模块可以适合整个DSSD架构（figure1 底部实心圆圈）。作者受到论文Learning to Refine Object Segments的启发，认为用于精细网络的反卷积模块的分解结构达到的精度可以和复杂网络一样，并且更有效率。作者对其进行了一定的修改，如Figure 3所示：其一，在每个卷积层后添加批归一化层；其二，使用基于学习的反卷积层而不是简单地双线性上采样；其三，作者测试了不同的结合方式，元素求和（element-wise sum）与元素点积（element-wise product）方式，实验证明点积计算能得到更好的精度。 
![这里写图片描述](https://img-blog.csdn.net/20170215171442159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmVzc2VfTXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 网络训练

训练技巧大部分和原始SSD类似。首先，依然采用了SSD的default boxes，把重叠率高于0.5的视为正样本。再设置一些负样本，使得正负样本的比例为3:1。训练中使Smooth L1+Softmax联合损失函数最小。训练前依然需要数据扩充 

为此，作者在这里采用K-means聚类方法重新得到了7种default boxes维度，得到的这些boxes维度更具代表性（此处和YOLOv2的聚类做法有点类似

.

## 7、几款框架对比

本节来源于BOT大赛嘉宾问答环节 

SSD 这里的设计就是导致你可以一下子可以检测 8 张图，FasterRCNN 一下子只能检测 1 张图片，这样的话会导致会有 8 张图片的延迟。但是我觉得如果你用 FasterRCNN 的话，你可以优化前面的这个，在实际使用的时候可以把这个网络简化一下。YOLO 做了这样一些设计，在做 3×3×256 的运算之前先有一个 1×1×128 的卷积层，这样的话，它会让你下一层的运算量减少一半。就是说前面一层输出的维数变为一半，下一层再做大卷积和的时候，这样的运算更小了。 

SSD 的输入是 300×300，其实这个图片的大小和处理速度也是有很大的影响。比如说如果你要检测一个很大的物体的话，我们觉得在计算的时候并不需要像 FasterRCNN 那样有成就感。如果你去看卷积的运算过程的话，这个图片大小是和运算量有一个关系的。就是你实际做检测的话，可以尽量的去压缩输入的大小，得到速度的提升，另外一方面就是这种网络结构。最近谷歌有专门讲检测的速度和精度的平衡。 

FasterRCNN 比 SSD 要好一些? 

 SSD 很多训练的策略是非常有效的，因为 FasterRCNN 已经出得非常久了，这个检测效果我觉得可以针对实际场景去看一下这几个框架在做训练的时候采用了一些数据增强的方法，包括一些训练之类的，我觉得这个对于检测效果影响也非常大。像 SSD 最开始其实效果并没有这么好，并没有这么高。但是它通过一些策略，让它训练的精度达到了这个效果。 

. 

.
# 二、2016年的CVPR会议目标检测

本节主要参考：文章《CVPR 2016论文快讯：目标检测领域的新进展》

> 
在目标检测中，以下几个方面非常重要，都有各自的专属paper： 

  （a）识别精度：ION（inside-outside-network）、HyperNet框架   （b）识别效率：YOLO、G-CNN 

  （c）定位准确性：LocNet
识别精度，不得不提目标检测中衡量检测精度的指标mAP(mean average precision)。简单来讲就是在多个类别的检测中，每一个类别都可以根据recall和precision绘制一条曲线，那么AP就是该曲线下的面积，而mAP是多个类别AP的平均值，这个值介于0到1之间，且越大越好。

.

## 1、识别精度方面
- （1） ION（inside-outside-network）

**这个工作的主要贡献有两个：加入周边信息、多层ROI提取和融合**
- 第一个是如何在Fast 

R-CNN的基础之上增加context信息，所谓context在目标检测领域是指感兴趣的ROI周围的信息，可以是局部的，也可以是全局的。为此，作者提出了IRNN的概念，这也就是outside-network。
- 第二个贡献是所谓skip-connection，通过将deep 

ConvNet的多层ROI特征进行提取和融合，利用该特征进行每一个位置的分类和进一步回归，这也就是inside-network。

依靠这两个改进，ION可以在Pascal VOC 2007数据集上边提高大约5个百分点。同时也获得了COCO 2015 detection竞赛的best student entry。

![这里写图片描述](https://img-blog.csdn.net/20170122210506563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- （2）HyperNet框架
- 高层信息：更强的语义信息，对于识别问题较为有效；
- 低层信息：的特征由于分辨率较高，对于目标定位有天然的优势

而检测问题恰恰是识别+定位，因此作者的贡献点在于如何将deep ConvNet的高低层特征进行融合，进而利用融合后的特征进行region proposal提取和进一步目标检测。 

不同于Faster R-CNN，文章的潜在Anchor是用类似于[BING]（Cheng M M, Zhang Z, Lin W Y, et al. BING: Binarized normed gradients for objectness estimation at 300fps. In CVPR 2014）的方法通过扫描窗口的方式生成的，但利用的是CNN的特征，因此取得了更好的性能。

```
通过以上的改进策略，HyperNet可以在产生大约100个region proposal的时候保证较高的recall，同时目标检测的mAP相对于Fast R-CNN也提高了大约6个百分点
```

![这里写图片描述](https://img-blog.csdn.net/20170122210605615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

. 

.2、识别效率方面- 
（1）YOLO

这个工作在识别效率方面的优势很明显，可以做到每秒钟45帧图像，处理视频是完全没有问题的。 

YOLO最大贡献是提出了一种全新的检测框架——直接利用CNN的全局特征预测每个位置可能的目标，相比于R-CNN系列的region proposal+CNN 这种两阶段的处理办法可以大大提高检测速度。今年新出来的[SSD]（Liu W, Anguelov D, Erhan D, et al. SSD: Single Shot MultiBox Detector[J]. arXiv preprint arXiv:1512.02325, 2015. 

）方法虽然在识别率上边有了很大的提升，但YOLO的先驱作用是显而易见的。 
![这里写图片描述](https://img-blog.csdn.net/20170122210715378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)- 
（2）G-CNN

不管是[Fast R-CNN]，还是Faster R-CNN，或者像HyperNet这样的变种，都需要考虑数以万计的潜在框来进行目标位置的搜索，这种方式的一个潜在问题是负样本空间非常大，因此需要一定的策略来进行抑制。

G-CNN从另一个角度来克服这个问题。G-CNN在在初始化的时候不需要那么多框，而是通过对图像进行划分（有交叠），产生少量的框（大约180个），通过一次回归之后得到更接近物体的位置。然后以回归之后的框作为原始窗口，不断进行迭代回归调整，得到最终的检测结果。``经过五次调整之后，G-CNN可以达到跟Fast R-CNN相当的识别性能，但速度是Fast R-CNN的5倍（3fps）。


![这里写图片描述](https://img-blog.csdn.net/20170122210751460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

. 

.3、准确性

LocNet：以上提到的工作都是在做整个目标检测的框架，而LocNet在做另一件事情—如何产生更准确的bounding box?  

 在目标检测的评价体系中，有一个参数叫做IoU，简单来讲就是模型产生的目标窗口和原来标记窗口的交叠率。在Pascal VOC中，这个值为0.5。而2014年以来出现的MS COCO竞赛规则把这个IoU变成了0.5-1.0之间的综合评价值，也就是说，定位越准确，其得分越高，这也侧面反映了目标检测在评价指标方面的不断进步。 
![这里写图片描述](https://img-blog.csdn.net/20170122210845848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    回到这个话题，如何产生更准确的目标位置呢？ 
LocNet的解决方案是：针对每一个给定的初始框进行适当的放大，然后用一个CNN的网络回归出这个放大后的框包含的那个正确框的位置。为了达到这个目标，需要定义回归方式，网络以及模型，具体的细节参见paper（Gidaris S, Komodakis N. LocNet: Improving Localization Accuracy for Object Detection. In CVPR 2016）``经过把原始的框（比如selective search生成的）进行再一次回归之后，再放入Fast R-CNN进行检测，在IoU=0.5的情况下，在Pascal VOC 数据集上mAP可以提升大约5个百分点，而IoU=0.7时可以达到13个百分点的提升，效果还是挺惊人的。


.延伸一：boosting+CNN的结合

来源于《NIPS 2016 – 增量Boosting CNN 用于面部动作单元识别》 

增量Boosting CNN 用于面部动作单元识别.卷积神经网络需要很多参数，尤其是网络结构比较深的情形。为取得较好的效果，需要大量的训练图片，也需要mini-batch 训练策略，即每次迭代只利用一小批的图片。 

Boosting， 比如 AdaBoost，是一种流行的集成学习技巧，这种方法通过结合多个弱分类器来提升效果，而且在面部单元识别中泛化效果较好。 

Boosting可以融入到CNN中，如此一来可以选择每次迭代所选择和激活的具有判别性的神经单元。然而，Boosting CNN (B-CNN)在每个小批上迭代时会由于缺少训练数据而过拟合。而且，上次迭代或上小批中所得信息不能传播，意即，每次迭代都会训练一批新的弱分类器，之前所得弱分类器会被丢弃。 

受增量学习启发，作者提出了一种新的增量Boosting CNN （IB-CNN），这种方法目标在于，每当新训练样本出现时，都会积累B-CNN学习中所得信息。

caffe实现，[](https://cse.sc.edu/~mengz/papers/NIPS2016.pdf)https://cse.sc.edu/~mengz/papers/NIPS2016.pdf

.延伸二：级联分类模型（Cascaded Classification Models）

paper:[](http://ai.stanford.edu/~koller/Papers/Heitz+al:NIPS08a.pdf)http://ai.stanford.edu/~koller/Papers/Heitz+al:NIPS08a.pdf

用来解决如何整体上理解图像问题的。Daphne Koller的学生提出了一种级联分类模型，这种模型可以同时学习一些有关联的模型，并且使得模型之间的效果互相提升。这种方法可以将场景分割，目标检测，多类图像分割以及3D重建等任务结合起来。 

首先单独学习第一层中地每个模型，通过训练得到效果最好的分类器，然后将原始数据和相对低层的模型输出集合结合起来输入到更高一层的模型中。 
![这里写图片描述](https://img-blog.csdn.net/20170219223645102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这种模型也有一些问题，比如，合适的深度如何确定？过深的话会导致过拟合。该模型没有考虑将高层中的分类器反馈给低层中的分类器。通过反馈，也许可以使得相对低层的分类器注意点有所集中，另外，也许可以使得相对底层的模型忽略一些不影响后续分类的误差。 

github：[](https://github.com/anson0910/CNN_face_detection)https://github.com/anson0910/CNN_face_detection

.延伸三：多姿态人脸检测-漏斗型级联结构（Funnel-Structured Cascade，FuSt）

来源于深度学习大讲堂，[](https://zhuanlan.zhihu.com/p/22451474)SeetaFace开源人脸识别引擎介绍，这边只看漏斗形级联结构

该模块基于我们提出的一种结合经典级联结构和多层神经网络的人脸检测方法[1]实现，其所采用的漏斗型级联结构（Funnel-Structured Cascade，FuSt）专门针对多姿态人脸检测而设计，其中引入了由粗到精的设计理念，兼顾了速度和精度的平衡。 

快速LAB级联分类器[2]构成，紧接着是若干个基于SURF特征的多层感知机（MLP）级联结构，最后由一个统一的MLP级联结构（同样基于SURF特征）来处理所有姿态的候选窗口，整体上呈现出上宽下窄的漏斗形状。 

从上往下，各个层次上的分类器及其所采用的特征逐步变得复杂，从而可以保留人脸窗口并排除越来越难与人脸区分的非人脸候选窗口。 
![这里写图片描述](https://img-blog.csdn.net/20170224180641470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

github开源地址，SeetaFace Engine是一个C++面部识别引擎： 
[](https://github.com/seetaface/SeetaFaceEngine)https://github.com/seetaface/SeetaFaceEngine

.延伸四：人脸识别项目

非极大值阈值：[](http://www.thinkface.cn/thread-4203-1-1.html)http://www.thinkface.cn/thread-4203-1-1.html

一键人脸预处理工具，适用于在人脸识别，人脸表情识别小工具： 

FaceTools，[](https://github.com/RiweiChen/FaceTools)https://github.com/RiweiChen/FaceTools

主要实现代码，[](http://blog.csdn.net/chenriwei2/article/details/50321085)来源于【Caffe实践】基于Caffe的人脸检测实现

作者github地址： 
[](https://github.com/RiweiChen/DeepFace/tree/master/FaceDetection/baseline)https://github.com/RiweiChen/DeepFace/tree/master/FaceDetection/baseline

.延伸五：改进RPN Proposal——海康威视

来自于：[](http://mp.weixin.qq.com/s?__biz=MzI1NTE4NTUwOQ==&mid=2650325480&idx=1&sn=e39482fc9e83b89d6e72efc5ee689b92&chksm=f235a6e2c5422ff494d14137ba4a25a14900cc8ebc05e6f9a48cf6cb8dd9d3e95396c0b1da06&mpshare=1&scene=1&srcid=0311VtmSiguZIYmdLAAOtjjK#rd)技术揭秘：海康威视PASCAL VOC2012目标检测权威评测夺冠之道

在Faster R-CNN框架里面，RPN提取Proposal和FRCN对Proposal进行分类其实是2个独立的问题。针对RPN，我们做出了以下2处改进：- 
a)       均衡正负Anchor比例。理想状态下，RPN 正负Anchor的比例是1:1。我们发现，在batch 

size比较大（256）的情况下，这个比例会非常悬殊。特别是目标数量比较少的图像，正的Anchor数量会非常少。这样训练出来的模型会偏向于背景类别，容易漏检。我们对这个比例做了限制，让负样本数量不超过正样本的1.5倍，发现Proposal的召回率可以提高5个点。- 
b)       级联RPN。受CRAFT [4]的启发，我们设计了自己的级联RPN。[4]中先得到标准的RPN 

Proposal，然后用一个分类性能更强的FRCN分支来改进Proposal质量。我们则是用了2个标准的RPN（图 

1）。第一个RPN用滑窗得到的Proposal作为Anchor，第二个RPN用第一个RPN输出的Proposal作为新的Anchor位置。相比[4]，我们的算法优势是实现简单，额外增加的计算量非常少。对于中大目标，可以明显提升Proposal位置的准确度。

![这里写图片描述](https://img-blog.csdn.net/20170430122357104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

全局上下文建模 

    每个Proposal对应原始图像中的一个ROI区域。对这个ROI区域进行分类时， FRCN先把ROI映射到中间特征图上，然后在中间特征图上做裁剪（RoIPooling）。裁剪出来的小特征图输入到CNN分类器中。可以看到，CNN分类只使用了ROI区域内的局部特征。实际上，ROI周围的上下文信息对于判断这个ROI类别是很有帮助的。例如对一个乒乓球分类，很容易和光源混淆。如果知道周围有乒乓球拍、乒乓球台等目标，则更容易判断这是个乒乓球。 

    全局上下文建模是从整幅图像提取特征，然后和每个Proposal的局部特征相融合，用于分类。去年MSRA [2]使用全局上下文，得到了1个点的性能提升。然而他们没有发布具体的实现细节。我们实现的全局上下文网络结构如图 2所示。 
![这里写图片描述](https://img-blog.csdn.net/20170430122409526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们发现，对于图中的全局上下文网络分支，训练时如果采用随机初始化，性能提升非常有限。如果用预训练的参数初始化，在ImageNet DET验证集上可以得到超过3个点的性能提升。对于ResNets，RoIPooling后面的conv5有9层卷积。而Faster R-CNN finetune时一般初始学习速率又会设得比较小（0.001）。这就导致从头训练这9层卷积比较困难。因此，这里预训练显得尤为重要。另外，[2]还把全局上下文特征同时用于分类和Bounding Box回归。我们发现全局上下文特征对于Bounding Box回归没有帮助，只对分类有帮助。 

.延伸六：SPPNet-引入空间金字塔池化改进RCNN

来源于知乎：[](https://zhuanlan.zhihu.com/p/24774302?refer=xiaoleimlnote)https://zhuanlan.zhihu.com/p/24774302?refer=xiaoleimlnote

RCNN一张图图片会有~2k个候选框，每一个都要单独输入CNN做卷积等操作很费时。SPP-net提出：能否在feature map上提取ROI特征，这样就只需要在整幅图像上做一次卷积。 
![这里写图片描述](https://img-blog.csdn.net/20170326114240043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

虽然总体流程还是 Selective Search得到候选区域->CNN提取ROI特征->类别判断->位置精修，但是由于所有ROI的特征直接在feature map上提取，大大减少了卷积操作，提高了效率。

有两个难点要解决：- 
原始图像的ROI如何映射到特征图（一系列卷积层的最后输出）- 
ROI的在特征图上的对应的特征区域的维度不满足全连接层的输入要求怎么办（又不可能像在原始ROI图像上那样进行截取和缩放）？

.延伸七：mask R-cnn检测，分割和特征点定位 同时实现！

来源于新智元文章：[](https://mp.weixin.qq.com/s?__biz=MzA5MDMwMTIyNQ==&mid=2649291175&idx=2&sn=976fcf751458185031cf8551d36db0f7&chksm=8811efe1bf6666f7d6d4aa489816f82c13e76c348404460af4ecd4aee0d5dbbc194221338878&mpshare=1&scene=1&srcid=0325PGmbVQfgWaYESFQcAtG9#rd)mask R-cnn检测，分割和特征点定位全部都做了

Mask R-CNN 的训练简单，仅比 Faster R-CNN 多一点系统开销，运行速度是 5 fps。此外，Mask R-CNN很容易推广到其他任务，例如可以用于在同一个框架中判断人的姿势。我们在 COCO 竞赛的3个任务上都得到最佳结果，包括实例分割，边界框对象检测，以及人物关键点检测。没有使用其他技巧，Mask R-CNN 在每个任务上都优于现有的单一模型，包括优于 COCO 2016 竞赛的获胜模型。我们希望这个简单而有效的方法将成为一个可靠的基准，有助于未来的实例层面识别的研究。我们将会公开相关代码。 

.延伸八：如何用Soft-NMS实现目标检测并提升准确率

传统的物体检测流程常常采用多尺度滑动窗口，根据每个物体类别的前景/背景分数对每个窗口计算其特征。然而，相邻窗口往往具有相关的分数，这会增加检测结果的假阳性。为了避免这样的问题，人们会采用非最大抑制的方法对检测结果进行后续处理来得到最终的检测结果。目前为止，非最大抑制算法仍然是流行的物体检测处理算法并能有效的降低检测结果的假阳性。

在现有的物体检测框架（如图一所示）中，每一个检测框均会产生检测分数，那么对于图片中的一个物体可能对应多个检测分数。在这种情况下，除了最正确（检测分数最高）的一个检测框，其余的检测框均产生假阳性结果。非最大抑制算法针对特定物体类别分别设定重叠阈值来解决这个问题。 
![这里写图片描述](https://img-blog.csdn.net/20170430122424652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们基于重叠部分的大小为相邻检测框设置一个衰减函数而非彻底将其分数置为零。简单来讲，如果一个检测框与M有大部分重叠，它会有很低的分数；而如果检测框与M只有小部分重叠，那么它的原有检测分数不会受太大影响。在标准数据集PASCAL VOC 和 MS-COCO等标准数据集上，Soft-NMS对现有物体检测算法在多个重叠物体检测的平均准确率有显著的提升。同时，Soft-NMS不需要额外的训练且易于实现，因此，它很容易被集成到当前的物体检测流程中。 

.延伸九：手机上的yolo–darkflow

这篇论文旨在利用二值化操作寻找到最优的简化网络，并分别介绍了两种有效的网络：Binary-Weight-Networks 和 XNOR-Networks。Binary-Weight-Networks 是对 CNN 中所有的权重做近似二值化，可以节省 32 倍的存储空间。而且，由于权重被二值化，卷积过程只剩加减算法，不再包括乘法运算，可以提高约两倍的运算速度，这促使 CNN 可以在不牺牲准确率的情况下在小存储设备上使用，包括便携式设备。 

回归主题。我们来看看手机端的yolo——darkflow。 

地址：[](https://github.com/thtrieu/darkflow)https://github.com/thtrieu/darkflow

我仔细看了下代码结构。 

CFG里面是一些配置文件，有些是网络结构和训练网络的超参数，如momentum等。 

Net下面是一些网络结构代码，根据tensorflow写的。包括yolo的两个版本。 

Dark是一些卷积核连接的底层定义，以及如何读取权重的函数。 

Test下面是一些测试图片。 

UTILS里面是一个工具。包括求框的大小及内积什么的。 

你把你想要检测的物体写在lable.txt里，就可以跑demo了。 

.延伸十：目标检测中小目标的特征学习

来源：[](https://mp.weixin.qq.com/s?__biz=MzU0NTAyNTQ1OQ==&mid=2247484853&idx=1&sn=71bb4ad5755f4be42675bd09a9f66743&chksm=fb727969cc05f07f37827a8c3d79cfd18ba5a0365180d9a27d799183d05246e67c3a61ccc3e7&mpshare=1&scene=1&srcid=102234O10r55pTpHhsCewlUp#rd)目标检测也就是这么简单

利用目标的局部特征、全局特征和一些常见的融合特征，并进行有规律的融合，既可以得到鲁棒的特征表达，如下图： 
![这里写图片描述](https://img-blog.csdn.net/20171029111437723?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通过人脸目标的不同特征进行详细学习训练特征表示。 
![这里写图片描述](https://img-blog.csdn.net/20171029111453295?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

想必上面几幅图大家都知道其出处，这种思想很好的利用了目标局部和全局特征，外加丰富的环境特征，从而很容易的就能把目标检测出来。 

最终的结果与下图想必，也有很好的提升，而且在网络过程中还加入了一些巧妙的改进，使其目标检测更加精确。 
![这里写图片描述](https://img-blog.csdn.net/20171029111506676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




















