# SSD算法的改进版之R-SSD - AI之路 - CSDN博客





2017年08月12日 20:57:59[AI之路](https://me.csdn.net/u014380165)阅读数：13511








论文：Enhancement of SSD by concatenating feature maps for object detection 

论文链接：[https://arxiv.org/abs/1705.09587](https://arxiv.org/abs/1705.09587)

**算法详解：**
**SSD算法在object detection领域的效果有目共睹，是proposal-free方面的代表算法之一，如果你对SSD算法不大熟悉，可以看看这篇博客：[SSD（single shot multibox detector）算法及Caffe代码详解](http://blog.csdn.net/u014380165/article/details/72824889)，而本文提出的是对传统SSD算法的改进。虽然深度网络的效果会随着feature map数量的增加而更好，但是这并不代表简单地增加feture map数量就能有更好的效果（文章的实验部分有关于简单增加feature map数量和本文对特征的融合效果对比）。另外作者并未将原本的VGG主网络换成ResNet，而是通过改进特征融合方式提升原有SSD算法的效果，使其充分利用特征。虽然速度比传统的SSD慢一点，但是mAP比传统的SSD高。**

先来看看文章中关于本文算法的效果：For the Pascal VOC 2007 test set trained with VOC 2007 and VOC 2012 training sets, the proposed network with the input size of 300*300 achieved 78.5% mAP (mean average precision) at the speed of 35.0 FPS (frame per second), while the network with a 512*512 sized input achieved 80.8% mAP at 16.6 FPS using Nvidia Titan X GPU.

传统的SSD通过利用不同层特征做object detection，使其对scale variation有较好的鲁棒性，同时兼顾了准确率和速度，**但是传统的SSD有两个小问题：**
**1、因为在SSD中，不同层的feature map都是独立作为分类网络的输入，因此容易出现相同物体被不同大小的框同时检测出来。**
**2、对小尺寸物体的检测效果比较差（大多数object detection算法都有这个问题）。**
因此这篇算法也主要从这两方面来改进传统的SSD算法：一方面利用分类网络增加不同层之间的feature map联系，减少重复框的出现；另一方面增加feature pyramid中feature map的个数，使其可以检测更多的小尺寸物体。Figure5（a）（b）分别表示传统的SSD算法的两个小问题，Figure（c）（d）表示改进后的SSD（**Rainbow SSD或称为R-SSD**）算法针对原有的两个小问题的改进情况。

![这里写图片描述](https://img-blog.csdn.net/20170812205409395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Figure3表示几种不同的特征融合方式。**（a）表示采用pooling的方式进行融合，什么意思呢？看（a）最左边的38*38的feature map，将其做pooling后和右边第二个的19*19的feature map做concate，这样就有两个19*19的feature map了（一个红色，一个橙色）；然后再对这两个19*19的feature map做pooling，再和左边第三个黄色的10*10的feature map做concate……（b）表示采用deconvolution的方式进行特征融合，这次是从最右边的1*1的紫色feature map往左做concate，因为deconvolution是升维，所以从右至左；前面pooling是降维，所以是从左至右。concate方法和前面（a）的pooling类似，不再细讲。**作者认为前两种特征融合方式的缺点在于信息的传递都是单向的，这样分类网络就没法利用其它方向的信息，因此就有了（c）。**（c）表示同时采用pooling和deconvolution进行特征融合，这也是本文rainbow SSD所采用的。应该是同时从左至右（pooling，concate）和从右至左（deconvolution，concate）。（c）中用不同颜色的矩形框表示不同层的feature map，其融合后的结果很像rainbow，可能这就是算法名称Rainbow SSD的由来吧。**一个细节是：在做caoncate之前都会对feature map做一个normalization操作，因为不同层的feature map的scale是不同的，文中的normalization方式采用 batch normalization。**

![这里写图片描述](https://img-blog.csdn.net/20170812205453038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

由于Figure3（c）这种特征融合方式使得融合后每一层的feature map个数都相同（2816），因此可以共用部分参数，具体来讲就是default boxes的参数共享。最后和传统SSD算法的中的default boxes的数量对比如下表Table1：

![这里写图片描述](https://img-blog.csdn.net/20170812205528590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验结果：**

不同算法的实验结果对比可以看Table3，这里还有Table2表示只在原来SSD基础上增加不同层的feature map的channel个数的I-SSD算法。通过Table3的实验可以看出I-SSD虽然效果不错，但是由于增加了feature map的数量会带来计算时间的消耗，所以FPS较低。R-SSD算法的效果和FPS都表现不错。R-FCN虽然效果不错，但是速度上不占优势。

![这里写图片描述](https://img-blog.csdn.net/20170812205631308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Figure4是recall和AP的曲线图：

![这里写图片描述](https://img-blog.csdn.net/20170812205656918?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Table4是AP和mAP的对比：

![这里写图片描述](https://img-blog.csdn.net/20170812205712802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Table5是关于不同scale的object的召回率情况对比：

![这里写图片描述](https://img-blog.csdn.net/20170812205729023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**总结：**
**总的来说，作者通过rainbow concatenation方式（pooling加deconvolution）融合不同层的特征，在增加不同层之间feature map关系的同时也增加了不同层的feature map个数。因此这种融合方式不仅解决了传统SSD算法存在的重复框问题，同时一定程度上解决了small object的检测问题。**




