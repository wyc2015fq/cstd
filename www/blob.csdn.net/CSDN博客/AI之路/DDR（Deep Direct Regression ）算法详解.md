# DDR（Deep Direct Regression ）算法详解 - AI之路 - CSDN博客





2017年05月25日 19:42:30[AI之路](https://me.csdn.net/u014380165)阅读数：1732








论文：Deep Direct regression for multi-oriented scene text detection 

论文链接：[https://arxiv.org/pdf/1703.08289.pdf](https://arxiv.org/pdf/1703.08289.pdf)

## **要解决的问题：**

传统的object detection算法比如Faster-RCNN，SSD和YOLO可以应用在horizontal scene texts detection上，而且检测效果也不错，但是难以应用在**multi-oriected scene text detection**中。其原因主要是因为texts的大小，方向，横纵比等差异较大，本文采用**直接回归**的方式可以避免将proposal应用在multi-oriented texts带来的困难。

**本文算法的结果在ICDAR2015数据集上的F-measure达到0.81**。 
**不过目前还没有公开代码**

## **算法概要:**

输入图像大小320*320，经过卷积和降采样的堆叠，另外中间有三次feature融合的过程，类似Resnet的做法，每次融合完都要做反卷积，最后网络得到128*80*80的输出。这个输出一方面作为分类模块的输入，分类模块将会得到输出map，map上每个点代表score，map大小是（1*80*80）；另一方面作为回归模块的输入，回归模块将会得到map，map上每个点代表回归出来的四边形框的四个点坐标的offset（8*80*80），可以简单理解为4个点坐标。最后用一个改进版本的NMS：Recalled NMS做精修，主要解决两个text相离很近时如何既能去除横跨这两个text的错误框，同时保留正确框。本文的一个特点是对ground truth的选取和传统做法不一样，采用和text center line距离在一定范围之内的像素点作为positive，围绕positive有一个过渡区域作为not care区域，再以外才是negative。另外对positive的短边尺寸还做了限制。本文的另一个特点就是直接回归生成预测结果，没有中间的proposal，这是和其它算法很大的不同。

## **算法详解：**

作者在这篇文章给出了两个定义：Indirect Regression和Direct Regression。可以从fig1中看出差别。fig1.a是Indirect Regression，类似Faster-RCNN，SSD等算法都是这种方式，通过回归proposal和ground truth的offset来得到预测结果，黄色虚线表示回归过程。而fig1.b是Direct Regression，就是本文提出来的，从一个点直接回归ground truth。

![这里写图片描述](https://img-blog.csdn.net/20170525193240993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

作者讲了**Indirect regression不适合应用在multi-oriented text detection**的三个理由：1、没有robust的方式生成word-level或line-level的proposal，很多方式只能产生character-level的proposal。2、faster-RCNN的anchor机制并不是生成proposal的有效方式。如图fig2的虚线表示的是6个不同大小和长宽比的anchor，anchor是首次在faster-RCNN中提出的概念，旨在生成region proposal。可以看出这些anchors和文字的重叠区域很小，因为文字是倾斜的。3、anchor机制会降低检测系统的效率。以一行文字为例，一般而言一行文字的横纵比都比较大，有些算法为了检测出这样的区域，生成了很多anchor或者multi-oriented anchors以得到有可能有效的proposal，这显然增加了计算成本。

![这里写图片描述](https://img-blog.csdn.net/20170525193305369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

作者提到多数的（scene text detection）场景文本检测都是将text当做characters的集合，因此一般都是先定位character或components candidates，然后组合成word或text line。但是这种方法在文字模糊或分辨率不高的时候效果很差。也有一些方法是将text detection 转化成object detection，但说到底还是Indirect regression。还有算法（DMPN）尝试生成不同角度的anchor来尽可能覆盖非水平的文字，但是文本的形状是任意的，而这种anchor还是传统的平行四边形。

因此，有了本文提出的算法。下图是本文算法的结构，主要分四个部分，如图示，可以参考这篇博客的最后一张图：网络结构图。就可以明白这里的前三个部分其实是可以合并在一起的。

![这里写图片描述](https://img-blog.csdn.net/20170525193415746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

稍微介绍下下面这个公式。这个其实就是坐标的一个转换，我们说到Regression的输出是Mloc，Mloc是80*80*8的大小，这里的80是输入图像320的1/4，这也是下面式子中w前面4的含义，因为你从Regression映射到原图需要扩大4倍。另外Mloc中的每个点都用L(w,h,c)表示。

![这里写图片描述](https://img-blog.csdn.net/20170525193427105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

关于**loss function**，主要包含分类和回归两个部分。

![这里写图片描述](https://img-blog.csdn.net/20170525193446201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

采用hinge loss，具体形式如下，sign（x）是符号函数，当y*和y^相等的时候（比如都是1或都是0）那么平方就是0，即没有损失；如果二者不相等（一个是0，一个是1），那么平方结果是1。比较好理解。

![这里写图片描述](https://img-blog.csdn.net/20170525193521732?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在**分类**任务中，对于Ground Truth（正样本）的选择，作者并不是将text region里面的所有像素点都作为正样本的像素点，而是将和text center line距离小于一定范围的像素点作为正样本的像素点，这个距离定义为text boundaries的短边的r倍，作者r取0.2。另外作者对正样本的短边范围作了限制，即当短边范围在[32*2^-1,32*2^1]范围内才有可能是positive sample，如果短边范围在[32*2^-1.5，32*2^-1] || [32*2^1，32*2^1.5]那么就是NOT CARE，其他作为negative。这样做的好处在最后总结处会提到。然后在正样本像素点周围还会围绕一圈NOT CARE的区域作为正样本到负样本的过渡，可以参考Fig5（a）。NOT CARE不参与训练过程。作者认为这种ground truth设计可以使得 text 和 non-text 之间的分界更加清晰。

![这里写图片描述](https://img-blog.csdn.net/20170525193544467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**回归**部分： 

一个细节：添加了scale&shift，原来的z是sigmoid函数的输出，范围是0到1，经过这样的操作后范围变成-400到400。

![这里写图片描述](https://img-blog.csdn.net/20170525193600232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面这个式子是回归的loss函数。z*表示ground truth，z^表示predict。这个式子和Fast RCNN里面的回归loss公式很像。smoothL1函数是一个开口朝上的类似二次函数的曲线，所以当z*和z^越相近的时候，值越小。当然这里选择L1 loss 而不是L2 loss的原因也是和Fast RCNN一样，因为L1 loss对离群点不敏感。

![这里写图片描述](https://img-blog.csdn.net/20170525193632264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Recalled NMS**： 

在multi-task（classification 和 regression）之后，output map（80*80）的每个点都对应一个四边形框。classification的output map（1*80*80，之所以是1可以参看下面网络结构中classification中的卷积层的filter数目）保留的是每个四边形框的score，regerssion的output map（8*80*80，之所以是8，可以参看下面网络结构中localization中前两个卷积层的filter数目）保留的是每个四边形框的四个点坐标的offset。为了滤除一些非text区域，作者只保留在classification以后分数比较高的点。但即便这样，仍然还有一些密集的重叠四边形，这时就需要采用Recalled NMS来去除。 

传统的NMS算法可以参考[YOLO算法的Caffe实现](http://blog.csdn.net/u014380165/article/details/72553074)那篇博客，这里不再叙述，也不难。 
**Recalled NMS主要分三步**：第一步就是Fig6中最左边一列，是用传统的NMS算法得到的结果，这个结果的主要问题在于当两个text距离很近的时候容易有一些横跨两个text的框，这样的框是错误的。第二步就是从第一步得到的框对应到未经过NMS时候与该框的IOU最大且大于某个阈值的那个框。有点拗口，原文描述如下：Second, each quadrilateral in Bsup is switched to the one with highest score in B beyond a given overlap.这里Bsup是NMS之后剩下的框，B是NMS之前的框。第三步就是把第二步得到的框做合并，为什么这个时候可以这么做呢？因为这个时候的框又变得重叠率比较高，而第一步也就是NMS之后重叠率比较高的框都已被剔除了。
![这里写图片描述](https://img-blog.csdn.net/20170525194114594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

综上，下面这个图是网络的详细结构图：

![这里写图片描述](https://img-blog.csdn.net/20170525194124766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **总结：**

**作者认为自己的算法之所以能够大幅度提升precision和recall主要在于三个方面：1、直接回归的object detection方式对于multi-oriented scene的text detection更有效。2、对于正样本的text size的限制。因为如果text size太小，那么容易随着降采样的进行而消失；如果text size太大，那么网络更多地是学到类似笔画等信息，而不是context information。3、end-to-end的优化方式更加robust。**




