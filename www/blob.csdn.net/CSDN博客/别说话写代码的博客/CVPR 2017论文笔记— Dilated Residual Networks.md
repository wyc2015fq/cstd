# CVPR 2017论文笔记— Dilated Residual Networks - 别说话写代码的博客 - CSDN博客





2018年12月07日 16:12:59[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：57标签：[Dilated Convolution																[空洞卷积																[Dilated Residual Networks																[空洞残差网络																[分类识别检测分割](https://so.csdn.net/so/search/s.do?q=分类识别检测分割&t=blog)
个人分类：[计算机视觉](https://blog.csdn.net/qq_21997625/article/category/7782165)





## 转自：极市平台微信公众号

## **1.Background**

这次我来介绍一篇深度网络文章《Dilated Residual Networks》，发表在CVPR 2017会议上。作者是普林斯顿大学的Fisher Yu博士等人。网络简称为DRN。文章原文可在作者主页阅览：Fisher Yu主页（http://www.yf.io/）

这篇文章实则是作者将何恺明(Kaiming He)博士残差网络Deep Residual Networks与其之前研究的Dilated Convolution相结合的结果。 前者是发表于CVPR2016上的《**Deep Residual Learning for Image Recognition**》的成果，后者是发表于ICLR2016会议上的《**Multi-Scale Context Aggregation by Dilated Convolutions**》的理论研究。论文链接分别是：
- 
Deep Residual Learning for Image Recognition（https://arxiv.org/pdf/1512.03385.pdf）

- 
Multi-Scale Context Aggregation by Dilated Convolutions（https://arxiv.org/pdf/1511.07122.pdf）


## **2. Introduction**

作者先提出，之前的卷积神经网络（Convolutional Networks）都是通过不断降低图像精度，直到图像被一个仅保留微弱空间信息的特征map表示（一般最后卷积层输出仅为7×7），最后通过计算类别概率来分类图像。这样情况下，尽管前面卷积网络做的很好，但是整个网络却不能获取一个十分精确的结果，例如一个很小的目标对解析图片信息十分重要，但是却被前面卷积网络因为过多降维和其体积很小而直接忽略掉了。

此外，图像分类的深度网络大多数还作为其他需要更多详细场景理解的任务的预训练模型，而很高的**空间分辨率损失**对这些任务而言是非常不利的。

所以卷积神经网络应用在图像分类中，**维护图片空间一定的分辨率**是一个很重要的任务。现有算法有以下的做法： **up-convolutions，skip connections 和 other post-hoc measures。**

**但是上面的方法会造成图片变形，所以本文提出使用Dilated Convolutions方法来解决这个问题。Dilated Convolutions的好处就是既能保持原有网络的感受野（Receptive Field），同时又不会损失图像空间的分辨率（224×224输入的最后卷积层输出特征map是28×28）。**

## **3. Dilated Convolutions**

Dilated Convolutions是为了在我们不使用池化层和striding前提下，能够保证与原深度网络诸如FCN每一层同样的感受野，同时图像输出的特征map尺寸不会变小。 实现过程如下图（摘自论文《Multi-Scale Context Aggregation by Dilated Convolutions》）：

![](https://img-blog.csdnimg.cn/20181207160025578.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

 上面实现过程总结来说就是，假设网络输入为28×28，我们使用padding和stride=1的卷积，卷积filter尺寸都是3×3。 

　　(a) 输入28*28基础上3×3卷积，也就是经过1-dilated处理，感受野为 3×3，该操作和其他正常卷积操作一样，没有区别； 

　　(b) 在(a)输出的基础上进行 3×3卷积，经过2-dilated处理，也就是隔一个像素进行与filter点乘最后相加作为中心像素的特征值，所以感受野变为 7×7； 

　　(c) 在(b)的输出基础上进行 3×3卷积，经过4-dilated处理，也就是隔三个像素进行预filter点乘最后相加作为中心像素的特征值，所以感受野变为 15×15；

**整个感受野的变化如上图绿色区域所示。并且我们看出，由于filter卷积过程stride=1以及依靠padding，最后每层输出的特征map尺寸都依旧保持为28×28，和原图同样的大小。但是我们可以看到，网络无需借助池化层也能增大后续网络的感受野。**



总结来说，Dilated Convolution可以不降低特征map的尺寸而增大卷积感受野。而之前例如FCN使用pooling下采样来增大感受野，但随后又不得不通过Deconvolution或者upsampling来增大特征map尺寸，这样先减后增的操作会让图片特征损失很多信息。

## **4. Dilated Residual Networks**

好了，有了上面Dilated Convolutions的理解，我们对于这种扩张卷积思想（暂且称谓它为扩张卷积）如何应用到残差网络就很清晰了。实际上，我要指出，扩张卷积可以应用到任何CNN上，从而达到保护网络的空间分辨率。至于作者为什么应用和撰写将其扩展到残差网络，自然是因为残差网络是当前表现最好的网络之一。 



原始的残差网络卷积层是经过了5次striding处理达到降采样的目的，如下图，每一种颜色对应同一尺度大小的特征map输出。（图片摘自文章《Deep Residual Learning for Image Recognition》，仅显示网络部分结构）

![](https://img-blog.csdnimg.cn/201812071603280.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

输出的特征也就是发生了5次尺度的变化，所以结果对于224×224分辨率输入，最后卷积层输出为7×7。每一层卷积忽略bias不写的算法表达式如下：

![](https://img-blog.csdnimg.cn/20181207160356465.png)

但是最终特征map输出仅为7×7，最直接增加特征map分辨率的方法就是直接移除层间的subsampling（striding）操作，但是这样操作不能达到原网络每一层同一感受野，显然为了高分辨率而降低感受野是非常不可取的行为。这里就体现了扩张卷积的优点了，就是保持原有网络的感受野并且还能提高图片空间分辨率。 这里由于前面三种类型层（g1,g2,g3）对原始输入进行了总共8倍的降采样，28×28分辨率足够识别图片场景的结构，并且保存了大多数的图片信息去帮助我们正确解析原始图像的像素级信息。 所以本文是在前三层网络输出28×28特征map之后改变原始网络结构（即改变g4,g5）。前后变化如下图：

![](https://img-blog.csdnimg.cn/20181207160425328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

首先作者移除这两层中striding操作，然后对g4,g5分别进行2-dilated和4-dilated convolutions操作，如下：

![](https://img-blog.csdnimg.cn/20181207160453230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

原始ResNet中两层网络分辨率分别相对于g3输出降了2倍和4倍，也就是从28×28降到了14×14和7×7。但是经过扩张卷积操作特征map的尺度没有发生任何改变，并且感受野和原ResNet对应层一样。最终DRN的g5输出为28×28。 

## **5. 图像分类和目标标定**

图像分类和目标标定在经过上面卷积操作得到特征map之后，分别进行下面的最后处理：

![](https://img-blog.csdnimg.cn/20181207160542449.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

对于图像分类，操作和ResNet中一样，进行global average pooling，然后再经过1×1×n卷积得到最终n类得分，从而判断图片中目标的分类。

而对于Localization预测，作者直接对g5（28×28）输出进行1×1×n的卷积来替代global average pooling操作。这样每一类包含一张对应激活的预测map。对于map中每个像素标签，表示这个像素属于该类目标的概率。

DRNs时为了图像分类而设计的网络，同时能够输出高分辨率的激活maps。实验表明，DRNs在图像分类上比原始ResNet表现要出色。此外，DRNs可以直接由输入而生成高分辨率的输出maps，其中无需对其增加任何其他层，无需多余参数和再训练模型从而很好的预测目标位置。

## **6. Degridding**

![](https://img-blog.csdnimg.cn/20181207160754901.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

但是从上图第三列我们会发现，最终输出maps会产生很差的网格状态（gridding artifacts）。作者主要通过Removing max pooling和Adding layers等操作一定程度上改善了最终输出结果。最终三种类型网络格局如下：

![](https://img-blog.csdnimg.cn/2018120716072697.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

## **7. Experiments　**

图像分类实验结果如下表，效果比原始ResNet要好。

![](https://img-blog.csdnimg.cn/20181207160832256.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

目标位置预测实验结果如下图，效果比原始ResNet也要好。

![](https://img-blog.csdnimg.cn/20181207160854468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)](https://so.csdn.net/so/search/s.do?q=空洞残差网络&t=blog)](https://so.csdn.net/so/search/s.do?q=Dilated Residual Networks&t=blog)](https://so.csdn.net/so/search/s.do?q=空洞卷积&t=blog)](https://so.csdn.net/so/search/s.do?q=Dilated Convolution&t=blog)




